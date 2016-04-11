#include "../ppc-instructions.h"
#include <string.h>
#include <r_types.h>
#include <r_lib.h>
#include <r_asm.h>
#include <r_anal.h>

static int powerpc_analyse(RAnal *anal, RAnalOp *op, ut64 addr, const ut8 *data, int len)
{
	memset(op, '\0', sizeof(RAnalOp));
	op->size = 4;
	op->addr = addr;
	op->type = R_ANAL_OP_TYPE_UNK;
	return op->size;
}

struct r_anal_plugin_t r_analysis_plugin_powerpc = {
	.name = "powerpc-plugin",
	.desc = "PowerPC analysis plugin",
	.license = "MIT License",
	.arch = R_SYS_ARCH_NONE,
	.bits = 32,
	.init = NULL,
	.fini = NULL,
	.op = &powerpc_analyse,
	.set_reg_profile = NULL,
	.fingerprint_bb = NULL,
	.fingerprint_fcn = NULL,
	.diff_bb = NULL,
	.diff_fcn = NULL,
	.diff_eval = NULL
};

#ifndef CORELIB
struct r_lib_struct_t radare_plugin = {
	.type = R_LIB_TYPE_ANAL,
	.data = &r_analysis_plugin_powerpc,
	.version = R2_VERSION
};
#endif
