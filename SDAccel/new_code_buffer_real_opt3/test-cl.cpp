#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <CL/opencl.h>
//#include "support.h"

#include "kernel.h"

/*
			Given an event, this function returns the kernel execution time in ms
			*/
			float getTimeDifference(cl_event event) {
				cl_ulong time_start = 0;
				cl_ulong time_end = 0;
				float total_time = 0.0f;

				clGetEventProfilingInfo(event,
					CL_PROFILING_COMMAND_START,
					sizeof(time_start),
					&time_start,
					NULL);
				clGetEventProfilingInfo(event, 
					CL_PROFILING_COMMAND_END,
					sizeof(time_end),
					&time_end,
					NULL);
				total_time = time_end - time_start;
				return total_time/ 1000000.0; // To convert nanoseconds to milliseconds
			}

int
load_file_to_memory(const char *filename, char **result)
{ 
  int size = 0;
  FILE *f = fopen(filename, "rb");
  if (f == NULL) 
  { 
    *result = NULL;
    return -1; // -1 means file opening fail 
  } 
  fseek(f, 0, SEEK_END);
  size = ftell(f);
  fseek(f, 0, SEEK_SET);
  *result = (char *)malloc(size+1);
  if (size != fread(*result, sizeof(char), size, f)) 
  { 
    free(*result);
    return -2; // -2 means file reading fail 
  } 
  fclose(f);
  (*result)[size] = 0;
  return size;
}

