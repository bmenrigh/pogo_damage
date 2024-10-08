#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include <assert.h>
#include <fenv.h>
/*#include <fpu_control.h>*/

#include "pokemon_data.h"

union fu {
    float f;
    uint32_t u;
};

union du {
    double d;
    uint64_t u;
};


double cpm_range[55][2][2]; /* level, full/half, min/max */


void print_double(double d, const char *s);
void print_float(float f, const char *s);
uint32_t f_to_bin(float f);
uint64_t d_to_bin(double d);
double get_cpm(int level, int half);
long double get_cpm_longdouble(int level, int half);
double get_cpm2(int level, int half);

int compare_cp_calc_whole_level(int l, int atk, int def, int hp);
double cp_double_simple(double cpm, int atk, int def, int hp);
double cp_double(double cpm, int atk, int def, int hp);
double cp_double_2(double cpm, int atk, int def, int hp);
long double cp_longdouble(double cpm, int atk, int def, int hp);
/*void check_known_cps();*/
void check_known_cps(double (*cpm_func)(int,int), double (*cp_func)(double,int,int,int));
void check_specific_cp(double (*cpm_func)(int,int), double (*cp_func)(double,int,int,int), int l, int half, const char *name, int truecp, int atk, int def, int hp, int a_iv, int d_iv, int h_iv);
float adjacent_float(float f, int n);
double adjacent_double(double d, long n);
double solve_cpm_from_cp(double cp, int atk, int def, int hp);
void add_true_cp(int l, int half, double (*cp_func)(double,int,int,int), const char *name, int truecp, int atk, int def, int hp, int a_iv, int d_iv, int h_iv);


int main(void) {

    /* init cpm range min/max */
    for (int l = 0; l < 55; l++) {
        /* Whole levels known exactly */
        cpm_range[l][0][0] = (double)CPM[l];
        cpm_range[l][0][1] = (double)CPM[l];

        /* Half levels known to be between full levels to start */
        if (l < 54) {
            cpm_range[l][1][0] = (double)CPM[l];
            cpm_range[l][1][1] = (double)CPM[l + 1];
        } else {
            /* There is no level 55.5 */
            cpm_range[l][1][0] = -1.0;
            cpm_range[l][1][1] = -1.0;
        }
    }


    /*float f_d1 = 0.8f;*/
    /* print_double((double)f_d1); */

    /*for (int i = 0; i < 55; i++) {
        fprintf(stderr, "%.016f, # %d\n", (double)CPM[i], i + 1);
    }
    exit(0);*/

    /* fpu_control_t cw; */
    /* cw = 0x27f; /\* Double precision *\/ */
    /* /\*_FPU_SETCW(cw);*\/ */
    /* _FPU_GETCW(cw); */
    /* fprintf(stderr, "FPU CW at start: 0x%x\n", cw); */

    for (int i = 1; i < 50; i++) {
        double cpm_1 = get_cpm(i, 1);
        double cpm_2 = get_cpm2(i, 1);
        float cpm_f1 = (float)cpm_1;
        float cpm_f2 = (float)cpm_2;
        (void)cpm_f1;
        (void)cpm_f2;

        /*if (cpm_1 != cpm_2) {
            fprintf(stderr, "Mismatch in double CPM values at level %d.5: (%.20f) vs (%.20f):\n",
                    i, cpm_1, cpm_2);
            print_double(cpm_1, "cpm_1");
            print_double(cpm_2, "cpm_2");
            }*/

        /*  if (cpm_f1 != cpm_f2) { */
        /*     fprintf(stderr, "Mismatch in float CPM values at level %d.5: (%.20f) vs (%.20f):\n", */
        /*             i, cpm_f1, cpm_f2); */
        /*     print_float(cpm_f1, "cpm_f1"); */
        /*     print_float(cpm_f2, "cpm_f2"); */
        /* } */

        long double cpm_ld = get_cpm_longdouble(i, 1);
        long double cpm_ld_f = (long double)((float)cpm_ld);
        long double cpm_ld_l, cpm_ld_h;
        if (cpm_ld_f < cpm_ld) {
            cpm_ld_l = cpm_ld_f;
            cpm_ld_h = (long double)adjacent_float((float)cpm_ld_l, 1);
        } else {
            cpm_ld_h = cpm_ld_f;
            cpm_ld_l = (long double)adjacent_float((float)cpm_ld_h, -1);
        }
        long double gap_l = cpm_ld - cpm_ld_l;
        long double gap_h = cpm_ld_h - cpm_ld;

        double f_dist;
        if (gap_l < gap_h) {
            f_dist = (double)(gap_l / (gap_l + gap_h));
        } else {
            f_dist = (double)(gap_h / (gap_l + gap_h));
        }
        (void)f_dist;

        /*fprintf(stderr, "Half-level %d.5 distance from nearest float: %.03f%%\n", i, f_dist * 100.0);*/
    }
    /*exit(0);*/


    check_known_cps(&get_cpm, &cp_double_simple);
    exit(0);

    /*double cpm385;
    double cpm345;
    double cpm15;
    double ramp_cp_d;*/

    /*cpm385 = get_cpm(38, 1);*/
    /*double ramp_cp_f = cp_double((float)cpm385, 295 + 11, 109 + 15, 219 + 14);*/
    /*ramp_cp_d = cp_double_2(cpm385, 295 + 11, 109 + 15, 219 + 14);
    ramp_cp_d = cp_double_2(0.78179006870666101316229714617593731046, 295 + 11, 109 + 15, 219 + 14);


    print_double(cpm385, "CPM 38.5");
    print_float((float)cpm385, "CPM 38.5");*/

    /* double cpm345 = get_cpm(34, 1); */
    /* print_double(cpm345, "CPM 34.5"); */
    /* print_float((float)cpm345, "CPM 34.5"); */

    /*fprintf(stderr, "Level 1.5 CPM\n");
    cpm15 = get_cpm(1, 1);
    print_double(cpm15, "CPM 1.5");

    fprintf(stderr, "Rampardos CP using double CPM\n");
    print_double(ramp_cp_d, "Ramp CP");*/

    /*fprintf(stderr, "Rampardos CP using float CPM\n");
      print_double(ramp_cp_f);*/

    /*exit(0);*/
    for (int l = 1; l <= 50; l++) {
        for (int half = 0; half <= 1; half++) {
            double hCPM, hCPM_2;
            /*double hCPMd = get_cpm(l, half);*/
            long double hCPMld = get_cpm_longdouble(l, half);
            (void)hCPMld;
            (void)hCPM_2;

            if ((long double)((float)hCPMld) < hCPMld) {
                hCPM = (double)((float)hCPMld);
                hCPM_2 = (double)adjacent_float((float)hCPMld, 1);
            }
            else {
                hCPM = (double)((float)hCPMld);
                hCPM_2 = (double)adjacent_float((float)hCPMld, -1);
            }
            hCPM = get_cpm(l, half);
            /*hCPM_2 = get_cpm2(l, half);*/

            /*double hCPM_2 = (double)adjacent_float((float)get_cpm2(l, half), 1);*/
            for (int p = 1; p < P_COUNT; p++) {
                for (int a_iv = 0; a_iv <= 15; a_iv++) {
                    for (int d_iv = 0; d_iv <= 15; d_iv++) {
                        for (int h_iv = 0; h_iv <= 15; h_iv++) {
                            int a = p_stats[p][0] + a_iv;
                            int d = p_stats[p][1] + d_iv;
                            int h = p_stats[p][2] + h_iv;

                            if (half == 0) {
                                int cp_cmp = compare_cp_calc_whole_level(l, a, d, h);
                                if (cp_cmp != 0) {
                                    fprintf(stderr, "Mismatch for %s %d/%d/%d\n", p_names[p], a_iv, d_iv, h_iv);
                                }
                                (void)cp_cmp;
                            }

                            long double cp_ld = cp_longdouble((double)((float)hCPM), a, d, h);
                            double cp_d = cp_double(hCPM, a, d, h);
                            double cp_d2 = cp_double(hCPM, a, d, h);
                            /*double cp_d2 = (double)cp_longdouble(hCPM, a, d, h);*/
                            /*double cp_d2 = cp_double_2(hCPM, a, d, h);*/
                            /*double cp_d2 = cp_double(hCPM_2, a, d, h);*/
                            /*double cp_d2 = (double)cp_longdouble(hCPM, a, d, h);*/
                            /*float cp_f = (float)cp_double(hCPM, a, d, h);*/

                            if (half == 1) {
                                cp_d2 = (cp_double(get_cpm(l, 0), a, d, h) + cp_double(get_cpm(l + 1, 0), a, d, h)) / 2.0;
                            }

                            (void)cp_d;
                            (void)cp_d2;

                            long double intmiss = fabsl(cp_ld - roundl(cp_ld));
                            (void)intmiss;
                            if (intmiss < 0.00001) {
                                /*fprintf(stderr, "%.16Lf miss for %s at level %d%s for %d/%d/%d: %.16Lf (ld) vs %d (int) CP\n",
                                  intmiss, p_names[p], l, (half == 0)? "" : ".5", a_iv, d_iv, h_iv, cp_ld, (int)roundl(cp_ld));*/
                            }

                            if ((int)floor(cp_d) != (int)floor(cp_d2)) {
                                /* fprintf(stderr, "Found CP mismatch for %s at level %d%s for %d/%d/%d: %d (d) vs %d (ds) CP\n", */
                                /*         p_names[p], l, (half == 0)? "" : ".5", a_iv, d_iv, h_iv, (int)floor(cp_d), (int)floor(cp_d2)); */
                            }
                        }
                    }
                }
            }
        }
    }

    /* _FPU_GETCW(cw); */
    /* fprintf(stderr, "FPU CW at exit: 0x%x\n", cw); */

    return 0;
}


