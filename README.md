Basic Graphics Engine
=====================

The project has been discontinued on linux and moved over to windows visual studio C++. Click [here](https://github.com/ah-aydin/opengl-engine-vs) to go to the new repo.

Requirements
------------

The code was written on a linux system.

Required library installations

```
sudo apt-get install libsdl2-dev libglew-dev libassimp-dev libglm-dev
```

Compiling the program
---------------------
Use:
```
source run.sh b r
```

Run.sh
------

This is a file that streamlines the compilation/running of the application.

``r`` - runs the program

``c`` - cleans the .obj files

``b`` - build the application

``d`` - puts the compiler in debug mode

Order of the options do not matter

### Usage

```
source run.sh [OPTION]
```

### Examples

Build in debug mode
```
source run.sh b d
```

Run in debug mode
```
source run.sh r d
```

Build and run in release mode
```
source run.sh b r
```
