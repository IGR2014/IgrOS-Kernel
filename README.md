# IgrOS-Kernel

IgrOS-Kernel is a simple **OS kernel** written in **C++20**.
Currently supports **i386** and **x86_64** platforms. **ARM** and **AVR** are the next aims.

---

## Project status:

| Action                    |       Badge       |
| :---                      |       :---        |
| License                   | [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) |
| GitHub Pipeline           | [![GitHub pipeline](https://github.com/IGR2014/IgrOS-Kernel/workflows/IgrOS-Kernel%20C/C++%20CI/badge.svg)](https://github.com/IGR2014/IgrOS-Kernel/actions) |
| GitLab Pipeline           | [![GitLab pipeline](https://gitlab.com/IGR2014/IgrOS-Kernel/badges/master/pipeline.svg)](https://gitlab.com/IGR2014/IgrOS-Kernel/-/commits/master) |
| Azure Pipeline            | [![Azure pipeline](https://dev.azure.com/prigoryan/IgrOS-Kernel/_apis/build/status/IGR2014.IgrOS-Kernel?branchName=master)](https://dev.azure.com/prigoryan/IgrOS-Kernel/_build/latest?definitionId=4&branchName=master) |
| Codacy Quality            | [![Codacy Badge](https://api.codacy.com/project/badge/Grade/1a4425daf2a946448a3d9c915c25da71)](https://app.codacy.com/app/IGR2014/IgrOS-Kernel?utm_source=github.com&utm_medium=referral&utm_content=IGR2014/IgrOS-Kernel&utm_campaign=Badge_Grade_Dashboard) |
| SonarCloud Security       | [![Security Rating](https://sonarcloud.io/api/project_badges/measure?project=IGR2014_IgrOS-Kernel&metric=security_rating)](https://sonarcloud.io/dashboard?id=IGR2014_IgrOS-Kernel) |
| SonarCloud Quality        | [![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=IGR2014_IgrOS-Kernel&metric=alert_status)](https://sonarcloud.io/dashboard?id=IGR2014_IgrOS-Kernel) |
| SonarCloud Reliability    | [![Reliability Rating](https://sonarcloud.io/api/project_badges/measure?project=IGR2014_IgrOS-Kernel&metric=reliability_rating)](https://sonarcloud.io/dashboard?id=IGR2014_IgrOS-Kernel) |
| SonarCloud Bugs           | [![Bugs](https://sonarcloud.io/api/project_badges/measure?project=IGR2014_IgrOS-Kernel&metric=bugs)](https://sonarcloud.io/dashboard?id=IGR2014_IgrOS-Kernel) |
| SonarCloud Coverage       | [![Coverage](https://sonarcloud.io/api/project_badges/measure?project=IGR2014_IgrOS-Kernel&metric=coverage)](https://sonarcloud.io/dashboard?id=IGR2014_IgrOS-Kernel) |
| SonarCloud Duplications   | [![Duplicated Lines (%)](https://sonarcloud.io/api/project_badges/measure?project=IGR2014_IgrOS-Kernel&metric=duplicated_lines_density)](https://sonarcloud.io/dashboard?id=IGR2014_IgrOS-Kernel) |
| SonarCloud LOC            | [![Lines of Code](https://sonarcloud.io/api/project_badges/measure?project=IGR2014_IgrOS-Kernel&metric=ncloc)](https://sonarcloud.io/dashboard?id=IGR2014_IgrOS-Kernel) |

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
###### ***Developed and maintained by Igor Baklykov (c) 2017 - 2021***

