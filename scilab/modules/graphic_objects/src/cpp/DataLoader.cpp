/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Pierre Lando
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <iostream>
#include <map>
#include "DataLoader.hxx"

extern "C"
{
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"


int getDataSize(char* id)
{
    int iSize = 0;
    int *piSize = &iSize;
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**) &piSize);
    return iSize;
}



void fillVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation)
{    
    double* t;
    getGraphicObjectProperty(id, __GO_DATA_MODEL_COORDINATES__, jni_double_vector, (void**) &t);

    // TODO Check object type.
    // TODO Optimize ? (test if s = 1 and t = 0, coordinateMask = 0 ...)
    for (int i = 0 ; i < bufferLength ; i++)
    {
      if (coordinateMask & 0x01) {
        buffer[elementsSize*i+0] = t[i] * scale[0] + translation[0];
      }
      
      if (coordinateMask & 0x02) {
        buffer[elementsSize*i+1] = t[i + bufferLength] * scale[1] + translation[1];
      }
      
      if (coordinateMask & 0x04) {
        buffer[elementsSize*i+2] = t[i + 2 * bufferLength] * scale[2] + translation[2];
      }
      
      if((elementsSize == 4) && (coordinateMask & 0x08)) {
        buffer[elementsSize*i+3] = 1.0;
      }
    }
}

void fillColors(char* id, float* BUFF, int bufferLength, int elementsSize)
{
  // TODO.
}


int getIndicesSize(char* id)
{
  // TODO.
}

void fillIndices(char* id, int* buffer, int bufferLength)
{
  // TODO.
}

int getWireIndicesSize(char* id)
{
  // TODO.
}
  
void fillWireIndices(char* id, int* BUFF, int bufferLength)
{
  // TODO.
}

} // extern "C"


