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

#ifndef POLYLINE_DECOMPOSER_H
#define POLYLINE_DECOMPOSER_H

#include <string>

extern "C" {
#include <stdio.h>
}

/**
 * Polyline decomposer class
 * Determines the vertices and the segments indices to be rendered
 * as a function of the decomposed Polyline's properties.
 */

class PolylineDecomposer
{

private :

    /**
     * Fills a buffer with vertex data from a polyline decomposed into consecutive segments.
     * @param[in] the id of the given polyline.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the number of coordinates taken by one element in the buffer.
     * @param[in] the byte mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
     * @param[in] the transformation to apply to data.
     * @param[in] the polyline coordinates.
     * @param[in] the polyline's number of points
     * @param[in] the polyline x-shift array.
     * @param[in] the polyline y-shift array.
     * @param[in] the polyline z-shift array.
     */
    static void fillLinearDecompositionVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
        double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift);

    /**
     * Writes the coordinates of a polyline vertex to a buffer.
     * @param[out] the buffer.
     * @param[in] the buffer offset of the vertex's first component.
     * @param[in] the polyline coordinates array.
     * @param[in] the indices of the vertex(ices) polyline to fetch (3 elements, 1 index for each of the X,Y and Z components).
     * @param[in] the polyline's number of points.
     * @param[in] the number of components per buffer element.
     * @param[in] the polyline x-shift array.
     * @param[in] the polyline y-shift array.
     * @param[in] the polyline z-shift array.
     * @param[in] the byte mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
     * @param[in] the transformation to apply to data.
     */
    static void getAndWriteVertexToBuffer(float* buffer, int offset, double* coordinates, int* vertexIndices, int nPoints, int elementsSize,
        double* xshift, double* yshift, double* zshift, int coordinateMask, double* scale, double* translation);

    /**
     * Fills a buffer with vertex data from a polyline using a staircase decomposition.
     * @param[in] the id of the given polyline.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the number of coordinates taken by one element in the buffer.
     * @param[in] the byte mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
     * @param[in] the transformation to apply to data.
     * @param[in] the polyline coordinates.
     * @param[in] the polyline's number of points
     * @param[in] the polyline x-shift array.
     * @param[in] the polyline y-shift array.
     * @param[in] the polyline z-shift array.
     */
    static void fillStairDecompositionVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
        double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift);

    /**
     * Fills a buffer with vertex data from a polyline decomposed into a series of vertical lines.
     * @param[in] the id of the given polyline.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the number of coordinates taken by one element in the buffer.
     * @param[in] the byte mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
     * @param[in] the transformation to apply to data.
     * @param[in] the polyline coordinates.
     * @param[in] the polyline's number of points
     * @param[in] the polyline x-shift array.
     * @param[in] the polyline y-shift array.
     * @param[in] the polyline z-shift array.
     */
    static void fillVerticalLinesDecompositionVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
        double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift);

    /**
     * Writes one component of the coordinates of a bar's four vertices into a buffer.
     * @param[out] the buffer which is written to.
     * @param[in] the offsets of the four vertices (4-element array).
     * @param[in] the offset specifying which component (0, 1 or 2 for respectively X, Y or Z) is written.
     * @param[in] the coordinates of the bar's vertices (4-element array).
     * @param[in] the shift value.
     * @param[in] a flag specifying whether the shift is applied or not.
     * @param[in] the conversion scale factor to apply.
     * @param[in] the conversion translation factor to apply.
     */
    static void writeBarVerticesToBuffer(float* buffer, int* offsets, int componentOffset, double* coordinates, double shift, int shiftUsed, double scale, double translation);

    /**
     * Fills a buffer with vertex data from a polyline decomposed into a series of vertical bars and consecutive segments.
     * @param[in] the id of the given polyline.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the number of coordinates taken by one element in the buffer.
     * @param[in] the byte mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
     * @param[in] the transformation to apply to data.
     * @param[in] the polyline coordinates.
     * @param[in] the polyline's number of points
     * @param[in] the polyline x-shift array.
     * @param[in] the polyline y-shift array.
     * @param[in] the polyline z-shift array.
     */
    static void fillMatlabBarDecompositionVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
        double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift);

    /**
     * Fills a buffer with the indices of a linearly decomposed polyline.
     * @param[in] the id of the polyline.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the polyline's number of points.
     * @param[in] a flag indicating whether the polyline is closed or not.
     */
    static void fillLinearDecompositionIndices(char* id, int* buffer, int bufferLength, int nPoints, int closed);

    /**
     * Fills a buffer with the indices of polyline decomposed into a series of steps.
     * @param[in] the id of the polyline.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the polyline's number of points.
     * @param[in] a flag indicating whether the polyline is closed or not.
     */
    static void fillStairDecompositionIndices(char* id, int* buffer, int bufferLength, int nPoints, int closed);

    /**
     * Fills a buffer with the indices of a polyline decomposed into a series of vertical lines.
     * @param[in] the id of the polyline.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the polyline's number of points.
     */
    static void fillVerticalLinesDecompositionIndices(char* id, int* buffer, int bufferLength, int nPoints);

    /**
     * Fills a buffer with the indices of a polyline decomposed into a series of vertical bars.
     * @param[in] the id of the polyline.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the polyline's number of points.
     */
    static void fillMatlabBarDecompositionIndices(char* id, int* buffer, int bufferLength, int nPoints);

public :

    /**
     * Returns the number of data elements for the given object.
     * @param[in] The given object id.
     * @return The number of data elements.
     */
    static int getDataSize(char* id);

    /**
     * Fills the given buffer with vertex data from the given object.
     * @param[in] the id of the given object.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the number of coordinates taken by one element in the buffer.
     * @param[in] the byte mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
     * @param[in] the transformation to apply to data.
     */
    static void fillVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation);

    /**
     * Returns the number of indices for the given object.
     * @param[in] The given object id.
     * @return The object's number of indices.
     */
    static int getIndicesSize(char* id);

    /**
     * Fills the given buffer with indices data of the given object.
     * @param[in] the id of the given object.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length.
     */
    static void fillIndices(char* id, int* buffer, int bufferLength);

};

#endif
