/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.io.scicos;

import java.util.LinkedHashMap;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.javasci.Call_Scilab;
import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.JavasciException.InitializationException;
import org.scilab.modules.javasci.Scilab;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabTypeEnum;
import org.scilab.modules.types.ScilabVariables;
import org.scilab.modules.types.ScilabVariablesHandler;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.VersionMismatchException;
import org.scilab.modules.xcos.utils.XcosMessages;


/**
 * Scilab data direct access.
 */
public class ScilabDirectHandler implements Handler {
    private static final String CONTEXT = "context";
    private static final String SCS_M = "scs_m";
    private static final Logger LOG = Logger.getLogger(ScilabDirectHandler.class.getPackage().getName());

    //	private final Scilab sci;

    public ScilabDirectHandler() {
        //		try {
        //			sci = new Scilab();
        //		} catch (InitializationException e) {
        //			throw new RuntimeException(e);
        //		}
    }

    @Override
    protected void finalize() throws Throwable {
        //		sci.close();
    }

    @Override
    public BasicBlock readBlock() throws ScicosFormatException {
        return readBlock(null);
    }

    @Override
    public synchronized BasicBlock readBlock(BasicBlock into) throws ScicosFormatException {
        throw new UnsupportedOperationException();
    }

    @Override
    public synchronized Map<String, String> readContext() {
        throw new UnsupportedOperationException();
    }

    @Override
    public XcosDiagram readDiagram() throws VersionMismatchException {
        return readDiagram(null);
    }

    @Override
    public synchronized XcosDiagram readDiagram(XcosDiagram instance) {
        throw new UnsupportedOperationException();
    }

    @Override
    public void writeBlock(BasicBlock block) {
        LOG.entering("ScilabDirectHandler", "writeBlock");

        final BlockElement element = new BlockElement();
        final ScilabType data = element.encode(block, null);

        LOG.finer("encoding done");

        try {
            Scilab.put(SCS_M, data);
        } catch (JavasciException e) {
            throw new RuntimeException(e);
        }

        LOG.exiting("ScilabDirectHandler", "writeBlock");
    }

    @Override
    public void writeContext(String[] context) {
        LOG.entering("ScilabDirectHandler", "writeContext");

        try {
            Scilab.put(CONTEXT, new ScilabString(context));
        } catch (JavasciException e) {
            throw new RuntimeException(e);
        }

        LOG.exiting("ScilabDirectHandler", "writeContext");
    }

    @Override
    public void writeDiagram(XcosDiagram diagram) {
        LOG.entering("ScilabDirectHandler", "writeDiagram");

        final DiagramElement element = new DiagramElement();
        final ScilabType data = element.encode(diagram, null);

        LOG.finer("encoding done");

        try {
            Scilab.put(SCS_M, data);
        } catch (JavasciException e) {
            throw new RuntimeException(e);
        }

        LOG.exiting("ScilabDirectHandler", "writeDiagram");
    }
}
