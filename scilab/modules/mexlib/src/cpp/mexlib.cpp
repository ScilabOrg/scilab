/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------
 *    mexlib  library
 *
 *    This library emulates Matlab' API functions. It is not fully tested...
 *    -Assumes that Scilab string matrices have one column, e.g.
 *    Str=["qwerty";"123456"]; here this is a 2 x 6 matrix but Scilab
 *    considers Str as a 2 x 1 matrix. ["123";"1234"] is a valid string
 *    matrix which cannot be used here.
 *    -Assumes that sparse matrices have been converted into the Matlab
 *    format. Scilab sparse matrices are stored in the transposed Matlab
 *    format. If A is a sparse Scilab matrix, it should be converted
 *    by the command A=mtlb_sparse(A) in the calling sequence of the
 *    mex function.
 *    -Structs and Cells are Scilab mlists:
 *    Struct=mlist(["st","dims","field1",...,"fieldk"],
 *                 int32([d1,d2,...,dn]),
 *                 list(obj1,      objN),
 *                 .....
 *                 list(obj1,      objN))     k such lists
 *           N = d1 x d2    x dn
 *           obj = Scilab variable or pointer to Scilab variable.
 *     Cell = Struct with one field called "entries" and "st" <- "ce"
 *    One dimensional structs or cells are as follows:
 *    Struct=mlist(["st","dims","field1",...,"fieldk"],
 *                 int32([1,1]),
 *                 obj1,...,objk)
 *
 *    -Nd dimensional arrays are Scilab mlists (for Nd > 2):
 *     X = mlist(["hm","dims","entries"],
 *                 int32([d1,d2,...,dn]),
 *                 values)
 *     values = vector of doubles or int8-16-32 or char
 --------------------------------------------------------------------------*/

#include <limits>

#include "types.hxx"
#include "int8.hxx"
#include "uint8.hxx"
#include "int16.hxx"
#include "uint16.hxx"
#include "int32.hxx"
#include "uint32.hxx"
#include "int64.hxx"
#include "uint64.hxx"
#include "double.hxx"
#include "bool.hxx"
#include "string.hxx"

extern "C"
{
#include "machine.h"
#include "mex.h"
}

static char *the_current_mex_name;

mxClassID mxGetClassID(const mxArray *ptr)
{
    // FIXME : use getType() enum
    return mxUNKNOWN_CLASS;
}

bool mxIsInt8(const mxArray *ptr)
{
    return mxGetClassID(ptr)==mxINT8_CLASS;
}

bool mxIsInt16(const mxArray *ptr)
{
    return mxGetClassID(ptr)==mxINT16_CLASS;
}

bool mxIsInt32(const mxArray *ptr)
{
    return mxGetClassID(ptr)==mxINT32_CLASS;
}

bool mxIsUint8(const mxArray *ptr)
{
    return mxGetClassID(ptr)==mxUINT8_CLASS;
}

bool mxIsUint16(const mxArray *ptr)
{
    return mxGetClassID(ptr)==mxUINT16_CLASS;
}

bool mxIsUint32(const mxArray *ptr)
{
    return mxGetClassID(ptr)==mxUINT32_CLASS;
}

double mxGetEps(void)
{
    return EPSILON;
}


double mxGetInf(void)
{
    return std::numeric_limits<double>::infinity();
}

double mxGetNaN(void)
{
    double x,y;
    x=mxGetInf();
    y=x/x;
    return y;
}

