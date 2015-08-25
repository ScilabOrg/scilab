/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <cstdlib>

#include "XMLtools.hxx"
#include "SLint.hxx"
#include "SLintXMLException.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "charEncoding.h"
#include "localization.h"
}

#define SLINT_XML_ERROR_BUFFER_SIZE 1024

namespace slint
{

    std::ostringstream XMLtools::errorBuffer;

    bool XMLtools::getDouble(xmlNode * node, const char * attrName, double & out)
    {
        xmlAttr * attr = xmlHasProp(node, (const xmlChar *)attrName);
        if (attr)
        {
            out = std::atof((const char *)attr->children->content);
            return true;
        }
        return false;
    }

    bool XMLtools::getInt(xmlNode * node, const char * attrName, int & out)
    {
        xmlAttr * attr = xmlHasProp(node, (const xmlChar *)attrName);
        if (attr)
        {
            out = std::atoi((const char *)attr->children->content);
            return true;
        }
        return false;
    }

    bool XMLtools::getString(xmlNode * node, const char * attrName, std::string & out)
    {
        xmlAttr * attr = xmlHasProp(node, (const xmlChar *)attrName);
        if (attr)
        {
            out = std::string((const char *)attr->children->content);
            return true;
        }
        return false;
    }

    bool XMLtools::getWString(xmlNode * node, const char * attrName, std::wstring & out)
    {
        xmlAttr * attr = xmlHasProp(node, (const xmlChar *)attrName);
        if (attr)
        {
	    wchar_t * content = to_wide_string((const char *)attr->children->content);
            out = std::wstring(content);
	    FREE(content);
	    
            return true;
        }
        return false;
    }

    bool XMLtools::getBool(xmlNode * node, const char * attrName, bool & out)
    {
        xmlAttr * attr = xmlHasProp(node, (const xmlChar *)attrName);
        if (attr)
        {
            std::string val((const char *)attr->children->content);
            if (val == "true" || val == "1")
            {
                out = true;
                return true;
            }
            else if (val == "false" || val == "0")
            {
                out = false;
                return true;
            }
        }
        return false;
    }

    xmlDoc * XMLtools::readXML(const std::wstring & path)
    {
        const std::wstring fullpath = SLint::getFullPath(path);
        char * _path = wide_string_to_UTF8(fullpath.c_str());
        std::string __path(_path);
        FREE(_path);
        xmlParserCtxt * ctxt = xmlNewParserCtxt();
        if (!ctxt)
        {
            throw SLintXMLException(__path, gettext("Cannot create a parser context"));
        }

        xmlSetGenericErrorFunc(ctxt, errorFunction);

        int options = XML_PARSE_NSCLEAN | XML_PARSE_NOBLANKS;
        xmlDoc * doc = xmlCtxtReadFile(ctxt, __path.c_str(), "UTF-8", options);
        if (!doc || !ctxt->valid)
        {
            std::string error = errorBuffer.str();
            errorBuffer.str("");
            errorBuffer.clear();
            throw SLintXMLException(__path, error);
        }

        xmlFreeParserCtxt(ctxt);

        return doc;
    }

    void XMLtools::errorFunction(void * ctx, const char * msg, ...)
    {
        char str[SLINT_XML_ERROR_BUFFER_SIZE];
        va_list args;

        va_start(args, msg);
        vsnprintf(str, SLINT_XML_ERROR_BUFFER_SIZE, msg, args);
        va_end(args);

        errorBuffer << str;
    }

} // namespace slint
