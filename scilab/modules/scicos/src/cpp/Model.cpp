/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "Model.hxx"
#include "utilities.hxx"

#include "model/BaseObject.hxx"
#include "model/Annotation.hxx"
#include "model/Diagram.hxx"
#include "model/Block.hxx"
#include "model/Link.hxx"
#include "model/Port.hxx"

namespace org_scilab_modules_scicos
{

Model::Model() :
    lastId(0)
{
}

Model::~Model()
{
}

ScicosID Model::createObject(kind_t k)
{
    /*
     * Allocate the object per kind
     */
    model::BaseObject* o;
    switch (k)
    {
        case ANNOTATION:
            o = new model::Annotation();
            break;
        case DIAGRAM:
            o = new model::Diagram();
            break;
        case BLOCK:
            o = new model::Block();
            break;
        case LINK:
            o = new model::Link();
            break;
        case PORT:
            o = new model::Port();
            break;
    }

    /*
     * Found the next unused id
     */
    lastId++;
    objects_map_t::iterator iter = allObjects.lower_bound(lastId);
    while (iter != allObjects.end() && !(lastId < iter->first))
    {
        // while key is found
        lastId++;
        iter = allObjects.lower_bound(lastId);
    }

    /*
     * Insert then return
     */
    allObjects.insert(iter, std::make_pair(lastId, o));
    o->id(lastId);
    return lastId;
}

void Model::deleteObject(ScicosID uid)
{
    objects_map_t::iterator iter = allObjects.lower_bound(uid);
    if (iter == allObjects.end() || uid < iter->first)
    {
        throw "key has not been found";
    }

    allObjects.erase(iter);
    delete iter->second;
}

model::BaseObject* Model::getObject(ScicosID uid) const
{
    objects_map_t::const_iterator iter = allObjects.lower_bound(uid);
    if (iter == allObjects.end() || uid < iter->first)
    {
        throw "key has not been found";
    }

    return iter->second;
}

update_status_t Model::setObject(model::BaseObject* o)
{
    objects_map_t::iterator iter = allObjects.lower_bound(o->id());
    if (iter == allObjects.end() || o->id() < iter->first)
    {
        throw "key has not been found";
    }

    if (*iter->second == *o)
    {
        return NO_CHANGES;
    }

    o->id(iter->second->id());
    delete iter->second;
    iter->second = o;
    return SUCCESS;
}

bool Model::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, double* v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    return false;
}

bool Model::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, int* v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    return false;
}

bool Model::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, bool* v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    return false;
}

bool Model::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::string* v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    return false;
}

bool Model::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, ScicosID* v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            case PARENT_DIAGRAM:
                *v = o->getParentDiagram();
                return true;
            case RELATED_TO:
                *v = o->getRelatedTo();
                return true;
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            case PARENT_DIAGRAM:
                *v = o->getParentDiagram();
                return true;
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));

    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            case PARENT_DIAGRAM:
                *v = o->getParentDiagram();
                return true;
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
    }
    else
    {
    }
    return false;
}

bool Model::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, size_t* len,
                              double** v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            case GEOMETRY:
                o->getGeometry(len, v);
                return true;
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            case GEOMETRY:
                o->getGeometry(len, v);
                return true;
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
    }
    else
    {
    }
    return false;
}

bool Model::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, size_t* len, int** v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    return false;
}

bool Model::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, size_t* len,
                              std::string** v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    return false;
}

bool Model::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, size_t* len,
                              ScicosID** v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    return false;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, double v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, int v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, bool v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p,
        ScicosID v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p,
        std::string v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, size_t len,
        double* v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            case GEOMETRY:
                return o->setGeometry(len, v);
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            case GEOMETRY:
                return o->setGeometry(len, v);
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, size_t len,
        int* v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, size_t len,
        std::string* v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, size_t len,
        ScicosID* v)
{

    if (k == ANNOTATION)
    {
        model::Annotation* o = static_cast<model::Annotation*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = static_cast<model::Block*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = static_cast<model::Diagram*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = static_cast<model::Link*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = static_cast<model::Port*>(getObject(uid));
        switch (p)
        {
            default:
                break;
        }
    }
    return FAIL;
}

} /* namespace org_scilab_modules_scicos */