int main(int argc, char** argv)
{
	printf("starting HOST code \n");
	fflush(stdout);
  int err;                            // error code returned from api calls
 

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
  unsigned int correct;               // number of correct results returned

	printf("array defined! \n");
	
	fflush(stdout);

  size_t global[2];                   // global domain size for our calculation
  size_t local[2];                    // local domain size for our calculation

  cl_platform_id platform_id;         // platform id
  cl_device_id device_id;             // compute device id 
  cl_context context;                 // compute context
  cl_command_queue commands;          // compute command queue
  cl_program program;                 // compute program
  cl_kernel kernel;                   // compute kernel
   
  char cl_platform_vendor[1001];
  char cl_platform_name[1001];
   
   	//buffers definition
	cl_mem rot_buff;
	cl_mem trans_buff;
	cl_mem bv1_m_center_buff;
	cl_mem bv2_m_center_buff;
	cl_mem bv1_m_rad_buff;
	cl_mem bv2_m_rad_buff;
	cl_mem CLeaf_m_rotate_buff;
    cl_mem CLeaf_next_m_positions_buff;
	cl_mem CLeaf_m_translate_buff;
	cl_mem pNode_m_rotate_buff;
    cl_mem pNode_next_m_positions_buff;
	cl_mem pNode_m_translate_buff;
    cl_mem pNode_m_positions_buff;
	cl_mem CLeaf_m_positions_buff;
	cl_mem CLeaf_m_distances_buff;
	cl_mem type1_m_aTypes_buff;
	cl_mem type2_m_aTypes_buff;
	cl_mem type1_m_groups_buff;
	cl_mem type2_m_groups_buff;
	cl_mem type1_m_charges_buff;
	cl_mem type2_m_charges_buff;

	cl_mem term_sum_buff;
    cl_mem interaction_buff;
    cl_mem resetTerm_buff;
    
    
    cl_mem CLeafType_buff;
    cl_mem pNodeType_buff;
    cl_mem size1_buff;
    cl_mem size2_buff;
    cl_mem CLeaf_m_index_buff;
    cl_mem pNode_m_index_buff;
    cl_mem type1_m_nGroups_buff;
    cl_mem type2_m_nGroups_buff;

	cl_mem int_array_buff;
	cl_mem real_array_buff;
  if (argc != 2){
    printf("%s <inputfile>\n", argv[0]);
    return EXIT_FAILURE;
  }

  // Fill our data sets with pattern
  //
  int i = 0;
  /*for(i = 0; i < DATA_SIZE; i++) {
    //a[i] = (int)i;
    //b[i] = (int)i;
    results[i] = 0;
  }*/
  /*
  for(int i = 0; i < 32; i++){
    a[i] = 'A';
    b[i] = 'C';
  } */

  // Connect to first platform
  //
  err = clGetPlatformIDs(1,&platform_id,NULL);
  if (err != CL_SUCCESS)
  {
    printf("Error: Failed to find an OpenCL platform!\n");
    printf("Test failed\n");
    return EXIT_FAILURE;
  }
  err = clGetPlatformInfo(platform_id,CL_PLATFORM_VENDOR,1000,(void *)cl_platform_vendor,NULL);
  if (err != CL_SUCCESS)
  {
    printf("Error: clGetPlatformInfo(CL_PLATFORM_VENDOR) failed!\n");
    printf("Test failed\n");
    return EXIT_FAILURE;
  }
  printf("CL_PLATFORM_VENDOR %s\n",cl_platform_vendor);
  err = clGetPlatformInfo(platform_id,CL_PLATFORM_NAME,1000,(void *)cl_platform_name,NULL);
  if (err != CL_SUCCESS)
  {
    printf("Error: clGetPlatformInfo(CL_PLATFORM_NAME) failed!\n");
    printf("Test failed\n");
    return EXIT_FAILURE;
  }
  printf("CL_PLATFORM_NAME %s\n",cl_platform_name);
 
  // Connect to a compute device
  //
  int fpga = 0;
#if defined (FPGA_DEVICE)
  fpga = 1;
#endif
  err = clGetDeviceIDs(platform_id, fpga ? CL_DEVICE_TYPE_ACCELERATOR : CL_DEVICE_TYPE_CPU,
                       1, &device_id, NULL);
  if (err != CL_SUCCESS)
  {
    printf("Error: Failed to create a device group!\n");
    printf("Test failed\n");
    return EXIT_FAILURE;
  }
  
  // Create a compute context 
  //
  context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
  if (!context)
  {
    printf("Error: Failed to create a compute context!\n");
    printf("Test failed\n");
    return EXIT_FAILURE;
  }

  // Create a command commands
  //
  commands = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &err);
  if (!commands)
  {
    printf("Error: Failed to create a command commands!\n");
    printf("Error: code %i\n",err);
    printf("Test failed\n");
    return EXIT_FAILURE;
  }

  int status;

  // Create Program Objects
  //
  
  // Load binary from disk
  unsigned char *kernelbinary;
  char *xclbin=argv[1];
  printf("loading %s\n", xclbin);
  int n_i = load_file_to_memory(xclbin, (char **) &kernelbinary);
  if (n_i < 0) {
    printf("failed to load kernel from xclbin: %s\n", xclbin);
    printf("Test failed\n");
    return EXIT_FAILURE;
  }
  size_t n = n_i;
  // Create the compute program from offline
  program = clCreateProgramWithBinary(context, 1, &device_id, &n,
                                      (const unsigned char **) &kernelbinary, &status, &err);
  if ((!program) || (err!=CL_SUCCESS)) {
    printf("Error: Failed to create compute program from binary %d!\n", err);
    printf("Test failed\n");
    return EXIT_FAILURE;
  }

  // Build the program executable
  //
  err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  if (err != CL_SUCCESS)
  {
    size_t len;
    char buffer[2048];

    printf("Error: Failed to build program executable!\n");
    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
    printf("%s\n", buffer);
    printf("Test failed\n");
    return EXIT_FAILURE;
  }

  // Create the compute kernel in the program we wish to run
  //
  kernel = clCreateKernel(program, "computePairEnergy", &err);
  if (!kernel || err != CL_SUCCESS)
  {
    printf("Error: Failed to create compute kernel!\n");
    printf("Test failed\n");
    return EXIT_FAILURE;
  }

  // Create the input and output arrays in device memory for our calculation
 //buffer creation
	//TODO understand if they are read only, write only or read write
