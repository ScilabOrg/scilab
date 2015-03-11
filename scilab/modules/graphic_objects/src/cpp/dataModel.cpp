/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "DataModel.hxx"

extern "C" {
#include "BOOL.h"

#include <string.h>
}

DataModel *DataModel::m_me = NULL;

BOOL DataModel::setGraphicObjectProperty(int iUID, int _iName, void const* _dblValue, int numElements)
{
    Data3D* dataObject = NULL;
    int property = 0;
    int returnValue = 0;

    dataObject = (*m_dataMap)[iUID];
    if (dataObject == NULL)
    {
        return FALSE;
    }

    property = dataObject->getPropertyFromName(_iName);

    if (property == UNKNOWN_DATA_PROPERTY)
    {
        return FALSE;
    }

    returnValue = dataObject->setDataProperty(property, _dblValue, numElements);

    return (BOOL) returnValue;
}

void DataModel::getGraphicObjectProperty(int iUID, int _iName, void **_pvData)
{
    Data3D* dataObject = NULL;
    int property = 0;

    dataObject = (*m_dataMap)[iUID];

    if (dataObject == NULL)
    {
        *_pvData = NULL;
        return;
    }

    property = dataObject->getPropertyFromName(_iName);
    if (property == UNKNOWN_DATA_PROPERTY)
    {
        *_pvData = NULL;
        return;
    }

    dataObject->getDataProperty(property, _pvData);
}

int DataModel::createDataObject(int iUID, int _iType)
{
    Data3D* newObject = NULL;

    /*
     * To be implemented as the Java graphicObject class' similar function
     * (getTypeFromName) in order to avoid strcmp calls.
     */

    switch (_iType)
    {
        case __GO_GRAYPLOT__ :
            newObject = new NgonGridData();
            break;
        case __GO_MATPLOT__ :
            newObject = new NgonGridMatplotData();
            break;
        case __GO_FAC3D__ :
            newObject = new NgonGeneralData();
            break;
        case __GO_PLOT3D__ :
            newObject = new NgonGridData();
            break;
        case __GO_POLYLINE__ :
            newObject = new NgonPolylineData();
            break;
        case __GO_FEC__ :
            newObject = new MeshFecData();
            break;
        default :
            return 0;
    }

    (*m_dataMap)[iUID] = newObject;

    return iUID;
}

void DataModel::deleteDataObject(int iUID)
{
    std::map<int, Data3D*>::iterator it = m_dataMap->find(iUID);
    if (it != m_dataMap->end() && it->second != NULL)
    {
        delete it->second;
        m_dataMap->erase(it);
    }

    /*Data3D* newObject = (*m_dataMap)[std::string(_pstID)];
      if (newObject != NULL)
      {
      delete newObject;
      }

      m_dataMap->erase(std::string(_pstID));*/
}

