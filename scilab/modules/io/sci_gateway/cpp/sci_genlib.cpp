/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
*
7* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifdef _MSC_VER
#pragma warning(disable : 4996) //It's not beautifull but that works !
#endif

#define DEFAULT_ENCODING "UTF-8"

#define FILE_SEPARATOR L"/"

//XML API
#include <libxml/xpath.h>
#include <libxml/xmlwriter.h>

#include <string.h>
#include "parser.hxx"
#include "context.hxx"
#include "io_gw.hxx"
#include "scilabWrite.hxx"
#include "expandPathVariable.h"
#include "configvariable.hxx"
#include "string.hxx"
#include "library.hxx"
#include "macrofile.hxx"
#include "serializervisitor.hxx"

extern "C"
{
#include "setenvvar.h"
#include "sci_malloc.h"
#include "localization.h"
#include "PATH_MAX.h"
#include "findfiles.h"
#include "FileExist.h"
#include "deleteafile.h"
#include "os_string.h"
#include "splitpath.h"
#include "os_wfopen.h"
#include "sciprint.h"
#include "freeArrayOfString.h"
}


xmlTextWriterPtr openXMLFile(const wchar_t *_pstFilename, const wchar_t* _pstLibName);
void closeXMLFile(xmlTextWriterPtr _pWriter);
bool AddMacroToXML(xmlTextWriterPtr _pWriter, pair<wstring, wstring> _pair);


using namespace types;
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_genlib(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    wchar_t pstParseFile[PATH_MAX + FILENAME_MAX];
    wchar_t pstVerbose[65535];

    int iNbFile	            = 0;
    wchar_t *pstParsePath      = NULL;
    int iParsePathLen		= 0;
    wchar_t* pstLibName		= NULL;
    bool bVerbose           = false;

    if (in.size() < 2 && in.size() > 4)
    {
        return Function::Error;
    }

    //param 1, library name
    InternalType* pIT = in[0];
    if (pIT->isString() == false)
    {
        return Function::Error;
    }

    String *pS = pIT->getAs<types::String>();
    if (pS->getSize() != 1)
    {
        return Function::Error;
    }
    pstLibName = pS->get(0);

    //param 2, library path
    pIT = in[1];
    if (pIT->isString() == false)
    {
        return Function::Error;
    }

    pS = pIT->getAs<types::String>();
    if (pS->isScalar() == false)
    {
        return Function::Error;
    }

    if (in.size() > 2)
    {
        //force flag, do nothing but keep for compatibility
    }

    if (in.size() > 3)
    {
        //versbose flag
        pIT = in[3];
        if (pIT->isBool() == false)
        {
            return Function::Error;
        }

        bVerbose = pIT->getAs<types::Bool>()->get()[0] == 1;
    }

    wchar_t* pstFile = pS->get(0);
    pstParsePath = expandPathVariableW(pstFile);

    os_swprintf(pstParseFile, PATH_MAX + FILENAME_MAX, L"%ls%lslib", pstParsePath, FILE_SEPARATOR);

    if (bVerbose)
    {
        os_swprintf(pstVerbose, 65535, _W("-- Creation of [%ls] (Macros) --\n").c_str(), pstLibName);

        //save current prompt mode
        int oldVal = ConfigVariable::getPromptMode();
        //set mode silent for errors
        ConfigVariable::setPromptMode(0);
        scilabWriteW(pstVerbose);
        //restore previous prompt mode
        ConfigVariable::setPromptMode(oldVal);
    }

    if (FileExistW(pstParseFile))
    {
        deleteafileW(pstParseFile);
    }

    xmlTextWriterPtr pWriter = openXMLFile(pstParseFile, pstLibName);

    if (pWriter == NULL)
    {
        os_swprintf(pstVerbose, 65535, _W("%ls: Cannot open file ''%ls''.\n").c_str(), L"genlib", pstParseFile);
        scilabWriteW(pstVerbose);

        out.push_back(new Bool(0));
        FREE(pstParsePath);
        return Function::OK;
    }


    wchar_t **pstPath = findfilesW(pstParsePath, L"*.sci", &iNbFile, FALSE);

    if (pstPath)
    {
        types::Library* pLib = new types::Library(pstParsePath);
        for (int k = 0 ; k < iNbFile ; k++)
        {
            //version with direct parsing
            //parse the file to find all functions
            wstring stFullPath = wstring(pstParsePath) + wstring(FILE_SEPARATOR) + wstring(pstPath[k]);
            wstring stFullPathBin(stFullPath);
            stFullPathBin.replace(stFullPathBin.end() - 3, stFullPathBin.end(), L"bin");
            wstring pstPathBin(pstPath[k]);
            pstPathBin.replace(pstPathBin.end() - 3, pstPathBin.end(), L"bin");

            //sciprint(_("%ls: Processing file: %ls\n"), L"genlib", pstPath[k]);

            Parser parser;
            parser.parseFile(stFullPath, ConfigVariable::getSCIPath());
            if (parser.getExitStatus() !=  Parser::Succeded)
            {
                os_swprintf(pstVerbose, 65535, _W("%ls: Warning: Error in file %ls : %ls. File ignored\n").c_str(), L"genlib", pstPath[k], parser.getErrorMessage());
                scilabWriteW(pstVerbose);
                delete parser.getTree();
                continue;
            }

            //serialize ast
            ast::SerializeVisitor* s = new ast::SerializeVisitor(parser.getTree());

            unsigned char* serialAst = s->serialize();
            // Header is : buffer size (4 bytes) + scilab version (4 bytes)
            unsigned int size = *((unsigned int*)serialAst);

            FILE* f = os_wfopen(stFullPathBin.c_str(), L"wb");
            fwrite(serialAst, 1, size, f);
            fclose(f);

            ast::exps_t LExp = parser.getTree()->getAs<ast::SeqExp>()->getExps();
            for (ast::exps_t::iterator j = LExp.begin(), itEnd = LExp.end() ; j != itEnd ; ++j)
            {
                if ((*j)->isFunctionDec())
                {
                    ast::FunctionDec* pFD = (*j)->getAs<ast::FunctionDec>();
                    if (AddMacroToXML(pWriter, pair<wstring, wstring>(pFD->getSymbol().getName(), pstPathBin)) == false)
                    {
                        os_swprintf(pstVerbose, 65535, _W("%ls: Warning: %ls information cannot be added to file %ls. File ignored\n").c_str(), L"genlib", pFD->getSymbol().getName().c_str(), pstPath[k]);
                        scilabWriteW(pstVerbose);
                    }

                    pLib->add(pFD->getSymbol().getName(), new types::MacroFile(pFD->getSymbol().getName(), stFullPathBin, pstLibName));
                }
            }

            free(serialAst);
            delete parser.getTree();
        }

        symbol::Context::getInstance()->put(symbol::Symbol(pstLibName), pLib);
    }

    freeArrayOfWideString(pstPath, iNbFile);
    out.push_back(new Bool(1));
    FREE(pstParsePath);
    closeXMLFile(pWriter);
    return Function::OK;
}

