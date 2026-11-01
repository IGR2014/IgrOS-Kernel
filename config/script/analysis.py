#!/usr/bin/env python3
################################################################
#
#	Kernel analysis helper (Python 3.8+, standard library only)
#
#	warnings	- recompile C++ sources of compile_commands.json into a temporary
#			  directory and save compiler diagnostics (build log isn't kept)
#	report		- merge reports of one or more build directories (compiler warnings,
#			  ClangTidy, Cppcheck, CodeQL, boot test) and host tests coverage
#			  into one self-contained HTML page; data is also written to
#			  <page>.data.js, which an open page reloads to update itself
#
#	Usage:
#	  analysis.py warnings --build-dir <dir> --output <file>
#	  analysis.py report --source-dir <dir> --output <file.html> [--status <text>]
#	                     [--markdown <summary.md>] [--tests <tests build dir>] [<name>=]<build dir>...
#
#	Build directory reports (<build dir>/reports, every file optional):
#	  compiler-warnings.txt, clang-tidy-report.txt, cppcheck-report.txt,
#	  codeql-results.sarif, boot.log, boot-trap.txt, steps.txt ("<step> <exit code>")
#
################################################################

import argparse
import datetime
import json
import os
import re
import shlex
import subprocess
import sys
import tempfile
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path


# GCC/Clang style diagnostic (compiler, ClangTidy, Cppcheck --template=gcc, boot trap)
DIAG = re.compile(
	r"^(?P<file>[^\s:][^:]*):(?P<line>\d+):(?:(?P<col>\d+):)?\s"
	r"(?P<kind>fatal error|error|warning|note|style|performance|portability|information):\s"
	r"(?P<msg>.*?)(?:\s\[(?P<rule>[^\]\s]+)\])?$"
)
# ANSI colors (IGROS_CXX_COLORED_OUTPUT)
ANSI = re.compile(r"\x1b\[[0-9;]*[mK]")
# Markdown links of SARIF messages: [text](id)
SARIF_LINK = re.compile(r"\[([^\]]+)\]\(\d+\)")

SEVERITY = {
	"fatal error":		"error",
	"error":		"error",
	"warning":		"warning",
	"style":		"info",
	"performance":		"info",
	"portability":		"info",
	"information":		"info",
	"recommendation":	"info",
	"note":			"info",
	"none":			"info"
}
SEVERITY_RANK = {"error": 3, "warning": 2, "info": 1}

# Tool: report file in <build dir>/reports
TOOLS = {
	"compiler":	"compiler-warnings.txt",
	"clang-tidy":	"clang-tidy-report.txt",
	"cppcheck":	"cppcheck-report.txt",
	"codeql":	"codeql-results.sarif",
	"boot":		"boot.log"
}

# Lines of source around a finding
SNIPPET_CONTEXT = 3
# Errors listed in Markdown summary
MARKDOWN_ERRORS = 10
# Step/tool that didn't run, empty Markdown cell
NOT_RUN = "not run"
NO_VALUE = "—"


# ---------------------------------------------------------------- warnings

def collect_warnings(build_dir, output):
	database = json.loads((build_dir / "compile_commands.json").read_text(encoding="utf-8"))
	# Only C++: assembly sources are in compile database too
	entries = [entry for entry in database if entry["file"].endswith(".cpp")]

	def compile_source(entry):
		args = entry.get("arguments") or shlex.split(entry["command"])
		# Own object file and no dependency files: build directory stays untouched
		command = []
		skip = False
		for arg in args:
			if skip:
				skip = False
			elif arg in ("-o", "-MF", "-MT", "-MQ"):
				skip = True
			elif arg not in ("-MD", "-MMD"):
				command.append(arg)
		with tempfile.TemporaryDirectory() as temp:
			result = subprocess.run(
				command + ["-o", os.path.join(temp, "source.o")],
				cwd=entry["directory"],
				capture_output=True,
				text=True,
				encoding="utf-8",
				errors="replace"
			)
		return entry["file"], result.returncode, ANSI.sub("", result.stderr)

	with ThreadPoolExecutor(max_workers=os.cpu_count() or 2) as pool:
		results = sorted(pool.map(compile_source, entries))

	failed = [source for source, code, _ in results if code != 0]
	output.parent.mkdir(parents=True, exist_ok=True)
	with output.open("w", encoding="utf-8") as report:
		for source, code, diagnostics in results:
			report.write(diagnostics)
			if code != 0:
				report.write(f"{source}:1:1: error: compilation failed (exit code {code}) [compile-failed]\n")
	print(f"Compiler warnings: {output} ({len(results)} sources, {len(failed)} failed)")
	return 0


# ---------------------------------------------------------------- report

