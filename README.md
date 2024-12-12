# CHlib

This project is a super-mini Raylib. It was originally intended to not be a separate project, but instead just in insert for another project of mine: [Image Reading](https://github.com/champso1/image-reading). However, I realized that in many of my other projects in which I employ any sort of rendering, it is always really simple stuff, such as just drawing shapes and stuff to the screen. [One of my other projects](https://github.com/champso1/rubikscube_app_sdl2) uses some text rendering, which in principle won't be super crazy. 

After realizing this, I decided to make this project.

## Building

This follows standard CMake building:

```
mkdir build
cd build
cmake ..
cmake --build .
cmake --install .
```

By default, these are installed to system directories. That can be changed by passing the flag `-DCMAKE_INSTALL_PREFIX=/wherever/you/want`. I typically create another folder on the same level as the `build` folder called `install` which I install the build artifacts to.

## Using with CMake

In the examples folder is just one example (at the moment), that draws a 640x480 screen with a dark gray background. Pressing ESC on the keyboard will close the window.

To build this (and any other project), just make sure that CMake can find the `CHlibConfig.cmake` file that was installed along with the other build artifacts. 

In the case of the example, just follow the same build steps as for the project itself, but without the install step. An executable named `example` will be created.

For other projects, within your `CMakeLists.txt` file, add 

```
find_package(CHlib REQUIRED)
target_link_library(... PRIVATE CHlib::CHlib)
```

Examine the file in the `examples/` directory for more clues.

# TODO

There is certainly a lot to do for this project. Despite the comment made at the beginning that this ended up becoming its own separate thing, its primary purpose is still to be used within the image-reading project. So, development will be focused there first, and the following TODO list will contain things relevent only for getting that project running successfully.


## Concrete Things
These are straight-forward things that are a one-and-done.

- [ ] Implement a batch-like system for rendering objects (I believe this is roughly how Raylib does it).
- [ ] Implement rendering of shapes.
- [ ] Have a different interface for specifying callbacks that doesn't make the user interact with glfw or GL in any way.


## Abstract Things
These are less straight-forward and are essentially stylistic and other non-important changes/considerations I want to make.

- [ ] Consider how to better handle errors. I may consider a sort of error union/optional type thing like Zig.
- [ ] Consider what I want to do about naming.
- * Specifically about prefixes of functions/types I want to keep hidden. For instance, a.t.m. `chgl` is the prefix for internal functions that interact with glfw/GL, as well as for all types. I may want to change this to avoid confusion.
