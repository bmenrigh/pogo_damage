#include <stdio.h>
#include <math.h>
#include <stdint.h>

union du {
    double d;
    uint64_t u;
};


double adjacent_double(double d, long n) {
    union du du = {.d = d};

    /* Adjust +/- n steps */
    du.u += n;

    return du.d;
}



double damage(int pwr, int atk, int def, float acpm, float dcpm, double mult) {

    /*double dmg = (0.5) * (double)pwr * (((double)atk * (double)acpm) / ((double)def * (double)dcpm)) * mult;*/

    /* calculation in steps to try to maximize intermediate result */
    double dmg;

    /* increasing values */
    dmg = (double)pwr;
    dmg *= (double)atk;
    dmg *= (double)mult;
    dmg /= (double)dcpm;

    /* decreasing values */
    dmg *= (double)acpm;
    dmg /= (double)def;
    dmg /= (double)2;

    return dmg + 1.0;
}


double damage2(int pwr, int atk, int def, float acpm, float dcpm, double mult) {

    float true_atk = (float)atk * acpm;
    float true_def = (float)def * dcpm;
    float true_mult = (float)mult;

    double dmg = (0.5) * (double)pwr * ((double)true_atk / (double)true_def) * (double)true_mult;

    return dmg + 1.0;
}


int main(void) {

    float cpm_20 = 0.5974;
    float cpm_34 = 0.7556855f;
    float cpm_50 = 0.8403f;

    double all_mults = 2.72999990613882824501930424578249810956501341386569286621;

    fprintf(stderr, "\n== Calculating damage with multiplier %.16f ==\n", all_mults);

    double d; /* damage */
    d = damage(9, 185 + 15, 244 + 15, cpm_34, cpm_20, all_mults);
    fprintf(stderr, "Mothim: %.16f (%d int)\n", d, (int)floor(d));

    d = damage(4, 135 + 15, 244 + 15, cpm_34, cpm_20, all_mults);
    fprintf(stderr, "Rat bite: %.16f (%d int)\n", d, (int)floor(d));

    d = damage(5, 135 + 15, 244 + 15, cpm_34, cpm_20, all_mults);
    fprintf(stderr, "Rat quick: %.16f (%d int)\n", d, (int)floor(d));

    /*double d2_all_mults = (double)((float)1.3f * (float)1.2f * ((float)1.0f / (float)0.5714286f));*/
    double d2_all_mults = (double)(((float)1.3f * (float)1.2f) / (float)0.5714286f);

    fprintf(stderr, "\n== Calculating damage2 with multiplier %.16f ==\n", d2_all_mults);

    d = damage2(9, 185 + 15, 244 + 15, cpm_34, cpm_20, d2_all_mults);
    fprintf(stderr, "Mothim: %.16f (%d int)\n", d, (int)floor(d));

    d = damage2(4, 135 + 15, 244 + 15, cpm_34, cpm_20, d2_all_mults);
    fprintf(stderr, "Rat bite: %.16f (%d int)\n", d, (int)floor(d));

    d = damage2(5, 135 + 15, 244 + 15, cpm_34, cpm_20, d2_all_mults);
    fprintf(stderr, "Rat quick: %.16f (%d int)\n", d, (int)floor(d));

    return 0;
}




