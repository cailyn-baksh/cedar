#include <cedar.h>
#include <cedar/label.h>

#define LBL_HELLO		0x0001

int main() {
	cedar_Init();

	CedarWindow window;
	cedar_InitWindow(&window);

	window.scrollMode = WINDOW_SCROLL_WIDGET;

	cedar_AddWidget(&window, CedarLabel(LBL_HELLO, 10, 10, 100, 40, "Hello World!"));

	cedar_Display(&window);

	cedar_DestroyWindow(&window);
	cedar_Cleanup();
	return 0;
}
