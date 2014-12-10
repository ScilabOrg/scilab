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
#include <iterator>
#include <algorithm>
#include <memory>

#include "internal.hxx"
#include "list.hxx"
#include "types.hxx"
#include "user.hxx"
#include "double.hxx"

#include "Controller.hxx"
#include "LinkAdapter.hxx"
#include "model/Link.hxx"
#include "model/Port.hxx"

extern "C" {
#include "sci_malloc.h"
#include "charEncoding.h"
}

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
namespace
{

const std::string split ("split");
const std::string lsplit ("lsplit");
const std::string limpsplit ("limpsplit");

struct xx
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<double> controlPoints;
        controller.getObjectProperty(adaptee, LINK, CONTROL_POINTS, controlPoints);

        double* data;
        int size = (int)controlPoints.size() / 2;
        types::Double* o = new types::Double(size, 1, &data);

#ifdef _MSC_VER
        std::copy(controlPoints.begin(), controlPoints.begin() + size, stdext::checked_array_iterator<double*>(data, size));
#else
        std::copy(controlPoints.begin(), controlPoints.begin() + size, data);
#endif
        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();

        std::vector<double> controlPoints;
        controller.getObjectProperty(adaptee, LINK, CONTROL_POINTS, controlPoints);

        int newXSize = current->getSize();
        int oldXSize = static_cast<int>(controlPoints.size() / 2);
        std::vector<double> newControlPoints (controlPoints);

        if (newXSize == oldXSize)
        {
            std::copy(current->getReal(), current->getReal() + newXSize, newControlPoints.begin());
        }
        else
        {
            newControlPoints.resize(2 * current->getSize(), 0);

            std::copy(current->getReal(), current->getReal() + newXSize, newControlPoints.begin());
            std::copy(controlPoints.begin() + oldXSize, controlPoints.begin() + oldXSize + std::min(newXSize, oldXSize), newControlPoints.begin() + newXSize);
        }

        controller.setObjectProperty(adaptee, LINK, CONTROL_POINTS, newControlPoints);
        return true;
    }
};

struct yy
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<double> controlPoints;
        controller.getObjectProperty(adaptee, LINK, CONTROL_POINTS, controlPoints);

        double* data;
        int size = (int)controlPoints.size() / 2;
        types::Double* o = new types::Double(size, 1, &data);

#ifdef _MSC_VER
        std::copy(controlPoints.begin() + size, controlPoints.end(), stdext::checked_array_iterator<double*>(data, size));
#else
        std::copy(controlPoints.begin() + size, controlPoints.end(), data);
#endif
        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();

        std::vector<double> controlPoints;
        controller.getObjectProperty(adaptee, LINK, CONTROL_POINTS, controlPoints);

        int newYSize = current->getSize();
        int oldYSize = static_cast<int>(controlPoints.size() / 2);
        std::vector<double> newControlPoints (controlPoints);

        if (newYSize == oldYSize)
        {
            std::copy(current->getReal(), current->getReal() + newYSize, newControlPoints.begin() + newYSize);
        }
        else
        {
            newControlPoints.resize(2 * current->getSize());

            std::copy(current->getReal(), current->getReal() + newYSize, newControlPoints.begin() + newYSize);
            if (newYSize > oldYSize)
            {
                std::fill(newControlPoints.begin() + oldYSize, newControlPoints.begin() + oldYSize + newYSize, 0);
            }
        }

        controller.setObjectProperty(adaptee, LINK, CONTROL_POINTS, newControlPoints);
        return true;
    }
};

struct id
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::string id;
        controller.getObjectProperty(adaptee, LINK, LABEL, id);

        types::String* o = new types::String(1, 1);
        o->set(0, id.data());

        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabString)
        {
            return false;
        }

        types::String* current = v->getAs<types::String>();
        if (current->getSize() != 1)
        {
            return false;
        }

        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::string id;
        char* c_str = wide_string_to_UTF8(current->get(0));
        id = std::string(c_str);
        FREE(c_str);

        controller.setObjectProperty(adaptee, LINK, LABEL, id);
        return true;
    }
};

