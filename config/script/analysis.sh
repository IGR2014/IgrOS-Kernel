#!/usr/bin/env bash
################################################################
#
#	Run every analysis tool over kernel builds and host tests, one HTML report
#
#	For each <compiler>:<arch> (default: all four) configures a debug build with
#	strict warnings and UBSan (trap mode), builds and installs it, boots it in
#	QEMU (a CPU exception, e.g. UBSan trap, is resolved to a source line) and
#	runs compiler warnings, ClangTidy, Cppcheck and CodeQL report targets.
#	Then host unit tests with coverage, then the report.
#
#	Usage: analysis.sh [<clang++|g++>:<i386|x86_64> ...]
#	Result: build/analysis/index.html, short summary.md, finding counts in counts.json
#	(configs in build/analysis/<compiler>-<arch>)
#
#	Environment:
#	  IGROS_ANALYSIS_DIR	- output directory inside sources (default: build/analysis)
#	  IGROS_ANALYSIS_CODEQL	- CodeQL for "first" config (default), "all" or "none"
#				  (separate clean build, a few minutes per config)
#
#	Tools that aren't installed are skipped (analysis Docker image has all of them).
#
################################################################

set -u

cd "$(dirname "$0")/../.."
SOURCE_DIR="$(pwd)"
OUT="${IGROS_ANALYSIS_DIR:-${SOURCE_DIR}/build/analysis}"
CODEQL="${IGROS_ANALYSIS_CODEQL:-first}"
PYTHON="$(command -v python3 || command -v python)"

if [[ "$#" -gt 0 ]]; then
	CONFIGS=("$@")
else
	CONFIGS=("clang++:x86_64" "clang++:i386" "g++:x86_64" "g++:i386")
fi

# Run step with output in log file, record its exit code in reports/steps.txt
# Usage: step <reports dir> <name> <command...>
step() {
	local reports="$1" name="$2"
	shift 2
	echo "  ${name}"
	"$@" > "${reports}/${name}.log" 2>&1
	local code=$?
	echo "${name} ${code}" >> "${reports}/steps.txt"
	[[ "${code}" -eq 0 ]] || echo "    failed (exit code ${code}): ${reports}/${name}.log"
	return "${code}"
}

# CPU exception that stopped the boot (e.g. UBSan ud2 before IDT is loaded):
# boot again with interrupt logging, resolve first exception address to source
# Usage: boot_trap <arch> <build dir>
boot_trap() {
	local arch="$1" build="$2"
	local log
	log="$(mktemp)"
	timeout 20 "qemu-system-${arch}" -m 128M -cdrom "${build}/install/kernel.iso" -boot d \
		-display none -serial null -monitor none -no-reboot -d int -D "${log}" > /dev/null 2>&1
	# First CPU exception: vector below 0x20, not a software interrupt (i=0)
	local line
	line="$(grep -m 1 -E ' v=[01][0-9a-f] e=[0-9a-f]+ i=0 ' "${log}")"
	rm -f "${log}"
	[[ -n "${line}" ]] || return 0
	local vector="${line#* v=}"
	vector="${vector%% *}"
	local ip="${line#*IP=}"
	ip="${ip%% *}"
	ip="${ip#*:}"
	local name
	case "${vector}" in
		00)	name="#DE divide error"	;;
		06)	name="#UD invalid opcode: UBSan check failed (ud2 trap)"	;;
		08)	name="#DF double fault"	;;
		0d)	name="#GP general protection"	;;
		0e)	name="#PF page fault"	;;
		*)	name="vector 0x${vector}"	;;
	esac
	# Innermost (inlined) function first
	local location
	mapfile -t location < <(addr2line -f -C -i -e "${build}/kernel.bin" "0x${ip}")
	local source="${location[1]%% *}"
	echo "${source:-??:0}:1: error: CPU exception ${name} at 0x${ip} in ${location[0]:-??}, before the kernel could report it [cpu-exception-${vector}]" \
		> "${build}/reports/boot-trap.txt"
}

tests="${OUT}/tests"

# Report of what is done so far: an open page updates itself after every config
# Usage: report [status]
report() {
	local status="${1:-}"
	"${PYTHON}" config/script/analysis.py report \
		--source-dir "${SOURCE_DIR}" \
		--output "${OUT}/index.html" \
		--markdown "${OUT}/summary.md" \
		--counts "${OUT}/counts.json" \
		--tests "${tests}" \
		${status:+--status "${status}"} \
		"${BUILDS[@]}"
}

# Config names, previous results removed (pending configs show as not run)
BUILDS=()
for config in "${CONFIGS[@]}"; do
	case "${config%%:*}" in
		clang++)	name="clang-${config##*:}"	;;
		g++)		name="gcc-${config##*:}"	;;
		*)		echo "Unknown compiler: ${config%%:*}" >&2; exit 1	;;
	esac
	BUILDS+=("${name}=${OUT}/${name}")
	rm -rf "${OUT}/${name}/reports"
	mkdir -p "${OUT}/${name}/reports"
done
rm -rf "${tests}/reports"
mkdir -p "${tests}/reports"
report "running: ${BUILDS[0]%%=*} (1/${#BUILDS[@]})" > /dev/null

for index in "${!CONFIGS[@]}"; do
	config="${CONFIGS[${index}]}"
	compiler="${config%%:*}"
	arch="${config##*:}"
	name="${BUILDS[${index}]%%=*}"
	build="${OUT}/${name}"
	reports="${build}/reports"
	echo "== ${name}"

	step "${reports}" configure cmake --preset "config-linux-${compiler}-${arch}-debug" -B "${build}" \
		-DCMAKE_INSTALL_PREFIX="${build}/install" \
		-DIGROS_CXX_STRICT_WARNINGS=ON \
		-DIGROS_KERNEL_UBSAN=ON \
		|| continue

	if step "${reports}" build cmake --build "${build}" --parallel \
		&& step "${reports}" install cmake --install "${build}"; then
		step "${reports}" boot bash tests/boot/qemu-boot.sh "${arch}" "${build}/install/kernel.iso" 30 \
			|| boot_trap "${arch}" "${build}"
	fi

	# Report targets exist only for installed tools
	targets="$(cmake --build "${build}" --target help 2> /dev/null)"
	for target in compiler-warnings clang-tidy cppcheck; do
		if grep -q "^${target}:" <<< "${targets}"; then
			step "${reports}" "${target}" cmake --build "${build}" --target "${target}"
		fi
	done
	if grep -q "^codeql:" <<< "${targets}" \
		&& { [[ "${CODEQL}" == "all" ]] || { [[ "${CODEQL}" == "first" ]] && [[ "${index}" -eq 0 ]]; }; }; then
		step "${reports}" codeql cmake --build "${build}" --target codeql
	fi

	next=$((index + 1))
	if [[ "${next}" -lt "${#BUILDS[@]}" ]]; then
		report "running: ${BUILDS[${next}]%%=*} (${next}/${#BUILDS[@]} configs done)" > /dev/null
	else
		report "running: host tests (${#BUILDS[@]}/${#BUILDS[@]} configs done)" > /dev/null
	fi
done

# Host unit tests with coverage (GCC, like SonarCloud job)
echo "== host tests"
step "${tests}/reports" configure cmake -S tests -B "${tests}" -G Ninja -DCMAKE_CXX_COMPILER=g++ -DIGROS_TESTS_COVERAGE=ON \
	&& step "${tests}/reports" build cmake --build "${tests}" --parallel \
	&& step "${tests}/reports" coverage cmake --build "${tests}" --target coverage

echo "== report"
report
