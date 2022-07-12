#include <cedar.h>
#include <cedar/label.h>

#define MENUBUTTON_ABOUT	0x1
#define MENUBUTTON_EXIT		0x2
#define MENUBUTTON_OTHER	0x3

uint24_t mainWindowEventHandler(Window *window, int event) {
	if (event == EVENT_MENUSELECT) {
		MenuItem *selected = getLastSelectedMenuItem(window->menu);

		switch (selected->buttonID) {
			case MENUBUTTON_EXIT:
				return HANDLER_EXIT;
		}
	}

	return defaultWindowEventHandler(window, event);
}

int main() {
	cedar_init();

	Window window;
	cedar_initWindow(&window, mainWindowEventHandler);

	// Add menu
	Menu menu;
	cedar_initMenu(&menu);
	cedar_setMenu(&window, &menu);
		Menu prgmMenu;
		cedar_initMenu(&prgmMenu);
		cedar_addSubmenu(&menu, "Program", &prgmMenu);
			cedar_addMenuItem(&prgmMenu, "About", MENUBUTTON_ABOUT);
			cedar_addMenuSeparator(&prgmMenu);
			cedar_addMenuItem(&prgmMenu, "Exit", MENUBUTTON_EXIT);
	cedar_addMenuSeparator(&menu);
	cedar_addMenuItem(&menu, "Other", MENUBUTTON_OTHER);

	// Add widgets
	cedar_addWidget(&window, Label(10, 10, 70, 40, "Hello World!"));
	cedar_addWidget(&window, Label(110, 40, 70, 40, "abcdefghijklmnopqrstuvwxyz"));

	cedar_display(&window);

	cedar_destroyWindow(&window);

	cedar_cleanup();
	return 0;
}