/*	rot_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(REAL) *3 * 3 , NULL, NULL);
	trans_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(REAL) * 3, NULL, NULL);
	bv1_m_center_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(REAL) * 3, NULL, NULL);
	bv2_m_center_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(REAL) * 3, NULL, NULL);
	bv1_m_rad_buff = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(REAL), NULL, NULL);
	bv2_m_rad_buff = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(REAL), NULL, NULL);
    CLeaf_m_rotate_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(REAL) *3 * 3 , NULL, NULL);
    CLeaf_next_m_positions_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(REAL) * 12 * 3, NULL, NULL);
    CLeaf_m_translate_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(REAL) * 3 , NULL, NULL);
	pNode_m_rotate_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(REAL) *3 * 3 , NULL, NULL);
    pNode_next_m_positions_buff = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(REAL) * 12 * 3, NULL, NULL);
	pNode_m_translate_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(REAL) * 3 , NULL, NULL);
    pNode_m_positions_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(REAL) * 12 * 3 , NULL, NULL);
    CLeaf_m_positions_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(REAL) * 12 * 3 , NULL, NULL);
	CLeaf_m_distances_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(REAL) * MAX_ROTAMER_SIZE * MAX_ROTAMER_SIZE , NULL, NULL);
	//type1_m_aTypes_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int) * TYPE1_M_ATYPES_DIM , NULL, NULL);
	//type2_m_aTypes_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int) * TYPE2_M_ATYPES_DIM , NULL, NULL);
		
	//type1_m_groups_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int) * TYPE1_M_GROUPS_DIM , NULL, NULL);
	//type2_m_groups_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int) * TYPE2_M_GROUPS_DIM , NULL, NULL);
	type1_m_charges_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(REAL) * TYPE1_M_CHARGES_DIM , NULL, NULL);
	type2_m_charges_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(REAL) * TYPE2_M_CHARGES_DIM , NULL, NULL);
*/
	term_sum_buff = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(REAL), NULL, NULL);
    interaction_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, NULL);
    resetTerm_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(short), NULL, NULL);
    
    //CLeafType_buff = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, NULL);
    //pNodeType_buff = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, NULL);
    //size1_buff = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, NULL);
    //size2_buff = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, NULL);
    //CLeaf_m_index_buff = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, NULL);
    //pNode_m_index_buff = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, NULL);
    //type1_m_nGroups_buff = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, NULL);
    //type2_m_nGroups_buff = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, NULL);
    

	int_array_buff = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int) * (8 + TYPE1_M_ATYPES_DIM + TYPE2_M_ATYPES_DIM + TYPE1_M_GROUPS_DIM + TYPE2_M_GROUPS_DIM), NULL, NULL);
	real_array_buff = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(REAL) *( 9 + 3 + 3 + 3 + 1 + 1 + 9 + 12*3 + 3 + 9 + 12*3 + 3 + 12*3 + 12*3 + MAX_ROTAMER_SIZE * MAX_ROTAMER_SIZE + TYPE1_M_CHARGES_DIM + TYPE2_M_CHARGES_DIM), NULL, NULL);
    //TODO CHECK BUFFERS
    
/*
	if(!rot_buff || !trans_buff || !bv1_m_center_buff || !bv2_m_center_buff || !CLeaf_m_rotate_buff || !CLeaf_m_translate_buff || !pNode_m_rotate_buff || !pNode_m_translate_buff || !CLeaf_m_distances_buff || !type1_m_aTypes_buff || !type2_m_aTypes_buff || !type1_m_groups_buff || !type1_m_groups_buff || ! type1_m_charges_buff || !type1_m_charges_buff || !term_sum_buff || !interaction_buff || !resetTerm_buff){
		printf("error in buffer creation!! \n");
	}*/

	int error;
  	//write buffers for the kernel
