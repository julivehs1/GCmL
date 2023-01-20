# GCmL: *Generate CMakeLists*


## Idea

Personally, I really like Java because of the organisation of aource Code in packages.
It enables to group related code directly.
Furthermore, the hierarchical structuring based on recursive directories is humanfriendly.

On the other hand I most often use C++ for programming.
CMake represents a pretty nice way of organizing a project and configuring all targets.

This projects aims to enable package-based grouping for C++.


## Usage

GCmL is very easy to use:

```
.\GCml.exe .\src\
```

You just have to specify a directory on which the package root is located.
GCml will discover all source and header files recursively.

You can also introduce a root-package with a specific name using:
```
.\GCml.exe .\src\ -r --root_name Core
```

The additional use of flag `-d` enables GCmL to print the package hierarchy:
```
.\GCml.exe .\src\ -r --root_name Core -d
Package Hierarchy:
Core ─┐
      ├─ Filesystem
      ├─ Gui
      └─ Math ─┐
               ├─ Matrix
               └─ Optimization
```
The resulting packages can be addressed in hierarchical C++-Style.
Fore example, the resulting possible packages are:
```
Core, Core::Filesystem, Core::Gui, Core::Math, Core::Math::Matrix, Core::Math::Optimization
```

GCmL creates a *package.cmake* and, if not already existing, a *CMakeLists.txt* in all directories important for the package hierarchy.
An existing CMakeLists.txt does not get destroyed while all package.cmake are replaced.
So only add manual commands to the CMakeLists.txt.


### Advanced Usage

CMake offers a `execute_process(...)` Method, which can be used to update all packages automatically when CMake is reloaded.
The only thing to do is to specify it in the main CMakeLists.txt:
```
execute_process(COMMAND GCmL.exe ${PROJECT_SOURCE_DIR}\\src -r --root_name Core)
```
It is important that this command is places before a package is used in `add_library`, `target_link_library` and so on, as well as `include(package.cmake)`.


## Build

The only prerequisite is the Boost library, especially the Library *program_options* which is used for parsing of the arguments.

The project easily can be built using CMake.

I recommend adding the executable to the PATH environment variable, so it can be called very easy.