class Findings:

	def __init__(self, source_dir):
		self.source_dir = source_dir.resolve()
		self.items = {}

	# Source relative path (findings outside sources keep absolute path)
	def path(self, file, base):
		if not file or file in ("??", "nofile"):
			return "(unknown)"
		absolute = Path(os.path.normpath(Path(base) / file))
		try:
			return absolute.relative_to(self.source_dir).as_posix()
		except ValueError:
			return absolute.as_posix()

	# Same rule on the same line is one finding for every tool/config:
	# ClangTidy repeats compiler diagnostics, headers are reported per source
	@staticmethod
	def rule_key(rule):
		if not rule:
			return ""
		rule = rule.split(",")[-1]
		if rule.startswith("clang-diagnostic-") and rule != "clang-diagnostic-error":
			return "-W" + rule[len("clang-diagnostic-"):]
		return rule

	def add(self, tool, config, file, line, severity, message, rule="", notes=(), description=""):
		rule = self.rule_key(rule)
		key = (file, line, rule or message)
		item = self.items.get(key)
		if item is None:
			item = self.items[key] = {
				"file":		file,
				"line":		line,
				"severity":	severity,
				"rule":		rule,
				"description":	description,
				"messages":	[],
				"notes":	[],
				"tools":	[],
				"configs":	[]
			}
		if SEVERITY_RANK[severity] > SEVERITY_RANK[item["severity"]]:
			item["severity"] = severity
		for field, value in (("messages", message), ("tools", tool), ("configs", config)):
			if value not in item[field]:
				item[field].append(value)
		for note in notes:
			if note not in item["notes"]:
				item["notes"].append(note)

	# GCC/Clang style report: notes belong to the diagnostic above them
	def parse_diagnostics(self, report, tool, config, base):
		pending = None

		def flush():
			if pending:
				self.add(tool, config, **pending)

		for raw in report.read_text(encoding="utf-8", errors="replace").splitlines():
			match = DIAG.match(ANSI.sub("", raw).rstrip())
			if not match:
				continue
			file = self.path(match["file"], base)
			line = int(match["line"])
			if match["kind"] == "note":
				if pending:
					pending["notes"].append({"file": file, "line": line, "message": match["msg"]})
				continue
			flush()
			pending = {
				"file":		file,
				"line":		line,
				"severity":	SEVERITY[match["kind"]],
				"message":	match["msg"],
				"rule":		match["rule"] or "",
				"notes":	[]
			}
		flush()

	def parse_sarif(self, report, tool, config):
		sarif = json.loads(report.read_text(encoding="utf-8"))
		for run in sarif.get("runs", []):
			# Query packs keep their rules in tool extensions
			rules = {}
			components = [run["tool"]["driver"]] + run["tool"].get("extensions", [])
			for component in components:
				for rule in component.get("rules", []):
					rules[rule["id"]] = rule
			for result in run.get("results", []):
				rule_id = result.get("ruleId") or result.get("rule", {}).get("id", "")
				rule = rules.get(rule_id, {})
				properties = rule.get("properties", {})
				level = (
					properties.get("problem.severity")
					or result.get("level")
					or rule.get("defaultConfiguration", {}).get("level", "warning")
				)
				locations = result.get("locations") or [{}]
				file, line = self.sarif_location(locations[0])
				notes = []
				for related in result.get("relatedLocations", []):
					note_file, note_line = self.sarif_location(related)
					notes.append({
						"file":		note_file,
						"line":		note_line,
						"message":	SARIF_LINK.sub(r"\1", related.get("message", {}).get("text", ""))
					})
				self.add(
					tool,
					config,
					file,
					line,
					SEVERITY.get(level, "warning"),
					SARIF_LINK.sub(r"\1", result.get("message", {}).get("text", "")),
					rule_id,
					notes,
					rule.get("shortDescription", {}).get("text", "")
				)

	def sarif_location(self, location):
		physical = location.get("physicalLocation", {})
		uri = physical.get("artifactLocation", {}).get("uri", "")
		uri = re.sub(r"^file://", "", uri)
		return self.path(uri, self.source_dir), physical.get("region", {}).get("startLine", 0)


# "<step> <exit code>" lines written by analysis.sh
def read_steps(reports):
	steps = {}
	path = reports / "steps.txt"
	if path.exists():
		for line in path.read_text(encoding="utf-8").splitlines():
			name, _, code = line.partition(" ")
			if code.strip().lstrip("-").isdigit():
				steps[name] = int(code)
	return steps


def read_cache(build_dir):
	cache = {}
	path = build_dir / "CMakeCache.txt"
	if path.exists():
		for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
			match = re.match(r"^(\w+):[A-Z]+=(.*)$", line)
			if match:
				cache[match[1]] = match[2]
	return cache


def git_revision(source_dir):
	try:
		revision = subprocess.run(
			["git", "-C", str(source_dir), "rev-parse", "--short", "HEAD"],
			capture_output=True, text=True, check=True
		).stdout.strip()
		dirty = subprocess.run(
			["git", "-C", str(source_dir), "status", "--porcelain", "--untracked-files=no"],
			capture_output=True, text=True, check=True
		).stdout.strip()
		return revision + (" (modified)" if dirty else "")
	except (OSError, subprocess.CalledProcessError):
		return ""


