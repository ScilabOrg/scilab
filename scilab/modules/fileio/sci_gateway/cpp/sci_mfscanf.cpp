/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Antoine ELIAS
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "funcmanager.hxx"
#include "fileio_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "scilab_sscanf.hxx"
#include "checkformat.hxx"
#include "yaspio.hxx"
#include "cell.hxx"
#include "int.hxx"

extern "C"
{
#include <stdio.h>
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"
#include "mgetl.h"
#include "do_xxprintf.h"
#include "do_xxscanf.h"
#include "scanf_functions.h"
}


types::Function::ReturnValue sci_mfscanf(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iFile                   = -1; //default file : last opened file
    int size                    = in.size();
    int iNiter                  = 1;
    int iLinesRead              = 0;
    int iErr                    = 0;
    wchar_t* wcsFormat          = NULL;
    wchar_t** wcsRead           = NULL;
    int dimsArray[2]            = {1,1};
    BOOL bIsStd                 = FALSE;
    std::vector<types::InternalType*>* pIT = new std::vector<types::InternalType*>();

    int args        = 0;
    int nrow        = 0;
    int ncol        = 0;
    int retval      = 0;
    int retval_s    = 0;
    int rowcount    = -1;
    rec_entry buf[MAXSCAN];
    entry *data;
    sfdir type[MAXSCAN],type_s[MAXSCAN];

    if(size < 2 || size >3)
    {
       ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"mfscanf", 2, 3);
        return types::Function::Error;
    }

    if(size == 3)
    {
        if(in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isScalar() == false || in[0]->getAs<types::Double>()->isComplex())
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A Real expected.\n"), L"mfscanf", 1);
            return types::Function::Error;
        }
        iNiter = static_cast<int>(in[0]->getAs<types::Double>()->getReal()[0]);
    }

    if(in[size-2]->isDouble() == false || in[size-2]->getAs<types::Double>()->isScalar() == false || in[size-2]->getAs<types::Double>()->isComplex())
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A Real expected.\n"), L"mfscanf", size-1);
        return types::Function::Error;
    }

    if(in[size-1]->isString() == false || in[size-1]->getAs<types::String>()->isScalar() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A String expected.\n"), L"mfscanf", size);
        return types::Function::Error;
    }

    iFile = static_cast<int>(in[size-2]->getAs<types::Double>()->getReal()[0]);
	switch (iFile)
	{
    case 0:
        // stderr
        ScierrorW(999, _W("%ls: Wrong file descriptor: %d.\n"), L"mfscanf", iFile);
        return types::Function::Error;
    case 5:
        // stdin
        if(iNiter < 0)
            iLinesRead = 1;
        else
            iLinesRead = iNiter;
        bIsStd = TRUE;
        break;
    case 6:
        // stdout
        ScierrorW(999, _W("%ls: Wrong file descriptor: %d.\n"), L"mfscanf", iFile);
        return types::Function::Error;
    default :
        break;
    }

    wcsFormat = in[size-1]->getAs<types::String>()->get(0);

