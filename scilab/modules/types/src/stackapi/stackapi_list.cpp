/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
* Please note that piece of code will be rewrited for the Scilab 6 family
* However, the API (profile of the functions in the header files) will be
* still available and supported in Scilab 6.
*/

#include <vector>
#include <map>
#include <string>

#include "stdio.h"
#include "MALLOC.h"
#include "stackapi_scilab.h"
#include "stackapi_internal_common.h"
#include "stackapi_internal_double.h"
#include "stackapi_internal_string.h"
#include "stackapi_internal_boolean.h"
#include "stackapi_internal_poly.h"
#include "stackapi_internal_int.h"
#include "stackapi_internal_sparse.h"
#include "stackapi_internal_boolean_sparse.h"
#include "stackapi_internal_pointer.h"
#include "localization.h"

//internal functions
static SciErr stackapi_createCommonList(void* _pvCtx, int _iVar, int _iListType, int _iNbItem, int** _piAddress);
static SciErr stackapi_createCommonListInList(void* _pvCtx, int _iVar, const char* _pstName, int* _piParent, int _iItemPos, int _iListType, int _iNbItem, int** _piAddress);
static SciErr stackapi_getCommonListInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iListType, int** _piAddress);
static SciErr stackapi_fillCommonList(void* _pvCtx, int* _piAddress, int _iListType, int _iNbItem);
static void stackapi_closeList(int _iVar, int *_piEnd);
static void stackapi_updateListOffset(void* _pvCtx, int _iVar, int *_piCurrentNode, int _iItemPos, int *_piEnd);
static void stackapi_updateCommunListOffset(void* _pvCtx, int _iVar, const char* _pstName, int *_piCurrentNode, int _iItemPos, int *_piEnd);

static SciErr stackapi_allocCommonMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double **_pdblReal, double **_pdblImg);
static SciErr stackapi_getCommonMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg);
static SciErr stackapi_createCommonMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg);
static SciErr stackapi_fillCommonMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg);
static SciErr stackapi_allocCommonItemInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piChildAddr);
static SciErr stackapi_fillMatrixOfBoolInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piBool);
static SciErr stackapi_getCommonMatrixOfPolyInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);
static SciErr stackapi_createCommonMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iComplex, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg);
static SciErr stackapi_fillCommonMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iComplex, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg, int* _piTotalLen);
static SciErr stackapi_fillCommonMatrixOfStringInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const char* const* _pstStrings, int* _piTotalLen);
static SciErr stackapi_createCommonSparseMatrixInList(void* _pvCtx, int _iVar, const char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal, const double* _pdblImg);

struct ListInfo
{
    ListInfo(int* _piAddr, int _iLast) : m_piAddr(_piAddr), m_iLast(_iLast) {};
    int* m_piAddr;
    int m_iLast;
};

typedef std::vector<ListInfo*> VectListInfo;
typedef std::map<int, VectListInfo*> StackList;
static StackList stackListPosition;


// Push a list address on the stackListPosition
static void stackapi_pushListAddress(int _iRhsPos, int* _piAddr)
{
    if (stackListPosition.find(_iRhsPos) == stackListPosition.end())
    {
        VectListInfo* pVect = new VectListInfo();
        pVect->push_back(new ListInfo(_piAddr, 0));
        stackListPosition[_iRhsPos] = pVect;
    }
    else
    {
        stackListPosition[_iRhsPos]->push_back(new ListInfo(_piAddr, 0));
    }
}

// Pop a list address from the stackListPosition
static void stackapi_popListAddress(int _iRhsPos)
{
    StackList::iterator it = stackListPosition.find(_iRhsPos);
    if (it != stackListPosition.end())
    {
        delete it->second->back();
        stackListPosition[_iRhsPos]->pop_back();
        if(it->second->size() > 0 && it->second->back()->m_iLast == 1)
        {//close cascade
            stackapi_popListAddress(_iRhsPos);
        }

        if(stackListPosition.find(_iRhsPos) != stackListPosition.end() && stackListPosition[_iRhsPos]->empty())
        {
            stackListPosition.erase(stackListPosition.find(_iRhsPos));
            //TODO : check to close list
        }
    }
}

/*get last store address*/
int* stackapi_getLastListAddress(int _iRhsPos, int _iItemPos)
{
    StackList::iterator it = stackListPosition.find(_iRhsPos);
    if (it == stackListPosition.end() || it->second->empty())
    {
        return NULL;
    }

    if(_iItemPos == it->second->back()->m_piAddr[1])
    {
        it->second->back()->m_iLast = 1;
    }

    return it->second->back()->m_piAddr;
}

//get address list
static void stackapi_getListAdressses(int _iRhsPos, int** _piAddresses)
{
    StackList::iterator it = stackListPosition.find(_iRhsPos);
    if (it == stackListPosition.end() || it->second->empty() || _piAddresses == NULL)
    {
        return;
    }

    VectListInfo::iterator vit;
    int i = 0;
    for (vit = it->second->begin() ; vit != it->second->end() ; vit++, i++)
    {
        _piAddresses[i] = (*vit)->m_piAddr;
    }
}

//get Depth of list
static int stackapi_getDepthList(int _iRhsPos)
{
    StackList::iterator it = stackListPosition.find(_iRhsPos);
    if (it == stackListPosition.end() || it->second->empty())
    {
        return 0;
    }
    return (int)it->second->size();
}

char* stackapi_getListTypeName(int _iType)
{
    switch (_iType)
    {
        case sci_list :
            return "list";
            break;
        case sci_tlist :
            return "tlist";
            break;
        case sci_mlist :
            return "mlist";
            break;
        default:
            break;
    }
    return "";
}

SciErr stackapi_getListItemNumber(void* _pvCtx, int* _piAddress, int* _piNbItem)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int iType = 0;

    sciErr = stackapi_getVarType(_pvCtx, _piAddress, &iType);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_LIST_ITEM_NUMBER, _("%s: Unable to get item number of list"), "stackapi_getListItemNumber");
        return sciErr;
    }
    switch (iType)
    {
        case sci_list :
        case sci_mlist :
        case sci_tlist :
            *_piNbItem = _piAddress[1];
            break;
        default :
            stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_LIST_TYPE, _("%s: Invalid argument type, %s excepted"), "stackapi_getListItemNumber", _("list"));
            return sciErr;
    }
    return sciErr;
}

