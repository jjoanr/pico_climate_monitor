#include "display.h"
#include <stdio.h>

void display_data(uint8_t data[5], ssd1306_t *disp) {
	ssd1306_clear(disp);
	// Format humidity and temperature data for display
	char temp_str[20];
	snprintf(temp_str, sizeof(temp_str), "%d.%d C", data[2], data[3]);
	ssd1306_draw_string(disp, 12, 12, 3, temp_str);
	char humidity_str[20];
	snprintf(humidity_str, sizeof(humidity_str), "%d.%d %%", data[0], data[1]);
	ssd1306_draw_string(disp, 30, 42, 2, humidity_str);

	ssd1306_show(disp);
}
