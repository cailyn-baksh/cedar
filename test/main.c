#include <cedar.h>
#include <cedar/label.h>

int main() {
	Window mainWindow;
	cedar_initWindow(&mainWindow);

	cedar_addWidget(&mainWindow, Label(0, 0, 10, 3, "Hello World!"));

	cedar_display(&mainWindow);

	cedar_destroyWindow(&mainWindow);
}
