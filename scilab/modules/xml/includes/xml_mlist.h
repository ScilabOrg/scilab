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

#ifndef XML_MLIST_H
#define XML_MLIST_H

#define XMLDOCUMENT 1
#define XMLELEMENT 2
#define XMLATTRIBUTE 3
#define XMLNAMESPACE 4
#define XMLLIST 5
#define XMLNOTHANDLED 6
#define XMLSET 7

/**
 * Creates an XML object at a given position in the stack
 * @param type the object type
 * @param pos the position in the stack
 * @param id the object id
 * @return 1 if all is ok else 0
 */
int createXMLObjectAtPos(int type, int pos, int id);

/**
 * Creates an XML object at a given position in a list
 * @param list the list addr
 * @param stackpos the position of the list in the stack
 * @param type the object type
 * @param pos the position in the list
 * @param id the object id
 * @return 1 if all is ok else 0
 */
int createXMLObjectAtPosInList(int * list, int stackPos, int type, int pos, int id);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
int isXMLDoc(int * mlist);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
int isXMLElem(int * mlist);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
int isXMLAttr(int * mlist);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
int isXMLNs(int * mlist);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
int isXMLList(int * mlist);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
int isXMLObject(int * mlist);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
int isXMLNotHandled(int * mlist);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
int isXMLSet(int * mlist);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
int getXMLObjectId(int * mlist);

#endif
