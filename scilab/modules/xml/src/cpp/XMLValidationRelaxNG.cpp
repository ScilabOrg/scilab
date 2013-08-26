/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "XMLObject.hxx"
#include "XMLValidation.hxx"
#include "XMLValidationRelaxNG.hxx"
#include "XMLDocument.hxx"
#include "VariableScope.hxx"

extern "C"
{
#include "expandPathVariable.h"
#include "MALLOC.h"
#include "localization.h"
#include "BOOL.h"
}

namespace org_modules_xml
{

XMLValidationRelaxNG::XMLValidationRelaxNG(const char *path, std::string * error): XMLValidation()
{
    char *expandedPath = expandPathVariable(const_cast < char *>(path));
    if (expandedPath)
    {
        xmlRelaxNGParserCtxt *pctxt = xmlRelaxNGNewParserCtxt(expandedPath);
        FREE(expandedPath);
        if (!pctxt)
        {
            if (errorBuffer)
            {
                delete errorBuffer;
            }
            errorBuffer = new std::string(gettext("Cannot create a validation context"));
            *error = *errorBuffer;
        }
        else
        {
            validationFile = (void *)xmlRelaxNGParse(pctxt);
            xmlRelaxNGFreeParserCtxt(pctxt);
            if (!validationFile)
            {
                if (errorBuffer)
                {
                    delete errorBuffer;
                }
                errorBuffer = new std::string(gettext("Cannot parse the Relax NG grammar"));
                *error = *errorBuffer;
            }
            else
            {
                openValidationFiles.push_back(this);
            }
        }
    }
    else
    {
        *error = std::string(gettext("Invalid file name: ")) + std::string(path);
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
        xmlRelaxNGFree((xmlRelaxNG *) validationFile);
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

bool XMLValidationRelaxNG::validate(const XMLDocument & doc, std::string * error) const
{
    bool ret;
    xmlRelaxNGValidCtxt *vctxt = xmlRelaxNGNewValidCtxt((xmlRelaxNG *) validationFile);

    if (errorBuffer)
    {
        delete errorBuffer;
    }
    errorBuffer = new std::string("");

    if (!vctxt)
    {
        errorBuffer->append(gettext("Cannot create a validation context"));
        *error = *errorBuffer;
        return false;
    }

    xmlRelaxNGSetValidErrors(vctxt, (xmlRelaxNGValidityErrorFunc) XMLValidation::errorFunction, 0, 0);

    ret = BOOLtobool(xmlRelaxNGValidateDoc(vctxt, doc.getRealDocument()));

    xmlRelaxNGSetValidErrors(vctxt, 0, 0, 0);
    xmlRelaxNGFreeValidCtxt(vctxt);

    if (ret)
    {
        *error = *errorBuffer;
    }

    return ret == 0;
}

bool XMLValidationRelaxNG::validate(xmlTextReader * reader, std::string * error) const
{
    int last;
    int valid;

    if (errorBuffer)
    {
        delete errorBuffer;
    }
    errorBuffer = new std::string();

    xmlTextReaderSetErrorHandler(reader, (xmlTextReaderErrorFunc) XMLValidation::errorReaderFunction, 0);
    xmlTextReaderRelaxNGSetSchema(reader, getValidationFile < xmlRelaxNG > ());

    while ((last = xmlTextReaderRead(reader)) == 1) ;
    valid = xmlTextReaderIsValid(reader);

    xmlTextReaderSetErrorHandler(reader, 0, 0);
    xmlFreeTextReader(reader);

    if (last == -1 || valid != 1)
    {
        *error = *errorBuffer;
        return false;
    }

    return true;
}

const std::string XMLValidationRelaxNG::toString() const
{
    return std::string("XML Relax NG\nNo public information");
}
}
