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

/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>
#include "getScilabPreference.h"
#include "GetXmlFileEncoding.h"
#include "MALLOC.h"
#include "FileExist.h"
#include "stricmp.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "getshortpathname.h"
#include "BOOL.h"
#include "SCIHOME.h"

#define XCONF "%s/XConfiguration.xml"

/*--------------------------------------------------------------------------*/
static unsigned char isInit = 0;
static ScilabPreferences scilabPref;
static char * emptyAttribute = "";
/*--------------------------------------------------------------------------*/
static void getPrefs();
static char * getAttribute(xmlDocPtr doc, xmlXPathContextPtr xpathCtxt, const char * xpath);
static void initPrefs();
static void getDocAndCtxt(xmlDocPtr * doc, xmlXPathContextPtr * xpathCtxt);
/*--------------------------------------------------------------------------*/
const ScilabPreferences * getScilabPreferences()
{
    getPrefs();
    return &scilabPref;
}
/*--------------------------------------------------------------------------*/
void initPrefs()
{
    scilabPref.heapSize = NULL;
    scilabPref.adaptToDisplay = NULL;
    scilabPref.columnsToDisplay = NULL;
    scilabPref.linesToDisplay = NULL;
    scilabPref.historySaveAfter = NULL;
    scilabPref.historyFile = NULL;
    scilabPref.historyLines = NULL;
    scilabPref.historyEnable = NULL;
    scilabPref.ieee = NULL;
    scilabPref.format = NULL;
    scilabPref.formatWidth = NULL;
    scilabPref.language = NULL;
    scilabPref.startup_dir_use = NULL;
    scilabPref.startup_dir_default = NULL;
    scilabPref.startup_dir_previous = NULL;
}
/*--------------------------------------------------------------------------*/
void reloadScilabPreferences()
{
    clearScilabPreferences();
    getPrefs();
}
/*--------------------------------------------------------------------------*/
void clearScilabPreferences()
{
    if (isInit == 1)
    {
        if (scilabPref.heapSize)
        {
            FREE((void*)scilabPref.heapSize);
        }
        if (scilabPref.adaptToDisplay)
        {
            FREE((void*)scilabPref.adaptToDisplay);
        }
        if (scilabPref.columnsToDisplay)
        {
            FREE((void*)scilabPref.columnsToDisplay);
        }
        if (scilabPref.linesToDisplay)
        {
            FREE((void*)scilabPref.linesToDisplay);
        }
        if (scilabPref.historySaveAfter)
        {
            FREE((void*)scilabPref.historySaveAfter);
        }
        if (scilabPref.historyFile)
        {
            FREE((void*)scilabPref.historyFile);
        }
        if (scilabPref.historyLines)
        {
            FREE((void*)scilabPref.historyLines);
        }
        if (scilabPref.historyEnable)
        {
            FREE((void*)scilabPref.historyEnable);
        }
        if (scilabPref.ieee)
        {
            FREE((void*)scilabPref.ieee);
        }
        if (scilabPref.format)
        {
            FREE((void*)scilabPref.format);
        }
        if (scilabPref.formatWidth)
        {
            FREE((void*)scilabPref.formatWidth);
        }
        if (scilabPref.language)
        {
            FREE((void*)scilabPref.language);
        }
        if (scilabPref.startup_dir_use)
        {
            FREE((void*)scilabPref.startup_dir_use);
        }
        if (scilabPref.startup_dir_default)
        {
            FREE((void*)scilabPref.startup_dir_default);
        }
        if (scilabPref.startup_dir_previous)
        {
            FREE((void*)scilabPref.startup_dir_previous);
        }
        initPrefs();
    }
    isInit = 0;
}
/*--------------------------------------------------------------------------*/
void getPrefs()
{
    if (!isInit)
    {
        xmlDocPtr doc = NULL;
        xmlXPathContextPtr xpathCtxt = NULL;

        initPrefs();

        getDocAndCtxt(&doc, &xpathCtxt);
        if (doc == NULL)
        {
            return;
        }

        scilabPref.heapSize = strdup(getAttribute(doc, xpathCtxt, HEAPSIZE_XPATH));
        scilabPref.adaptToDisplay = strdup(getAttribute(doc, xpathCtxt, ADAPTTODISPLAY_XPATH));
        scilabPref.columnsToDisplay = strdup(getAttribute(doc, xpathCtxt, COLUMNSTODISPLAY_XPATH));
        scilabPref.linesToDisplay = strdup(getAttribute(doc, xpathCtxt, LINESTODISPLAY_XPATH));
        scilabPref.historySaveAfter = strdup(getAttribute(doc, xpathCtxt, HISTORYSAVEAFTER_XPATH));
        scilabPref.historyFile = strdup(getAttribute(doc, xpathCtxt, HISTORYFILE_XPATH));
        scilabPref.historyLines = strdup(getAttribute(doc, xpathCtxt, HISTORYLINES_XPATH));
        scilabPref.historyEnable = strdup(getAttribute(doc, xpathCtxt, HISTORYENABLE_XPATH));
        scilabPref.ieee = strdup(getAttribute(doc, xpathCtxt, IEEE_XPATH));
        scilabPref.format = strdup(getAttribute(doc, xpathCtxt, FORMAT_XPATH));
        scilabPref.formatWidth = strdup(getAttribute(doc, xpathCtxt, FORMATWIDTH_XPATH));
        scilabPref.language = strdup(getAttribute(doc, xpathCtxt, LANGUAGE_XPATH));
        scilabPref.startup_dir_use = strdup(getAttribute(doc, xpathCtxt, STARTUP_DIR_USE_XPATH));
        scilabPref.startup_dir_default = strdup(getAttribute(doc, xpathCtxt, STARTUP_DIR_DEFAULT_XPATH));
        scilabPref.startup_dir_previous = strdup(getAttribute(doc, xpathCtxt, STARTUP_DIR_PREVIOUS_XPATH));

        xmlXPathFreeContext(xpathCtxt);
        xmlFreeDoc(doc);

        isInit = 1;
    }
}
/*--------------------------------------------------------------------------*/
char * getAttribute(xmlDocPtr doc, xmlXPathContextPtr xpathCtxt, const char * xpath)
{
    char * value = emptyAttribute;
    xmlXPathObjectPtr xpathObj = xmlXPathEval((const xmlChar*)xpath, xpathCtxt);
    if (xpathObj && xpathObj->nodesetval && xpathObj->nodesetval->nodeMax)
    {
        value = (char *)((xmlAttrPtr)xpathObj->nodesetval->nodeTab[0])->children->content;
    }

    if (xpathObj)
    {
        xmlXPathFreeObject(xpathObj);
    }

    return value;
}
/*--------------------------------------------------------------------------*/
void getDocAndCtxt(xmlDocPtr * doc, xmlXPathContextPtr * xpathCtxt)
{
    char * SCIHOME = NULL;
    char * path = NULL;
    BOOL bConvert = FALSE;
    char * shortfilename_xml_conf = NULL;
    char * ret = NULL;
    *doc = NULL;
    *xpathCtxt = NULL;

    SCIHOME = getSCIHOME();
    path = (char *)MALLOC(strlen(SCIHOME) + strlen(XCONF));

    sprintf(path, XCONF, SCIHOME);
    FREE(SCIHOME);

    if (FileExist(path))
    {
        shortfilename_xml_conf = getshortpathname(path, &bConvert);
        if (shortfilename_xml_conf)
        {
            *doc = xmlParseFile(shortfilename_xml_conf);
            FREE(shortfilename_xml_conf);
            shortfilename_xml_conf = NULL;
        }
    }

    FREE(path);
    path = NULL;

    if (*doc == NULL)
    {
        *xpathCtxt = NULL;
        return;
    }

    if (stricmp((*doc)->encoding, "utf-8"))
    {
        xmlFreeDoc(*doc);
        *doc = NULL;
        *xpathCtxt = NULL;
        return;
    }

    *xpathCtxt = xmlXPathNewContext(*doc);
    if (*xpathCtxt == NULL)
    {
        xmlFreeDoc(*doc);
        *doc = NULL;
    }
}
/*--------------------------------------------------------------------------*/
char * getPrefAttributeValue(const char * xpath, const char * attribute)
{
    xmlDocPtr doc = NULL;
    xmlXPathContextPtr xpathCtxt = NULL;
    char * ret = NULL;
    unsigned int xlen = 0;
    unsigned int alen = 0;
    char * query = NULL;

    if (!xpath || !attribute)
    {
        return NULL;
    }

    query = (char *)MALLOC((xlen + alen + 2 + 1) * sizeof(char));
    if (!query)
    {
        return NULL;
    }

    xlen = (unsigned int) strlen(xpath);
    alen = (unsigned int) strlen(attribute);

    getDocAndCtxt(&doc, &xpathCtxt);
    if (doc == NULL)
    {
        FREE(query);
        return NULL;
    }

    sprintf(query, "%s/@%s", xpath, attribute);
    query[xlen + alen + 2] = '\0';

    ret = strdup(getAttribute(doc, xpathCtxt, (const xmlChar*)query));
    FREE(query);

    xmlXPathFreeContext(xpathCtxt);
    xmlFreeDoc(doc);

    return ret;
}
/*--------------------------------------------------------------------------*/
char ** getPrefAttributesValues(const char * xpath, const char ** attributes, const unsigned int attrLen)
{
    xmlDocPtr doc = NULL;
    xmlXPathContextPtr xpathCtxt = NULL;
    xmlXPathObjectPtr xpathObj = NULL;
    char ** ret = NULL;

    if (!xpath || !attributes || !attrLen)
    {
        return NULL;
    }

    getDocAndCtxt(&doc, &xpathCtxt);
    if (doc == NULL || xpathCtxt == NULL)
    {
        return NULL;
    }

    xpathObj = xmlXPathEval((const xmlChar*)xpath, xpathCtxt);
    if (xpathObj && xpathObj->nodesetval && xpathObj->nodesetval->nodeMax)
    {
        unsigned int i;
        xmlNode * node = (xmlNode*)xpathObj->nodesetval->nodeTab[0];
        ret = (char**)MALLOC(sizeof(char*) * attrLen);
        if (!ret)
        {
            xmlXPathFreeObject(xpathObj);
            xmlXPathFreeContext(xpathCtxt);
            xmlFreeDoc(doc);
            return NULL;
        }

        for (i = 0; i < attrLen; i++)
        {
            xmlAttr * attrs = xmlHasProp(node, (const xmlChar *)attributes[i]);
            if (attrs)
            {
                ret[i] = strdup((const char *)attrs->children->content);
            }

            if (!attrs || !ret[i])
            {
                unsigned int j;
                for (j = 0; j < i; j++)
                {
                    free(ret[j]);
                }
                FREE(ret);
                ret = NULL;
                break;
            }
        }
    }

    if (xpathObj)
    {
        xmlXPathFreeObject(xpathObj);
    }

    xmlXPathFreeContext(xpathCtxt);
    xmlFreeDoc(doc);

    return ret;
}
/*--------------------------------------------------------------------------*/
void setPrefAttributesValues(const char * xpath, const char ** kv, const unsigned int kvLen)
{
    xmlDocPtr doc = NULL;
    xmlXPathContextPtr xpathCtxt = NULL;
    xmlXPathObjectPtr xpathObj = NULL;
    char * SCIHOME = NULL;
    char * path = NULL;
    BOOL bConvert = FALSE;
    char * shortfilename_xml_conf = NULL;

    if (!xpath || !kv || !kvLen)
    {
        return;
    }

    getDocAndCtxt(&doc, &xpathCtxt);
    if (doc == NULL)
    {
        return;
    }

    xpathObj = xmlXPathEval((const xmlChar*)xpath, xpathCtxt);
    if (xpathObj && xpathObj->nodesetval && xpathObj->nodesetval->nodeMax)
    {
        int i;
        xmlNode * node = (xmlNode*)xpathObj->nodesetval->nodeTab[0];
        for (i = 0; i < (int)kvLen / 2; i++)
        {
            xmlAttr * attrs = xmlHasProp(node, (const xmlChar *)kv[2 * i]);
            if (attrs)
            {
                attrs->children->content = xmlStrdup((const xmlChar *)kv[2 * i + 1]);
            }
            else
            {
                break;
            }
        }
    }

    if (xpathObj)
    {
        xmlXPathFreeObject(xpathObj);
    }

    xmlXPathFreeContext(xpathCtxt);

    // Save
    xmlThrDefIndentTreeOutput(1);

    SCIHOME = getSCIHOME();
    path = (char *)MALLOC(strlen(SCIHOME) + strlen(XCONF));

    sprintf(path, XCONF, SCIHOME);
    FREE(SCIHOME);

    if (FileExist(path))
    {
        shortfilename_xml_conf = getshortpathname(path, &bConvert);
        if (shortfilename_xml_conf)
        {
            xmlSaveFormatFile(shortfilename_xml_conf, doc, 2);
            FREE(shortfilename_xml_conf);
            shortfilename_xml_conf = NULL;
        }
    }

    FREE(path);
    path = NULL;

    xmlFreeDoc(doc);
}
