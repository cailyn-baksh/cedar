#include <stdbool.h>

#include <cedar.h>
#include <cedar/label.h>
#include <cedar/button.h>

#define MENU_PROGRAM		0x001
#define MENUITEM_START		0x002
#define MENUITEM_PAUSE		0x003
#define MENUITEM_RESET		0x004
#define MENUITEM_EXIT		0x005

#define LBL_TIME		0x101
#define BTN_START		0x111
#define BTN_PAUSE		0x112
#define BTN_RESET		0x113
#define TMR_TIMER		0x121

CALLBACKRESULT mainWindowEventHandler(CedarWindow *self, EVENT event, uint24_t param) {
	static bool timerOn = false;
	static uint24_t time = 0;

	switch (event) {
		case EVENT_MENUSELECT:
		case EVENT_BUTTONPRESS:
			switch (param) {
				case BTN_START:
				case MENUITEM_START:
					timerOn = true;
					break;
				case BTN_PAUSE:
				case MENUITEM_PAUSE:
					timerOn = false;
					break;
				case BTN_RESET:
				case MENUITEM_RESET:
					time = 0;
					break;
				case MENUITEM_EXIT:
					return CALLBACK_EXIT;
			}
			break;
		case EVENT_TICK:
			if (timerOn) {
				++time;
			}
			break;
	}

	return CALLBACK_NORMAL;
}

int main() {
	cedar_Init();

	CedarWindow window;
	cedar_InitWindow(&window);

	window.scrollMode = WINDOW_SCROLL_WIDGET;

	// Add Menu
	CedarMenu menu;
	cedar_InitMenu(&menu);
	cedar_SetMenu(&window, &menu);
		CedarMenu prgmMenu;
		cedar_InitMenu(&prgmMenu);
		cedar_AddSubmenu(&menu, MENU_PROGRAM, "Program", &prgmMenu);
			cedar_AddMenuItem(&prgmMenu, MENUITEM_START, "Start");
			cedar_AddMenuItem(&prgmMenu, MENUITEM_START, "Pause");
			cedar_AddMenuItem(&prgmMenu, MENUITEM_START, "Reset");
			cedar_AddMenuSeparator(&prgmMenu);
			cedar_AddMenuItem(&prgmMenu, MENUITEM_EXIT, "Exit");

	cedar_AddWidget(&window, CedarLabel(LBL_TIME, 10, 10, 50, 20, ""));

	cedar_AddWidget(&window, CedarButton(BTN_START, 10, 50, 50, 20, "Start"));
	cedar_AddWidget(&window, CedarButton(BTN_PAUSE, 70, 50, 50, 20, "Pause"));
	cedar_AddWidget(&window, CedarButton(BTN_RESET, 130, 50, 50, 20, "Reset"));

	cedar_AddTimer(&window, TMR_TIMER, 1000);

	cedar_RegisterEventHandler(window.handlers, mainWindowEventHandler);

	cedar_Display(&window);

	cedar_DestroyWindow(&window);
	cedar_Cleanup();
	return 0;
}