SciErr stackapi_getListItemAddress(void* _pvCtx, int* _piAddress, int _iItemNum, int** _piItemAddress)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int iItem      = 0;
    int* piOffset    = NULL;
    int* piItemAddress = NULL;

    //get item count
    sciErr = stackapi_getListItemNumber(_pvCtx, _piAddress, &iItem);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ITEM_ADDRESS, _("%s: Unable to get address of item #%d in argument #%d"), "stackapi_getListItemAddress", _iItemNum + 1, stackapi_getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }

    if (_iItemNum > iItem)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_ITEM_ADDRESS, _("%s: Unable to get address of item #%d in argument #%d"), "stackapi_getListItemAddress", _iItemNum + 1, stackapi_getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }

    //get offset of item array
    piOffset    =  _piAddress + 2;
    if (piOffset[_iItemNum] == piOffset[_iItemNum - 1])
    {
        *_piItemAddress = NULL;
    }
    else
    {
        piItemAddress = piOffset + iItem  + 1 + !(iItem % 2);
        *_piItemAddress = piItemAddress + (piOffset[_iItemNum - 1] - 1) * (sizeof(double) / sizeof(int));
    }
    return sciErr;
}

SciErr stackapi_createList(void* _pvCtx, int _iVar, int _iNbItem, int** _piAddress)
{
    return stackapi_createCommonList(_pvCtx, _iVar, sci_list, _iNbItem, _piAddress);
}

SciErr stackapi_createMList(void* _pvCtx, int _iVar, int _iNbItem, int** _piAddress)
{
    return stackapi_createCommonList(_pvCtx, _iVar, sci_mlist, _iNbItem, _piAddress);
}

SciErr stackapi_createTList(void* _pvCtx, int _iVar, int _iNbItem, int** _piAddress)
{
    return stackapi_createCommonList(_pvCtx, _iVar, sci_tlist, _iNbItem, _piAddress);
}

static SciErr stackapi_createCommonList(void* _pvCtx, int _iVar, int _iListType, int _iNbItem, int** _piAddress)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int *piAddr   = NULL;
    int iNewPos   = Top - Rhs + _iVar;
    int iAddr    = *Lstk(iNewPos);

    stackapi_getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

    sciErr = stackapi_fillCommonList(_pvCtx, piAddr, _iListType, _iNbItem);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_LIST, _("%s: Unable to create variable in Scilab memory"), "createList");
        return sciErr;
    }

    *_piAddress = piAddr;
    stackapi_updateInterSCI(_iVar, '$', iAddr, sadr(iadr(iAddr) + 2 + _iNbItem + 1 + !(_iNbItem % 2)));
    stackapi_closeList(iNewPos, piAddr + 2 + _iNbItem + 1 + !(_iNbItem % 2));

    if(_iNbItem != 0)
    {
        stackapi_pushListAddress(_iVar, piAddr);
    }

    return sciErr;
}

SciErr stackapi_fillCommonList(void* _pvCtx, int* _piAddress, int _iListType, int _iNbItem)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int* piOffset = NULL;

    _piAddress[0] = _iListType;
    _piAddress[1] = _iNbItem;

    piOffset  = _piAddress + 2;
    piOffset[0] = 1; //always

    for (int i = 0 ; i < _iNbItem; i++)
    {
        //initialize item offset
        piOffset[i + 1] = -1;
    }
    return sciErr;
}

SciErr stackapi_getListInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piAddress)
{
    return stackapi_getCommonListInList(_pvCtx, _piParent, _iItemPos, sci_list, _piAddress);
}

SciErr stackapi_getTListInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piAddress)
{
    return stackapi_getCommonListInList(_pvCtx, _piParent, _iItemPos, sci_tlist, _piAddress);
}

SciErr stackapi_getMListInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piAddress)
{
    return stackapi_getCommonListInList(_pvCtx, _piParent, _iItemPos, sci_mlist, _piAddress);
}

SciErr stackapi_getCommonListInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iListType, int** _piAddress)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;

    sciErr = stackapi_getListItemAddress(_pvCtx, _piParent, _iItemPos, _piAddress);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_LIST_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getListInList", _iItemPos + 1, stackapi_getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    if ((*_piAddress)[0] != _iListType)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_INVALID_LIST_TYPE, _("%s: Invalid argument type, %s excepted"), "getListInList", stackapi_getListTypeName(_iListType));
        return sciErr;
    }
    return sciErr;
}

SciErr stackapi_createListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
    return stackapi_createCommonListInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, sci_list, _iNbItem, _piAddress);
}

SciErr stackapi_createTListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
    return stackapi_createCommonListInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, sci_tlist, _iNbItem, _piAddress);
}

SciErr stackapi_createMListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
    return stackapi_createCommonListInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, sci_mlist, _iNbItem, _piAddress);
}

static SciErr stackapi_createCommonListInList(void* _pvCtx, int _iVar, const char* _pstName, int* /*_piParent*/, int _iItemPos, int _iListType, int _iNbItem, int** _piAddress)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int iNewPos    = Top - Rhs + _iVar;
    int iNbItem    = 0;
    int* piChildAddr = NULL;
    int* piParent   = NULL;

    piParent = stackapi_getLastListAddress(_iVar, _iItemPos);

    //Does item can be added in the list
    sciErr = stackapi_getListItemNumber(_pvCtx, piParent, &iNbItem);

    if (iNbItem < _iItemPos)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), "createListInList", _iItemPos + 1);
        return sciErr;
    }

    sciErr = stackapi_allocCommonItemInList(_pvCtx, piParent, _iItemPos, &piChildAddr);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_LIST_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createListInList", _iItemPos + 1);
        return sciErr;
    }

    sciErr = stackapi_fillCommonList(_pvCtx, piChildAddr, _iListType, _iNbItem);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_LIST_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createListInList", _iItemPos + 1);
        return sciErr;
    }

    *_piAddress = piChildAddr;
    stackapi_closeList(iNewPos, piChildAddr + 2 + _iNbItem + 1 + !(_iNbItem % 2));

    if(_iNbItem == 0)
    {//for empty list
        int *piOffset   = piParent + 2;
        int* piEnd      = piChildAddr + 4;

        piOffset[_iItemPos] = piOffset[_iItemPos - 1] + 2;
        stackapi_updateListOffset(_pvCtx, _iVar, piParent, _iItemPos, piEnd);
        if(_iItemPos == piParent[1])
        {
            stackapi_popListAddress(_iVar);
        }
    }
    else
    {//not empty list
        //add new child address in stacklist
        stackapi_pushListAddress(_iVar, *_piAddress);
    }

    return sciErr;
}

