/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2012 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ColorComputer.hxx"
#include  "DecompositionUtils.hxx"

extern "C"
{
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

void ColorComputer::getColor(double s, double smin, double srange, double indexOffset, double* colormap, int colormapSize, float* returnedColor)
{
    double value;
    int index;

    if (!DecompositionUtils::isANumber(s))
    {
        /* Black is output if s is a Nan */
        returnedColor[0] = MIN_COMPONENT_VALUE;
        returnedColor[1] = MIN_COMPONENT_VALUE;
        returnedColor[2] = MIN_COMPONENT_VALUE;
    }
    else
    {
        value = (s - smin) / (srange);
        index = (int) ((double)(colormapSize-1)*value + indexOffset);

        /* Clamp */
        if (index < 0)
        {
            index = 0;
        }
        else if (index > colormapSize - 1)
        {
            index = colormapSize - 1;
        }

        returnedColor[0] = (float)colormap[index];
        returnedColor[1] = (float)colormap[colormapSize+index];
        returnedColor[2] = (float)colormap[2*colormapSize+index];
    }
}

void ColorComputer::getColor(double s, double smin, double srange, double indexOffset, double* colormap, int minIndex, int maxIndex, int colormapSize, float* returnedColor)
{
    double value;
    int index;

    if (!DecompositionUtils::isANumber(s))
    {
        /* Black is output if s is a Nan */
        returnedColor[0] = MIN_COMPONENT_VALUE;
        returnedColor[1] = MIN_COMPONENT_VALUE;
        returnedColor[2] = MIN_COMPONENT_VALUE;
    }
    else
    {
        value = (s - smin) / (srange);
        index = (int) ((double)(maxIndex - minIndex)*value + indexOffset + (double) minIndex);

        /* Clamp */
        if (index < minIndex)
        {
            index = minIndex;
        }
        else if (index > maxIndex)
        {
            index = maxIndex;
        }

        returnedColor[0] = (float)colormap[index];
        returnedColor[1] = (float)colormap[colormapSize+index];
        returnedColor[2] = (float)colormap[2*colormapSize+index];
    }
}

double ColorComputer::getIndex(double s, double smin, double srange, double indexOffset, int minIndex, int maxIndex)
{
    double value;
    double index;

    if (!DecompositionUtils::isANumber(s))
    {
        /*
         * The minimum possible value is output if s is a Nan.
         * It should be an index corresponding to black.
         */
        index = MIN_COMPONENT_VALUE;
    }
    else
    {
        value = (s - smin) / (srange);
        index = (double)(maxIndex - minIndex)*value + indexOffset + (double) minIndex;

        /* Clamp */
        if (index < (double) minIndex)
        {
            index = (double) minIndex;
        }
        else if (index > (double) maxIndex)
        {
            index = (double) maxIndex;
        }
    }

    return index;
}

void ColorComputer::getDirectColor(double s, double* colormap, int colormapSize, float* returnedColor)
{
    int index;

    if (s <= (double) BLACK_LOWER_INDEX)
    {
        /* Clamp to white */
        returnedColor[0] = MAX_COMPONENT_VALUE;
        returnedColor[1] = MAX_COMPONENT_VALUE;
        returnedColor[2] = MAX_COMPONENT_VALUE;
    }
    else if ((((double) BLACK_LOWER_INDEX < s) && (s < (double) BLACK_UPPER_INDEX)) || !DecompositionUtils::isANumber(s))
    {
        /* Black is also output for Nan values */
        returnedColor[0] = MIN_COMPONENT_VALUE;
        returnedColor[1] = MIN_COMPONENT_VALUE;
        returnedColor[2] = MIN_COMPONENT_VALUE;
    }
    else
    {
        if (s > (double)(colormapSize - 1))
        {
            s = (double) (colormapSize - 1);
        }

        index = (int) s;

        returnedColor[0] = (float)colormap[index];
        returnedColor[1] = (float)colormap[colormapSize+index];
        returnedColor[2] = (float)colormap[2*colormapSize+index];
    }
}

double ColorComputer::getClampedDirectIndex(double s, int colormapSize)
{
    double index = s;

    if (!DecompositionUtils::isANumber(s))
    {
        /* Black is output if s is a Nan */
        index = 0.0; // TODO add black and white in texture.
    }
    else
    {
        /* Clamp */
        if (s < 0.0)
        {
            index = 0.0;
        }
        else if (s > (double)(colormapSize - 1))
        {
            index = (double) (colormapSize - 1);
        }
    }

    return index;
}

void ColorComputer::getClampedDirectColor(double s, double* colormap, int colormapSize, float* returnedColor)
{
    int index;

    if (!DecompositionUtils::isANumber(s))
    {
        /* Black is output if s is a Nan */
        returnedColor[0] = MIN_COMPONENT_VALUE;
        returnedColor[1] = MIN_COMPONENT_VALUE;
        returnedColor[2] = MIN_COMPONENT_VALUE;
    }
    else
    {
        /* Clamp */
        if (s < 0.0)
        {
            s = 0.0;
        }
        else if (s > (double)(colormapSize - 1))
        {
            s = (double) (colormapSize - 1);
        }

        index = (int) s;

        returnedColor[0] = (float)colormap[index];
        returnedColor[1] = (float)colormap[colormapSize+index];
        returnedColor[2] = (float)colormap[2*colormapSize+index];
    }
}

