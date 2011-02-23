/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Pierre Lando
 *  Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <iostream>

#include "PolylineDecomposer.hxx"

extern "C"
{
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

int PolylineDecomposer::getDataSize(char* id)
{
    int nPoints = 0;
    int *piNPoints = &nPoints;
    int polylineStyle = 0;
    int* piPolylineStyle = &polylineStyle;
    int closed = 0;
    int* piClosed = &closed;

    getGraphicObjectProperty(id, __GO_POLYLINE_STYLE__, jni_int, (void**) &piPolylineStyle);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**) &piNPoints);
    getGraphicObjectProperty(id, __GO_CLOSED__, jni_bool, (void**) &piClosed);

    /* If 0 points, 0 elements */
    if (nPoints == 0)
    {
        return 0;
    }

    /* Segments */
    if (polylineStyle == 1)
    {
        return nPoints;
    }
    /* Staircase */
    else if (polylineStyle == 2)
    {
        if (closed)
        {
            return 2*nPoints;
        }
        else
        {
            return (nPoints*2)-1;
        }
    }
    /* Vertical segments plus segments */
    else if (polylineStyle == 3)
    {
        /* To be done: take into account the line mode */
        return nPoints*2;
    }
    /* Arrowed segments */
    else if (polylineStyle == 4)
    {
        return nPoints;
    }
    /* Vertical bars plus segments */
    else if (polylineStyle == 6)
    {
        return 5*nPoints;
    }
    /* To be done: remaining styles */
    else
    {
        return 0;
    }

}

void PolylineDecomposer::fillVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation)
{
    double* t;
    double* xshift;
    double* yshift;
    double* zshift;

    int polylineStyle = 0;
    int* piPolylineStyle = &polylineStyle;

    int nPoints = 0;
    int *piNPoints = &nPoints;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_COORDINATES__, jni_double_vector, (void**) &t);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**) &piNPoints);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_X_COORDINATES_SHIFT__, jni_double_vector, (void**) &xshift);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y_COORDINATES_SHIFT__, jni_double_vector, (void**) &yshift);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__, jni_double_vector, (void**) &zshift);

    getGraphicObjectProperty(id, __GO_POLYLINE_STYLE__, jni_int, (void**) &piPolylineStyle);

    if (polylineStyle == 1)
    {
        fillLinearDecompositionVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, t, nPoints, xshift, yshift, zshift);
    }
    else if (polylineStyle == 2)
    {
        fillStairDecompositionVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, t, nPoints, xshift, yshift, zshift);
    }
    else if (polylineStyle == 3)
    {
        fillVerticalLinesDecompositionVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, t, nPoints, xshift, yshift, zshift);
    }
    else if (polylineStyle == 4)
    {
        fillLinearDecompositionVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, t, nPoints, xshift, yshift, zshift);
    }
    else if (polylineStyle == 6)
    {
        fillMatlabBarDecompositionVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, t, nPoints, xshift, yshift, zshift);
    }

}

void PolylineDecomposer::fillLinearDecompositionVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
    double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift)
{
    double coordinate;

    int componentIndices[3];
    int i;

    // TODO Optimize ? (test if s = 1 and t = 0, coordinateMask = 0 ...)
    for (i = 0; i < nPoints; i++)
    {
        /* Offset of a polyline vertex */
        int v0;

        v0 = elementsSize*i;

        componentIndices[0] = i;
        componentIndices[1] = i;
        componentIndices[2] = i;

        getAndWriteVertexToBuffer(buffer, v0, coordinates, componentIndices, nPoints, elementsSize,
            xshift, yshift, zshift, coordinateMask, scale, translation);
    }

}

