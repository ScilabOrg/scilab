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

import java.util.Collections;
import java.util.EnumMap;
import java.util.Map;

import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLStreamWriter;

import org.scilab.modules.types.ScilabList;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.io.HandledElementsCategory;
import org.scilab.modules.xcos.utils.Stack;

/**
 * Implement an XML writer for any kind of model object
 */
public class XcosWriter {

    /*
     * Shared data
     */
    protected final JavaController controller;
    protected final ScilabList dictionary;
    protected final XMLStreamWriter stream;
    protected final Stack<String> layers;

    protected final Map<HandledElementsCategory, ScilabWriter> writers;

    public XcosWriter(ScilabList dictionary, XMLStreamWriter writer) {
        this.controller = new JavaController();
        this.dictionary = dictionary;
        this.stream = writer;

        this.layers = new Stack<>();

        // add all the known writers to the map
        EnumMap<HandledElementsCategory, ScilabWriter> localWriters = new EnumMap<>(HandledElementsCategory.class);
        localWriters.put(HandledElementsCategory.JGRAPHX, new JGraphXWriter(this));
        localWriters.put(HandledElementsCategory.BLOCK, new BlockWriter(this));
        localWriters.put(HandledElementsCategory.LINK, new LinkWriter(this));
        localWriters.put(HandledElementsCategory.PORT, new PortWriter(this));
        localWriters.put(HandledElementsCategory.RAW_DATA, new RawDataWriter(this));
        localWriters.put(HandledElementsCategory.CUSTOM, new CustomWriter(this));

        this.writers = Collections.unmodifiableMap(localWriters);
    }

    public void write(long uid, Kind kind) throws XMLStreamException {
        switch (kind) {
            case ANNOTATION:
            case BLOCK:
                writers.get(HandledElementsCategory.BLOCK).write(uid, kind);
                break;
            case DIAGRAM:
                writers.get(HandledElementsCategory.CUSTOM).write(uid, kind);
                // post processing on the for binary data handling
                ((RawDataWriter) writers.get(HandledElementsCategory.RAW_DATA)).fillSharedDictionary();
                break;
            case LINK:
                writers.get(HandledElementsCategory.LINK).write(uid, kind);
                break;
            case PORT:
                writers.get(HandledElementsCategory.PORT).write(uid, kind);
                break;
        }
    }
}
