#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <r_asm.h>
#include <r_lib.h>
#include <r_types.h>

/* max. primary opcode (integer instructions only) */
#define OPCODE_MAX 0x2f

#define STR_BUF 20

enum INSTR_FORM {
	FORM_INV,	/* invalid instruction */
	FORM_B,
	FORM_D,
	FORM_I,
	FORM_M,
	FORM_SC,
	FORM_X,
	FORM_XL
};

enum FIELD1 {
/* unused */
	UNUSED1,
/* D */
	D1_TO,
	D1_RD,
	D1_CRFD_L,
	D1_RS
};

enum FIELD2 {
/* unused */
	UNUSED2,
/* D */
	D2_RA,
	D2_RA_ZERO
};

enum FIELD3 {
/* unused */
	UNUSED3,
/* D */
	D3_UIMM,
	D3_SIMM,
	D3_D,
/* M */
	M3_SH,
	M3_RB
};

struct INSTR {
	enum INSTR_FORM form;
	const char *mnemonic;
	enum FIELD1 field1;
	enum FIELD2 field2;
	enum FIELD3 field3;
};

static const struct INSTR instructions[] = {
	/* 0x00 */ {FORM_INV, ""                             , UNUSED1  , UNUSED2   , UNUSED3},
	/* 0x01 */ {FORM_INV, ""                             , UNUSED1  , UNUSED2   , UNUSED3},
	/* 0x02 */ {FORM_INV, ""                             , UNUSED1  , UNUSED2   , UNUSED3},
	/* 0x03 */ {FORM_D  , "twi %i, r%i, 0x%x"            , D1_TO    , D2_RA     , D3_SIMM},
	/* 0x04 */ {FORM_INV, ""                             , UNUSED1  , UNUSED2   , UNUSED3},
	/* 0x05 */ {FORM_INV, ""                             , UNUSED1  , UNUSED2   , UNUSED3},
	/* 0x06 */ {FORM_INV, ""                             , UNUSED1  , UNUSED2   , UNUSED3},
	/* 0x07 */ {FORM_D  , "mulli r%i, r%i, 0x%x"         , D1_RD    , D2_RA     , D3_SIMM},
	/* 0x08 */ {FORM_D  , "subfic r%i, r%i, 0x%x"        , D1_RD    , D2_RA     , D3_SIMM},
	/* 0x09 */ {FORM_INV, ""                             , UNUSED1  , UNUSED2   , UNUSED3},
	/* 0x0a */ {FORM_D  , "cmpli %i, %i, r%i, 0x%x"      , D1_CRFD_L, D2_RA     , D3_UIMM},
	/* 0x0b */ {FORM_D  , "cmpi %i, %i, r%i, 0x%x"       , D1_CRFD_L, D2_RA     , D3_SIMM},
	/* 0x0c */ {FORM_D  , "addic r%i, r%i, 0x%x"         , D1_RD    , D2_RA     , D3_SIMM},
	/* 0x0d */ {FORM_D  , "addic. r%i, r%i, 0x%x"        , D1_RD    , D2_RA     , D3_SIMM},
	/* 0x0e */ {FORM_D  , "addi r%i, %s, 0x%x"           , D1_RD    , D2_RA_ZERO, D3_SIMM},
	/* 0x0f */ {FORM_D  , "addis r%i, %s, 0x%x"          , D1_RD    , D2_RA_ZERO, D3_SIMM},
	/* 0x10 */ {FORM_B  , ""                             , UNUSED1  , UNUSED2   , UNUSED3},
	/* 0x11 */ {FORM_SC , "sc"                           , UNUSED1  , UNUSED2   , UNUSED3},
	/* 0x12 */ {FORM_I  , ""                             , UNUSED1  , UNUSED2   , UNUSED3},
	/* 0x13 */ {FORM_XL , ""                             , UNUSED1  , UNUSED2   , UNUSED3},
	/* 0x14 */ {FORM_M  , "rlwimi%s r%i, r%i, %s, %i, %i", UNUSED1  , UNUSED2   , M3_SH  },
	/* 0x15 */ {FORM_M  , "rlwinm%s r%i, r%i, %s, %i, %i", UNUSED1  , UNUSED2   , M3_SH  },
	/* 0x16 */ {FORM_INV, ""                             , UNUSED1  , UNUSED2   , UNUSED3},
	/* 0x17 */ {FORM_M  , "rlwnm%s r%i, r%i, %s, %i, %i" , UNUSED1  , UNUSED2   , M3_RB  },
	/* 0x18 */ {FORM_D  , "ori r%i, r%i, 0x%x"           , D1_RS    , D2_RA     , D3_UIMM},
	/* 0x19 */ {FORM_D  , "oris r%i, r%i, 0x%x"          , D1_RS    , D2_RA     , D3_UIMM},
	/* 0x1a */ {FORM_D  , "xori r%i, r%i, 0x%x"          , D1_RS    , D2_RA     , D3_UIMM},
	/* 0x1b */ {FORM_D  , "xoris r%i, r%i, 0x%x"         , D1_RS    , D2_RA     , D3_UIMM},
	/* 0x1c */ {FORM_D  , "andi. r%i, r%i, 0x%x"         , D1_RS    , D2_RA     , D3_UIMM},
	/* 0x1d */ {FORM_D  , "andis. r%i, r%i, 0x%x"        , D1_RS    , D2_RA     , D3_UIMM},
	/* 0x1e */ {FORM_INV, ""                             , UNUSED1  , UNUSED2   , UNUSED3},
	/* 0x1f */ {FORM_X  , ""                             , UNUSED1  , UNUSED2   , UNUSED3},
	/* 0x20 */ {FORM_D  , "lwz r%i, 0x%x(%s)"            , D1_RD    , D2_RA_ZERO, D3_D   },
	/* 0x21 */ {FORM_D  , "lwzu r%i, 0x%x(r%i)"          , D1_RD    , D2_RA     , D3_D   },
	/* 0x22 */ {FORM_D  , "lbz r%i, 0x%x(%s)"            , D1_RD    , D2_RA_ZERO, D3_D   },
	/* 0x23 */ {FORM_D  , "lbzu r%i, 0x%x(r%i)"          , D1_RD    , D2_RA     , D3_D   },
	/* 0x24 */ {FORM_D  , "stw r%i, 0x%x(%s)"            , D1_RS    , D2_RA_ZERO, D3_D   },
	/* 0x25 */ {FORM_D  , "stwu r%i, 0x%x(r%i)"          , D1_RS    , D2_RA     , D3_D   },
	/* 0x26 */ {FORM_D  , "stb r%i, 0x%x(%s)"            , D1_RS    , D2_RA_ZERO, D3_D   },
	/* 0x27 */ {FORM_D  , "stbu r%i, 0x%x(r%i)"          , D1_RS    , D2_RA     , D3_D   },
	/* 0x28 */ {FORM_D  , "lhz r%i, 0x%x(%s)"            , D1_RD    , D2_RA_ZERO, D3_D   },
	/* 0x29 */ {FORM_D  , "lhzu r%i, 0x%x(r%i)"          , D1_RD    , D2_RA     , D3_D   },
	/* 0x2a */ {FORM_D  , "lha r%i, 0x%x(%s)"            , D1_RD    , D2_RA_ZERO, D3_D   },
	/* 0x2b */ {FORM_D  , "lhau r%i, 0x%x(r%i)"          , D1_RD    , D2_RA     , D3_D   },
	/* 0x2c */ {FORM_D  , "sth r%i, 0x%x(%s)"            , D1_RS    , D2_RA_ZERO, D3_D   },
	/* 0x2d */ {FORM_D  , "sthu r%i, 0x%x(r%i)"          , D1_RS    , D2_RA     , D3_D   },
	/* 0x2e */ {FORM_D  , "lmw r%i, 0x%x(%s)"            , D1_RD    , D2_RA_ZERO, D3_D   },
	/* 0x2f */ {FORM_D  , "stmw r%i, 0x%x(r%i)"          , D1_RS    , D2_RA     , D3_D   }
};

