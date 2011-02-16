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

#ifndef DATA_LOADER_HXX
#define DATA_LOADER_HXX

extern "C"
{

  /**
   * Return the number of data elements for the given object.
   * @param[in] The given object id.
   */
  int getDataSize(char* id);

  /**
   * Returns the number of data elements for a given polyline.
   * @param[in] The polyline's id.
   * @return The polyline's number of data elements.
   */
  int getPolylineDataSize(char* id);

  /**
   * Fill the given buffer with vertex data from the given object.
   * @param[in] the id of the given object.
   * @param[out] the buffer to fill.
   * @param[in] the buffer length in number of elements.
   * @param[in] the number of coordinate taken by one element in the buffer.
   * @param[in] this byte mask specify which coordinates are filled (1 for X, 2 for Y, 4 for Z).
   * @param[in] the transformation to apply to data.
   */
  void fillVertices(char* id, float* BUFF, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation);

  /**
   * Fill the given buffer with vertex data from the given polyline.
   * @param[in] the id of the given polyline.
   * @param[out] the buffer to fill.
   * @param[in] the buffer length in number of elements.
   * @param[in] the number of coordinate taken by one element in the buffer.
   * @param[in] this byte mask specify which coordinates are filled (1 for X, 2 for Y, 4 for Z).
   * @param[in] the transformation to apply to data.
   */
  void fillPolylineVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation);

  /**
   * Fills a buffer with vertex data from a polyline decomposed into consecutive segments.
   * @param[in] the id of the given polyline.
   * @param[out] the buffer to fill.
   * @param[in] the buffer length in number of elements.
   * @param[in] the number of coordinate taken by one element in the buffer.
   * @param[in] this byte mask specify which coordinates are filled (1 for X, 2 for Y, 4 for Z).
   * @param[in] the transformation to apply to data.
   * @param[in] the polyline coordinates.
   * @param[in] the polyline's number of points
   * @param[in] the polyline x-shift array.
   * @param[in] the polyline y-shift array.
   * @param[in] the polyline z-shift array.
   */
  void polylineLinearDecomposition(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
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
  void polylineGetAndWriteVertexToBuffer(float* buffer, int offset, double* coordinates, int* vertexIndices, int nPoints, int elementsSize,
      double* xshift, double* yshift, double* zshift, int coordinateMask, double* scale, double* translation);

  /**
   * Fills a buffer with vertex data from polyline using a staircase decomposition.
   * @param[in] the id of the given polyline.
   * @param[out] the buffer to fill.
   * @param[in] the buffer length in number of elements.
   * @param[in] the number of coordinate taken by one element in the buffer.
   * @param[in] this byte mask specify which coordinates are filled (1 for X, 2 for Y, 4 for Z).
   * @param[in] the transformation to apply to data.
   * @param[in] the polyline coordinates.
   * @param[in] the polyline's number of points
   * @param[in] the polyline x-shift array.
   * @param[in] the polyline y-shift array.
   * @param[in] the polyline z-shift array.
   */
  void polylineStairDecomposition(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
      double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift);

  /**
   * Fills a buffer with vertex data from polyline decomposed into a series of vertical lines.
   * @param[in] the id of the given polyline.
   * @param[out] the buffer to fill.
   * @param[in] the buffer length in number of elements.
   * @param[in] the number of coordinate taken by one element in the buffer.
   * @param[in] this byte mask specify which coordinates are filled (1 for X, 2 for Y, 4 for Z).
   * @param[in] the transformation to apply to data.
   * @param[in] the polyline coordinates.
   * @param[in] the polyline's number of points
   * @param[in] the polyline x-shift array.
   * @param[in] the polyline y-shift array.
   * @param[in] the polyline z-shift array.
   */
  void polylineVerticalLinesDecomposition(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
      double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift);

  /**
   * Fills a buffer with vertex data from polyline decomposed into a series of vertical bars.
   * @param[in] the id of the given polyline.
   * @param[out] the buffer to fill.
   * @param[in] the buffer length in number of elements.
   * @param[in] the number of coordinate taken by one element in the buffer.
   * @param[in] this byte mask specify which coordinates are filled (1 for X, 2 for Y, 4 for Z).
   * @param[in] the transformation to apply to data.
   * @param[in] the polyline coordinates.
   * @param[in] the polyline's number of points
   * @param[in] the polyline x-shift array.
   * @param[in] the polyline y-shift array.
   * @param[in] the polyline z-shift array.
   */
  void polylineMatlabBarDecomposition(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
      double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift);

  /**
   * Fill the given buffer with color data from the given object.
   * @param[in] the id of the given object.
   * @param[in] the buffer to fill.
   * @param[in] the buffer length in number of elements.
   * @param[in] the number of coordinate taken by one element in the buffer (3 for RGB, 4 for RGBA).
   */
  void fillColors(char* id, float* BUFF, int bufferLength, int elementsSize);

  /**
   * Return the number of indices for the given object.
   * @param[in] The given object id.
   * @return The object's number of indices.
   */
  int getIndicesSize(char* id);

  /**
   * Return the number of indices of a given polyline.
   * It is equal to twice the number of segments making up
   * the decomposed polyline.
   * @param[in] The given polyline id.
   * @return The polyline's number of indices.
   */
  int getPolylineIndicesSize(char* id);

  /**
   * Fill the given buffer with indices data of the given.
   * @param[in] the id of the given object.
   * @param[out] the buffer to fill.
   * @param[in] the buffer length.
   */
  void fillIndices(char* id, int* BUFF, int bufferLength);

  /**
   * Fill the given buffer with the indices of the given polyline.
   * @param[in] the polyline's id.
   * @param[out] the buffer to fill.
   * @param[in] the buffer length in number of elements.
   */
  void fillPolylineIndices(char* id, int* buffer, int bufferLength);

  /**
   * Fills a buffer with the indices of a linearly decomposed polyline.
   * @param[in] the id of the polyline.
   * @param[out] the buffer to fill.
   * @param[in] the buffer length in number of elements.
   * @param[in] the polyline's number of points.
   * @param[in] a flag indicating whether the polyline is closed or not.
   */
  void polylineLinearDecompositionIndices(char* id, int* buffer, int bufferLength, int nPoints, int closed);

  /**
   * Fills a buffer with the indices of polyline decomposed into a series of steps.
   * @param[in] the id of the polyline.
   * @param[out] the buffer to fill.
   * @param[in] the buffer length in number of elements.
   * @param[in] the polyline's number of points.
   * @param[in] a flag indicating whether the polyline is closed or not.
   */
  void polylineStairDecompositionIndices(char* id, int* buffer, int bufferLength, int nPoints, int closed);

  /**
   * Fills a buffer with the indices of a polyline decomposed into a series of vertical lines.
   * @param[in] the id of the polyline.
   * @param[out] the buffer to fill.
   * @param[in] the buffer length in number of elements.
   * @param[in] the polyline's number of points.
   */
  void polylineVerticalLinesDecompositionIndices(char* id, int* buffer, int bufferLength, int nPoints);

  /**
   * Fills a buffer with the indices of a polyline decomposed into a series of vertical bars.
   * @param[in] the id of the polyline.
   * @param[out] the buffer to fill.
   * @param[in] the buffer length in number of elements.
   * @param[in] the polyline's number of points.
   */
  void polylineMatlabBarDecompositionIndices(char* id, int* buffer, int bufferLength, int nPoints);

  /**
   * Return the number of indices for the wire of the given object.
   * @param[in] The given object id.
   */
  int getWireIndicesSize(char* id);

  /**
   * Fill the given buffer with wire indices data of the given.
   * @param[in] the id of the given object.
   * @param[out] the buffer to fill.
   * @param[in] the buffer length.
   */
  void fillWireIndices(char* id, int* BUFF, int bufferLength);
}

#endif DATA_LOADER_HXX