struct thick
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        std::vector<double> thick;
        controller.getObjectProperty(adaptee, LINK, THICK, thick);

        double* data;
        types::Double* o = new types::Double(1, 2, &data);

        data[0] = thick[0];
        data[1] = thick[1];
        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getRows() != 1 || current->getCols() != 2)
        {
            return false;
        }

        std::vector<double> thick (2);
        thick[0] = current->get(0);
        thick[1] = current->get(1);

        controller.setObjectProperty(adaptee, LINK, THICK, thick);
        return true;
    }
};

struct ct
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        int color;
        int kind;
        controller.getObjectProperty(adaptee, LINK, COLOR, color);
        controller.getObjectProperty(adaptee, LINK, KIND, kind);

        double* data;
        types::Double* o = new types::Double(1, 2, &data);

        data[0] = static_cast<double>(color);
        data[1] = static_cast<double>(kind);
        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ScicosID adaptee = adaptor.getAdaptee()->id();

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getRows() != 1 || current->getCols() != 2)
        {
            return false;
        }
        if (floor(current->get(0)) != current->get(0) || floor(current->get(1)) != current->get(1))
        {
            return false;
        }

        int color = static_cast<int>(current->get(0));
        int kind  = static_cast<int>(current->get(1));

        controller.setObjectProperty(adaptee, LINK, COLOR, color);
        controller.setObjectProperty(adaptee, LINK, KIND, kind);
        return true;
    }
};

types::Double* getLinkEnd(const LinkAdapter& adaptor, const Controller& controller, const object_properties_t end)
{
    ScicosID adaptee = adaptor.getAdaptee()->id();

    double* data;
    types::Double* o = new types::Double(1, 3, &data);
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;

    ScicosID endID;
    controller.getObjectProperty(adaptee, LINK, end, endID);
    if (endID != 0)
    {
        ScicosID sourceBlock;
        controller.getObjectProperty(endID, PORT, SOURCE_BLOCK, sourceBlock);

        // Looking for the block number among the block IDs
        ScicosID parentDiagram;
        controller.getObjectProperty(adaptee, BLOCK, PARENT_DIAGRAM, parentDiagram);
        std::vector<ScicosID> children;
        if (parentDiagram == 0)
        {
            return o;
        }
        controller.getObjectProperty(parentDiagram, DIAGRAM, CHILDREN, children);
        data[0] = static_cast<double>(std::distance(children.begin(), std::find(children.begin(), children.end(), sourceBlock)) + 1);

        // To find the port index from its 'endID' ID, search through all the block's ports lists
        std::vector<ScicosID> sourceBlockPorts;
        controller.getObjectProperty(sourceBlock, BLOCK, INPUTS, sourceBlockPorts);

        std::vector<ScicosID>::iterator found = std::find(sourceBlockPorts.begin(), sourceBlockPorts.end(), endID);
        if (found == sourceBlockPorts.end()) // Not found in the data inputs
        {
            sourceBlockPorts.clear();
            controller.getObjectProperty(sourceBlock, BLOCK, OUTPUTS, sourceBlockPorts);
            found = std::find(sourceBlockPorts.begin(), sourceBlockPorts.end(), endID);
            if (found == sourceBlockPorts.end()) // Not found in the data outputs
            {
                sourceBlockPorts.clear();
                controller.getObjectProperty(sourceBlock, BLOCK, EVENT_INPUTS, sourceBlockPorts);
                found = std::find(sourceBlockPorts.begin(), sourceBlockPorts.end(), endID);
                if (found == sourceBlockPorts.end()) // Not found in the event inputs
                {
                    sourceBlockPorts.clear();
                    controller.getObjectProperty(sourceBlock, BLOCK, EVENT_OUTPUTS, sourceBlockPorts);
                    found = std::find(sourceBlockPorts.begin(), sourceBlockPorts.end(), endID);
                    if (found == sourceBlockPorts.end()) // Not found in the event outputs
                    {
                        return 0;
                    }
                }
            }
        }
        data[1] = static_cast<double>(std::distance(sourceBlockPorts.begin(), found) + 1);

        bool isImplicit;
        controller.getObjectProperty(endID, PORT, IMPLICIT, isImplicit);

        if (isImplicit == false)
        {
            int kind;
            controller.getObjectProperty(endID, PORT, PORT_KIND, kind);
            if (kind == model::PORT_IN || kind == model::PORT_EIN)
            {
                data[2] = 1;
            }
        }
    }
    // Default case, the property was initialized at [].
    return o;
}

