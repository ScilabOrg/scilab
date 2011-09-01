/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "XMLObject.hxx"
#include "XMLValidation.hxx"
#include "XMLValidationDTD.hxx"
#include "XMLDocument.hxx"
#include "VariableScope.hxx"

extern "C" {
#include "expandPathVariable.h"
#include "MALLOC.h"
}

namespace org_modules_xml
{

    XMLValidationDTD::XMLValidationDTD(const char * path, char ** error) : XMLValidation()
    {
        char * expandedPath = expandPathVariable(const_cast<char *>(path));
        validationFile = (void *)xmlParseDTD(0, (const xmlChar *)expandedPath);
        FREE(expandedPath);
        if (!validationFile)
        {
            *error = const_cast<char *>("Cannot parse the DTD");
        }
        else
        {
            openValidationFiles.push_back(this);
        }
        scope->registerPointers(validationFile, this);
        id = scope->getVariableId(*this);
    }

    XMLValidationDTD::~XMLValidationDTD()
    {
        scope->unregisterPointer(validationFile);
        scope->removeId(id);
        if (validationFile)
        {
            xmlFreeDtd(getValidationFile<xmlDtd>());
            openValidationFiles.remove(this);
            if (openValidationFiles.size() == 0 && XMLDocument::getOpenDocuments().size() == 0)
            {
                resetScope();
            }
        }
        if (errorBuffer)
        {
            delete errorBuffer;
            errorBuffer = 0;
        }
    }

    bool XMLValidationDTD::validate(const XMLDocument & doc, char ** error) const
    {
        bool ret;
        xmlValidCtxt * vctxt = xmlNewValidCtxt();

        if (!vctxt)
        {
            *error = const_cast<char *>("Cannot create a valid context");
            return false;
        }

        if (errorBuffer)
        {
            delete errorBuffer;
        }
        errorBuffer = new std::string("");

        vctxt->error = (xmlValidityErrorFunc)XMLValidation::errorFunction;

        ret = xmlValidateDtd(vctxt, doc.getRealDocument(), getValidationFile<xmlDtd>()) == 1;

        vctxt->error = 0;
        xmlFreeValidCtxt(vctxt);

        if (!ret)
        {
            *error = const_cast<char *>(errorBuffer->c_str());
        }

        return ret;
    }

    const std::string XMLValidationDTD::toString() const
    {
        xmlDtd * dtd = getValidationFile<xmlDtd>();

        std::string str = "XML DTD\n";
        str += "name: " + std::string(dtd->name ? (const char *)dtd->name : "") + "\n";
        str += "external ID: " + std::string(dtd->ExternalID ? (const char *)dtd->ExternalID : "") + "\n";
        str += "system ID: " + std::string(dtd->SystemID ? (const char *)dtd->SystemID : "");

        return str;
    }
}
