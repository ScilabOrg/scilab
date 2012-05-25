/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __GW_EXTERNAL_OBJECTS_H__
#define __GW_EXTERNAL_OBJECTS_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_external_objects_scilab.h"
/*--------------------------------------------------------------------------*/
EXTERNAL_OBJECTS_SCILAB_IMPEXP int gw_external_objects(void);

/*--------------------------------------------------------------------------*/
int sci_percent_EObj_e(char * fname, unsigned long fname_len);
int sci_percent_EObj_p(char * fname, unsigned long fname_len);
int sci_percent_EClass_e(char * fname, unsigned long fname_len);
int sci_percent_foo_i_EObj(char * fname, unsigned long fname_len);
int sci_percent_EObj_a_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_d_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_g_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_h_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_j_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_k_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_l_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_m_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_n_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_o_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_p_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_q_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_r_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_s_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_t_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_x_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_y_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_z_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_1_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_2_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_3_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_4_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_0(char * fname, unsigned long fname_len);
int sci_percent_EObj_5(char * fname, unsigned long fname_len);
int sci_percent_EObj_t(char * fname, unsigned long fname_len);
int sci_invoke_lu(char * fname, unsigned long fname_len);
int sci_doubleExclam_invoke_(char * fname, unsigned long fname_len);
int sci_percent_EObj_disp(char * fname, unsigned long fname_len);
int sci_pyImport(char * fname, unsigned long fname_len);
int sci_pyAutounwrap(char * fname, unsigned long fname_len);
int sci_pyGetMethods(char * fname, unsigned long fname_len);
int sci_pyGetFields(char * fname, unsigned long fname_len);
int sci_pyDeff(char * fname, unsigned long fname_len);
int sci_pyRepr(char * fname, unsigned long fname_len);
int sci_pyExists(char * fname, unsigned long fname_len);
int sci_pyGetField(char * fname, unsigned long fname_len);
int sci_pyRemove(char * fname, unsigned long fname_len);
int sci_pyUnwrap(char * fname, unsigned long fname_len);
int sci_pyWrap(char * fname, unsigned long fname_len);
int sci_pyAllowModuleReloading(char * fname, unsigned long fname_len);
int sci_pyGetInfos(char * fname, unsigned long fname_len);
int sci_pyGetPath(char * fname, unsigned long fname_len);
int sci_pyAddToPath(char * fname, unsigned long fname_len);
int sci_pyTuple(char * fname, unsigned long fname_len);
int sci_pyDict(char * fname, unsigned long fname_len);
int sci_pyList(char * fname, unsigned long fname_len);
int sci_pySet(char * fname, unsigned long fname_len);
int sci_pyGetName(char * fname, unsigned long fname_len);
int sci_pyEvalStr(char * fname, unsigned long fname_len);
int sci_pyWrapAsRef(char * fname, unsigned long fname_len);
int sci_pyUnwrapRem(char * fname, unsigned long fname_len);
int sci_pyByCopy(char * fname, unsigned long fname_len);
int sci_pyUseNumpy(char * fname, unsigned long fname_len);
int sci_pyUseScilabIndex(char * fname, unsigned long fname_len);
int sci_pyGetEnvId(char * fname, unsigned long fname_len);
int sci_pyKill(char * fname, unsigned long fname_len);
int sci_pyTrace(char * fname, unsigned long fname_len);
int sci_pyAddVar(char * fname, unsigned long fname_len);
int sci_pyGetVar(char * fname, unsigned long fname_len);
int sci_pyShowPrivate(char * fname, unsigned long fname_len);
int sci_pyAttachModule(char * fname, unsigned long fname_len);
int sci_pyGetAttr(char * fname, unsigned long fname_len);
int sci_pyBuiltin(char * fname, unsigned long fname_len);
int sci_pyMain(char * fname, unsigned long fname_len);
int sci_pyInvoke(char * fname, unsigned long fname_len);
int sci_pyNpWrap(char * fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /* __GW_EXTERNAL_OBJECTS_H__ */
/*--------------------------------------------------------------------------*/
