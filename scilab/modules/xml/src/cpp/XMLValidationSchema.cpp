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
#include "XMLValidationSchema.hxx"
#include "XMLDocument.hxx"
#include "VariableScope.hxx"

extern "C" {
#include "expandPathVariable.h"
#include "MALLOC.h"
}

namespace org_modules_xml
{

    XMLValidationSchema::XMLValidationSchema(const char * path, char ** error) : XMLValidation()
    {
        char * expandedPath = expandPathVariable(const_cast<char *>(path));
        xmlSchemaParserCtxt * pctxt = xmlSchemaNewParserCtxt(expandedPath);
        FREE(expandedPath);
        if (!pctxt)
        {
            *error = const_cast<char *>("Cannot create a validation context");
        }
        else
        {
            validationFile = (void *)xmlSchemaParse(pctxt);
            xmlSchemaFreeParserCtxt(pctxt);
            if (!validationFile)
            {
                *error = const_cast<char *>("Cannot parse the schema");
            }
            else
            {
                openValidationFiles.push_back(this);
            }
        }
        scope->registerPointers(validationFile, this);
        id = scope->getVariableId(*this);
    }

    XMLValidationSchema::~XMLValidationSchema()
    {
        scope->unregisterPointer(validationFile);
        scope->removeId(id);
        if (validationFile)
        {
            xmlSchemaFree((xmlSchema *)validationFile);
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

    bool XMLValidationSchema::validate(const XMLDocument & doc, char ** error) const
    {
        bool ret;
        xmlSchemaValidCtxt * vctxt = xmlSchemaNewValidCtxt((xmlSchema *)validationFile);

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

        xmlSchemaSetValidErrors(vctxt, (xmlSchemaValidityErrorFunc)XMLValidation::errorFunction, 0, 0);

        ret = xmlSchemaValidateDoc(vctxt, doc.getRealDocument());

        xmlSchemaSetValidErrors(vctxt, 0, 0, 0);
        xmlSchemaFreeValidCtxt(vctxt);

        if (ret)
        {
            *error = const_cast<char *>(errorBuffer->c_str());
        }

        return ret == 0;
    }


    const std::string XMLValidationSchema::toString() const
    {
        xmlSchema * schema = getValidationFile<xmlSchema>();

        std::string str = "XML Schema\n";
        str += "name: " + std::string(schema->name ? (const char *)schema->name : "") + "\n";
        str += "target namespace: " + std::string(schema->targetNamespace ? (const char *)schema->targetNamespace : "") + "\n";
        str += "version: " + std::string(schema->version ? (const char *)schema->version : "");

        return str;
    }

}
