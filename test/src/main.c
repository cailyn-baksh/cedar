#include <cedar.h>
#include <cedar/button.h>
#include <cedar/label.h>

#undef NDEBUG
#include <debug.h>

#define MENU_PROGRAM		0x0001
#define MENUITEM_ABOUT		0x0002
#define MENUITEM_EXIT		0x0003

#define LABEL_HELLO			0x0101
#define LABEL_ALPHABET		0x0102
#define BUTTON_BUTTON		0x0103

CALLBACKRESULT mainWindowEventHandler(CedarWindow *self, EVENT event, uint24_t param) {
	switch (event) {
		case EVENT_PAINT:
			self->repaint = true;
			break;
		case EVENT_MENUSELECT:
			if (param == MENUITEM_EXIT) {
				return CALLBACK_EXIT;
			}
			break;
		case EVENT_BUTTONPRESS:
			dbg_printf("button press!\n");
			break;
	}

	return CALLBACK_NORMAL;
}

int main() {
	cedar_Init();

	CedarWindow window;
	cedar_InitWindow(&window);

	// Add menu
	CedarMenu menu;
	cedar_InitMenu(&menu);
	cedar_SetMenu(&window, &menu);
		CedarMenu prgmMenu;
		cedar_InitMenu(&prgmMenu);
		cedar_AddSubmenu(&menu, MENU_PROGRAM, "Program", &prgmMenu);
			cedar_AddMenuItem(&prgmMenu, MENUITEM_ABOUT, "About");
			cedar_AddMenuSeparator(&prgmMenu);
			cedar_AddMenuItem(&prgmMenu, MENUITEM_EXIT, "Exit");

	// Add widgets
	cedar_AddWidget(&window, CedarLabel(LABEL_HELLO, 10, 10, 70, 40, "Hello World!"));
	cedar_AddWidget(&window, CedarLabel(LABEL_ALPHABET, 110, 40, 70, 40, "abcdefghijklmnopqrstuvwxyz"));

	cedar_AddWidget(&window, CedarButton(BUTTON_BUTTON, 40, 180, 60, 20, "Button!"));

	cedar_RegisterEventHandler(&window, mainWindowEventHandler);

	cedar_Display(&window);

	cedar_DestroyWindow(&window);

	cedar_Cleanup();
	return 0;
}
