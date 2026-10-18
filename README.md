# IgrOS-Kernel

[![Badge-CPP]][Link-CPP]
[![Badge-License]][Link-License]

**IgrOS-Kernel** is a hobby operating system kernel written in modern **C++20** with a thin layer of assembly.
It boots via **Multiboot** (GRUB) on **i386** and **x86_64**, sets up the CPU (GDT, IDT, exceptions, interrupts, paging),
manages physical memory and drives basic PC hardware: VGA text mode, serial port, PIT timer, keyboard and CMOS clock.

[![Badge-Arch-i386]][Link-Arch-i386]
[![Badge-Arch-x86_64]][Link-Arch-x86_64]
[![Badge-Arch-arm]][Link-Arch-arm]
[![Badge-Arch-arm64]][Link-Arch-arm64]
[![Badge-Arch-avr]][Link-Arch-avr]

---

## Status

| | |
| :--- | :--- |
| **CI/CD**      | [![Badge-GitHub]][Link-GitHub] [![Badge-GitLab]][Link-GitLab] [![Badge-Azure-DevOps]][Link-Azure-DevOps] |
| **Codacy**     | [![Badge-Codacy-Quality]][Link-Codacy] [![Badge-Codacy-Coverage]][Link-Codacy-Coverage] |
| **SonarCloud** | [![Badge-Sonar-Quality-Gate]][Link-Sonar] [![Badge-Sonar-Coverage]][Link-Sonar] [![Badge-Sonar-Bugs]][Link-Sonar] [![Badge-Sonar-Vulnerabilities]][Link-Sonar] [![Badge-Sonar-Code-Smells]][Link-Sonar] |

---

## Features

| Area           | Implemented                                                        | Planned                          |
| :---           | :---                                                               | :---                             |
| **Boot**       | Multiboot (GRUB), protected mode (i386), long mode switch (x86_64) | Multiboot 2, custom bootloader   |
| **CPU**        | GDT, IDT, exceptions, hardware interrupts (8259 PIC)               | APIC                             |
| **Memory**     | Physical page allocator, paging                                    | Virtual memory allocator         |
| **Drivers**    | VGA text mode, serial port (UART), keyboard (read), CMOS RTC (read), PIT timer (off by default) | Loadable kernel drivers |
| **User space** |                                                                    | User mode, user programs, GUI    |

---

## Quick start

### Requirements

Linux host with:

- CMake 3.21+ and Ninja
- Clang + LLD, or GCC with multilib
- GRUB tools: `grub-mkrescue` (`grub-common`, `grub-pc-bin`), `xorriso`, `mtools`
- QEMU (`qemu-system-x86`) to run; BOCHS optionally

On Ubuntu:

```sh
sudo apt install ccache clang cmake g++-multilib gcc-multilib grub-common grub-pc-bin lld mtools ninja-build qemu-system-x86 xorriso
```

### Build, run and test

Build, run and test scripts take `<i386|x86_64> [clang++|g++] [debug|release]` (defaults: `clang++ debug`).

```sh
# Build bootable ISO
config/script/build.sh x86_64
# Run in QEMU (serial output in terminal)
config/script/run-qemu.sh x86_64
# Run in BOCHS
config/script/run-bochs.sh x86_64
# Host unit tests + QEMU boot test
config/script/test.sh x86_64
# Remove build artifacts
config/script/clean.sh
```

The ISO is placed at `install/<Debug|Release>/config-linux-<compiler>-<arch>-<type>/kernel.iso`.

You can also build with CMake presets directly:

```sh
cmake --preset config-linux-clang++-x86_64-debug
cmake --build --preset build-linux-clang++-x86_64-debug --target install
```

### Docker

The image contains only the toolchain; sources are mounted from the host, so code changes don't require rebuilding it:

```sh
docker compose up kernel_x86_64_clang_build   # also: kernel_i386_clang_build, kernel_{i386,x86_64}_gcc_build
```

---

## Testing

