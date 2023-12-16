#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include <assert.h>
#include <fenv.h>
#include <fpu_control.h>

#include "pokemon_data.h"

union fu {
    float f;
    uint32_t u;
};

union du {
    double d;
    uint64_t u;
};



void print_double(double d, const char *s);
uint32_t f_to_bin(float f);
uint64_t d_to_bin(double d);
double get_cpm(int level, int half);
double cp_double(double cpm, int atk, int def, int hp);
double cp_double_2(double cpm, int atk, int def, int hp);
long double cp_longdouble(double cpm, int atk, int def, int hp);
void check_known_cps();
void check_specific_cp(double cmp, const char *name, int truecp, int atk, int def, int hp, int a_iv, int d_iv, int h_iv);

int main(void) {

    fpu_control_t cw;

    /*float f_d1 = 0.8f;*/
    /* print_double((double)f_d1); */

    double cpm385;
    double cpm345;
    double ramp_cp_d;

    cw = 0x27f; /* Double precision */
    _FPU_SETCW(cw);
    _FPU_GETCW(cw);
    fprintf(stderr, "FPU CW at start: 0x%x\n", cw);

    check_known_cps();
    exit(0);

    cpm385 = get_cpm(38, 1);
    /*double ramp_cp_f = cp_double((float)cpm385, 295 + 11, 109 + 15, 219 + 14);*/
    ramp_cp_d = cp_double_2(cpm385, 295 + 11, 109 + 15, 219 + 14);
    ramp_cp_d = cp_double_2(0.78179006870666101316229714617593731046, 295 + 11, 109 + 15, 219 + 14);


    fprintf(stderr, "Level 38.5 CPM\n");
    print_double(cpm385, "CPM 38.5");

    fprintf(stderr, "Level 38.5 CPM\n");
    cpm345 = get_cpm(34, 1);
    print_double(cpm345, "CPM 34.5");

    fprintf(stderr, "Rampardos CP using double CPM\n");
    print_double(ramp_cp_d, "Ramp CP");

    /*fprintf(stderr, "Rampardos CP using float CPM\n");
      print_double(ramp_cp_f);*/

    for (int l = 1; l <= 50; l++) {
        double hCPM = get_cpm(l, 1);
        for (int p = 1; p < P_COUNT; p++) {
            for (int a_iv = 0; a_iv <= 15; a_iv++) {
                for (int d_iv = 0; d_iv <= 15; d_iv++) {
                    for (int h_iv = 0; h_iv <= 15; h_iv++) {
                        int a = p_stats[p][0] + a_iv;
                        int d = p_stats[p][1] + d_iv;
                        int h = p_stats[p][2] + h_iv;

                        long double cp_ld = cp_longdouble((double)((float)hCPM), a, d, h);
                        double cp_d = cp_double(hCPM, a, d, h);
                        double cp_d2 = cp_double_2(hCPM, a, d, h);
                        /*float cp_f = (float)cp_double(hCPM, a, d, h);*/
                        (void)cp_d;
                        (void)cp_d2;

                        long double intmiss = fabsl(cp_ld - roundl(cp_ld));
                        (void)intmiss;
                        if (intmiss < 0.0001) {
                            fprintf(stderr, "%.10Lf miss for %s at level %d.5 for %d/%d/%d: %.10Lf (ld) vs %d (int) CP\n",
                                    intmiss, p_names[p], l, a_iv, d_iv, h_iv, cp_ld, (int)roundl(cp_ld));
                        }


                        /*if ((int)floor(cp_d) != (int)floor(cp_d2)) {
                            fprintf(stderr, "Found CP mismatch for %s at level %d.5 for %d/%d/%d: %d (d) vs %d (ds) CP\n",
                                    p_names[p], l, a_iv, d_iv, h_iv, (int)floor(cp_d), (int)floor(cp_d2));
                                    }*/
                    }
                }
            }
        }
    }

    _FPU_GETCW(cw);
    fprintf(stderr, "FPU CW at exit: 0x%x\n", cw);

    return 0;
}


