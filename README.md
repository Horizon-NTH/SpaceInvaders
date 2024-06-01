# SpaceInvaders

[![Release](https://img.shields.io/badge/Release-v1.0-blueviolet)](https://github.com/Horizon-NTH/SpaceInvaders/releases)
[![Language](https://img.shields.io/badge/Language-C++-0052cf?logo=cplusplus&logoColor=blue)](https://en.wikipedia.org/wiki/C%2B%2B)
[![Licence](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

## Introduction

**SpaceInvaders** is a game inspired by the famous [arcade game](https://en.wikipedia.org/wiki/Space_Invaders).

## Installation Instructions

> You can also simply install a pre-built version [here](https://github.com/Horizon-NTH/SpaceInvaders/releases).

### Get Source Code

You first need to clone the [repository](https://github.com/Horizon-NTH/SpaceInvaders).
Make sure to use [git](https://git-scm.com) and don't forget to include `--recurse-submodules` when cloning.

```bash
git clone https://github.com/Horizon-NTH/SpaceInvaders --recurse-submodules
```

You will now able to choose between [script](#script-installation) or [manual](#manual-installation) installation

### Script Installation

Use the [`install.sh`](https://github.com/Horizon-NTH/SpaceInvaders/blob/master/install.sh) script to compile the
executable.
You can choose to erase build files by adding `-e` / `--erase`.

  ```bash
  ./install.sh --erase
  ```

### Manual Installation

Ensure you have [CMake](https://cmake.org/) installed.
Generate the build environment using CMake.

```bash
cd SpaceInvaders
mkdir build && cd build
cmake ..
```

Then build directly using CMake:

```bash
cmake --build .
```

This command will create the executable in the main folder.

## Documentation

To learn how this application work and all the possibilities , please refer to
the [wiki](https://github.com/Horizon-NTH/SpaceInvaders/wiki).

## Dependencies

- **HorizonGUI**

  The code relies on [HorizonGUI](https://github.com/Horizon-NTH/HorizonGUI) for all the graphics-related functionality
  of the application.

## License

[**SpaceInvaders**](https://github.com/Horizon-NTH/SpaceInvaders) is licensed under
the [MIT license](https://github.com/Horizon-NTH/SpaceInvaders/blob/master/LICENSE).