enum XL_FIELD1 {
	XL1_UNUSED,
	XL1_BO,
	XL1_CRBD
};

enum XL_FIELD2 {
	XL2_UNUSED,
	XL2_BI,
	XL2_CRBA
};

enum XL_FIELD3 {
	XL3_UNUSED,
	XL3_CRBB
};

struct INSTR_XL {
	uint16_t opcode;
	const char *mnemonic;
	enum XL_FIELD1 field1;
	enum XL_FIELD2 field2;
	enum XL_FIELD3 field3;
};

static const struct INSTR_XL instructions_xl[] = {
	{0x000, "mcrf %i, %i"      , XL1_UNUSED, XL2_UNUSED, XL3_UNUSED},
	{0x020, "bclr %i, %i"      , XL1_BO    , XL2_BI    , XL3_UNUSED},
	{0x021, "bclrl %i, %i"     , XL1_BO    , XL2_BI    , XL3_UNUSED},
	{0x042, "crnor %i, %i, %i" , XL1_CRBD  , XL2_CRBA  , XL3_CRBB  },
	{0x064, "rfi"              , XL1_UNUSED, XL2_UNUSED, XL3_UNUSED},
	{0x102, "crandc %i, %i, %i", XL1_CRBD  , XL2_CRBA  , XL3_CRBB  },
	{0x12c, "isync"            , XL1_UNUSED, XL2_UNUSED, XL3_UNUSED},
	{0x182, "crxor %i, %i, %i" , XL1_CRBD  , XL2_CRBA  , XL3_CRBB  },
	{0x202, "crand %i, %i, %i" , XL1_CRBD  , XL2_CRBA  , XL3_CRBB  },
	{0x1c2, "crnand %i, %i, %i", XL1_CRBD  , XL2_CRBA  , XL3_CRBB  },
	{0x242, "creqv %i, %i, %i" , XL1_CRBD  , XL2_CRBA  , XL3_CRBB  },
	{0x342, "crorc %i, %i, %i" , XL1_CRBD  , XL2_CRBA  , XL3_CRBB  },
	{0x382, "cror %i, %i, %i"  , XL1_CRBD  , XL2_CRBA  , XL3_CRBB  },
	{0x420, "bcctr %i, %i"     , XL1_BO    , XL2_BI    , XL3_UNUSED},
	{0x421, "bcctrl %i, %i"    , XL1_BO    , XL2_BI    , XL3_UNUSED}
};

enum X_FIELD1 {
	X1_UNUSED,
	X1_CRFD_L,
	X1_CRFD,
	X1_TO,
	X1_RD,
	X1_RS,
	X1_CT
};

enum X_FIELD2 {
	X2_UNUSED,
	X2_RA,
	X2_RA_ZERO,
	X2_CRM,
	X2_SR,
	X2_SPR,
	X2_TBR
};

enum X_FIELD3 {
	X3_UNUSED,
	X3_RB,
	X3_CRM,
	X3_SPR,
	X3_TBR,
	X3_NB,
	X3_SH
};

struct INSTR_X {
	uint16_t opcode;
	const char *mnemonic;
	enum X_FIELD1 field1;
	enum X_FIELD2 field2;
	enum X_FIELD3 field3;
};

