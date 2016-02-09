#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "kernel.h"

using namespace std;

int SUCCESS =0;

int main(int argc, char* argv[]){

/* input declaration */
	int CLeafType;
	int pNodeType;
    short pNodeIsLeaf;
	REAL rot[3][3];
       	REAL trans[3];
	short bSeparated;
	REAL bv1_m_center[3];
	REAL bv2_m_center[3];
	REAL bv1_m_rad[1];
	REAL bv2_m_rad[1];
	int size1;
	int size2;
	REAL CLeaf_m_rotate[3][3];
	COORDS CLeaf_next_m_positions;
	REAL CLeaf_m_translate[3];
	REAL pNode_m_rotate[3][3];
	COORDS pNode_next_m_positions;
	REAL pNode_m_translate[3];
	COORDS pNode_m_positions;
	COORDS CLeaf_m_positions;
	REAL CLeaf_m_distances[MAX_ROTAMER_SIZE][MAX_ROTAMER_SIZE];	
	int CLeaf_m_index;
	int pNode_m_index;
    //size of m_aTypes is size 1 and size2
	int * type1_m_aTypes = (int *)malloc(sizeof(int) * TYPE1_M_ATYPES_DIM);
	int * type2_m_aTypes = (int *)malloc(sizeof(int) * TYPE2_M_ATYPES_DIM);
       	int type1_m_nGroups;
	int type2_m_nGroups;
	int * type1_m_groups = (int *)malloc(sizeof(int) * TYPE1_M_GROUPS_DIM);
      	int * type2_m_groups = (int *)malloc(sizeof(int) * TYPE2_M_GROUPS_DIM);
	REAL * type1_m_charges = (REAL *)malloc(sizeof(REAL) * TYPE1_M_CHARGES_DIM);
      	REAL * type2_m_charges = (REAL *)malloc(sizeof(REAL) * TYPE2_M_CHARGES_DIM);
 
	/* output result */
	REAL * termSum = (REAL *)malloc(sizeof(REAL));	
	int * interaction = (int *)malloc(sizeof(int));
	short resetTerm[1];

    //init input
    CLeafType = 20;
    
    pNodeType = 0;
    
    pNodeIsLeaf = 0;
    
    rot[0][0] = 1;
    rot[0][1] = 0;
    rot[0][2] = 0;
    rot[1][0] = 0;
    rot[1][1] = 1;
    rot[1][2] = 0;
    rot[2][0] = 0;
    rot[2][1] = 0;
    rot[2][2] = 1;
    
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
    
    size1 = 5;
    size2 = 12;
    
    CLeaf_m_rotate[0][0] = 1;
    CLeaf_m_rotate[0][1] = 0;
    CLeaf_m_rotate[0][2] = 0;
    CLeaf_m_rotate[1][0] = 0;
    CLeaf_m_rotate[1][1] = 1;
    CLeaf_m_rotate[1][2] = 0;
    CLeaf_m_rotate[2][0] = 0;
    CLeaf_m_rotate[2][1] = 0;
    CLeaf_m_rotate[2][2] = 1;
    
    CLeaf_next_m_positions[0][0] = -0.701;
    CLeaf_next_m_positions[0][1] = -1.266;
    CLeaf_next_m_positions[0][2] = 0.525;
    CLeaf_next_m_positions[1][0] = 0.172;
    CLeaf_next_m_positions[1][1] = -2.499;
    CLeaf_next_m_positions[1][2] = 0.226;
    CLeaf_next_m_positions[2][0] = 1.25;
    CLeaf_next_m_positions[2][1] = -2.642;
    CLeaf_next_m_positions[2][2] = 1.316;
    
    CLeaf_m_translate[0] = 0;
    CLeaf_m_translate[1] = 0;
    CLeaf_m_translate[2] = 1.5;
    
    pNode_m_rotate[0][0] = 0.778867;
    pNode_m_rotate[0][1] = -0.227354;
    pNode_m_rotate[0][2] = 0.584235;
    pNode_m_rotate[1][0] = 0.227354;
    pNode_m_rotate[1][1] = -0.766044;
    pNode_m_rotate[1][2] = -0.601199;
    pNode_m_rotate[2][0] = 0.584235;
    pNode_m_rotate[2][1] = 0.601199;
    pNode_m_rotate[2][2] = -0.545106;

    pNode_next_m_positions[0][0] = 0;
    pNode_next_m_positions[0][1] = 0;
    pNode_next_m_positions[0][2] = 0;
    pNode_next_m_positions[1][0] = 0.25;
    pNode_next_m_positions[1][1] = 0;
    pNode_next_m_positions[1][2] = 1.194;
    pNode_next_m_positions[2][0] = 0.921;
    pNode_next_m_positions[2][1] = 0;
    pNode_next_m_positions[2][2] = -0.974;
    
    pNode_m_translate[0] = 1.412;
    pNode_m_translate[1] = 0;
    pNode_m_translate[2] = 0.534;
    
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


    for (int i = 0; i<MAX_ROTAMER_SIZE; i++) {
        for (int j = 0; j<MAX_ROTAMER_SIZE; j++) {
            CLeaf_m_distances[i][j] = 0;
        }
        
    }
    
    CLeaf_m_index = 0;
    pNode_m_index = 1;
    
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
    
    type1_m_nGroups = 1;
    
    type2_m_nGroups = 4;
    
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
    
	computePairEnergy(CLeafType, pNodeType, pNodeIsLeaf,rot,trans,bSeparated,bv1_m_center,bv2_m_center,bv1_m_rad,bv2_m_rad,size1,size2,CLeaf_m_rotate,CLeaf_next_m_positions,CLeaf_m_translate,pNode_m_rotate,pNode_next_m_positions,  pNode_m_translate, pNode_m_positions,  CLeaf_m_positions,  CLeaf_m_distances,CLeaf_m_index, pNode_m_index,  type1_m_aTypes, type2_m_aTypes, type1_m_nGroups, type2_m_nGroups, type1_m_groups,  type2_m_groups, type1_m_charges, type2_m_charges,  termSum, interaction, resetTerm);


	cout << "kernel computation ended.." << endl;

	REAL golden_sum = 0.590004f;
	cout << "The sum is " << termSum[0] << endl;
	cout << "goldern sum " << golden_sum << endl;
    if(golden_sum != termSum[0]) cout << "THEY SEEM DIFFERENT, CHECK" << endl;
	return 0;
}
