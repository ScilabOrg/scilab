/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015-2015 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.io.sax;

import java.util.ArrayList;

import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfInt;
import org.scilab.modules.xcos.VectorOfScicosID;
import org.scilab.modules.xcos.graph.model.ScicosObjectOwner;
import org.scilab.modules.xcos.graph.model.XcosCell;
import org.scilab.modules.xcos.io.HandledElement;
import org.scilab.modules.xcos.io.sax.XcosSAXHandler.UnresolvedReference;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.xml.sax.Attributes;

class PortHandler implements ScilabHandler {

    private final XcosSAXHandler shared;

    /**
     * Default constructor
     * @param saxHandler the shared sax handler
     */
    PortHandler(XcosSAXHandler saxHandler) {
        this.shared = saxHandler;
    }

    @Override
    public BasicPort startElement(HandledElement found, Attributes atts) {
        String v;
        BasicPort port;
        ObjectProperties relatedProperty;
        boolean isImplicit;

        /*
         * First, check if the port has already been defined. Otherwise, create the object in the model
         */
        String strUID = atts.getValue("id");
        long uid = 0;
        if (strUID != null) {
            if (shared.allChildren.peek().containsKey(strUID)) {
                uid = shared.allChildren.peek().get(strUID);
            } else {
                uid = shared.controller.createObject(Kind.PORT);
                shared.allChildren.peek().put(strUID, uid);
            }
        }

        String style = atts.getValue("style");
        if (style != null) {
            shared.controller.setObjectProperty(uid, Kind.PORT, ObjectProperties.STYLE, style);
        }

        String value = atts.getValue("value");
        if (value != null) {
            shared.controller.setObjectProperty(uid, Kind.PORT, ObjectProperties.LABEL, value);
        }

        /*
         * Allocate the port with the right class to set default properties
         */

        switch (found) {
            case CommandPort:
                port = new CommandPort(shared.controller, uid, Kind.PORT, value, style, strUID);
                relatedProperty = ObjectProperties.EVENT_OUTPUTS;
                isImplicit = false;
                break;
            case ControlPort:
                port = new ControlPort(shared.controller, uid, Kind.PORT, value, style, strUID);
                relatedProperty = ObjectProperties.EVENT_INPUTS;
                isImplicit = false;
                break;
            case ExplicitInputPort:
                port = new ExplicitInputPort(shared.controller, uid, Kind.PORT, value, style, strUID);
                relatedProperty = ObjectProperties.INPUTS;
                isImplicit = false;
                break;
            case ExplicitOutputPort:
                port = new ExplicitOutputPort(shared.controller, uid, Kind.PORT, value, style, strUID);
                relatedProperty = ObjectProperties.OUTPUTS;
                isImplicit = false;
                break;
            case ImplicitInputPort:
                port = new ImplicitInputPort(shared.controller, uid, Kind.PORT, value, style, strUID);
                relatedProperty = ObjectProperties.INPUTS;
                isImplicit = true;
                break;
            case ImplicitOutputPort:
                port = new ImplicitOutputPort(shared.controller, uid, Kind.PORT, value, style, strUID);
                relatedProperty = ObjectProperties.OUTPUTS;
                isImplicit = true;
                break;
            default:
                throw new IllegalArgumentException();
        }

        /*
         * Setup the properties
         */

        VectorOfInt datatype = new VectorOfInt();
        shared.controller.getObjectProperty(uid, Kind.PORT, ObjectProperties.DATATYPE, datatype);

        v = atts.getValue("dataType");
        if (v != null) {
            datatype.set(0, BasicPort.DataType.valueOf(v).ordinal());
        }
        v = atts.getValue("dataColumns");
        if (v != null) {
            datatype.set(1, Integer.valueOf(v));
        }
        v = atts.getValue("dataLines");
        if (v != null) {
            datatype.set(2, Integer.valueOf(v));
        }

        shared.controller.setObjectProperty(uid, Kind.PORT, ObjectProperties.DATATYPE, datatype);

        v = atts.getValue("initialState");
        if (v != null) {
            shared.controller.setObjectProperty(uid, Kind.PORT, ObjectProperties.FIRING, Double.valueOf(v));
        }

        /*
         * Associate to the parent block : now or later
         */

        int ordering = 0;
        Long parent = 0l;

        v = atts.getValue("ordering");
        if (v != null) {
            ordering = Integer.valueOf(v) - 1;
        }

        v = atts.getValue("parent");
        if (v != null) {
            parent = shared.allChildren.peek().get(v);

            // if we can resolve the parent, then connect it directly
            if (parent != null) {
                VectorOfScicosID associatedPorts = new VectorOfScicosID();
                shared.controller.getObjectProperty(parent, Kind.BLOCK, relatedProperty, associatedPorts);

                associatedPorts.resize(Math.max(associatedPorts.size(), ordering + 1));
                associatedPorts.set(ordering, uid);
                shared.controller.referenceObject(uid);

                shared.controller.setObjectProperty(uid, Kind.PORT, ObjectProperties.SOURCE_BLOCK, parent);
                shared.controller.setObjectProperty(parent, Kind.BLOCK, relatedProperty, associatedPorts);
            } else {
                // resolve the parent later
                ArrayList<UnresolvedReference> refList = shared.unresolvedReferences.get(v);
                if (refList == null) {
                    refList = new ArrayList<>();
                    shared.unresolvedReferences.put(v, refList);
                }
                refList.add(new UnresolvedReference(new ScicosObjectOwner(uid, Kind.PORT), ObjectProperties.SOURCE_BLOCK, relatedProperty, ordering));
            }
        }

        /*
         * Associate to the link if possible (reverse linking)
         */
        v = atts.getValue("as");
        if (v != null) {
            ObjectProperties opposite = null;
            if ("source".equals(v)) {
                opposite = ObjectProperties.SOURCE_PORT;
            } else if ("target".equals(v)) {
                opposite = ObjectProperties.DESTINATION_PORT;
            }

            XcosCell cell = shared.lookupForParentXcosCellElement();
            if (cell.getKind() == Kind.LINK) {
                shared.controller.setObjectProperty(cell.getUID(), cell.getKind(), opposite, port.getUID());
                shared.controller.setObjectProperty(port.getUID(), port.getKind(), ObjectProperties.CONNECTED_SIGNALS, cell.getUID());
            }
        }

        return port;
    }

    @Override
    public void endElement(HandledElement found) {
    }
}