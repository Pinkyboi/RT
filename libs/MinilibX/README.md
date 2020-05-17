# MinilibX
Simple graphics library

This is a mirror of ol's MinilibX, in both it's X11 and Cocoa versions. The
following text is derived from the original README which you can find in the
subfolders.

I added a Makefile which compiles the right version for the current platform and
an additional file with variables for easier linking. The libraries themselves
are entirely ol's work!

---

This is the MinilibX, a simple X-Window (X11R6) programming API in C, designed
for students, suitable for X-beginners.

## Contents
* Source code in C
* Both Cocoa and X11
* A public include file mlx.h

## Requirements
* X11 include files if applicable
* XShm for the X11 version

## Compiling
Run `make`, an appropriate version for your current platform will be created and
copied to the root of this repository.

## Installing
No installation script is provided. You may want to install
* `libmlx.a` and/or `libmlx_$(HOSTTYPE).a` in `/usr/X11/lib` or `/usr/local/lib`
* `mlx.h` in `/usr/X11/include` or `/usr/local/include`
* `man/man3/mlx*.1` in `/usr/X11/man/man3` or `/usr/local/man/man3`

## License
MinilibX is licensed under the BSD license: Copyright Olivier Crouzet -
1999-2015
