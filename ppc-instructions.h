#pragma once

#include <stdint.h>

/* max. primary opcode (integer instructions only) */
#define OPCODE_MAX 0x2f

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
