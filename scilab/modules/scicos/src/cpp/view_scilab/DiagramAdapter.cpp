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
#include <sstream>

#include "internal.hxx"
#include "double.hxx"
#include "list.hxx"
#include "tlist.hxx"
#include "string.hxx"
#include "types.hxx"
#include "user.hxx"

#include "utilities.hxx"
#include "Controller.hxx"
#include "DiagramAdapter.hxx"
#include "Adapters.hxx"
#include "ParamsAdapter.hxx"
#include "TextAdapter.hxx"
#include "BlockAdapter.hxx"
#include "LinkAdapter.hxx"
#include "model/BaseObject.hxx"
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

struct props
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        ParamsAdapter localAdaptor = ParamsAdapter(false, adaptor.getAdaptee());
        return localAdaptor.getAsTList(new types::TList(), controller);
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ParamsAdapter localAdaptor = ParamsAdapter(false, adaptor.getAdaptee());
        return localAdaptor.setAsTList(v, controller);
    }
};

enum startOrEnd
{
    Start = 0,
    End = 1
};

static types::Double* getLinkEnd(const LinkAdapter& adaptor, const Controller& controller, object_properties_t end)
{
    model::Link* adaptee = adaptor.getAdaptee();

    double* data;
    types::Double* o = new types::Double(1, 3, &data);
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;

    ScicosID endID;
    controller.getObjectProperty(adaptee->id(), adaptee->kind(), end, endID);
    if (endID != 0)
    {
        ScicosID sourceBlock;
        controller.getObjectProperty(endID, PORT, SOURCE_BLOCK, sourceBlock);

        // Looking for the block number among the block IDs
        ScicosID parentDiagram;
        controller.getObjectProperty(adaptee->id(), BLOCK, PARENT_DIAGRAM, parentDiagram);
        std::vector<ScicosID> children;
        if (parentDiagram == 0)
        {
            return o;
        }
        controller.getObjectProperty(parentDiagram, DIAGRAM, CHILDREN, children);
        data[0] = static_cast<double>(std::distance(children.begin(), std::find(children.begin(), children.end(), sourceBlock)) + 1);

        std::vector<ScicosID> sourceBlockPorts;
        switch (end)
        {
            case SOURCE_PORT:
                controller.getObjectProperty(sourceBlock, BLOCK, OUTPUTS, sourceBlockPorts);
                break;
            case DESTINATION_PORT:
                controller.getObjectProperty(sourceBlock, BLOCK, INPUTS, sourceBlockPorts);
                break;
            default:
                return 0;
        }
        data[1] = static_cast<double>(std::distance(sourceBlockPorts.begin(), std::find(sourceBlockPorts.begin(), sourceBlockPorts.end(), endID)) + 1);

        bool isImplicit;
        controller.getObjectProperty(endID, PORT, IMPLICIT, isImplicit);

        if (isImplicit == false)
        {
            int kind;
            controller.getObjectProperty(endID, PORT, PORT_KIND, kind);
            if (kind == model::IN || kind == model::EIN)
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
static bool checkConnectivity(const int neededType, const ScicosID port, const ScicosID blk1, Controller& controller)
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

static bool setLinkEnd(LinkAdapter& adaptor, Controller& controller, object_properties_t end, types::InternalType* v)
{
    model::Link* adaptee = adaptor.getAdaptee();

    if (v->getType() != types::InternalType::ScilabDouble)
    {
        return false;
    }

    types::Double* current = v->getAs<types::Double>();

    if ((current->getRows() != 1 || current->getCols() != 3) && current->getSize() != 0)
    {
        return false; // Must be [] or [x y z]
    }

    ScicosID from;
    controller.getObjectProperty(adaptee->id(), adaptee->kind(), SOURCE_PORT, from);
    ScicosID to;
    controller.getObjectProperty(adaptee->id(), adaptee->kind(), DESTINATION_PORT, to);
    ScicosID concernedPort;
    ScicosID otherPort;
    object_properties_t otherEnd;
    switch (end)
    {
        case SOURCE_PORT:
            concernedPort = from;
            otherPort = to;
            otherEnd = DESTINATION_PORT;
            break;
        case DESTINATION_PORT:
            concernedPort = to;
            otherPort = from;
            otherEnd = SOURCE_PORT;
            break;
        default:
            return false;
    }
    ScicosID unconnected = 0;

    if (current->getSize() == 0 || (current->get(0) == 0 || current->get(1) == 0))
    {
        // We want to set an empty link
        if (concernedPort == 0)
        {
            // In this case, the link was already empty, do a dummy call to display the console status.
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), end, concernedPort);
        }
        else
        {
            // Untie the old link on both ends and set the 2 concerned ports as unconnected
            controller.setObjectProperty(from, PORT, CONNECTED_SIGNALS, unconnected);
            controller.setObjectProperty(to, PORT, CONNECTED_SIGNALS, unconnected);

            controller.setObjectProperty(adaptee->id(), adaptee->kind(), SOURCE_PORT, unconnected);
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), DESTINATION_PORT, unconnected);
        }
        // Notify the Adapter that the link is being untied
        types::Double* zeros = new types::Double(1, 3);
        zeros->setZeros();
        adaptor.setFrom(zeros);
        adaptor.setTo(zeros);
        return true;
    }

    if (current->get(2) != 0 && current->get(2) != 1)
    {
        return false;
    }

    if (floor(current->get(0)) != current->get(0) || floor(current->get(1)) != current->get(1))
    {
        return false; // Must be an integer value
    }

    ScicosID parentDiagram;
    controller.getObjectProperty(adaptee->id(), LINK, PARENT_DIAGRAM, parentDiagram);
    std::vector<ScicosID> children;
    if (parentDiagram != 0)
    {
        controller.getObjectProperty(parentDiagram, DIAGRAM, CHILDREN, children);
    }

    // Connect the new one
    int blk  = static_cast<int>(current->get(0));
    int port = static_cast<int>(current->get(1));
    int kind = static_cast<int>(current->get(2));
    if (kind != Start && kind != End)
    {
        return false;
    }
    // kind == 0: trying to set the start of the link (output port)
    // kind == 1: trying to set the end of the link (input port)

    if (blk < 0 || blk > static_cast<int>(children.size()))
    {
        return false; // Trying to link to a non-existing block
    }
    ScicosID blkID = children[blk - 1];

    // Check that the ID designates a BLOCK (and not an ANNOTATION)
    if (controller.getObject(blkID)->kind() != BLOCK)
    {
        return false;
    }

    std::vector<ScicosID> sourceBlockPorts;
    int nBlockPorts;
    bool newPortIsImplicit = false;
    int newPortKind = static_cast<int>(model::UNDEF);
    int linkType;
    controller.getObjectProperty(adaptee->id(), adaptee->kind(), KIND, linkType);
    if (linkType == model::activation)
    {
        std::vector<ScicosID> evtin;
        std::vector<ScicosID> evtout;
        controller.getObjectProperty(blkID, BLOCK, EVENT_INPUTS, evtin);
        controller.getObjectProperty(blkID, BLOCK, EVENT_OUTPUTS, evtout);

        if (kind == Start)
        {
            if (otherPort != 0)
            {
                if (!checkConnectivity(model::EIN, otherPort, blkID, controller))
                {
                    return false;
                }
            }
            newPortKind = static_cast<int>(model::EOUT);
            sourceBlockPorts = evtout;
        }
        else
        {
            if (otherPort != 0)
            {
                if (!checkConnectivity(model::EOUT, otherPort, blkID, controller))
                {
                    return false;
                }
            }
            newPortKind = static_cast<int>(model::EIN);
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
            if (kind == Start)
            {
                if (otherPort != 0)
                {
                    if (!checkConnectivity(model::IN, otherPort, blkID, controller))
                    {
                        return false;
                    }
                }
                newPortKind = static_cast<int>(model::OUT);
                sourceBlockPorts = out;
            }
            else
            {
                if (otherPort != 0)
                {
                    if (!checkConnectivity(model::OUT, otherPort, blkID, controller))
                    {
                        return false;
                    }
                }
                newPortKind = static_cast<int>(model::IN);
                sourceBlockPorts = in;
            }

            // Rule out the implicit ports
            for (std::vector<ScicosID>::iterator it = sourceBlockPorts.begin(); it != sourceBlockPorts.end(); ++it)
            {
                bool isImplicit;
                controller.getObjectProperty(*it, PORT, IMPLICIT, isImplicit);
                if (isImplicit == true)
                {
                    sourceBlockPorts.erase(it);
                }
            }
        }
        else // model::implicit
        {
            newPortIsImplicit = true;
            sourceBlockPorts.insert(sourceBlockPorts.begin(), in.begin(), in.end());
            sourceBlockPorts.insert(sourceBlockPorts.begin(), out.begin(), out.end());

            // Rule out the explicit ports
            for (std::vector<ScicosID>::iterator it = sourceBlockPorts.begin(); it != sourceBlockPorts.end(); ++it)
            {
                bool isImplicit;
                controller.getObjectProperty(*it, PORT, IMPLICIT, isImplicit);
                if (isImplicit == false)
                {
                    sourceBlockPorts.erase(it);
                }
            }
        }
    }

    // Disconnect the old port if it was connected. After that, concernedPort will be reused to designate the new port
    if (concernedPort != 0)
    {
        controller.setObjectProperty(concernedPort, PORT, CONNECTED_SIGNALS, unconnected);
    }

    nBlockPorts = static_cast<int>(sourceBlockPorts.size());
    if (nBlockPorts >= port)
    {
        concernedPort = sourceBlockPorts[port - 1];
    }
    else
    {
        while (nBlockPorts < port) // Create as many ports as necessary
        {
            concernedPort = controller.createObject(PORT);
            controller.setObjectProperty(concernedPort, PORT, IMPLICIT, newPortIsImplicit);
            controller.setObjectProperty(concernedPort, PORT, PORT_KIND, newPortKind);
            controller.setObjectProperty(concernedPort, PORT, SOURCE_BLOCK, blkID);
            controller.setObjectProperty(concernedPort, PORT, CONNECTED_SIGNALS, unconnected);
            std::vector<int> dataType;
            controller.getObjectProperty(concernedPort, PORT, DATATYPE, dataType);
            dataType[0] = -1; // Default number of rows for new ports
            controller.setObjectProperty(concernedPort, PORT, DATATYPE, dataType);

            std::vector<ScicosID> concernedPorts;
            if (linkType == model::activation)
            {
                if (kind == Start)
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
                if (kind == Start)
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
        // Disconnect the old other end port and delete the old link
        ScicosID oldPort;
        controller.getObjectProperty(oldLink, LINK, otherEnd, oldPort);
        controller.setObjectProperty(oldPort, PORT, CONNECTED_SIGNALS, unconnected);
        controller.deleteObject(oldLink);
    }

    // Connect the new source and destination ports together
    controller.setObjectProperty(concernedPort, PORT, CONNECTED_SIGNALS, adaptee->id());
    controller.setObjectProperty(adaptee->id(), adaptee->kind(), end, concernedPort);
    return true;
}

struct objs
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        // FIXME: get all children of the Diagram and return them as a list
        model::Diagram* adaptee = adaptor.getAdaptee();

        std::vector<ScicosID> children;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), CHILDREN, children);

        types::List* o = new types::List();

        for (int i = 0; i < static_cast<int>(children.size()); ++i)
        {
            model::BaseObject* item = Controller().getObject(children[i]);
            switch (item->kind())
            {
                case ANNOTATION:
                {
                    model::Annotation* annotation = static_cast<model::Annotation*>(item);
                    TextAdapter* localAdaptor = new TextAdapter(false, annotation);
                    o->set(i, localAdaptor);
                    continue;
                }
                case BLOCK:
                {
                    model::Block* block = static_cast<model::Block*>(item);
                    BlockAdapter* localAdaptor = new BlockAdapter(false, block);
                    o->set(i, localAdaptor);
                    continue;
                }
                case LINK:
                {
                    model::Link* link = static_cast<model::Link*>(item);
                    LinkAdapter* localAdaptor = new LinkAdapter(false, link);

                    localAdaptor->setFrom(getLinkEnd(*localAdaptor, controller, SOURCE_PORT));
                    localAdaptor->setTo(getLinkEnd(*localAdaptor, controller, DESTINATION_PORT));
                    o->set(i, localAdaptor);
                    continue;
                }
                default:
                    return 0;
            }
        }
        return o;
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        // FIXME implement, decode the list and set all children of the Diagram
        if (v->getType() != types::InternalType::ScilabList)
        {
            return false;
        }

        model::Diagram* adaptee = adaptor.getAdaptee();

        types::List* list = v->getAs<types::List>();
        std::vector<ScicosID> diagramChildren (list->getSize());
        for (int i = 0; i < list->getSize(); ++i)
        {
            if (list->get(i)->getType() != types::InternalType::ScilabUserType)
            {
                return false;
            }

            // Find the type of the input object through Adapters' mapping.
            const Adapters::adapters_index_t adapter_index = Adapters::instance().lookup_by_typename(list->get(i)->getShortTypeStr());

            // Then, each adapter gets linked to the diagram through its adaptee (PARENT_DIAGRAM)
            // and the diagram's adaptee lists its adaptees (CHILDREN).
            ScicosID id;
            switch (adapter_index)
            {
                case Adapters::BLOCK_ADAPTER:
                {
                    BlockAdapter* modelElement = list->get(i)->getAs<BlockAdapter>();
                    model::Block* subAdaptee = modelElement->getAdaptee();

                    id = subAdaptee->id();

                    controller.setObjectProperty(id, subAdaptee->kind(), PARENT_DIAGRAM, adaptee->id());
                    break;
                }
                case Adapters::LINK_ADAPTER:
                {
                    LinkAdapter* modelElement = list->get(i)->getAs<LinkAdapter>();
                    model::Link* subAdaptee = modelElement->getAdaptee();

                    id = subAdaptee->id();

                    controller.setObjectProperty(id, subAdaptee->kind(), PARENT_DIAGRAM, adaptee->id());

                    // Do the linking at model-level thanks to the information that was saved in LinkAdapter at the Link's creation
                    if (!setLinkEnd(*modelElement, controller, SOURCE_PORT, modelElement->getFrom()))
                    {
                        return false;
                    }
                    if (!setLinkEnd(*modelElement, controller, DESTINATION_PORT, modelElement->getTo()))
                    {
                        return false;
                    }

                    break;
                }
                case Adapters::TEXT_ADAPTER:
                {
                    TextAdapter* modelElement = list->get(i)->getAs<TextAdapter>();
                    model::Annotation* subAdaptee = modelElement->getAdaptee();

                    id = subAdaptee->id();

                    controller.setObjectProperty(id, subAdaptee->kind(), PARENT_DIAGRAM, adaptee->id());
                    break;
                }
                default:
                    return false;
            }

            diagramChildren[i] = id;
        }

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), CHILDREN, diagramChildren);
        return true;
    }
};

