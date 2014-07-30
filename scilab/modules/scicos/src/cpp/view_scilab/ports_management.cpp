/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string>
#include <vector>

#include "ports_management.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

static const wchar_t E[] = L"E";
static const wchar_t I[] = L"I";

static types::Double* get_ports(const GraphicsAdapter& adaptor, object_properties_t p, const Controller& controller)
{
    model::Block* adaptee = adaptor.getAdaptee();

    // retrieve the identifiers
    std::vector<ScicosID> ids;
    controller.getObjectProperty(adaptee->id(), adaptee->kind(), p, ids);

    // allocate some space for the return value
    double* ports;
    types::Double* o = new types::Double(ids.size(), 1, &ports);

    // translate identifiers to return values
    for (std::vector<ScicosID>::iterator it = ids.begin(); it != ids.end(); ++it, ++ports)
    {
        std::vector<ScicosID> connectedSignals;

        controller.getObjectProperty(*it, PORT, CONNECTED_SIGNALS, connectedSignals);

        // always return the first connected signals, at scilab level a port can only be connected to one signal at a time.
        if (connectedSignals.empty())
        {
            // if the port is not connected returns 0
            *ports = 0;
        }
        else
        {
            // if the port is connected, return the index of the link in the parent diagram
            ScicosID link = connectedSignals[0];

            ScicosID parentDiagram = 0;
            controller.getObjectProperty(link, LINK, PARENT_DIAGRAM, parentDiagram);

            std::vector<ScicosID> children;
            controller.getObjectProperty(parentDiagram, DIAGRAM, CHILDREN, children);

            std::vector<ScicosID>::iterator it = std::find(children.begin(), children.end(), link);
            if (it != children.end())
            {
                int index = std::distance(children.begin(), it);
                *ports = index;
            }
            else
            {
                *ports = 0;
            }
        }

        ports++;
    }

    return o;
}

static types::String* get_ports_property(const GraphicsAdapter& adaptor, object_properties_t p, const Controller& controller, object_properties_t p2)
{
    model::Block* adaptee = adaptor.getAdaptee();

    // Retrieve the identifiers
    std::vector<ScicosID> ids;
    controller.getObjectProperty(adaptee->id(), adaptee->kind(), p, ids);

    // Allocate some space for the return value
    types::String* o = new types::String(ids.size(), 1);

    // Translate identifiers to return values
    int i = 0;
    switch (p2)
    {
        case IMPLICIT:
        {
            for (std::vector<ScicosID>::iterator it = ids.begin(); it != ids.end(); ++it, ++i)
            {
                bool v;
                controller.getObjectProperty(*it, PORT, p2, v);
                o->set(i, (v == false) ? E : I);
            }
            break;
        }
        case STYLE:
        case LABEL:
        {
            for (std::vector<ScicosID>::iterator it = ids.begin(); it != ids.end(); ++it, ++i)
            {
                std::string s;
                controller.getObjectProperty(*it, PORT, p2, s);
                o->set(i, s.data());
            }
            break;
        }
        default:
            return o;
    }

    return o;
}