void check_known_cps(double (*cpm_func)(int,int), double (*cp_func)(double,int,int,int)) {

    check_specific_cp(cpm_func, cp_func, 1, 1, "Delibird", 32, 128, 90, 128, 15, 15, 15);
    check_specific_cp(cpm_func, cp_func, 2, 1, "Avalugg", 181, 196, 240, 216, 11, 4, 14);
    check_specific_cp(cpm_func, cp_func, 4, 1, "Amaura", 141, 124, 109, 184, 8, 0, 3);
    check_specific_cp(cpm_func, cp_func, 5, 1, "Cetitan", 456, 208, 123, 347, 15, 12, 6);
    check_specific_cp(cpm_func, cp_func, 6, 1, "Galvantula", 395, 201, 128, 172, 15, 14, 14);
    check_specific_cp(cpm_func, cp_func, 6, 1, "Eevee", 183, 104, 114, 146, 10, 14, 13);
    check_specific_cp(cpm_func, cp_func, 7, 1, "Cryogonal", 526, 190, 218, 190, 2, 5, 5);
    check_specific_cp(cpm_func, cp_func, 8, 1, "Sandshrew (Alola)", 273, 125, 129, 137, 5, 4, 11);
    check_specific_cp(cpm_func, cp_func, 9, 1, "Seel", 241, 85, 121, 163, 8, 14, 11);
    check_specific_cp(cpm_func, cp_func, 10, 1, "Cetoddle", 338, 119, 80, 239, 7, 5, 2);
    check_specific_cp(cpm_func, cp_func, 10, 1, "Fennekin", 291, 116, 102, 120, 9, 14, 13);
    check_specific_cp(cpm_func, cp_func, 11, 1, "Granbull", 767, 212, 131, 207, 5, 6, 10);
    check_specific_cp(cpm_func, cp_func, 13, 1, "Gurdurr", 836, 180, 134, 198, 15, 15, 15);
    check_specific_cp(cpm_func, cp_func, 14, 1, "Dewgong", 792, 139, 177, 207, 11, 11, 15);
    check_specific_cp(cpm_func, cp_func, 15, 1, "Lickitung", 624, 108, 137, 207, 15, 15, 15);
    check_specific_cp(cpm_func, cp_func, 16, 1, "Eevee", 429, 104, 114, 146, 7, 4, 0);
    check_specific_cp(cpm_func, cp_func, 16, 1, "Stantler", 978, 192, 131, 177, 11, 14, 8);
    check_specific_cp(cpm_func, cp_func, 16, 1, "Dewgong", 892, 139, 177, 207, 9, 13, 14);
    check_specific_cp(cpm_func, cp_func, 16, 1, "Koffing", 528, 119, 141, 120, 11, 10, 6);
    check_specific_cp(cpm_func, cp_func, 16, 1, "Spheal", 392, 95, 90, 172, 12, 0, 0);
    check_specific_cp(cpm_func, cp_func, 17, 1, "Whiscash", 1037, 151, 141, 242, 15, 15, 15);
    check_specific_cp(cpm_func, cp_func, 18, 1, "Eevee", 482, 104, 114, 146, 2, 10, 7);
    check_specific_cp(cpm_func, cp_func, 18, 1, "Spheal", 455, 95, 90, 172, 7, 12, 7);
    check_specific_cp(cpm_func, cp_func, 19, 1, "Stantler", 1116, 192, 131, 177, 9, 3, 13);
    check_specific_cp(cpm_func, cp_func, 20, 1, "Raichu", 1243, 193, 151, 155, 15, 11, 10);
    check_specific_cp(cpm_func, cp_func, 21, 1, "Eevee", 617, 104, 114, 146, 9, 13, 14);
    check_specific_cp(cpm_func, cp_func, 21, 1, "Stantler", 1233, 192, 131, 177, 15, 0, 7);
    check_specific_cp(cpm_func, cp_func, 21, 1, "Cetitan", 1812, 208, 123, 347, 9, 8, 15);
    check_specific_cp(cpm_func, cp_func, 21, 1, "Gulpin", 456, 80, 99, 172, 6, 10, 4);
    check_specific_cp(cpm_func, cp_func, 21, 1, "Swalot", 1145, 140, 159, 225, 13, 5, 7);
    check_specific_cp(cpm_func, cp_func, 21, 1, "Sneasel", 1083, 189, 146, 146, 0, 5, 2);
    check_specific_cp(cpm_func, cp_func, 22, 1, "Eevee", 572, 104, 114, 146, 2, 0, 13);
    check_specific_cp(cpm_func, cp_func, 22, 1, "Spheal", 605, 95, 90, 172, 13, 15, 14);
    check_specific_cp(cpm_func, cp_func, 23, 1, "Seel", 573, 85, 121, 163, 6, 14, 4);
    check_specific_cp(cpm_func, cp_func, 24, 1, "Hoppip", 336, 67, 94, 111, 3, 3, 14);
    check_specific_cp(cpm_func, cp_func, 25, 1, "Vulpix", 548, 96, 109, 116, 5, 7, 7);
    check_specific_cp(cpm_func, cp_func, 26, 1, "Falinks", 1784, 193, 170, 163, 15, 15, 15);
    check_specific_cp(cpm_func, cp_func, 27, 1, "Spheal", 690, 95, 90, 172, 12, 7, 6);
    check_specific_cp(cpm_func, cp_func, 27, 1, "Sentret", 467, 79, 73, 111, 15, 11, 11);
    check_specific_cp(cpm_func, cp_func, 27, 1, "Cetoddle", 879, 119, 80, 239, 0, 14, 2);
    check_specific_cp(cpm_func, cp_func, 27, 1, "Snubbull", 874, 137, 85, 155, 6, 9, 10);
    check_specific_cp(cpm_func, cp_func, 27, 1, "Swalot", 1490, 140, 159, 225, 14, 5, 12);
    check_specific_cp(cpm_func, cp_func, 28, 1, "Grubbin", 725, 115, 85, 132, 11, 8, 6);
    check_specific_cp(cpm_func, cp_func, 29, 1, "Dewgong", 1623, 139, 177, 207, 11, 15, 13);
    check_specific_cp(cpm_func, cp_func, 30, 1, "Sandslash (Alola)", 1881, 177, 195, 181, 5, 1, 6);
    check_specific_cp(cpm_func, cp_func, 30, 1, "Cryogonal", 2253, 190, 218, 190, 7, 2, 14);
    check_specific_cp(cpm_func, cp_func, 31, 1, "Spheal", 778, 95, 90, 172, 10, 13, 5);
    check_specific_cp(cpm_func, cp_func, 32, 1, "Donphan", 2691, 214, 185, 207, 15, 15, 15);
    check_specific_cp(cpm_func, cp_func, 33, 1, "Crobat", 2401, 194, 178, 198, 15, 15, 15);
    check_specific_cp(cpm_func, cp_func, 34, 1, "Swadloon", 1263, 115, 146, 162, 15, 15, 15);
    check_specific_cp(cpm_func, cp_func, 34, 1, "Rhyperior", 3434, 241, 190, 251, 15, 15, 14);
    check_specific_cp(cpm_func, cp_func, 35, 1, "Dewgong", 1697, 139, 177, 207, 4, 13, 10);
    check_specific_cp(cpm_func, cp_func, 36, 1, "Cryogonal", 2336, 190, 218, 190, 3, 1, 0);
    check_specific_cp(cpm_func, cp_func, 37, 1, "Mothim", 1750, 185, 98, 172, 15, 15, 15);
    check_specific_cp(cpm_func, cp_func, 37, 1, "Spheal", 861, 95, 90, 172, 11, 13, 5);
    check_specific_cp(cpm_func, cp_func, 38, 1, "Rampardos", 3179, 295, 109, 219, 11, 15, 14);
    check_specific_cp(cpm_func, cp_func, 39, 1, "Spheal", 890, 95, 90, 172, 12, 8, 12);
    check_specific_cp(cpm_func, cp_func, 39, 1, "Spheal", 854, 95, 90, 172, 8, 9, 9);
    check_specific_cp(cpm_func, cp_func, 39, 1, "Joltik", 923, 110, 98, 137, 6, 12, 13);
    check_specific_cp(cpm_func, cp_func, 40, 1, "Gible", 1120, 124, 84, 151, 15, 15, 15);
    check_specific_cp(cpm_func, cp_func, 40, 1, "Mankey", 1050, 148, 82, 120, 12, 4, 7);
    check_specific_cp(cpm_func, cp_func, 41, 1, "Darmanitan", 3009, 263, 114, 233, 9, 14, 3);
    check_specific_cp(cpm_func, cp_func, 41, 1, "Spheal", 863, 95, 90, 172, 7, 5, 14);
    check_specific_cp(cpm_func, cp_func, 42, 1, "Darmumaka", 1399, 153, 86, 172, 10, 10, 13);
    check_specific_cp(cpm_func, cp_func, 42, 1, "Spheal", 916, 95, 90, 172, 11, 11, 6);
    check_specific_cp(cpm_func, cp_func, 43, 1, "Stantler", 2089, 192, 131, 177, 10, 7, 5);
    check_specific_cp(cpm_func, cp_func, 43, 1, "Bunnelby", 481, 68, 72, 116, 9, 7, 0);
    check_specific_cp(cpm_func, cp_func, 44, 1, "Swalot", 1838, 140, 159, 225, 4, 4, 4);
    check_specific_cp(cpm_func, cp_func, 45, 1, "Vanillite", 1106, 118, 106, 113, 15, 15, 15);
    check_specific_cp(cpm_func, cp_func, 45, 1, "Bellsprout", 1106, 139, 61, 137, 15, 15, 15);
    check_specific_cp(cpm_func, cp_func, 45, 1, "Gulpin", 748, 80, 99, 172, 1, 9, 5);
    check_specific_cp(cpm_func, cp_func, 46, 1, "Stantler", 2262, 192, 131, 177, 12, 14, 8);
    check_specific_cp(cpm_func, cp_func, 47, 1, "Stantler", 2143, 192, 131, 177, 1, 15, 3);
    check_specific_cp(cpm_func, cp_func, 48, 1, "Mewtwo", 4619, 300, 182, 214, 15, 15, 13);
    check_specific_cp(cpm_func, cp_func, 48, 1, "Ho-Oh", 4281, 239, 244, 214, 15, 14, 15);
    check_specific_cp(cpm_func, cp_func, 49, 1, "Seel", 1006, 85, 121, 163, 14, 4, 5);
    check_specific_cp(cpm_func, cp_func, 49, 1, "Seel", 992, 85, 121, 163, 8, 9, 15);
    check_specific_cp(cpm_func, cp_func, 49, 1, "Weezing", 2427, 174, 197, 163, 9, 7, 12);
    check_specific_cp(cpm_func, cp_func, 49, 1, "Patrat", 854, 98, 73, 128, 15, 10, 12);
    check_specific_cp(cpm_func, cp_func, 49, 1, "Eevee", 1166, 104, 114, 146, 13, 13, 13);
    check_specific_cp(cpm_func, cp_func, 49, 1, "Granbull", 2658, 212, 131, 207, 10, 7, 4);
    check_specific_cp(cpm_func, cp_func, 49, 1, "Spheal", 1008, 95, 90, 172, 13, 13, 0);
    check_specific_cp(cpm_func, cp_func, 49, 1, "Furret", 1777, 148, 125, 198, 4, 6, 14);

    check_specific_cp(cpm_func, cp_func, 5, 0, "Pikipek", 109, 136, 59, 111, 4, 14, 6);
    check_specific_cp(cpm_func, cp_func, 8, 0, "Murkrow", 322, 175, 87, 155, 8, 10, 7);
    check_specific_cp(cpm_func, cp_func, 11, 0, "Murkrow", 418, 175, 87, 155, 2, 5, 3);
    check_specific_cp(cpm_func, cp_func, 12, 0, "Bayleef", 441, 122, 155, 155, 5, 6, 9);
    check_specific_cp(cpm_func, cp_func, 12, 0, "Cherrim", 623, 170, 153, 172, 0, 12, 6);
    check_specific_cp(cpm_func, cp_func, 12, 0, "Medicham", 441, 121, 152, 155, 6, 9, 9);
    check_specific_cp(cpm_func, cp_func, 12, 0, "Mightyena", 608, 171, 132, 172, 6, 6, 15);
    check_specific_cp(cpm_func, cp_func, 12, 0, "Quilava", 517, 158, 126, 151, 10, 6, 6);
    check_specific_cp(cpm_func, cp_func, 13, 0, "Shroomish", 258, 74, 110, 155, 6, 10, 6);
    check_specific_cp(cpm_func, cp_func, 13, 0, "Dedenne", 640, 164, 134, 167, 5, 14, 13);
    check_specific_cp(cpm_func, cp_func, 13, 0, "Deerling", 387, 115, 100, 155, 5, 15, 13);
    check_specific_cp(cpm_func, cp_func, 13, 0, "Drifloon", 387, 117, 80, 207, 3, 12, 3);
    check_specific_cp(cpm_func, cp_func, 13, 0, "Shroomish", 258, 74, 110, 155, 6, 5, 13);
    check_specific_cp(cpm_func, cp_func, 13, 0, "Swablu", 258, 76, 132, 128, 4, 6, 12);
    check_specific_cp(cpm_func, cp_func, 13, 0, "Swablu", 258, 76, 132, 128, 4, 8, 10);
    check_specific_cp(cpm_func, cp_func, 15, 0, "Togetic", 664, 139, 181, 146, 5, 5, 14);
    check_specific_cp(cpm_func, cp_func, 16, 0, "Fomantis", 302, 100, 64, 120, 6, 12, 11);
    check_specific_cp(cpm_func, cp_func, 16, 0, "Nidoran_male", 302, 105, 76, 130, 1, 0, 1);
    check_specific_cp(cpm_func, cp_func, 16, 0, "Furfrou", 884, 164, 167, 181, 3, 12, 11);
    check_specific_cp(cpm_func, cp_func, 16, 0, "Gothita", 377, 98, 112, 128, 7, 6, 6);
    check_specific_cp(cpm_func, cp_func, 16, 0, "Gulpin", 346, 80, 99, 172, 10, 4, 4);
    check_specific_cp(cpm_func, cp_func, 16, 0, "Murkrow", 641, 175, 87, 155, 10, 8, 0);
    check_specific_cp(cpm_func, cp_func, 16, 0, "Scyther", 1105, 218, 170, 172, 7, 1, 1);
    check_specific_cp(cpm_func, cp_func, 16, 0, "Tauros", 1068, 198, 183, 181, 1, 1, 11);
    check_specific_cp(cpm_func, cp_func, 16, 0, "Trapinch", 515, 162, 78, 128, 7, 7, 6);
    check_specific_cp(cpm_func, cp_func, 16, 0, "Ursaring", 1306, 236, 144, 207, 8, 15, 14);
    check_specific_cp(cpm_func, cp_func, 17, 0, "Togetic", 732, 139, 181, 146, 3, 7, 8);
    check_specific_cp(cpm_func, cp_func, 18, 0, "Swalot", 954, 140, 159, 225, 13, 7, 2);
    check_specific_cp(cpm_func, cp_func, 18, 0, "Amaura", 652, 124, 109, 184, 13, 3, 12);
    check_specific_cp(cpm_func, cp_func, 18, 0, "Eevee", 518, 104, 114, 146, 13, 4, 15);
    check_specific_cp(cpm_func, cp_func, 19, 0, "Litleo", 704, 139, 112, 158, 8, 4, 14);
    check_specific_cp(cpm_func, cp_func, 19, 0, "Noibat", 352, 83, 73, 120, 15, 14, 9);
    check_specific_cp(cpm_func, cp_func, 19, 0, "Zubat", 352, 83, 73, 120, 15, 14, 9);
    check_specific_cp(cpm_func, cp_func, 20, 0, "Deerling", 623, 115, 100, 155, 14, 9, 13);
    check_specific_cp(cpm_func, cp_func, 20, 0, "Kirlia", 491, 117, 90, 116, 4, 11, 12);
    check_specific_cp(cpm_func, cp_func, 20, 0, "Lillipup", 491, 107, 86, 128, 14, 15, 0);
    check_specific_cp(cpm_func, cp_func, 21, 0, "Nidoran_female", 416, 86, 89, 146, 6, 6, 8);
    check_specific_cp(cpm_func, cp_func, 21, 0, "Sableye", 804, 141, 136, 137, 13, 2, 4);
    check_specific_cp(cpm_func, cp_func, 21, 0, "Sableye", 804, 141, 136, 137, 13, 5, 1);
    check_specific_cp(cpm_func, cp_func, 21, 0, "Swablu", 459, 76, 132, 128, 12, 9, 10);
    check_specific_cp(cpm_func, cp_func, 21, 0, "Tauros", 1482, 198, 183, 181, 11, 14, 1);
    check_specific_cp(cpm_func, cp_func, 21, 0, "Ursaring", 1644, 236, 144, 207, 13, 4, 3);
    check_specific_cp(cpm_func, cp_func, 22, 0, "Buneary", 726, 130, 105, 146, 9, 13, 4);
    check_specific_cp(cpm_func, cp_func, 22, 0, "Hitmontop", 1220, 173, 207, 137, 6, 13, 0);
    check_specific_cp(cpm_func, cp_func, 22, 0, "Lopunny", 1202, 156, 194, 163, 14, 5, 0);
    check_specific_cp(cpm_func, cp_func, 22, 0, "Nidoking", 1553, 204, 156, 191, 10, 14, 10);
    check_specific_cp(cpm_func, cp_func, 22, 0, "Piplup", 628, 112, 102, 142, 12, 4, 15);
    check_specific_cp(cpm_func, cp_func, 22, 0, "Teddiursa", 785, 142, 93, 155, 13, 13, 2);
    check_specific_cp(cpm_func, cp_func, 22, 0, "Togetic", 1022, 139, 181, 146, 14, 12, 4);
    check_specific_cp(cpm_func, cp_func, 22, 0, "Magcargo", 1022, 139, 191, 137, 14, 2, 13);
    check_specific_cp(cpm_func, cp_func, 23, 0, "Absol", 1608, 246, 120, 163, 15, 11, 9);
    check_specific_cp(cpm_func, cp_func, 23, 0, "Deerling", 653, 115, 100, 155, 10, 0, 7);
    check_specific_cp(cpm_func, cp_func, 23, 0, "Piplup", 653, 112, 102, 142, 13, 6, 8);
    check_specific_cp(cpm_func, cp_func, 23, 0, "Stunfisk", 1306, 144, 171, 240, 6, 9, 10);
    check_specific_cp(cpm_func, cp_func, 23, 0, "Stunky", 653, 121, 90, 160, 4, 10, 2);
    check_specific_cp(cpm_func, cp_func, 24, 0, "Bagon", 669, 134, 93, 128, 3, 7, 2);
    check_specific_cp(cpm_func, cp_func, 24, 0, "Espeon", 2066, 261, 175, 163, 14, 6, 7);
    check_specific_cp(cpm_func, cp_func, 26, 0, "Blissey", 1865, 129, 169, 496, 9, 0, 6);
    check_specific_cp(cpm_func, cp_func, 26, 0, "Girafarig", 1433, 182, 133, 172, 13, 12, 1);
    check_specific_cp(cpm_func, cp_func, 26, 0, "Roggenrola", 848, 121, 110, 146, 14, 6, 12);
    check_specific_cp(cpm_func, cp_func, 26, 0, "Teddiursa", 832, 142, 93, 155, 5, 0, 5);
    check_specific_cp(cpm_func, cp_func, 26, 0, "Throh", 1769, 172, 160, 260, 9, 10, 1);
    check_specific_cp(cpm_func, cp_func, 26, 0, "Chikorita", 617, 92, 122, 128, 14, 0, 1);
    check_specific_cp(cpm_func, cp_func, 26, 0, "Croagunk", 635, 116, 76, 134, 6, 12, 9);
    check_specific_cp(cpm_func, cp_func, 26, 0, "Cyndaquil", 635, 116, 93, 118, 6, 11, 3);
    check_specific_cp(cpm_func, cp_func, 26, 0, "Frillish", 878, 115, 134, 146, 12, 15, 3);
    check_specific_cp(cpm_func, cp_func, 26, 0, "Geodude", 791, 132, 132, 120, 0, 7, 0);
    check_specific_cp(cpm_func, cp_func, 26, 0, "Gothita", 617, 98, 112, 128, 8, 10, 1);
    check_specific_cp(cpm_func, cp_func, 26, 0, "Illumise", 1257, 143, 166, 163, 15, 9, 5);
    check_specific_cp(cpm_func, cp_func, 26, 0, "Litleo", 943, 139, 112, 158, 5, 8, 8);
    check_specific_cp(cpm_func, cp_func, 26, 0, "Nidorino", 944, 137, 111, 156, 12, 2, 9);
    check_specific_cp(cpm_func, cp_func, 26, 0, "Nidorino", 943, 137, 111, 156, 7, 9, 10);
    check_specific_cp(cpm_func, cp_func, 26, 0, "Nuzleaf", 832, 134, 78, 172, 13, 2, 14);
    check_specific_cp(cpm_func, cp_func, 26, 0, "Pikachu", 635, 112, 96, 111, 10, 8, 10);
    check_specific_cp(cpm_func, cp_func, 26, 0, "Piloswine", 1574, 181, 138, 225, 5, 6, 6);
    check_specific_cp(cpm_func, cp_func, 26, 0, "Teddiursa", 832, 142, 93, 155, 5, 3, 0);
    check_specific_cp(cpm_func, cp_func, 26, 0, "Vulpix", 617, 96, 109, 116, 10, 13, 13);
    check_specific_cp(cpm_func, cp_func, 27, 0, "Dedenne", 1327, 164, 134, 167, 6, 11, 14);
    check_specific_cp(cpm_func, cp_func, 28, 0, "Sableye", 1071, 141, 136, 137, 10, 10, 1);
    check_specific_cp(cpm_func, cp_func, 28, 0, "Sableye", 1071, 141, 136, 137, 10, 2, 9);
    check_specific_cp(cpm_func, cp_func, 29, 0, "Phantump", 870, 125, 103, 125, 13, 4, 14);
    check_specific_cp(cpm_func, cp_func, 29, 0, "Pawmi", 416, 95, 45, 128, 2, 5, 10);
    check_specific_cp(cpm_func, cp_func, 29, 0, "Phantump", 870, 125, 103, 125, 13, 4, 14);
    check_specific_cp(cpm_func, cp_func, 30, 0, "Shiftry", 1951, 200, 121, 207, 15, 12, 9);
    check_specific_cp(cpm_func, cp_func, 30, 0, "Vigoroth", 1553, 159, 145, 190, 6, 8, 12);
    check_specific_cp(cpm_func, cp_func, 31, 0, "Houndour", 1012, 152, 83, 128, 12, 11, 9);
    check_specific_cp(cpm_func, cp_func, 31, 0, "Pikachu", 722, 112, 96, 111, 9, 7, 6);
    check_specific_cp(cpm_func, cp_func, 31, 0, "Pineco", 897, 108, 122, 137, 11, 7, 12);
    check_specific_cp(cpm_func, cp_func, 31, 0, "Togedemaru", 1773, 190, 145, 163, 13, 4, 10);
    check_specific_cp(cpm_func, cp_func, 31, 0, "Xurkitree", 3414, 330, 144, 195, 15, 15, 13);
    check_specific_cp(cpm_func, cp_func, 32, 0, "Golbat", 1718, 161, 150, 181, 15, 13, 10);
    check_specific_cp(cpm_func, cp_func, 32, 0, "Slugma", 669, 118, 71, 120, 4, 4, 11);
    check_specific_cp(cpm_func, cp_func, 34, 0, "Cyndaquil", 834, 116, 93, 118, 7, 13, 15);
    check_specific_cp(cpm_func, cp_func, 34, 0, "Furfrou", 1803, 164, 167, 181, 11, 8, 5);
    check_specific_cp(cpm_func, cp_func, 34, 0, "Stantler", 1809, 192, 131, 177, 7, 9, 4);
    check_specific_cp(cpm_func, cp_func, 40, 0, "Articuno", 3045, 192, 236, 207, 15, 15, 14);

}