void check_known_cps() {

    check_specific_cp(get_cpm(38, 1), "Rampardos 11/15/14 @38.5", 3179, 295, 109, 219, 11, 15, 14);
    check_specific_cp(get_cpm(17, 1), "Whishcash 15/15/15 @17.5", 1037, 151, 141, 242, 15, 15, 15);
    check_specific_cp(get_cpm(48, 1), "Ho-Oh 15/14/15 @48.5", 4281, 239, 244, 214, 15, 14, 15);
    check_specific_cp(get_cpm(13, 1), "Gurdurr 15/15/15 @13.5", 836, 180, 134, 198, 15, 15, 15);
    check_specific_cp(get_cpm(33, 1), "Crobat 15/15/15 @33.5", 2401, 194, 178, 198, 15, 15, 15);
    check_specific_cp(get_cpm(37, 1), "Mothim 15/15/15 @37.5", 1750, 185, 98, 172, 15, 15, 15);
    check_specific_cp(get_cpm(34, 1), "Swadloon 15/15/15 @34.5", 1263, 115, 146, 162, 15, 15, 15);
    check_specific_cp(get_cpm(34, 1), "Ryhperior 15/15/14 @34.5", 3434, 241, 190, 251, 15, 15, 14);
    check_specific_cp(get_cpm(40, 1), "Gible 15/15/15 @34.5", 1120, 124, 84, 151, 15, 15, 15);
    check_specific_cp(get_cpm(1, 1), "Delibird 15/15/15 @1.5", 32, 128, 90, 128, 15, 15, 15);

}

void check_specific_cp(double cpm, const char *name, int truecp, int atk, int def, int hp, int a_iv, int d_iv, int h_iv) {

    double cp;
    int foundcp;
    cp = cp_double(cpm, atk + a_iv, def + d_iv, hp + h_iv);
    foundcp = (int)floor(cp);
    printf("CP Check: %s: True CP %d; Calculated CP %d (%.15f); %s\n", name, truecp, foundcp, cp, (truecp == foundcp)? "PASS" : "FAIL");
}



void print_double(double d, const char *s) {

    fprintf(stderr, "Double %s: %.30f (0x%16lx)\n", s, d, d_to_bin(d));
    /*fprintf(stderr, "Double %s as float: %.30f (0x%8x)\n", s, (float)(d), f_to_bin((float)d));*/}


uint32_t f_to_bin(float f) {
    union fu fu = {.f = f};

    return fu.u;
}

uint64_t d_to_bin(double d) {
    union du du = {.d = d};

    return du.u;
}


double get_cpm(int level, int half) {
    double cpm;

    assert((level >= 1) && (level <= 55));
    assert((half == 0) || (level < 55));

    if (half == 0) {
        cpm = (double)CPM[level - 1];
    } else {
        cpm = sqrt((pow((double)CPM[level - 1], 2.0) + pow((double)CPM[level], 2.0)) / 2.0);
    }

    return cpm;
}


double cp_double(double cpm, int atk, int def, int hp) {
    double cp;

    /* Truncate the CPM to a float.
     * This is needed for half-levels which
     * are computed as doubles but
     * used as floats for CP calculations.
     */
    double lcpm = (double)((float)cpm);

    /* Compute each stat as a doubles */
    double a = (double)atk * lcpm;
    double d = (double)def * lcpm;
    double h = (double)hp * lcpm;

    /*cp = (a * sqrt(d * h)) / 10.0;*/
    cp = (a * sqrt(d) * sqrt(h)) / 10.0;

    return cp;
}


long double cp_longdouble(double cpm, int atk, int def, int hp) {
    long double cp;

    cp = ((long double)atk * sqrtl((long double)(def * hp)) * (long double)cpm * (long double)cpm)  / (long double)10.0;
    /*cp = (((double)atk) * sqrt((double)def * (double)hp) * pow(cpm, 2.0)) / 10.0;*/

    return cp;
}


double cp_double_2(double cpm, int atk, int def, int hp) {
    double cp;

    /* cp = (((double)atk * cpm) * sqrt((double)def * cpm) * sqrt((double)hp * cpm)) / 10.0; */
    /* cp = (((double)atk * cpm) * sqrt((double)def * cpm / 10.0) * sqrt((double)hp * cpm / 10.0)); */
    /* cp = (((double)atk) * sqrt((double)def * (double)hp) * cpm * cpm) / 10.0; */
    /* cp = (((float)atk * (float)cpm) * sqrtf((float)def * (float)cpm) * sqrtf((float)hp * (float)cpm)) / 10.0f; */
    /*cp = sqrt((((double)atk * cpm) * ((double)atk * cpm) * ((double)def * cpm) * ((double)hp * cpm))) / 10.0;*/

    /*double a = (double)atk * cpm;
    double d = (double)def * cpm;
    double h = (double)hp * cpm;*/

    double lcpm = (double)((float)cpm);

    /*double a = ((double)atk * lcpm);
    double d = ((double)def * lcpm);
    double h = ((double)hp * lcpm);*/
    /*print_double(a, "Atk");
    print_double(d, "Def");
    print_double(h, "HP");*/

    /*cp = (a * sqrt(d * h)) / 10.0;*/
    /* cp = (a * sqrt(d * h)) / 10.0; */
    cp = ((double)atk * sqrt((double)(def * hp)) * lcpm * lcpm) / 10.0;

    return cp;
}
