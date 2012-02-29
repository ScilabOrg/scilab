/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __XML_MLIST_H__
#define __XML_MLIST_H__

#define XMLDOCUMENT 1
#define XMLELEMENT 2
#define XMLATTRIBUTE 3
#define XMLNAMESPACE 4
#define XMLLIST 5
#define XMLNOTHANDLED 6
#define XMLSET 7
#define XMLVALID 8

/**
 * Creates an XML object at a given position in the stack
 * @param type the object type
 * @param pos the position in the stack
 * @param id the object id
 * @return 1 if all is ok else 0
 */
int createXMLObjectAtPos(int type, int pos, int id, void *pvApiCtx);

/**
 * Creates an XML object at a given position in a list
 * @param list the list addr
 * @param stackpos the position of the list in the stack
 * @param type the object type
 * @param pos the position in the list
 * @param id the object id
 * @return 1 if all is ok else 0
 */
int createXMLObjectAtPosInList(int *list, int stackPos, int type, int pos, int id, void *pvApiCtx);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
int isXMLDoc(int *mlist, void *pvApiCtx);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
int isXMLElem(int *mlist, void *pvApiCtx);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
int isXMLAttr(int *mlist, void *pvApiCtx);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
int isXMLNs(int *mlist, void *pvApiCtx);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
int isXMLList(int *mlist, void *pvApiCtx);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
int isXMLObject(int *mlist, void *pvApiCtx);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
int isXMLNotHandled(int *mlist, void *pvApiCtx);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
int isXMLSet(int *mlist, void *pvApiCtx);

/**
 * Checks if a mlist has the good type
 * @param mlist the mlist address
 * @return 1 if the mlist has the good type, else 0.
 */
int isXMLValid(int *mlist, void *pvApiCtx);

/**
 * Get the XMLObject field _id
 * @param mlist the mlist address
 * @return the object id.
 */
int getXMLObjectId(int *mlist, void *pvApiCtx);

#endif /* __XML_MLIST_H__ */