/*
 * Connectivity is ensured if 'port' is of the desired type or if either of the concerned blocks is a split block,
 * because they are connectable to anything
 */
bool checkConnectivity(const int neededType, const ScicosID port, const ScicosID blk1, Controller& controller)
{
    int portKind;
    controller.getObjectProperty(port, PORT, PORT_KIND, portKind);

    if (portKind != neededType)
    {
        // Last chance if one of the connecting blocks is just a split block
        std::string name1;
        controller.getObjectProperty(blk1, BLOCK, SIM_FUNCTION_NAME, name1);
        if (name1 != split && name1 != lsplit && name1 != limpsplit)
        {
            ScicosID blk2;
            controller.getObjectProperty(port, PORT, SOURCE_BLOCK, blk2);
            std::string name2;
            controller.getObjectProperty(blk2, BLOCK, SIM_FUNCTION_NAME, name2);
            if (name2 != split && name2 != lsplit && name2 != limpsplit)
            {
                return false;
            }
        }
    }
    return true;
}

void setLinkEnd(const ScicosID id, Controller& controller, const object_properties_t end, const link_t& v)
{

    ScicosID from;
    controller.getObjectProperty(id, LINK, SOURCE_PORT, from);
    ScicosID to;
    controller.getObjectProperty(id, LINK, DESTINATION_PORT, to);
    ScicosID concernedPort;
    ScicosID otherPort;
    switch (end)
    {
        case SOURCE_PORT:
            concernedPort = from;
            otherPort = to;
            break;
        case DESTINATION_PORT:
            concernedPort = to;
            otherPort = from;
            break;
        default:
            return;
    }
    ScicosID unconnected = 0;

    if (v.block == 0 || v.port == 0)
    {
        // We want to set an empty link
        if (concernedPort == 0)
        {
            // In this case, the link was already empty, do a dummy call to display the console status.
            controller.setObjectProperty(id, LINK, end, concernedPort);
        }
        else
        {
            // Untie the old link on the concerned end and set its port as unconnected
            controller.setObjectProperty(concernedPort, PORT, CONNECTED_SIGNALS, unconnected);
            controller.setObjectProperty(id, LINK, end, unconnected);
        }
        return;
    }

    ScicosID parentDiagram;
    controller.getObjectProperty(id, LINK, PARENT_DIAGRAM, parentDiagram);
    std::vector<ScicosID> children;
    if (parentDiagram != 0)
    {
        controller.getObjectProperty(parentDiagram, DIAGRAM, CHILDREN, children);
    }

    // Connect the new one

    if (v.kind != Start && v.kind != End)
    {
        return;
    }
    // kind == 0: trying to set the start of the link (output port)
    // kind == 1: trying to set the end of the link (input port)

    if (v.block > static_cast<int>(children.size()))
    {
        return; // Trying to link to a non-existing block
    }
    ScicosID blkID = children[v.block - 1];

    // Check that the ID designates a BLOCK (and not an ANNOTATION)
    if (controller.getObject(blkID)->kind() != BLOCK)
    {
        return;
    }

    // v.port may be decremented locally to square with the port indexes
    int portIndex = v.port;

    std::vector<ScicosID> sourceBlockPorts;
    bool newPortIsImplicit = false;
    enum model::portKind newPortKind = model::PORT_UNDEF;
    int linkType;
    controller.getObjectProperty(id, LINK, KIND, linkType);
    if (linkType == model::activation)
    {
        std::vector<ScicosID> evtin;
        std::vector<ScicosID> evtout;
        controller.getObjectProperty(blkID, BLOCK, EVENT_INPUTS, evtin);
        controller.getObjectProperty(blkID, BLOCK, EVENT_OUTPUTS, evtout);

        if (v.kind == Start)
        {
            if (otherPort != 0)
            {
                if (!checkConnectivity(model::PORT_EIN, otherPort, blkID, controller))
                {
                    return;
                }
            }
            newPortKind = model::PORT_EOUT;
            sourceBlockPorts = evtout;
        }
        else
        {
            if (otherPort != 0)
            {
                if (!checkConnectivity(model::PORT_EOUT, otherPort, blkID, controller))
                {
                    return;
                }
            }
            newPortKind = model::PORT_EIN;
            sourceBlockPorts = evtin;
        }

    }
    else if (linkType == model::regular || linkType == model::implicit)
    {
        std::vector<ScicosID> in;
        std::vector<ScicosID> out;
        controller.getObjectProperty(blkID, BLOCK, INPUTS, in);
        controller.getObjectProperty(blkID, BLOCK, OUTPUTS, out);

        if (linkType == model::regular)
        {
            if (v.kind == Start)
            {
                if (otherPort != 0)
                {
                    if (!checkConnectivity(model::PORT_IN, otherPort, blkID, controller))
                    {
                        return;
                    }
                }
                newPortKind = model::PORT_OUT;
                sourceBlockPorts = out;
            }
            else
            {
                if (otherPort != 0)
                {
                    if (!checkConnectivity(model::PORT_OUT, otherPort, blkID, controller))
                    {
                        return;
                    }
                }
                newPortKind = model::PORT_IN;
                sourceBlockPorts = in;
            }

            // Rule out the implicit ports
            for (size_t i = 0; i < sourceBlockPorts.size(); ++i)
            {
                bool isImplicit;
                controller.getObjectProperty(sourceBlockPorts[i], PORT, IMPLICIT, isImplicit);
                if (isImplicit == true)
                {
                    sourceBlockPorts.erase(sourceBlockPorts.begin() + i);
                    if (portIndex > static_cast<int>(i + 1))
                    {
                        portIndex--; // Keep portIndex consistent with the port indexes
                    }
                }
            }
        }
        else // model::implicit
        {
            newPortIsImplicit = true;
            if (v.kind == Start)
            {
                sourceBlockPorts = out;
            }
            else // End
            {
                sourceBlockPorts = in;
            }

            // Rule out the explicit ports
            for (size_t i = 0; i < sourceBlockPorts.size(); ++i)
            {
                bool isImplicit;
                controller.getObjectProperty(sourceBlockPorts[i], PORT, IMPLICIT, isImplicit);
                if (isImplicit == false)
                {
                    sourceBlockPorts.erase(sourceBlockPorts.begin() + i);
                    if (portIndex > static_cast<int>(i + 1))
                    {
                        portIndex--; // Keep portIndex consistent with the port indexes
                    }
                }
            }
        }
    }

    // Disconnect the old port if it was connected. After that, concernedPort will be reused to designate the new port
    if (concernedPort != 0)
    {
        controller.setObjectProperty(concernedPort, PORT, CONNECTED_SIGNALS, unconnected);
    }

    int nBlockPorts = sourceBlockPorts.size();
    if (nBlockPorts >= portIndex)
    {
        concernedPort = sourceBlockPorts[portIndex - 1];
    }
    else
    {
        while (nBlockPorts < portIndex) // Create as many ports as necessary
        {
            concernedPort = controller.createObject(PORT);
            controller.setObjectProperty(concernedPort, PORT, IMPLICIT, newPortIsImplicit);
            controller.setObjectProperty(concernedPort, PORT, PORT_KIND, newPortKind);
            controller.setObjectProperty(concernedPort, PORT, SOURCE_BLOCK, blkID);
            controller.setObjectProperty(concernedPort, PORT, CONNECTED_SIGNALS, unconnected);
            // Set the default dataType so it is saved in the model
            std::vector<int> dataType;
            controller.getObjectProperty(concernedPort, PORT, DATATYPE, dataType);
            controller.setObjectProperty(concernedPort, PORT, DATATYPE, dataType);

            std::vector<ScicosID> concernedPorts;
            if (linkType == model::activation)
            {
                if (v.kind == Start)
                {
                    controller.getObjectProperty(blkID, BLOCK, EVENT_OUTPUTS, concernedPorts);
                    concernedPorts.push_back(concernedPort);
                    controller.setObjectProperty(blkID, BLOCK, EVENT_OUTPUTS, concernedPorts);
                }
                else
                {
                    controller.getObjectProperty(blkID, BLOCK, EVENT_INPUTS, concernedPorts);
                    concernedPorts.push_back(concernedPort);
                    controller.setObjectProperty(blkID, BLOCK, EVENT_INPUTS, concernedPorts);
                }
            }
            else // model::regular || model::implicit
            {
                if (v.kind == Start)
                {
                    controller.getObjectProperty(blkID, BLOCK, OUTPUTS, concernedPorts);
                    concernedPorts.push_back(concernedPort);
                    controller.setObjectProperty(blkID, BLOCK, OUTPUTS, concernedPorts);
                }
                else
                {
                    controller.getObjectProperty(blkID, BLOCK, INPUTS, concernedPorts);
                    concernedPorts.push_back(concernedPort);
                    controller.setObjectProperty(blkID, BLOCK, INPUTS, concernedPorts);
                }
            }

            nBlockPorts++;
        }
    }
    ScicosID oldLink;
    controller.getObjectProperty(concernedPort, PORT, CONNECTED_SIGNALS, oldLink);
    if (oldLink != 0)
    {
        // Disconnect the old link
        controller.setObjectProperty(oldLink, LINK, end, unconnected);
        controller.setObjectProperty(concernedPort, PORT, CONNECTED_SIGNALS, unconnected);
    }

    // Connect the new source and destination ports together
    controller.setObjectProperty(concernedPort, PORT, CONNECTED_SIGNALS, id);
    controller.setObjectProperty(id, LINK, end, concernedPort);
}