void PolylineDecomposer::getAndWriteVertexToBuffer(float* buffer, int offset, double* coordinates, int* vertexIndices, int nPoints, int elementsSize,
    double* xshift, double* yshift, double* zshift, int coordinateMask, double* scale, double* translation)
{
    double coordinate;

    if (coordinateMask & 0x01)
    {
        coordinate = coordinates[vertexIndices[0]];

        if (xshift != NULL)
        {
            coordinate += xshift[vertexIndices[0]];
        }

        buffer[offset +0] = coordinate * scale[0] + translation[0];
    }

    if (coordinateMask & 0x02)
    {
        coordinate = coordinates[vertexIndices[1] + nPoints];

        if (yshift != NULL)
        {
            coordinate += yshift[vertexIndices[1]];
        }

        buffer[offset +1] = coordinate * scale[1] + translation[1];
    }

    if (coordinateMask & 0x04)
    {
        coordinate = coordinates[vertexIndices[2] + 2*nPoints];

        if (zshift != NULL)
        {
            coordinate += zshift[vertexIndices[2]];
        }

        buffer[offset +2] = coordinate * scale[2] + translation[2];
    }

    if((elementsSize == 4) && (coordinateMask & 0x08))
    {
        buffer[offset +3] = 1.0;
    }

}

void PolylineDecomposer::fillStairDecompositionVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
    double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift)
{
    int i;

    int closed = 0;
    int* piClosed = &closed;

    /* Offsets of the left and right vertices (respectively) */
    int v0;
    int v1;

    int componentIndices[3];

    if (nPoints == 0)
    {
        return;
    }

    getGraphicObjectProperty(id, __GO_CLOSED__, jni_bool, (void**) &piClosed);

    for (i = 0; i < nPoints-1; i++)
    {
        v0 = elementsSize*2*i;
        v1 = elementsSize*(2*i+1);

        componentIndices[0] = i;
        componentIndices[1] = i;
        componentIndices[2] = i;

        getAndWriteVertexToBuffer(buffer, v0, coordinates, componentIndices, nPoints, elementsSize, xshift, yshift, zshift, coordinateMask, scale, translation);

        componentIndices[0] = i+1;
        componentIndices[1] = i;
        componentIndices[2] = i;

        getAndWriteVertexToBuffer(buffer, v1, coordinates, componentIndices, nPoints, elementsSize, xshift, yshift, zshift, coordinateMask, scale, translation);
    }

    /* Last point */
    v0 = elementsSize*2*i;

    componentIndices[0] = i;
    componentIndices[1] = i;
    componentIndices[2] = i;

    getAndWriteVertexToBuffer(buffer, v0, coordinates, componentIndices, nPoints, elementsSize, xshift, yshift, zshift, coordinateMask, scale, translation);

    /*
     * One additional vertex if closed
     * Its x-coordinate is equal to the first polyline point's one,
     * whereas its y and z coordinates are equal to those of the last point.
     */
    if (closed)
    {
        v0 = elementsSize*(2*i+1);

        componentIndices[0] = 0;
        componentIndices[1] = i;
        componentIndices[2] = i;

        getAndWriteVertexToBuffer(buffer, v0, coordinates, componentIndices, nPoints, elementsSize, xshift, yshift, zshift, coordinateMask, scale, translation);
    }

}

void PolylineDecomposer::fillVerticalLinesDecompositionVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
    double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift)
{
    int i;

    for (i = 0; i < nPoints; i++)
    {
        /* Offsets of the lower and upper vertices (respectively) */
        int v0;
        int v1;

        /* Coordinates of the lower and upper vertices (respectively) */
        double coord0;
        double coord1;

        v0 = elementsSize*2*i;
        v1 = elementsSize*(2*i+1);

        /* Lower and upper endpoints x coordinates */
        if (coordinateMask & 0x01)
        {
            coord0 = coordinates[i];
            coord1 = coordinates[i];

            if (xshift != NULL)
            {
                coord0 += xshift[i];
                coord1 += xshift[i];
            }

            buffer[v0 +0] = coord0 * scale[0] + translation[0];
            buffer[v1 +0] = coord1 * scale[0] + translation[0];
        }

        /* Lower and upper endpoints y coordinates */
        if (coordinateMask & 0x02)
        {
            /* Log coordinates are not taken into account yet */
            coord0 = 0.0;
            coord1 = coordinates[i + nPoints];

            if (yshift != NULL)
            {
                /*
                 * Only the upper vertex's y coordinate is shifted,
                 * the lower vertex's one remains unchanged.
                 */
                coord1 += yshift[i];
            }

            buffer[v0 +1] = coord0 * scale[1] + translation[1];
            buffer[v1 +1] = coord1 * scale[1] + translation[1];
        }

        /* Lower and upper endpoints z coordinates */
        if (coordinateMask & 0x04)
        {
            coord0 = coordinates[2*nPoints+i];
            coord1 = coordinates[2*nPoints+i];

            if (zshift != NULL)
            {
                coord0 += zshift[i];
                coord1 += zshift[i];
            }

            buffer[v0 +2] = coord0 * scale[2] + translation[2];
            buffer[v1 +2] = coord1 * scale[2] + translation[2];
        }

        if((elementsSize == 4) && (coordinateMask & 0x08))
        {
            buffer[v0 +3] = 1.0;
            buffer[v1 +3] = 1.0;
        }

    }

}

