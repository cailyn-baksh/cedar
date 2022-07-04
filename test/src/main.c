#include <cedar.h>
#include <cedar/label.h>

int menuExitSelectHandler(Menu *menu) {

}

int main() {
	cedar_init();

	Window window;
	cedar_initWindow(&window);

	// Add menu
	Menu menu;
	cedar_setMenu(&window, &menu);

	Menu prgmMenu;
	cedar_addSubmenu(&menu, "Program", &prgmMenu);
	cedar_addMenuSeparator(&prgmMenu);
	cedar_addMenuItem(&prgmMenu, "Exit", menuExitSelectHandler);

	// Add widgets
	cedar_addWidget(&window, Label(10, 10, 10, 30, "Hello World!"));

	cedar_display(&window);

	cedar_destroyWindow(&window);

	cedar_cleanup();
	return 0;
}