def config_summary(name, build_dir, findings):
	reports = build_dir / "reports"
	steps = read_steps(reports)
	cache = read_cache(build_dir)
	options = [
		option.replace("IGROS_", "").replace("CXX_", "").replace("KERNEL_", "").lower()
		for option in ("IGROS_CXX_STRICT_WARNINGS", "IGROS_KERNEL_UBSAN", "IGROS_CXX_CLANG_TIDY")
		if cache.get(option, "OFF").upper() in ("ON", "TRUE", "1")
	]
	tools = {}
	for tool, file in TOOLS.items():
		present = (reports / file).exists()
		code = steps.get(tool)
		tools[tool] = {
			"ran":		present or code is not None,
			"exit":		code,
			"count":	sum(1 for item in findings if tool in item["tools"] and name in item["configs"])
		}
	# Boot test: verdict and failure reasons from qemu-boot.sh output
	boot = {"status": NOT_RUN, "reasons": []}
	boot_log = reports / "boot.log"
	if boot_log.exists():
		text = boot_log.read_text(encoding="utf-8", errors="replace")
		boot["status"] = "passed" if "BOOT TEST PASSED" in text else "failed"
		boot["reasons"] = sorted(set(re.findall(r"^FAILED: (.*)$", text, re.M)))
	return {
		"name":		name,
		"compiler":	Path(cache.get("CMAKE_CXX_COMPILER", "")).name,
		"type":		cache.get("CMAKE_BUILD_TYPE", ""),
		"options":	options,
		"steps":	steps,
		"tools":	tools,
		"boot":		boot
	}


def tests_summary(tests_dir, report_dir):
	if not tests_dir:
		return None
	reports = tests_dir / "reports"
	summary = {"steps": read_steps(reports), "ctest": "", "coverage": None, "link": ""}
	log = reports / "coverage.log"
	if log.exists():
		match = re.search(r"^\d+% tests passed.*$", log.read_text(encoding="utf-8", errors="replace"), re.M)
		summary["ctest"] = match[0] if match else ""
	coverage = reports / "coverage" / "summary.json"
	if coverage.exists():
		data = json.loads(coverage.read_text(encoding="utf-8"))
		summary["coverage"] = {
			kind: {
				"covered":	data.get(f"{kind}_covered", 0),
				"total":	data.get(f"{kind}_total", 0),
				"percent":	data.get(f"{kind}_percent", 0)
			}
			for kind in ("line", "function", "branch")
		}
		summary["files"] = {
			file["filename"]: file.get("line_percent", 0)
			for file in data.get("files", [])
		}
		summary["link"] = os.path.relpath(reports / "coverage" / "index.html", report_dir).replace(os.sep, "/")
	return summary


def snippets(findings, source_dir):
	wanted = {}
	for item in findings:
		locations = [(item["file"], item["line"])] + [(note["file"], note["line"]) for note in item["notes"]]
		for file, line in locations:
			if line > 0:
				wanted.setdefault(file, set()).update(range(max(1, line - SNIPPET_CONTEXT), line + SNIPPET_CONTEXT + 1))
	sources = {}
	for file, lines in wanted.items():
		path = Path(file) if Path(file).is_absolute() else source_dir / file
		try:
			text = path.read_text(encoding="utf-8", errors="replace").splitlines()
		except OSError:
			continue
		sources[file] = {number: text[number - 1].expandtabs(8) for number in sorted(lines) if number <= len(text)}
	return sources


# Markdown summary: counts, boot, coverage and tests in one line
def markdown_headline(data):
	findings = data["findings"]
	counts = {severity: sum(1 for item in findings if item["severity"] == severity) for severity in SEVERITY_RANK}
	booted = [config for config in data["configs"] if config["boot"]["status"] == "passed"]
	tried = [config for config in data["configs"] if config["boot"]["status"] != NOT_RUN]
	headline = [f"**{counts['error']} errors · {counts['warning']} warnings · {counts['info']} info**"]
	if tried:
		headline.append(f"{len(booted)}/{len(tried)} configs boot")
	tests = data["tests"] or {}
	if tests.get("coverage"):
		headline.append(f"line coverage {tests['coverage']['line']['percent']:.1f}%")
	if tests.get("ctest"):
		headline.append(tests["ctest"])
	return " · ".join(headline)


# Markdown summary: build, boot and finding counts per config
def markdown_configs(data):
	tools = [tool for tool in data["tools"] if tool != "boot"]
	lines = [
		"| Config | Build | Boot | " + " | ".join(tools) + " |",
		"|---|---|---|" + "---:|" * len(tools)
	]
	for config in data["configs"]:
		code = config["steps"].get("build")
		build = NO_VALUE
		if code is not None:
			build = f"❌ exit {code}" if code else "✅"
		boot = {"passed": "✅", "failed": "❌ " + "; ".join(config["boot"]["reasons"]), NOT_RUN: NO_VALUE}[config["boot"]["status"]]
		cells = [str(config["tools"][tool]["count"]) if config["tools"][tool]["ran"] else NO_VALUE for tool in tools]
		lines.append(f"| {config['name']} | {build} | {boot} | " + " | ".join(cells) + " |")
	return lines