void PolylineDecomposer::writeBarVerticesToBuffer(float* buffer, int* offsets, int componentOffset, double* coordinates, double shift, int shiftUsed,
    double scale, double translation)
{
    if (shiftUsed)
    {
        coordinates[0] += shift;
        coordinates[1] += shift;
        coordinates[2] += shift;
        coordinates[3] += shift;

        coordinates[4] += shift;
    }

    buffer[offsets[0] +componentOffset] = coordinates[0] * scale + translation;
    buffer[offsets[1] +componentOffset] = coordinates[1] * scale + translation;
    buffer[offsets[2] +componentOffset] = coordinates[2] * scale + translation;
    buffer[offsets[3] +componentOffset] = coordinates[3] * scale + translation;

    buffer[offsets[4] +componentOffset] = coordinates[4] * scale + translation;
}

void PolylineDecomposer::fillMatlabBarDecompositionVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
    double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift)
{
    double barWidth = 0.0;
    double* pdBarWidth = &barWidth;

    int i;

    int shiftUsed[3];
    int *piShiftUsed;

    getGraphicObjectProperty(id, __GO_BAR_WIDTH__, jni_double, (void**) &pdBarWidth);

    piShiftUsed = &shiftUsed[0];
    getGraphicObjectProperty(id, __GO_DATA_MODEL_X_COORDINATES_SHIFT_SET__, jni_double_vector, (void**) &piShiftUsed);
    piShiftUsed = &shiftUsed[1];
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y_COORDINATES_SHIFT_SET__, jni_double_vector, (void**) &piShiftUsed);
    piShiftUsed = &shiftUsed[2];
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z_COORDINATES_SHIFT_SET__, jni_double_vector, (void**) &piShiftUsed);


    for (i = 0; i < nPoints; i++)
    {
        /*
         * Offsets of the lower-left, lower-right, upper-right and upper-left bar vertices (respectively)
         * and of the polyline vertex proper
         */
        int offsets[5];

        /* {x,y or z}-component values of a bar's 4 vertices and of the polyline vertex proper */
        double coords[5];

        double shift;

        offsets[0] = elementsSize*4*i;
        offsets[1] = elementsSize*(4*i+1);
        offsets[2] = elementsSize*(4*i+2);
        offsets[3] = elementsSize*(4*i+3);

        offsets[4] = elementsSize*(4*nPoints+i);

        if (coordinateMask & 0x01)
        {
            coords[0] = coordinates[i] - 0.5*barWidth;
            coords[1] = coordinates[i] + 0.5*barWidth;
            coords[2] = coordinates[i] + 0.5*barWidth;
            coords[3] = coordinates[i] - 0.5*barWidth;

            coords[4] = coordinates[i];

            if (shiftUsed[0])
            {
                shift = xshift[i];
            }

            writeBarVerticesToBuffer(buffer, offsets, 0, coords, shift, shiftUsed[0], scale[0], translation[0]);
        }

        if (coordinateMask & 0x02)
        {
            /* Log coordinates are not taken into account yet */
            coords[0] = 0.0;
            coords[1] = 0.0;
            coords[2] = coordinates[i + nPoints];
            coords[3] = coordinates[i + nPoints];

            coords[4] = coordinates[i + nPoints];

            if (shiftUsed[1])
            {
                shift = yshift[i];
            }

            writeBarVerticesToBuffer(buffer, offsets, 1, coords, shift, shiftUsed[1], scale[1], translation[1]);
        }

        if (coordinateMask & 0x04)
        {
            coords[0] = coordinates[i + 2 * nPoints];
            coords[1] = coordinates[i + 2 * nPoints];
            coords[2] = coordinates[i + 2 * nPoints];
            coords[3] = coordinates[i + 2 * nPoints];

            coords[4] = coordinates[i + 2* nPoints];

            if (shiftUsed[2])
            {
                shift = zshift[i];
            }

            writeBarVerticesToBuffer(buffer, offsets, 2, coords, shift, shiftUsed[2], scale[2], translation[2]);
        }

        if((elementsSize == 4) && (coordinateMask & 0x08))
        {
            buffer[offsets[0] +3] = 1.0;
            buffer[offsets[1] +3] = 1.0;
            buffer[offsets[2] +3] = 1.0;
            buffer[offsets[3] +3] = 1.0;

            buffer[offsets[4] +3] = 1.0;
        }

    }

}

