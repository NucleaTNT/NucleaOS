# Welcome to the Repository!

## What is this?
<b>NucleaOS</b> is a UNIX-like [Hobby OS](https://en.wikipedia.org/wiki/Hobbyist_operating_system_development) developed by [NucleaTNT](https://github.com/NucleaTNT) in his free time. There will likely be many bugs and it is not intended as an alternative to mainstream OS's like Linux and Windows.

## Why is this?
NucleaOS was created  with the intention of NucleaTNT learning more about the inner workings of computer systems. He enjoys tearing things apart to the lowest levels possible and Operating Systems are one of the more challenging and low-level projects available.

## How is this?
Utilising third-party applications/sources such as [Docker](https://www.docker.com) and the [GRand Unified Bootloader](https://www.gnu.org/software/grub) ("GRUB") this OS easily builds and boots (much more reliably!) into a 64-bit environment with little need for a large amount of bootloader code. The majority of the kernel is programmed in C++ with the occasional use of assembly here and there.

## Setup
Build an image for our build-environment:
- `docker build buildenv -t nucleaos-buildenv`

## Build
Enter build environment:
- Linux or MacOS: `docker run --rm -it -v "$PWD":/root/env nucleaos-buildenv`
- Windows (CMD): `docker run --rm -it -v "%cd%":/root/env nucleaos-buildenv`

Build for x86_64:
- `make build-x86_64`

## Run
Either run the built image in `dist` or if QEMU is installed:
- `make run-{architecture}`