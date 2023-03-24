# IgrOS-Kernel [![Badge-CPP]][Link-CPP]

IgrOS-Kernel is a simple **OS kernel**.

##### Supported Architectures:
[![Badge-Arch-i386]][Link-Arch-i386]
[![Badge-Arch-x86_64]][Link-Arch-x86_64]

##### NOT YET Supported Architectures:
[![Badge-Arch-arm]][Link-Arch-arm]
[![Badge-Arch-arm64]][Link-Arch-arm64]
[![Badge-Arch-avr]][Link-Arch-avr]


---

## Project stats:

[![Badge-License]][Link-License]
[![Badge-GitHub]][Link-GitHub]
[![Badge-GitLab]][Link-GitLab]
[![Badge-Azure-DevOps]][Link-Azure-DevOps]
[![Badge-Codacy-Quality]][Link-Codacy]
[![Badge-Codacy-Coverage]][Link-Codacy]
[![Security Rating](https://sonarcloud.io/api/project_badges/measure?project=IGR2014_IgrOS-Kernel&metric=security_rating)](https://sonarcloud.io/dashboard?id=IGR2014_IgrOS-Kernel)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=IGR2014_IgrOS-Kernel&metric=alert_status)](https://sonarcloud.io/dashboard?id=IGR2014_IgrOS-Kernel)
[![Reliability Rating](https://sonarcloud.io/api/project_badges/measure?project=IGR2014_IgrOS-Kernel&metric=reliability_rating)](https://sonarcloud.io/dashboard?id=IGR2014_IgrOS-Kernel)
[![Bugs](https://sonarcloud.io/api/project_badges/measure?project=IGR2014_IgrOS-Kernel&metric=bugs)](https://sonarcloud.io/dashboard?id=IGR2014_IgrOS-Kernel)
[![Coverage](https://sonarcloud.io/api/project_badges/measure?project=IGR2014_IgrOS-Kernel&metric=coverage)](https://sonarcloud.io/dashboard?id=IGR2014_IgrOS-Kernel)
[![Duplicated Lines (%)](https://sonarcloud.io/api/project_badges/measure?project=IGR2014_IgrOS-Kernel&metric=duplicated_lines_density)](https://sonarcloud.io/dashboard?id=IGR2014_IgrOS-Kernel)
[![Lines of Code](https://sonarcloud.io/api/project_badges/measure?project=IGR2014_IgrOS-Kernel&metric=ncloc)](https://sonarcloud.io/dashboard?id=IGR2014_IgrOS-Kernel)

---

## Implemented features:

| Feature                    |       Status       |
| :---                       |       :---:        |
| **Custom bootloader**      |                    |
| **Multiboot**              | :heavy_check_mark: |
| **Multiboot 2**            |                    |
| **GDT**                    | :heavy_check_mark: |
| **IDT**                    | :heavy_check_mark: |
| **Exceptions**             | :heavy_check_mark: |
| **Interrupts**             | :heavy_check_mark: |
| **Paging**                 | :heavy_check_mark: |
| **Phys. page allocator**   | :heavy_check_mark: |
| **Virt. memory allocator** |                    |
| **VGA driver (text mode)** | :heavy_check_mark: |
| **PIT driver**             | :heavy_check_mark: |
| **Keyboard driver (read)** | :heavy_check_mark: |
| **CMOS RTC driver (read)** | :heavy_check_mark: |
| **User mode**              |                    |
| **Kernel drivers**         |                    |
| **User programs**          |                    |
| **GUI**                    |                    |

---

### ***P.S.:***
###### ***Developed and maintained by Igor Baklykov (c) 2017 - 2023***


[Badge-Arch-arm]: https://img.shields.io/static/v1?label=Arch&message=arm&color=orangered&style=for-the-badge
[Badge-Arch-arm64]: https://img.shields.io/static/v1?label=Arch&message=arm64&color=orangered&style=for-the-badge
[Badge-Arch-avr]: https://img.shields.io/static/v1?label=Arch&message=avr&color=orangered&style=for-the-badge
[Badge-Arch-i386]: https://img.shields.io/static/v1?label=Arch&message=i386&color=lime&style=for-the-badge
[Badge-Arch-x86_64]: https://img.shields.io/static/v1?label=Arch&message=x86_64&color=lime&style=for-the-badge
[Badge-Azure-DevOps]: https://img.shields.io/azure-devops/build/prigoryan/1dea483f-6c7a-4efb-8631-5309fd3370ec/4/master?label=CI%2FCD%20Azure%20DevOps&style=for-the-badge&logo=Azure%20DevOps
[Badge-Codacy-Coverage]: https://img.shields.io/codacy/coverage/1a4425daf2a946448a3d9c915c25da71/master?label=Codacy%20Coverage&style=for-the-badge&logo=Codacy
[Badge-Codacy-Quality]: https://img.shields.io/codacy/grade/1a4425daf2a946448a3d9c915c25da71/master?label=CODACY%20QUALITY&style=for-the-badge&logo=Codacy
[Badge-CPP]: https://img.shields.io/static/v1?label=Standard&message=C%2B%2B20&color=blue&style=for-the-badge&logo=C%2B%2B
[Badge-GitHub]: https://img.shields.io/github/actions/workflow/status/IGR2014/IgrOS-Kernel/github-ci.yml?branch=master&label=CI%2FCD%20GitHub&style=for-the-badge&logo=GitHub
[Badge-GitLab]: https://img.shields.io/gitlab/pipeline-status/IGR2014/IgrOS-Kernel?branch=master&label=CI%2FCD%20GitLab&style=for-the-badge&logo=GitLab
[Badge-License]: https://img.shields.io/github/license/IGR2014/IgrOS-Kernel?label=License&style=for-the-badge&logo=Open%20Source%20Initiative

[Link-Arch-arm]: ./README.md
[Link-Arch-arm64]: ./README.md
[Link-Arch-avr]: ./README.md
[Link-Arch-i386]: ./arch/i386
[Link-Arch-x86_64]: ./arch/x86_64
[Link-Azure-DevOps]: https://dev.azure.com/prigoryan/IgrOS-Kernel/_build/latest?definitionId=4&branchName=master
[Link-Codacy]: https://app.codacy.com/gh/IGR2014/IgrOS-Kernel/dashboard
[Link-CPP]: https://en.cppreference.com/w/cpp/20
[Link-GitHub]: https://github.com/IGR2014/IgrOS-Kernel/actions
[Link-GitLab]: https://gitlab.com/IGR2014/IgrOS-Kernel/-/commits/master
[Link-License]: https://opensource.org/licenses/MIT