static const struct INSTR_X instructions_x[] = {
	{0x000, "cmp %i, %i, r%i, r%i" , X1_CRFD_L, X2_RA     , X3_RB    },
	{0x008, "tw %i, r%i, r%i"      , X1_TO    , X2_RA     , X3_RB    },
	{0x010, "subfc r%i, r%i, r%i"  , X1_RD    , X2_RA     , X3_RB    },
	{0x011, "subfc. r%i, r%i, r%i" , X1_RD    , X2_RA     , X3_RB    },
	{0x014, "addc r%i, r%i, r%i"   , X1_RD    , X2_RA     , X3_RB    },
	{0x015, "addc. r%i, r%i, r%i"  , X1_RD    , X2_RA     , X3_RB    },
	{0x016, "mulhwu r%i, r%i, r%i" , X1_RD    , X2_RA     , X3_RB    },
	{0x017, "mulhwu. r%i, r%i, r%i", X1_RD    , X2_RA     , X3_RB    },
	{0x026, "mfcr r%i"             , X1_RD    , X2_UNUSED , X3_UNUSED},
	{0x028, "lwarx r%i, %s, r%i"   , X1_RD    , X2_RA_ZERO, X3_RB    },
	{0x02e, "lwzx r%i, %s, r%i"    , X1_RD    , X2_RA_ZERO, X3_RB    },
	{0x030, "slw r%i, r%i, r%i"    , X1_RS    , X2_RA     , X3_RB    },
	{0x031, "slw. r%i, r%i, r%i"   , X1_RS    , X2_RA     , X3_RB    },
	{0x034, "cntlzw r%i, r%i"      , X1_RS    , X2_RA     , X3_UNUSED},
	{0x035, "cntlzw. r%i, r%i"     , X1_RS    , X2_RA     , X3_UNUSED},
	{0x038, "and r%i, r%i, r%i"    , X1_RS    , X2_RA     , X3_RB    },
	{0x039, "and. r%i, r%i, r%i"   , X1_RS    , X2_RA     , X3_RB    },
	{0x040, "cmpl %i, %i, r%i, r%i", X1_CRFD_L, X2_UNUSED , X3_UNUSED},
	{0x050, "subf r%i, r%i, r%i"   , X1_RD    , X2_RA     , X3_RB    },
	{0x051, "subf. r%i, r%i, r%i"  , X1_RD    , X2_RA     , X3_RB    },
	{0x06c, "dcbst %s, r%i"        , X1_UNUSED, X2_RA_ZERO, X3_RB    },
	{0x06e, "lwzux r%i, r%i, r%i"  , X1_RD    , X2_RA     , X3_RB    },
	{0x078, "andc r%i, r%i, r%i"   , X1_RS    , X2_RA     , X3_RB    },
	{0x079, "andc. r%i, r%i, r%i"  , X1_RS    , X2_RA     , X3_RB    },
	{0x096, "mulhw r%i, r%i, r%i"  , X1_RD    , X2_RA     , X3_RB    },
	{0x097, "mulhw. r%i, r%i, r%i" , X1_RD    , X2_RA     , X3_RB    },
	{0x0a6, "mfmsr r%i"            , X1_RD    , X2_UNUSED , X3_UNUSED},
	{0x0ac, "dcbf %s, r%i"         , X1_UNUSED, X2_RA_ZERO, X3_RB    },
	{0x0ae, "lbzx r%i, %s, r%i"    , X1_RD    , X2_RA_ZERO, X3_RB    },
	{0x0d0, "neg r%i, r%i"         , X1_RD    , X2_RA     , X3_UNUSED},
	{0x0d1, "neg. r%i, r%i"        , X1_RD    , X2_RA     , X3_UNUSED},
	{0x0ee, "lbzux r%i, r%i, r%i"  , X1_RD    , X2_RA     , X3_RB    },
	{0x0f8, "nor r%i, r%i, r%i"    , X1_RS    , X2_RA     , X3_RB    },
	{0x0f9, "nor. r%i, r%i, r%i"   , X1_RS    , X2_RA     , X3_RB    },
	{0x110, "subfe r%i, r%i, r%i"  , X1_RD    , X2_RA     , X3_RB    },
	{0x111, "subfe. r%i, r%i, r%i" , X1_RD    , X2_RA     , X3_RB    },
	{0x114, "adde r%i, r%i, r%i"   , X1_RD    , X2_RA     , X3_RB    },
	{0x115, "adde. r%i, r%i, r%i"  , X1_RD    , X2_RA     , X3_RB    },
	{0x120, "mtcrf 0x%x, r%i"      , X1_RS    , X2_CRM    , X3_CRM   },
	{0x124, "mtmsr r%i"            , X1_RS    , X2_UNUSED , X3_UNUSED},
	{0x12d, "stwcx. r%i, %s, r%i"  , X1_RS    , X2_RA_ZERO, X3_RB    },
	{0x12e, "stwx r%i, %s, r%i"    , X1_RS    , X2_RA_ZERO, X3_RB    },
	{0x16e, "stwux r%i, %s, r%i"   , X1_RS    , X2_RA_ZERO, X3_RB    }, /* actually X2_RA */
	{0x190, "subfze r%i, r%i"      , X1_RD    , X2_RA     , X3_UNUSED},
	{0x191, "subfze. r%i, r%i"     , X1_RD    , X2_RA     , X3_UNUSED},
	{0x194, "addze r%i, r%i"       , X1_RD    , X2_RA     , X3_UNUSED},
	{0x195, "addze. r%i, r%i"      , X1_RD    , X2_RA     , X3_UNUSED},
	{0x1a4, "mtsr %i, r%i"         , X1_RS    , X2_SR     , X3_UNUSED},
	{0x1ae, "stbx r%i, %s, r%i"    , X1_RS    , X2_RA_ZERO, X3_RB    },
	{0x1d0, "subfme r%i, r%i"      , X1_RD    , X2_RA     , X3_UNUSED},
	{0x1d1, "subfme. r%i, r%i"     , X1_RD    , X2_RA     , X3_UNUSED},
	{0x1d4, "addme r%i, r%i"       , X1_RD    , X2_RA     , X3_UNUSED},
	{0x1d5, "addme. r%i, r%i"      , X1_RD    , X2_RA     , X3_UNUSED},
	{0x1d6, "mullw r%i, r%i, r%i"  , X1_RD    , X2_RA     , X3_RB    },
	{0x1d7, "mullw. r%i, r%i, r%i" , X1_RD    , X2_RA     , X3_RB    },
	{0x1e4, "mtsrin r%i, r%i"      , X1_RS    , X2_UNUSED , X3_RB    },
	{0x1ec, "dcbtst %s, r%i"       , X1_CT    , X2_RA_ZERO, X3_RB    },
	{0x1ee, "stbux r%i, %s, r%i"   , X1_RS    , X2_RA_ZERO, X3_RB    }, /* actually X2_RA */
	{0x214, "add r%i, r%i, r%i"    , X1_RD    , X2_RA     , X3_RB    },
	{0x215, "add. r%i, r%i, r%i"   , X1_RD    , X2_RA     , X3_RB    },
	{0x22c, "dcbt %s, r%i"         , X1_CT    , X2_RA_ZERO, X3_RB    },
	{0x22e, "lhzx r%i, %s, r%i"    , X1_RD    , X2_RA_ZERO, X3_RB    },
	{0x238, "eqv r%i, r%i, r%i"    , X1_RS    , X2_RA     , X3_RB    },
	{0x239, "eqv. r%i, r%i, r%i"   , X1_RS    , X2_RA     , X3_RB    },
	{0x264, "tlbie r%i"            , X1_UNUSED, X2_UNUSED , X3_RB    },
	{0x26c, "eciwx r%i, %s, r%i"   , X1_RD    , X2_RA_ZERO, X3_RB    },
	{0x26e, "lhzux r%i, r%i, r%i"  , X1_RD    , X2_RA     , X3_RB    },
	{0x278, "xor r%i, r%i, r%i"    , X1_RS    , X2_RA     , X3_RB    },
	{0x279, "xor. r%i, r%i, r%i"   , X1_RS    , X2_RA     , X3_RB    },
	{0x2a6, "mfspr r%i, %i"        , X1_RD    , X2_SPR    , X3_SPR   },
	{0x2ae, "lhax r%i, %s, r%i"    , X1_RD    , X2_RA_ZERO, X3_RB    },
	{0x2e4, "tlbia"                , X1_UNUSED, X2_UNUSED , X3_UNUSED},
	{0x2e6, "mftb r%i, %i"         , X1_RD    , X2_TBR    , X3_TBR   },
	{0x2ee, "lhaux r%i, r%i, r%i"  , X1_RD    , X2_RA     , X3_RB    },
	{0x32e, "sthx r%i, %s, r%i"    , X1_RS    , X2_RA_ZERO, X3_RB    },
	{0x338, "orc r%i, r%i, r%i"    , X1_RS    , X2_RA     , X3_RB    },
	{0x339, "orc. r%i, r%i, r%i"   , X1_RS    , X2_RA     , X3_RB    },
	{0x36c, "ecowx r%i, %s, r%i"   , X1_RS    , X2_RA_ZERO, X3_RB    },
	{0x36e, "sthux r%i, %s, r%i"   , X1_RS    , X2_RA_ZERO, X3_RB    },
	{0x378, "or r%i, r%i, r%i"     , X1_RS    , X2_RA     , X3_RB    },
	{0x379, "or. r%i, r%i, r%i"    , X1_RS    , X2_RA     , X3_RB    },
	{0x396, "divwu r%i, r%i, r%i"  , X1_RD    , X2_RA     , X3_RB    },
	{0x397, "divwu. r%i, r%i, r%i" , X1_RD    , X2_RA     , X3_RB    },
	{0x3a6, "mtspr %i, r%i"        , X1_RS    , X2_SPR    , X3_SPR   },
	{0x3ac, "dcbi %s, r%i"         , X1_UNUSED, X2_RA_ZERO, X3_RB    },
	{0x3b8, "nand r%i, r%i, r%i"   , X1_RS    , X2_RA     , X3_RB    },
	{0x3b9, "nand. r%i, r%i, r%i"  , X1_RS    , X2_RA     , X3_RB    },
	{0x3d6, "divw r%i, r%i, r%i"   , X1_RD    , X2_RA     , X3_RB    },
	{0x3d7, "divw. r%i, r%i, r%i"  , X1_RD    , X2_RA     , X3_RB    },
	{0x400, "mcrxr %i"             , X1_CRFD  , X2_UNUSED , X3_UNUSED},
	{0x410, "subfco r%i, r%i, r%i" , X1_RD    , X2_RA     , X3_RB    },
	{0x411, "subfco. r%i, r%i, r%i", X1_RD    , X2_RA     , X3_RB    },
	{0x414, "addco r%i, r%i, r%i"  , X1_RD    , X2_RA     , X3_RB    },
	{0x415, "addco. r%i, r%i, r%i" , X1_RD    , X2_RA     , X3_RB    },
	{0x42a, "lswx r%i, %s, r%i"    , X1_RD    , X2_RA_ZERO, X3_RB    },
	{0x42c, "lwbrx r%i, %s, r%i"   , X1_RD    , X2_RA_ZERO, X3_RB    },
	{0x430, "srw r%i, r%i, r%i"    , X1_RS    , X2_RA     , X3_RB    },
	{0x431, "srw. r%i, r%i, r%i"   , X1_RS    , X2_RA     , X3_RB    },
	{0x450, "subfo r%i, r%i, r%i"  , X1_RD    , X2_RA     , X3_RB    },
	{0x451, "subfo. r%i, r%i, r%i" , X1_RD    , X2_RA     , X3_RB    },
	{0x46c, "tlbsync"              , X1_UNUSED, X2_UNUSED , X3_UNUSED},
	{0x4a6, "mfsr r%i, %i"         , X1_RD    , X2_SR     , X3_UNUSED},
	{0x4aa, "lswi r%i, %s, %i"     , X1_RD    , X2_RA_ZERO, X3_NB    },
	{0x4ac, "sync"                 , X1_UNUSED, X2_UNUSED , X3_UNUSED},
	{0x4d0, "nego r%i, r%i"        , X1_RD    , X2_RA     , X3_UNUSED},
	{0x4d1, "nego. r%i, r%i"       , X1_RD    , X2_RA     , X3_UNUSED},
	{0x510, "subfeo r%i, r%i, r%i" , X1_RD    , X2_RA     , X3_RB    },
	{0x511, "subfeo. r%i, r%i, r%i", X1_RD    , X2_RA     , X3_RB    },
	{0x514, "addeo r%i, r%i, r%i"  , X1_RD    , X2_RA     , X3_RB    },
	{0x515, "addeo. r%i, r%i, r%i" , X1_RD    , X2_RA     , X3_RB    },
	{0x526, "mfsrin r%i, r%i"      , X1_RS    , X2_UNUSED , X3_RB    },
	{0x52a, "stswx r%i, %s, r%i"   , X1_RS    , X2_RA_ZERO, X3_RB    },
	{0x52c, "stwbrx r%i, %s, r%i"  , X1_RS    , X2_RA_ZERO, X3_RB    },
	{0x590, "subfzeo r%i, r%i"     , X1_RD    , X2_RA     , X3_UNUSED},
	{0x591, "subfzeo. r%i, r%i"    , X1_RD    , X2_RA     , X3_UNUSED},
	{0x594, "addzeo r%i, r%i"      , X1_RD    , X2_RA     , X3_UNUSED},
	{0x595, "addzeo. r%i, r%i"     , X1_RD    , X2_RA     , X3_UNUSED},
	{0x5aa, "stswi r%i, %s, %i"    , X1_RS    , X2_RA_ZERO, X3_NB    },
	{0x5d0, "subfmeo r%i, r%i"     , X1_RD    , X2_RA     , X3_UNUSED},
	{0x5d1, "subfmeo. r%i, r%i"    , X1_RD    , X2_RA     , X3_UNUSED},
	{0x5d4, "addmeo r%i, r%i"      , X1_RD    , X2_RA     , X3_UNUSED},
	{0x5d5, "addmeo. r%i, r%i"     , X1_RD    , X2_RA     , X3_UNUSED},
	{0x5d6, "mullwo r%i, r%i, r%i" , X1_RD    , X2_RA     , X3_RB    },
	{0x5d7, "mullwo. r%i, r%i, r%i", X1_RD    , X2_RA     , X3_RB    },
	{0x5ec, "dcba %s, r%i"         , X1_UNUSED, X2_RA_ZERO, X3_RB    },
	{0x614, "addo r%i, r%i, r%i"   , X1_RD    , X2_RA     , X3_RB    },
	{0x615, "addo. r%i, r%i, r%i"  , X1_RD    , X2_RA     , X3_RB    },
	{0x62c, "lhbrx r%i, %s, r%i"   , X1_RD    , X2_RA_ZERO, X3_RB    },
	{0x630, "sraw r%i, r%i, r%i"   , X1_RS    , X2_RA     , X3_RB    },
	{0x631, "sraw. r%i, r%i, r%i"  , X1_RS    , X2_RA     , X3_RB    },
	{0x670, "srawi r%i, r%i, %i"   , X1_RS    , X2_RA     , X3_SH    },
	{0x671, "srawi. r%i, r%i, %i"  , X1_RS    , X2_RA     , X3_SH    },
	{0x6ac, "eieio"                , X1_UNUSED, X2_UNUSED , X3_UNUSED},
	{0x72c, "sthbrx r%i, %s, r%i"  , X1_RS    , X2_RA_ZERO, X3_RB    },
	{0x734, "extsh r%i, r%i"       , X1_RS    , X2_RA     , X3_UNUSED},
	{0x735, "extsh. r%i, r%i"      , X1_RS    , X2_RA     , X3_UNUSED},
	{0x774, "extsb r%i, r%i"       , X1_RS    , X2_RA     , X3_UNUSED},
	{0x775, "extsb. r%i, r%i"      , X1_RS    , X2_RA     , X3_UNUSED},
	{0x796, "divwuo r%i, r%i, r%i" , X1_RD    , X2_RA     , X3_RB    },
	{0x797, "divwuo. r%i, r%i, r%i", X1_RD    , X2_RA     , X3_RB    },
	{0x7ac, "icbi %s, r%i"         , X1_UNUSED, X2_RA_ZERO, X3_RB    },
	{0x7d6, "divwo r%i, r%i, r%i"  , X1_RD    , X2_RA     , X3_RB    },
	{0x7d7, "divwo. r%i, r%i, r%i" , X1_RD    , X2_RA     , X3_RB    },
	{0x7ec, "dcbz %s, r%i"         , X1_UNUSED, X2_RA_ZERO, X3_RB    }
};

