#include <iostream>
#include <map>
#include "DataLoader.hxx"

extern "C"
{
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"


/**
 * Return the number of data elements for the given object.
 *  - id : The id of the given object.
 */
int getDataSize(char* id)
{
    int iSize = 0;
    int *piSize = &iSize;
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**) &piSize);
    return iSize;
}


/**
 * Fill the given buffer with vertex data from the given object.
 *  - id : The id of the given object.
 *  - buffer : the buffer to fill.
 *  - elementSize : the size of one element in the buffer. 3 for (x, y, z) and 4 for (x, y, z, w) with w = 1.
 *  - coordinateMask : this byte mask specify witch coordinates are filled (0x1 for X, 0x2 for Y, 0x4 for Z, 0x8 for w).
 *  - scale, translation : the transformation to apply to data.
 */
void fillVertices(char* id, float* buffer, int elementsSize, char coordinateMask, double* scale, double* translation)
{
    int iSize = 0;
    double* t;
    getGraphicObjectProperty(id, __GO_DATA_MODEL_COORDINATES__, jni_double_vector, (void**) &t);
    int* piSize = &iSize;
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**) &piSize);

    // TODO Check object type.
    // TODO Optimize ? (test if s = 1 and t = 0, coordinateMask = 0 ...)
    for (int i = 0 ; i < iSize ; i++)
    {
      if (coordinateMask & 0x01) {
        buffer[elementsSize*i+0] = t[i] * scale[0] + translation[0];
      }
      
      if (coordinateMask & 0x02) {
        buffer[elementsSize*i+1] = t[i + iSize] * scale[1] + translation[1];
      }
      
      if (coordinateMask & 0x04) {
        buffer[elementsSize*i+2] = t[i + 2 * iSize] * scale[2] + translation[2];
      }
      
      if((elementsSize == 4) && (coordinateMask & 0x08)) {
        buffer[elementsSize*i+3] = 1.0;
      }
    }
}


/**
 * Return the number of indices for the given object.
 *  - id : The id of the given object.
 */
int getIndicesSize(char* id)
{
  // TODO.
}

void fillIndices(char* id, int* buffer)
{
  // TODO
}



} // extern "C"


