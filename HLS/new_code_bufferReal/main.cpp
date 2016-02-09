#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "kernel.h"

using namespace std;

int SUCCESS =0;

int main(int argc, char* argv[]){

/* input declaration */
	/* input declaration */
	//int CLeafType[1];
	int * int_array = (int *)malloc(sizeof(int) *(8 + TYPE1_M_ATYPES_DIM + TYPE2_M_ATYPES_DIM + TYPE1_M_GROUPS_DIM + TYPE2_M_GROUPS_DIM) );
	REAL * real_array = (REAL *)malloc(sizeof(REAL) * ( 9 + 3 + 3 + 3 + 1 + 1 + 9 + 12*3 + 3 + 9 + 12*3 + 3 + 12*3 + 12*3 + MAX_ROTAMER_SIZE * MAX_ROTAMER_SIZE + TYPE1_M_CHARGES_DIM + TYPE2_M_CHARGES_DIM));

	int * CLeafType = (int *)malloc(sizeof(int));
	//int pNodeType[1];
	int * pNodeType = (int *)malloc(sizeof(int));
    short pNodeIsLeaf;
	//REAL rot[3][3];
	REAL *rot = (REAL *)malloc(sizeof(REAL) * 3 * 3);
       	//REAL trans[3];
       	REAL *trans = (REAL *)malloc(sizeof(REAL) * 3);
	short bSeparated;
	//REAL bv1_m_center[3];
       	REAL *bv1_m_center = (REAL *)malloc(sizeof(REAL) * 3);
	//REAL bv2_m_center[3];
       	REAL *bv2_m_center = (REAL *)malloc(sizeof(REAL) * 3);
	//REAL bv1_m_rad[1];
	REAL * bv1_m_rad = (REAL *)malloc(sizeof(REAL));
	//REAL bv2_m_rad[1];
	REAL * bv2_m_rad = (REAL *)malloc(sizeof(REAL));
	//int size1[1];
	int * size1 = (int *)malloc(sizeof(int));
	//int size2[1];
	int * size2 = (int *)malloc(sizeof(int));
	//REAL CLeaf_m_rotate[3][3];
	REAL * CLeaf_m_rotate = (REAL *)malloc(sizeof(REAL) *3 *3);
	//REAL *[3] CLeaf_m_rotate = (*[3]) malloc(sizeof(REAL) * 3 * 3);
	//COORDS CLeaf_next_m_positions;
	REAL * CLeaf_next_m_positions = (REAL *)malloc(sizeof(REAL) * 12 * 3); //COORDS	
	//REAL CLeaf_m_translate[3];
	REAL * CLeaf_m_translate = (REAL *)malloc(sizeof(REAL) * 3);
	//REAL pNode_m_rotate[3][3];
	REAL * pNode_m_rotate = (REAL *)malloc(sizeof(REAL) * 3* 3);
	//COORDS pNode_next_m_positions;
	REAL * pNode_next_m_positions = (REAL *)malloc(sizeof(REAL) * 12 *3); //COORDS
	//REAL pNode_m_translate[3];
    REAL * pNode_m_translate = (REAL *)malloc(sizeof(REAL) * 3);
	//COORDS pNode_m_positions;
    REAL * pNode_m_positions = (REAL *)malloc(sizeof(REAL) * 12 * 3); //COORDS
	//COORDS CLeaf_m_positions;
    REAL * CLeaf_m_positions = (REAL *)malloc(sizeof(REAL) * 12 * 3); //COORDS
	//REAL CLeaf_m_distances[MAX_ROTAMER_SIZE][MAX_ROTAMER_SIZE];
    REAL * CLeaf_m_distances = (REAL *)malloc(sizeof(REAL) * MAX_ROTAMER_SIZE * MAX_ROTAMER_SIZE);
    //int CLeaf_m_index[1];
    	int * CLeaf_m_index = (int *)malloc(sizeof(int));
	//int pNode_m_index[1];
	int * pNode_m_index = (int *)malloc(sizeof(int));
    //size of m_aTypes is size 1 and size2
	int * type1_m_aTypes = (int *)malloc(sizeof(int) * TYPE1_M_ATYPES_DIM);
	int * type2_m_aTypes = (int *)malloc(sizeof(int) * TYPE2_M_ATYPES_DIM);
       	//int type1_m_nGroups[1];
       	int * type1_m_nGroups = (int *)malloc(sizeof(int));
	//int type2_m_nGroups[1];
	int * type2_m_nGroups = (int *)malloc(sizeof(int));
	int * type1_m_groups = (int *)malloc(sizeof(int) * TYPE1_M_GROUPS_DIM);
      	int * type2_m_groups = (int *)malloc(sizeof(int) * TYPE2_M_GROUPS_DIM);
	REAL * type1_m_charges = (REAL *)malloc(sizeof(REAL) * TYPE1_M_CHARGES_DIM);
      	REAL * type2_m_charges = (REAL *)malloc(sizeof(REAL) * TYPE2_M_CHARGES_DIM);
 
	/* output result */
    REAL * termSum = (REAL *)malloc(sizeof(REAL));
    int * interaction = (int *)malloc(sizeof(int));
    //short resetTerm[1];
    short * resetTerm = (short *)malloc(sizeof(short));
        //init input
    CLeafType[0] = 20;
    
    pNodeType[0] = 0;
    
    pNodeIsLeaf = 0;
    
    for(int i = 0; i < 9; i++){
        rot[i] = 0;
    }
    
    rot[0] = 1;
    rot[4] = 1;
    rot[8] = 1;
    /*
    rot[0][0] = 1;
    rot[0][1] = 0;
    rot[0][2] = 0;
    rot[1][0] = 0;
    rot[1][1] = 1;
    rot[1][2] = 0;
    rot[2][0] = 0;
    rot[2][1] = 0;
    rot[2][2] = 1;
    */
    
    trans[0] = 0;
    trans[1] = 0;
    trans[2] = 1.5;
    
    bSeparated = 0;
    
    bv1_m_center[0] = 0.0102;
    bv1_m_center[1] = 0;
    bv1_m_center[2] = 0.096;
    
    bv2_m_center[0] = 0.0102;
    bv2_m_center[1] = 0;
    bv2_m_center[2] = 0.096;
    
    bv1_m_rad[0] = 1.405037050793176;
    bv2_m_rad[0] = 3.7430290813719407;
    
    size1[0] = 5;
    size2[0] = 12;
    
    for(int i = 0; i < 9; i++){
        CLeaf_m_rotate[i] = 0;
    }
    
    CLeaf_m_rotate[0] = 1;
    CLeaf_m_rotate[4] = 1;
    CLeaf_m_rotate[8] = 1;
    /*
    CLeaf_m_rotate[0][0] = 1;
    CLeaf_m_rotate[0][1] = 0;
    CLeaf_m_rotate[0][2] = 0;
    CLeaf_m_rotate[1][0] = 0;
    CLeaf_m_rotate[1][1] = 1;
    CLeaf_m_rotate[1][2] = 0;
    CLeaf_m_rotate[2][0] = 0;
    CLeaf_m_rotate[2][1] = 0;
    CLeaf_m_rotate[2][2] = 1;
    */
    
    CLeaf_next_m_positions[0] = -0.701;
    CLeaf_next_m_positions[1] = -1.266;
    CLeaf_next_m_positions[2] = 0.525;
    CLeaf_next_m_positions[3] = 0.172;
    CLeaf_next_m_positions[4] = -2.499;
    CLeaf_next_m_positions[5] = 0.226;
    CLeaf_next_m_positions[6] = 1.25;
    CLeaf_next_m_positions[7] = -2.642;
    CLeaf_next_m_positions[8] = 1.316;
    CLeaf_next_m_positions[9] = 1.826000;
    CLeaf_next_m_positions[10] = -3.971000;
    CLeaf_next_m_positions[11] = 1.256000;
    CLeaf_next_m_positions[12] = 2.710000;
    CLeaf_next_m_positions[13] = -4.122000;
    CLeaf_next_m_positions[14] = 0.770000;
    CLeaf_next_m_positions[15] = 1.193000;
    CLeaf_next_m_positions[16] = -5.009000;
    CLeaf_next_m_positions[17] = 1.843000;
    CLeaf_next_m_positions[18] = 1.729000;
    CLeaf_next_m_positions[19] = -6.246000;
    CLeaf_next_m_positions[20] = 1.787000;
    CLeaf_next_m_positions[21] = 1.251000;
    CLeaf_next_m_positions[22] = -7.030000;
    CLeaf_next_m_positions[23] = 2.230000;
    CLeaf_next_m_positions[24] = 2.613000;
    CLeaf_next_m_positions[25] = -6.397000;
    CLeaf_next_m_positions[26] = 1.301000;
    CLeaf_next_m_positions[27] = 0.023000;
    CLeaf_next_m_positions[28] = -4.809000;
    CLeaf_next_m_positions[29] = 2.486000;
    CLeaf_next_m_positions[30] = -0.456000;
    CLeaf_next_m_positions[31] = -5.593000;
    CLeaf_next_m_positions[32] = 2.930000;
    CLeaf_next_m_positions[33] = -0.382000;
    CLeaf_next_m_positions[34] = -3.874000;
    CLeaf_next_m_positions[35] = 2.529000;
    
    
    
    
    
    
    
    
    
    
    
    
    /*
    CLeaf_next_m_positions[0][0] = -0.701;
    CLeaf_next_m_positions[0][1] = -1.266;
    CLeaf_next_m_positions[0][2] = 0.525;
    CLeaf_next_m_positions[1][0] = 0.172;
    CLeaf_next_m_positions[1][1] = -2.499;
    CLeaf_next_m_positions[1][2] = 0.226;
    CLeaf_next_m_positions[2][0] = 1.25;
    CLeaf_next_m_positions[2][1] = -2.642;
    CLeaf_next_m_positions[2][2] = 1.316;
    */
    CLeaf_m_translate[0] = 0;
    CLeaf_m_translate[1] = 0;
    CLeaf_m_translate[2] = 1.5;
    
    pNode_m_rotate[0] = 0.778867;
    pNode_m_rotate[1] = -0.227354;
    pNode_m_rotate[2] = 0.584235;
    pNode_m_rotate[3] = 0.227354;
    pNode_m_rotate[4] = -0.766044;
    pNode_m_rotate[5] = -0.601199;
    pNode_m_rotate[6] = 0.584235;
    pNode_m_rotate[7] = 0.601199;
    pNode_m_rotate[8] = -0.545106;
    /*
    pNode_m_rotate[0][0] = 0.778867;
    pNode_m_rotate[0][1] = -0.227354;
    pNode_m_rotate[0][2] = 0.584235;
    pNode_m_rotate[1][0] = 0.227354;
    pNode_m_rotate[1][1] = -0.766044;
    pNode_m_rotate[1][2] = -0.601199;
    pNode_m_rotate[2][0] = 0.584235;
    pNode_m_rotate[2][1] = 0.601199;
    pNode_m_rotate[2][2] = -0.545106;
    */
    
    pNode_next_m_positions[0] = 0;
    pNode_next_m_positions[1] = 0;
    pNode_next_m_positions[2] = 0;
    pNode_next_m_positions[3] = 0.25;
    pNode_next_m_positions[4] = 0;
    pNode_next_m_positions[5] = 1.194;
    pNode_next_m_positions[6] = 0.921;
    pNode_next_m_positions[7] = 0;
    pNode_next_m_positions[8] = -0.974;
    
    pNode_next_m_positions[9] = 0.596000;
    pNode_next_m_positions[10] = 0.000000;
    pNode_next_m_positions[11] = -1.940000;
    pNode_next_m_positions[12] = 2.347000;
    pNode_next_m_positions[13] = 0.000000;
    pNode_next_m_positions[14] = -0.660000;
    pNode_next_m_positions[15] = 0.000000;
    pNode_next_m_positions[16] = 0.000000;
    pNode_next_m_positions[17] = 0.000000;
    pNode_next_m_positions[18] = 0.000000;
    pNode_next_m_positions[19] = 1.121000;
    pNode_next_m_positions[20] = 0.000000;
    
    pNode_next_m_positions[21] = 0.483000;
    pNode_next_m_positions[22] = -1.138000;
    pNode_next_m_positions[23] = 0.000000;
    pNode_next_m_positions[24] = 0.708000;
    pNode_next_m_positions[25] = -2.020000;
    pNode_next_m_positions[26] = 0.000000;
    pNode_next_m_positions[27] = 0.197000;
    pNode_next_m_positions[28] = -1.115000;
    pNode_next_m_positions[29] = 0.000000;
    pNode_next_m_positions[30] = 2.168000;
    
    pNode_next_m_positions[31] = 0.000000;
    pNode_next_m_positions[32] = 0.000000;
    pNode_next_m_positions[33] = 0.000000;
    pNode_next_m_positions[34] = 0.000000;
    pNode_next_m_positions[35] = 0.000000;
    
    
    /*
    pNode_next_m_positions[0][0] = 0;
    pNode_next_m_positions[0][1] = 0;
    pNode_next_m_positions[0][2] = 0;
    pNode_next_m_positions[1][0] = 0.25;
    pNode_next_m_positions[1][1] = 0;
    pNode_next_m_positions[1][2] = 1.194;
    pNode_next_m_positions[2][0] = 0.921;
    pNode_next_m_positions[2][1] = 0;
    pNode_next_m_positions[2][2] = -0.974;
    */
    pNode_m_translate[0] = 1.412;
    pNode_m_translate[1] = 0;
    pNode_m_translate[2] = 0.534;
    
    pNode_m_positions[0] = -0.701;
    pNode_m_positions[1] = -1.266;
    pNode_m_positions[2] = 0.525;
    pNode_m_positions[3] = 0.172;
    pNode_m_positions[4] = -2.499;
    pNode_m_positions[5] = 0.226;
    pNode_m_positions[6] = 1.25;
    pNode_m_positions[7] = -2.642;
    pNode_m_positions[8] = 1.316;
    pNode_m_positions[9] = 1.826;
    pNode_m_positions[10] = -3.971;
    pNode_m_positions[11] = 1.256;
    pNode_m_positions[12] = 2.71;
    pNode_m_positions[13] = -4.122;
    pNode_m_positions[14] = 0.77;
    pNode_m_positions[15] = 1.193;
    pNode_m_positions[16] = -5.009;
    pNode_m_positions[17] = 1.843;
    pNode_m_positions[18] = 1.729;
    pNode_m_positions[19] = -6.246;
    pNode_m_positions[20] = 1.787;
    pNode_m_positions[21] = 1.251;
    pNode_m_positions[22] = -7.03;
    pNode_m_positions[23] = 2.23;
    pNode_m_positions[24] = 2.613;
    pNode_m_positions[25] = -6.397;
    pNode_m_positions[26] = 1.301;
    pNode_m_positions[27] = 0.023;
    pNode_m_positions[28] = -4.809;
    pNode_m_positions[29] = 2.486;
    pNode_m_positions[30] = -0.456;
    pNode_m_positions[31] = -5.593;
    pNode_m_positions[32] = 2.93;
    pNode_m_positions[33] = -0.382;
    pNode_m_positions[34] = -3.874;
    pNode_m_positions[35] = 2.529;
    /*
    pNode_m_positions[0][0] = -0.701;
    pNode_m_positions[0][1] = -1.266;
    pNode_m_positions[0][2] = 0.525;
    pNode_m_positions[1][0] = 0.172;
    pNode_m_positions[1][1] = -2.499;
    pNode_m_positions[1][2] = 0.226;
    pNode_m_positions[2][0] = 1.25;
    pNode_m_positions[2][1] = -2.642;
    pNode_m_positions[2][2] = 1.316;
    pNode_m_positions[3][0] = 1.826;
    pNode_m_positions[3][1] = -3.971;
    pNode_m_positions[3][2] = 1.256;
    pNode_m_positions[4][0] = 2.71;
    pNode_m_positions[4][1] = -4.122;
    pNode_m_positions[4][2] = 0.77;
    pNode_m_positions[5][0] = 1.193;
    pNode_m_positions[5][1] = -5.009;
    pNode_m_positions[5][2] = 1.843;
    pNode_m_positions[6][0] = 1.729;
    pNode_m_positions[6][1] = -6.246;
    pNode_m_positions[6][2] = 1.787;
    pNode_m_positions[7][0] = 1.251;
    pNode_m_positions[7][1] = -7.03;
    pNode_m_positions[7][2] = 2.23;
    pNode_m_positions[8][0] = 2.613;
    pNode_m_positions[8][1] = -6.397;
    pNode_m_positions[8][2] = 1.301;
    pNode_m_positions[9][0] = 0.023;
    pNode_m_positions[9][1] = -4.809;
    pNode_m_positions[9][2] = 2.486;
    pNode_m_positions[10][0] = -0.456;
    pNode_m_positions[10][1] = -5.593;
    pNode_m_positions[10][2] = 2.93;
    pNode_m_positions[11][0] = -0.382;
    pNode_m_positions[11][1] = -3.874;
    pNode_m_positions[11][2] = 2.529;
    */
    
    CLeaf_m_positions[0] = 0;
    CLeaf_m_positions[1] = 0;
    CLeaf_m_positions[2] = 0;
    CLeaf_m_positions[3] = 0.961;
    CLeaf_m_positions[4] = 0;
    CLeaf_m_positions[5] = -0.34;
    CLeaf_m_positions[6] = -0.455;
    CLeaf_m_positions[7] = 0.847;
    CLeaf_m_positions[8] = -0.34;
    CLeaf_m_positions[9] = -0.455;
    CLeaf_m_positions[10] = -0.847;
    CLeaf_m_positions[11] = -0.34;
    CLeaf_m_positions[12] = 0;
    CLeaf_m_positions[13] = 0;
    CLeaf_m_positions[14] = 1.5;
    
    for (int i = 15; i < 12 * 3; i ++) {
        CLeaf_m_positions[i] = 0;
    }
    
    CLeaf_m_positions[30] = -1.665373;
    
    /*
    CLeaf_m_positions[0][0] = 0;
    CLeaf_m_positions[0][1] = 0;
    CLeaf_m_positions[0][2] = 0;
    CLeaf_m_positions[1][0] = 0.961;
    CLeaf_m_positions[1][1] = 0;
    CLeaf_m_positions[1][2] = -0.34;
    CLeaf_m_positions[2][0] = -0.455;
    CLeaf_m_positions[2][1] = 0.847;
    CLeaf_m_positions[2][2] = -0.34;
    CLeaf_m_positions[3][0] = -0.455;
    CLeaf_m_positions[3][1] = -0.847;
    CLeaf_m_positions[3][2] = -0.34;
    CLeaf_m_positions[4][0] = 0;
    CLeaf_m_positions[4][1] = 0;
    CLeaf_m_positions[4][2] = 1.5;
    */
    
    for(int i = 0; i < MAX_ROTAMER_SIZE * MAX_ROTAMER_SIZE; i++){
        CLeaf_m_distances[i] = 0;
    }
    /*
    for (int i = 0; i<MAX_ROTAMER_SIZE; i++) {
        for (int j = 0; j<MAX_ROTAMER_SIZE; j++) {
            CLeaf_m_distances[i][j] = 0;
        }
        
    }*/
    
    CLeaf_m_index[0] = 0;
    pNode_m_index[0] = 1;
    
    type1_m_aTypes[0] = 9;
    type1_m_aTypes[1] = 18;
    type1_m_aTypes[2] = 18;
    type1_m_aTypes[3] = 18;
    type1_m_aTypes[4] = 2;
    
    type2_m_aTypes[0] = 3;
    type2_m_aTypes[1] = 3;
    type2_m_aTypes[2] = 3;
    type2_m_aTypes[3] = 6;
    type2_m_aTypes[4] = 17;
    type2_m_aTypes[5] = 1;
    type2_m_aTypes[6] = 10;
    type2_m_aTypes[7] = 18;
    type2_m_aTypes[8] = 18;
    type2_m_aTypes[9] = 10;
    type2_m_aTypes[10] = 18;
    type2_m_aTypes[11] = 18;
    
    type1_m_nGroups[0] = 1;
    
    type2_m_nGroups[0] = 4;
    
    type1_m_groups[0] = 0;
    
    type2_m_groups[0] = 0;
    type2_m_groups[1] = 2;
    type2_m_groups[2] = 6;
    type2_m_groups[3] = 9;
    
    
    type1_m_charges[0] = -1.35;
    type1_m_charges[1] = 0.45;
    type1_m_charges[2] = 0.45;
    type1_m_charges[3] = 0.45;
    type1_m_charges[4] = 0.25;
    
    type2_m_charges[0] = 0;
    type2_m_charges[1] = 0;
    type2_m_charges[2] = 0.1;
    type2_m_charges[3] = -0.4;
    type2_m_charges[4] = 0.15;
    type2_m_charges[5] = 0.25;
    type2_m_charges[6] = -0.85;
    type2_m_charges[7] = 0.4;
    type2_m_charges[8] = 0.4;
    type2_m_charges[9] = -0.85;
    type2_m_charges[10] = 0.4;
    type2_m_charges[11] = 0.4;
    
    termSum[0] = -100;
    
	//init the int buffer...
	int_array[0] = CLeafType[0];
	int_array[1] = pNodeType[0];
	int_array[2] = size1[0];
	int_array[3] = size2[0];
	int_array[4] = CLeaf_m_index[0];
	int_array[5] = pNode_m_index[0];
	int index = 6;
	for(int i = index, s_i = 0; i < index + TYPE1_M_ATYPES_DIM; i++, s_i++) int_array[i] = type1_m_aTypes[s_i];
	index += TYPE1_M_ATYPES_DIM;
	for(int i = index, s_i = 0; i < index + TYPE2_M_ATYPES_DIM; i++, s_i++) int_array[i] = type2_m_aTypes[s_i];
	index += TYPE2_M_ATYPES_DIM;
	int_array[index] = type1_m_nGroups[0];
	index++;
	int_array[index] = type2_m_nGroups[0];
	index++;
	for(int i = index, s_i = 0; i < index + TYPE1_M_GROUPS_DIM; i++, s_i++) int_array[i] = type1_m_groups[s_i];
	index += TYPE1_M_GROUPS_DIM;
	for(int i = index, s_i = 0; i < index + TYPE2_M_GROUPS_DIM; i++, s_i++) int_array[i] = type2_m_groups[s_i];



	for(int i = 0; i < (8 + TYPE1_M_ATYPES_DIM + TYPE2_M_ATYPES_DIM + TYPE1_M_GROUPS_DIM + TYPE2_M_GROUPS_DIM); i++){
		printf(" %d \n", int_array[i]);
	}    

	//buffer REAL
	index = 0;
	for(int i = index, s_i = 0; i < index + 9; i++, s_i++) real_array[i] = rot[s_i];
	index += 9;
	for(int i = index, s_i = 0; i < index + 3; i++, s_i++) real_array[i] = trans[s_i];
	index += 3;
	for(int i = index, s_i = 0; i < index + 3; i++, s_i++) real_array[i] = bv1_m_center[s_i];
	index +=3;
	for(int i = index, s_i = 0; i < index + 3; i++, s_i++) real_array[i] = bv1_m_center[s_i];
	index +=3;
	real_array[index] = bv1_m_rad[0];
	index ++;
	real_array[index] = bv2_m_rad[0];
	index ++;
	for(int i = index, s_i = 0; i < index + 9; i++, s_i++) real_array[i] = CLeaf_m_rotate[s_i];
	index += 9;
	for(int i = index, s_i = 0; i < index + 12 * 3; i++, s_i++) real_array[i] = CLeaf_next_m_positions[s_i];
	index += 12 * 3;
	for(int i = index, s_i = 0; i < index + 3; i++, s_i++) real_array[i] = CLeaf_m_translate[s_i];
	index += 3;
	for(int i = index, s_i = 0; i < index + 9; i++, s_i++) real_array[i] = pNode_m_rotate[s_i];
	index += 9;
	for(int i = index, s_i = 0; i < index + 12 * 3; i++, s_i++) real_array[i] = pNode_next_m_positions[s_i];
	index += 12 * 3;
	for(int i = index, s_i = 0; i < index + 3; i++, s_i++) real_array[i] = pNode_m_translate[s_i];
	index += 3;
	for(int i = index, s_i = 0; i < index + 12 * 3; i++, s_i++) real_array[i] = pNode_m_positions[s_i];
	index += 12 * 3;
	for(int i = index, s_i = 0; i < index + 12 * 3; i++, s_i++) real_array[i] = CLeaf_m_positions[s_i];
	index += 12 * 3;
	for(int i = index, s_i = 0; i < index + MAX_ROTAMER_SIZE * MAX_ROTAMER_SIZE; i++, s_i++) real_array[i] = CLeaf_m_distances[s_i];
	index += MAX_ROTAMER_SIZE * MAX_ROTAMER_SIZE;
	for(int i = index, s_i = 0; i < index + TYPE1_M_CHARGES_DIM; i++, s_i++) real_array[i] = type1_m_charges[s_i];
	index += TYPE1_M_CHARGES_DIM;
	for(int i = index, s_i = 0; i < index + TYPE2_M_CHARGES_DIM; i++, s_i++) real_array[i] = type2_m_charges[s_i];

	for(int i = 0; i < ( 9 + 3 + 3 + 3 + 1 + 1 + 9 + 12*3 + 3 + 9 + 12*3 + 3 + 12*3 + 12*3 + MAX_ROTAMER_SIZE * MAX_ROTAMER_SIZE + TYPE1_M_CHARGES_DIM + TYPE2_M_CHARGES_DIM); i++){
		printf(" %f \n", real_array[i]);
	}

computePairEnergy(int_array,real_array,termSum,interaction,resetTerm);


	cout << "kernel computation ended.." << endl;

	REAL golden_sum = 0.590004f;
	cout << "The sum is " << termSum[0] << endl;
	cout << "goldern sum " << golden_sum << endl;
    if(golden_sum != termSum[0]) cout << "THEY SEEM DIFFERENT, CHECK" << endl;
	return 0;
}
