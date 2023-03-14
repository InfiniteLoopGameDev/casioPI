#include <gint/display.h>
#include <gint/keyboard.h>
#define N 263
#define len ((10 * N/3) + 1)

int A[len] = {[0 ... len-1] = 2}; // Non-standard array initialization GCC only

int nines = 0;
int predigit = 0;

int x_pos = 4;
int y_pos = 0;

int skip_count = 2;

void add_digit(int digit) {
	if (skip_count > 0) {
		if (skip_count == 1) { 
            dtext(0, 0, C_BLACK, "3");
            dpixel(5, 3, C_BLACK);
        }
		--skip_count;
		return;
	}
	char buf = digit + '0';
	x_pos += 4;
	if (x_pos > 123) {
		x_pos = 0;
		y_pos += 5;
	}
	dtext(x_pos, y_pos, C_BLACK, &buf);
	dupdate();
}

// Pi calculated using the Rabinowitz/Wagon pi spigot algorithm
// Based of the Pascal code by Simeon Simeonov
// Both can be found at: 
// https://www.maa.org/sites/default/files/pdf/pubs/amm_supplements/Monthly_Reference_12.pdf

int main(void) {
    extern font_t tiny_font;

    dfont(&tiny_font);

    for(int j = 1; j < N + 1; ++j) {
        int q = 0;

        for(int i = len; i > 0; --i) {
            int x  = 10 * A[i-1] + q*i;
            A[i-1] = x % (2*i - 1);
            q = x / (2*i - 1);
        }

        A[0] = q%10;
        q = q/10;

        if (9 == q) {
            ++nines;
        } else if (10 == q) {
            add_digit(predigit + 1);

            for (int k = 0; k < nines; ++k) { add_digit(0); }
            predigit = 0;
            nines = 0;
        } else {
            add_digit(predigit);
            predigit = q;

            if (0 != nines) {    
                for (int k = 0; k < nines; ++k) { add_digit(9); }
                nines = 0;
            }
        }
    }
    add_digit(predigit);
	getkey();
	return 0;
}