/*	error = clEnqueueWriteBuffer(commands, rot_buff, CL_TRUE, 0, sizeof(REAL) * 3 * 3, rot, 0, NULL, NULL);
	if(error != CL_SUCCESS){
            printf("error while writing the buffer!!\n");
            return EXIT_FAILURE;
          }
	
	error = clEnqueueWriteBuffer(commands, trans_buff, CL_TRUE, 0, sizeof(REAL) * 3, trans, 0, NULL, NULL);
	if(error != CL_SUCCESS){
            printf("error while writing the buffer!!\n");
            return EXIT_FAILURE;
          }
	
	error = clEnqueueWriteBuffer(commands, bv1_m_center_buff, CL_TRUE, 0, sizeof(REAL) * 3, bv1_m_center, 0, NULL, NULL);
	if(error != CL_SUCCESS){
            printf("error while writing the buffer!!\n");
            return EXIT_FAILURE;
          }
	
	error = clEnqueueWriteBuffer(commands, bv2_m_center_buff, CL_TRUE, 0, sizeof(REAL) * 3, bv2_m_center, 0, NULL, NULL);
	if(error != CL_SUCCESS){
            printf("error while writing the buffer!!\n");
            return EXIT_FAILURE;
          }
	
	error = clEnqueueWriteBuffer(commands, bv1_m_rad_buff, CL_TRUE, 0, sizeof(REAL), bv1_m_rad, 0, NULL, NULL);
	if(error != CL_SUCCESS){
		printf("error while writing the buffer!!\n");
		return EXIT_FAILURE;
	}

	error = clEnqueueWriteBuffer(commands, bv2_m_rad_buff, CL_TRUE, 0, sizeof(REAL), bv1_m_rad, 0, NULL, NULL);
	if(error != CL_SUCCESS){
		printf("error while writing the buffer!!\n");
		return EXIT_FAILURE;
	}

    error = clEnqueueWriteBuffer(commands, CLeaf_m_rotate_buff, CL_TRUE, 0, sizeof(REAL) * 3 * 3, CLeaf_m_rotate, 0, NULL, NULL);
    if(error != CL_SUCCESS){
        printf("error while writing the buffer!!\n");
        return EXIT_FAILURE;
    }
    
    error = clEnqueueWriteBuffer(commands, CLeaf_next_m_positions_buff, CL_TRUE, 0, sizeof(REAL) * 12 * 3, CLeaf_next_m_positions, 0, NULL, NULL);
    if(error != CL_SUCCESS){
        printf("error while writing the buffer!!\n");
        return EXIT_FAILURE;
    }
	
	error = clEnqueueWriteBuffer(commands, CLeaf_m_translate_buff, CL_TRUE, 0, sizeof(REAL) * 3, CLeaf_m_translate, 0, NULL, NULL);
	if(error != CL_SUCCESS){
            printf("error while writing the buffer!!\n");
            return EXIT_FAILURE;
          }
	
	error = clEnqueueWriteBuffer(commands, pNode_m_rotate_buff, CL_TRUE, 0, sizeof(REAL) * 3 * 3, pNode_m_rotate, 0, NULL, NULL);
	if(error != CL_SUCCESS){
            printf("error while writing the buffer!!\n");
            return EXIT_FAILURE;
          }
    
    error = clEnqueueWriteBuffer(commands, pNode_next_m_positions_buff, CL_TRUE, 0, sizeof(REAL) * 12 * 3, pNode_next_m_positions, 0, NULL, NULL);
    if(error != CL_SUCCESS){
        printf("error while writing the buffer!!\n");
        return EXIT_FAILURE;
    }
	
	error = clEnqueueWriteBuffer(commands, pNode_m_translate_buff, CL_TRUE, 0, sizeof(REAL) * 3, pNode_m_translate, 0, NULL, NULL);
	if(error != CL_SUCCESS){
            printf("error while writing the buffer!!\n");
            return EXIT_FAILURE;
          }
    
    error = clEnqueueWriteBuffer(commands, pNode_m_positions_buff, CL_TRUE, 0, sizeof(REAL) * 12 * 3, pNode_m_positions, 0, NULL, NULL);
    if(error != CL_SUCCESS){
        printf("error while writing the buffer!!\n");
        return EXIT_FAILURE;
    }
    
    error = clEnqueueWriteBuffer(commands, CLeaf_m_positions_buff, CL_TRUE, 0, sizeof(REAL) * 12 * 3, CLeaf_m_positions, 0, NULL, NULL);
    if(error != CL_SUCCESS){
        printf("error while writing the buffer!!\n");
        return EXIT_FAILURE;
    }
	
	error = clEnqueueWriteBuffer(commands, CLeaf_m_distances_buff, CL_TRUE, 0, sizeof(REAL) * MAX_ROTAMER_SIZE * MAX_ROTAMER_SIZE, CLeaf_m_distances, 0, NULL, NULL);
	if(error != CL_SUCCESS){
            printf("error while writing the buffer!!\n");
            return EXIT_FAILURE;
          }
*/	/*
	error = clEnqueueWriteBuffer(commands, type1_m_aTypes_buff, CL_TRUE, 0, sizeof(int) * TYPE1_M_ATYPES_DIM, type1_m_aTypes, 0, NULL, NULL);
	if(error != CL_SUCCESS){
            printf("error while writing the buffer!!\n");
            return EXIT_FAILURE;
          }
	
	error = clEnqueueWriteBuffer(commands, type2_m_aTypes_buff, CL_TRUE, 0, sizeof(int) * TYPE2_M_ATYPES_DIM, type2_m_aTypes, 0, NULL, NULL);
	if(error != CL_SUCCESS){
            printf("error while writing the buffer!!\n");
            return EXIT_FAILURE;
          }
	
	error = clEnqueueWriteBuffer(commands, type1_m_groups_buff, CL_TRUE, 0, sizeof(int) * TYPE1_M_GROUPS_DIM, type1_m_groups, 0, NULL, NULL);
	if(error != CL_SUCCESS){
            printf("error while writing the buffer!!\n");
            return EXIT_FAILURE;
          }
	
	error = clEnqueueWriteBuffer(commands, type2_m_groups_buff, CL_TRUE, 0, sizeof(int) * TYPE2_M_GROUPS_DIM, type2_m_groups, 0, NULL, NULL);
	if(error != CL_SUCCESS){
            printf("error while writing the buffer!!\n");
            return EXIT_FAILURE;
          }
	*/