static void decode_b_form(uint32_t i, uint32_t pc, char *s)
{
	uint8_t branch_type = i & 0x3;
	uint8_t bo, bi;
	uint16_t bd;
	uint32_t tmp;

	bo = i>>21 & 0x1f;
	bi = i>>16 & 0x1f;
	bd = i>>2  & 0x3fff;

	tmp = bd << 2;
	/* sign extension */
	if (tmp & 0x8000)
		tmp |= 0xffff0000;

	switch (branch_type) {
	/* bc */
	case 0x0:
		pc = pc + (int32_t)tmp;
		snprintf(s, R_ASM_BUFSIZE, "bc %i, %i, 0x%x", bo, bi, pc);
		break;

	/* bcl */
	case 0x1:
		pc = pc + (int32_t)tmp;
		snprintf(s, R_ASM_BUFSIZE, "bcl %i, %i, 0x%x", bo, bi, pc);
		break;
	
	/* bca */
	case 0x2:
		snprintf(s, R_ASM_BUFSIZE, "bca %i, %i, 0x%x", bo, bi, tmp);
		break;

	/* bcla */
	case 0x3:
		snprintf(s, R_ASM_BUFSIZE, "bcla %i, %i, 0x%x", bo, bi, tmp);
		break;
	}
}

static void decode_i_form(uint32_t i, uint32_t pc, char *s)
{
	uint8_t branch_type = i & 0x3;
	uint32_t li, tmp;

	li = i>>2 & 0xffffff;

	tmp = li << 2;
	/* sign extension */
	if (tmp & 0x2000000)
		tmp |= 0xfc000000;

	switch (branch_type) {
	/* b */
	case 0x0:
		pc = pc + (int32_t)tmp;
		snprintf(s, R_ASM_BUFSIZE, "b 0x%x", pc);
		break;

	/* bl */
	case 0x1:
		pc = pc + (int32_t)tmp;
		snprintf(s, R_ASM_BUFSIZE, "bl 0x%x", pc);
		break;

	/* ba */
	case 0x2:
		snprintf(s, R_ASM_BUFSIZE, "ba 0x%x", tmp);
		break;

	/* bla */
	case 0x3:
		snprintf(s, R_ASM_BUFSIZE, "bla 0x%x", tmp);
		break;
	}
}