int PolylineDecomposer::getIndicesSize(char* id)
{
    int nPoints = 0;
    int *piNPoints = &nPoints;
    int polylineStyle = 0;
    int* piPolylineStyle = &polylineStyle;

    int lineMode = 0;
    int* piLineMode = &lineMode;

    int closed = 0;
    int* piClosed = &closed;

    getGraphicObjectProperty(id, __GO_POLYLINE_STYLE__, jni_int, (void**) &piPolylineStyle);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**) &piNPoints);
    getGraphicObjectProperty(id, __GO_LINE_MODE__, jni_bool, (void**) &piLineMode);
    getGraphicObjectProperty(id, __GO_CLOSED__, jni_bool, (void**) &piClosed);


    /* No segments if 0 points */
    if (nPoints == 0)
    {
        return 0;
    }

    /* Segments */
    if (polylineStyle == 1)
    {
        /* To be modified */
        if (1 || lineMode)
        {
            if (closed)
            {
                return 2*(nPoints);
            }
            else
            {
                return 2*(nPoints-1);
            }
        }
        else
        {
            return 0;
        }
    }
    /* Staircase */
    else if (polylineStyle == 2)
    {
        /* To be modified */
        if (1 || lineMode)
        {
            if (closed)
            {
                return 4*(nPoints);
            }
            else
            {
                return 4*(nPoints-1);
            }
        }
        else
        {
            return 0;
        }
    }
    /* Vertical segments plus segments */
    else if (polylineStyle == 3)
    {
        if (lineMode)
        {
            return 2*(nPoints) + 2*(nPoints-1);
        }
        else
        {
            return 2*(nPoints);
        }
    }
    /* Arrowed segments */
    else if (polylineStyle == 4)
    {
        if (1 || lineMode)
        {
            if (closed)
            {
                return 2*(nPoints);
            }
            else
            {
                return 2*(nPoints-1);
            }
        }
        else
        {
            return 0;
        }
    }
    /* Vertical bars plus segments */
    else if (polylineStyle == 6)
    {
        if (lineMode)
        {
            return 2*4*(nPoints) + 2*(nPoints-1);
        }
        else
        {
            return 2*4*(nPoints);
        }
    }
    else
    {
        return 0;
    }

}

