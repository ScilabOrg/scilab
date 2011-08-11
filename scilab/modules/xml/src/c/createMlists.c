#define XML_MLIST

#include "xml_mlist.h"
#include "api_scilab.h"
#include "stack-c.h"
#include "Scierror.h"
#include "MALLOC.h"

/*--------------------------------------------------------------------------*/
/**
 * Create a XML Object at the given position on the stack
 */
int createXMLObjectAtPos(int type, int pos, int id)
{
    const char ** fields = NULL;
    int * mlistaddr = NULL;
    SciErr err;

    switch (type)
    {
    case XMLDOCUMENT:;
        fields = _XMLDoc;
        break;
    case XMLELEMENT:;
        fields = _XMLElem;
        break;
    case XMLATTRIBUTE:;
        fields = _XMLAttr;
        break;
    case XMLNAMESPACE:;
        fields = _XMLNs;
        break;
    case XMLLIST:;
        fields = _XMLList;
        break;
    case XMLNOTHANDLED:;
        fields = _XMLNotHandled;
        break;
    }

    err = createMList(pvApiCtx, pos, 2, &mlistaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    err = createMatrixOfStringInList(pvApiCtx, pos, mlistaddr, 1, 1, 2, fields);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    err = createMatrixOfInteger32InList(pvApiCtx, pos, mlistaddr, 2, 1, 1, &id);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    return 1;
}
/*--------------------------------------------------------------------------*/
/**
 *
 */
int createXMLObjectAtPosInList(int * list, int stackPos, int type, int pos, int id)
{
    const char ** fields = NULL;
    int * mlistaddr = NULL;
    SciErr err;

    err = createMListInList(pvApiCtx, stackPos, list, pos, 2, &mlistaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    switch (type)
    {
    case XMLDOCUMENT:;
        fields = _XMLDoc;
        break;
    case XMLELEMENT:;
        fields = _XMLElem;
        break;
    case XMLATTRIBUTE:;
        fields = _XMLAttr;
        break;
    case XMLNAMESPACE:;
        fields = _XMLNs;
        break;
    case XMLLIST:;
        fields = _XMLList;
        break;
    case XMLNOTHANDLED:;
        fields = _XMLNotHandled;
        break;
    }

    err = createMatrixOfStringInList(pvApiCtx, pos, mlistaddr, 1, 1, 2, fields);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    err = createMatrixOfInteger32InList(pvApiCtx, pos, mlistaddr, 2, 1, 1, &id);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    return 1;
}
/*--------------------------------------------------------------------------*/
/**
 * Compare a string to the mlist type
 */
int compareStrToMlistType(const char ** str, int nb, int * mlist)
{
    char ** mlist_type = NULL;
    int i = 0, type;
    int rows, cols;
    int * lengths = NULL;
    int cmp = 0;

    SciErr err = getVarType(pvApiCtx, mlist, &type);
    if (err.iErr || type != sci_mlist)
    {
        return 0;
    }

    err = getMatrixOfStringInList(pvApiCtx, mlist, 1, &rows, &cols, NULL, NULL);
    if (err.iErr || rows != 1 || cols <= 0)
    {
        return 0;
    }

    lengths = (int*)MALLOC(sizeof(int) * rows * cols);
    err = getMatrixOfStringInList(pvApiCtx, mlist, 1, &rows, &cols, lengths, NULL);
    if (err.iErr)
    {
        return 0;
    }

    mlist_type = (char **)MALLOC(sizeof(char *) * rows * cols);
    for (; i < rows * cols; i++)
    {
        mlist_type[i] = (char *)MALLOC(sizeof(char) * (lengths[i] + 1));
    }

    err = getMatrixOfStringInList(pvApiCtx, mlist, 1, &rows, &cols, lengths, mlist_type);
    if (err.iErr)
    {
        return 0;
    }

    for (i = 0; i < nb && !cmp; i++)
    {
        cmp = !strcmp(mlist_type[0], str[i]);
    }

    freeAllocatedMatrixOfString(rows, cols, mlist_type);
    FREE(lengths);

    if (cmp)
    {
        // useful when called by isXMLObject
        cmp = i;
    }

    return cmp;
}
/*--------------------------------------------------------------------------*/
int isXMLDoc(int * mlist)
{
    return compareStrToMlistType(XMLObjects, 1, mlist);
}
/*--------------------------------------------------------------------------*/
int isXMLElem(int * mlist)
{
    return compareStrToMlistType(XMLObjects + 1, 1, mlist);
}
/*--------------------------------------------------------------------------*/
int isXMLAttr(int * mlist)
{
    return compareStrToMlistType(XMLObjects + 2, 1, mlist);
}
/*--------------------------------------------------------------------------*/
int isXMLNs(int * mlist)
{
    return compareStrToMlistType(XMLObjects + 3, 1, mlist);
}
/*--------------------------------------------------------------------------*/
int isXMLList(int * mlist)
{
    return compareStrToMlistType(XMLObjects + 4, 1, mlist);
}
/*--------------------------------------------------------------------------*/
int isXMLNotHandled(int * mlist)
{
    return compareStrToMlistType(XMLObjects + 5, 1, mlist);
}
/*--------------------------------------------------------------------------*/
int isXMLObject(int * mlist)
{
    return compareStrToMlistType(XMLObjects, NB_XMLOBJECTS, mlist);
}
/*--------------------------------------------------------------------------*/
int getXMLObjectId(int * mlist)
{
    int * id = NULL;
    int row, col;

    SciErr err = getMatrixOfInteger32InList(pvApiCtx, mlist, 2, &row, &col, &id);
    if (err.iErr)
    {
        printError(&err, 0);
        return -1;
    }

    return *id;
}
