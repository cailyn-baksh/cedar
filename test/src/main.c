#include <cedar.h>
#include <cedar/button.h>
#include <cedar/checkbox.h>
#include <cedar/label.h>

#undef NDEBUG
#include <debug.h>

#define MENU_PROGRAM		0x0001
#define MENUITEM_ABOUT		0x0002
#define MENUITEM_EXIT		0x0003

#define LABEL_HELLO			0x0101
#define LABEL_ALPHABET		0x0102
#define BUTTON_BUTTON		0x0103
#define BUTTON_OFFSCREEN	0x0104
#define CHECKBOX_CHK		0x0105
#define TMR_TIMER			0x0106

CALLBACKRESULT mainWindowEventHandler(CedarWindow *self, EVENT event, uint24_t param) {
	switch (event) {
		case EVENT_MENUSELECT:
			if (param == MENUITEM_EXIT) {
				return CALLBACK_EXIT;
			}
			break;
		case EVENT_BUTTONPRESS:
			switch (param) {
				case BUTTON_BUTTON:
					dbg_printf("button press!\n");
					break;
				case BUTTON_OFFSCREEN:
					dbg_printf("btn2\n");
					break;
			}
			break;
		case EVENT_TICK:
			dbg_printf("%d ticked\n", param);
			break;
	}

	return CALLBACK_NORMAL;
}

int main() {
	cedar_Init();

	CedarWindow window;
	cedar_InitWindow(&window);

	window.scrollMode = WINDOW_SCROLL_WIDGET;

	window.colors.fg = 0xF0;
	window.colors.bg = 0x00;

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
	cedar_AddWidget(&window, CedarButton(BUTTON_OFFSCREEN, 420, 80, 50, 20, "btn2"));

	cedar_AddWidget(&window, CedarCheckbox(CHECKBOX_CHK, 180, 80));

	cedar_AddTimer(&window, TMR_TIMER, 1000);

	cedar_RegisterEventHandler(window.handlers, mainWindowEventHandler);

	cedar_Display(&window);

	cedar_DestroyWindow(&window);

	cedar_Cleanup();
	return 0;
}