void check_specific_cp(double (*cpm_func)(int,int), double (*cp_func)(double,int,int,int), int l, int half, const char *name, int truecp, int atk, int def, int hp, int a_iv, int d_iv, int h_iv) {

    if (half == 1) {
        add_true_cp(l, half, cp_func, name, truecp, atk, def, hp, a_iv, d_iv, h_iv);
    }

    double cp;
    double cp_l, cp_h;
    int foundcp;
    if (half == 0) {
        cp = (*cp_func)((*cpm_func)(l, 0), atk + a_iv, def + d_iv, hp + h_iv);
    } else {
        cp_l = (*cp_func)((*cpm_func)(l, 0), atk + a_iv, def + d_iv, hp + h_iv);
        cp_h = (*cp_func)((*cpm_func)(l + 1, 0), atk + a_iv, def + d_iv, hp + h_iv);

        /*cp = ceil((floor(cp_l) + floor(cp_h)) / 2.0);*/
        cp = (cp_h + cp_l) / 2.0;
    }

    foundcp = (int)floor(cp);
    if (foundcp != truecp) {
        if (half == 0) {
            printf("CP Check: %s %d/%d/%d @%d%s: True CP %d; Calculated CP %d (%.15f); %s\n", name, a_iv, d_iv, h_iv, l, (half == 1)? ".5" : "", truecp, foundcp, cp, (truecp == foundcp)? "PASS" : "FAIL");
        } else {
            printf("CP Check: %s %d/%d/%d @%d%s: True CP %d; Calculated CP %d (%.15f) [%0.15f, %.015f]; %s\n", name, a_iv, d_iv, h_iv, l, (half == 1)? ".5" : "", truecp, foundcp, cp, cp_l, cp_h, (truecp == foundcp)? "PASS" : "FAIL");
        }
    }
}


