/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2012 - DIGITEO - Pierre Lando
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#include "DecompositionUtils.hxx"
#include "MatPlotDecomposer.hxx"
#include "DataProperties.hxx"
#include "ColorComputer.hxx"
#include <iostream>

extern "C" {
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

float MatPlotDecomposer::VERTICES_DATA [] = {
    0, 0, 0, 1,
    0, 1, 0, 1,
    1, 1, 0, 1,
    1, 0, 0, 1,
  };
  
float MatPlotDecomposer::TEXTURE_COORDINATES_DATA [] = {
    0, 1, 0, 1,
    0, 0, 0, 1,
    1, 0, 0, 1,
    1, 1, 0, 1,
  };

int MatPlotDecomposer::getTextureWidth(char* id)
{
  int width;
  int* piWidth = &width;
  getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piWidth);
  return width - 1;
}


int MatPlotDecomposer::getTextureHeight(char* id)
{
  int height;
  int* piHeight = &height;
  getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piHeight);
  return height - 1;
}


int MatPlotDecomposer::fillTextureData(char* id, float* buffer, int bufferLength)
{
  double* value;
  getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_double_vector, (void**) &value);
  int textureWidth = getTextureWidth(id);
  int textureHeight = getTextureHeight(id);
  int dataSize = textureWidth * textureHeight;
  if (dataSize * 4 == bufferLength)
  {
  
    char* parentFigure;
    double* colormap;
    int colormapSize;
    int* piColormapSize = &colormapSize;

    getGraphicObjectProperty(id, __GO_PARENT_FIGURE__, jni_string, (void**) &parentFigure);    
    getGraphicObjectProperty(parentFigure, __GO_COLORMAP__, jni_double_vector, (void**) &colormap);
    getGraphicObjectProperty(parentFigure, __GO_COLORMAP_SIZE__, jni_int, (void**) &piColormapSize);
  
    for (int i = 0 ; i < textureWidth ; i++)
    {
      for (int j = 0 ; j < textureHeight ; j++)
      {    
        ColorComputer::getDirectColor(value[j + i * textureHeight] - 1, colormap, colormapSize, &buffer[4 * (i + j * textureWidth)]);
        buffer[4 * (i + j * textureWidth) + 3] = 1;
      }
    }
    
    return bufferLength;
  }
  else
  {
    return 0;
  }
}


int MatPlotDecomposer::getDataSize(char* id)
{
  return 4;
}


void MatPlotDecomposer::fillVertices(
  char* id, float* buffer, int bufferLength,
  int elementsSize, int coordinateMask, double* scale,
  double* translation, int logMask)
{
  int xSize = getTextureWidth(id);
  int ySize = getTextureHeight(id);
  for (int i = 0 ; i < 16 ; i+=4) {
    buffer[i + 0] = VERTICES_DATA[i + 0] * xSize * scale[0] + translation[0] + 1/2.;
    buffer[i + 1] = VERTICES_DATA[i + 1] * ySize * scale[1] + translation[1] + 1/2.;
    buffer[i + 2] = VERTICES_DATA[i + 2] * scale[2] + translation[2]; 
    buffer[i + 3] = VERTICES_DATA[i + 3]; 
  }
}

int MatPlotDecomposer::fillTextureCoordinates(char* id, float* buffer, int bufferLength)
{
  int xSize = getTextureWidth(id);
  int ySize = getTextureHeight(id);
  for (int i = 0 ; i < 16 ; i+=4) {
    buffer[i + 0] = TEXTURE_COORDINATES_DATA[i + 0];
    buffer[i + 1] = TEXTURE_COORDINATES_DATA[i + 1];
    buffer[i + 2] = TEXTURE_COORDINATES_DATA[i + 2]; 
    buffer[i + 3] = TEXTURE_COORDINATES_DATA[i + 3]; 
  }
  return 16;
}


int MatPlotDecomposer::getIndicesSize(char* id)
{
  return 6;
}


int MatPlotDecomposer::fillIndices(char* id, int* buffer, int bufferLength, int logMask)
{
  buffer[0] = 0;
  buffer[1] = 1;
  buffer[2] = 2;
  
  buffer[3] = 0;
  buffer[4] = 2;
  buffer[5] = 3;
  
  return 6;
}