SciErr stackapi_allocCommonItemInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piChildAddr)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int* piOffset = NULL;

    //Does previous items was already inserted
    piOffset  = _piParent + 2;
    if (piOffset[_iItemPos - 1] == -1)
    {
        //Previous items wasn't inserted
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_NON_ORDERED_INSERTION, _("%s: Items must be inserted in order"), "allocItemInList");
        return sciErr;
    }

    sciErr = stackapi_getListItemAddress(_pvCtx, _piParent, _iItemPos, _piChildAddr);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_ALLOC_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "allocItemInList", _iItemPos + 1);
        return sciErr;
    }

    return sciErr;
}

/*********************
* Double functions *
*********************/

SciErr stackapi_getMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal)
{
    return stackapi_getCommonMatrixOfDoubleInList(_pvCtx, _piParent, _iItemPos, 0, _piRows, _piCols, _pdblReal, NULL);
}

SciErr stackapi_getComplexMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg)
{
    return stackapi_getCommonMatrixOfDoubleInList(_pvCtx, _piParent, _iItemPos, 1, _piRows, _piCols, _pdblReal, _pdblImg);
}

static SciErr stackapi_getCommonMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int* piAddr  = NULL;

    sciErr = stackapi_getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_DOUBLE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "getComplexMatrixOfDoubleInList" : "stackapi_getMatrixOfDoubleInList", _iItemPos + 1, stackapi_getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    sciErr = stackapi_getCommonMatrixOfDouble(_pvCtx, piAddr, _iComplex, _piRows, _piCols, _pdblReal, _pdblImg);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_DOUBLE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "getComplexMatrixOfDoubleInList" : "stackapi_getMatrixOfDoubleInList", _iItemPos + 1, stackapi_getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }
    return sciErr;
}

SciErr stackapi_allocMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double** _pdblReal)
{
    return stackapi_allocCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, 0, _iRows, _iCols, _pdblReal, NULL);
}

SciErr stackapi_allocComplexMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
    return stackapi_allocCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, 1, _iRows, _iCols, _pdblReal, _pdblImg);
}

static SciErr stackapi_allocCommonMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* /*_piParent*/, int _iItemPos, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
    SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int iNewPos     = Top - Rhs + _iVar;
    int* piEnd      = NULL;
    int* piParent   = stackapi_getLastListAddress(_iVar, _iItemPos);

    sciErr = stackapi_fillCommonMatrixOfDoubleInList(_pvCtx, _iVar, piParent, _iItemPos, _iComplex, _iRows, _iCols, _pdblReal, _pdblImg);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_ALLOC_DOUBLE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "allocComplexMatrixOfDoubleInList" : "allocMatrixOfDoubleInList", _iItemPos + 1, stackapi_getRhsFromAddress(_pvCtx, piParent));
        return sciErr;
    }

    piEnd = (int*) (*_pdblReal + _iRows * _iCols * (_iComplex + 1));
    stackapi_closeList(iNewPos, piEnd);

    if(_iItemPos == piParent[1])
    {
        stackapi_updateListOffset(_pvCtx, _iVar, piParent, _iItemPos, piEnd);
        stackapi_popListAddress(_iVar);
    }

    return sciErr;
}

static SciErr stackapi_fillCommonMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int iNbItem    = 0;
    int* piOffset   = NULL;
    int* piChildAddr = NULL;

    //Does item can be added in the list
    sciErr = stackapi_getListItemNumber(_pvCtx, _piParent, &iNbItem);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_ALLOC_DOUBLE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "allocComplexMatrixOfDoubleInList" : "allocMatrixOfDoubleInList", _iItemPos + 1, stackapi_getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    if (iNbItem < _iItemPos)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfDoubleInList" : "createMatrixOfDoubleInList", _iItemPos + 1);
        return sciErr;
    }


    sciErr = stackapi_allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_ALLOC_DOUBLE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "allocComplexMatrixOfDoubleInList" : "allocMatrixOfDoubleInList", _iItemPos + 1, stackapi_getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    sciErr = stackapi_fillCommonMatrixOfDouble(_pvCtx, piChildAddr, _iComplex, _iRows, _iCols, _pdblReal, _pdblImg);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_ALLOC_DOUBLE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "allocComplexMatrixOfDoubleInList" : "allocMatrixOfDoubleInList", _iItemPos + 1, stackapi_getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    piOffset      = _piParent + 2;
    piOffset[_iItemPos] = piOffset[_iItemPos - 1] + _iRows * _iCols * (_iComplex + 1) + 2;

    return sciErr;
}

SciErr stackapi_createMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const double* _pdblReal)
{
    return stackapi_createCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, 0, _iRows, _iCols, _pdblReal, NULL);
}

SciErr stackapi_createComplexMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg)
{
    return stackapi_createCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, 1, _iRows, _iCols, _pdblReal, _pdblImg);
}

SciErr stackapi_createComplexZMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const doublecomplex* _pdblData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    double *pdblReal = NULL;
    double *pdblImg  = NULL;

    sciErr = stackapi_allocCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, 1, _iRows, _iCols, &pdblReal, &pdblImg);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_DOUBLE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createComplexZMatrixOfDoubleInList", _iItemPos + 1);
        return sciErr;
    }

    //vGetPointerFromDoubleComplex(_pdblData, _iRows * _iCols, pdblReal, pdblImg);

    return sciErr;
}

SciErr stackapi_createCommonMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* /*_piParent*/, int _iItemPos, int _iComplex, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    double *pdblReal = NULL;
    double *pdblImg  = NULL;

    sciErr = stackapi_allocCommonMatrixOfDoubleInList(_pvCtx, _iVar, NULL/*_piParent*/, _iItemPos, _iComplex, _iRows, _iCols, &pdblReal, &pdblImg);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_DOUBLE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfDoubleInList" : "createMatrixOfDoubleInList", _iItemPos + 1);
        return sciErr;
    }

    if (_pdblReal != NULL)
    {
        memcpy(pdblReal, _pdblReal, _iRows * _iCols * sizeof(double));
    }

    if (_iComplex && _pdblImg != NULL)
    {
        memcpy(pdblImg, _pdblImg, _iRows * _iCols * sizeof(double));
    }
    return sciErr;
}

/*********************
* Strings functions *
*********************/

SciErr stackapi_getMatrixOfStringInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int* piAddr  = NULL;

    sciErr = stackapi_getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_STRING_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "stackapi_getMatrixOfStringInList", _iItemPos + 1, stackapi_getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    sciErr = stackapi_getMatrixOfString(_pvCtx, piAddr, _piRows, _piCols, _piLength, _pstStrings);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_STRING_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "stackapi_getMatrixOfStringInList", _iItemPos + 1, stackapi_getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    return sciErr;
}