void closeXMLFile(xmlTextWriterPtr _pWriter)
{
    int iLen;

    //close opened nodes
    iLen = xmlTextWriterEndElement(_pWriter);
    if (iLen < 0)
    {
        return;
    }

    //close document
    iLen = xmlTextWriterEndDocument(_pWriter);
    if (iLen < 0)
    {
        return;
    }

    //close xml writer
    xmlFreeTextWriter(_pWriter);
}

xmlTextWriterPtr openXMLFile(const wchar_t *_pstFilename, const wchar_t* _pstLibName)
{
    int iLen;
    xmlTextWriterPtr pWriter = NULL;;
    char *pstFilename = wide_string_to_UTF8(_pstFilename);
    char *pstLibName = wide_string_to_UTF8(_pstLibName);


    //create a writer
    pWriter = xmlNewTextWriterFilename(pstFilename, 0);
    if (pWriter == NULL)
    {
        return NULL;
    }

    //setup indentation
    xmlTextWriterSetIndent (pWriter, 1);
    xmlTextWriterSetIndentString (pWriter, (xmlChar*)"  ");

    //create a new document
    iLen = xmlTextWriterStartDocument(pWriter, NULL, DEFAULT_ENCODING, "no");
    if (iLen < 0)
    {
        return NULL;
    }

    //add a node "scilablib"
    iLen = xmlTextWriterStartElement(pWriter, (xmlChar*)"scilablib");
    if (iLen < 0)
    {
        return NULL;
    }

    //Add attribute "name"
    iLen = xmlTextWriterWriteAttribute(pWriter, (xmlChar*)"name", (xmlChar*)pstLibName);
    if (iLen < 0)
    {
        return NULL;
    }

    FREE(pstFilename);
    FREE(pstLibName);

    return pWriter;
}

bool AddMacroToXML(xmlTextWriterPtr _pWriter, pair<wstring, wstring> _pair)
{
    int iLen;

    if (_pWriter == NULL)
    {
        return false;
    }

    //create node "macro"
    iLen = xmlTextWriterStartElement(_pWriter, (xmlChar*)"macro");
    if (iLen < 0)
    {
        return false;
    }

    //Add attribute "name"
    char* pstFirst = wide_string_to_UTF8(_pair.first.c_str());
    iLen = xmlTextWriterWriteAttribute(_pWriter, (xmlChar*)"name", (xmlChar*)pstFirst);
    if (iLen < 0)
    {
        return false;
    }
    FREE(pstFirst);

    //Add attribute "file"
    char* pstSecond = wide_string_to_UTF8(_pair.second.c_str());
    iLen = xmlTextWriterWriteAttribute(_pWriter, (xmlChar*)"file", (xmlChar*)pstSecond);
    if (iLen < 0)
    {
        return false;
    }
    FREE(pstSecond);

    //close "macro" node
    iLen = xmlTextWriterEndElement(_pWriter);
    if (iLen < 0)
    {
        return false;
    }
    return true;
}
