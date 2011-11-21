/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>
#include "setgetSCIpath.h"
#ifdef _MSC_VER
#include <Windows.h>
#endif
#include <stdio.h>
#include "version.h"
#include "scilabmode.h"
#include "localization.h"
#include "FileExist.h"
#include "stricmp.h"
#include "MALLOC.h"
#include "GetXmlFileEncoding.h"
#include "getshortpathname.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
#define FORMAT_VERSION_STRING_PATH "%s/version.incl.xml" 
/*--------------------------------------------------------------------------*/ 
static char *version_string = NULL;
/*--------------------------------------------------------------------------*/ 
void disp_scilab_version(void)
{
    char *ver_str = getVersionString();

	if ( (getScilabMode() == SCILAB_NWNI) || (getScilabMode() == SCILAB_NW) || (getScilabMode() == SCILAB_API) )
	{
		printf(_("Scilab version \"%d.%d.%d.%d\"\n"),SCI_VERSION_MAJOR,SCI_VERSION_MINOR,SCI_VERSION_MAINTENANCE,SCI_VERSION_TIMESTAMP);
		printf("%s\n\n", ver_str);
	}
	else
	{
		#ifdef _MSC_VER
		{
			char msg[1024];
			wsprintf(msg,gettext("Scilab version \"%d.%d.%d.%d\"\n%s\n"),SCI_VERSION_MAJOR,SCI_VERSION_MINOR,SCI_VERSION_MAINTENANCE,SCI_VERSION_TIMESTAMP, ver_str);
			MessageBox(NULL,msg,gettext("Scilab Version Info."),MB_ICONINFORMATION);
		}
		#else
		printf(_("Scilab version \"%d.%d.%d.%d\"\n"),SCI_VERSION_MAJOR,SCI_VERSION_MINOR,SCI_VERSION_MAINTENANCE,SCI_VERSION_TIMESTAMP);
		printf("%s\n\n", ver_str);

		#endif
	}
    FREE(ver_str);
    ver_str = NULL;
}
/*--------------------------------------------------------------------------*/ 
char *getVersionString(void)
{
    if (version_string == NULL)
    {
        char *filename_version_string = NULL;
        char *SciPath = getSCIpath();
        size_t lenpath = strlen(SciPath) + strlen(FORMAT_VERSION_STRING_PATH) + 1;
        filename_version_string = (char*)MALLOC(sizeof(char) * lenpath);
        if (filename_version_string)
        {
            sprintf(filename_version_string, FORMAT_VERSION_STRING_PATH, SciPath);
            if (FileExist(filename_version_string))
            {
                xmlDocPtr doc = NULL;
                char *encoding = GetXmlFileEncoding(filename_version_string);
                /* Don't care about line return / empty line */
                xmlKeepBlanksDefault(0);
                /* check if the XML file has been encoded with utf8 (unicode) or not */
                if ( stricmp("utf-8", encoding)==0) 
                {
                    BOOL bConvert = FALSE;
                    char *shortfilename = getshortpathname(filename_version_string, &bConvert);
                    if (shortfilename)
                    {
                        doc = xmlParseFile (shortfilename);
                        FREE(shortfilename);
                        shortfilename = NULL;
                    }
                }

                if (doc != NULL)
                {
                    xmlXPathContextPtr xpathCtxt = xmlXPathNewContext(doc);
                    xmlXPathObjectPtr xpathObj = xmlXPathEval((const xmlChar*)"//scilab//version_name", xpathCtxt);
                    xmlNodePtr node = xpathObj->nodesetval->nodeTab[0];

                    if (node != NULL)
                    {
                        if (xmlStrEqual (node->name, (const xmlChar*) "version_name"))
                        {
                            const char *str = (const char*)node->children->content;
                            if (str)
                            {
                                // backup version_string for next calls
                                version_string = strdup(str);

                                if(xpathObj) 
                                {
                                    xmlXPathFreeObject(xpathObj);
                                    xpathObj = NULL;
                                }

                                if(xpathCtxt) 
                                {
                                    xmlXPathFreeContext(xpathCtxt);
                                    xpathCtxt = NULL;
                                }

                                if (doc)
                                {
                                    xmlFreeDoc(doc);
                                    doc = NULL;
                                }

                                return strdup(version_string);
                            }
                        }
                    }

                    if(xpathObj) 
                    {
                        xmlXPathFreeObject(xpathObj);
                        xpathObj = NULL;
                    }

                    if(xpathCtxt) 
                    {
                        xmlXPathFreeContext(xpathCtxt);
                        xpathCtxt = NULL;
                    }

                    if (doc)
                    {
                        xmlFreeDoc(doc);
                        doc = NULL;
                    }
                }
            }
        }
        // if a error returns SCI_VERSION_STRING
        version_string = strdup(SCI_VERSION_STRING);
    }

    // returns backup version
    return strdup(version_string);
}
/*--------------------------------------------------------------------------*/ 
