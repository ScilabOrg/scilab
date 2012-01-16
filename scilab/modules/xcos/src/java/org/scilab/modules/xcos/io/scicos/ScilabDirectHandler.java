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
public class ScilabDirectHandler implements Handler, ScilabVariablesHandler {
	private static final Logger LOG = Logger.getLogger(ScilabDirectHandler.class.getPackage().getName());
	
	final private int descriptor;
	final private String name;
	final private ScilabTypeEnum type;
	
	private ScilabType data;
	
	public ScilabDirectHandler(final String name, final ScilabTypeEnum type) {
		descriptor = ScilabVariables.addScilabVariablesHandler(this);
		this.name = name;
		this.type = type;
		
		data = null;
	}
	
	@Override
	protected void finalize() throws Throwable {
		ScilabVariables.removeScilabVariablesHandler(descriptor);
		
		super.finalize();
	}
	
	@Override
	public BasicBlock readBlock() throws ScicosFormatException {
		return readBlock(null);
	}

	@Override
	public synchronized BasicBlock readBlock(BasicBlock into) throws ScicosFormatException {
        final BlockElement element = new BlockElement();
        BasicBlock instance;

        if (LOG.isLoggable(Level.INFO)) {
            LOG.info("Reading block from " + descriptor);
        }

        try {
        	// first wait for data on the requester thread
            wait();
        	
			// At this point data is available
            instance = element.decode(data, into);
            StyleMap style = new StyleMap(instance.getInterfaceFunctionName());
            style.putAll(instance.getStyle());

            instance.setStyle(style.toString());

        } catch (InterruptedException e) {
            LOG.severe(e.toString());
            instance = null;
        }

        if (LOG.isLoggable(Level.INFO)) {
            LOG.info("End of reading block from " + descriptor);
        }

        return instance;
	}

	@Override
	public synchronized Map<String, String> readContext() {
        ScilabList list = new ScilabList();
        final Map<String, String> result = new LinkedHashMap<String, String>();

        if (LOG.isLoggable(Level.INFO)) {
            LOG.info("Reading context from " + descriptor);
        }

        try {
        	// first wait for data on the requester thread
        	wait();

			// At this point data is available
        	list = (ScilabList) data;
            
        } catch (InterruptedException e) {
            LOG.severe(e.toString());
            return result;
        }

        // We are starting at 2 because a struct is composed of
        // - the fields names (ScilabString)
        // - the dimension
        // - variables values...
        for (int index = 2; index < list.size(); index++) {
            String key = ((ScilabString) list.get(0)).getData()[0][index];
            String value = list.get(index).toString();

            result.put(key, value);
        }

        if (LOG.isLoggable(Level.INFO)) {
            LOG.info("End of reading context from " + descriptor);
        }

        return result;
	}

	@Override
	public XcosDiagram readDiagram() throws VersionMismatchException {
		return readDiagram(null);
	}

	@Override
	public synchronized XcosDiagram readDiagram(XcosDiagram instance) {
        final DiagramElement element = new DiagramElement();
        
        final XcosDiagram diagram;
        if (instance == null) {
            diagram = new XcosDiagram();
        } else {
            diagram = instance;
        }

        if (LOG.isLoggable(Level.INFO)) {
            LOG.info("Reading diagram from " + descriptor);
        }

        diagram.getModel().beginUpdate();
        try {
        	// first wait for data on the requester thread
			wait();
			
			// At this point data is available
			element.decode(data, diagram);

		} catch (VersionMismatchException e) {
            /*
             * On version mismatch alert the user but the current instance
             * contains the partially decoded data so continue.
             */
            diagram.error(XcosMessages.UNKNOW_VERSION
                    + ((VersionMismatchException) e).getWrongVersion()
                    + "\n" + XcosMessages.TRY_TO_CONTINUE);
		} catch (ScicosFormatException e) {
			// rethrow
            throw new RuntimeException(e);
		} catch (InterruptedException e) {
			LOG.severe(e.toString());
		} finally {
			diagram.getModel().endUpdate();
		}
        
        if (LOG.isLoggable(Level.INFO)) {
            LOG.info("End of reading diagram from " + descriptor);
        }
        
		return diagram;
	}

	@Override
	public void writeBlock(BasicBlock block) {
		throw new UnsupportedOperationException();
	}

	@Override
	public void writeContext(String[] context) {
		throw new UnsupportedOperationException();
	}

	@Override
	public void writeDiagram(XcosDiagram diagram) {
		throw new UnsupportedOperationException();
	}

	/*
	 * ScilabVariablesHandler implementation
	 */
	
	@Override
	public synchronized void handle(ScilabType var) {
		if (var.getVarName().equals(name) && var.getType().equals(type)) {
			data = var;
			
			notify();
		}
	}
}
