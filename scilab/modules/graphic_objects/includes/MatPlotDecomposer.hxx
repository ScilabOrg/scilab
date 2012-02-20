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

#ifndef MATPLOT_DECOMPOSER_HXX
#define MATPLOT_DECOMPOSER_HXX

class MatPlotDecomposer
{
private:
  static float VERTICES_DATA [];
  static float TEXTURE_COORDINATES_DATA [];
public:

  static int getTextureWidth(char* id);
  static int getTextureHeight(char* id);
  static int fillTextureData(char* id, float* buffer, int bufferLength);
  
  static int getDataSize(char* id);
  static void fillVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask);
  static int fillTextureCoordinates(char* id, float* buffer, int bufferLength);
  
  static int getIndicesSize(char* id);
  static int fillIndices(char* id, int* buffer, int bufferLength, int logMask);
};

#endif