void add_true_cp(int l, int half, double (*cp_func)(double,int,int,int), const char *name, int truecp, int atk, int def, int hp, int a_iv, int d_iv, int h_iv) {

    if (half == 0) {
        fprintf(stderr, "Can not add true cp for full-level!\n");
        return;
    }

    double min_cp = (double)truecp;
    double max_cp = adjacent_double((double)(truecp + 1), -1);

    double min_cpm = solve_cpm_from_cp(min_cp, atk + a_iv, def + d_iv, hp + h_iv);
    double max_cpm = solve_cpm_from_cp(max_cp, atk + a_iv, def + d_iv, hp + h_iv);

    int min_down = 0;
    while ((int)floor((*cp_func)(min_cpm, atk + a_iv, def + d_iv, hp + h_iv)) < truecp) {
        min_cpm = adjacent_double(min_cpm, 1);
        min_down--;
    }
    while ((int)floor((*cp_func)(adjacent_double(min_cpm, -1), atk + a_iv, def + d_iv, hp + h_iv)) == truecp) {
        min_cpm = adjacent_double(min_cpm, -1);
        min_down++;
    }

    int max_up = 0;
    while ((int)floor((*cp_func)(max_cpm, atk + a_iv, def + d_iv, hp + h_iv)) > truecp) {
        max_cpm = adjacent_double(max_cpm, -1);
        max_up--;
    }
    while ((int)floor((*cp_func)(adjacent_double(max_cpm, 1), atk + a_iv, def + d_iv, hp + h_iv)) == truecp) {
        max_cpm = adjacent_double(max_cpm, 1);
        max_up++;
    }

    if ((min_down > 0) || (max_up > 0)) {
        fprintf(stderr, "Expanded min/max by %d/%d\n", min_down, max_up);
    }

    fprintf(stderr, "Computed CPM range [%.015f, %.015f]\n", min_cpm, max_cpm);

    double cur_gap = cpm_range[l - 1][half][1] - cpm_range[l - 1][half][0];

    if (min_cpm > cpm_range[l - 1][half][0]) {
        cpm_range[l - 1][half][0] = min_cpm;
    }
    if (max_cpm < cpm_range[l - 1][half][1]) {
        cpm_range[l - 1][half][1] = max_cpm;
    }

    double new_gap = cpm_range[l - 1][half][1] - cpm_range[l - 1][half][0];

    if (new_gap < 0.0) {
        fprintf(stderr, "Impossible CPM Range caused by %s %d/%d/%d @%d%s with %d CP\n", name, a_iv, d_iv, h_iv, l, (half == 1)? ".5" : "", truecp);
        return;
    }

    if (new_gap < cur_gap) {
        double old_bits = -1.0 * log(cur_gap) / log(2.0);
        double new_bits = -1.0 * log(new_gap) / log(2.0);

        fprintf(stderr, "CPM Range improved by %s %d/%d/%d @%d%s with %d CP: +%.03f bits (%.03f -> %.03f)\n", name, a_iv, d_iv, h_iv, l, (half == 1)? ".5" : "", truecp, (new_bits - old_bits), old_bits, new_bits);
    }
}