// check if the link is valid
bool is_valid(types::Double* o)
{
    if (o->getSize() >= 2)
    {
        if (floor(o->get(0)) != o->get(0) || floor(o->get(1)) != o->get(1))
        {
            return false; // Must be an integer value
        }
        if (o->get(1) < 0)
        {
            return false; // Must be positive
        }
    }
    if (o->getSize() == 3)
    {
        if (floor(o->get(2)) != o->get(2))
        {
            return false; // Must be an integer value
        }
        if (o->get(2) < 0)
        {
            return false; // Must be positive
        }
    }

    return true;
}

struct from
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& /*controller*/)
    {
        link_t from_content = adaptor.getFrom();

        double* data;
        types::Double* o = new types::Double(1, 3, &data);

        data[0] = static_cast<double>(from_content.block);
        data[1] = static_cast<double>(from_content.port);
        data[2] = static_cast<double>(from_content.kind);
        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();

        if (current->getSize() != 0 && current->getSize() != 2 && current->getSize() != 3)
        {
            return false;
        }

        if (!is_valid(current))
        {
            return false;
        }

        link_t from_content {0, 0, Start};
        if (current->getSize() >= 2)
        {
            from_content.block = current->get(0);
            from_content.port = current->get(1);
            // By default, kind designates an output (set to 0)
        }
        if (current->getSize() == 3)
        {
            from_content.kind = (current->get(2) == 0.) ? Start : End;
        }

        return adaptor.setFromInModel(from_content, controller);
    }
};

