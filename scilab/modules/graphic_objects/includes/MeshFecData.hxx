/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef MESH_FEC_DATA_HXX
#define MESH_FEC_DATA_HXX

#include "Data3D.hxx"
#include "MeshData.hxx"

extern "C" {
#include "BOOL.h"
}

/**
 * Fec triangle mesh data class
 */

class MeshFecData: public MeshData
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
    /**
     * Constructor
     */
    MeshFecData(void);

    /**
     * Constructor
     * To be implemented
     */
    MeshFecData(unsigned int numberVertices, unsigned int numberTriangles, unsigned int numberVerticesByElem = 3);

    /**
     * Destructor
     */
    virtual ~MeshFecData();

    /**
     * Returns the identifier associated to a property name
     * @param[in] propertyName the property name
     * @return the property identifier
     */
    int getPropertyFromName(int propertyName);

    /**
     * Sets a data property
     * @param[in] property the property identifier
     * @param[in] value a pointer to the property values
     * @param[in] numElements the number of elements to set
     * @return 1 if the property has been successfully set, 0 otherwise
     */
    int setDataProperty(int property, void const* value, int numElements);

    /**
     * Returns a data property
     * @param[in] property the property identifier
     * @param[out] a pointer to a pointer to the returned property values
     */
    void getDataProperty(int property, void **_pvData);

    /**
     * Returns the number of index triplets
     * @return the number of index triplets
     */
    unsigned int getNumIndices();

    /**
     * Sets the number of number of index triplets
     * Resizes the arrays of indices and fec triangle values if required
     * @param[in] numIndices the number of index triplets to set
     * @return 1 if the number of index triplets has been successfully set, 0 otherwise (failed allocation)
     */
    int setNumIndices(unsigned int numIndices);

    /**
     * Sets the array of fec triangle values
     * @param[in] a pointer to the array of fec triangles values
     * @param[in] numElements the number of triangles to set
     */
    void setFecElements(double const* data, int numElements);

    /**
     * Returns the array of fec triangle values
     * @return a pointer to the array of fec triangle values
     */
    double* getFecElements(void);
};

#endif