static void decode_m_form(uint32_t i, uint8_t prim_opcode, char *s)
{
	uint8_t update_cr0 = i & 0x1;
	char dot[2] = ".";
	uint8_t rS, rA, SH_rB, MB, ME;
	char operand3[STR_BUF];

	rS    = i>>21 & 0x1f;
	rA    = i>>16 & 0x1f;
	SH_rB = i>>11 & 0x1f;
	MB    = i>>6  & 0x1f;
	ME    = i>>1  & 0x1f;

	if (update_cr0 == 0)
		dot[0] = '\0';
	if (instructions[prim_opcode].field3 == M3_RB) {
		snprintf(operand3, STR_BUF, "r%i", SH_rB);
	} else {
		snprintf(operand3, STR_BUF, "%i", SH_rB);
	}

	snprintf(s, R_ASM_BUFSIZE, instructions[prim_opcode].mnemonic,
	         dot, rA, rS, operand3, MB, ME);
}

static void decode_d_form(uint32_t i, uint8_t prim_opcode, char *s)
{
	uint8_t field1, field2;
	uint16_t field3;
	uint8_t crfD, L;
	char reg[STR_BUF] = "0";

	field1 = i>>21 & 0x1f;
	field2 = i>>16 & 0x1f;
	field3 = i     & 0xffff;

	/* cmpli/cmpi crfD, L, rA, UIMM/SIMM */
	if (instructions[prim_opcode].field1 == D1_CRFD_L) {
		crfD = field1>>2 & 0x7;
		L    = field1    & 0x1;
		snprintf(s, R_ASM_BUFSIZE, instructions[prim_opcode].mnemonic,
		         crfD, L, field2, field3);
		return;
	}

	/* twi TO, rA, SIMM */
	if (instructions[prim_opcode].field1 == D1_TO) {
		snprintf(s, R_ASM_BUFSIZE, instructions[prim_opcode].mnemonic,
		         field1, field2, field3);
		return;
	}

	/* ori, oris, xori, xoris, andi., andis. */
	if (prim_opcode >= 0x18 && prim_opcode <= 0x1d) {
		snprintf(s, R_ASM_BUFSIZE, instructions[prim_opcode].mnemonic,
		         field2, field1, field3);
		return;
	}

	if (instructions[prim_opcode].field2 == D2_RA_ZERO) {
		/* rA==0: use literal zero instead of r0 */
		if (field2 != 0x00)
			snprintf(reg, STR_BUF, "r%i", field2);

		if (instructions[prim_opcode].field3 == D3_D) {
			snprintf(s, R_ASM_BUFSIZE,
			         instructions[prim_opcode].mnemonic,
			         field1, field3, reg);
		} else {
			snprintf(s, R_ASM_BUFSIZE,
			         instructions[prim_opcode].mnemonic,
				 field1, reg, field3);
		}
	} else {
		if (instructions[prim_opcode].field3 == D3_D) {
			snprintf(s, R_ASM_BUFSIZE,
			         instructions[prim_opcode].mnemonic,
			         field1, field3, field2);
		} else {
			snprintf(s, R_ASM_BUFSIZE,
			         instructions[prim_opcode].mnemonic,
			         field1, field2, field3);
		}
	}
}