struct to
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& /*controller*/)
    {
        link_t to_content = adaptor.getTo();

        double* data;
        types::Double* o = new types::Double(1, 3, &data);

        data[0] = static_cast<double>(to_content.block);
        data[1] = static_cast<double>(to_content.port);
        data[2] = static_cast<double>(to_content.kind);
        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();

        if (current->getSize() != 0 && current->getSize() != 2 && current->getSize() != 3)
        {
            return false;
        }

        if (!is_valid(current))
        {
            return false;
        }

        link_t to_content {0, 0, End};
        if (current->getSize() >= 2)
        {
            to_content.block = current->get(0);
            to_content.port = current->get(1);
            // By default, kind designates an input (set to 1)
        }
        if (current->getSize() == 3)
        {
            to_content.kind = (current->get(2) == 0.) ? Start : End;
        }

        return adaptor.setToInModel(to_content, controller);
    }
};

} /* namespace */

template<> property<LinkAdapter>::props_t property<LinkAdapter>::fields = property<LinkAdapter>::props_t();

LinkAdapter::LinkAdapter(std::shared_ptr<org_scilab_modules_scicos::model::Link> adaptee) :
    BaseAdapter<LinkAdapter, org_scilab_modules_scicos::model::Link>(adaptee),
    m_from(),
    m_to()
{
    if (property<LinkAdapter>::properties_have_not_been_set())
    {
        property<LinkAdapter>::fields.reserve(7);
        property<LinkAdapter>::add_property(L"xx", &xx::get, &xx::set);
        property<LinkAdapter>::add_property(L"yy", &yy::get, &yy::set);
        property<LinkAdapter>::add_property(L"id", &id::get, &id::set);
        property<LinkAdapter>::add_property(L"thick", &thick::get, &thick::set);
        property<LinkAdapter>::add_property(L"ct", &ct::get, &ct::set);
        property<LinkAdapter>::add_property(L"from", &from::get, &from::set);
        property<LinkAdapter>::add_property(L"to", &to::get, &to::set);
    }

    // If the Link has been added to a diagram, the following lines will dig up its information at model-level
    Controller controller;
    types::Double* current = getLinkEnd(*this, controller, SOURCE_PORT);
    m_from.block = current->get(0);
    m_from.port = current->get(1);
    m_from.kind = (current->get(2) == 0.) ? Start : End;

    current   = getLinkEnd(*this, controller, DESTINATION_PORT);
    m_to.block = current->get(0);
    m_to.port = current->get(1);
    m_to.kind = (current->get(2) == 0.) ? Start : End;
}

