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

import javax.xml.stream.XMLStreamException;

import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfString;

public class RawDataWriter extends ScilabWriter {

    public RawDataWriter(XcosWriter writer) {
        super(writer);
    }

    @Override
    public void write(long uid, Kind kind) throws XMLStreamException {

        switch (kind) {
            case ANNOTATION:
            case BLOCK:
                writeBlock(uid, kind);
                break;
            case DIAGRAM:
                writeContext(uid, kind);
                break;
            case LINK:
            default:
                break;
        }
    }

    private void writeContext(long uid, Kind kind) throws XMLStreamException {
        VectorOfString context = new VectorOfString();
        shared.controller.getObjectProperty(uid, kind, ObjectProperties.DIAGRAM_CONTEXT, context);

        String[] value = new String[context.size()];
        for (int i = 0; i < value.length; i++) {
            value[i] = context.get(i);
        }
        write(value, "context");
    }

    private void writeBlock(long uid, Kind kind) {
        VectorOfDouble vDouble = new VectorOfDouble();
    }

    /*
     * Low-level serialization methods
     */

    private void write(String[] value, String as) throws XMLStreamException {
        shared.stream.writeStartElement("Array");
        shared.stream.writeAttribute("as", as);
        shared.stream.writeAttribute("scilabClass", "String[]");

        for (String v : value) {
            shared.stream.writeEmptyElement("add");
            shared.stream.writeAttribute("value", v);
        }

        shared.stream.writeEndElement(); // Array
    }

    private void write(ScilabType value, String as) {

    }
}
