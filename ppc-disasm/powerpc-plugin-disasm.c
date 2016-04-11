#include "../ppc-instructions.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <r_asm.h>
#include <r_lib.h>
#include <r_types.h>

#define STR_BUF 20

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