void print_double(double d, const char *s) {

    fprintf(stderr, "Double %s: %.30f (0x%16llx)\n", s, d, d_to_bin(d));
    /*fprintf(stderr, "Double %s as float: %.30f (0x%8x)\n", s, (float)(d), f_to_bin((float)d));*/
}

void print_float(float f, const char *s) {

    fprintf(stderr, "Float %s: %.30f (0x%08x)\n", s, f, f_to_bin(f));
    /*fprintf(stderr, "Double %s as float: %.30f (0x%8x)\n", s, (float)(d), f_to_bin((float)d));*/
}


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

        float fll = (float)CPM[level - 1];
        float flh = (float)CPM[level];

        cpm = sqrt((double)((fll * fll + flh * flh) / 2.0f));
    }

    return cpm;
}



double get_cpm2(int level, int half) {
    double cpm;

    assert((level >= 1) && (level <= 55));
    assert((half == 0) || (level < 55));

    if (half == 0) {
        cpm = (double)CPM[level - 1];
    } else {

        double ll = (double)CPM[level - 1];
        double lh = (double)CPM[level];
        (void)ll;
        (void)lh;
        float fll = (float)CPM[level - 1];
        float flh = (float)CPM[level];
        (void)fll;
        (void)flh;

        /* Quadratic Mean */
        cpm = sqrt((ll * ll + lh * lh) / 2.0);
        /*cpm = sqrt((((ll * ll) / 2.0) + ((lh * lh) / 2.0)));*/

        /*cpm = pow(((pow(ll, 2.0) + pow(lh, 2.0)) / 2.0), 0.5);*/

        /* Quadratic Mean Float */
        /*cpm = sqrt((double)((fll * fll + flh * flh) / 2.0f));*/

        /* Quadratic Mean Float 2 */
        /*cpm = sqrt((double)((fll / 2.0f) * fll + (flh / 2.0f) * flh));*/

        /* Quadratic Mean double -> float*/
        /*cpm = sqrt((double)((float)((ll * ll + lh * lh) / 2.0)));*/

        /* Quadratic Mean Float sqrtf */
        /*cpm = (double)sqrtf(((fll * fll + flh * flh) / 2.0f));*/

        /* Gamepress algorithm
         * https://gamepress.gg/pokemongo/cp-multiplier
         */
        /* Gamepress correction
         * https://www.reddit.com/r/TheSilphRoad/comments/bmslle/gamepress_article_on_cpm_values_provides_wrong/
         */

        /*
        if (level < 10) {
            float cpm_step = 0.009426125469f;
            cpm = sqrt((double)(fll * fll + cpm_step));
        } else if ((level >= 10) && (level < 20)) {
            float cpm_step = 0.008919025675f;
            cpm = sqrt((double)(fll * fll + cpm_step));
        } else if ((level >= 20) && (level < 30)) {
            float cpm_step = 0.008924905903f;
            cpm = sqrt((double)(fll * fll + cpm_step));
        } else if ((level >= 30) && (level < 40)) {
            float cpm_step = 0.00445946079f;
            cpm = sqrt(fll * fll + cpm_step);
        } else {
            cpm = sqrt((double)((fll * fll + flh * flh) / 2.0f));
        }
        */
    }


    return cpm;
}