SciErr stackapi_createMatrixOfStringInList(void* _pvCtx, int _iVar, int* /*_piParent*/, int _iItemPos, int _iRows, int _iCols, const char* const* _pstStrings)
{
    SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int iNbItem     = 0;
    int iTotalLen   = 0;
    int iNewPos     = Top - Rhs + _iVar;

    int* piItemAddr = NULL;
    int* piEnd      = NULL;
    int* piParent   = stackapi_getLastListAddress(_iVar, _iItemPos);

    sciErr = stackapi_getListItemNumber(_pvCtx, piParent, &iNbItem);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_STRING_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfStringInList", _iItemPos + 1);
        return sciErr;
    }

    if (iNbItem < _iItemPos)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfStringInList", _iItemPos + 1);
        return sciErr;
    }

    sciErr = stackapi_getListItemAddress(_pvCtx, piParent, _iItemPos, &piItemAddr);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_STRING_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfStringInList", _iItemPos + 1);
        return sciErr;
    }

    sciErr = stackapi_fillCommonMatrixOfStringInList(_pvCtx, _iVar, piParent, _iItemPos, _iRows, _iCols, _pstStrings, &iTotalLen);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_STRING_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfStringInList", _iItemPos + 1);
        return sciErr;
    }

    piEnd = piItemAddr + iTotalLen + 5 + _iRows * _iCols + !((iTotalLen + _iRows * _iCols) % 2);
    stackapi_closeList(iNewPos, piEnd);

    if (_iItemPos == iNbItem)
    {
        stackapi_updateListOffset(_pvCtx, _iVar, piParent, _iItemPos, piEnd);
        stackapi_popListAddress(_iVar);
    }

    return sciErr;
}

SciErr stackapi_fillCommonMatrixOfStringInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const char* const* _pstStrings, int* _piTotalLen)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int iNbItem   = 0;

    int* piAddr   = NULL;
    int* piOffset  = NULL;

    //Does item can be added in the list
    stackapi_getListItemNumber(_pvCtx, _piParent, &iNbItem);
    if (iNbItem < _iItemPos)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfStringInList", _iItemPos + 1);
        return sciErr;
    }

    sciErr = stackapi_allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piAddr);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_FILL_STRING_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "fillMatrixOfStringInList", _iItemPos + 1);
        return sciErr;
    }

    sciErr = stackapi_fillMatrixOfString(_pvCtx, piAddr, _iRows, _iCols, _pstStrings, _piTotalLen);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_FILL_STRING_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "fillMatrixOfStringInList", _iItemPos + 1);
        return sciErr;
    }

    piOffset      = _piParent + 2;
    piOffset[_iItemPos] = piOffset[_iItemPos - 1] + (*_piTotalLen + 5 + _iRows * _iCols + !((*_piTotalLen + _iRows * _iCols) % 2)) / 2;

    return sciErr;
}

/*********************
* boolean functions *
*********************/
SciErr stackapi_getMatrixOfBooleanInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int** _piBool)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int* piAddr  = NULL;

    sciErr = stackapi_getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_BOOLEAN_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "stackapi_getMatrixOfBooleanInList", _iItemPos + 1, stackapi_getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    sciErr = stackapi_getMatrixOfBoolean(_pvCtx, piAddr, _piRows, _piCols, _piBool);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_BOOLEAN_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "stackapi_getMatrixOfBooleanInList", _iItemPos + 1, stackapi_getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    return sciErr;
}

SciErr stackapi_createMatrixOfBooleanInList(void* _pvCtx, int _iVar, int* /*_piParent*/, int _iItemPos, int _iRows, int _iCols, const int* _piBool)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int *piBool   = NULL;

    sciErr = stackapi_allocMatrixOfBooleanInList(_pvCtx, _iVar, NULL/*_piParent*/, _iItemPos, _iRows, _iCols, &piBool);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_BOOLEAN_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfBooleanInList", _iItemPos + 1);
        return sciErr;
    }

    if (_piBool != NULL)
    {
        memcpy(piBool, _piBool, _iRows * _iCols * sizeof(int));
    }
    return sciErr;
}

SciErr stackapi_allocMatrixOfBooleanInList(void* _pvCtx, int _iVar, int* /*_piParent*/, int _iItemPos, int _iRows, int _iCols, int** _piBool)
{
    SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int iNewPos     = Top - Rhs + _iVar;
    int* piEnd      = NULL;
    int* piParent   = stackapi_getLastListAddress(_iVar, _iItemPos);

    sciErr = stackapi_fillMatrixOfBoolInList(_pvCtx, _iVar, piParent, _iItemPos, _iRows, _iCols, _piBool);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_ALLOC_BOOLEAN_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "allocMatrixOfBooleanInList", _iItemPos + 1);
        return sciErr;
    }

    piEnd = *_piBool + _iRows * _iCols + !((_iRows * _iCols) % 2);
    stackapi_closeList(iNewPos, piEnd);

    if(_iItemPos == piParent[1])
    {
        stackapi_updateListOffset(_pvCtx, _iVar, piParent, _iItemPos, piEnd);
        stackapi_popListAddress(_iVar);
    }
    return sciErr;
}

static SciErr stackapi_fillMatrixOfBoolInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piBool)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int iNbItem    = 0;
    int* piOffset   = NULL;
    int* piChildAddr = NULL;

    //Does item can be added in the list
    sciErr = stackapi_getListItemNumber(_pvCtx, _piParent, &iNbItem);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_FILL_BOOLEAN_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfBoolInList", _iItemPos + 1);
        return sciErr;
    }

    if (iNbItem < _iItemPos)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfBooleanInList", _iItemPos + 1);
        return sciErr;
    }


    sciErr = stackapi_allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_FILL_BOOLEAN_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfBoolInList", _iItemPos + 1);
        return sciErr;
    }

    sciErr = stackapi_fillMatrixOfBoolean(_pvCtx, piChildAddr, _iRows, _iCols, _piBool);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_FILL_BOOLEAN_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfBoolInList", _iItemPos + 1);
        return sciErr;
    }

    piOffset      = _piParent + 2;
    piOffset[_iItemPos] = piOffset[_iItemPos - 1] + ((3 + _iRows * _iCols + !((_iRows * _iCols) % 2)) / 2);

    return sciErr;
}

/*************************
* polynomials functions *
*************************/
SciErr stackapi_getMatrixOfPolyInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal)
{
    return stackapi_getCommonMatrixOfPolyInList(_pvCtx, _piParent, _iItemPos, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}

SciErr stackapi_getComplexMatrixOfPolyInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
    return stackapi_getCommonMatrixOfPolyInList(_pvCtx, _piParent, _iItemPos, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}

SciErr stackapi_getCommonMatrixOfPolyInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int* piAddr  = NULL;

    sciErr = stackapi_getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_POLY_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "getComplexMatrixOfPolyInList" : "stackapi_getMatrixOfPolyInList", _iItemPos + 1, stackapi_getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    sciErr = stackapi_getCommonMatrixOfPoly(_pvCtx, piAddr, _iComplex, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_POLY_IN_LIST, _("STACKAPI_ERROR_GET_POLY_IN_LIST"));
        return sciErr;
    }

    return sciErr;
}

