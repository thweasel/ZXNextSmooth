// PRAGMAs for your project
//    #pragma define NAME = value
//    #pragma redirect NAME = value


// NEW LIB

//#pragma output CRT_ORG_CODE = 0x8000
#pragma output CRT_ORG_CODE = 0x6000
#pragma output REGISTER_SP = 0xBFE0
//#pragma output REGISTER_SP = 0x9FE0

// CLASSIC LIB

// Memory address program runs from
//#pragma define CRT_ORG_CODE=0x8000

// Stack location

// Terminal
//#pragma need=ansiterminal
//#pragma define ansicolumns=64

//#pragma redirect ansifont=
//#pragma ansifont_is_packed=0