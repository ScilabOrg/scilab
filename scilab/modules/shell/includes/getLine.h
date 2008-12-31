/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __GETLINE_H__
#define __GETLINE_H__


/**
* line editor
* @param buffer
* @param buffer size
* @param length line
* @param eof
*/ 
void getLine(char *buffer,int *buf_size,int *len_line,int * eof);

#endif /* __GETLINE_H__ */
/*--------------------------------------------------------------------------*/