static void decode_xl_form(uint32_t i, uint16_t sec_opcode, uint32_t index, char *s)
{
	uint8_t field1, field2, field3;
	uint8_t crfD, crfS;

	field1 = i>>21 & 0x1f;
	field2 = i>>16 & 0x1f;
	field3 = i>>11 & 0x1f;

	/* mcrf */
	if (sec_opcode == 0x00) {
		crfD = field1>>2 & 0x7;
		crfS = field2>>2 & 0x7;
		snprintf(s, R_ASM_BUFSIZE, instructions_xl[index].mnemonic,
		         crfD, crfS);
		return;
	}

	if (instructions_xl[index].field1 == XL1_UNUSED) {
		snprintf(s, R_ASM_BUFSIZE, instructions_xl[index].mnemonic);
		return;
	}

	if (instructions_xl[index].field1 == XL1_BO) {
		snprintf(s, R_ASM_BUFSIZE, instructions_xl[index].mnemonic,
		         field1, field2);
		return;
	}

	snprintf(s, R_ASM_BUFSIZE, instructions_xl[index].mnemonic,
	         field1, field2, field3);
}

static void decode_x_form(uint32_t i, uint32_t index, char *s)
{
	uint8_t field1, field2, field3;
	uint8_t crfD, L;
	char reg[STR_BUF] = "0";
	uint8_t CRM;

	field1 = i>>21 & 0x1f;
	field2 = i>>16 & 0x1f;
	field3 = i>>11 & 0x1f;

	/* cmp/cmpl crfD, L, rA, rB */
	if (instructions_x[index].field1 == X1_CRFD_L) {
		crfD = field1>>2 & 0x7;
		L    = field1    & 0x1;
		snprintf(s, R_ASM_BUFSIZE, instructions_x[index].mnemonic,
		         crfD, L, field2, field3);
		return;
	}

	/* tw TO, rA, rB */
	if (instructions_x[index].field1 == X1_TO) {
		snprintf(s, R_ASM_BUFSIZE, instructions_x[index].mnemonic,
		         field1, field2, field3);
		return;
	}

	/* mcrxr crfD */
	if (instructions_x[index].field1 == X1_CRFD) {
		crfD = field1>>2 & 0x7;
		snprintf(s, R_ASM_BUFSIZE, instructions_x[index].mnemonic,
		         crfD);
		return;
	}

	/* tlbia, tlbsync, sync, eieio */
	if (instructions_x[index].field1 == X1_UNUSED
	    && instructions_x[index].field2 == X2_UNUSED
	    && instructions_x[index].field3 == X3_UNUSED) {
		snprintf(s, R_ASM_BUFSIZE, instructions_x[index].mnemonic);
		return;
	}

	/* mfcr, mfmsr, mtmsr */
	if (instructions_x[index].field1 != X1_UNUSED
	    && instructions_x[index].field2 == X2_UNUSED
	    && instructions_x[index].field3 == X3_UNUSED) {
		snprintf(s, R_ASM_BUFSIZE, instructions_x[index].mnemonic,
		         field1);
		return;
	}

	/* dcbst, dcbf, dcbi, dcba, icbi, dcbz, dcbtst, dcbt */
	if ((instructions_x[index].field1 == X1_UNUSED
	     || instructions_x[index].field1 == X1_CT)
	    && instructions_x[index].field2 == X2_RA_ZERO
	    && instructions_x[index].field3 == X3_RB) {
		/* rA == 0: use literal zero instead of r0 */
		if (field2 != 0)
			snprintf(reg, STR_BUF, "r%i", field2);
		snprintf(s, R_ASM_BUFSIZE, instructions_x[index].mnemonic,
		         reg, field3);
		return;
	}

	/* mtcrf CRM, rS */
	if (instructions_x[index].field2 == X2_CRM
	    && instructions_x[index].field3 == X3_CRM) {
		CRM = i>>12 & 0xff;
		snprintf(s, R_ASM_BUFSIZE, instructions_x[index].mnemonic,
		         CRM, field1);
		return;
	}

	/* mtsr, mfsr */
	if (instructions_x[index].field2 == X2_SR) {
		/* mtsr SR, rS */
		if (instructions_x[index].field1 == X1_RS) {
			snprintf(s, R_ASM_BUFSIZE, instructions_x[index].mnemonic,
			         field2 & 0xf, field1);
		} else {
		/* mfsr rD, SR */
			snprintf(s, R_ASM_BUFSIZE, instructions_x[index].mnemonic,
			         field1, field2 & 0xf);
		}
		return;
	}

	/* mfspr, mtspr */
	if (instructions_x[index].field2 == X2_SPR
	    && instructions_x[index].field3 == X3_SPR) {
		/* mfspr rD, SPR */
		if (instructions_x[index].field1 == X1_RD) {
			snprintf(s, R_ASM_BUFSIZE, instructions_x[index].mnemonic,
			        field1, (field3<<5 | field2) & 0x3ff);
		} else {
		/* mtspr SPR, rS */
			snprintf(s, R_ASM_BUFSIZE, instructions_x[index].mnemonic,
			         (field3<<5 | field2) & 0x3ff, field1);
		return;
		}
	}

	/* mftb rD, TBR */
	if (instructions_x[index].field2 == X2_TBR
	    && instructions_x[index].field3 == X3_TBR) {
		snprintf(s, R_ASM_BUFSIZE, instructions_x[index].mnemonic,
		         field1, (field3<<5 | field2) & 0x3ff);
		return;
	}

	/* mtsrin, mfsrin */
	if (instructions_x[index].field1 != X1_UNUSED
	    && instructions_x[index].field2 == X2_UNUSED
	    && instructions_x[index].field3 != X3_UNUSED) {
		snprintf(s, R_ASM_BUFSIZE, instructions_x[index].mnemonic,
		         field1, field3);
		return;
	}

	/* lswi, stswi */
	if (instructions_x[index].field3 == X3_NB) {
		/* rA == 0: use literal zero instead of r0 */
		if (field2 != 0)
			snprintf(reg, STR_BUF, "r%i", field2);
		/* NB == 0: load 32 bytes */
		if (field3 == 0)
			field3 = 32;
		snprintf(s, R_ASM_BUFSIZE, instructions_x[index].mnemonic,
		         field1, reg, field3);
		return;
	}

	/* tlbie rB */
	if (instructions_x[index].field1 == X1_UNUSED
	    && instructions_x[index].field2 == X2_UNUSED
	    && instructions_x[index].field3 != X3_UNUSED) {
		snprintf(s, R_ASM_BUFSIZE, instructions_x[index].mnemonic,
		         field3);
		return;
	}

	/* srawi, srawi. */
	if (instructions_x[index].field1 == X1_RS
	    && instructions_x[index].field2 == X2_RA
	    && instructions_x[index].field3 == X3_SH) {
		snprintf(s, R_ASM_BUFSIZE, instructions_x[index].mnemonic,
		         field2, field1, field3);
		return;
	}

	/* instructions with two registers as operands:
	 * cntlzw, cntlzw., neg, neg., subfze, subfze., addze, addze., subfme,
	 * subfme., addme, addme., nego, nego., subfzeo, subfzeo., subfmeo,
	 * subfmeo., addmeo, addmeo., extsh, extsh., extsb, extsb. */
	if ((instructions_x[index].field1 == X1_RS
	     || instructions_x[index].field1 == X1_RD)
	    && instructions_x[index].field2 == X2_RA
	    && instructions_x[index].field3 == X3_UNUSED) {
		if (instructions_x[index].field1 == X1_RD) {
			snprintf(s, R_ASM_BUFSIZE, instructions_x[index].mnemonic,
			         field1, field2);
		} else {
			snprintf(s, R_ASM_BUFSIZE, instructions_x[index].mnemonic,
			         field2, field1);
		}
		return;
	}

	/* instructions with three registers as operands:
	 * subfc, subfc., addc, addc., mulhwu, mulhwu., lwarx, lwzx, slw, slw.,
	 * and, and., subf, subf., lwzux, andc, andc., mulhw, mulhw., lbzx,
	 * lbzux, nor, nor., subfe, subfe., adde, adde., stwcx., stwx, stwux,
	 * stbx, mullw, mullw., stbux, lhzx, eqv, eqv., eciwx, lhzux, xor, xor.,
	 * lhax, lhaux, sthx, orc, orc., ecowx, sthux, or, or., divwu, divwu.,
	 * nand, nand., divw, divw., subfco, subfco., addco, addco., lswx, lbrx,
	 * srw, srw., subfo, subfo., subfeo, subfeo., addeo, addeo., stswx,
	 * stwbrx, mullwo, mullwo., addo, addo., lhbrx, sraw, sraw., sthbrx,
	 * divwuo, divwuo., divwo, divwo. */
	if ((instructions_x[index].field1 == X1_RS
	     || instructions_x[index].field1 == X1_RD)
	    && instructions_x[index].field2 != X2_UNUSED
	    && instructions_x[index].field3 == X3_RB) {
		/* instr rS, (rA|0), rB  or instr rD, (rA|0), rB */
		if (instructions_x[index].field2 == X2_RA_ZERO) {
			/* rA == 0: use literal zero instead of r0 */
			if (field2 != 0)
				snprintf(reg, STR_BUF, "r%i", field2);
			snprintf(s, R_ASM_BUFSIZE, instructions_x[index].mnemonic,
			         field1, reg, field3);
			return;
		}
		/* instr rD, rA, rB */
		if (instructions_x[index].field1 == X1_RD) {
			snprintf(s, R_ASM_BUFSIZE, instructions_x[index].mnemonic,
			         field1, field2, field3);
		} else {
		/* instr rA, rS, rB */
			snprintf(s, R_ASM_BUFSIZE, instructions_x[index].mnemonic,
			         field2, field1, field3);
		}
		return;
	}
}