struct version
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        model::Diagram* adaptee = adaptor.getAdaptee();

        std::string version;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), VERSION_NUMBER, version);

        return new types::String(version.data());
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() == types::InternalType::ScilabString)
        {
            types::String* current = v->getAs<types::String>();
            if (current->getSize() != 1)
            {
                return false;
            }

            model::Diagram* adaptee = adaptor.getAdaptee();

            char* c_str = wide_string_to_UTF8(current->get(0));
            std::string version (c_str);
            FREE(c_str);

            controller.setObjectProperty(adaptee->id(), adaptee->kind(), VERSION_NUMBER, version);
            return true;
        }
        else if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();
            if (current->getSize() != 0)
            {
                return false;
            }

            model::Diagram* adaptee = adaptor.getAdaptee();

            std::string version;
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), VERSION_NUMBER, version);
            return true;
        }

        return false;
    }
};

struct contrib
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        // silent unused parameter warnings
        (void) controller;

        return adaptor.getContribContent();
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        // silent unused parameter warnings
        (void) v;
        (void) controller;

        adaptor.setContribContent(v->clone());
        return true;
    }
};

} /* namespace */

template<> property<DiagramAdapter>::props_t property<DiagramAdapter>::fields = property<DiagramAdapter>::props_t();

DiagramAdapter::DiagramAdapter(bool ownAdaptee, org_scilab_modules_scicos::model::Diagram* adaptee) :
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>(ownAdaptee, adaptee)
{
    if (property<DiagramAdapter>::properties_have_not_been_set())
    {
        property<DiagramAdapter>::fields.reserve(4);
        property<DiagramAdapter>::add_property(L"props", &props::get, &props::set);
        property<DiagramAdapter>::add_property(L"objs", &objs::get, &objs::set);
        property<DiagramAdapter>::add_property(L"version", &version::get, &version::set);
        property<DiagramAdapter>::add_property(L"contrib", &contrib::get, &contrib::set);
    }

    contrib_content = new types::List();
}

DiagramAdapter::~DiagramAdapter()
{
    delete contrib_content;
}

std::wstring DiagramAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring DiagramAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

types::InternalType* DiagramAdapter::getContribContent() const
{
    return contrib_content->clone();
}

void DiagramAdapter::setContribContent(types::InternalType* v)
{
    delete contrib_content;
    contrib_content = v->clone();
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
