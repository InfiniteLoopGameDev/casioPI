#include <gint/display.h>
#include <gint/keyboard.h>

#include "pi.h"

int x_pos = 4;
int y_pos = 0;

int skip_count = 2;

void add_digit(char digit) {
	if (skip_count > 0) {
		if (skip_count == 1) { 
            dtext(0, 0, C_BLACK, "3");
            dpixel(5, 3, C_BLACK);
        }
		skip_count--;
		return;
	} else {
        y_pos += 8;
        if (x_pos > 123) {
            x_pos = 0;
            y_pos += 5;
        }
        dtext(x_pos, y_pos, C_BLACK, &digit);
        dupdate();
    }
}

int main(void) {
    // Set font to custom tiny_font.png
    extern font_t tiny_font;
    dfont(&tiny_font);

    // Get first 52 digits of pi before switch to bellard
    char digits[5];
    pi_rabinowitz(5, digits);

    // Print first 52 digits
    for (int i = 0; i < 5; ++i) {
        add_digit(digits[i] - '0');
    }

    // Get key before quitting app preventing the app from closing
    getkey();
	return 0;
}