SciErr stackapi_createMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal)
{
    return stackapi_createCommonMatrixOfPolyInList(_pvCtx, _iVar, _piParent, _iItemPos, _pstVarName, 0, _iRows, _iCols, _piNbCoef, _pdblReal, NULL);
}

SciErr stackapi_createComplexMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg)
{
    return stackapi_createCommonMatrixOfPolyInList(_pvCtx, _iVar, _piParent, _iItemPos, _pstVarName, 1, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg);
}

SciErr stackapi_createCommonMatrixOfPolyInList(void* _pvCtx, int _iVar, int* /*_piParent*/, int _iItemPos, char* _pstVarName, int _iComplex, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg)
{
    SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int* piItemAddr = NULL;
    int *piEnd      = NULL;
    int iItemLen    = 0;
    int iTotalLen   = 0;
    int* piParent   = stackapi_getLastListAddress(_iVar, _iItemPos);

    sciErr = stackapi_getListItemAddress(_pvCtx, piParent, _iItemPos, &piItemAddr);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_POLY_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfPolyInList" : "createMatrixOfPolyInList", _iItemPos + 1);
        return sciErr;
    }

    sciErr = stackapi_fillCommonMatrixOfPolyInList(_pvCtx, _iVar, piParent, _iItemPos, _pstVarName, _iComplex, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg, &iTotalLen);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_POLY_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfPolyInList" : "createMatrixOfPolyInList", _iItemPos + 1);
        return sciErr;
    }

    iItemLen      = 9 + _iRows * _iCols + (9 + _iRows * _iCols) % 2;
    iItemLen      += iTotalLen;
    piEnd        = piItemAddr + iItemLen;
    if(_iItemPos == piParent[1])
    {
        stackapi_updateListOffset(_pvCtx, _iVar, piParent, _iItemPos, piEnd);
        stackapi_popListAddress(_iVar);
    }

    stackapi_closeList(_iVar, piEnd);

    return sciErr;
}

static SciErr stackapi_fillCommonMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iComplex, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg, int* _piTotalLen)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int iNbItem    = 0;
    int iTotalLen   = 0;
    int* piOffset   = NULL;
    int* piChildAddr = NULL;

    int iItemLen   = 0;

    //Does item can be added in the list
    sciErr = stackapi_getListItemNumber(_pvCtx, _piParent, &iNbItem);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_FILL_POLY_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfPolyInList" : "createMatrixOfPolyInList", _iItemPos + 1);
        return sciErr;
    }

    if (iNbItem < _iItemPos)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfPolyInList" : "createMatrixOfPolyInList", _iItemPos + 1);
        return sciErr;
    }

    sciErr = stackapi_allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_FILL_POLY_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfPolyInList" : "createMatrixOfPolyInList", _iItemPos + 1);
        return sciErr;
    }

    sciErr = stackapi_fillCommonMatrixOfPoly(_pvCtx, piChildAddr, _pstVarName, _iComplex, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg, &iTotalLen);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_FILL_POLY_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfPolyInList" : "createMatrixOfPolyInList", _iItemPos + 1);
        return sciErr;
    }

    piOffset      = _piParent + 2;

    iItemLen      = 9 + _iRows * _iCols + (9 + _iRows * _iCols) % 2;
    iItemLen      += iTotalLen;
    piOffset[_iItemPos] = piOffset[_iItemPos - 1] + ((iItemLen + 1) / 2);

    *_piTotalLen = iTotalLen;
    return sciErr;
}



/**********************
* integers functions *
**********************/
static SciErr stackapi_fillCommonMatrixOfIntegerInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iPrecision, int _iRows, int _iCols, void** _pvData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int iNbItem    = 0;
    int* piOffset   = NULL;
    int* piChildAddr = NULL;

    //Does item can be added in the list
    sciErr = stackapi_getListItemNumber(_pvCtx, _piParent, &iNbItem);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_FILL_INT_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfIntegerInList", _iItemPos + 1);
        return sciErr;
    }

    if (iNbItem < _iItemPos)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfIntegerInList", _iItemPos + 1);
        return sciErr;
    }


    sciErr = stackapi_allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_FILL_INT_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfIntegerInList", _iItemPos + 1);
        return sciErr;
    }

    sciErr = stackapi_fillCommonMatrixOfInteger(_pvCtx, piChildAddr, _iPrecision, _iRows, _iCols, _pvData);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_FILL_INT_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfIntegerInList", _iItemPos + 1);
        return sciErr;
    }

    piOffset      = _piParent + 2;

    //integer : size in double
    //1st case, 5 * 1 int8  -> (10,5) (1,1) (1,2,3,4,5,x,x,x)           -> 3 : 2 + 5/8 + !!(5%8) -> 2 + 0 + 1 -> 3
    //2nd case, 5 * 1 int16 -> (10,5) (1,2)   (1,2,3,4)    (5,x,x,x)     -> 4 : 2 + 5/4 + !!(5%4) -> 2 + 1 + 1 -> 4
    //3th case, 5 * 1 int32 -> (10,5) (1,3)    (1,2)      (3,4)   (5,x) -> 5 : 2 + 5/2 + !!(5%2) -> 2 + 2 + 1 -> 5

    //with 5*5 int matrix
    //1st case, 5 * 5 int8  -> (10,5) (5,1) (1:25) -> 3 : 2 + 25/8 + !!(25%8) -> 2 + 3  + 1 -> 6
    //2nd case, 5 * 1 int16 -> (10,5) (5,2) (1:25) -> 4 : 2 + 25/4 + !!(25%4) -> 2 + 6  + 1 -> 9
    //3th case, 5 * 5 int32 -> (10,5) (5,3) (1:25) -> 5 : 2 + 25/2 + !!(25%2) -> 2 + 12 + 1 -> 15
    piOffset[_iItemPos] = piOffset[_iItemPos - 1] + 2 + _iRows * _iCols / (sizeof(double) / (_iPrecision % 10 )) + (int)(!!(_iRows * _iCols)) % (sizeof(double) / (_iPrecision % 10 ));

    return sciErr;
}

