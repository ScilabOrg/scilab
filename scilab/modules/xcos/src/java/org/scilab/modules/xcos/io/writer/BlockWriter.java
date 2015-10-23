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
package org.scilab.modules.xcos.io.writer;

import java.util.UUID;

import javax.xml.stream.XMLStreamException;

import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BasicBlock.SimulationFunctionType;
import org.scilab.modules.xcos.graph.model.BlockInterFunction;
import org.scilab.modules.xcos.graph.model.XcosCellFactory;
import org.scilab.modules.xcos.io.HandledElementsCategory;

public class BlockWriter extends ScilabWriter {

    public BlockWriter(XcosWriter writer) {
        super(writer);
    }

    @Override
    public void write(long uid, Kind kind) throws XMLStreamException {
        // Lookup for the element name
        String[] interfaceFunction = new String[1];
        BlockInterFunction interFunction;
        if (kind == Kind.BLOCK) {
            shared.controller.getObjectProperty(uid, kind, ObjectProperties.INTERFACE_FUNCTION, interfaceFunction);
            interFunction = XcosCellFactory.lookForInterfunction(interfaceFunction[0]);
        } else {
            interfaceFunction[0] = BlockInterFunction.TEXT_f.name();
            interFunction = BlockInterFunction.TEXT_f;
        }

        shared.stream.writeStartElement(interFunction.getKlass().getSimpleName());

        String[] v = new String[1];

        shared.controller.getObjectProperty(uid, kind, ObjectProperties.UID, v);
        if (v[0].isEmpty()) {
            v[0] = UUID.randomUUID().toString();
            shared.controller.setObjectProperty(uid, kind, ObjectProperties.UID, v[0]);
        }
        System.err.println("block " + uid + " as " + v[0]);
        shared.stream.writeAttribute("id", v[0]);
        shared.stream.writeAttribute("parent", shared.layers.peek());

        shared.stream.writeAttribute("interfaceFunctionName", interfaceFunction[0]);

        if (kind == Kind.BLOCK) {
            shared.controller.getObjectProperty(uid, kind, ObjectProperties.SIM_BLOCKTYPE, v);
            shared.stream.writeAttribute("blockType", v[0]);

            shared.controller.getObjectProperty(uid, kind, ObjectProperties.SIM_FUNCTION_NAME, v);
            shared.stream.writeAttribute("simulationFunctionName", v[0]);

            int[] functionAPI = new int[1];
            shared.controller.getObjectProperty(uid, kind, ObjectProperties.SIM_FUNCTION_API, functionAPI);
            SimulationFunctionType type = BasicBlock.SimulationFunctionType.convertScilabValue(functionAPI[0]);
            shared.stream.writeAttribute("simulationFunctionType", type.name());
        }

        shared.controller.getObjectProperty(uid, kind, ObjectProperties.STYLE, v);
        shared.stream.writeAttribute("style", v[0]);

        shared.controller.getObjectProperty(uid, kind, ObjectProperties.DESCRIPTION, v);
        if (!v[0].isEmpty()) {
            shared.stream.writeAttribute("value", v[0]);
        }

        // then delegate the sub-elements to their writers
        shared.writers.get(HandledElementsCategory.RAW_DATA).write(uid, kind);
        shared.writers.get(HandledElementsCategory.JGRAPHX).write(uid, kind);

        shared.stream.writeEndElement();
    }

}
