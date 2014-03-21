/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.io.scicos;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.SortedMap;
import java.util.TreeMap;

import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.VersionMismatchException;

/**
 * Output the Scilab graph mlists as text (like .cosf files)
 *
 */
public class ScilabTextHandler implements Handler {
    private static final String INDENT_STRING = "    ";
    private static final TreeMap<String, String> TYPE_CONSTRUCTOR_MAPPING;

    static {
        TYPE_CONSTRUCTOR_MAPPING = new TreeMap<String, String>();
        TYPE_CONSTRUCTOR_MAPPING.put("diagram", "scicos_diagram");
        TYPE_CONSTRUCTOR_MAPPING.put("graphics", "scicos_graphics");
        TYPE_CONSTRUCTOR_MAPPING.put("model", "scicos_model");
        TYPE_CONSTRUCTOR_MAPPING.put("params", "scicos_params");
        TYPE_CONSTRUCTOR_MAPPING.put("Block", "scicos_block");
        TYPE_CONSTRUCTOR_MAPPING.put("Link", "scicos_link");
    }

    int indent = 0;
    final ArrayDeque<String> currentPath = new ArrayDeque<String>();
    final TreeMap<String, ScilabType> delayedWriting = new TreeMap<String, ScilabType>();
    final File f;

    /**
     * Default constructor
     *
     * @param f
     *            where to read/write data
     */
    public ScilabTextHandler(final File f) {
        this.f = f;
    }

    /*
     * (non-Javadoc)
     *
     * @see org.scilab.modules.xcos.io.scicos.Handler#readBlock()
     */
    @Override
    public BasicBlock readBlock() throws ScicosFormatException {
        return readBlock(null);
    }

    /*
     * (non-Javadoc)
     *
     * @see
     * org.scilab.modules.xcos.io.scicos.Handler#readBlock(org.scilab.modules
     * .xcos.block.BasicBlock)
     */
    @Override
    public BasicBlock readBlock(BasicBlock into) throws ScicosFormatException {
        // TODO Auto-generated method stub
        return null;
    }

    /*
     * (non-Javadoc)
     *
     * @see org.scilab.modules.xcos.io.scicos.Handler#readContext()
     */
    @Override
    public Map<String, String> readContext() {
        // TODO Auto-generated method stub
        return null;
    }

    /*
     * (non-Javadoc)
     *
     * @see org.scilab.modules.xcos.io.scicos.Handler#readDiagram()
     */
    @Override
    public XcosDiagram readDiagram() throws VersionMismatchException {
        // TODO Auto-generated method stub
        return null;
    }

    /*
     * (non-Javadoc)
     *
     * @see
     * org.scilab.modules.xcos.io.scicos.Handler#readDiagram(org.scilab.modules
     * .xcos.graph.XcosDiagram)
     */
    @Override
    public XcosDiagram readDiagram(XcosDiagram instance) {
        // TODO Auto-generated method stub
        return null;
    }

    /*
     * (non-Javadoc)
     *
     * @see
     * org.scilab.modules.xcos.io.scicos.Handler#writeBlock(org.scilab.modules
     * .xcos.block.BasicBlock)
     */
    @Override
    public void writeBlock(BasicBlock block) {
        // TODO Auto-generated method stub

    }

    /*
     * (non-Javadoc)
     *
     * @see
     * org.scilab.modules.xcos.io.scicos.Handler#writeContext(java.lang.String
     * [])
     */
    @Override
    public void writeContext(String[] context) {
        // TODO Auto-generated method stub

    }

    /*
     * (non-Javadoc)
     *
     * @see
     * org.scilab.modules.xcos.io.scicos.Handler#writeDiagram(org.scilab.modules
     * .xcos.graph.XcosDiagram)
     */
    @Override
    public void writeDiagram(XcosDiagram diagram) {
        final DiagramElement element = new DiagramElement();
        final ScilabType data = element.encode(diagram, null);

        try {
            final FileWriter writer = new FileWriter(f);

            indent = 0;
            currentPath.addLast("scs_m");
            writer.append("scs_m = ");
            write(data, writer);
            writer.append(";\n");

            writeDelayedData(writer);

            writer.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /*
     * Internal methods
     */
    private void write(ScilabType data, FileWriter writer) throws IOException {
        if (data instanceof ScilabList) {
            writeList((ScilabList) data, writer);
        } else if (data instanceof ScilabMList || data instanceof ScilabTList) {
            writeStruct(data, writer);
        } else {
            writer.append(data.toString());
        }
    }

    private void writeList(ScilabList data, FileWriter writer) throws IOException {
        writer.append("list(");

        if (data.size() > 0 && data.get(0) instanceof ScilabMList) {
            // specific case, instantiate and insert later
            final StringBuilder path = new StringBuilder();
            for (String str : currentPath) {
                path.append(str);
            }
            delayedWriting.put(path.toString(), data);
        } else {
            for (int i = 0; i < data.size(); i++) {
                write(data.get(i), writer);
                if (i < data.size() - 1) {
                    writer.append(", ");
                }
            }
        }
        writer.append(")");
    }

    private void writeStruct(ScilabType data, FileWriter writer) throws IOException {
        final ArrayList<ScilabType> typedData = (ArrayList<ScilabType>) data;

        final ScilabString header = (ScilabString) typedData.get(0);
        final String typeName = header.getData()[0][0];

        final String functor = TYPE_CONSTRUCTOR_MAPPING.get(typeName);
        if (functor != null) {
            writer.append(functor).append("(..\n");
            indent++;

            int n_fields = header.getHeight() * header.getWidth() - 1;
            for (int i = 1; i <= n_fields; i++) {
                final String fieldName = header.getData()[0][i];
                final ScilabType fieldValue = typedData.get(i);

                indent(writer);
                writer.append(fieldName);
                writer.append('=');

                currentPath.addLast(".");
                currentPath.addLast(fieldName);
                write(fieldValue, writer);
                currentPath.removeLast();
                currentPath.removeLast();

                if (i < n_fields) {
                    writer.append(",..\n");
                }
            }

            writer.append(")");
            indent--;
        } else { // there is no mapping for this mlist, fallqback
            writer.append(data.toString());
        }
    }

    private void writeDelayedData(FileWriter writer) throws IOException {
        currentPath.clear();

        for (Map.Entry<String, ScilabType> e = delayedWriting.pollFirstEntry(); e != null; e = delayedWriting.pollFirstEntry()) {
            final ScilabList list = (ScilabList) e.getValue();
            for (int i = 0; i < list.size(); i++) {
                final String path = e.getKey() + "(" + (i + 1) + ")";

                writer.append(path);
                writer.append('=');

                currentPath.addLast(path);
                write(list.get(i), writer);
                currentPath.removeLast();

                writer.append(";\n");
            }
        }
    }

    private void indent(FileWriter writer) throws IOException {
        for (int i = 0; i < indent; i++) {
            writer.append(INDENT_STRING);
        }
    }
}