void PolylineDecomposer::fillIndices(char* id, int* buffer, int bufferLength)
{
    int polylineStyle = 0;
    int* piPolylineStyle = &polylineStyle;
    int nPoints = 0;
    int* piNPoints = &nPoints;
    int closed = 0;
    int* piClosed = &closed;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**) &piNPoints);
    getGraphicObjectProperty(id, __GO_POLYLINE_STYLE__, jni_int, (void**) &piPolylineStyle);
    getGraphicObjectProperty(id, __GO_CLOSED__, jni_bool, (void**) &piClosed);

    if (polylineStyle == 1)
    {
        fillLinearDecompositionIndices(id, buffer, bufferLength, nPoints, closed);
    }
    else if (polylineStyle == 2)
    {
        fillStairDecompositionIndices(id, buffer, bufferLength, nPoints, closed);
    }
    else if (polylineStyle == 3)
    {
        fillVerticalLinesDecompositionIndices(id, buffer, bufferLength, nPoints);
    }
    else if (polylineStyle == 4)
    {
        fillLinearDecompositionIndices(id, buffer, bufferLength, nPoints, closed);
    }
    else if (polylineStyle == 6)
    {
        fillMatlabBarDecompositionIndices(id, buffer, bufferLength, nPoints);
    }

}

void PolylineDecomposer::fillLinearDecompositionIndices(char* id, int* buffer, int bufferLength, int nPoints, int closed)
{
    int i;

    if (nPoints == 0)
    {
        return;
    }

    for (i = 0; i < nPoints-1; i++)
    {
        buffer[2*i] = i;
        buffer[2*i+1] = i+1;
    }

    if (closed)
    {
        buffer[2*i] = i;
        buffer[2*i+1] = 0;
    }

}

void PolylineDecomposer::fillStairDecompositionIndices(char* id, int* buffer, int bufferLength, int nPoints, int closed)
{
    int i;

    if (nPoints == 0)
    {
        return;
    }

    for (i = 0; i < nPoints-1; i++)
    {
        buffer[4*i] = 2*i;
        buffer[4*i+1] = 2*i+1;

        buffer[4*i+2] = 2*i+1;
        buffer[4*i+3] = 2*i+2;
    }

    /* One additional step */
    if (closed)
    {
        buffer[4*i] = 2*i;
        buffer[4*i+1] = 2*i+1;

        buffer[4*i+2] = 2*i+1;
        buffer[4*i+3] = 0;
    }

}

void PolylineDecomposer::fillVerticalLinesDecompositionIndices(char* id, int* buffer, int bufferLength, int nPoints)
{
    int i;
    int lineMode = 0;
    int* piLineMode = &lineMode;

    getGraphicObjectProperty(id, __GO_LINE_MODE__, jni_bool, (void**) &piLineMode);

    /* Vertical lines */
    for (i = 0; i < nPoints; i++)
    {
        buffer[2*i] = 2*i;
        buffer[2*i+1] = 2*i+1;
    }

    /* Lines */
    if (lineMode)
    {
        for (i = 0; i < nPoints-1; i++)
        {
            buffer[2*(nPoints+i)] = 2*i+1;
            buffer[2*(nPoints+i)+1] = 2*(i+1)+1;
        }
    }

}


void PolylineDecomposer::fillMatlabBarDecompositionIndices(char* id, int* buffer, int bufferLength, int nPoints)
{
    int i;
    int lineMode = 0;
    int* piLineMode = &lineMode;

    getGraphicObjectProperty(id, __GO_LINE_MODE__, jni_bool, (void**) &piLineMode);

    /* Bars */
    for (i = 0; i < nPoints; i++)
    {
        buffer[8*i] = 4*i;
        buffer[8*i+1] = 4*i+1;
        buffer[8*i+2] = 4*i+1;
        buffer[8*i+3] = 4*i+2;
        buffer[8*i+4] = 4*i+2;
        buffer[8*i+5] = 4*i+3;
        buffer[8*i+6] = 4*i+3;
        buffer[8*i+7] = 4*i;
    }

    /* Lines */
    if (lineMode)
    {
        for (i = 0; i < nPoints-1; i++)
        {
            buffer[8*nPoints+2*i] = 4*nPoints+i;
            buffer[8*nPoints+2*i+1] = 4*nPoints+i+1;
        }
    }

}