static int disassemble(RAsm *a, RAsmOp *op, const ut8 *buf, int len)
{
	uint32_t pc, i;
	uint8_t prim_opcode;
	uint16_t sec_opcode;
	uint32_t k;
	int found;

	/* get current program counter */
	pc = a->pc;
	/* convert big endian instruction word to little endian */
	i = buf[0]<<24 | buf[1]<<16 | buf[2]<<8 | buf[3];
	/* all instructions are 32 bits wide */
	op->size = 4;

	/* extract primary opcode (bits 0:5) */
	prim_opcode = i >> 26;
	/* extract secondary opcode (bits 16:31) */
	sec_opcode = i & 0x7ff;

	/* only integer instructions are supported */
	if (prim_opcode > OPCODE_MAX)
		return 0;

	switch (instructions[prim_opcode].form) {
	case FORM_INV:
		/* invalid instruction */
		break;
	
	case FORM_B:
		/* conditional branch instructions */
		decode_b_form(i, pc, op->buf_asm);
		return 4;
		break;
	
	case FORM_D:
		decode_d_form(i, prim_opcode, op->buf_asm);
		return 4;
		break;
	
	case FORM_I:
		/* branch instructions */
		decode_i_form(i, pc, op->buf_asm);
		return 4;
		break;
	
	case FORM_M:
		/* rotate and shift instructions */
		decode_m_form(i, prim_opcode, op->buf_asm);
		return 4;
		break;
	
	case FORM_SC:
		/* sc instruction */
		snprintf(op->buf_asm, R_ASM_BUFSIZE, "%s",
		         instructions[prim_opcode].mnemonic);
		return 4;
		break;
	
	case FORM_X:
		found = 0;
		for (k = 0; k < sizeof instructions_x/sizeof(struct INSTR_X); ++k) {
			if (instructions_x[k].opcode == sec_opcode) {
				found = 1;
				break;
			}
		}
		if (found) {
			decode_x_form(i, k, op->buf_asm);
			return 4;
		}
		break;

	case FORM_XL:
		found = 0;
		for (k = 0; k < sizeof instructions_xl/sizeof(struct INSTR_XL); ++k) {
			if (instructions_xl[k].opcode == sec_opcode) {
				found = 1;
				break;
			}
		}
		if (found) {
			decode_xl_form(i, sec_opcode, k, op->buf_asm);
			return 4;
		}
		break;
	}

	/* invalid instruction */
	return 0;
}

RAsmPlugin r_asm_plugin_powerpc = {
        .name = "powerpc-plugin",
        .arch = "PowerPC",
        .license = "MIT License",
        .bits = 32,
        .desc = "PowerPC plugin",
        .disassemble = &disassemble,
	.init = NULL,
	.fini = NULL,
	.modify = NULL,
	.assemble = NULL
};

#ifndef CORELIB
struct r_lib_struct_t radare_plugin = {
        .type = R_LIB_TYPE_ASM,
        .data = &r_asm_plugin_powerpc
};
#endif
