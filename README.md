# Cedar

Widget library for TI-84+ CE

## Building

To build Cedar, install the [CE toolchain](https://github.com/CE-Programming/toolchain), then run `make`.
This will produce several `.src` files in `out/`. To link these to a program, add the files as
`EXTRA_ASM_SOURCES` in your program's makefile.

## Basic Usage

Add `include/` to your include path, and then include `<cedar.h>`. Widgets are defined in other header files,
in the `include/cedar` subdirectory. To use them include `<cedar/WIDGET.h>`.

To begin using cedar, start by calling `cedar_init`. You must also call `cedar_cleanup` at the end of your program.

In Cedar, GUIs are created by adding widgets to windows. To create a window declare a `Window` variable and pass it to
`cedar_initWindow`. After you add your widgets to the window, call `cedar_display` to enter the window's mainloop.
The window will run indefinitely until one of the widget's event handlers closes it, or the user presses the Quit
button. When you are done with a window (i.e. you will not be rendering it again), call `cedar_destroyWindow` to clean
up the window's memory.

Below is a sample program.

```c
#include <cedar.h>

int main() {
	cedar_init();
	
	Window window;
	cedar_initWindow(&window);

	cedar_addWidget(&window, Label(25, 25, 15, 10, "Hello World!"));
	
	cedar_display(&window);
	
	cedar_destroyWindow(&window);

	cedar_cleanup();
	return 0;
}
```
