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

#define XMLDOCUMENT 1
#define XMLELEMENT 2
#define XMLATTRIBUTE 3
#define XMLNAMESPACE 4
#define XMLLIST 5
#define XMLNOTHANDLED 6

#ifndef XML_MLIST
extern int createXMLObjectAtPos(int type, int pos, int id);
extern int createXMLObjectAtPosInList(int * list, int stackPos, int type, int pos, int id);
extern int isXMLDoc(int * mlist);
extern int isXMLElem(int * mlist);
extern int isXMLAttr(int * mlist);
extern int isXMLNs(int * mlist);
extern int isXMLList(int * mlist);
extern int isXMLObject(int * mlist);
extern int isXMLNotHandled(int * mlist);
extern int getXMLObjectId(int * mlist);
#else
static const char * _XMLDoc[] = {"XMLDoc", "_id"};
static const char * _XMLElem[] = {"XMLElem", "_id"};
static const char * _XMLAttr[] = {"XMLAttr", "_id"};
static const char * _XMLNs[] = {"XMLNs", "_id"};
static const char * _XMLList[] = {"XMLList", "_id"};
static const char * _XMLNotHandled[] = {"XMLNH", "_id"};

#define NB_XMLOBJECTS 6
static const char * XMLObjects[] = {"XMLDoc", "XMLElem", "XMLAttr", "XMLNs", "XMLList", "XMLNH"};

#endif
