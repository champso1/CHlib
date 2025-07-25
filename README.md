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
- [ ] Implement rendering of shapes, rectangles first.
- [ ] Have a different interface for specifying callbacks that doesn't make the user interact with glfw or GL in any way.
- [ ] Text rendering!
- [ ] Switch to CPP (see [Notes](#notes))


## Abstract Things
These are less straight-forward and are essentially stylistic and other non-important changes/considerations I want to make.

- [ ] Consider how to better handle errors. I may consider a sort of error union/optional type thing like Zig.
- [ ] Consider what I want to do about naming.
- * Specifically about prefixes of functions/types I want to keep hidden. For instance, a.t.m. `cl` is the prefix for internal functions that interact with glfw/GL, as well as for all types. I may want to change this to avoid confusion.

## Notes
There are two things worth mentioning, one being that this project was put off for several (I think over 7) months, and along with this, I've been doing more development on my Windows machine with MSYS2 tools (previously this had only been worked on with my Linux machine). To get things working I have had to do a lot of reworking, including completely removing rectangles (which previously worked, I believe). So, again, those have to be reworked a bunch.

Next, my previous opinion towards C++ was fairly poor; the STL was just so extremely daunting, especially past C++11, but after having used C++ for a relatively large project and currently doing C++ for my theoretical physics research, I've grown more accustomed to it. To be more specific, I've come to accept that I'll simply never fully understand the STL in its entirety, but the basic things I do know and the basic things that come with C++ as opposed to C (mainly generics and classes and such) are good enough. With this, I am probably next going to convert everything here to CPP. This also allows GLM to be used as intended and not having a dependency on CGLM; it allows my entire custom hashmap to be removed in favor of `std::map`, and so on. Some other things with strings and files become easier, it will allow for more generic programming in the future, it will allow easier error and memory handling, and so on.

This isn't that I enjoy pure C any less; I find it very fascinating working with raw memory and doing many things manually. At some point I'll make my own C library with a lot of the basic things in the C++ STL, like better string/file control, logging/error handling, and so on, and maybe do some other substantial project in the future with that. But for this, I simply think it'd be more fun with C++. This was more than just a "note", but oh well.