# Short summary for CI (job summary, pull request comment)
def write_markdown(data, path):
	lines = [markdown_headline(data), ""]
	if data["status"]:
		lines += [f"_{data['status']}_", ""]
	lines += markdown_configs(data)
	errors = [item for item in data["findings"] if item["severity"] == "error"]
	if errors:
		lines += ["", "**Errors**", ""]
		lines += [
			f"- `{item['file']}:{item['line']}` {item['messages'][0]} ({', '.join(item['tools'])}: {', '.join(item['configs'])})"
			for item in errors[:MARKDOWN_ERRORS]
		]
		if len(errors) > MARKDOWN_ERRORS:
			lines.append(f"- … and {len(errors) - MARKDOWN_ERRORS} more")
	path.parent.mkdir(parents=True, exist_ok=True)
	path.write_text("\n".join(lines) + "\n", encoding="utf-8")


# Open page reloads the data file: never let it see a half-written one
def write_atomic(path, text):
	temp = path.with_name(path.name + ".tmp")
	temp.write_text(text, encoding="utf-8")
	os.replace(temp, path)


# Findings of one build directory reports: "[name=]directory" -> (name, directory)
def parse_build(findings, spec, source_dir):
	name, separator, directory = spec.rpartition("=")
	build_dir = Path(directory).resolve()
	if not separator:
		cache = read_cache(build_dir)
		name = f"{cache.get('CMAKE_CXX_COMPILER_ID', '').lower() or 'kernel'}-{build_dir.name}"
	reports = build_dir / "reports"
	# Paths in compiler/ClangTidy output are relative to build directory, Cppcheck runs in sources
	for tool, base in (("compiler", build_dir), ("clang-tidy", build_dir), ("cppcheck", source_dir)):
		report = reports / TOOLS[tool]
		if report.exists():
			findings.parse_diagnostics(report, tool, name, base)
	trap = reports / "boot-trap.txt"
	if trap.exists():
		findings.parse_diagnostics(trap, "boot", name, build_dir)
	sarif = reports / TOOLS["codeql"]
	if sarif.exists():
		findings.parse_sarif(sarif, "codeql", name)
	return name, build_dir


def make_report(args):
	source_dir = Path(args.source_dir).resolve()
	output = Path(args.output).resolve()
	findings = Findings(source_dir)
	configs = [parse_build(findings, spec, source_dir) for spec in args.builds]

	items = sorted(findings.items.values(), key=lambda item: (item["file"], item["line"], -SEVERITY_RANK[item["severity"]]))
	data = {
		"id":		str(time.time_ns()),
		"title":	"IgrOS-Kernel analysis",
		"status":	args.status or "",
		"dataFile":	output.stem + ".data.js",
		"generated":	datetime.datetime.now().strftime("%Y-%m-%d %H:%M"),
		"revision":	git_revision(source_dir),
		"configs":	[config_summary(name, build_dir, items) for name, build_dir in configs],
		"tools":	list(TOOLS),
		"tests":	tests_summary(Path(args.tests).resolve() if args.tests else None, output.parent),
		"findings":	items,
		"sources":	snippets(items, source_dir)
	}
	payload = json.dumps(data, separators=(",", ":")).replace("</", "<\\/")
	output.parent.mkdir(parents=True, exist_ok=True)
	write_atomic(output, PAGE.replace("%%DATA%%", payload))
	write_atomic(output.with_name(data["dataFile"]), f"window.igrosAnalysis && window.igrosAnalysis({payload});\n")
	if args.markdown:
		write_markdown(data, Path(args.markdown))
	counts = {severity: sum(1 for item in items if item["severity"] == severity) for severity in SEVERITY_RANK}
	print(f"Analysis report: {output} ({counts['error']} errors, {counts['warning']} warnings, {counts['info']} info)")
	return 0


# ---------------------------------------------------------------- page

