# CHlib

This project is a super-mini Raylib. It was originally intended to not be a separate project, but instead just in insert for another project of mine: [Image Reading](https://github.com/champso1/image-reading). However, I realized that in many of my other projects in which I employ any sort of rendering, it is always really simple stuff, such as just drawing shapes and stuff to the screen. [One of my other projects](https://github.com/champso1/rubikscube_app_sdl2) uses some text rendering, which in principle won't be super crazy. So, I figured I'd start this and add basically a bunch of basic rendering functionality.

## Building

This follows standard CMake building:

```
mkdir build
cd build
cmake ..
cmake --build .
cmake --install .
```

By default, these are installed to system directories. That can be changed by passing the flag `-DCMAKE_INSTALL_PREFIX=/wherever/you/want`. I typically create another folder on the same level as the `build` folder called `install` which I install the build artifacts to. Just be sure that wherever it is installed, CMake knows to look there for configuration files.

## Using with CMake

In the examples folder are two examples. The first draws a grid to the screen by creating an array of colors. The second draws a single rectangle to the screen. 

To build these (and any other project), just make sure that CMake can find the `CHlibConfig.cmake` file that was installed along with the other build artifacts as mentioned above. 

In the case of the example, just follow the same build steps as for the project itself, but without the install step. An executable named `example` will be created.

For other projects, within your `CMakeLists.txt` file, add 

```
find_package(CHlib REQUIRED)
target_link_library(... PRIVATE CHlib::CHlib)
```

Examine the `CMakeLists.txt` files in the examples for hints.

# TODO

There is certainly a lot to do for this project. Despite the comment made at the beginning that this ended up becoming its own separate thing, its primary purpose is still to be used within the image-reading project. So, development will be focused there first, and the following TODO list will contain things relevent only for getting that project running successfully.


## Concrete Things
These are straight-forward things that are a one-and-done.

- [ ] Implement a batch-like system for rendering objects (I believe this is roughly how Raylib does it).
- This is implemented, you could say, but more testing would have to be done for more use cases definitely.
- [ ] Implement rendering of shapes.
- Rectangles are done.
- [ ] Have a different interface for specifying callbacks that doesn't make the user interact with glfw or GL in any way.
- [ ] Text rendering!


## Abstract Things
These are less straight-forward and are essentially stylistic and other non-important changes/considerations I want to make.

- [ ] Consider how to better handle errors. I may consider a sort of error union/optional type thing like Zig.
- [ ] Consider what I want to do about naming.
- * Specifically about prefixes of functions/types I want to keep hidden. For instance, a.t.m. `chgl` is the prefix for internal functions that interact with glfw/GL, as well as for all types. I may want to change this to avoid confusion.