static SciErr stackapi_allocCommonMatrixOfIntegerInList(void* _pvCtx, int _iVar, const char* _pstName, int* /*_piParent*/, int _iItemPos, int _iPrecision, int _iRows, int _iCols, void** _pvData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int iNewPos    = Top - Rhs + _iVar;
    int* piEnd    = NULL;
    int* piParent   = NULL;

    piParent = stackapi_getLastListAddress(_iVar, _iItemPos);

    sciErr = stackapi_fillCommonMatrixOfIntegerInList(_pvCtx, _iVar, piParent, _iItemPos, _iPrecision, _iRows, _iCols, _pvData);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_ALLOC_INT_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "allocMatrixOfIntegerInList", _iItemPos + 1);
        return sciErr;
    }

    piEnd = (int*) * _pvData + _iRows * _iCols / (sizeof(int) / (_iPrecision % 10)) + (int)(!!(_iRows * _iCols)) % (sizeof(int) / (_iPrecision % 10));
    stackapi_closeList(iNewPos, piEnd);

    if(_iItemPos == piParent[1])
    {
        stackapi_updateListOffset(_pvCtx, _iVar, piParent, _iItemPos, piEnd);
        stackapi_popListAddress(_iVar);
    }
    return sciErr;
}

SciErr stackapi_allocMatrixOfUnsignedInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned char** _pucData)
{
    return stackapi_allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_UINT8, _iRows, _iCols, (void **)_pucData);
}

SciErr stackapi_allocMatrixOfUnsignedInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned short** _pusData)
{
    return stackapi_allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_UINT16, _iRows, _iCols, (void**)_pusData);
}

SciErr stackapi_allocMatrixOfUnsignedInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned int** _puiData)
{
    return stackapi_allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_UINT32, _iRows, _iCols, (void**)_puiData);
}

SciErr stackapi_allocMatrixOfUnsignedInteger64InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned long long** _pullData)
{
    return stackapi_allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_UINT64, _iRows, _iCols, (void**)_pullData);
}

SciErr stackapi_allocMatrixOfInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pcData)
{
    return stackapi_allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_INT8, _iRows, _iCols, (void**)_pcData);
}

SciErr stackapi_allocMatrixOfInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, short** _psData)
{
    return stackapi_allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_INT16, _iRows, _iCols, (void**)_psData);
}

SciErr stackapi_allocMatrixOfInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piData)
{
    return stackapi_allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_INT32, _iRows, _iCols, (void**)_piData);
}

SciErr stackapi_allocMatrixOfInteger64InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, long long** _pllData)
{
    return stackapi_allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_INT64, _iRows, _iCols, (void**)_pllData);
}

static SciErr stackapi_createCommomMatrixOfIntegerInList(void* _pvCtx, int _iVar, const char* _pstName, int* _piParent, int _iItemPos, int _iPrecision, int _iRows, int _iCols, const void* _pvData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    void *pvData = NULL;

    sciErr = stackapi_allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, _pstName, _piParent, _iItemPos, _iPrecision, _iRows, _iCols, &pvData);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_INT_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfIntegerInList", _iItemPos + 1);
        return sciErr;
    }

    if (pvData != NULL)
    {
        memcpy(pvData, _pvData, _iRows * _iCols * (_iPrecision % 10));
    }
    return sciErr;
}

SciErr stackapi_createMatrixOfUnsigedInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned char* _pucData)
{
    return stackapi_createCommomMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_UINT8, _iRows, _iCols, _pucData);
}

SciErr stackapi_createMatrixOfUnsigedInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned short* _pusData)
{
    return stackapi_createCommomMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_UINT16, _iRows, _iCols, _pusData);
}

SciErr stackapi_createMatrixOfUnsigedInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned int* _puiData)
{
    return stackapi_createCommomMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_UINT32, _iRows, _iCols, _puiData);
}

SciErr stackapi_createMatrixOfUnsigedInteger64InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned long long* _pullData)
{
    return stackapi_createCommomMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_UINT64, _iRows, _iCols, _pullData);
}

SciErr stackapi_createMatrixOfInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const char* _pcData)
{
    return stackapi_createCommomMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_INT8, _iRows, _iCols, _pcData);
}

SciErr stackapi_createMatrixOfInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const short* _psData)
{
    return stackapi_createCommomMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_INT16, _iRows, _iCols, _psData);
}

SciErr stackapi_createMatrixOfInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const int* _piData)
{
    return stackapi_createCommomMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_INT32, _iRows, _iCols, _piData);
}

SciErr stackapi_createMatrixOfInteger64InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const long long* _pllData)
{
    return stackapi_createCommomMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_INT64, _iRows, _iCols, _pllData);
}

static SciErr stackapi_getCommonMatrixOfIntegerInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iPrecision, int* _piRows, int* _piCols, void** _pvData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int* piAddr  = NULL;

    sciErr = stackapi_getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_INT_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "stackapi_getMatrixOfIntegerInList", _iItemPos + 1, stackapi_getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    sciErr = stackapi_getCommonMatrixOfInteger(_pvCtx, piAddr, _iPrecision, _piRows, _piCols, _pvData);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_INT_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "stackapi_getMatrixOfIntegerInList", _iItemPos + 1, stackapi_getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    return sciErr;
}

SciErr stackapi_getMatrixOfUnsignedInteger8InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned char** _pucData)
{
    return stackapi_getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_UINT8, _piRows, _piCols, (void**)_pucData);
}

SciErr stackapi_getMatrixOfUnsignedInteger16InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned short** _pusData)
{
    return stackapi_getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_UINT16, _piRows, _piCols, (void**)_pusData);
}

SciErr stackapi_getMatrixOfUnsignedInteger32InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned int** _puiData)
{
    return stackapi_getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_UINT32, _piRows, _piCols, (void**)_puiData);
}

SciErr stackapi_getMatrixOfInteger8InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, char** _pcData)
{
    return stackapi_getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_INT8, _piRows, _piCols, (void**)_pcData);
}

SciErr stackapi_getMatrixOfInteger16InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, short** _psData)
{
    return stackapi_getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_INT16, _piRows, _piCols, (void**)_psData);
}

SciErr stackapi_getMatrixOfInteger32InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int** _piData)
{
    return stackapi_getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_INT32, _piRows, _piCols, (void**)_piData);
}

