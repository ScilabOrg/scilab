/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2012 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef DATA_PROPERTIES_H
#define DATA_PROPERTIES_H

/*
 * Properties values
 * A static enum would probably be better.
 * To be modified.
 */

#define UNKNOWN_DATA_PROPERTY       0

#define NUM_GONS                    1
#define NUM_VERTICES_PER_GON        2

#define NUM_ELEMENTS                3
#define NUM_ELEMENTS_ARRAY          4
#define COORDINATES                 5
#define X_COORDINATES               6
#define Y_COORDINATES               7
#define Z_COORDINATES               8

#define X_COORDINATES_SHIFT         9
#define Y_COORDINATES_SHIFT         10
#define Z_COORDINATES_SHIFT         11
#define X_COORDINATES_SHIFT_SET     12
#define Y_COORDINATES_SHIFT_SET     13
#define Z_COORDINATES_SHIFT_SET     14
#define Z_COORDINATES_SET           15

#define NUM_VERTICES                16
#define NUM_INDICES                 17
#define INDICES                     18
#define VALUES                      19

#define FEC_ELEMENTS                20
#define NUM_VERTICES_BY_ELEM        21

#define NUM_X                       22
#define NUM_Y                       23
#define NUM_Z                       24
#define GRID_SIZE                   25
#define X_DIMENSIONS                26
#define Y_DIMENSIONS                27

#define COLORS                      28
#define NUM_COLORS                  29

#define MATPLOT_BOUNDS              30
#define MATPLOT_TYPE                31
#define MATPLOT_VIDEO_MODE          32
#define MATPLOT_GL_TYPE             33
#define MATPLOT_DATA_INFOS          34
#define MATPLOT_DATA_TYPE           35
#define MATPLOT_DATA_ORDER          36
#define MATPLOT_IMAGE_TYPE          37
#define MATPLOT_IMAGE_DATA          38
#define MATPLOT_IMAGE_DATASIZE      39
#define DISPLAY_FUNCTION_DATA       40
#define DISPLAY_FUNCTION_DATA_SIZE  41

#endif
