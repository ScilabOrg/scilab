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

#include <algorithm>
#include <string>
#include <vector>

#include "internal.hxx"
#include "double.hxx"
#include "string.hxx"

#include "ports_management.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

static const wchar_t E[] = L"E";
static const wchar_t I[] = L"I";

types::InternalType* get_ports_property(const GraphicsAdapter& adaptor, object_properties_t port_kind, const Controller& controller, object_properties_t p)
{
    model::Block* adaptee = adaptor.getAdaptee();

    // Retrieve the identifiers
    std::vector<ScicosID> ids;
    controller.getObjectProperty(adaptee->id(), adaptee->kind(), p, ids);

    // Translate identifiers to return values
    int i = 0;
    switch (p)
    {
        case CONNECTED_SIGNALS:
        {
            double* v;
            types::Double* o = new types::Double(ids.size(), 1, &v);

            ScicosID diagram;
            controller.getObjectProperty(adaptee->id(), adaptee->kind(), PARENT_DIAGRAM, diagram);

            std::vector<ScicosID> children;
            controller.getObjectProperty(diagram, DIAGRAM, CHILDREN, children);

            for (std::vector<ScicosID>::iterator it = ids.begin(); it != ids.end(); ++it, ++i)
            {
                ScicosID id;
                controller.getObjectProperty(*it, PORT, p, id);

                std::vector<ScicosID>::iterator found = std::find(children.begin(), children.end(), id);

                if (found != children.end())
                {
                    v[i] = std::distance(found, children.begin());
                }
                else
                {
                    v[i] = 0;
                }
            }
            return o;
        }
        case STYLE:
        case LABEL:
        {
            types::String* o = new types::String(ids.size(), 1);
            for (std::vector<ScicosID>::iterator it = ids.begin(); it != ids.end(); ++it, ++i)
            {
                std::string s;
                controller.getObjectProperty(*it, PORT, p, s);
                o->set(i, s.data());
            }
            return o;
        }
        case IMPLICIT:
        {
            types::String* o = new types::String(ids.size(), 1);
            for (std::vector<ScicosID>::iterator it = ids.begin(); it != ids.end(); ++it, ++i)
            {
                bool v;
                controller.getObjectProperty(*it, PORT, p, v);
                o->set(i, (v == false) ? E : I);
            }
            return o;
        }
        default:
            return 0;
    }
}

bool set_ports_property(const GraphicsAdapter& adaptor, object_properties_t port_kind, Controller& controller, object_properties_t p, types::InternalType* v)
{
    model::Block* adaptee = adaptor.getAdaptee();

    // Retrieve the ports identifiers
    std::vector<ScicosID> ids;
    controller.getObjectProperty(adaptee->id(), adaptee->kind(), port_kind, ids);

    if (v->getType() == types::InternalType::ScilabString)
    {
        types::String* current = v->getAs<types::String>();
        if (current->getCols() != 0 && current->getCols() != 1)
        {
            return false;
        }

        size_t rows = current->getRows();
        if (rows != ids.size())
        {
            return false;
        }

        int i = 0;
        switch (p)
        {
            case IMPLICIT:
            {
                for (std::vector<ScicosID>::iterator it = ids.begin(); it != ids.end(); ++it, ++i)
                {
                    if (current->get(i) == I)
                    {
                        controller.setObjectProperty(*it, PORT, p, true);
                    }
                    else if (current->get(i) == E)
                    {
                        controller.setObjectProperty(*it, PORT, p, false);
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
                    controller.setObjectProperty(*it, PORT, p, style[i]);
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
        if (ids.size() != 0)
        {
            return false;
        }
        // Do nothing, because if the sizes match, then there are already zero concerned ports, so no ports to update
        return true;
    }
    return false;
}

bool update_ports_with_property(const GraphicsAdapter& adaptor, object_properties_t port_kind, Controller& controller, object_properties_t p, types::InternalType* v)
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
    controller.getObjectProperty(adaptee->id(), adaptee->kind(), port_kind, oldPorts);
    std::vector<ScicosID> previousPorts = oldPorts;

    if (p != CONNECTED_SIGNALS)
    {
        // FIXME: implement port creation for the blk.model
        return false;
    }

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

        controller.setObjectProperty(adaptee->id(), BLOCK, port_kind, previousPorts);
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

        controller.setObjectProperty(adaptee->id(), BLOCK, port_kind, previousPorts);
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

} /* view_scilab */
} /* namespace org_scilab_modules_scicos */
