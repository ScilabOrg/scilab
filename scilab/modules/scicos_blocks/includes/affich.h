/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Simone Mannori
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _AFFICH_H_
#define _AFFICH_H_

#include "machine.h" /* C2F */

/**
 * Initializattion routine of the text object for Affich block.
 */
void C2F(affichin)(int * fontId, 
                   int * fontSize,
                   int * color,
                   double * winNum, 
                   double * posX, 
                   double * posY,
                   double * width,
                   double * height,
                   double * textIndex);

/**
 * Update of the Affich block text object with a new value.
 */
void C2F(affichup)(double * textIndex,
                   double * winNum,
                   char * value,
                   int valueLength);

/**
 * Initialization of the text object for Affich2 block.
 */
void C2F(affin2)(int * fontId, 
                 int * fontSize,
                 int * color,
                 int * nbRow,
                 int * nbCol,
                 double * winNum, 
                 double * posX, 
                 double * posY,
                 double * width,
                 double * height,
                 double * textIndex);

/**
 * Update of one element of the text object used by Affich2 block.
 */
void C2F(settxtel)(int * numRow, 
                   int * numCol,
                   double * winNum,
                   double * textIndex,
                   char * value,
                   int valueLength);

/**
 * Printing of the text object element used by Affich2 routine
 */
void C2F(affup2)( double * winNum, 
                  double * textIndex);


#endif