bool mxIsInf(double x)
{
    if (x == x+1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool mxIsFinite(double x)
{
    if (x < x + 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool mxIsNaN(double x)
{
    if ( x != x )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

mxArray *mxCreateData(int m)
/* Utility fctn : create a no-header, unpopulated  *
 * mxArray of size=m*sizeof(double)               */
{
    // FIXME
    return NULL;
}


int mxGetNumberOfElements(const mxArray *ptr)
{
    return ((types::GenericType *) ptr)->getSize();
}


double *mxGetPr(const mxArray *ptr)
{
    return ((types::Double *) ptr)->get();
}



double *mxGetPi(const mxArray *ptr)
{
    return ((types::Double *) ptr)->getImg();
}

int mxGetNumberOfDimensions(const mxArray *ptr)
{
    return ((types::GenericType *) ptr)->getDims();
}

int *mxGetDimensions(const mxArray *ptr)
{
    return ((types::GenericType *) ptr)->getDimsArray();
}

int mxGetM(const mxArray *ptr)
{
    return ((types::GenericType *) ptr)->getRows();
}

void mxSetM(mxArray *ptr, int M)
{
    types::GenericType *pa = (types::GenericType *) ptr;
    int ndim = pa->getDims();
    int *dims = pa->getDimsArray();
    dims[0] = M;
    // TODO: how to handle all data types?
    ((types::Double *) pa)->resize(ndim, *dims);
}


int *mxGetJc(const mxArray *ptr)
{
    // FIXME
    return NULL;
}

int *mxGetIr(const mxArray *ptr)
{
    // FIXME
    return NULL;
}

void mxSetJc(mxArray *array_ptr, int *jc_data)
{
    // FIXME
}

void mxSetIr(mxArray *array_ptr, int *ir_data)
{
    // FIXME
}

void mxSetNzmax(mxArray *array_ptr, int nzmax)
{
    //FIXME
}

int mxGetN(const mxArray *ptr)
{
    return ((types::GenericType *) ptr)->getCols();
}

void mxSetN(mxArray *ptr, int N)
{
    types::GenericType *pa = (types::GenericType *) ptr;
    int *dims = pa->getDimsArray();
    int M = dims[0];
    // TODO: how to handle all data types?
    ((types::Double *) pa)->resize(M, N);
}


bool mxIsString(const mxArray *ptr)
{
    return mxGetClassID(ptr)==mxCHAR_CLASS;
}

bool mxIsChar(const mxArray *ptr)
{
    // FIXME
    return false;
}

bool mxIsNumeric(const mxArray *ptr)
{
    // FIXME
    return false;
}

bool mxIsDouble(const mxArray *ptr)
{
    // FIXME
    return false;
}

bool mxIsSingle(const mxArray *ptr)
{
    return mxGetClassID(ptr)==mxSINGLE_CLASS;
}

bool mxIsEmpty(const mxArray *ptr)
{
    types::GenericType *pa = ((types::GenericType *) ptr);
    int ndim = pa->getDims();
    int *dims = pa->getDimsArray();

    int size = 1;
    for (int i = 0; i < ndim; i++)
    {
        // FIXME : Some overflow may happen here.
        size *= dims[i];
    }
    return size == 0;
}

bool mxIsFull(const mxArray *ptr)
{
    return !mxIsSparse(ptr);
}

bool mxIsSparse(const mxArray *ptr)
{
    // FIXME
    return false;
}

bool mxIsLogical(const mxArray *ptr)
{
    return ((types::InternalType *) ptr)->isBool();
}

bool mxIsLogicalScalar(const mxArray *ptr)
{
    // FIXME
    return false;
}

void mxSetLogical(mxArray *ptr)
{
    // FIXME
}

void mxClearLogical(mxArray *ptr)
{
    // FIXME
}

bool mxIsComplex(const mxArray *ptr)
{
    types::Double * pDbl = (types::Double *) ptr;
    return pDbl->isComplex();
}

double mxGetScalar(const mxArray *ptr)
{
    types::Double *array = (types::Double *) ptr;

    for (int i=0; i < array->getSize(); i++) {
        double value = array->get(i);
        if (value)
        {
            return value;
        }
    }
    return 0;
}

void *mxGetData(const mxArray *ptr)
{
    return mxGetPr(ptr);
}

void *mxGetImagData(const mxArray *ptr)
{
    return mxGetPi(ptr);
}

void mexErrMsgTxt(const char *error_msg)
{
    // FIXME: Try to use ScilabException.
}

void mxAssert(int expr, char *error_message)
{
    if (!expr)
    {
        mexErrMsgTxt(error_message);
    }
}

mxArray *mxCreateDoubleMatrix (int m, int n, mxComplexity complexFlag)
{
    types::Double *ptr = new types::Double(m, n, complexFlag == mxCOMPLEX);
    return (mxArray *) ptr;
}

mxArray *mxCreateDoubleScalar(double value)
{
    mxArray *ptr = mxCreateDoubleMatrix(1, 1, mxREAL);
    ((types::Double *) ptr)->set(0, value);
    return ptr;
}

bool mxIsClass(const mxArray *ptr, const char *name)
{
    // FIXME
    return false;
}

mxArray *mxCreateStructArray(int ndim, const int *dims, int nfields, const char **field_names)
{
    // FIXME
    return NULL;
}

mxArray *mxCreateStructMatrix(int m, int n, int nfields, const char **field_names)
{
    // FIXME
    return NULL;
}

void mxSetFieldByNumber(mxArray *array_ptr, int lindex, int field_number, mxArray *value)
{
    // FIXME
}

void mxSetField(mxArray *array_ptr, int lindex, const char *field_name, mxArray *value)
{
    int field_num;
    field_num = mxGetFieldNumber(array_ptr, field_name);
    mxSetFieldByNumber(array_ptr, lindex, field_num, value);
}

const char *mxGetFieldNameByNumber(const mxArray *array_ptr, int field_number)
{
    // FIXME
    return NULL;
}


mxChar *mxGetChars(mxArray *array_ptr)
{
    mexPrintf("%Not yet implemented\n");
}


int IsReference(mxArray *array_ptr)
{
    // FIXME
    return 0;
}

mxArray *mxCreateNumericArray(int ndim, const int *dims, mxClassID CLASS, mxComplexity complexFlag)
{
    types::GenericType *ptr = NULL;

    if (CLASS == mxINT8_CLASS) {
        ptr = new types::Int8(ndim, (int *) dims);
    }
    if (CLASS == mxUINT8_CLASS) {
        ptr = new types::UInt8(ndim, (int *) dims);
    }
    if (CLASS == mxINT16_CLASS) {
        ptr = new types::Int16(ndim, (int *) dims);
    }
    if (CLASS == mxUINT16_CLASS) {
        ptr = new types::UInt16(ndim, (int *) dims);
    }
    if (CLASS == mxINT32_CLASS) {
        ptr = new types::Int32(ndim, (int *) dims);
    }
    if (CLASS == mxUINT32_CLASS) {
        ptr = new types::UInt32(ndim, (int *) dims);
    }
    if (CLASS == mxINT64_CLASS) {
        ptr = new types::Int64(ndim, (int *) dims);
    }
    if (CLASS == mxUINT64_CLASS) {
        ptr = new types::UInt64(ndim, (int *) dims);
    }
    if (CLASS == mxSINGLE_CLASS) {
        // TODO: this constructor does not exist
        //ptr = new types::Float(ndim, (int *) dims, complexFlag == mxCOMPLEX);
    }
    if (CLASS == mxDOUBLE_CLASS) {
        ptr = new types::Double(ndim, (int *) dims, complexFlag == mxCOMPLEX);
    }
    return (mxArray *) ptr;
}

mxArray *mxCreateNumericMatrix(int m, int n, mxClassID CLASS, mxComplexity complexFlag)
{
    int dims[2] = {m, n};
    return mxCreateNumericArray(2, dims, CLASS, complexFlag);
}


mxArray *mxCreateCharArray(int ndim, const int *dims)
{
    // FIXME
    return NULL;
}

mxArray *mxCreateCellArray(int ndim, const int *dims)
{
    // FIXME
    return NULL;
}

mxArray *mxCreateCellMatrix(int nrows, int ncols)
{
    int two=2;int dims[2];
    dims[0] = nrows; dims[1] = ncols;
    return mxCreateCellArray(two, dims);
}

mxArray *mxGetCell(const mxArray *ptr, int lindex)
{
    // FIXME
    return NULL;
}

int mxGetFieldNumber(const mxArray *ptr, const char *string)
{
    // FIXME
    return 0;
}

mxArray *mxGetField(const mxArray *ptr, int lindex, const char *string)
{
    // FIXME
    return NULL;
}

mxArray *mxGetFieldByNumber(const mxArray *ptr, int lindex, int field_number)
{
    // FIXME
    return NULL;
}

int mxGetNumberOfFields(const mxArray *ptr)
{
    // FIXME
    return 0;
}

/*----------------------------------------------------
 * mxCalloc is supposed to initialize data to 0
 * but what does it means since size can be anythink
 * we initialize to zero for double and int data types
 *----------------------------------------------------*/


void *mxRealloc(void *ptr, size_t nsize)
{
    // FIXME
    return NULL;
}

void *mxCalloc(size_t n, size_t size)
{
    // FIXME
    return NULL;
}

/*  void *mxMalloc(unsigned int nsize)  */
void *mxMalloc(size_t nsize)
{
    // FIXME
    return NULL;
}

void *mxCalloc_m(unsigned int n, unsigned int size)
{
    // FIXME
    return NULL;
}

void *mxMalloc_m(unsigned int n)
{
    // FIXME
    return NULL;
}


void mexMakeMemoryPersistent(void *ptr)
{
    // FIXME
}

/* free : explicit free of a mxCalloc_m allocated space
 *        except if space is protected
 */

void mxFree_m(void *ptr)
{
    // FIXME
}

/* free : explicit free of all mxCalloc_m allocated space
 *        except if space is protected
 */

static void mxFree_m_all()
{
    // FIXME
}


bool mxIsCell(const mxArray *ptr)
{
    return mxGetClassID(ptr)==mxCELL_CLASS;
}

bool mxIsStruct(const mxArray *ptr)
{
    return mxGetClassID(ptr)==mxSTRUCT_CLASS;
}

/***************************************************************
 * Return in str at most strl characters from first element of
 * string Matrix pointed to by ptr ( ptr is assumed to be a String Matrix )
 **************************************************************/

int mxGetString(const mxArray *ptr, char *str, int strl)
{
    // FIXME
    return 0;
}

char *mxArrayToString(const mxArray *array_ptr)
{
    // FIXME
    return NULL;
}

/*-------------------------------------------------
 * mxFreeMatrix :
 * mxFreeMatrix will do something only if it is called
 * in the reverse order of variable allocation
 * Thus after calling mexCallScilab in order to
 * free the stack, one has to call mxFree as in the
 * following example :
 *
 * int nlhs = 3; int nrhs = 2;
 * rhs[0] = mxCreateString(filename);
 * rhs[1] = mxCreateFull(1,1,REAL);
 * mexCallMATLAB(nlhs, lhs, nrhs, rhs, "gm_ifile");
 *  free space in reverse order
 * for (i= nlhs -1 ; i >=0 ; i--) mxFreeMatrix(lhs[i]);
 * for (i= rhs -1 ; i >=0 ; i--) mxFreeMatrix(rhs[i]);
 *-------------------------------------------------*/

void mxFreeMatrix(mxArray *ptr)
{
    // FIXME
}

bool mexIsGlobal(const mxArray *ptr)
{
    // FIXME
    return false;
}


mxArray *mxDuplicateArray(const mxArray *ptr)
{
    types::InternalType *newPtr = ((types::InternalType *) ptr)->clone();
    return (mxArray *) newPtr;
}

mxArray *UnrefStruct(mxArray *ptr)
{
    // FIXME
    return NULL;
}

void mxDestroyArray(mxArray *ptr)
{   /* No need */
    return;
}

void mxFree(void *ptr)
{
    // FIXME
}

int mexAtExit(mxArray *ptr)
{
    // FIXME
    /* XXXXX To be done....*/
    return 0;
}

mxArray *mxCreateSparse(int m, int n, int nzmax, mxComplexity cmplx)
{
    // FIXME
    return NULL;
}


/***************************************************************
 * Create on Scilab Stack a 1x1 string matrix filled with string
 **************************************************************/

mxArray *mxCreateString(const char *string)
{
    types::String *ptr = new types::String(string);
    return (mxArray *) ptr;
}

mxArray *mxCreateLogicalMatrix(int m, int n)
{
    types::Bool *ptr = new types::Bool(m, n);
    return (mxArray *) ptr;
}

mxArray *mxCreateLogicalScalar(mxLogical *value)
{
    mxArray *ptr = mxCreateLogicalMatrix(1, 1);
    ((types::Bool *) ptr)->set(0, (bool) value);
    return ptr;
}

bool mxIsLogicalScalarTrue(mxArray *pa)
{
    return mxIsLogicalScalar(pa) && *mxGetLogicals(pa) == 1;
}


bool mxIsLogicalScalar(mxArray *pa)
{
    return mxIsLogical(pa) && mxGetNumberOfElements(pa) == 1;
}
/*
  Print function which prints (format,args,....)
  in Scilab window
*/

void mexPrintf (const char *fmt,...)
{
    // FIXME : Use YaSpWrite
}


void mexWarnMsgTxt(const char *error_msg)
{
    // FIXME : Use YaSpError ???
    mexPrintf(_("Warning: "));
    mexPrintf(error_msg);
    mexPrintf("\n\n");
}

int mexCallSCILAB(int nlhs, mxArray **plhs, int nrhs, mxArray **prhs, const char *name) {
    // FIXME
    return 0;
}

int mexCallMATLAB(int nlhs, mxArray **plhs, int nrhs, mxArray **prhs, const char *name) {
    // FIXME
    return 0;
}

int mxCalcSingleSubscript(const mxArray *ptr, int nsubs, const int *subs)
{
    // FIXME
    return 0;
}

int C2F(mexcallscilab)(int *nlhs, mxArray **plhs, int *nrhs, mxArray **prhs, char *name, int namelen)
{
    return mexCallSCILAB(*nlhs, plhs, *nrhs, prhs, name);
}

/** generic mex interface **/
const char *mexFunctionName(void)
{
    return the_current_mex_name;
}

int mxGetElementSize(const mxArray *ptr)
{
    // FIXME
    return 0;
}

mxArray *mxCreateCharMatrixFromStrings(int m, const char **str)
{
    // FIXME
    return NULL;
}

int mexEvalString(const char *name)
{
    // FIXME : Better use directly YaSp engine here.
    // double *val ;
    // int rep;
    // mxArray *ppr[3];mxArray *ppl[1];
    // int nlhs;     int nrhs;
    // ppr[0] = mxCreateString(name);
    // ppr[1] = mxCreateString("errcatch");
    // ppr[2] = mxCreateString("m");
    // nrhs=3;nlhs=1;
    // rep = mexCallSCI(nlhs, ppl, nrhs, ppr, "execstr",0);
    // /* check returned value */
    // val = mxGetPr(ppl[0]);
    // mxFreeMatrix(ppl[0]);
    // mxFreeMatrix(ppr[2]);
    // mxFreeMatrix(ppr[1]);
    // mxFreeMatrix(ppr[0]);
    // if ( rep == 1 || (int) (*val) != 0 )
    // {
    //     errjump();
    // }
    return 0;
}

mxArray *mexGetArray(char *name, char *workspace)
{
    // FIXME : use Context::get
    return NULL;
}

const mxArray *mexGetVariablePtr(const char *workspace, const char *var_name)
{
   // FIXME : use Context::get
    return NULL;
}

mxArray *mexGetVariable(const char *workspace, const char *name)
{
    // FIXME : use Context::get
    return NULL;
}

int mexPutArray(mxArray *array_ptr, char *workspace)
{
    /* TO BE DONE obsolete ?  */
    mexPrintf( _("Function mexPutArray is obsolete, use mexPutVariable!\n"));
    return 1;
}

int mexPutVariable(const char *workspace, char *var_name, mxArray *array_ptr)
{
    // FIXME : use Context::put
    return NULL;
}

void mxSetName(mxArray *array_ptr, const char *name)
{
    // FIXME
    mexErrMsgTxt(_("Routine mxSetName not implemented !\n"));
    exit(1);  /* TO BE DONE */
}

void mxSetData(mxArray *array_ptr, void *data_ptr)
{
    // FIXME
    mexErrMsgTxt(_("Routine mxSetData not implemented.\n"));
    exit(1);  /* TO BE DONE */
}

void mxSetPr(mxArray *ptr, double *pr)
{
    ((types::Double *) ptr)->set(pr);
}

void mxSetPi(mxArray *ptr, double *pi)
{
    ((types::Double *) ptr)->setImg(pi);
}

const char *mxGetName(const mxArray *array_ptr)
{
    // FIXME
    mexPrintf(_("Routine mxGetName not implemented.\n"));
    exit(1);
	return 0;
}

int mxSetDimensions(mxArray *array_ptr, const int *dims, int ndim)
{
    // FIXME
    mexPrintf(_("Routine mxSetDimensions not implemented.\n"));
    exit(1);  /* TO BE DONE */
    return 0;
}

const char *mxGetClassName(const mxArray *ptr)
{
    // FIXME
    return "unknown";
}

void mxSetCell(mxArray *array_ptr, int lindex, mxArray *value)
{
    // FIXME : ???
    mxSetFieldByNumber(array_ptr, lindex, 0 , value);
    return;
}

int mxGetNzmax(const mxArray *ptr)
{
    // FIXME
    return 0;
}

mxLogical *mxGetLogicals(const mxArray *array_ptr)
{
    // FIXME
    return NULL;
}

void mexInfo(char *str) {
    // FIXME : Use YaSpWrite
}

int mexCheck(char *str,int nbvars) {
    // FIXME : Where does Nbvars come from ??
    // if ( nbvars != -1 && Nbvars != nbvars)
    //     fprintf(stderr,"%s %d %d\n",str,Nbvars,nbvars);
    // return Nbvars ;
    return 0;
}


/****************************************************
 * C functions for Fortran  mexfunctions
 ****************************************************/

double * C2F(mxgetpr)(mxArray *ptr)
{
    // FIXME : Wrap this one to the C one
    return NULL;
}


double * C2F(mxgetpi)(mxArray *ptr)
{
    // FIXME : Wrap this one to the C one
    return NULL;
}

int  C2F(mxgetm)(mxArray *ptr)
{
    // FIXME : Wrap this one to the C one
    return 0;
}

int  C2F(mxgetn)(mxArray *ptr)
{
    // FIXME : Wrap this one to the C one
    return 0;
}

int  C2F(mxisstring)(mxArray *ptr)
{
    // FIXME : Wrap this one to the C one
    return 0;
}

int  C2F(mxisnumeric)(mxArray *ptr)
{
    // FIXME : Wrap this one to the C one
    return 0;
}

int  C2F(mxisfull)(mxArray *ptr)
{
    // FIXME : Wrap this one to the C one
    return 0;
}

int  C2F(mxissparse)(mxArray *ptr)
{
    // FIXME : Wrap this one to the C one
    return 0;
}


int  C2F(mxiscomplex)(mxArray *ptr)
{
    // FIXME : Wrap this one to the C one
    return 0;
}

double  C2F(mxgetscalar)(mxArray *ptr)
{
    // FIXME : Wrap this one to the C one
}

void  C2F(mexprintf)(char *error_msg, int len)
{
    // FIXME : Wrap this one to the C one
}

void C2F(mexerrmsgtxt)(char *error_msg, int len)
{
    // FIXME : Wrap this one to the C one
}

mxArray *C2F(mxcreatefull)(int *m, int *n, int *it)
{
    // FIXME : Wrap this one to the C one
    return NULL;
}

mxArray *C2F(mxcreatedoublematrix)(int *m, int *n, int *it)
{
    return (mxArray *) mxCreateDoubleMatrix(*m, *n, (mxComplexity)*it);
}


unsigned long int C2F(mxcalloc)(unsigned int *n, unsigned int *size)
{
    // FIXME : Wrap this one to the C one
    return 0;
}

int  C2F(mxgetstring)(mxArray *ptr, char *str, int *strl)
{
    // FIXME : Wrap this one to the C one
    return 0;
}

void C2F(mxfreematrix)(mxArray *ptr)
{
    // FIXME : Wrap this one to the C one
}


mxArray * C2F(mxcreatestring)(char *string, long int l)
{
    // FIXME : Wrap this one to the C one
    return NULL;
}


int C2F(mxcopyreal8toptr)(double *y, mxArray *ptr, int *n)
{
    // FIXME : Wrap this one to the C one
    return 0;
}

int C2F(mxcopycomplex16toptr)(double *y, mxArray *ptr, mxArray *pti, int *n)
{
    // FIXME : Wrap this one to the C one
    return 0;
}

int C2F(mxcopyptrtoreal8)(mxArray *ptr, double *y, int *n)
{
    // FIXME : Wrap this one to the C one
    return 0;
}

int C2F(mxcopyptrtocomplex16)(mxArray *ptr, mxArray *pti, double *y, int *n)
{
    // FIXME : Wrap this one to the C one
    return 0;
}

/* mxCreateLogicalArray
 * mxIsLogicalScalarTrue */

/* *mxRealloc(void *ptr, size_t size);
   mxArray *mxCreateStringFromNChars(const char *str, int n);
   int mxSetClassName(mxArray *pa, const char *classname);
   int mxAddField(mxArray *pa, const char *fieldname);
   void mxRemoveField(mxArray *pa, int field);
   void mxSetCopyInCell(mxArray *pa, int i, mxArray *value);  */
