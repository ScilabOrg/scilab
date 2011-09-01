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
#include "XMLValidationRelaxNG.hxx"
#include "XMLDocument.hxx"
#include "VariableScope.hxx"

extern "C" {
#include "expandPathVariable.h"
#include "MALLOC.h"
}

namespace org_modules_xml
{

    XMLValidationRelaxNG::XMLValidationRelaxNG(const char * path, char ** error) : XMLValidation()
    {
        char * expandedPath = expandPathVariable(const_cast<char *>(path));
        xmlRelaxNGParserCtxt * pctxt = xmlRelaxNGNewParserCtxt(expandedPath);
        FREE(expandedPath);
        if (!pctxt)
        {
            *error = const_cast<char *>("Cannot create a validation context");
        }
        else
        {
            validationFile = (void *)xmlRelaxNGParse(pctxt);
            xmlRelaxNGFreeParserCtxt(pctxt);
            if (!validationFile)
            {
                *error = const_cast<char *>("Cannot parse the Relax NG grammar");
            }
            else
            {
                openValidationFiles.push_back(this);
            }
        }
        scope->registerPointers(validationFile, this);
        id = scope->getVariableId(*this);
    }

    XMLValidationRelaxNG::~XMLValidationRelaxNG()
    {
        scope->unregisterPointer(validationFile);
        scope->removeId(id);
        if (validationFile)
        {
            xmlRelaxNGFree((xmlRelaxNG *)validationFile);
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

    bool XMLValidationRelaxNG::validate(const XMLDocument & doc, char ** error) const
    {
        bool ret;
        xmlRelaxNGValidCtxt * vctxt = xmlRelaxNGNewValidCtxt((xmlRelaxNG *)validationFile);

        if (!vctxt)
        {
            *error = const_cast<char *>("Cannot create a validation context");
            return false;
        }

        if (errorBuffer)
        {
            delete errorBuffer;
        }
        errorBuffer = new std::string("");

        xmlRelaxNGSetValidErrors(vctxt, (xmlRelaxNGValidityErrorFunc)XMLValidation::errorFunction, 0, 0);

        ret = xmlRelaxNGValidateDoc(vctxt, doc.getRealDocument());

        xmlRelaxNGSetValidErrors(vctxt, 0, 0, 0);
        xmlRelaxNGFreeValidCtxt(vctxt);

        if (ret)
        {
            *error = const_cast<char *>(errorBuffer->c_str());
        }

        return ret == 0;
    }

    const std::string XMLValidationRelaxNG::toString() const
    {
        std::string str = "XML Relax NG\n";
        str += "No public informations";

        return str;
    }
}
