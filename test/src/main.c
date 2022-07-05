#include <cedar.h>
#include <cedar/label.h>

int menuAboutSelectHandler() {
	return 0;
}

int menuExitSelectHandler() {
	return 0;
}

int main() {
	cedar_init();

	Window window;
	cedar_initWindow(&window);

	// Add menu
	Menu menu;
	cedar_initMenu(&menu);
	cedar_setMenu(&window, &menu);

	Menu prgmMenu;
	cedar_initMenu(&prgmMenu);
	cedar_addSubmenu(&menu, "Program", &prgmMenu);

	cedar_addMenuItem(&prgmMenu, "About", menuAboutSelectHandler);
	cedar_addMenuSeparator(&prgmMenu);
	cedar_addMenuItem(&prgmMenu, "Exit", menuExitSelectHandler);

	cedar_addMenuSeparator(&menu);  // this doesnt work for some reason...

	// Add widgets
	cedar_addWidget(&window, Label(10, 10, 10, 30, "Hello World!"));

	cedar_display(&window);

	cedar_destroyWindow(&window);

	cedar_cleanup();
	return 0;
}
