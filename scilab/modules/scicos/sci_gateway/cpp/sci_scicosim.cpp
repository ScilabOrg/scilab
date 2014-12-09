/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <string>

#include "gw_scicos.hxx"

#include "internal.hxx"
#include "types.hxx"
#include "double.hxx"
#include "function.hxx"
#include "list.hxx"
#include "tlist.hxx"
#include "string.hxx"

#include "il_state.hxx"
#include "il_sim.hxx"

extern "C"
{
#include <string.h>

#include "wchar.h"
#include "stack-c.h"
#include "scicos-def.h"
#include "localization.h"
#include "Scierror.h"
}

/*--------------------------------------------------------------------------*/
/* intsicosimc scicosim interface routine.
*
* [state,t] = scicosim(state,tcur,tf,sim,str,tol)
*
* rhs 1 state : Tlist
*        - 1  : state(1)     : !xcs  x  z  oz iz  tevts  evtspt  pointi  outtb  !
*        - 2  : state.x      : column vector of real
*        - 3  : state.z      : column vector of real
*        - 4  : state.oz     : list of scilab object
*        - 5  : state.iz     : column vector of real (empty object with flag "finish")
*        - 6  : state.tevts  : column vector of real
*        - 7  : state.evtspt : column vector of real
*        - 8  : state.pointi : real scalar
*        - 9  : state.outtb  : list of scilab object
* rhs 2 tcur  : real scalar
* rhs 3 tf    : real scalar
* rhs 4 sim   : Tlist
*        - 1  : sim(1) : !scs    funs    xptr    zptr    ozptr   zcptr   inpptr
*                         outptr inplnk  outlnk  rpar    rpptr   ipar    ipptr
*                         opar   opptr   clkptr  ordptr  execlk  ordclk  cord
*                         oord   zord    critev  nb      ztyp    nblk    ndcblk
*                         subscr funtyp  iord    labels  modptr  uids !
*        - 2  : sim.funs   : list of strings and/or scilab function
*        - 3  : sim.xptr   : column vector of real
*        - 4  : sim.zptr   : column vector of real
*        - 5  : sim.ozptr  : column vector of real
*        - 6  : sim.zcptr  : column vector of real
*        - 7  : sim.inpptr : column vector of real
*        - 8  : sim.outptr : column vector of real
*        - 9  : sim.inplnk : column vector of real
*        - 10 : sim.outlnk : column vector of real
*        - 11 : sim.rpar   : column vector of real
*        - 12 : sim.rpptr  : column vector of real
*        - 13 : sim.ipar   : column vector of real
*        - 14 : sim.ipptr  : column vector of real
*        - 15 : sim.opar   : list of scilab object
*        - 16 : sim.opptr  : column vector of real
*        - 17 : sim.clkptr : column vector of real
*        - 18 : sim.ordptr : column vector of real
*        - 19 : sim.execlk : matrix of real
*        - 20 : sim.ordclk : matrix of real
*        - 21 : sim.cord   : matrix of real
*        - 22 : sim.oord   : matrix of real
*        - 23 : sim.zord   : column vector ? of real
*        - 24 : sim.critev : column vector of real
*        - 25 : sim.nb     : real scalar
*        - 26 : sim.ztyp   : column vector of real
*        - 27 : sim.nblk   : real scalar
*        - 28 : sim.ndcblk : real scalar
*        - 29 : sim.subscr : column vector of real
*        - 30 : sim.funtyp : column vector of real
*        - 31 : sim.iord   : column vector of real
*        - 32 : sim.labels : column vector of strings
*        - 33 : sim.modptr : column vector of real
*        - 34 : sim.uids : column vector of strings
*
* rhs 5 str   : string flag : 'start','run','finish','linear'
* rhs 6 tol   : real vector of size (7,1) minimum (4,1)
*               [atol rtol ttol [deltat realtimescale solver hmax]]'
*
* 16/03/06, A.Layec : Rewritten from original fortran
*                     source code intsscicos in intcos.f.
*
* 29/03/06, Alan    : Improvement in accordance to c_pass2
*                     (int32 parameters).
*
* 31/05/06, Alan    : Add global variable int *il_state_save
*                     and int *il_sim_save in intcscicos.h to store
*                     stack address of list %cpr.state and %cpr.sim
*                     (to use with get/setscicosvars).
*
* 14/06/06, Alan    : Save common intersci before calling scicos
*                     (to disable scilab crash with scifunc.f).
*
* 13/11/06, Alan    : Get back to double parameters for sim and state
*                     (for better compatibility with scilab-4.x families).
*                     Remove il_sim_save global variable.
*
* 15/12/06, Alan    : Warnings compilation removed.
*                     This can crash scilab/scicos.
*                     Please report.
*
* xx/02/07, Alan    : Add opar/odstate : scilab lists of arbitrary object
*                     parameters/states.
*
* 08/12/14, Paul    : Rewrite to C++.
*
*/
/*--------------------------------------------------------------------------*/

