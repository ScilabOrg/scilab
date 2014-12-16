/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <string>
#include <vector>

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

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, double v)
{
    std::shared_ptr<model::BaseObject> baseObject = getObject(uid);
    if (baseObject == nullptr)
    {
        return FAIL;
    }

    if (k == ANNOTATION)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = std::static_pointer_cast<model::Port>(baseObject).get();
        switch (p)
        {
            case FIRING:
                return o->setFiring(v);
            default:
                break;
        }
    }
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, int v)
{
    std::shared_ptr<model::BaseObject> baseObject = getObject(uid);
    if (baseObject == nullptr)
    {
        return FAIL;
    }

    if (k == ANNOTATION)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = std::static_pointer_cast<model::Block>(baseObject).get();
        switch (p)
        {
            case SIM_FUNCTION_API:
                return o->setSimFunctionApi(v);
            case NZCROSS:
                return o->setNZcross(v);
            case NMODE:
                return o->setNMode(v);
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = std::static_pointer_cast<model::Link>(baseObject).get();
        switch (p)
        {
            case COLOR:
                return o->setColor(v);
            case KIND:
                return o->setKind(v);
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = std::static_pointer_cast<model::Port>(baseObject).get();
        switch (p)
        {
            case PORT_KIND:
                return o->setKind(v);
            default:
                break;
        }
    }
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, bool v)
{
    std::shared_ptr<model::BaseObject> baseObject = getObject(uid);
    if (baseObject == nullptr)
    {
        return FAIL;
    }

    if (k == ANNOTATION)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = std::static_pointer_cast<model::Port>(baseObject).get();
        switch (p)
        {
            case IMPLICIT:
                return o->setImplicit(v);
            default:
                break;
        }
    }
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, ScicosID v)
{
    std::shared_ptr<model::BaseObject> baseObject = getObject(uid);
    if (baseObject == nullptr)
    {
        return FAIL;
    }

    if (k == ANNOTATION)
    {
        model::Annotation* o = std::static_pointer_cast<model::Annotation>(baseObject).get();;
        switch (p)
        {
            case PARENT_DIAGRAM:
                return o->setParentDiagram(v);
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = std::static_pointer_cast<model::Block>(baseObject).get();
        switch (p)
        {
            case PARENT_DIAGRAM:
                return o->setParentDiagram(v);
            case PARENT_BLOCK:
                return o->setParentBlock(v);
            case PORT_REFERENCE:
                return o->setPortReference(v);
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = std::static_pointer_cast<model::Link>(baseObject).get();
        switch (p)
        {
            case PARENT_DIAGRAM:
                return o->setParentDiagram(v);
            case SOURCE_PORT:
                return o->setSourcePort(v);
            case DESTINATION_PORT:
                return o->setDestinationPort(v);
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = std::static_pointer_cast<model::Port>(baseObject).get();
        switch (p)
        {
            case SOURCE_BLOCK:
                return o->setSourceBlock(v);
            case CONNECTED_SIGNALS:
                return o->setConnectedSignals(std::vector<ScicosID> (1, v));
            default:
                break;
        }
    }
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::string v)
{
    std::shared_ptr<model::BaseObject> baseObject = getObject(uid);
    if (baseObject == nullptr)
    {
        return FAIL;
    }

    if (k == ANNOTATION)
    {
        model::Annotation* o = std::static_pointer_cast<model::Annotation>(baseObject).get();;
        switch (p)
        {
            case DESCRIPTION:
                return o->setDescription(v);
            case FONT:
                return o->setFont(v);
            case FONT_SIZE:
                return o->setFontSize(v);
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = std::static_pointer_cast<model::Block>(baseObject).get();
        switch (p)
        {
            case INTERFACE_FUNCTION:
                return o->setInterfaceFunction(v);
            case SIM_FUNCTION_NAME:
                return o->setSimFunctionName(v);
            case SIM_BLOCKTYPE:
                return o->setSimBlocktype(v);
            case STYLE:
                return o->setStyle(v);
            case LABEL:
                return o->setLabel(v);
            case UID:
                return o->setUID(v);
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = std::static_pointer_cast<model::Diagram>(baseObject).get();
        switch (p)
        {
            case TITLE:
                return o->setTitle(v);
            case PATH:
                return o->setPath(v);
            case VERSION_NUMBER:
                return o->setVersionNumber(v);
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = std::static_pointer_cast<model::Link>(baseObject).get();
        switch (p)
        {
            case LABEL:
                return o->setLabel(v);
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = std::static_pointer_cast<model::Port>(baseObject).get();
        switch (p)
        {
            case STYLE:
                return o->setStyle(v);
            case LABEL:
                return o->setLabel(v);
            default:
                break;
        }
    }
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<double>& v)
{
    std::shared_ptr<model::BaseObject> baseObject = getObject(uid);
    if (baseObject == nullptr)
    {
        return FAIL;
    }

    if (k == ANNOTATION)
    {
        model::Annotation* o = std::static_pointer_cast<model::Annotation>(baseObject).get();;
        switch (p)
        {
            case GEOMETRY:
                return o->setGeometry(v);
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = std::static_pointer_cast<model::Block>(baseObject).get();
        switch (p)
        {
            case GEOMETRY:
                return o->setGeometry(v);
            case ANGLE:
                return o->setAngle(v);
            case STATE:
                return o->setState(v);
            case DSTATE:
                return o->setDState(v);
            case RPAR:
                return o->setRpar(v);
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = std::static_pointer_cast<model::Diagram>(baseObject).get();
        switch (p)
        {
            case PROPERTIES:
                return o->setProperties(v);
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        model::Link* o = std::static_pointer_cast<model::Link>(baseObject).get();
        switch (p)
        {
            case CONTROL_POINTS:
                return o->setControlPoints(v);
            case THICK:
                return o->setThick(v);
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<int>& v)
{
    std::shared_ptr<model::BaseObject> baseObject = getObject(uid);
    if (baseObject == nullptr)
    {
        return FAIL;
    }

    if (k == ANNOTATION)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = std::static_pointer_cast<model::Block>(baseObject).get();
        switch (p)
        {
            case SIM_DEP_UT:
                return o->setSimDepUT(v);
            case ODSTATE:
                return o->setODState(v);
            case IPAR:
                return o->setIpar(v);
            case OPAR:
                return o->setOpar(v);
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = std::static_pointer_cast<model::Port>(baseObject).get();
        switch (p)
        {
            case DATATYPE:
                return o->setDataType(this, v);
            default:
                break;
        }
    }
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<std::string>& v)
{
    std::shared_ptr<model::BaseObject> baseObject = getObject(uid);
    if (baseObject == nullptr)
    {
        return FAIL;
    }

    if (k == ANNOTATION)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = std::static_pointer_cast<model::Block>(baseObject).get();
        switch (p)
        {
            case EXPRS:
                return o->setExprs(v);
            case EQUATIONS:
                return o->setEquations(v);
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = std::static_pointer_cast<model::Diagram>(baseObject).get();
        switch (p)
        {
            case DIAGRAM_CONTEXT:
                return o->setContext(v);
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    return FAIL;
}

update_status_t Model::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<ScicosID>& v)
{
    std::shared_ptr<model::BaseObject> baseObject = getObject(uid);
    if (baseObject == nullptr)
    {
        return FAIL;
    }

    if (k == ANNOTATION)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == BLOCK)
    {
        model::Block* o = std::static_pointer_cast<model::Block>(baseObject).get();
        switch (p)
        {
            case INPUTS:
                return o->setIn(v);
            case OUTPUTS:
                return o->setOut(v);
            case EVENT_INPUTS:
                return o->setEin(v);
            case EVENT_OUTPUTS:
                return o->setEout(v);
            case CHILDREN:
                return o->setChildren(v);
            default:
                break;
        }
    }
    else if (k == DIAGRAM)
    {
        model::Diagram* o = std::static_pointer_cast<model::Diagram>(baseObject).get();
        switch (p)
        {
            case CHILDREN:
                return o->setChildren(v);
            default:
                break;
        }
    }
    else if (k == LINK)
    {
        switch (p)
        {
            default:
                break;
        }
    }
    else if (k == PORT)
    {
        model::Port* o = std::static_pointer_cast<model::Port>(baseObject).get();
        switch (p)
        {
            case CONNECTED_SIGNALS:
                return o->setConnectedSignals(v);
            default:
                break;
        }
    }
    return FAIL;
}

} /* namespace org_scilab_modules_scicos */
