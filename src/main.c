#ifdef __CASIO__
#include <gint/display.h>
#include <gint/keyboard.h>
#endif
#include <stdio.h>

#include <stdlib.h>
#include <math.h>

#include "pi.h"

int x_pos = 4;
int y_pos = 0;

int skip_count = 2;

void add_digit(char digit)
{
    if (skip_count > 0)
    {
        // Logic to skip the first 2 digits of pi and print "3." instead of "03"
        if (skip_count == 1)
        {
#ifdef __CASIO__
            dtext(0, 0, C_BLACK, "3");
            dpixel(5, 3, C_BLACK);
            dupdate();
#else
            printf("3.");
#endif
        }
        skip_count--;
        return;
    }
    else
    {
        x_pos += 4;
        if (x_pos > 123)
        { // 127 - 4 (width of character)
            x_pos = 0;
            y_pos += 5;
        }
        char digit_print = digit; // Fixes a bug where a 0 is printed as after the digit
#ifdef __CASIO__
        dtext(x_pos, y_pos, C_BLACK, &digit_print);
        dupdate();
#else
        printf("%c", digit_print);
#endif
    }
}

int main(void)
{
#ifdef __CASIO__
    // Set font to custom tiny_font.png
    extern font_t tiny_font;
    dfont(&tiny_font);
#endif
    // Get first 52 digits of pi before switch to bellard
    char *digits;
    digits = (char *)malloc(97);
    pi_rabinowitz(96, digits); // Generates 1 more character than needed

    // Print first 52 digits
    for (int i = 0; i < 97; ++i)
    {
        add_digit(digits[i]);
    }
    free(digits); // Free digits from memory

    for (int counter = 95; counter < 3000; counter += NUMBER_OF_DIGITS)
    {
        double pifrac = pi_bellard(counter);
        for (int i = 0; i < NUMBER_OF_DIGITS; i++)
        {
            pifrac -= (int)pifrac; // Doesn't use math.h :p
            pifrac *= 10;
            add_digit((int)pifrac + '0');
        }
    }
#ifdef __CASIO__
    // Get key before quitting app preventing the app from closing
    getkey();
#else
    printf("\n");
#endif
    return 0;
}