const std::string funname = "scicosim";

#define freeparam \
	FREE(l_sim_iord);\
	FREE(l_sim_funtyp);\
	FREE(l_sim_subscr);\
	FREE(l_sim_ndcblk);\
	FREE(l_sim_nblk);\
	FREE(l_sim_ztyp);\
	FREE(l_sim_nb);\
	FREE(l_sim_critev);\
	FREE(l_sim_zord);\
	FREE(l_sim_oord);\
	FREE(l_sim_cord);\
	FREE(l_sim_ordclk);\
	FREE(l_sim_execlk);\
	FREE(l_sim_ordptr);\
	FREE(l_sim_clkptr);\
	FREE(l_sim_rpptr);\
	FREE(l_sim_ipar);\
	FREE(l_sim_ipptr);\
	FREE(l_sim_opptr);\
	FREE(l_sim_outlnk);\
	FREE(l_sim_inplnk);\
	FREE(l_sim_outptr);\
	FREE(l_sim_inpptr);\
	FREE(l_sim_zcptr);\
	FREE(l_sim_ozptr);\
	FREE(l_sim_zptr);\
	FREE(l_sim_xptr);\
	FREE(l_sim_modptr);\
	FREE(l_state_evtspt);\
	FREE(l_pointi)
/*--------------------------------------------------------------------------*/
COSIM_struct C2F(cosim); /* declaration of cosim -valable partout- */
/*--------------------------------------------------------------------------*/
extern COSERR_struct coserr;    /* declaration of coserr -defined in scicos- */
extern int C2F(namstr)();
/*--------------------------------------------------------------------------*/
/* variable defined in scicos.c */
extern CURBLK_struct C2F(curblk);
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_scicosim(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    /************************************
    * Variables and constants definition
    ************************************/
    static int id[nsiz];

    // Declaration of static structure
    static struct
    {
        int idb;
    } C2F(dbcos);   // Declaration of dbcos

    typedef struct inter_s_
    {
        int iwhere, nbrows, nbcols, itflag, ntypes, lad, ladc, lhsvar;
    } intersci_state ;

    typedef struct inter_l
    {
        intersci_state *state ;
        int nbvars;
    } intersci_list ;

    double simpar[7];

    BOOL allocatedError = FALSE;
    const int MAX_ERROR_LEN = 512;

    if (in.size() != 6)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), funname.data(), 6);
        return types::Function::Error;
    }

    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), funname.data(), 1, 2);
        return types::Function::Error;
    }

    /****************
    * state (rhs 1)
    ****************/
    if (in[0]->isTList() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A tlist expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }
    types::TList* il_state = in[0]->getAs<types::TList>();

    if (il_state->getSize() < 9)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : %d elements expected.\n"), funname.data(), 1, 9);
        return types::Function::Error;
    }

    // Make a copy of 'il_state' in a global variabe
    set_il_state(il_state);

    /*2 : state.x      */
    if (il_state->get(1)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 2, 1);
        return types::Function::Error;
    }
    types::Double* il_state_x = il_state->get(1)->getAs<types::Double>();
    double* l_state_x = il_state_x->get();
    int m1e2 = il_state_x->getRows();
    int n1e2 = il_state_x->getCols();

    /*3 : state.z      */
    if (il_state->get(2)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 3, 1);
        return types::Function::Error;
    }
    types::Double* il_state_z = il_state->get(2)->getAs<types::Double>();
    double* l_state_z = il_state_z->get();
    int m1e3 = il_state_z->getRows();
    int n1e3 = il_state_z->getCols();

    /*4 : state.oz     */
    if (il_state->get(3)->isList() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A list expected.\n"), funname.data(), 4, 1);
        return types::Function::Error;
    }
    types::List* il_state_oz = il_state->get(3)->getAs<types::List>();
    int noz = il_state_oz->getSize();

    /*5 : state.iz     */
    if (il_state->get(4)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 5, 1);
        return types::Function::Error;
    }
    types::Double* il_state_iz = il_state->get(4)->getAs<types::Double>();
    double* l_state_iz = il_state_iz->get();
    int m1e5 = il_state_iz->getRows();
    int n1e5 = il_state_iz->getCols();

    /*6 : state.tevts  */
    if (il_state->get(5)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 6, 1);
        return types::Function::Error;
    }
    types::Double* il_state_tevts = il_state->get(5)->getAs<types::Double>();
    double* l_state_tevts = il_state_tevts->get();
    int m1e6 = il_state_tevts->getRows();
    int n1e6 = il_state_tevts->getCols();

    /*7 : state.evtspt */
    if (il_state->get(6)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 7, 1);
        return types::Function::Error;
    }
    types::Double* il_state_evtspt = il_state->get(6)->getAs<types::Double>();
    double* l_state_evtspt = il_state_evtspt->get();
    int m1e7 = il_state_evtspt->getRows();
    int n1e7 = il_state_evtspt->getCols();

    /*8 : state.pointi */
    if (il_state->get(7)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 8, 1);
        return types::Function::Error;
    }
    types::Double* il_pointi = il_state->get(7)->getAs<types::Double>();
    double* l_pointi = il_pointi->get();
    int m1e8 = il_pointi->getRows();
    int n1e8 = il_pointi->getCols();

    /*9 : state.outtb  */
    if (il_state->get(8)->isList() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A list expected.\n"), funname.data(), 9, 1);
        return types::Function::Error;
    }
    types::List* il_state_outtb = il_state->get(8)->getAs<types::List>();
    int nlnk = il_state_outtb->getSize(); // 'nlnk' is the dimension of the list state_louttb

    /***************
    * tcur (rhs 2)
    ***************/
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), funname.data(), 2);
        return types::Function::Error;
    }
    types::Double* il_tcur = in[1]->getAs<types::Double>();
    if (il_tcur->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : A scalar expected.\n"), funname.data(), 2);
        return types::Function::Error;
    }
    double* l_tcur = il_tcur->get();
    int m2 = 1;
    int n2 = 1;

    /*************
    * tf (rhs 3)
    *************/
    if (in[2]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), funname.data(), 3);
        return types::Function::Error;
    }
    types::Double* il_tf = in[2]->getAs<types::Double>();
    if (il_tf->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : A scalar expected.\n"), funname.data(), 3);
        return types::Function::Error;
    }
    double* l_tf = il_tf->get();
    int m3 = 1;
    int n3 = 1;

    /*************
    * sim (rhs 4)
    *************/
    if (in[3]->isTList() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A tlist expected.\n"), funname.data(), 4);
        return types::Function::Error;
    }
    types::TList* il_sim = in[0]->getAs<types::TList>();

    // Make a copy of 'il_sim' in a global variabe
    set_il_sim(il_sim);

    int m4 = il_sim->getSize();
    if (m4 < 34)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : %d elements expected.\n"), funname.data(), 4, 34);
        return types::Function::Error;
    }

    /*2  : sim.funs*/
    if (il_sim->get(1)->isList() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A list expected.\n"), funname.data(), 2, 4);
        return types::Function::Error;
    }
    types::List* il_sim_fun = il_sim->get(1)->getAs<types::List>();
    int nblk = il_sim_fun->getSize(); // nblk is the dimension of the list sim.funs

    /*3  : sim.xptr   */
    if (il_sim->get(2)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 3, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_xptr = il_sim->get(2)->getAs<types::Double>();
    double* l_sim_xptr = il_sim_xptr->get();
    int m_xptr = il_sim_xptr->getRows();
    int n_xptr = il_sim_xptr->getCols();

    /*4  : sim.zptr   */
    if (il_sim->get(3)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 4, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_zptr = il_sim->get(3)->getAs<types::Double>();
    double* l_sim_zptr = il_sim_zptr->get();
    int m_zptr = il_sim_zptr->getRows();
    int n_zptr = il_sim_zptr->getCols();

    /*5  : sim.ozptr   */
    if (il_sim->get(4)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 5, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_ozptr = il_sim->get(4)->getAs<types::Double>();
    double* l_sim_ozptr = il_sim_ozptr->get();
    int m_ozptr = il_sim_ozptr->getRows();
    int n_ozptr = il_sim_ozptr->getCols();

    /*6  : sim.zcptr  */
    if (il_sim->get(5)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 6, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_zcptr = il_sim->get(5)->getAs<types::Double>();
    double* l_sim_zcptr = il_sim_zcptr->get();
    int m_zcptr = il_sim_zcptr->getRows();
    int n_zcptr = il_sim_zcptr->getCols();

    /*7  : sim.inpptr */
    if (il_sim->get(6)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 7, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_inpptr = il_sim->get(6)->getAs<types::Double>();
    double* l_sim_inpptr = il_sim_inpptr->get();
    int m_inpptr = il_sim_inpptr->getRows();
    int n_inpptr = il_sim_inpptr->getCols();

    /*8  : sim.outptr */
    if (il_sim->get(7)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 8, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_outptr = il_sim->get(7)->getAs<types::Double>();
    double* l_sim_outptr = il_sim_outptr->get();
    int m_outptr = il_sim_outptr->getRows();
    int n_outptr = il_sim_outptr->getCols();

    /*9  : sim.inplnk */
    if (il_sim->get(8)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 9, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_inplnk = il_sim->get(8)->getAs<types::Double>();
    double* l_sim_inplnk = il_sim_inplnk->get();
    int m_inplnk = il_sim_inplnk->getRows();
    int n_inplnk = il_sim_inplnk->getCols();

    /*10  : sim.outlnk */
    if (il_sim->get(9)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 10, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_outlnk = il_sim->get(9)->getAs<types::Double>();
    double* l_sim_outlnk = il_sim_outlnk->get();
    int m_outlnk = il_sim_outlnk->getRows();
    int n_outlnk = il_sim_outlnk->getCols();

    /*11 : sim.rpar   */
    if (il_sim->get(10)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 11, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_rpar = il_sim->get(10)->getAs<types::Double>();
    double* l_sim_rpar = il_sim_rpar->get();
    int m_rpar = il_sim_rpar->getRows();
    int n_rpar = il_sim_rpar->getCols();

    /*12 : sim.rpptr  */
    if (il_sim->get(11)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 12, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_rpptr = il_sim->get(11)->getAs<types::Double>();
    double* l_sim_rpptr = il_sim_rpptr->get();
    int m_rpptr = il_sim_rpptr->getRows();
    int n_rpptr = il_sim_rpptr->getCols();

    /*13 : sim.ipar   */
    if (il_sim->get(12)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 13, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_ipar = il_sim->get(12)->getAs<types::Double>();
    double* l_sim_ipar = il_sim_ipar->get();
    int m_ipar = il_sim_ipar->getRows();
    int n_ipar = il_sim_ipar->getCols();

    /*14 : sim.ipptr  */
    if (il_sim->get(13)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 14, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_ipptr = il_sim->get(13)->getAs<types::Double>();
    double* l_sim_ipptr = il_sim_ipptr->get();
    int m_ipptr = il_sim_ipptr->getRows();
    int n_ipptr = il_sim_ipptr->getCols();

    /*15 : sim.opar   */
    if (il_sim->get(14)->isList() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A list expected.\n"), funname.data(), 15, 4);
        return types::Function::Error;
    }
    types::List* il_sim_opar = il_sim->get(14)->getAs<types::List>();
    int nopar = il_sim_opar->getSize();

    /*16 : sim.opptr  */
    if (il_sim->get(15)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 16, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_opptr = il_sim->get(16)->getAs<types::Double>();
    double* l_sim_opptr = il_sim_opptr->get();
    int m_opptr = il_sim_opptr->getRows();
    int n_opptr = il_sim_opptr->getCols();

    /*17 : sim.clkptr */
    if (il_sim->get(16)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 17, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_clkptr = il_sim->get(16)->getAs<types::Double>();
    double* l_sim_clkptr = il_sim_clkptr->get();
    int m_clkptr = il_sim_clkptr->getRows();
    int n_clkptr = il_sim_clkptr->getCols();

    /*18 : sim.ordptr */
    if (il_sim->get(17)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 18, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_ordptr = il_sim->get(17)->getAs<types::Double>();
    double* l_sim_ordptr = il_sim_ordptr->get();
    int m_ordptr = il_sim_ordptr->getRows();
    int n_ordptr = il_sim_ordptr->getCols();

    /*19 : sim.execlk */
    if (il_sim->get(18)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 19, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_execlk = il_sim->get(18)->getAs<types::Double>();
    double* l_sim_execlk = il_sim_execlk->get();
    int m_execlk = il_sim_execlk->getRows();
    int n_execlk = il_sim_execlk->getCols();

    /*20 : sim.ordclk */
    if (il_sim->get(19)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 20, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_ordclk = il_sim->get(19)->getAs<types::Double>();
    double* l_sim_ordclk = il_sim_ordclk->get();
    int m_ordclk = il_sim_ordclk->getRows();
    int n_ordclk = il_sim_ordclk->getCols();

    /*21 : sim.cord   */
    if (il_sim->get(20)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 21, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_cord = il_sim->get(20)->getAs<types::Double>();
    double* l_sim_cord = il_sim_cord->get();
    int m_cord = il_sim_cord->getRows();
    int n_cord = il_sim_cord->getCols();

    /*22 : sim.oord   */
    if (il_sim->get(21)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 22, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_oord = il_sim->get(21)->getAs<types::Double>();
    double* l_sim_oord = il_sim_oord->get();
    int m_oord = il_sim_oord->getRows();
    int n_oord = il_sim_oord->getCols();

    /*23 : sim.zord   */
    if (il_sim->get(22)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 23, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_zord = il_sim->get(22)->getAs<types::Double>();
    double* l_sim_zord = il_sim_zord->get();
    int m_zord = il_sim_zord->getRows();
    int n_zord = il_sim_zord->getCols();

    /*24 : sim.critev */
    if (il_sim->get(23)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 24, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_critev = il_sim->get(23)->getAs<types::Double>();
    double* l_sim_critev = il_sim_critev->get();
    int m_critev = il_sim_critev->getRows();
    int n_critev = il_sim_critev->getCols();

    /*25 : sim.nb     */
    if (il_sim->get(24)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 25, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_nb = il_sim->get(24)->getAs<types::Double>();
    double* l_sim_nb = il_sim_nb->get();
    int m_nb = il_sim_nb->getRows();
    int n_nb = il_sim_nb->getCols();

    if (static_cast<int>(l_sim_nb[0]) != nblk) // Value of 'nb' must be equal to 'nblk'
    {
        Scierror(42, _("%s : Incompatible sim.nb RHS parameter.\n"), funname.data());
        return types::Function::Error;
    }

    /*26 : sim.ztyp   */
    if (il_sim->get(25)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 26, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_ztyp = il_sim->get(25)->getAs<types::Double>();
    double* l_sim_ztyp = il_sim_ztyp->get();
    int m_ztyp = il_sim_ztyp->getRows();
    int n_ztyp = il_sim_ztyp->getCols();

    /*27 : sim.nblk   */
    if (il_sim->get(26)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 27, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_nblk = il_sim->get(26)->getAs<types::Double>();
    double* l_sim_nblk = il_sim_nblk->get();
    int m_nblk = il_sim_nblk->getRows();
    int n_nblk = il_sim_nblk->getCols();

    /*28 : sim.ndcblk */
    if (il_sim->get(27)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 28, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_ndcblk = il_sim->get(27)->getAs<types::Double>();
    double* l_sim_ndcblk = il_sim_ndcblk->get();
    int m_ndcblk = il_sim_ndcblk->getRows();
    int n_ndcblk = il_sim_ndcblk->getCols();

    /*29 : sim.subscr */
    if (il_sim->get(28)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 29, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_subscr = il_sim->get(28)->getAs<types::Double>();
    double* l_sim_subscr = il_sim_subscr->get();
    int m_subscr = il_sim_subscr->getRows();
    int n_subscr = il_sim_subscr->getCols();

    /*30 : sim.funtyp */
    if (il_sim->get(29)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 30, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_funtyp = il_sim->get(29)->getAs<types::Double>();
    double* l_sim_funtyp = il_sim_funtyp->get();
    int m_funtyp = il_sim_funtyp->getRows();
    int n_funtyp = il_sim_funtyp->getCols();

    /*31 : sim.iord   */
    if (il_sim->get(30)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 31, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_iord = il_sim->get(30)->getAs<types::Double>();
    double* l_sim_iord = il_sim_iord->get();
    int m_iord = il_sim_iord->getRows();
    int n_iord = il_sim_iord->getCols();

    /*32 : sim.labels */
    if (il_sim->get(31)->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A string matrix expected.\n"), funname.data(), 32, 4);
        return types::Function::Error;
    }
    types::String* il_sim_labels = il_sim->get(31)->getAs<types::String>();
    wchar_t** l_sim_labels = il_sim_labels->get();
    int m_labels = il_sim_labels->getRows();
    int n_labels = il_sim_labels->getCols();

    /*33 : sim.modptr */
    if (il_sim->get(32)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 33, 4);
        return types::Function::Error;
    }
    types::Double* il_sim_modptr = il_sim->get(32)->getAs<types::Double>();
    double* l_sim_modptr = il_sim_modptr->get();
    int m_modptr = il_sim_modptr->getRows();
    int n_modptr = il_sim_modptr->getCols();

    /*34 : sim.uids */
    if (il_sim->get(33)->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A string matrix expected.\n"), funname.data(), 34, 4);
        return types::Function::Error;
    }
    types::String* il_sim_uids = il_sim->get(33)->getAs<types::String>();
    wchar_t** l_sim_uids = il_sim_uids->get();
    int m_uids = il_sim_uids->getRows();
    int n_uids = il_sim_uids->getCols();

    /*************
    * str (rhs 5)
    *************/
    if (in[4]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), funname.data(), 5);
        return types::Function::Error;
    }
    types::String* il_str = in[4]->getAs<types::String>();
    if (il_str->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : A scalar expected.\n"), funname.data(), 5);
        return types::Function::Error;
    }
    int m5 = 1;
    int n5 = 1;

    const std::wstring start  (L"start");
    const std::wstring run    (L"run");
    const std::wstring finish (L"finish");
    const std::wstring linear (L"linear");
    const std::wstring Kinsol (L"Kinsol");
    int flag;
    if (il_str->get(0) == start)
    {
        flag = 1;
    }
    else if (il_str->get(0) == run)
    {
        flag = 2;
    }
    else if (il_str->get(0) == finish)
    {
        flag = 3;
    }
    else if (il_str->get(0) == linear)
    {
        flag = 4;
    }
    else if (il_str->get(0) == Kinsol)
    {
        flag = 5;
    }
    else
    {
        Scierror(44, _("%s: Wrong value for input argument #%d : ""%s"", ""%s"" ""%s"" ""%s"" or ""%s"" expected.\n"), funname.data(), 5,
                 "start", "run", "finish", "linear", "Kinsol");
        return types::Function::Error;
    }

    /*************
    * tol (rhs 6)
    *************/
    if (in[5]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), funname.data(), 6);
        return types::Function::Error;
    }
    types::Double* il_tol = in[5]->getAs<types::Double>();
    int m6 = il_tol->getSize();
    if (m6 < 4 || m6 > 7) // Check if 'tol' has 4 to 7 elements
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : %d to %d elements expected.\n"), funname.data(), 6, 4, 7);
        return types::Function::Error;
    }

    /******************
    * set simpar array
    ******************/
    for (int i = 0; i < m6; ++i)
    {
        simpar[i] = il_tol->get(i);
    }
    for (int i = m6; i < 7; ++i)
    {
        simpar[i] = 0;
    }
    int solver = static_cast<int>(simpar[5]); // Set 'solver' variable

    /******************************
    * Cross variable size checking
    ******************************/
    int err_check = 0;
    if (m1e6 != m1e7)
    {
        err_check = 1;    // tevts vs evtspt
    }
    else if (m_xptr != m_zptr)
    {
        err_check = 2;    // xptr vs zptr
    }
    else if (m_xptr != m_ozptr)
    {
        err_check = 3;    // xptr vs ozptr
    }
    else if (m_xptr != m_zcptr)
    {
        err_check = 4;    // xptr vs zcptr
    }
    else if (m_xptr != m_inpptr)
    {
        err_check = 5;    // xptr vs inpptr
    }
    else if (m_xptr != m_outptr)
    {
        err_check = 6;    // xptr vs outptr
    }
    else if (m_xptr != m_rpptr)
    {
        err_check = 7;    // xptr vs rpptr
    }
    else if (m_xptr != m_ipptr)
    {
        err_check = 8;    // xptr vs ipptr
    }
    else if (m_xptr != m_opptr)
    {
        err_check = 8;    // xptr vs opptr
    }
    else if (m_xptr != m_clkptr)
    {
        err_check = 10;    // xptr vs clkptr
    }
    else if ((n_ordclk != 2) & (m_ordclk != 0))
    {
        err_check = 11;    // sim.ordclk
    }
    else if ((n_cord != 2) & (m_cord != 0))
    {
        err_check = 12;    // sim.cord
    }
    else if ((n_oord != 2) & (m_oord != 0))
    {
        err_check = 13;    // sim.oord
    }
    else if ((n_zord != 2) & (m_zord != 0))
    {
        err_check = 14;    // sim.zord
    }
    else if ((n_iord != 2) & (m_iord != 0))
    {
        err_check = 15;    // sim.iord
    }
    if (err_check != 0)
    {
        // Please write an error table here
        Scierror(42, _("%s : error in cross variable size checking : %d\n"), funname.data(), err_check);
        return types::Function::Error;
    }

    types::InternalType* pOut;
    out.push_back(pOut);

    return types::Function::OK;
}