static bool create_ports(const GraphicsAdapter& adaptor, types::InternalType* v, object_properties_t p, Controller& controller)
{
    model::Block* adaptee = adaptor.getAdaptee();

    if (v->getType() != types::InternalType::ScilabDouble)
    {
        return false;
    }
    types::Double* value = v->getAs<types::Double>();

    ScicosID parentDiagram;
    controller.getObjectProperty(adaptee->id(), BLOCK, PARENT_DIAGRAM, parentDiagram);

    std::vector<ScicosID> children;
    if (parentDiagram != 0)
    {
        controller.getObjectProperty(parentDiagram, DIAGRAM, CHILDREN, children);
    }

    std::vector<int> newPorts = std::vector<int>(value->getSize());
    double* d = value->getReal();
    for (std::vector<int>::iterator it = newPorts.begin(); it != newPorts.end(); ++it, ++d)
    {
        if (0 > *d && *d >= children.size())
        {
            return false;
        }

        *it = (int) * d;
    }
    std::vector<ScicosID> deletedObjects;

    // retrieve old data
    std::vector<ScicosID> oldPorts;
    controller.getObjectProperty(adaptee->id(), adaptee->kind(), p, oldPorts);
    std::vector<ScicosID> previousPorts = oldPorts;

    // updated ports
    while (!oldPorts.empty() || !newPorts.empty())
    {
        ScicosID oldPort = oldPorts.back();
        oldPorts.pop_back();
        int newPort = newPorts.back();
        newPorts.pop_back();

        ScicosID oldSignal;
        controller.getObjectProperty(oldPort, PORT, CONNECTED_SIGNALS, oldSignal);
        ScicosID newSignal = children[newPort];

        if (oldSignal != newSignal)
        {
            // disconnect the old link
            ScicosID oldSignalSrc;
            controller.getObjectProperty(oldSignal, LINK, SOURCE_PORT, oldSignalSrc);
            ScicosID oldSignalDst;
            controller.getObjectProperty(oldSignal, LINK, DESTINATION_PORT, oldSignalDst);

            ScicosID unconnected = 0;
            if (oldSignalSrc == oldPort)
            {
                controller.setObjectProperty(oldSignalDst, PORT, CONNECTED_SIGNALS, unconnected);
            }
            else // oldSignalDst == oldPort
            {
                controller.setObjectProperty(oldSignalSrc, PORT, CONNECTED_SIGNALS, unconnected);
            }
            // Link de-association is not performed as the link will be removed

            // connect the new link
            controller.setObjectProperty(newSignal, LINK, SOURCE_PORT, 0);
            controller.setObjectProperty(oldPort, PORT, CONNECTED_SIGNALS, newSignal);

            children.erase(std::find(children.begin(), children.end(), oldSignal));
            deletedObjects.push_back(oldSignal);
        }
    }

    // removed ports
    if (!oldPorts.empty())
    {
        previousPorts.erase(previousPorts.begin() + oldPorts.size(), previousPorts.end());

        while (!oldPorts.empty())
        {
            ScicosID oldPort = oldPorts.back();
            oldPorts.pop_back();

            ScicosID signal;
            controller.getObjectProperty(oldPort, PORT, CONNECTED_SIGNALS, signal);
            if (signal != 0)
            {
                // the link is connected, disconnect the other side
                ScicosID oldSignalSrc;
                controller.getObjectProperty(signal, LINK, SOURCE_PORT, oldSignalSrc);
                ScicosID oldSignalDst;
                controller.getObjectProperty(signal, LINK, DESTINATION_PORT, oldSignalDst);

                ScicosID unconnected = 0;
                if (oldSignalSrc == oldPort)
                {
                    controller.setObjectProperty(oldSignalDst, PORT, CONNECTED_SIGNALS, unconnected);
                }
                else     // oldSignalDst == oldPort
                {
                    controller.setObjectProperty(oldSignalSrc, PORT, CONNECTED_SIGNALS, unconnected);
                }

                children.erase(std::find(children.begin(), children.end(), signal));
                deletedObjects.push_back(signal);
            }

            deletedObjects.push_back(oldPort);
        }

        controller.setObjectProperty(adaptee->id(), BLOCK, p, previousPorts);
    }

    // added ports
    if (!newPorts.empty())
    {
        while (!newPorts.empty())
        {
            int newPort = newPorts.back();
            oldPorts.pop_back();

            ScicosID id = controller.createObject(PORT);
            controller.setObjectProperty(id, PORT, SOURCE_BLOCK, adaptee->id());
            // set the connected signal if applicable
            if (newPort != 0)
            {
                ScicosID signal = children[newPort];
                controller.setObjectProperty(id, PORT, CONNECTED_SIGNALS, signal);
            }

            previousPorts.push_back(id);
        }

        controller.setObjectProperty(adaptee->id(), BLOCK, p, previousPorts);
    }

    // remove objects from the model after de-association
    if (parentDiagram != 0)
    {
        controller.setObjectProperty(parentDiagram, DIAGRAM, CHILDREN, children);
    }
    for (std::vector<ScicosID>::iterator it = deletedObjects.begin(); it != deletedObjects.end(); ++it)
    {
        controller.deleteObject(*it);
    }

    return true;
}

static bool set_ports_property(const GraphicsAdapter& adaptor, types::InternalType* v, object_properties_t p, Controller& controller, object_properties_t p2)
{
    if (v->getType() == types::InternalType::ScilabString)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        types::String* current = v->getAs<types::String>();
        if (current->getCols() != 0 && current->getCols() != 1)
        {
            return false;
        }

        // Retrieve the ports i dentifiers
        std::vector<ScicosID> ids;
        size_t rows = current->getRows();
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), p, ids);
        if (rows != ids.size())
        {
            return false;
        }

        int i = 0;
        switch (p2)
        {
            case IMPLICIT:
            {
                for (std::vector<ScicosID>::iterator it = ids.begin(); it != ids.end(); ++it, ++i)
                {
                    if (current->get(i) == I)
                    {
                        controller.setObjectProperty(*it, PORT, p2, true);
                    }
                    else if (current->get(i) == E)
                    {
                        controller.setObjectProperty(*it, PORT, p2, false);
                    }
                    else
                    {
                        return false;
                    }
                }
                break;
            }
            case STYLE:
            case LABEL:
            {
                std::vector<std::string> style = std::vector<std::string>(current->getSize());
                for (std::vector<ScicosID>::iterator it = ids.begin(); it != ids.end(); ++it, ++i)
                {
                    char* c_str = wide_string_to_UTF8(current->get(i));
                    style[i] = std::string(c_str);
                    FREE(c_str);
                    controller.setObjectProperty(*it, PORT, p2, style[i]);
                }
                break;
            }
            default:
                return false;
        }
        return true;
    }
    else if (v->getType() == types::InternalType::ScilabDouble)
    {
        types::Double* current = v->getAs<types::Double>();
        if (current->getRows() != 0 || current->getCols() != 0)
        {
            return false;
        }
        types::String* o = get_ports_property(adaptor, p, controller, p2);
        if (o->getSize() != 0)
        {
            return false;
        }
        // Do nothing, because if the sizes match, then there are already zero concerned ports, so no ports to update
        delete o;
        return true;
    }
    return false;
}

} /* view_scilab */
} /* namespace org_scilab_modules_scicos */