long double get_cpm_longdouble(int level, int half) {
    long double cpm;

    assert((level >= 1) && (level <= 55));
    assert((half == 0) || (level < 55));

    long double ll = (long double)CPM[level - 1];
    long double lh = (long double)CPM[level];
    (void)ll;
    (void)lh;
    float fl = (float)CPM[level - 1];
    float fh = (float)CPM[level];
    (void)fl;
    (void)fh;

    if (half == 0) {
        cpm = ll;
    } else {
        cpm = sqrtl((ll * ll + lh * lh) / 2.0L);
        /*cpm = sqrtl((long double)((fl * fl + fh * fh) / 2.0f));*/
    }

    return cpm;
}


int compare_cp_calc_whole_level(int l, int atk, int def, int hp) {

    double cpm = get_cpm(l, 0);

    double d_a = (double)atk * cpm;
    double d_d = (double)def * cpm;
    double d_h = (double)hp * cpm;

    float f_cpm = (float)cpm;

    float f_a = (float)atk * f_cpm;
    float f_d = (float)def * f_cpm;
    float f_h = (float)hp * f_cpm;

    /* Simple */
    int cp_simple = (int)floor((d_a * sqrt(d_d) * sqrt(d_h)) / 10.0);

    /* Combined square root */
    int cp_dh_combo = (int)floor((d_a * sqrt(d_d * d_h)) / 10.0);

    /* Factored cpm */
    int cp_factored = (int)floor(((double)atk * sqrt((double)def * (double)hp) * (cpm * cpm)) / 10.0);

    /* Zoé Cassiopée RE of arm32 */
    /* Version 0.327.0 does (f32(atk_base * atk_iv) * cpm) * sqrtf((f32(def_base + def_iv) * cpm) * (f32(sta_base + sta_iv) * cpm)) * 0.1 (returns float) */
    int cp_arm32 = (int)floor(f_a * sqrtf(f_d * f_h) * 0.1f);

    /* Arm64 guess (based on arm32) */
    int cp_arm64_guess = (int)floor(d_a * sqrt(d_d * d_h) * 0.1);

    /*if ((cp_simple != cp_dh_combo) || (cp_simple != cp_factored) || (cp_simple != cp_arm32) || (cp_simple != cp_arm64_guess)) {*/
    if ((cp_simple != cp_dh_combo) || (cp_simple != cp_factored) || (cp_simple != cp_arm64_guess)) {
        fprintf(stderr, "CP calculation missmatch for %d/%d/%d @%d: "
                "Simple: %d; "
                "Combo DH: %d (%d); "
                "Factored: %d (%d); "
                "Arm32: %d (%d) "
                "Arm64 guess: %d (%d)\n",
                atk, def, hp, l, cp_simple,
                cp_dh_combo, cp_simple - cp_dh_combo,
                cp_factored, cp_simple - cp_factored,
                cp_arm32, cp_simple - cp_arm32,
                cp_arm64_guess, cp_simple - cp_arm64_guess);

        return 1;
    }

    return 0;
}