/*	error = clEnqueueWriteBuffer(commands, type1_m_charges_buff, CL_TRUE, 0, sizeof(REAL) * TYPE1_M_CHARGES_DIM, type1_m_charges, 0, NULL, NULL);
	if(error != CL_SUCCESS){
            printf("error while writing the buffer!!\n");
            return EXIT_FAILURE;
          }
	
	error = clEnqueueWriteBuffer(commands, type2_m_charges_buff, CL_TRUE, 0, sizeof(REAL) * TYPE2_M_CHARGES_DIM, type2_m_charges, 0, NULL, NULL);
	if(error != CL_SUCCESS){
            printf("error while writing the buffer!!\n");
            return EXIT_FAILURE;
          }
   */ /*
    error = clEnqueueWriteBuffer(commands, CLeafType_buff, CL_TRUE, 0, sizeof(int), CLeafType, 0, NULL, NULL);
    if(error != CL_SUCCESS){
        printf("error while writing the buffer!!\n");
        return EXIT_FAILURE;
    }
    
    error = clEnqueueWriteBuffer(commands, pNodeType_buff, CL_TRUE, 0, sizeof(int), pNodeType, 0, NULL, NULL);
    if(error != CL_SUCCESS){
        printf("error while writing the buffer!!\n");
        return EXIT_FAILURE;
    }
    
    error = clEnqueueWriteBuffer(commands, size1_buff, CL_TRUE, 0, sizeof(int), size1, 0, NULL, NULL);
    if(error != CL_SUCCESS){
        printf("error while writing the buffer!!\n");
        return EXIT_FAILURE;
    }
    
    error = clEnqueueWriteBuffer(commands, size2_buff, CL_TRUE, 0, sizeof(int), size2, 0, NULL, NULL);
    if(error != CL_SUCCESS){
        printf("error while writing the buffer!!\n");
        return EXIT_FAILURE;
    }
    
    error = clEnqueueWriteBuffer(commands, CLeaf_m_index_buff, CL_TRUE, 0, sizeof(int), CLeaf_m_index, 0, NULL, NULL);
    if(error != CL_SUCCESS){
        printf("error while writing the buffer!!\n");
        return EXIT_FAILURE;
    }
    
    error = clEnqueueWriteBuffer(commands, pNode_m_index_buff, CL_TRUE, 0, sizeof(int), pNode_m_index, 0, NULL, NULL);
    if(error != CL_SUCCESS){
        printf("error while writing the buffer!!\n");
        return EXIT_FAILURE;
    }
    
    error = clEnqueueWriteBuffer(commands, type1_m_nGroups_buff, CL_TRUE, 0, sizeof(int), type1_m_nGroups, 0, NULL, NULL);
    if(error != CL_SUCCESS){
        printf("error while writing the buffer!!\n");
        return EXIT_FAILURE;
    }
    
    error = clEnqueueWriteBuffer(commands, type2_m_nGroups_buff , CL_TRUE, 0, sizeof(int), type2_m_nGroups, 0, NULL, NULL);
    if(error != CL_SUCCESS){
        printf("error while writing the buffer!!\n");
        return EXIT_FAILURE;
    }
	*/
	
	error = clEnqueueWriteBuffer(commands, int_array_buff, CL_TRUE, 0, sizeof(int) * (8 + TYPE1_M_ATYPES_DIM + TYPE2_M_ATYPES_DIM + TYPE1_M_GROUPS_DIM + TYPE2_M_GROUPS_DIM), int_array, 0, NULL, NULL);
	if(error != CL_SUCCESS){
        printf("error while writing the buffer!!\n");
        return EXIT_FAILURE;
    }

	error = clEnqueueWriteBuffer(commands, real_array_buff, CL_TRUE, 0, sizeof(REAL) * ( 9 + 3 + 3 + 3 + 1 + 1 + 9 + 12*3 + 3 + 9 + 12*3 + 3 + 12*3 + 12*3 + MAX_ROTAMER_SIZE * MAX_ROTAMER_SIZE + TYPE1_M_CHARGES_DIM + TYPE2_M_CHARGES_DIM), real_array, 0, NULL, NULL); 
	if(error != CL_SUCCESS){
        printf("error while writing the buffer!!\n");
        return EXIT_FAILURE;
    }

	//set the arguments for the kernel
	error = 0;
	error = clSetKernelArg(kernel, 0, sizeof(cl_mem), &int_array_buff);
	if (error != CL_SUCCESS){
       		 printf("Error: Failed to set kernel arguments 0! %d\n", error);
        	printf("Test failed\n");
        	return EXIT_FAILURE;
    	}
	error = clSetKernelArg(kernel, 1, sizeof(cl_mem), &real_array_buff);
	if (error != CL_SUCCESS){
       		 printf("Error: Failed to set kernel arguments 0! %d\n", error);
        	printf("Test failed\n");
        	return EXIT_FAILURE;
    	}
	/*error = clSetKernelArg(kernel, 0, sizeof(cl_mem), &CLeafType_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 0! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &pNodeType_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 1! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }*//*
	error |= clSetKernelArg(kernel, 2, sizeof(short), &pNodeIsLeaf);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 2! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &rot_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 3! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &trans_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 4! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 5, sizeof(short), &bSeparated);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 5! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &bv1_m_center_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 6! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 4, sizeof(cl_mem), &bv2_m_center_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 7! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 5, sizeof(cl_mem), &bv1_m_rad_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 8! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 6, sizeof(cl_mem), &bv2_m_rad_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 9! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
    error |= clSetKernelArg(kernel, 8, sizeof(cl_mem), &size1_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 10! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 9, sizeof(cl_mem), &size2_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 11! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 7, sizeof(cl_mem), &CLeaf_m_rotate_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 12! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 8, sizeof(cl_mem), &CLeaf_next_m_positions_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 13! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 9, sizeof(cl_mem), &CLeaf_m_translate_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 14! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 10, sizeof(cl_mem), &pNode_m_rotate_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 15! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 11, sizeof(cl_mem), &pNode_next_m_positions_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 16! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 12, sizeof(cl_mem), &pNode_m_translate_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 17! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 13, sizeof(cl_mem), &pNode_m_positions_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 18! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 14, sizeof(cl_mem), &CLeaf_m_positions_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 19! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 15, sizeof(cl_mem), &CLeaf_m_distances_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 20! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 19, sizeof(cl_mem), &CLeaf_m_index_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 21! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 20, sizeof(cl_mem), &pNode_m_index_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 22! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 21, sizeof(cl_mem), &type1_m_aTypes_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 23! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 22, sizeof(cl_mem), &type2_m_aTypes_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 24! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 23, sizeof(cl_mem), &type1_m_nGroups_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 25! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 24, sizeof(cl_mem), &type2_m_nGroups_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 26! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 25, sizeof(cl_mem), &type1_m_groups_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 27! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 26, sizeof(cl_mem), &type2_m_groups_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 28! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 16, sizeof(cl_mem), &type1_m_charges_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 29! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
	error |= clSetKernelArg(kernel, 17, sizeof(cl_mem), &type2_m_charges_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 30! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }*/
	error |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &term_sum_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 31! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
    error |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &interaction_buff);
    if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 32! %d\n", error);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
    error |= clSetKernelArg(kernel, 4, sizeof(cl_mem), &resetTerm_buff);
	
	if (error != CL_SUCCESS){
        printf("Error: Failed to set kernel arguments 33! %d\n", error);
        printf("Test failed\n");
		return EXIT_FAILURE;
    }

					fflush(stdout);
		
    // Execute the kernel over the entire range of our 1d input data set
  // using the maximum number of work group items for this device
  //
