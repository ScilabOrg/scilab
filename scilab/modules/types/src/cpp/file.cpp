/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include <fstream>
#include "file.hxx"

extern "C"
{
#include "charEncoding.h"
#include "sci_malloc.h"
}

namespace types
{

File::File()
{
    m_fileDesc = NULL;
    m_iSwap = 0;
    m_pstMode = L"";
    m_iFortranMode = -1; // see clunit.f
    m_iType = 0; // 1 : fortran open   2 : c open
    m_stFilename = L"";
}

File::~File()
{
}

void File::setFileDesc(FILE* _fileDesc)
{
    m_fileDesc = _fileDesc;
}

FILE* File::getFiledesc()
{
    return m_fileDesc;
}

void File::setFileMode(std::wstring _pstMode)
{
    m_pstMode = _pstMode;
}

std::wstring& File::getFileMode()
{
    return m_pstMode;
}

int File::getFileModeAsInt()
{
    int iMode  = 0;
    int iPlus  = 0;
    int iBin   = 0;

    for (int i = 0 ; i < wcslen(m_pstMode.c_str()) ; i++)
    {
        if (m_pstMode[i] == L'r')
        {
            iMode = 1;
        }
        else if (m_pstMode[i] == L'w')
        {
            iMode = 2;
        }
        else if (m_pstMode[i] == L'a')
        {
            iMode = 3;
        }
        else if (m_pstMode[i] == L'+')
        {
            iPlus = 1;
        }
        else if (m_pstMode[i] == L'b')
        {
            iBin = 1;
        }
    }

    return 100 * iMode + 10 * iPlus + iBin;
}

void File::setFileModeAsInt(int _iMode)
{
    int iMode  = (int)(_iMode / 100);
    int iPlus  = (int)((_iMode - iMode * 100) / 10);
    int iBin   = _iMode - iMode * 100 - iPlus * 10;

    m_pstMode = L"";

    switch(iMode)
    {
        case 2 : m_pstMode += L"w"; break;
        case 3 : m_pstMode += L"a"; break;
        default: m_pstMode += L"r"; // default mode "rb"
    }

    if(iPlus)
    {
        m_pstMode += L"+";
    }

    if(iBin)
    {
        m_pstMode += L"b";
    }
}

void File::setFileFortranMode(int _iMode)
{
    m_iFortranMode = _iMode;
}

int File::getFileFortranMode()
{
    return m_iFortranMode;
}

void File::setFileSwap(int _iSwap)
{
    m_iSwap = _iSwap;
}

int File::getFileSwap()
{
    return m_iSwap;
}

void File::setFileType(int _iType)
{
    m_iType = _iType;
}

int File::getFileType()
{
    return m_iType;
}

std::wstring File::getFileTypeAsString()
{
    switch (getFileType())
    {
        case 1 :
            return L"F";
            break;
        case 2 :
            return L"C";
            break;
        default :
            return L"Error";
            break;
    }
}

void File::setFilename(std::wstring _stFilename)
{
    m_stFilename = _stFilename;
}

std::wstring File::getFilename()
{
    return m_stFilename;
}

int File::getCountLines()
{
    char* pstFileName = wide_string_to_UTF8(m_stFilename.c_str());
    std::ifstream in(pstFileName);
    std::string stLine;
    int iLines = 0;

    while (std::getline(in, stLine))
    {
        iLines++;
    }

    in.close();
    FREE(pstFileName);
    return iLines;
}

}