// get data
    if(bIsStd == FALSE)
    {
        wcsRead = mgetl(iFile, iNiter, &iLinesRead, &iErr);
        switch(iErr)
        {
            case MGETL_NO_ERROR :
            break;
            case MGETL_EOF :
                sciprintW(_W("Warning: Only %d line(s) read.\n"), iLinesRead);
            break;
            case MGETL_MEMORY_ALLOCATION_ERROR :
                ScierrorW(999, _W("%ls: Memory allocation error in %ls function.\n"), L"mfscanf", L"mgetl");
            return types::Function::Error;
            case MGETL_ERROR :
                ScierrorW(999, _W("%ls: Cannot read file %d.\n"), L"mfscanf", iFile);
            return types::Function::Error;
        }
    }
    else
    {
        wcsRead = (wchar_t**)MALLOC(iLinesRead * sizeof(wchar_t*));
        for(int i = 0; i < iLinesRead; i++)
        {
            wcsRead[i] = to_wide_string(YaspRead());
        }
    }
    nrow = iLinesRead;
	while(++rowcount < iLinesRead)
    {
        if ((iLinesRead >= 0) && (rowcount >= iLinesRead)) break;
        // get data
        int err = do_xxscanf(L"sscanf",(FILE *)0,wcsFormat,&args,wcsRead[rowcount],&retval,buf,type);
        if(err < 0) return types::Function::Error;
        err=Store_Scan(&nrow,&ncol,type_s,type,&retval,&retval_s,buf,&data,rowcount,args);
        if(err < 0)
        {
            switch (err)
            {
                case DO_XXPRINTF_MISMATCH:
                    if (iLinesRead >= 0)
                    {
                        Free_Scan(rowcount,ncol,type_s,&data);
                        ScierrorW(999,_W("%ls: Data mismatch.\n"),L"mfscanf");
                        return types::Function::Error;
                    }
                break;

                case DO_XXPRINTF_MEM_LACK:
                    Free_Scan(rowcount,ncol,type_s,&data);
                    ScierrorW(999,_W("%ls: No more memory.\n"),L"mfscanf");
                    return types::Function::Error;
                break;
            }
            if (err==DO_XXPRINTF_MISMATCH) break;
        }
        FREE(wcsRead[rowcount]);
    }
    FREE(wcsRead);
    unsigned int uiFormatUsed = 0;
    for(int i=0; i<ncol; i++)
    {
        switch ( type_s[i] )
        {
            case SF_C:
            case SF_S:
            {
                types::String* ps = new types::String(iLinesRead,1);
                for(int j=0; j<iLinesRead; j++)
                {
                    ps->set(j, data[i+ncol*j].s);
                }
                pIT->push_back(ps);
                uiFormatUsed |= (1 << 1);
            }
            break;
        	case SF_LUI:
            case SF_SUI:
            case SF_UI:
            case SF_LI:
            case SF_SI:
            case SF_I:
            case SF_LF:
            case SF_F:
            {
                types::Double* p = new types::Double(iLinesRead,1);
                for(int j=0; j<iLinesRead; j++)
                {
                    p->set(j, data[i+ncol*j].d);
                }
                pIT->push_back(p);
                uiFormatUsed |= (1 << 2);
            }
            break;
        }
    }

    int sizeOfVector = pIT->size();
    if(_iRetCount > 1)
    {
        types::Double* pDouble = new types::Double(2,dimsArray);
        pDouble->set(0,retval);
        out.push_back(pDouble);

        for(int i = 0; i < sizeOfVector; i++)
        {
            out.push_back((*pIT)[i]);
        }
        for(int i = sizeOfVector + 1; i < _iRetCount; i++)
        {
            out.push_back(types::Double::Empty());
        }
    }
    else
    {
        if(sizeOfVector == 0)
        {
            out.push_back(types::Double::Empty());
            return types::Function::OK;
        }

        switch(uiFormatUsed)
        {
            case (1 << 1) :
            {
                int sizeOfString = (*pIT)[0]->getAs<types::String>()->getRows();
                int dimsArrayOfRes[2] = {sizeOfString, sizeOfVector};
                types::String* pString = new types::String(2, dimsArrayOfRes);
                for(int i = 0; i < sizeOfVector; i++)
                {
                    for(int j = 0; j < sizeOfString; j++)
                    {
                        pString->set(i*sizeOfString+j, (*pIT)[i]->getAs<types::String>()->get(j));
                    }
                }
                out.push_back(pString);
            }
            break;
            case (1 << 2) :
            {
                int sizeOfDouble = (*pIT)[0]->getAs<types::Double>()->getRows();
                int dimsArrayOfRes[2] = {sizeOfDouble, sizeOfVector};
                types::Double* pDouble = new types::Double(2, dimsArrayOfRes);
                for(int i = 0; i < sizeOfVector; i++)
                {
                    for(int j = 0; j < sizeOfDouble; j++)
                    {
                        pDouble->set(i*sizeOfDouble+j, (*pIT)[i]->getAs<types::Double>()->get(j));
                    }
                }
                out.push_back(pDouble);
            }
            break;
            default :
            {
                std::vector<types::InternalType*>* pITTemp = new std::vector<types::InternalType*>();
                pITTemp->push_back((*pIT)[0]);

                // sizeOfVector always > 1
                for(int i = 1; i < sizeOfVector; i++) // concatenates the Cells. ex : [String 4x1] [String 4x1] = [String 4x2]
                {
                    if(pITTemp->back()->getType() == (*pIT)[i]->getType())
                    {
                        switch(pITTemp->back()->getType())
                        {
                            case types::InternalType::RealString :
                            {
                                int iRows               = pITTemp->back()->getAs<types::String>()->getRows();
                                int iCols               = pITTemp->back()->getAs<types::String>()->getCols();
                                int arrayOfType[2]      = {iRows, iCols + 1};
                                types::String* pType    = new types::String(2, arrayOfType);

                                for(int k=0; k<pITTemp->back()->getAs<types::String>()->getSize(); k++)
                                {
                                    pType->set(k, pITTemp->back()->getAs<types::String>()->get(k));
                                }
                                for(int k=0; k<(*pIT)[i]->getAs<types::String>()->getSize(); k++)
                                {
                                    pType->set(iRows * iCols + k, (*pIT)[i]->getAs<types::String>()->get(k));
                                }
                                pITTemp->pop_back();
                                pITTemp->push_back(pType);
                            }
                            break;
                            case types::InternalType::RealDouble :
                            {
                                int iRows               = pITTemp->back()->getAs<types::Double>()->getRows();
                                int iCols               = pITTemp->back()->getAs<types::Double>()->getCols();
                                int arrayOfType[2]      = {iRows, iCols + 1};
                                types::Double* pType    = new types::Double(2, arrayOfType);

                                pType->set(pITTemp->back()->getAs<types::Double>()->get());
                                for(int k=0; k<(*pIT)[i]->getAs<types::Double>()->getSize(); k++)
                                {
                                    pType->set(iRows * iCols + k, (*pIT)[i]->getAs<types::Double>()->get(k));
                                }
                                pITTemp->pop_back();
                                pITTemp->push_back(pType);
                            }
                            break;
                            default :
                                return types::Function::Error;
                        }
                    }
                    else
                    {
                        pITTemp->push_back((*pIT)[i]);
                    }
                }

                int dimsArrayOfCell[2] = {1, pITTemp->size()};
                types::Cell* pCell = new types::Cell(2, dimsArrayOfCell);
                for(int i = 0; i < pITTemp->size(); i++)
                {
                    pCell->set(i, (*pITTemp)[i]);
                }
                out.push_back(pCell);
            }
        }
    }
	Free_Scan(rowcount,ncol,type_s,&data);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