LinkAdapter::LinkAdapter(const LinkAdapter& adapter) :
    BaseAdapter<LinkAdapter, org_scilab_modules_scicos::model::Link>(adapter),
    m_from(adapter.getFrom()),
    m_to(adapter.getTo())
{
}

LinkAdapter::~LinkAdapter()
{
}

std::wstring LinkAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring LinkAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

link_t LinkAdapter::getFrom() const
{
    return m_from;
}

void LinkAdapter::setFrom(const link_t& v)
{
    m_from = v;
}

bool LinkAdapter::setFromInModel(const link_t& v, Controller& controller)
{
    m_from = v;

    ScicosID parentDiagram;
    controller.getObjectProperty(getAdaptee()->id(), LINK, PARENT_DIAGRAM, parentDiagram);

    if (parentDiagram != 0)
    {
        // If the Link has been added to a diagram, do the linking at model-level
        // If the provided values are wrong, the model is not updated but the info is stored in the Adapter for future attempts
        setLinkEnd(getAdaptee()->id(), controller, SOURCE_PORT, v);
    }

    return true;
}

link_t LinkAdapter::getTo() const
{
    return m_to;
}

void LinkAdapter::setTo(const link_t& v)
{
    m_to = v;
}

bool LinkAdapter::setToInModel(const link_t& v, Controller& controller)
{
    m_to = v;

    ScicosID parentDiagram;
    controller.getObjectProperty(getAdaptee()->id(), LINK, PARENT_DIAGRAM, parentDiagram);

    if (parentDiagram != 0)
    {
        // If the Link has been added to a diagram, do the linking at model-level
        // If the provided values are wrong, the model is not updated but the info is stored in the Adapter for future attempts
        setLinkEnd(getAdaptee()->id(), controller, DESTINATION_PORT, v);
    }

    return true;
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