- **Unit tests** – host-compiled tests of kernel library, memory, interrupts, paging, drivers and multiboot code for both architectures' code paths, in [`tests/`](./tests). Assembly functions (port I/O, control registers, CPU halt) are replaced with fake hardware ([`tests/hw.hpp`](./tests/hw.hpp)):
  ```sh
  cmake -S tests -B build/tests -G Ninja -DIGROS_TEST_ARCH=x86_64 [-DIGROS_TESTS_COVERAGE=ON]
  cmake --build build/tests
  ctest --test-dir build/tests --output-on-failure
  ```
- **Boot test** – [`tests/boot/qemu-boot.sh`](./tests/boot/qemu-boot.sh) boots the ISO in QEMU and checks that the kernel reaches the end of `kmain` and handles a keyboard IRQ.

GitHub Actions and GitLab CI run both for every compiler/architecture combination; Azure Pipelines runs the boot test.
Coverage is reported to Codacy and SonarCloud.

---

## Project layout

| Directory                        | Contents                                                     |
| :---                             | :---                                                         |
| [`arch/`](./arch)                | CPU support: common x86 code and `i386` / `x86_64` boot code, GDT, IDT |
| [`drivers/`](./drivers)          | VGA, serial (UART), PIT, CMOS RTC, keyboard                  |
| [`klib/`](./klib)                | Freestanding kernel library: memory, strings, printing, math |
| [`mem/`](./mem)                  | Physical memory allocator                                    |
| [`multiboot/`](./multiboot)      | Multiboot information parsing                                |
| [`platform/`](./platform)        | Platform initialization                                      |
| [`dev/`](./dev), [`sys/`](./sys) | Device abstraction, ELF definitions                          |
| [`config/`](./config)            | Build/run scripts, linker scripts, CMake toolchain configs   |
| [`tests/`](./tests)              | Unit and boot tests                                          |
| [`kmain.cpp`](./kmain.cpp)       | Kernel entry point                                           |

---

## Contributing

See [CONTRIBUTING.md](./CONTRIBUTING.md) and the [Code of Conduct](./CODE_OF_CONDUCT.md).

The repository is mirrored on [GitHub][Link-Repo-GitHub], [GitLab][Link-Repo-GitLab] and [Azure DevOps][Link-Repo-Azure].

## License

[MIT](./LICENSE) © 2017 - 2026 Igor Baklykov


