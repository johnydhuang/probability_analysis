/************************************************************
 * prob_analysis.c -- program to print out "prob_analysis.c". *
 *                                                          *
 * Author:  Johnny                                          *
 *                                                          *
 * Purpose:                                                 *
 *                                                          *
 * Usage:                                                   *
 *                                                          *
 ***********************************************************/

#include "prob_analysis.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE *pFile_success_improvement;
	FILE *pFile_fail_improvement;
	FILE *pFile_original_GAT_LAA;
	FILE *pFile_original_GAT_WIFI;

	int scenario_relation[16][4];
	double scenario_prob[16];
	double wifi_dl_p = 0.45 , wifi_ul_p = 0.45 ;
	double laa_dl_p = 0.45 , laa_ul_p = 0.45 ;
	double ack_fail = 0.1;
	double wifi_idle_p = 1 - wifi_dl_p - wifi_ul_p;
	double laa_idle_p = 1 - laa_dl_p - laa_ul_p;
	double laa_original_prob[4][4] = {0}; // [laa][wifi]   success fail csma idle
	double laa_LBR_prob[4][4] = {0};
	double laa_LBR_GAT_prob[P_NUMBER][4][4] = {0};
	double wifi_original_prob[4][4] = {0}; // [laa][wifi]
	double wifi_LBR_prob[4][4] = {0};
	double wifi_LBR_GAT_prob[4][4] = {0};
	double haveToken_prob = 0;
	double p_demo = 0.3 * P_NUMBER ;
	double p_persistent = 0;
	double enbs_wifi_DC_info[16];
	double scenario_prob_test[16] = {0};
	int i , j , k , t , n;
	srand(time(NULL));
	pFile_success_improvement = fopen("p_09_success.txt", "w");
	pFile_fail_improvement = fopen("p_09_fail.txt", "w");
	pFile_original_GAT_LAA = fopen("p_09_LAA_GAT.txt", "w");
	pFile_original_GAT_WIFI = fopen("p_09_WIFI_GAT.txt", "w");
	if (pFile_success_improvement == NULL || pFile_fail_improvement == NULL) {
		printf("open failure");
		return 1;
	}


	scenario_relation[0][0] = 0; scenario_relation[0][1] = 0; scenario_relation[0][2] = 0; scenario_relation[0][3] = 0;
	scenario_relation[1][0] = 1; scenario_relation[1][1] = 0; scenario_relation[1][2] = 0; scenario_relation[1][3] = 0;
	scenario_relation[2][0] = 0; scenario_relation[2][1] = 1; scenario_relation[2][2] = 0; scenario_relation[2][3] = 0;
	scenario_relation[3][0] = 0; scenario_relation[3][1] = 0; scenario_relation[3][2] = 1; scenario_relation[3][3] = 0;
	scenario_relation[4][0] = 0; scenario_relation[4][1] = 0; scenario_relation[4][2] = 0; scenario_relation[4][3] = 1;
	scenario_relation[5][0] = 1; scenario_relation[5][1] = 1; scenario_relation[5][2] = 0; scenario_relation[5][3] = 0;
	scenario_relation[6][0] = 1; scenario_relation[6][1] = 0; scenario_relation[6][2] = 1; scenario_relation[6][3] = 0;
	scenario_relation[7][0] = 0; scenario_relation[7][1] = 1; scenario_relation[7][2] = 0; scenario_relation[7][3] = 1;
	scenario_relation[8][0] = 0; scenario_relation[8][1] = 0; scenario_relation[8][2] = 1; scenario_relation[8][3] = 1;
	scenario_relation[9][0] = 1; scenario_relation[9][1] = 0; scenario_relation[9][2] = 0; scenario_relation[9][3] = 1;
	scenario_relation[10][0] = 0; scenario_relation[10][1] = 1; scenario_relation[10][2] = 1; scenario_relation[10][3] = 0;
	scenario_relation[11][0] = 1; scenario_relation[11][1] = 1; scenario_relation[11][2] = 1; scenario_relation[11][3] = 0;
	scenario_relation[12][0] = 1; scenario_relation[12][1] = 1; scenario_relation[12][2] = 0; scenario_relation[12][3] = 1;
	scenario_relation[13][0] = 1; scenario_relation[13][1] = 0; scenario_relation[13][2] = 1; scenario_relation[13][3] = 1;
	scenario_relation[14][0] = 0; scenario_relation[14][1] = 1; scenario_relation[14][2] = 1; scenario_relation[14][3] = 1;
	scenario_relation[15][0] = 1; scenario_relation[15][1] = 1; scenario_relation[15][2] = 1; scenario_relation[15][3] = 1;

	scenario_prob[0] = 0 ;
	scenario_prob[1] = 9.69 ;
	scenario_prob[2] = 9.69 ;
	scenario_prob[3] = 9.69 ;
	scenario_prob[4] = 9.69 ;
	scenario_prob[5] = 6.9 ;
	scenario_prob[6] = 6.9 ;
	scenario_prob[7] = 6.9 ;
	scenario_prob[8] = 6.9 ;
	scenario_prob[9] = 0.285 ;
	scenario_prob[10] = 0.285 ;
	scenario_prob[11] = 4.69 ;
	scenario_prob[12] = 4.69 ;
	scenario_prob[13] = 4.69 ;
	scenario_prob[14] = 4.69 ;
	scenario_prob[15] = 14.31 ;

	// scenario_prob[0] = 0 ;
	// scenario_prob[1] = 0 ;
	// scenario_prob[2] = 0 ;
	// scenario_prob[3] = 0;
	// scenario_prob[4] = 0 ;
	// scenario_prob[5] = 0 ;
	// scenario_prob[6] = 0 ;
	// scenario_prob[7] = 0 ;
	// scenario_prob[8] = 0 ;
	// scenario_prob[9] = 0 ;
	// scenario_prob[10] = 100 ;
	// scenario_prob[11] = 0 ;
	// scenario_prob[12] = 0 ;
	// scenario_prob[13] = 0 ;
	// scenario_prob[14] = 0 ;
	// scenario_prob[15] = 0 ;

	for (i = 0 ; i < 16; i++) {
		double info = 0;
		if (scenario_relation[i][0] == 1)
			info += wifi_dl_p;
		if (scenario_relation[i][2] == 1)
			info += wifi_ul_p;
		enbs_wifi_DC_info[i] = info;
		printf("%d : %.2f\n", i, enbs_wifi_DC_info[i]);
	}
	int token = 0;
	double test = 0;
	double pt = 0 ;
	int rand_scenario , f_laa , f_wifi; //when f_laa == 0 means dl , 1 means ul ,2 means idle
	for (n = 0 ; n < SIMU_NUMBER ; n++ ) {
		double tmp;
		tmp = rand() % 100000  + 1;
		tmp /= 1000;
		for (i = 0 ; i < 16 ; i++) {
			tmp -= scenario_prob[i];
			if (tmp < 0) {
				rand_scenario = i;
				break;
			}
		}
		tmp = rand() % 100000  + 1;
		tmp /= 100000;
		if (tmp < laa_dl_p) {
			f_laa = 0 ;
		} else if (tmp < laa_dl_p + laa_ul_p) {
			f_laa = 1 ;
			test ++;
		} else {
			f_laa = 2 ;
		}
		tmp = rand() % 100000  + 1;
		tmp /= 100000;
		if (tmp < wifi_dl_p) {
			f_wifi = 0 ;
		} else if (tmp < wifi_dl_p + wifi_ul_p) {
			f_wifi = 1 ;
		} else {
			f_wifi = 2 ;
		}

		if (f_laa == 0) {
			if (f_wifi == 0) {
				if (scenario_relation[rand_scenario][0] != 1 && scenario_relation[rand_scenario][1] == 1) {
					if (token < TOKEN_LIMIT)
						token ++;
				}
				else if (scenario_relation[rand_scenario][0] == 1 && scenario_relation[rand_scenario][1] == 0) {
					if (token != 0) {
						token -- ;
					}
				}
			} else if (f_wifi == 1) {
				if (scenario_relation[rand_scenario][2] != 1 && scenario_relation[rand_scenario][3] == 1) {
					if (token < TOKEN_LIMIT)
						token ++;
				}
				else if (scenario_relation[rand_scenario][2] == 1 && scenario_relation[rand_scenario][3] == 0) {
					if (token != 0) {
						token -- ;
					}
				}
			}
		} else if (f_laa == 1) {
			if (f_wifi == 0) {
				if (scenario_relation[rand_scenario][1] != 1 && scenario_relation[rand_scenario][0] == 1) {
					if (token < TOKEN_LIMIT)
						token ++;
				}
				else if (scenario_relation[rand_scenario][1] == 1 && scenario_relation[rand_scenario][0] == 0) {
					if (token != 0) {
						token -- ;
					}
				}
			} else if (f_wifi == 1) {
				if (scenario_relation[rand_scenario][3] != 1 && scenario_relation[rand_scenario][2] == 1) {
					if (token < TOKEN_LIMIT)
						token ++;
				}
				else if (scenario_relation[rand_scenario][3] == 1 && scenario_relation[rand_scenario][2] == 0) {
					if (token != 0) {
						token -- ;
					}
				}
			}
		}
		//printf("t = %d\n", token);
		if (token > 0) {
			pt++;
		}
	}
	printf("test = %f\n", test / SIMU_NUMBER);
	printf("p_token = %f\n", pt / SIMU_NUMBER * 100);
	haveToken_prob = pt / SIMU_NUMBER;
	// printf("rand_scenario = %d\n", rand_scenario);

	//laa original prob
	for (i = 0 ; i < 16 ; i ++) {
		for (j = 0 ; j < 3 ; j ++) {
			for (k = 0 ; k < 3 ; k ++) {
				if (j == 0) { 		//when laa what to dl.
					if (k == 0) { 		//when wifi is dling.
						if (scenario_relation[i][0] == 1)
							laa_original_prob[2][0] += laa_dl_p * wifi_dl_p * scenario_prob[i];
						else if (scenario_relation[i][2] == 1 && scenario_relation[i][1] == 0)
							laa_original_prob[0][1] += laa_dl_p * wifi_dl_p * scenario_prob[i];
						else if (scenario_relation[i][2] == 1 && scenario_relation[i][1] == 1)
							laa_original_prob[1][1] += laa_dl_p * wifi_dl_p * scenario_prob[i];
						else if (scenario_relation[i][2] == 0 && scenario_relation[i][1] == 1)
							laa_original_prob[1][0] += laa_dl_p * wifi_dl_p * scenario_prob[i];
						else if (scenario_relation[i][2] == 0 && scenario_relation[i][1] == 0)
							laa_original_prob[0][0] += laa_dl_p * wifi_dl_p * scenario_prob[i];
					} else if (k == 1) { 	//when wifi is uling.
						if (scenario_relation[i][2] == 1)
							laa_original_prob[2][0] += laa_dl_p * wifi_ul_p * scenario_prob[i];
						else if (scenario_relation[i][0] == 1 && scenario_relation[i][3] == 0)
							laa_original_prob[0][1] += laa_dl_p * wifi_ul_p * scenario_prob[i];
						else if (scenario_relation[i][0] == 1 && scenario_relation[i][3] == 1)
							laa_original_prob[1][1] += laa_dl_p * wifi_ul_p * scenario_prob[i];
						else if (scenario_relation[i][0] == 0 && scenario_relation[i][3] == 1)
							laa_original_prob[1][0] += laa_dl_p * wifi_ul_p * scenario_prob[i];
						else if (scenario_relation[i][0] == 0 && scenario_relation[i][3] == 0)
							laa_original_prob[0][0] += laa_dl_p * wifi_ul_p * scenario_prob[i];
					} else {				//when wifi is idle.
						laa_original_prob[0][3] += laa_dl_p * wifi_idle_p * scenario_prob[i];
					}
				} else if (j == 1) { 	//when laa what to ul.
					if (k == 0) { 		//when wifi is dling.
						if (scenario_relation[i][1] == 1)
							laa_original_prob[2][0] += laa_ul_p * wifi_dl_p * scenario_prob[i];
						else if (scenario_relation[i][3] == 1 && scenario_relation[i][0] == 0)
							laa_original_prob[0][1] += laa_ul_p * wifi_dl_p * scenario_prob[i];
						else if (scenario_relation[i][3] == 1 && scenario_relation[i][0] == 1)
							laa_original_prob[1][1] += laa_ul_p * wifi_dl_p * scenario_prob[i];
						else if (scenario_relation[i][3] == 0 && scenario_relation[i][0] == 1)
							laa_original_prob[1][0] += laa_ul_p * wifi_dl_p * scenario_prob[i];
						else if (scenario_relation[i][3] == 0 && scenario_relation[i][0] == 0)
							laa_original_prob[0][0] += laa_ul_p * wifi_dl_p * scenario_prob[i];
					} else if (k == 1) { 	//when wifi is uling.
						if (scenario_relation[i][3] == 1)
							laa_original_prob[2][0] += laa_ul_p * wifi_ul_p * scenario_prob[i];
						else if (scenario_relation[i][1] == 1 && scenario_relation[i][2] == 0)
							laa_original_prob[0][1] += laa_ul_p * wifi_ul_p * scenario_prob[i];
						else if (scenario_relation[i][1] == 1 && scenario_relation[i][2] == 1)
							laa_original_prob[1][1] += laa_ul_p * wifi_ul_p * scenario_prob[i];
						else if (scenario_relation[i][1] == 0 && scenario_relation[i][2] == 1)
							laa_original_prob[1][0] += laa_ul_p * wifi_ul_p * scenario_prob[i];
						else if (scenario_relation[i][1] == 0 && scenario_relation[i][2] == 0)
							laa_original_prob[0][0] += laa_ul_p * wifi_ul_p * scenario_prob[i];
					} else {				//when wifi is idle.
						laa_original_prob[0][3] += laa_ul_p * wifi_idle_p * scenario_prob[i];
					}
				} else {				//when laa is idle.
					if (k == 0) { 		//when wifi is dling.
						laa_original_prob[3][0] += laa_idle_p * wifi_dl_p * scenario_prob[i];
					} else if (k == 1) { 	//when wifi is uling.
						laa_original_prob[3][0] += laa_idle_p * wifi_ul_p * scenario_prob[i];
					} else {				//when wifi is idle.
						laa_original_prob[3][3] += laa_idle_p * wifi_idle_p * scenario_prob[i];
					}
				}
			}
		}
	}

	//wifi original prob
	for (i = 0 ; i < 16 ; i ++) {
		for (j = 0 ; j < 3 ; j ++) {
			for (k = 0 ; k < 3 ; k ++) {
				if (j == 0) { 		//when wifi what to dl.
					if (k == 0) { 		//when laa is dling.
						if (scenario_relation[i][0] == 1)
							wifi_original_prob[0][2] += laa_dl_p * wifi_dl_p * scenario_prob[i];
						else if (scenario_relation[i][1] == 1 && scenario_relation[i][2] == 0)
							wifi_original_prob[1][0] += laa_dl_p * wifi_dl_p * scenario_prob[i];
						else if (scenario_relation[i][1] == 1 && scenario_relation[i][2] == 1)
							wifi_original_prob[1][1] += laa_dl_p * wifi_dl_p * scenario_prob[i];
						else if (scenario_relation[i][1] == 0 && scenario_relation[i][2] == 1)
							wifi_original_prob[0][1] += laa_dl_p * wifi_dl_p * scenario_prob[i];
						else if (scenario_relation[i][1] == 0 && scenario_relation[i][2] == 0)
							wifi_original_prob[0][0] += laa_dl_p * wifi_dl_p * scenario_prob[i];
					} else if (k == 1) { 	//when laa is uling.
						if (scenario_relation[i][1] == 1)
							wifi_original_prob[0][2] += laa_ul_p * wifi_dl_p * scenario_prob[i];
						else if (scenario_relation[i][0] == 1 && scenario_relation[i][3] == 0)
							wifi_original_prob[1][0] += laa_ul_p * wifi_dl_p * scenario_prob[i];
						else if (scenario_relation[i][0] == 1 && scenario_relation[i][3] == 1)
							wifi_original_prob[1][1] += laa_ul_p * wifi_dl_p * scenario_prob[i];
						else if (scenario_relation[i][0] == 0 && scenario_relation[i][3] == 1)
							wifi_original_prob[0][1] += laa_ul_p * wifi_dl_p * scenario_prob[i];
						else if (scenario_relation[i][0] == 0 && scenario_relation[i][3] == 0)
							wifi_original_prob[0][0] += laa_ul_p * wifi_dl_p * scenario_prob[i];
					} else {				//when laa is idle.
						wifi_original_prob[3][0] += laa_idle_p * wifi_dl_p * scenario_prob[i];
					}
				} else if (j == 1) { 	//when wifi what to ul.
					if (k == 0) { 		//when laa is dling.
						if (scenario_relation[i][2] == 1)
							wifi_original_prob[0][2] += laa_dl_p * wifi_ul_p * scenario_prob[i];
						else if (scenario_relation[i][3] == 1 && scenario_relation[i][0] == 0)
							wifi_original_prob[1][0] += laa_dl_p * wifi_ul_p * scenario_prob[i];
						else if (scenario_relation[i][3] == 1 && scenario_relation[i][0] == 1)
							wifi_original_prob[1][1] += laa_dl_p * wifi_ul_p * scenario_prob[i];
						else if (scenario_relation[i][3] == 0 && scenario_relation[i][0] == 1)
							wifi_original_prob[0][1] += laa_dl_p * wifi_ul_p * scenario_prob[i];
						else if (scenario_relation[i][3] == 0 && scenario_relation[i][0] == 0)
							wifi_original_prob[0][0] += laa_dl_p * wifi_ul_p * scenario_prob[i];
					} else if (k == 1) { 	//when laa is uling.
						if (scenario_relation[i][3] == 1)
							wifi_original_prob[0][2] += laa_ul_p * wifi_ul_p * scenario_prob[i];
						else if (scenario_relation[i][2] == 1 && scenario_relation[i][1] == 0)
							wifi_original_prob[1][0] += laa_ul_p * wifi_ul_p * scenario_prob[i];
						else if (scenario_relation[i][2] == 1 && scenario_relation[i][1] == 1)
							wifi_original_prob[1][1] += laa_ul_p * wifi_ul_p * scenario_prob[i];
						else if (scenario_relation[i][2] == 0 && scenario_relation[i][1] == 1)
							wifi_original_prob[0][1] += laa_ul_p * wifi_ul_p * scenario_prob[i];
						else if (scenario_relation[i][2] == 0 && scenario_relation[i][1] == 0)
							wifi_original_prob[0][0] += laa_ul_p * wifi_ul_p * scenario_prob[i];
					} else {				//when laa is idle.
						wifi_original_prob[3][0] += laa_idle_p * wifi_ul_p * scenario_prob[i];
					}
				} else {				//when wifi is idle.
					if (k == 0) { 		//when laa is dling.
						wifi_original_prob[0][3] += laa_dl_p * wifi_idle_p * scenario_prob[i];
					} else if (k == 1) { 	//when laa is uling.
						wifi_original_prob[0][3] += laa_ul_p * wifi_idle_p * scenario_prob[i];
					} else {				//when laa is idle.
						wifi_original_prob[3][3] += laa_idle_p * wifi_idle_p * scenario_prob[i];
					}
				}
			}
		}
	}

	//laa LBR prob
	for (i = 0 ; i < 16 ; i ++) {
		for (j = 0 ; j < 3 ; j ++) {
			for (k = 0 ; k < 3 ; k ++) {
				if (j == 0) { 		//when laa what to dl.
					if (k == 0) { 		//when wifi is dling.
						if (scenario_relation[i][0] == 1) {
							laa_LBR_prob[2][0] += laa_dl_p * wifi_dl_p * scenario_prob[i];
						}
						else if (scenario_relation[i][1] == 1) {
							laa_LBR_prob[2][0] += laa_dl_p * wifi_dl_p * scenario_prob[i];
						}
						else if (scenario_relation[i][2] == 1 && scenario_relation[i][1] == 0) {
							laa_LBR_prob[0][1] += (1 - ack_fail) * laa_dl_p * wifi_dl_p * scenario_prob[i];
							laa_LBR_prob[1][1] += ack_fail * laa_dl_p * wifi_dl_p * scenario_prob[i];
						}
						else if (scenario_relation[i][2] == 0 && scenario_relation[i][1] == 0) {
							laa_LBR_prob[0][0] += (1 - ack_fail) * laa_dl_p * wifi_dl_p * scenario_prob[i];
							laa_LBR_prob[1][0] += ack_fail * laa_dl_p * wifi_dl_p * scenario_prob[i];
						}
					} else if (k == 1) { 	//when wifi is uling.
						if (scenario_relation[i][2] == 1) {
							laa_LBR_prob[2][0] += laa_dl_p * wifi_ul_p * scenario_prob[i];
						}
						else if (scenario_relation[i][3] == 1) {
							laa_LBR_prob[2][0] += laa_dl_p * wifi_ul_p * scenario_prob[i];
						}
						else if (scenario_relation[i][0] == 1 && scenario_relation[i][3] == 0) {
							laa_LBR_prob[0][1] += (1 - ack_fail) * laa_dl_p * wifi_ul_p * scenario_prob[i];
							laa_LBR_prob[1][1] += ack_fail * laa_dl_p * wifi_ul_p * scenario_prob[i];
						}
						else if (scenario_relation[i][0] == 0 && scenario_relation[i][3] == 0) {
							laa_LBR_prob[0][0] += (1 - ack_fail) * laa_dl_p * wifi_ul_p * scenario_prob[i];
							laa_LBR_prob[1][0] += ack_fail * laa_dl_p * wifi_ul_p * scenario_prob[i];
						}
					} else {				//when wifi is idle.
						laa_LBR_prob[0][3] += laa_dl_p * wifi_idle_p * scenario_prob[i];
					}
				} else if (j == 1) { 	//when laa what to ul.
					if (k == 0) { 		//when wifi is dling.
						if (scenario_relation[i][1] == 1) {
							laa_LBR_prob[2][0] += laa_ul_p * wifi_dl_p * scenario_prob[i];
						}
						else if (scenario_relation[i][0] == 1) {
							laa_LBR_prob[2][0] += laa_ul_p * wifi_dl_p * scenario_prob[i];
						}
						else if (scenario_relation[i][3] == 1 && scenario_relation[i][0] == 0) {
							laa_LBR_prob[0][1] += (1 - ack_fail) * laa_ul_p * wifi_dl_p * scenario_prob[i];
							laa_LBR_prob[1][1] += ack_fail * laa_ul_p * wifi_dl_p * scenario_prob[i];
						}
						else if (scenario_relation[i][3] == 0 && scenario_relation[i][0] == 0) {
							laa_LBR_prob[0][0] += (1 - ack_fail) * laa_ul_p * wifi_dl_p * scenario_prob[i];
							laa_LBR_prob[1][0] += ack_fail * laa_ul_p * wifi_dl_p * scenario_prob[i];
						}
					} else if (k == 1) { 	//when wifi is uling.
						if (scenario_relation[i][3] == 1) {
							laa_LBR_prob[2][0] += laa_ul_p * wifi_ul_p * scenario_prob[i];
						}
						else if (scenario_relation[i][2] == 1) {
							laa_LBR_prob[2][0] += laa_ul_p * wifi_ul_p * scenario_prob[i];
						}
						else if (scenario_relation[i][1] == 1 && scenario_relation[i][2] == 0) {
							laa_LBR_prob[0][1] += (1 - ack_fail) * laa_ul_p * wifi_ul_p * scenario_prob[i];
							laa_LBR_prob[1][1] += ack_fail * laa_ul_p * wifi_ul_p * scenario_prob[i];
						}
						else if (scenario_relation[i][1] == 0 && scenario_relation[i][2] == 0) {
							laa_LBR_prob[0][0] += (1 - ack_fail) * laa_ul_p * wifi_ul_p * scenario_prob[i];
							laa_LBR_prob[1][0] += ack_fail * laa_ul_p * wifi_ul_p * scenario_prob[i];
						}
					} else {				//when wifi is idle.
						laa_LBR_prob[0][3] += (1 - ack_fail) * laa_ul_p * wifi_idle_p * scenario_prob[i];
						laa_LBR_prob[1][3] += ack_fail * laa_ul_p * wifi_idle_p * scenario_prob[i];
					}
				} else {				//when laa is idle.
					if (k == 0) { 		//when wifi is dling.
						laa_LBR_prob[3][0] += laa_idle_p * wifi_dl_p * scenario_prob[i];
					} else if (k == 1) { 	//when wifi is uling.
						laa_LBR_prob[3][0] += laa_idle_p * wifi_ul_p * scenario_prob[i];
					} else {				//when wifi is idle.
						laa_LBR_prob[3][3] += laa_idle_p * wifi_idle_p * scenario_prob[i];
					}
				}
			}
		}
	}



	//laa LBR and GAT
	for (n = 0 ; n < P_NUMBER ; n ++) {
		double tmp_n = (double)n;
		p_persistent = (tmp_n) / P_NUMBER;
		for (i = 0 ; i < 16 ; i ++) {
			// if(enbs_wifi_DC_info[i] != 0)
			// 	p_persistent = 1 / enbs_wifi_DC_info[i] / 5;
			// else
			// 	p_persistent = 0;
			for (j = 0 ; j < 3 ; j ++) {
				for (k = 0 ; k < 3 ; k ++) {
					if (j == 0) { 		//when laa what to dl.
						if (k == 0) { 		//when wifi is dling.
							if (scenario_relation[i][1] == 1) {
								laa_LBR_GAT_prob[n][2][0] += laa_dl_p * wifi_dl_p * scenario_prob[i];
							}
							else if (scenario_relation[i][0] == 1 && scenario_relation[i][2] == 0) {
								laa_LBR_GAT_prob[n][0][0] += (1 - ack_fail) * haveToken_prob * p_persistent * laa_dl_p * wifi_dl_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][1][0] += ack_fail * haveToken_prob * p_persistent * laa_dl_p * wifi_dl_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][2][0] += haveToken_prob * (1 - p_persistent) * laa_dl_p * wifi_dl_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][2][0] += (1 - haveToken_prob) * laa_dl_p * wifi_dl_p * scenario_prob[i];
							}
							else if (scenario_relation[i][0] == 1 && scenario_relation[i][2] == 1) {
								laa_LBR_GAT_prob[n][0][1] += (1 - ack_fail) * haveToken_prob * p_persistent * laa_dl_p * wifi_dl_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][1][1] += ack_fail * haveToken_prob * p_persistent * laa_dl_p * wifi_dl_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][2][0] += haveToken_prob * (1 - p_persistent) * laa_dl_p * wifi_dl_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][2][0] += (1 - haveToken_prob) * laa_dl_p * wifi_dl_p * scenario_prob[i];
							}
							else if (scenario_relation[i][0] == 0 && scenario_relation[i][2] == 0) {
								laa_LBR_GAT_prob[n][0][0] += (1 - ack_fail) * laa_dl_p * wifi_dl_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][1][0] += ack_fail * laa_dl_p * wifi_dl_p * scenario_prob[i];
							}
							else if (scenario_relation[i][0] == 0 && scenario_relation[i][2] == 1) {
								laa_LBR_GAT_prob[n][0][1] += (1 - ack_fail) * laa_dl_p * wifi_dl_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][1][1] += ack_fail * laa_dl_p * wifi_dl_p * scenario_prob[i];
							}
						} else if (k == 1) { 	//when wifi is uling.
							if (scenario_relation[i][3] == 1) {
								laa_LBR_GAT_prob[n][2][0] += laa_dl_p * wifi_ul_p * scenario_prob[i];
							}
							else if (scenario_relation[i][2] == 1 && scenario_relation[i][0] == 0) {
								laa_LBR_GAT_prob[n][0][0] += (1 - ack_fail) * haveToken_prob * p_persistent * laa_dl_p * wifi_ul_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][1][0] += ack_fail * haveToken_prob * p_persistent * laa_dl_p * wifi_ul_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][2][0] += haveToken_prob * (1 - p_persistent) * laa_dl_p * wifi_ul_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][2][0] += (1 - haveToken_prob) * laa_dl_p * wifi_ul_p * scenario_prob[i];
							}
							else if (scenario_relation[i][2] == 1 && scenario_relation[i][0] == 1) {
								laa_LBR_GAT_prob[n][0][1] += (1 - ack_fail) * haveToken_prob * p_persistent * laa_dl_p * wifi_ul_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][1][1] += ack_fail * haveToken_prob * p_persistent * laa_dl_p * wifi_ul_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][2][0] += haveToken_prob * (1 - p_persistent) * laa_dl_p * wifi_ul_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][2][0] += (1 - haveToken_prob) * laa_dl_p * wifi_ul_p * scenario_prob[i];
							}
							else if (scenario_relation[i][2] == 0 && scenario_relation[i][0] == 0) {
								laa_LBR_GAT_prob[n][0][0] += (1 - ack_fail) * laa_dl_p * wifi_ul_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][1][0] += ack_fail * laa_dl_p * wifi_ul_p * scenario_prob[i];
							}
							else if (scenario_relation[i][2] == 0 && scenario_relation[i][0] == 1) {
								laa_LBR_GAT_prob[n][0][1] += (1 - ack_fail) * laa_dl_p * wifi_ul_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][1][1] += ack_fail * laa_dl_p * wifi_ul_p * scenario_prob[i];
							}
						} else {				//when wifi is idle.
							laa_LBR_GAT_prob[n][0][3] += (1 - ack_fail) * laa_dl_p * wifi_idle_p * scenario_prob[i];
							laa_LBR_GAT_prob[n][1][3] += ack_fail * laa_dl_p * wifi_idle_p * scenario_prob[i];
						}
					} else if (j == 1) { 	//when laa what to ul.
						if (k == 0) { 		//when wifi is dling.
							if (scenario_relation[i][0] == 1) {
								laa_LBR_GAT_prob[n][2][0] += laa_ul_p * wifi_dl_p * scenario_prob[i];
							}
							else if (scenario_relation[i][1] == 1 && scenario_relation[i][3] == 0) {
								laa_LBR_GAT_prob[n][0][0] += (1 - ack_fail) * haveToken_prob * p_persistent * laa_ul_p * wifi_dl_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][1][0] += ack_fail * haveToken_prob * p_persistent * laa_ul_p * wifi_dl_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][2][0] += haveToken_prob * (1 - p_persistent) * laa_ul_p * wifi_dl_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][2][0] += (1 - haveToken_prob) * laa_ul_p * wifi_dl_p * scenario_prob[i];
							}
							else if (scenario_relation[i][1] == 1 && scenario_relation[i][3] == 1) {
								laa_LBR_GAT_prob[n][0][1] += (1 - ack_fail) * haveToken_prob * p_persistent * laa_ul_p * wifi_dl_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][1][1] += ack_fail * haveToken_prob * p_persistent * laa_ul_p * wifi_dl_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][2][0] += haveToken_prob * (1 - p_persistent) * laa_ul_p * wifi_dl_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][2][0] += (1 - haveToken_prob) * laa_ul_p * wifi_dl_p * scenario_prob[i];
							}
							else if (scenario_relation[i][1] == 0 && scenario_relation[i][3] == 0) {
								laa_LBR_GAT_prob[n][0][0] += (1 - ack_fail) * laa_ul_p * wifi_dl_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][1][0] += ack_fail * laa_ul_p * wifi_dl_p * scenario_prob[i];
							}
							else if (scenario_relation[i][1] == 0 && scenario_relation[i][3] == 1) {
								laa_LBR_GAT_prob[n][0][1] += (1 - ack_fail) * laa_ul_p * wifi_dl_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][1][1] += ack_fail * laa_ul_p * wifi_dl_p * scenario_prob[i];
							}
						} else if (k == 1) { 	//when wifi is uling.
							if (scenario_relation[i][2] == 1)
								laa_LBR_GAT_prob[n][2][0] += laa_ul_p * wifi_ul_p * scenario_prob[i];
							else if (scenario_relation[i][3] == 1 && scenario_relation[i][1] == 0) {
								laa_LBR_GAT_prob[n][0][0] += (1 - ack_fail) * haveToken_prob * p_persistent * laa_ul_p * wifi_ul_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][1][0] += ack_fail * haveToken_prob * p_persistent * laa_ul_p * wifi_ul_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][2][0] += haveToken_prob * (1 - p_persistent) * laa_ul_p * wifi_ul_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][2][0] += (1 - haveToken_prob) * laa_ul_p * wifi_ul_p * scenario_prob[i];
							}
							else if (scenario_relation[i][3] == 1 && scenario_relation[i][1] == 1) {
								laa_LBR_GAT_prob[n][0][1] += (1 - ack_fail) * haveToken_prob * p_persistent * laa_ul_p * wifi_ul_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][1][1] += ack_fail * haveToken_prob * p_persistent * laa_ul_p * wifi_ul_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][2][0] += haveToken_prob * (1 - p_persistent) * laa_ul_p * wifi_ul_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][2][0] += (1 - haveToken_prob) * laa_ul_p * wifi_ul_p * scenario_prob[i];
							}
							else if (scenario_relation[i][3] == 0 && scenario_relation[i][1] == 0) {
								laa_LBR_GAT_prob[n][0][0] += (1 - ack_fail) * laa_ul_p * wifi_ul_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][1][0] += ack_fail * laa_ul_p * wifi_ul_p * scenario_prob[i];
							}
							else if (scenario_relation[i][3] == 0 && scenario_relation[i][1] == 1) {
								laa_LBR_GAT_prob[n][0][1] += (1 - ack_fail) * laa_ul_p * wifi_ul_p * scenario_prob[i];
								laa_LBR_GAT_prob[n][1][1] += ack_fail * laa_ul_p * wifi_ul_p * scenario_prob[i];
							}
						} else {				//when wifi is iule.
							laa_LBR_GAT_prob[n][0][3] += (1 - ack_fail) * laa_ul_p * wifi_idle_p * scenario_prob[i];
							laa_LBR_GAT_prob[n][1][3] += ack_fail * laa_ul_p * wifi_idle_p * scenario_prob[i];
						}
					} else {				//when laa is idle.
						if (k == 0) { 		//when wifi is dling.
							laa_LBR_GAT_prob[n][3][0] += laa_idle_p * wifi_dl_p * scenario_prob[i];
						} else if (k == 1) { 	//when wifi is uling.
							laa_LBR_GAT_prob[n][3][0] += laa_idle_p * wifi_ul_p * scenario_prob[i];
						} else {				//when wifi is idle.
							laa_LBR_GAT_prob[n][3][3] += laa_idle_p * wifi_idle_p * scenario_prob[i];
						}
					}

				}
			}
		}
	}


	double total_prob[4][4];
	double LBR_total_prob[4][4];
	double GAT_total_prob[P_NUMBER][4][4];
	double GAT_total_prob_demo[4][4];
	double laa_original_efficacy[4] = {0};
	double wifi_original_efficacy[4] = {0};
	double laa_GAT_efficacy[P_NUMBER][4] = {0};
	double wifi_GAT_efficacy[P_NUMBER][4] = {0};

	double laa_improvement[P_NUMBER][2] = {0};
	double wifi_improvement[P_NUMBER][2] = {0};
	double laa_improvement_demo[2] = {0};
	double wifi_improvement_demo[2] = {0};
	double tp = wifi_dl_p + wifi_ul_p + laa_ul_p + laa_dl_p;
	double lt = (laa_ul_p + laa_dl_p) / tp;
	double wt = (wifi_dl_p + wifi_ul_p) / tp;
	for (i = 0 ; i < 4 ; i ++) {
		for (j = 0 ; j < 4 ; j ++) {
			total_prob[i][j] = laa_original_prob[i][j] * wt + wifi_original_prob[i][j] * lt;
			// if (i == 0 || i == 1) {
			laa_original_efficacy[i] += total_prob[i][j];
			// }
			// if (j == 0 || j == 1) {
			wifi_original_efficacy[j] += total_prob[i][j];
			// }
		}
	}


	for (i = 0 ; i < 4 ; i ++) {
		for (j = 0 ; j < 4 ; j ++) {
			LBR_total_prob[i][j] = laa_LBR_prob[i][j] * wt + wifi_original_prob[i][j] * lt;
		}
	}

	for (i = 0 ; i < 4 ; i ++) {
		for (j = 0 ; j < 4 ; j ++) {
			GAT_total_prob_demo[i][j] = laa_LBR_GAT_prob[(int)p_demo][i][j] * wt + wifi_original_prob[i][j] * lt;
			if (i == 0 || i == 1) {
				laa_improvement_demo[i] += GAT_total_prob_demo[i][j];
			}
			if (j == 0 || j == 1) {
				wifi_improvement_demo[j] += GAT_total_prob_demo[i][j];
			}
		}
	}


	for (n = 0 ; n < P_NUMBER ; n ++) {
		for (i = 0 ; i < 4 ; i ++) {
			for (j = 0 ; j < 4 ; j ++) {
				GAT_total_prob[n][i][j] = laa_LBR_GAT_prob[n][i][j] * wt + wifi_original_prob[i][j] * lt;
				if (i == 0 || i == 1) {
					laa_improvement[n][i] += GAT_total_prob[n][i][j];
				}
				if (j == 0 || j == 1) {
					wifi_improvement[n][j] += GAT_total_prob[n][i][j];
				}
				laa_GAT_efficacy[n][i] +=  GAT_total_prob[n][i][j];
				wifi_GAT_efficacy[n][j] += GAT_total_prob[n][i][j];
			}
		}
	}

	for (i = 0 ; i < 2 ; i++) {
		laa_improvement_demo[i] = (laa_improvement_demo[i] - laa_original_efficacy[i]) / laa_original_efficacy[i];
		wifi_improvement_demo[i] = (wifi_improvement_demo[i] - wifi_original_efficacy[i]) / wifi_original_efficacy[i];
	}

	for (n = 0 ; n < P_NUMBER ; n ++) {
		for (i = 0 ; i < 2 ; i++) {
			laa_improvement[n][i] = (laa_improvement[n][i] - laa_original_efficacy[i]) / laa_original_efficacy[i];
			wifi_improvement[n][i] = (wifi_improvement[n][i] - wifi_original_efficacy[i]) / wifi_original_efficacy[i];
		}
	}


	printf("wifi_dl = %.2f\twifi_ul = %.2f\n", wifi_dl_p, wifi_ul_p);
	printf(" laa_dl = %.2f\t laa_ul = %.2f\n", laa_dl_p, laa_ul_p);


	printf("\n\n*************************************   laa_original_prob   *************************************\n");
	print_array(laa_original_prob);
	printf("\n\n*************************************   wifi_original_prob  *************************************\n");
	print_array(wifi_original_prob);
	printf("\n\n*************************************       total_prob      *************************************\n");
	print_array(total_prob);
	printf("\n\n*************************************      LBR_total_prob   *************************************\n");
	print_array(LBR_total_prob);
	// // printf("\n\n*******************************      laa_LBR_prob     *******************************\n");
	// // print_array(laa_LBR_prob);
	printf("\nhaveToken_prob = %f\np_persistent = %f", haveToken_prob, p_demo / P_NUMBER );
	// // printf("\n\n*******************************    laa_LBR_GAT_prob   *******************************\n");
	// // print_array(laa_LBR_GAT_prob);
	printf("\n\n*************************************     GAT_total_prob    *************************************\n");
	print_array(GAT_total_prob_demo);

	printf("\np_persistent = %f\n", p_demo / P_NUMBER );
	printf("laa_improvement\n");
	printf("success \t= %.3f %%\n", laa_improvement_demo[0] * 100);
	printf("fail    \t= %.3f %%\n", laa_improvement_demo[1] * 100);
	printf("wifi_improvement\n");
	printf("success \t= %.3f %%\n", wifi_improvement_demo[0] * 100);
	printf("fail    \t= %.3f %%\n", wifi_improvement_demo[1] * 100);

	for (n = 0 ; n < P_NUMBER ; n ++) {
		double tmp_n = (double) n;
		// printf("\np_persistent = %f\n", (tmp_n + 1) / P_NUMBER );
		// printf("laa_improvement\n");
		// printf("success \t= %.3f %%\n", laa_improvement[n][0] * 100);
		// printf("fail    \t= %.3f %%\n", laa_improvement[n][1] * 100);
		// printf("wifi_improvement\n");
		// printf("success \t= %.3f %%\n", wifi_improvement[n][0] * 100);
		// printf("fail    \t= %.3f %%\n", wifi_improvement[n][1] * 100);
		fprintf(pFile_success_improvement, "%f\t%f\t%f\n", (tmp_n) / P_NUMBER * 100, laa_improvement[n][0] , wifi_improvement[n][0] );
		fprintf(pFile_fail_improvement, "%f\t%f\t%f\n", (tmp_n) / P_NUMBER * 100, laa_improvement[n][1] , wifi_improvement[n][1] );
		// LAA probability_p     without_GAT_success     without_GAT_fail     without_GAT_idle     GAT_success     GAT_fail     GAT_idle
		fprintf(pFile_original_GAT_LAA, "%f\t%f\t%f\t%f\t%f\t%f\t%f\t\n", (tmp_n) / P_NUMBER * 100, laa_original_efficacy[0], laa_original_efficacy[1], laa_original_efficacy[2] + laa_original_efficacy[3], laa_GAT_efficacy[n][0], laa_GAT_efficacy[n][1], laa_GAT_efficacy[n][2] + laa_GAT_efficacy[n][3]);
		fprintf(pFile_original_GAT_WIFI, "%f\t%f\t%f\t%f\t%f\t%f\t%f\t\n", (tmp_n) / P_NUMBER * 100, wifi_original_efficacy[0], wifi_original_efficacy[1], wifi_original_efficacy[2] + wifi_original_efficacy[3], wifi_GAT_efficacy[n][0], wifi_GAT_efficacy[n][1], wifi_GAT_efficacy[n][2] + wifi_GAT_efficacy[n][3]);
	}

	fclose(pFile_success_improvement);
	fclose(pFile_fail_improvement);
}