PAGE = r"""<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>IgrOS-Kernel analysis</title>
<style>
:root {
	color-scheme: light dark;
	--bg: #f6f7f9; --card: #ffffff; --fg: #1d2330; --muted: #5d6676; --border: #dde1e7; --hover: #f0f2f5;
	--code-bg: #f3f4f7; --mark: #fff4c2;
	--error: #c62828; --error-bg: #fdecec; --warning: #a35c00; --warning-bg: #fff3e0;
	--info: #2f5fb3; --info-bg: #e9f0fc; --ok: #2e7d32; --ok-bg: #e8f5e9;
	--mono: ui-monospace, "Cascadia Mono", "JetBrains Mono", Menlo, Consolas, monospace;
}
@media (prefers-color-scheme: dark) {
	:root {
		--bg: #12151b; --card: #1a1e26; --fg: #e3e7ee; --muted: #98a1b0; --border: #2c323d; --hover: #222833;
		--code-bg: #151920; --mark: #3b3418;
		--error: #ff8a80; --error-bg: #3a1d1d; --warning: #ffb74d; --warning-bg: #3a2c16;
		--info: #8ab4f8; --info-bg: #1c2a40; --ok: #81c784; --ok-bg: #1b3020;
	}
}
* { box-sizing: border-box; }
body { margin: 0; background: var(--bg); color: var(--fg); font: 14px/1.45 system-ui, -apple-system, "Segoe UI", sans-serif; }
main { max-width: 1200px; margin: 0 auto; padding: 24px 16px 64px; }
h1 { font-size: 22px; margin: 0 0 4px; }
h2 { font-size: 16px; margin: 28px 0 10px; }
.meta { color: var(--muted); display: flex; flex-wrap: wrap; gap: 6px 10px; align-items: center; }
.live { display: inline-flex; gap: 5px; align-items: center; }
.live::before { content: ""; width: 7px; height: 7px; border-radius: 50%; background: var(--ok); }
.live.lost::before { background: var(--muted); }
@keyframes flash { from { background: var(--mark); } to { background: transparent; } }
.updated { animation: flash 1.5s ease-out; }
code, .mono { font-family: var(--mono); font-size: 12.5px; }
a { color: var(--info); }
.tiles { display: grid; grid-template-columns: repeat(auto-fit, minmax(150px, 1fr)); gap: 10px; margin-top: 18px; }
.tile { background: var(--card); border: 1px solid var(--border); border-radius: 8px; padding: 12px 14px; cursor: default; }
.tile[data-severity] { cursor: pointer; }
.tile .value { font-size: 26px; font-weight: 650; }
.tile .label { color: var(--muted); font-size: 12.5px; }
.tile.error .value { color: var(--error); } .tile.warning .value { color: var(--warning); }
.tile.info .value { color: var(--info); } .tile.ok .value { color: var(--ok); }
.card { background: var(--card); border: 1px solid var(--border); border-radius: 8px; }
.scroll { overflow-x: auto; }
table { border-collapse: collapse; width: 100%; }
th, td { text-align: left; padding: 7px 10px; border-bottom: 1px solid var(--border); white-space: nowrap; }
th { color: var(--muted); font-weight: 600; font-size: 12.5px; }
tr:last-child td { border-bottom: none; }
td.num { text-align: right; font-variant-numeric: tabular-nums; }
td.count { cursor: pointer; } td.count:hover { background: var(--hover); }
.pill { display: inline-block; padding: 1px 7px; border-radius: 10px; font-size: 12px; font-weight: 600; }
.pill.error, .pill.failed { color: var(--error); background: var(--error-bg); }
.pill.warning { color: var(--warning); background: var(--warning-bg); }
.pill.info { color: var(--info); background: var(--info-bg); }
.pill.passed, .pill.ok { color: var(--ok); background: var(--ok-bg); }
.pill.none { color: var(--muted); background: var(--hover); }
.tag { display: inline-block; padding: 0 6px; margin: 0 4px 2px 0; border: 1px solid var(--border); border-radius: 4px; font-size: 11.5px; color: var(--muted); }
.coverage { display: grid; grid-template-columns: repeat(auto-fit, minmax(220px, 1fr)); gap: 14px; padding: 14px; }
.bar { height: 6px; border-radius: 3px; background: var(--hover); overflow: hidden; margin-top: 4px; }
.bar > div { height: 100%; background: var(--ok); }
.filters { position: sticky; top: 0; z-index: 2; display: flex; flex-wrap: wrap; gap: 8px; align-items: center; padding: 10px 0; background: var(--bg); }
.filters label { display: inline-flex; gap: 4px; align-items: center; }
.filters select, .filters input[type=search] { font: inherit; color: var(--fg); background: var(--card); border: 1px solid var(--border); border-radius: 6px; padding: 4px 8px; }
.filters input[type=search] { flex: 1 1 200px; min-width: 0; }
.filters button { font: inherit; color: var(--fg); background: var(--card); border: 1px solid var(--border); border-radius: 6px; padding: 4px 10px; cursor: pointer; }
.filters .shown { color: var(--muted); margin-left: auto; }
details.file { margin-bottom: 10px; }
details.file > summary { list-style: none; cursor: pointer; padding: 9px 12px; display: flex; gap: 10px; align-items: center; flex-wrap: wrap; }
details.file > summary::-webkit-details-marker { display: none; }
details.file > summary::before { content: "\25B8"; color: var(--muted); }
details.file[open] > summary::before { content: "\25BE"; }
details.file > summary .path { font-family: var(--mono); font-weight: 600; word-break: break-all; }
details.file > summary .right { margin-left: auto; display: flex; gap: 6px; align-items: center; }
.finding { border-top: 1px solid var(--border); }
.finding > .head { display: grid; grid-template-columns: 56px 70px 1fr; gap: 10px; padding: 8px 12px; cursor: pointer; }
.finding > .head:hover { background: var(--hover); }
.finding .line { font-family: var(--mono); color: var(--muted); }
.finding .message { word-break: break-word; }
.finding .variants { color: var(--muted); font-size: 12.5px; }
.finding .rule { font-family: var(--mono); font-size: 12px; color: var(--muted); }
.finding .body { display: none; padding: 0 12px 12px 148px; }
.finding.open .body { display: block; }
pre.snippet { margin: 6px 0; padding: 6px 0; background: var(--code-bg); border-radius: 6px; overflow-x: auto; font-family: var(--mono); font-size: 12.5px; }
pre.snippet span { display: block; padding: 0 10px; white-space: pre; }
pre.snippet span.hit { background: var(--mark); }
pre.snippet i { font-style: normal; color: var(--muted); display: inline-block; min-width: 4ch; margin-right: 10px; text-align: right; user-select: none; }
.note { color: var(--muted); margin-top: 8px; }
.empty { padding: 24px; text-align: center; color: var(--muted); }
@media (max-width: 700px) {
	.finding > .head { grid-template-columns: 48px 1fr; }
	.finding > .head .sev { display: none; }
	.finding .body { padding-left: 12px; }
}
</style>
</head>
<body>
<main>
	<h1 id="title"></h1>
	<div class="meta" id="meta"></div>
	<div class="tiles" id="tiles"></div>
	<h2>Configurations</h2>
	<div class="card scroll"><table id="configs"></table></div>
	<div id="tests"></div>
	<h2>Findings</h2>
	<div class="filters">
		<label><input type="checkbox" data-severity="error" checked> errors</label>
		<label><input type="checkbox" data-severity="warning" checked> warnings</label>
		<label><input type="checkbox" data-severity="info" checked> info</label>
		<select id="tool"><option value="">all tools</option></select>
		<select id="config"><option value="">all configs</option></select>
		<input type="search" id="search" placeholder="Filter by file, rule or message">
		<button type="button" id="expand-all">Expand all</button>
		<span class="shown" id="shown"></span>
	</div>
	<div id="findings"></div>
</main>
<script type="application/json" id="data">%%DATA%%</script>
<script>
"use strict";
let D = JSON.parse(document.getElementById("data").textContent);
const RANK = { error: 3, warning: 2, info: 1 };
const $ = (id) => document.getElementById(id);
const esc = (text) => String(text).replace(/[&<>"]/g, (c) => ({ "&": "&amp;", "<": "&lt;", ">": "&gt;", '"': "&quot;" }[c]));
const state = { severity: new Set(["error", "warning", "info"]), tool: "", config: "", search: "", open: new Set(), all: location.hash === "#expand" };

// Live update: report generation rewrites <page>.data.js; a file:// page can't fetch it,
// but can load it as a script, which calls igrosAnalysis() with the new data
const live = { seen: Date.now() };
window.igrosAnalysis = (data) => {
	live.seen = Date.now();
	if (data.id !== D.id) {
		D = data;
		refresh();
		document.querySelector("main").classList.remove("updated");
		void document.querySelector("main").offsetWidth;
		document.querySelector("main").classList.add("updated");
	}
	header();
};
function poll() {
	const script = document.createElement("script");
	script.src = D.dataFile + "?" + Date.now();
	script.onload = script.onerror = () => { script.remove(); header(); };
	document.head.appendChild(script);
}

function header() {
	document.title = (D.status ? "⏳ " : "") + D.title;
	$("title").textContent = D.title;
	const lost = Date.now() - live.seen > 10000;
	$("meta").innerHTML = [
		"Generated " + esc(D.generated),
		D.revision && "revision " + esc(D.revision),
		D.status && `<span class="pill warning">${esc(D.status)}</span>`,
		`<span class="live${lost ? " lost" : ""}" title="${lost ? "Data file not found: showing last loaded results" : "Page updates when the report is regenerated"}">${lost ? "not updating" : "live"}</span>`
	].filter(Boolean).join(" · ");
}

// Summary tiles
function tiles() {
	const count = (severity) => D.findings.filter((f) => f.severity === severity).length;
	const booted = D.configs.filter((c) => c.boot.status === "passed").length;
	const bootRan = D.configs.filter((c) => c.boot.status !== "not run").length;
	const list = [
		{ cls: "error", value: count("error"), label: "errors", severity: "error" },
		{ cls: "warning", value: count("warning"), label: "warnings", severity: "warning" },
		{ cls: "info", value: count("info"), label: "info / style", severity: "info" }
	];
	if (bootRan) list.push({ cls: booted === bootRan ? "ok" : "error", value: booted + "/" + bootRan, label: "configs boot" });
	const coverage = D.tests && D.tests.coverage;
	if (coverage) list.push({ cls: "ok", value: coverage.line.percent.toFixed(1) + "%", label: "line coverage (host tests)" });
	$("tiles").innerHTML = list.map((t) =>
		`<div class="tile ${t.cls}" ${t.severity ? `data-severity="${t.severity}" title="Show only ${t.label}"` : ""}>` +
		`<div class="value">${esc(t.value)}</div><div class="label">${esc(t.label)}</div></div>`).join("");
	$("tiles").querySelectorAll("[data-severity]").forEach((tile) => tile.addEventListener("click", () => {
		state.severity = new Set([tile.dataset.severity]);
		document.querySelectorAll(".filters [data-severity]").forEach((box) => { box.checked = box.dataset.severity === tile.dataset.severity; });
		render();
		$("findings").scrollIntoView({ behavior: "smooth" });
	}));
}

// Step status cell
function status(code, ran) {
	if (code === undefined || code === null) return ran ? `<span class="pill ok">done</span>` : `<span class="pill none">—</span>`;
	return code === 0 ? `<span class="pill ok">ok</span>` : `<span class="pill failed">exit ${code}</span>`;
}

function configs() {
	const tools = D.tools.filter((tool) => tool !== "boot");
	let html = "<tr><th>Config</th><th>Compiler</th><th>Options</th><th>Build</th><th>Boot</th>" +
		tools.map((tool) => `<th class="num">${esc(tool)}</th>`).join("") + "</tr>";
	for (const c of D.configs) {
		const boot = c.boot.status === "not run" ? `<span class="pill none">—</span>`
			: `<span class="pill ${c.boot.status}" title="${esc(c.boot.reasons.join("; "))}">${c.boot.status}</span>`;
		html += `<tr><td><b>${esc(c.name)}</b></td><td class="mono">${esc(c.compiler)} ${esc(c.type)}</td>` +
			`<td>${c.options.map((o) => `<span class="tag">${esc(o)}</span>`).join("") || "—"}</td>` +
			`<td>${status(c.steps.build, c.steps.configure !== undefined)}</td><td>${boot}` +
			(c.boot.status === "failed" && c.tools.boot.count ? ` <span class="count" data-tool="boot" data-config="${esc(c.name)}" style="cursor:pointer">→ trap</span>` : "") + "</td>" +
			tools.map((tool) => {
				const t = c.tools[tool];
				if (!t.ran) return `<td class="num"><span class="pill none">not run</span></td>`;
				const failed = t.exit ? ` <span class="pill failed" title="tool exit code">exit ${t.exit}</span>` : "";
				return `<td class="num count" data-tool="${tool}" data-config="${esc(c.name)}" title="Show ${esc(tool)} findings of ${esc(c.name)}">${t.count}${failed}</td>`;
			}).join("") + "</tr>";
	}
	$("configs").innerHTML = html;
	document.querySelectorAll("[data-tool][data-config]").forEach((cell) => cell.addEventListener("click", () => {
		state.tool = $("tool").value = cell.dataset.tool;
		state.config = $("config").value = cell.dataset.config;
		render();
		$("findings").scrollIntoView({ behavior: "smooth" });
	}));
}

function tests() {
	const t = D.tests;
	if (!t) return;
	let html = `<h2>Host unit tests</h2><div class="card">`;
	if (t.coverage) {
		html += `<div class="coverage">` + ["line", "function", "branch"].map((kind) => {
			const k = t.coverage[kind];
			return `<div><b>${{ line: "lines", function: "functions", branch: "branches" }[kind]}</b> ${k.percent.toFixed(1)}% <span class="meta">(${k.covered} / ${k.total})</span>` +
				`<div class="bar"><div style="width:${k.percent}%"></div></div></div>`;
		}).join("") + `</div>`;
	}
	const parts = [];
	if (t.ctest) parts.push(esc(t.ctest));
	if (t.steps.coverage !== undefined && t.steps.coverage !== 0) parts.push(`<span class="pill failed">coverage target exit ${t.steps.coverage}</span>`);
	if (t.link) parts.push(`<a href="${esc(t.link)}">Detailed coverage report</a>`);
	if (t.files) {
		const low = Object.entries(t.files).filter(([, p]) => p < 90).sort((a, b) => a[1] - b[1]).slice(0, 6);
		if (low.length) parts.push("Lowest: " + low.map(([f, p]) => `<code>${esc(f)}</code> ${p.toFixed(0)}%`).join(", "));
	}
	html += `<div style="padding:0 14px 14px">${parts.join(" · ")}</div></div>`;
	$("tests").innerHTML = html;
}

// Filters (options follow the data, selection is kept)
function options() {
	const fill = (select, label, values) => {
		select.innerHTML = `<option value="">${label}</option>` + values.map((v) => `<option value="${esc(v)}">${esc(v)}</option>`).join("");
		select.value = values.includes(state[select.id]) ? state[select.id] : (state[select.id] = "");
	};
	fill($("tool"), "all tools", D.tools);
	fill($("config"), "all configs", D.configs.map((c) => c.name));
}
document.querySelectorAll(".filters [data-severity]").forEach((box) => box.addEventListener("change", () => {
	box.checked ? state.severity.add(box.dataset.severity) : state.severity.delete(box.dataset.severity);
	render();
}));
$("tool").addEventListener("change", (e) => { state.tool = e.target.value; render(); });
$("config").addEventListener("change", (e) => { state.config = e.target.value; render(); });
$("search").addEventListener("input", (e) => { state.search = e.target.value.toLowerCase(); render(); });

function visible(f) {
	if (!state.severity.has(f.severity)) return false;
	if (state.tool && !f.tools.includes(state.tool)) return false;
	if (state.config && !f.configs.includes(state.config)) return false;
	if (state.search) {
		const text = [f.file, f.rule, f.description, ...f.messages].join(" ").toLowerCase();
		if (!text.includes(state.search)) return false;
	}
	return true;
}

function snippet(file, line) {
	const lines = D.sources[file];
	if (!lines || !lines[line]) return "";
	const numbers = Object.keys(lines).map(Number).filter((n) => Math.abs(n - line) <= 3).sort((a, b) => a - b);
	return `<pre class="snippet">` + numbers.map((n) =>
		`<span${n === line ? ` class="hit"` : ""}><i>${n}</i>${esc(lines[n])}</span>`).join("") + `</pre>`;
}

function finding(f, index) {
	const id = f.file + ":" + f.line + ":" + f.rule + ":" + f.messages[0];
	const variants = f.messages.length > 1 ? `<div class="variants">${f.messages.slice(1).map(esc).join("<br>")}</div>` : "";
	const allConfigs = f.configs.length === D.configs.length && D.configs.length > 1;
	const notes = f.notes.map((n) =>
		`<div class="note"><code>${esc(n.file)}:${n.line}</code> ${esc(n.message)}</div>` + (n.file === f.file && Math.abs(n.line - f.line) <= 3 ? "" : snippet(n.file, n.line))).join("");
	return `<div class="finding${state.all !== state.open.has(id) ? " open" : ""}" data-id="${esc(id)}">` +
		`<div class="head"><span class="line">L${f.line}</span><span class="sev"><span class="pill ${f.severity}">${f.severity}</span></span>` +
		`<div><div class="message">${esc(f.messages[0])}</div>${variants}<div>` +
		(f.rule ? `<span class="rule" title="${esc(f.description)}">${esc(f.rule)}</span> ` : "") +
		f.tools.map((t) => `<span class="tag">${esc(t)}</span>`).join("") +
		(allConfigs ? `<span class="tag">all configs</span>` : f.configs.map((c) => `<span class="tag">${esc(c)}</span>`).join("")) +
		`</div></div></div><div class="body">${f.description ? `<div class="note">${esc(f.description)}</div>` : ""}` +
		`${snippet(f.file, f.line) || `<div class="note">No source available</div>`}${notes}</div></div>`;
}

function render() {
	const shown = D.findings.filter(visible);
	$("shown").textContent = `${shown.length} of ${D.findings.length} findings`;
	if (!shown.length) {
		$("findings").innerHTML = `<div class="card empty">${D.findings.length ? "No findings match the filters" : "No findings — everything is clean"}</div>`;
		return;
	}
	const files = new Map();
	for (const f of shown) {
		if (!files.has(f.file)) files.set(f.file, []);
		files.get(f.file).push(f);
	}
	// Worst files first
	const order = [...files.entries()].sort((a, b) => {
		const worst = (list) => Math.max(...list.map((f) => RANK[f.severity]));
		return worst(b[1]) - worst(a[1]) || b[1].length - a[1].length || a[0].localeCompare(b[0]);
	});
	const coverage = (D.tests && D.tests.files) || {};
	$("findings").innerHTML = order.map(([file, list]) => {
		const counts = ["error", "warning", "info"].map((s) => [s, list.filter((f) => f.severity === s).length]).filter(([, n]) => n);
		const cov = coverage[file] !== undefined ? `<span class="tag" title="Line coverage by host tests">cov ${coverage[file].toFixed(0)}%</span>` : "";
		return `<details class="file card" open><summary><span class="path">${esc(file)}</span><span class="right">${cov}` +
			counts.map(([s, n]) => `<span class="pill ${s}">${n}</span>`).join("") + `</span></summary>` +
			list.map(finding).join("") + `</details>`;
	}).join("");
	document.querySelectorAll(".finding > .head").forEach((head) => head.addEventListener("click", () => {
		const node = head.parentElement;
		node.classList.toggle("open");
		state.open.has(node.dataset.id) ? state.open.delete(node.dataset.id) : state.open.add(node.dataset.id);
	}));
	$("expand-all").textContent = state.all ? "Collapse all" : "Expand all";
}
$("expand-all").addEventListener("click", () => { state.all = !state.all; state.open.clear(); render(); });

function refresh() {
	header();
	tiles();
	configs();
	tests();
	options();
	render();
}

refresh();
setInterval(poll, 3000);
</script>
</body>
</html>
"""


# ---------------------------------------------------------------- main

def main():
	parser = argparse.ArgumentParser(description="IgrOS-Kernel analysis helper")
	commands = parser.add_subparsers(dest="command", required=True)

	warnings = commands.add_parser("warnings", help="Collect compiler warnings from compile_commands.json")
	warnings.add_argument("--build-dir", required=True)
	warnings.add_argument("--output", required=True)

	report = commands.add_parser("report", help="Merge analysis reports into an HTML page")
	report.add_argument("--source-dir", required=True)
	report.add_argument("--output", required=True)
	report.add_argument("--tests", help="Host tests build directory (coverage)")
	report.add_argument("--status", help="Analysis state shown in the page (e.g. still running)")
	report.add_argument("--markdown", help="Also write a short Markdown summary (CI job summary, PR comment)")
	report.add_argument("builds", nargs="+", metavar="[name=]build-dir")

	args = parser.parse_args()
	if args.command == "warnings":
		return collect_warnings(Path(args.build_dir), Path(args.output))
	return make_report(args)


if __name__ == "__main__":
	sys.exit(main())
