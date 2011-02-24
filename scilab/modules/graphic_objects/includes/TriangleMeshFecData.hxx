/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef TRIANGLE_MESH_FEC_DATA_H
#define TRIANGLE_MESH_FEC_DATA_H

#include "Data3D.hxx"
#include "TriangleMeshData.hxx"

extern "C" {
#include "BOOL.h"
}

/**
 * Fec triangle mesh data class
 */

class TriangleMeshFecData: public TriangleMeshData
{

private:

    /**
     * Fec-specific triangle values array
     * Each fec triangle is a 5-tuple (number, v0, v1, v2, flag), the numTriangles
     * values being stored contiguously for each element (numTriangles number values,
     * numTriangles v0 values, etc.)
     * The v0, v1, v2 values are duplicate with the parent class' indices.
     */
    double* fecValues;

public:
    TriangleMeshFecData(void);

    /* To be implemented */
    TriangleMeshFecData(unsigned int numberVertices, unsigned int numberTriangles);

    virtual ~TriangleMeshFecData();

    /**
     * Returns the property value (identifier) associated to a name
     * @return the property identifier
     */
    int getPropertyFromName(char* propertyName);

    /**
     * Sets a data property
     * @param property the property identifier
     * @param value the property values
     * @param numElements the number of elements to set
     * @return 1 if the property has correctly been set, 0 otherwise
     */
    int setDataProperty(int property, void* value, int numElements);

    /**
     * Returns a data property
     * @param property the property identifier
     * @return a pointer to the data property
     */
    void getDataProperty(int property, void **_pvData);

    /**
     * Returns the number of index triplets (number of triangles)
     * @return the number of index triplets
     */
    unsigned int getNumIndices();

    /**
     * Sets the number of number of index triplets (number of triangles)
     * Resizes the arrays of indices and fec triangle values if required
     * @param numIndices the number of index triplets to set
     * @return 1 if the number of index triplets has been correctly set, 0 otherwise (failed allocation)
     */
    int setNumIndices(unsigned int numIndices);

    /**
     * Sets the array of fec triangle values
     * @param the array of fec triangles values
     * @param numElements the number of triangles to set
     */
    void setFecTriangles(double* data, int numElements);

    /**
     * Returns the array of fec triangle values
     * @return the array of fec triangle values
     */
    double* getFecTriangles(void);
};

#endif
