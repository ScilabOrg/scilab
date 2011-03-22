/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef PLOT3D_DECOMPOSER_H
#define PLOT3D_DECOMPOSER_H

#include "NgonGridDataDecomposer.hxx"

#include <string>

extern "C" {
#include <stdlib.h>
}

/**
 * Plot3D decomposer class
 * Determines the vertices and the segments indices to be rendered
 * as a function of the decomposed Plot3D object's properties.
 */

class Plot3DDecomposer : public NgonGridDataDecomposer
{

private :

    /** Plot3D decomposer instance */
    static Plot3DDecomposer* decomposer;

protected :

    /**
     * Returns the z-coordinate of the (i,j) grid point.
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid's number of points along the x-axis.
     * @param[in] the grid's number of points along the y-axis.
     * @param[in] the point's x index.
     * @param[in] the point's y index.
     * @return the (i,j) grid point's z-coordinate.
     */
    double getZCoordinate(double* z, int numX, int numY, int i, int j);

    /**
     * Returns the z-coordinate of the (i,j) grid point, taking
     * into account logarithmic scaling.
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid's number of points along the x-axis.
     * @param[in] the grid's number of points along the y-axis.
     * @param[in] the point's x index.
     * @param[in] the point's y index.
     * @param[in] a flag specifying whether logarithmic scaling is used.
     * @return the (i,j) grid point's z-coordinate.
     */
    double getZCoordinate(double* z, int numX, int numY, int i, int j, int logUsed);

public :

    /**
     * Returns the class' single instance.
     * @return the class instance
     */
    static Plot3DDecomposer* get(void)
    {
        if (decomposer == NULL)
        {
            decomposer = new Plot3DDecomposer();
        }

        return decomposer;
    }

    /**
     * Fills the given buffer with vertex data from the given object.
     * @param[in] the id of the given object.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the number of coordinates taken by one element in the buffer.
     * @param[in] the byte mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
     * @param[in] the transformation to apply to data.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     */
    static void fillVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask);

    /**
     * Fills the given buffer with color data from the given object.
     * @param[in] the id of the given object.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the number of coordinates taken by one element in the buffer.
     */
    static void fillColors(char* id, float* buffer, int bufferLength, int elementsSize);

    /**
     * Fills the given buffer with index data from the given object.
     * @param[in] the id of the given object.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @return the number of indices actually written.
     */
    static int fillIndices(char* id, int* buffer, int bufferLength, int logMask);

    /**
     * Returns the number of wireframe indices for the given object.
     * It is equal to twice the number of segments that can be at most
     * drawn according to the Plot3D object's properties and therefore does not
     * take non-representable points into account.
     * @param[in] the given object id.
     * @return the object's number of indices.
     */
    static int getWireIndicesSize(char* id);

    /**
     * Fills the given buffer with wireframe indices data of the given object.
     * The number of indices actually written is equal to or less than
     * the buffer length, which is the object's maximum number of indices.
     * @param[in] the id of the given object.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @return the number of indices actually written.
     */
    static int fillWireIndices(char* id, int* buffer, int bufferLength, int logMask);
};

#endif