/*********************
* sparses functions *
*********************/
static SciErr stackapi_fillCommonSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal, const double* _pdblImg, int* _piTotalSize)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int iNbItem    = 0;
    int iTotalLen   = 0;
    int* piOffset   = NULL;
    int* piNbItemRow = NULL;
    int* piColPos   = NULL;
    int* piChildAddr = NULL;
    double* pdblReal = NULL;
    double* pdblImg  = NULL;

    int iItemLen   = 0;

    //Does item can be added in the list
    sciErr = stackapi_getListItemNumber(_pvCtx, _piParent, &iNbItem);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_FILL_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexSparseMatrixInList" : "createComplexSparseMatrixInList", _iItemPos + 1);
        return sciErr;
    }

    if (iNbItem < _iItemPos)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexSparseMatrixInList" : "createSparseMatrixInList", _iItemPos + 1);
        return sciErr;
    }

    sciErr = stackapi_allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_FILL_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexSparseMatrixInList" : "createComplexSparseMatrixInList", _iItemPos + 1);
        return sciErr;
    }

    sciErr = stackapi_fillCommonSparseMatrix(_pvCtx, piChildAddr, _iComplex, _iRows, _iCols, _iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg, &iTotalLen);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_FILL_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexSparseMatrixInList" : "createComplexSparseMatrixInList", _iItemPos + 1);
        return sciErr;
    }

    piOffset      = _piParent + 2;

    iItemLen      = 5 + _iRows + _iNbItem + !((_iRows + _iNbItem) % 2);
    iItemLen      += iTotalLen * 2;
    piOffset[_iItemPos] = piOffset[_iItemPos - 1] + ((iItemLen + 1) / 2);

    memcpy(piNbItemRow, _piNbItemRow, _iRows * sizeof(int));
    memcpy(piColPos, _piColPos, _iNbItem * sizeof(int));

    memcpy(pdblReal, _pdblReal, _iNbItem * sizeof(double));
    if (_iComplex)
    {
        memcpy(pdblImg, _pdblImg, _iNbItem * sizeof(double));
    }

    *_piTotalSize = iTotalLen;
    return sciErr;
}

static SciErr stackapi_createCommonSparseMatrixInList(void* _pvCtx, int _iVar, const char* _pstName, int* /*_piParent*/, int _iItemPos, int _iComplex, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal, const double* _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int* piAddr     = NULL;
    int *piEnd      = NULL;
    int iItemLen    = 0;
    int iTotalLen   = 0;
    int* piParent   = NULL;

    piParent = stackapi_getLastListAddress(_iVar, _iItemPos);
    sciErr = stackapi_getListItemAddress(_pvCtx, piParent, _iItemPos, &piAddr);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexSparseMatrixInList" : "createComplexSparseMatrixInList", _iItemPos + 1);
        return sciErr;
    }

    sciErr = stackapi_fillCommonSparseMatrixInList(_pvCtx, _iVar, piParent, _iItemPos, _iComplex, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg, &iTotalLen);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexSparseMatrixInList" : "createComplexSparseMatrixInList", _iItemPos + 1);
        return sciErr;
    }

    iItemLen      = 5 + _iRows + _iNbItem + !((_iRows + _iNbItem) % 2);
    iItemLen      += iTotalLen * 2;
    piEnd        = piAddr + iItemLen;
    if(_iItemPos == piParent[1])
    {
        stackapi_updateListOffset(_pvCtx, _iVar, piParent, _iItemPos, piEnd);
        stackapi_popListAddress(_iVar);
    }

    stackapi_closeList(_iVar, piEnd);

    return sciErr;
}

SciErr stackapi_createSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal)
{
    return stackapi_createCommonSparseMatrixInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, 0, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

SciErr stackapi_createComplexSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal, const double* _pdblImg)
{
    return stackapi_createCommonSparseMatrixInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, 1, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

static SciErr stackapi_getCommonSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int* piAddr  = NULL;

    sciErr = stackapi_getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_SPARSE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "stackapi_getComplexSparseMatrixInList" : "getSparseMatrixInList", _iItemPos + 1, stackapi_getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    sciErr = stackapi_getCommonSparseMatrix(_pvCtx, piAddr, _iComplex, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_SPARSE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "stackapi_getComplexSparseMatrixInList" : "getSparseMatrixInList", _iItemPos + 1, stackapi_getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    return sciErr;
}

SciErr stackapi_getSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal)
{
    return stackapi_getCommonSparseMatrixInList(_pvCtx, _piParent, _iItemPos, 0, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

SciErr stackapi_getComplexSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
    return stackapi_getCommonSparseMatrixInList(_pvCtx, _piParent, _iItemPos, 1, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

/*****************************
* boolean sparses functions *
*****************************/
static SciErr stackapi_fillBooleanSparseMatrixInList(void* _pvCtx, int _iVar, const char* _pstName, int* /*_piParent*/, int _iItemPos, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int iNbItem    = 0;
    int* piOffset   = NULL;
    int* piNbItemRow = NULL;
    int* piColPos   = NULL;
    int* piChildAddr = NULL;
    int iItemLen   = 0;

    int* piParent = NULL;

    piParent = stackapi_getLastListAddress(_iVar, _iItemPos);
    //Does item can be added in the list
    sciErr = stackapi_getListItemNumber(_pvCtx, piParent, &iNbItem);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_FILL_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createBooleanSparseMatrixInList", _iItemPos + 1);
        return sciErr;
    }

    if (iNbItem < _iItemPos)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), "createBooleanSparseMatrixInList", _iItemPos + 1);
        return sciErr;
    }

    sciErr = stackapi_allocCommonItemInList(_pvCtx, piParent, _iItemPos, &piChildAddr);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_FILL_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createBooleanSparseMatrixInList", _iItemPos + 1);
        return sciErr;
    }

    sciErr = stackapi_fillBooleanSparseMatrix(_pvCtx, piChildAddr, _iRows, _iCols, _iNbItem, &piNbItemRow, &piColPos);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_FILL_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createBooleanSparseMatrixInList", _iItemPos + 1);
        return sciErr;
    }

    piOffset      = piParent + 2;

    iItemLen      = 5 + _iRows + _iNbItem + !((_iRows + _iNbItem) % 2);
    piOffset[_iItemPos] = piOffset[_iItemPos - 1] + ((iItemLen + 1) / 2);

    memcpy(piNbItemRow, _piNbItemRow, _iRows * sizeof(int));
    memcpy(piColPos, _piColPos, _iNbItem * sizeof(int));

    return sciErr;
}

SciErr stackapi_createBooleanSparseMatrixInList(void* _pvCtx, int _iVar, int* /*_piParent*/, int _iItemPos, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int* piAddr     = NULL;
    int *piEnd      = NULL;
    int iItemLen    = 0;
    int* piParent   = stackapi_getLastListAddress(_iVar, _iItemPos);

    sciErr = stackapi_getListItemAddress(_pvCtx, piParent, _iItemPos, &piAddr);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createBooleanSparseMatrixInList", _iItemPos + 1);
        return sciErr;
    }

    sciErr = stackapi_fillBooleanSparseMatrixInList(_pvCtx, _iVar, NULL, piParent, _iItemPos, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createBooleanSparseMatrixInList", _iItemPos + 1);
        return sciErr;
    }

    iItemLen      = 5 + _iRows + _iNbItem + !((_iRows + _iNbItem) % 2);
    piEnd        = piAddr + iItemLen;
    if(_iItemPos == piParent[1])
    {
        stackapi_updateListOffset(_pvCtx, _iVar, piParent, _iItemPos, piEnd);
        stackapi_popListAddress(_iVar);
    }

    stackapi_closeList(_iVar, piEnd);

    return sciErr;
}