[Badge-Arch-arm]: https://img.shields.io/static/v1?label=Arch&message=arm%20(planned)&color=orangered&style=for-the-badge
[Badge-Arch-arm64]: https://img.shields.io/static/v1?label=Arch&message=arm64%20(planned)&color=orangered&style=for-the-badge
[Badge-Arch-avr]: https://img.shields.io/static/v1?label=Arch&message=avr%20(planned)&color=orangered&style=for-the-badge
[Badge-Arch-i386]: https://img.shields.io/static/v1?label=Arch&message=i386&color=lime&style=for-the-badge
[Badge-Arch-x86_64]: https://img.shields.io/static/v1?label=Arch&message=x86_64&color=lime&style=for-the-badge
[Badge-Azure-DevOps]: https://img.shields.io/azure-devops/build/prigoryan/1dea483f-6c7a-4efb-8631-5309fd3370ec/3/master?label=Azure%20Pipelines&style=for-the-badge&logo=data:image/svg%2Bxml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHZpZXdCb3g9IjAgMCAyNCAyNCI+PHBhdGggZmlsbD0iI2ZmZiIgZD0iTTAgOC44NzdMMi4yNDcgNS45MWw4LjQwNS0zLjQxNlYuMDIybDcuMzcgNS4zOTNMMi45NjYgOC4zMzh2OC4yMjVMMCAxNS43MDd6bTI0LTQuNDV2MTQuNjUxbC01Ljc1MyA0LjktOS4zMDMtMy4wNTd2My4wNTZsLTUuOTc4LTcuNDE2IDE1LjA1NyAxLjc5OFY1LjQxNXoiLz48L3N2Zz4K
[Badge-Codacy-Coverage]: https://img.shields.io/codacy/coverage/42842492126743259fdcb9501dfd0eee/master?label=Coverage&style=for-the-badge&logo=Codacy
[Badge-Codacy-Quality]: https://img.shields.io/codacy/grade/42842492126743259fdcb9501dfd0eee/master?label=Code%20Quality&style=for-the-badge&logo=Codacy
[Badge-CPP]: https://img.shields.io/static/v1?label=Standard&message=C%2B%2B20&color=blue&style=for-the-badge&logo=C%2B%2B
[Badge-GitHub]: https://img.shields.io/github/actions/workflow/status/IGR2014/IgrOS-Kernel/github-ci.yml?branch=master&label=GitHub%20Actions&style=for-the-badge&logo=GitHub
[Badge-GitLab]: https://img.shields.io/gitlab/pipeline-status/IGR2014/IgrOS-Kernel?branch=master&label=GitLab%20CI&style=for-the-badge&logo=GitLab
[Badge-License]: https://img.shields.io/github/license/IGR2014/IgrOS-Kernel?label=License&style=for-the-badge&logo=Open%20Source%20Initiative
[Badge-Sonar-Bugs]: https://img.shields.io/sonar/bugs/IGR2014_IgrOS-Kernel/master?server=https%3A%2F%2Fsonarcloud.io&label=Bugs&style=for-the-badge&logo=SonarQubeCloud
[Badge-Sonar-Code-Smells]: https://img.shields.io/sonar/code_smells/IGR2014_IgrOS-Kernel/master?server=https%3A%2F%2Fsonarcloud.io&label=Code%20Smells&style=for-the-badge&logo=SonarQubeCloud
[Badge-Sonar-Coverage]: https://img.shields.io/sonar/coverage/IGR2014_IgrOS-Kernel/master?server=https%3A%2F%2Fsonarcloud.io&label=Coverage&style=for-the-badge&logo=SonarQubeCloud
[Badge-Sonar-Quality-Gate]: https://img.shields.io/sonar/quality_gate/IGR2014_IgrOS-Kernel/master?server=https%3A%2F%2Fsonarcloud.io&label=Quality%20Gate&style=for-the-badge&logo=SonarQubeCloud
[Badge-Sonar-Vulnerabilities]: https://img.shields.io/sonar/vulnerabilities/IGR2014_IgrOS-Kernel/master?server=https%3A%2F%2Fsonarcloud.io&label=Vulnerabilities&style=for-the-badge&logo=SonarQubeCloud

[Link-Arch-arm]: ./README.md
[Link-Arch-arm64]: ./README.md
[Link-Arch-avr]: ./README.md
[Link-Arch-i386]: ./arch/i386
[Link-Arch-x86_64]: ./arch/x86_64
[Link-Azure-DevOps]: https://dev.azure.com/prigoryan/IgrOS-Kernel/_build/latest?definitionId=3&branchName=master
[Link-Codacy]: https://app.codacy.com/gh/IGR2014/IgrOS-Kernel/dashboard
[Link-Codacy-Coverage]: https://app.codacy.com/gh/IGR2014/IgrOS-Kernel/coverage
[Link-CPP]: https://en.cppreference.com/w/cpp/20
[Link-GitHub]: https://github.com/IGR2014/IgrOS-Kernel/actions
[Link-GitLab]: https://gitlab.com/IGR2014/IgrOS-Kernel/-/pipelines
[Link-License]: https://opensource.org/licenses/MIT
[Link-Repo-Azure]: https://dev.azure.com/prigoryan/IgrOS-Kernel/_git/IgrOS-Kernel
[Link-Repo-GitHub]: https://github.com/IGR2014/IgrOS-Kernel
[Link-Repo-GitLab]: https://gitlab.com/IGR2014/IgrOS-Kernel
[Link-Sonar]: https://sonarcloud.io/dashboard?id=IGR2014_IgrOS-Kernel
