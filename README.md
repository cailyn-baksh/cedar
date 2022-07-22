# Cedar

A widget library for the TI-84+ CE. Documentation is available [here](https://cailyn-baksh.github.io/cedar/).

## Building

To build Cedar, install the [CE toolchain](https://github.com/CE-Programming/toolchain), then run `make`.
This will produce a file called `libcedar.src` in `out/`.


## Installation
Install the [CE toolchain](https://github.com/CE-Programming/toolchain). This is required to make programs with Cedar.

To begin installation, either [build Cedar yourself](#building), or [download the latest release from GitHub](https://github.com/cailyn-baksh/cedar/releases).
You should have the file `libcedar.src`, and a folder called `include/`.

At this point, Cedar is ready for you to use in your programs!
Add `include/` to your include directories, and `libcedar.src` to your makefile's
`EXTRA_ASM_SOURCES`.


## Basic Usage

Below is a sample program.

```c
#include <cedar.h>

#define LBL_HELLO	0x0001

int main() {
	cedar_Init();
	
	Window window;
	cedar_InitWindow(&window);

	cedar_AddWidget(&window, Label(LBL_HELLO, 25, 25, 15, 40, "Hello World!"));
	
	cedar_Display(&window);
	
	cedar_DestroyWindow(&window);
	cedar_Cleanup();
	return 0;
}
```

See `examples/` for more examples.