double cp_double_simple(double cpm, int atk, int def, int hp) {

    double lcpm = cpm;

    /* Compute each stat as a doubles */
    double a = (double)atk * lcpm;
    double d = (double)def * lcpm;
    double h = (double)hp * lcpm;

    /*cp = (a * sqrt(d * h)) / 10.0;*/
    double cp = (a * sqrt(d * h)) * 0.1;

    return cp;
}


double cp_double(double cpm, int atk, int def, int hp) {
    double cp;

    /* Truncate the CPM to a float.
     * This is needed for half-levels which
     * are computed as doubles but
     * used as floats for CP calculations.
     */
    double lcpm = (double)((float)cpm);
    /*double lcpm = cpm;*/

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

    cp = ((long double)atk * sqrtl((long double)(def * hp)) * (long double)cpm * (long double)cpm)  / (long double)10.0L;
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

    double a = (double)atk * cpm;
    double d = (double)def * cpm;
    double h = (double)hp * cpm;

    double lcpm = (double)((float)cpm);
    float lcpm_f = (float)cpm;
    (void)lcpm;
    (void)lcpm_f;
    /*double lcpm = (double)(adjacent_float((float)cpm, 1));*/

    /* Compute each stat as a floats */
    /* float a = (float)atk * lcpm_f; */
    /* float d = (float)def * lcpm_f; */
    /* float h = (float)hp * lcpm_f; */

    /*double a = ((double)atk * lcpm);
    double d = ((double)def * lcpm);
    double h = ((double)hp * lcpm);*/
    /*print_double(a, "Atk");
    print_double(d, "Def");
    print_double(h, "HP");*/

    /*cp = (a * sqrt(d * h)) / 10.0;*/
    /* cp = (a * sqrt(d * h)) / 10.0; */
    /*cp = ((double)atk * sqrt((double)(def * hp)) * lcpm * lcpm) / 10.0;*/

    /*cp = ((double)a * sqrt((double)d) * sqrt((double)h)) / 10.0;*/
    cp = ((double)a * sqrt((double)d * (double)h)) * 0.1;
    /*cp = (sqrt((double)(a * a * d * h))) / 10.0;*/

    return cp;
}

double solve_cpm_from_cp(double cp, int atk, int def, int hp) {

    double x = (double)cp / 0.1;
    /*double x = (double)cp * 10.0;*/
    x = x / ((double)atk * sqrt((double)(def * hp)));

    double cpm = (double)sqrt(x);

    return cpm;
}


double solve_cpm_from_cp_ld(double cp, int atk, int def, int hp) {

    long double x = (long double)cp * 10.0L;
    x = x / ((long double)atk * sqrtl((long double)(def * hp)));

    double cpm = (double)sqrtl(x);

    return cpm;
}


float adjacent_float(float f, int n) {
    union fu fu = {.f = f};

    /* Adjust +/- n steps */
    fu.u += n;

    return fu.f;
}


double adjacent_double(double d, long n) {
    union du du = {.d = d};

    /* Adjust +/- n steps */
    du.u += n;

    return du.d;
}
