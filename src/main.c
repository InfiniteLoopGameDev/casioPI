#include <gint/display.h>
#include <gint/keyboard.h>

#include "pi.h"

int x_pos = 4;
int y_pos = 0;

int skip_count = 2;

void add_digit(char digit) {
	if (skip_count > 0) {
        // Logic to skip the first 2 digits of pi and print "3." instead of "03"
		if (skip_count == 1) { 
            dtext(0, 0, C_BLACK, "3");
            dpixel(5, 3, C_BLACK);
            dupdate();
        }
		skip_count--;
		return;
	} else {
        x_pos += 4;
        if (x_pos > 123) {  // 127 - 4 (width of character)
            x_pos = 0;
            y_pos += 5;
        }
        char digit_print = digit; // Fixes a bug where a 0 is printed as afte the digit
        dtext(x_pos, y_pos, C_BLACK, &digit_print);
        dupdate();
    }
}

int main(void) {
    // Set font to custom tiny_font.png
    extern font_t tiny_font;
    dfont(&tiny_font);

    // Get first 52 digits of pi before switch to bellard
    char digits[52];
    pi_rabinowitz(51, digits); // Generates 1 more character than needed

    // Print first 52 digits
    for (int i = 0; i < 52; ++i) {
        add_digit(digits[i]);
    }

    // Get key before quitting app preventing the app from closing
    getkey();
	return 0;
}
