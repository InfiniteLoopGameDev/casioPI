void pi_rabinowitz(int num, char *buf){
    int len = ((10 * num/3) + 1);
    int* A = (int*)malloc(len * sizeof(int));

    int nines = 0;
    int predigit = 0;

    for(int j = 0; j < num; ++j) {
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
            buf[j] = predigit + 1 + '0';

            for (int k = 0; k < nines; ++k) { buf[j] = '0'; }
            predigit = 0;
            nines = 0;
        } else {
            buf[j] = predigit + '0';
            predigit = q;

            if (0 != nines) {    
                for (int k = 0; k < nines; ++k) { buf[j] = '9'; }
                nines = 0;
            }
        }
    }
    buf[len - 1] = predigit + '0';
    free(A);
    return;
}

void pi_bellard(long num, char *buf) {
    
}