SciErr stackapi_getBooleanSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int* piAddr  = NULL;

    sciErr = stackapi_getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getBooleanSparseMatrixInList", _iItemPos + 1, stackapi_getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    sciErr = stackapi_getBooleanSparseMatrix(_pvCtx, piAddr, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getBooleanSparseMatrixInList", _iItemPos + 1, stackapi_getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    return sciErr;
}

/*********************
* Pointer functions *
*********************/
SciErr stackapi_getPointerInList(void* _pvCtx, int* _piParent, int _iItemPos, void** _pvPtr)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int* piAddr  = NULL;

    sciErr = stackapi_getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_POINTER_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getPointerInList", _iItemPos + 1, stackapi_getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    sciErr = stackapi_getPointer(_pvCtx, piAddr, _pvPtr);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_GET_POINTER_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getPointerInList", _iItemPos + 1, stackapi_getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    return sciErr;
}

SciErr stackapi_createPointerInList(void* _pvCtx, int _iVar, int* /*_piParent*/, int _iItemPos, void* _pvPtr)
{
    SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
    int iNbItem         = 0;
    int* piOffset       = NULL;
    int* piChildAddr    = NULL;
    void* pvPtr         = NULL;
    int* piEnd          = NULL;
    int iNewPos         = Top - Rhs + _iVar;
    int* piParent       = stackapi_getLastListAddress(_iVar, _iItemPos);

    //Does item can be added in the list
    sciErr = stackapi_getListItemNumber(_pvCtx, piParent, &iNbItem);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_POINTER_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createListInList", _iItemPos + 1);
        return sciErr;
    }

    if (iNbItem < _iItemPos)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), "stackapi_createPointerInList", _iItemPos + 1);
        return sciErr;
    }


    sciErr = stackapi_allocCommonItemInList(_pvCtx, piParent, _iItemPos, &piChildAddr);
    if(sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_POINTER_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createListInList", _iItemPos + 1);
        return sciErr;
    }

    sciErr = stackapi_fillPointer(_pvCtx, piChildAddr, &pvPtr);
    if (sciErr.iErr)
    {
        stackapi_addErrorMessage(&sciErr, STACKAPI_ERROR_CREATE_POINTER_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createListInList", _iItemPos + 1);
        return sciErr;
    }

    ((double*)pvPtr)[0] = (double) ((unsigned long int) _pvPtr);

    piOffset      = piParent + 2;
    piOffset[_iItemPos] = piOffset[_iItemPos - 1] + 3;//2 for header and 1 for data ( n * 64 bits )

    piEnd = piChildAddr + 6;//4 for header and 2 for data ( n * 32 bits )
    stackapi_closeList(iNewPos, piEnd);

    if(_iItemPos == piParent[1])
    {
        stackapi_updateListOffset(_pvCtx, _iVar, piParent, _iItemPos, piEnd);
        stackapi_popListAddress(_iVar);
    }
    return sciErr;
}

/********************
* tools  functions *
********************/
static void stackapi_updateListOffset(void* _pvCtx, int _iVar, int *_piCurrentNode, int _iItemPos, int *_piEnd)
{
    int iNewPos    = Top - Rhs + _iVar;
    stackapi_updateCommunListOffset(_pvCtx, iNewPos, NULL, _piCurrentNode, _iItemPos, _piEnd);
}

//internal tool functions
static void stackapi_updateCommunListOffset(void* _pvCtx, int _iVar, const char* _pstName, int *_piCurrentNode, int _iItemPos, int *_piEnd)
{
    //find list depth and update list offset for last item
    int i       = 0;
    int iMaxDepth   = 0; //we are already in a list
    int **piParent   = NULL;

    int iLocalVar = _iVar - Top + Rhs;
    iMaxDepth = stackapi_getDepthList(iLocalVar);
    piParent = (int**)MALLOC(sizeof(int*) * iMaxDepth);
    stackapi_getListAdressses(iLocalVar, piParent);

    for (i = iMaxDepth - 2 ; i >= 0 ; i--)
    {
        int j     = 0;
        int iItem   = piParent[i][1];
        int *piOffset = piParent[i] + 2;
        int *piData  = piOffset + iItem + 1 + !(iItem % 2);

        //for all nodes
        for (j = iItem - 1 ; j >= 0 ; j--)
        {
            if(piOffset[j] == -1)
            {
                continue;
            }

            int* piItem = piData + ((piOffset[j] - 1) * 2);

            if (piItem == piParent[i + 1])
            {
                int iOffset = 0;
                iOffset  = piOffset[j] + (int)((_piEnd - piItem + 1) / 2);
                piOffset[j + 1] = iOffset;
                break;
            }
            //else
            //{
            // break;
            // //if this item is not the last of the parent list
            // //we don't need to continue to check the uppers levels
            //}
        }
    }

    FREE(piParent);
}

static void stackapi_closeList(int _iVar, int *_piEnd)
{
    //Get Root address;
    int *piRoot         = istk(iadr(*Lstk(_iVar)));
    int iAddr           = *Lstk(_iVar);

    int iOffsetData     = 2 + piRoot[1] + 1 + !(piRoot[1] % 2);
    int iScale          = (int)(_piEnd - (piRoot + iOffsetData));
    int iDoubleSclale   = (iScale + 1) / 2;

    stackapi_updateLstk(_iVar, sadr(iadr(iAddr) + iOffsetData), iDoubleSclale);
}
/*--------------------------------------------------------------------------*/

int stackapi_isListType(void* _pvCtx, int* _piAddress)
{
    return stackapi_checkVarType(_pvCtx, _piAddress, sci_list);
}
/*--------------------------------------------------------------------------*/
int stackapi_isTListType(void* _pvCtx, int* _piAddress)
{
    return stackapi_checkVarType(_pvCtx, _piAddress, sci_tlist);
}
/*--------------------------------------------------------------------------*/
int stackapi_isMListType(void* _pvCtx, int* _piAddress)
{
    return stackapi_checkVarType(_pvCtx, _piAddress, sci_mlist);
}
/*--------------------------------------------------------------------------*/