void print_array(double prob_array[4][4]) {
	int i, j;
	double sum_wifi[4] = {0}, sum_laa[4] = {0};
	printf("\t\t                      WiFi\n");
	printf("\t\t|     success   |     fail      |      CSMA      |     silent    |      sum     |\n");
	printf("_________________________________________________________________________________________________\n");
	for ( i = 0 ; i < 4 ; i ++) {
		switch (i) {
		case 0:
			printf("    |\tsuccess\t");
			break;
		case 1:
			printf("LAA |\tfail\t");
			break;
		case 2:
			printf("    |\tLBT/LBR\t");
			break;
		case 3:
			printf("    |\tsilent\t");
			break;
		}
		printf("|");
		for ( j = 0 ; j < 4 ; j ++) {
			printf("     %.3f\t ", prob_array[i][j]);
			sum_wifi[j] += prob_array[i][j];
			sum_laa[i] += prob_array[i][j];
			if (j == 3) {
				printf("|    %.3f\t|", sum_laa[i]);
			}
		}
		printf("\n");
	}
	printf("_________________________________________________________________________________________________\n");
	printf("    |\tsum\t|     %.3f\t      %.3f\t      %.3f\t      %.3f\t \n", sum_wifi[0], sum_wifi[1], sum_wifi[2], sum_wifi[3]);

}
