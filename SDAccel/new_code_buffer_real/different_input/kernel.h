
typedef float REAL;
//typedef REAL (*COORDS)[3];
typedef REAL COORDS[12][3];
#define T_0 298.15
#define MAX_ROTAMER_SIZE 13 // size of ARG
#define MAX_ROTAMER_PAIRS (MAX_ROTAMER_SIZE*(MAX_ROTAMER_SIZE-1))
//extern REAL CUTOFF_DISTANCE;
//extern REAL CUTOFF_DISTANCE_2; 
//extern REAL CLASH_CUTOFF_DISTANCE;

//REAL CUTOFF_DISTANCE = 9.0;
// The Boltzman constant
#define Kb 0.00198576

//Atom types
#define NUM_ATYPES 19
#define NUM_HEAVY_TYPES 17

#define  C    0
#define  CR   1
#define  CH1E 2
#define  CH2E 3
#define  CH3E 4
#define  CR1E 5
#define  NH1  6
#define  NR   7
#define  NH2  8
#define  NH3  9
#define  NC2  10
#define  N    11
#define  OH1  12
#define  O    13
#define  OC   14
#define  S    15
#define  SH1E 16
#define  H    17
#define  HC   18

// Amino acid types
#define NUMAA 25

#define IND -1
#define ARG 0
#define ASN 1
#define ASP 2
#define CYS 3 
#define GLN 4
#define GLU 5
#define HIS 6
#define ILE 7 
#define LEU 8 
#define LYS 9
#define MET 10
#define PHE 11 
#define PRO 12
#define SER 13
#define THR 14
#define TRP 15
#define TYR 16
#define VAL 17
#define GLY 18
#define ALA 19
#define NTR 20 // C-terminal cap
#define CTR 21 // N-terminal cap
#define BBN 22 // C - N backbone piece
#define BBP 23 // C - N backbone piece for Proline
#define BBG 24 // C - N backbone piece for Glycine

extern char AA_NAMES[][5];
extern int NUM_ROTAMERS[];
extern int ROTAMER_START[];
extern float ROTAMER_VALUE[][4];

//extern REAL epsilon[NUM_ATYPES];
//extern REAL SIGMA[NUM_ATYPES];

// The factor used to determine when we have a steric clash
#define SIGMA_FACTOR 0.5


/*
 * UNDERSTAND REAL VALUES: theese are only for the simulation
 */
#define TYPE1_M_ATYPES_DIM 12
#define TYPE2_M_ATYPES_DIM 12
#define TYPE1_M_GROUPS_DIM 4
#define TYPE2_M_GROUPS_DIM 4
#define TYPE1_M_CHARGES_DIM 12
#define TYPE2_M_CHARGES_DIM 12

#define EXCLUDED 1
#define PAIR1_4  2
#define NOT_EXCLUDED 0
/*

void computePairEnergy(int CLeafType, int pNodeType, short pNodeIsLeaf, REAL rot[][3],  const REAL * trans, short bSeparated,  const REAL * bv1_m_center,  const REAL * bv2_m_center,  const REAL * bv1_m_rad,  const REAL * bv2_m_rad, int size1, int size2,  REAL CLeaf_m_rotate[][3], const  COORDS CLeaf_next_m_positions,  REAL * CLeaf_m_translate,   REAL  pNode_m_rotate[][3],  COORDS  pNode_next_m_positions,  REAL * pNode_m_translate,  COORDS pNode_m_positions,  COORDS CLeaf_m_positions,  REAL CLeaf_m_distances[][MAX_ROTAMER_SIZE], int CLeaf_m_index, int pNode_m_index,  int * type1_m_aTypes,  int * type2_m_aTypes, int type1_m_nGroups, int type2_m_nGroups,  int * type1_m_groups,  int * type2_m_groups,  REAL * type1_m_charges,  REAL * type2_m_charges,  REAL * termSum, int* interaction, short *resetTerm);

*/
