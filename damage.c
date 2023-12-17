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
void print_float(float f, const char *s);
uint32_t f_to_bin(float f);
uint64_t d_to_bin(double d);
double get_cpm(int level, int half);
long double get_cpm_longdouble(int level, int half);
double get_cpm2(int level, int half);
double cp_double(double cpm, int atk, int def, int hp);
double cp_double_2(double cpm, int atk, int def, int hp);
long double cp_longdouble(double cpm, int atk, int def, int hp);
void check_known_cps();
void check_specific_cp(double cmp, const char *name, int truecp, int atk, int def, int hp, int a_iv, int d_iv, int h_iv);
float adjacent_float(float f, int n);


int main(void) {

    fpu_control_t cw;

    /*float f_d1 = 0.8f;*/
    /* print_double((double)f_d1); */

    double cpm385;
    double cpm345;
    double cpm15;
    double ramp_cp_d;

    cw = 0x27f; /* Double precision */
    /*_FPU_SETCW(cw);
    _FPU_GETCW(cw);
    fprintf(stderr, "FPU CW at start: 0x%x\n", cw);*/

    for (int i = 1; i < 50; i++) {
        double cpm_1 = get_cpm(i, 1);
        double cpm_2 = get_cpm2(i, 1);
        float cpm_f1 = (float)cpm_1;
        float cpm_f2 = (float)cpm_2;

        if (cpm_1 != cpm_2) {
            /*fprintf(stderr, "Mismatch in double CPM values at level %d.5: (%.20f) vs (%.20f):\n",
                    i, cpm_1, cpm_2);
            print_double(cpm_1, "cpm_1");
            print_double(cpm_2, "cpm_2");*/
        }

        if (cpm_f1 != cpm_f2) {
            /*fprintf(stderr, "Mismatch in float CPM values at level %d.5: (%.20f) vs (%.20f):\n",
                    i, cpm_f1, cpm_f2);
            print_float(cpm_f1, "cpm_f1");
            print_float(cpm_f2, "cpm_f2");*/
        }

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


    check_known_cps();
    /*exit(0);*/

    cpm385 = get_cpm(38, 1);
    /*double ramp_cp_f = cp_double((float)cpm385, 295 + 11, 109 + 15, 219 + 14);*/
    ramp_cp_d = cp_double_2(cpm385, 295 + 11, 109 + 15, 219 + 14);
    ramp_cp_d = cp_double_2(0.78179006870666101316229714617593731046, 295 + 11, 109 + 15, 219 + 14);


    print_double(cpm385, "CPM 38.5");
    print_float((float)cpm385, "CPM 38.5");

    cpm345 = get_cpm(34, 1);
    print_double(cpm345, "CPM 34.5");

    fprintf(stderr, "Level 1.5 CPM\n");
    cpm15 = get_cpm(1, 1);
    print_double(cpm15, "CPM 1.5");

    fprintf(stderr, "Rampardos CP using double CPM\n");
    print_double(ramp_cp_d, "Ramp CP");

    /*fprintf(stderr, "Rampardos CP using float CPM\n");
      print_double(ramp_cp_f);*/

    /*exit(0);*/
    for (int l = 1; l <= 50; l++) {
        double hCPM = (float)get_cpm(l, 1);
        double hCPM_2 = (float)get_cpm2(l, 1);
        (void)hCPM_2;
        /*double hCPM_2 = (double)adjacent_float((float)get_cpm2(l, 1), 1);*/
        for (int p = 1; p < P_COUNT; p++) {
            for (int a_iv = 0; a_iv <= 15; a_iv++) {
                for (int d_iv = 0; d_iv <= 15; d_iv++) {
                    for (int h_iv = 0; h_iv <= 15; h_iv++) {
                        int a = p_stats[p][0] + a_iv;
                        int d = p_stats[p][1] + d_iv;
                        int h = p_stats[p][2] + h_iv;

                        long double cp_ld = cp_longdouble((double)((float)hCPM), a, d, h);
                        double cp_d = cp_double(hCPM, a, d, h);
                        double cp_d2 = (double)cp_longdouble(hCPM, a, d, h);
                        /*double cp_d2 = cp_double_2(hCPM, a, d, h);*/
                        /*double cp_d2 = cp_double(hCPM_2, a, d, h);*/
                        /*float cp_f = (float)cp_double(hCPM, a, d, h);*/
                        (void)cp_d;
                        (void)cp_d2;

                        long double intmiss = fabsl(cp_ld - roundl(cp_ld));
                        (void)intmiss;
                        /*if (intmiss < 0.0001) {
                            fprintf(stderr, "%.10Lf miss for %s at level %d.5 for %d/%d/%d: %.10Lf (ld) vs %d (int) CP\n",
                                    intmiss, p_names[p], l, a_iv, d_iv, h_iv, cp_ld, (int)roundl(cp_ld));
                                    }*/


                        if ((int)floor(cp_d) != (int)floor(cp_d2)) {
                            fprintf(stderr, "Found CP mismatch for %s at level %d.5 for %d/%d/%d: %d (d) vs %d (ds) CP\n",
                                    p_names[p], l, a_iv, d_iv, h_iv, (int)floor(cp_d), (int)floor(cp_d2));
                        }
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
    check_specific_cp(get_cpm(40, 1), "Gible 15/15/15 @40.5", 1120, 124, 84, 151, 15, 15, 15);
    check_specific_cp(get_cpm(1, 1), "Delibird 15/15/15 @1.5", 32, 128, 90, 128, 15, 15, 15);
    check_specific_cp(get_cpm(26, 1), "Falinks 15/15/15 @26.5", 1784, 193, 170, 163, 15, 15, 15);
    check_specific_cp(get_cpm(32, 1), "Donphan 15/15/15 @32.5", 2691, 214, 185, 207, 15, 15, 15);

    check_specific_cp(get_cpm(29, 0), "Phantump 13/4/14 @29", 870, 125, 103, 125, 13, 4, 14);
    check_specific_cp(get_cpm(11, 0), "Murkrow 2/5/3 @11", 418, 175, 87, 155, 2, 5, 3);
    check_specific_cp(get_cpm(12, 0), "Bayleef 5/6/9 @12", 441, 122, 155, 155, 5, 6, 9);
    check_specific_cp(get_cpm(12, 0), "Cherrim 0/12/6 @12", 623, 170, 153, 172, 0, 12, 6);
    check_specific_cp(get_cpm(12, 0), "Medicham 6/9/9 @12", 441, 121, 152, 155, 6, 9, 9);
    check_specific_cp(get_cpm(12, 0), "Mightyena 6/6/15 @12", 608, 171, 132, 172, 6, 6, 15);
    check_specific_cp(get_cpm(12, 0), "Quilava 10/6/6 @12", 517, 158, 126, 151, 10, 6, 6);
    check_specific_cp(get_cpm(13, 0), "Dedenne 5/14/13 @13", 640, 164, 134, 167, 5, 14, 13);
    check_specific_cp(get_cpm(13, 0), "Deerling 5/15/13 @13", 387, 115, 100, 155, 5, 15, 13);
    check_specific_cp(get_cpm(13, 0), "Drifloon 3/12/3 @13", 387, 117, 80, 207, 3, 12, 3);
    check_specific_cp(get_cpm(13, 0), "Shroomish 6/5/13 @13", 258, 74, 110, 155, 6, 5, 13);
    check_specific_cp(get_cpm(13, 0), "Swablu 4/6/12 @13", 258, 76, 132, 128, 4, 6, 12);
    check_specific_cp(get_cpm(13, 0), "Swablu 4/8/10 @13", 258, 76, 132, 128, 4, 8, 10);
    check_specific_cp(get_cpm(16, 0), "Furfrou 3/12/11 @16", 884, 164, 167, 181, 3, 12, 11);
    check_specific_cp(get_cpm(16, 0), "Gothita 7/6/6 @16", 377, 98, 112, 128, 7, 6, 6);
    check_specific_cp(get_cpm(16, 0), "Gulpin 10/4/4 @16", 346, 80, 99, 172, 10, 4, 4);
    check_specific_cp(get_cpm(16, 0), "Murkrow 10/8/0 @16", 641, 175, 87, 155, 10, 8, 0);
    check_specific_cp(get_cpm(16, 0), "Scyther 7/1/1 @16", 1105, 218, 170, 172, 7, 1, 1);
    check_specific_cp(get_cpm(16, 0), "Tauros 1/1/11 @16", 1068, 198, 183, 181, 1, 1, 11);
    check_specific_cp(get_cpm(16, 0), "Trapinch 7/7/6 @16", 515, 162, 78, 128, 7, 7, 6);
    check_specific_cp(get_cpm(16, 0), "Ursaring 8/15/14 @16", 1306, 236, 144, 207, 8, 15, 14);
    check_specific_cp(get_cpm(17, 0), "Togetic 3/7/8 @17", 732, 139, 181, 146, 3, 7, 8);
    check_specific_cp(get_cpm(18, 0), "Amaura 13/3/12 @18", 652, 124, 109, 184, 13, 3, 12);
    check_specific_cp(get_cpm(18, 0), "Eevee 13/4/15 @18", 518, 104, 114, 146, 13, 4, 15);
    check_specific_cp(get_cpm(19, 0), "Litleo 8/4/14 @19", 704, 139, 112, 158, 8, 4, 14);
    check_specific_cp(get_cpm(19, 0), "Noibat 15/14/9 @19", 352, 83, 73, 120, 15, 14, 9);
    check_specific_cp(get_cpm(19, 0), "Zubat 15/14/9 @19", 352, 83, 73, 120, 15, 14, 9);
    check_specific_cp(get_cpm(20, 0), "Deerling 14/9/13 @20", 623, 115, 100, 155, 14, 9, 13);
    check_specific_cp(get_cpm(20, 0), "Kirlia 4/11/12 @20", 491, 117, 90, 116, 4, 11, 12);
    check_specific_cp(get_cpm(20, 0), "Lillipup 14/15/0 @20", 491, 107, 86, 128, 14, 15, 0);
    check_specific_cp(get_cpm(21, 0), "Nidoran_female 6/6/8 @21", 416, 86, 89, 146, 6, 6, 8);
    check_specific_cp(get_cpm(21, 0), "Sableye 13/2/4 @21", 804, 141, 136, 137, 13, 2, 4);
    check_specific_cp(get_cpm(21, 0), "Sableye 13/5/1 @21", 804, 141, 136, 137, 13, 5, 1);
    check_specific_cp(get_cpm(21, 0), "Swablu 12/9/10 @21", 459, 76, 132, 128, 12, 9, 10);
    check_specific_cp(get_cpm(21, 0), "Tauros 11/14/1 @21", 1482, 198, 183, 181, 11, 14, 1);
    check_specific_cp(get_cpm(21, 0), "Ursaring 13/4/3 @21", 1644, 236, 144, 207, 13, 4, 3);
    check_specific_cp(get_cpm(22, 0), "Buneary 9/13/4 @22", 726, 130, 105, 146, 9, 13, 4);
    check_specific_cp(get_cpm(22, 0), "Hitmontop 6/13/0 @22", 1220, 173, 207, 137, 6, 13, 0);
    check_specific_cp(get_cpm(22, 0), "Lopunny 14/5/0 @22", 1202, 156, 194, 163, 14, 5, 0);
    check_specific_cp(get_cpm(22, 0), "Nidoking 10/14/10 @22", 1553, 204, 156, 191, 10, 14, 10);
    check_specific_cp(get_cpm(22, 0), "Piplup 12/4/15 @22", 628, 112, 102, 142, 12, 4, 15);
    check_specific_cp(get_cpm(22, 0), "Teddiursa 13/13/2 @22", 785, 142, 93, 155, 13, 13, 2);
    check_specific_cp(get_cpm(22, 0), "Togetic 14/12/4 @22", 1022, 139, 181, 146, 14, 12, 4);
    check_specific_cp(get_cpm(23, 0), "Absol 15/11/9 @23", 1608, 246, 120, 163, 15, 11, 9);
    check_specific_cp(get_cpm(23, 0), "Deerling 10/0/7 @23", 653, 115, 100, 155, 10, 0, 7);
    check_specific_cp(get_cpm(23, 0), "Piplup 13/6/8 @23", 653, 112, 102, 142, 13, 6, 8);
    check_specific_cp(get_cpm(23, 0), "Stunfisk 6/9/10 @23", 1306, 144, 171, 240, 6, 9, 10);
    check_specific_cp(get_cpm(23, 0), "Stunky 4/10/2 @23", 653, 121, 90, 160, 4, 10, 2);
    check_specific_cp(get_cpm(24, 0), "Bagon 3/7/2 @24", 669, 134, 93, 128, 3, 7, 2);
    check_specific_cp(get_cpm(24, 0), "Espeon 14/6/7 @24", 2066, 261, 175, 163, 14, 6, 7);
    check_specific_cp(get_cpm(26, 0), "Chikorita 14/0/1 @26", 617, 92, 122, 128, 14, 0, 1);
    check_specific_cp(get_cpm(26, 0), "Croagunk 6/12/9 @26", 635, 116, 76, 134, 6, 12, 9);
    check_specific_cp(get_cpm(26, 0), "Cyndaquil 6/11/3 @26", 635, 116, 93, 118, 6, 11, 3);
    check_specific_cp(get_cpm(26, 0), "Frillish 12/15/3 @26", 878, 115, 134, 146, 12, 15, 3);
    check_specific_cp(get_cpm(26, 0), "Geodude 0/7/0 @26", 791, 132, 132, 120, 0, 7, 0);
    check_specific_cp(get_cpm(26, 0), "Gothita 8/10/1 @26", 617, 98, 112, 128, 8, 10, 1);
    check_specific_cp(get_cpm(26, 0), "Illumise 15/9/5 @26", 1257, 143, 166, 163, 15, 9, 5);
    check_specific_cp(get_cpm(26, 0), "Litleo 5/8/8 @26", 943, 139, 112, 158, 5, 8, 8);
    check_specific_cp(get_cpm(26, 0), "Nidorino 12/2/9 @26", 944, 137, 111, 156, 12, 2, 9);
    check_specific_cp(get_cpm(26, 0), "Nidorino 7/9/10 @26", 943, 137, 111, 156, 7, 9, 10);
    check_specific_cp(get_cpm(26, 0), "Nuzleaf 13/2/14 @26", 832, 134, 78, 172, 13, 2, 14);
    check_specific_cp(get_cpm(26, 0), "Pikachu 10/8/10 @26", 635, 112, 96, 111, 10, 8, 10);
    check_specific_cp(get_cpm(26, 0), "Piloswine 5/6/6 @26", 1574, 181, 138, 225, 5, 6, 6);
    check_specific_cp(get_cpm(26, 0), "Teddiursa 5/3/0 @26", 832, 142, 93, 155, 5, 3, 0);
    check_specific_cp(get_cpm(26, 0), "Vulpix 10/13/13 @26", 617, 96, 109, 116, 10, 13, 13);
    check_specific_cp(get_cpm(27, 0), "Dedenne 6/11/14 @27", 1327, 164, 134, 167, 6, 11, 14);
    check_specific_cp(get_cpm(28, 0), "Sableye 10/10/1 @28", 1071, 141, 136, 137, 10, 10, 1);
    check_specific_cp(get_cpm(28, 0), "Sableye 10/2/9 @28", 1071, 141, 136, 137, 10, 2, 9);
    check_specific_cp(get_cpm(29, 0), "Pawmi 2/5/10 @29", 416, 95, 45, 128, 2, 5, 10);
    check_specific_cp(get_cpm(29, 0), "Phantump 13/4/14 @29", 870, 125, 103, 125, 13, 4, 14);
    check_specific_cp(get_cpm(30, 0), "Shiftry 15/12/9 @30", 1951, 200, 121, 207, 15, 12, 9);
    check_specific_cp(get_cpm(30, 0), "Vigoroth 6/8/12 @30", 1553, 159, 145, 190, 6, 8, 12);
    check_specific_cp(get_cpm(31, 0), "Houndour 12/11/9 @31", 1012, 152, 83, 128, 12, 11, 9);
    check_specific_cp(get_cpm(31, 0), "Pikachu 9/7/6 @31", 722, 112, 96, 111, 9, 7, 6);
    check_specific_cp(get_cpm(31, 0), "Pineco 11/7/12 @31", 897, 108, 122, 137, 11, 7, 12);
    check_specific_cp(get_cpm(31, 0), "Togedemaru 13/4/10 @31", 1773, 190, 145, 163, 13, 4, 10);
    check_specific_cp(get_cpm(32, 0), "Slugma 4/4/11 @32", 669, 118, 71, 120, 4, 4, 11);
    check_specific_cp(get_cpm(34, 0), "Cyndaquil 7/13/15 @34", 834, 116, 93, 118, 7, 13, 15);
    check_specific_cp(get_cpm(34, 0), "Furfrou 11/8/5 @34", 1803, 164, 167, 181, 11, 8, 5);
    check_specific_cp(get_cpm(34, 0), "Stantler 7/9/4 @34", 1809, 192, 131, 177, 7, 9, 4);
    check_specific_cp(get_cpm(5, 0), "Pikipek 4/14/6 @5", 109, 136, 59, 111, 4, 14, 6);
    check_specific_cp(get_cpm(8, 0), "Murkrow 8/10/7 @8", 322, 175, 87, 155, 8, 10, 7);
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

    if (half == 0) {
        cpm = ll;
    } else {
        /*cpm = sqrtl((ll * ll + lh * lh) / 2.0L);*/
        cpm = sqrtl((long double)((fl * fl + fh * fh) / 2.0f));
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
        /*cpm = sqrt((ll * ll + lh * lh) / 2.0);*/

        /* Quadratic Mean Float */
        cpm = sqrt((double)((fll * fll + flh * flh) / 2.0f));

        /* Quadratic Mean Float sqrtf */
        /*cpm = (double)sqrtf(((fll * fll + flh * flh) / 2.0f));*/

        /* Gamepress algorithm
         * https://gamepress.gg/pokemongo/cp-multiplier
         */
        /*
        if (level < 10) {
            float cpm_step = 0.009426125469f;
            cpm = sqrt(ll * ll + (double)cpm_step);
        } else if ((level >= 10) && (level < 20)) {
            float cpm_step = 0.008919025675f;
            cpm = sqrt(ll * ll + (double)cpm_step);
        } else if ((level >= 20) && (level < 30)) {
            float cpm_step = 0.008924905903f;
            cpm = sqrt(ll * ll + (double)cpm_step);
        } else if ((level >= 30) && (level < 40)) {
            float cpm_step = 0.00445946079f;
            cpm = sqrt(ll * ll + (double)cpm_step);
        } else {
            cpm = sqrt((ll * ll + lh * lh) / 2.0);
        }
        */
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
    float lcpm_f = (float)cpm;
    (void)lcpm;
    /*double lcpm = (double)(adjacent_float((float)cpm, 1));*/

    /* Compute each stat as a floats */
    float a = (float)atk * lcpm_f;
    float d = (float)def * lcpm_f;
    float h = (float)hp * lcpm_f;

    /*double a = ((double)atk * lcpm);
    double d = ((double)def * lcpm);
    double h = ((double)hp * lcpm);*/
    /*print_double(a, "Atk");
    print_double(d, "Def");
    print_double(h, "HP");*/

    /*cp = (a * sqrt(d * h)) / 10.0;*/
    /* cp = (a * sqrt(d * h)) / 10.0; */
    /*cp = ((double)atk * sqrt((double)(def * hp)) * lcpm * lcpm) / 10.0;*/

    cp = ((double)a * sqrt((double)d) * sqrt((double)h)) / 10.0;

    return cp;
}


float adjacent_float(float f, int n) {
    union fu fu = {.f = f};

    /* Adjust +/- n steps */
    fu.u += n;

    return fu.f;
}
