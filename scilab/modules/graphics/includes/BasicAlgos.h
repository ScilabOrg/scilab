/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: BasicAlgos.h                                                     */

/* desc : Several basic algorithms which can be used in several places in */
/*        the code.                                                       */
/*------------------------------------------------------------------------*/

#ifndef _BASIC_ALGOS_H_
#define _BASIC_ALGOS_H_

#include "machine.h"
#include <string.h>

/**
 * Find the striclty positive minimum value of an array of double.
 * @param[in] x array of size n.
 * @return The stricly posistive minimum of x if exists, a negative value otherwise
 */
double sciFindStPosMin( double x[], int n ) ;

void MaxiInd( double vect[], integer n, integer * ind, double maxi ) ;

/**
 *     convertion d'entier vers double
 *     d et s peuvent en fait pointer sur le meme tableau
 *     car la recopie est fait de n,1,-1
 *      implicit undefined (a-z)
 */
int C2F(entier2d)( integer * n, double * dx,integer * s ) ;

/**
 * check the strict monotony of a vector
 * @return  1 if the vector is increasing or constant
 *         -1 if decreasing and not constant
 *          0 if not monotonous
 * @param nbElement must be greater than 2.
 */
int checkMonotony( double vector[], int nbElement ) ;

/*--------------------------------------------------------------------------*/
/**
 * copy an array into an other
 */
void doubleArrayCopy( double dest[], const double src[], int nbElement ) ;

/**
 * copy an int array into an other
 */
void intArrayCopy( int dest[], const int src[], int nbElement ) ;

/**
 * copy a char * array into an other
 */
void stringArrayCopy( char * dest[], char * src[], int nbElement ) ;

/**
 * set each element of an array to a certain value
 */
void setDoubleArraySingleValue( double dest[], double value, int nbElement ) ;

/**
 * create a new array which is kind of copy of another
 * the elements are copied beginning with the first
 * if the array is longer than the other, then the last values are set to 0.0
 * @param destSize the wanted size for the new array
 * @return  the newly created array or NULL if an error occured
 */
double * createNewArrayFromSource( int destSize, const double src[], int srcSize ) ;

/**
 * free an array of strings
 */
void destroyStringArray( char * src[], int nbStrings ) ;

/**
 * allocate a copy of a double array
 */
double * createDoubleArrayCopy( const double src[], int nbElement ) ;

/**
 * allocate a copy of a int array.
 */
int * createIntArrayCopy( const int src[], int nbElement ) ;

/**
 * allocate a copy of a string array
 */
char ** createStringArrayCopy( char * src[], int nbElement ) ;

/**
 * allocate a string array and set all its element to NULL
 */
char ** createStringArray(int nbElement);
/*--------------------------------------------------------------------------*/

#endif /* _BASIC_ALGOS_H_ */