cl_event enqueue_kernel;
#ifdef C_KERNEL
  err = clEnqueueTask(commands, kernel, 0, NULL, &enqueue_kernel);
#else
  global[0] = MATRIX_RANK;
  global[1] = MATRIX_RANK;
  local[0] = MATRIX_RANK;
  local[1] = MATRIX_RANK;
  err = clEnqueueNDRangeKernel(commands, kernel, 2, NULL, 
                               (size_t*)&global, (size_t*)&local, 0, NULL, NULL);
#endif
  if (err)
  {
    printf("Error: Failed to execute kernel! %d\n", err);
    printf("Test failed\n");
    return EXIT_FAILURE;
  }

  // Read back the results from the device to verify the output
  //
  cl_event readEvent;
	error =  clEnqueueReadBuffer(commands, term_sum_buff, CL_TRUE, 0, sizeof(REAL), termSum, 0, NULL, &readEvent);

					if(error != CL_SUCCESS){
						printf("error in reading the output!! %d \n", error);
						fflush(stdout);
						return EXIT_FAILURE;
					}


  clWaitForEvents(1, &readEvent);
   clWaitForEvents(1, &enqueue_kernel);

	float executionTime = getTimeDifference(enqueue_kernel); 
    		printf(" execution time is %f ms \n", executionTime);
   	printf("READ %f as sum \n", termSum[0]);
  // Print a brief summary detailing the results
  //
  printf("computation ended! \n");
    
  // Shutdown and cleanup
  //
  clReleaseProgram(program);
  clReleaseKernel(kernel);
  clReleaseCommandQueue(commands);
  clReleaseContext(context);

  return EXIT_SUCCESS;
}
