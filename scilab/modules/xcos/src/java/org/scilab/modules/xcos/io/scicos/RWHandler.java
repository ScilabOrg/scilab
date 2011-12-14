/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.io.scicos;

import java.io.File;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;

import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.hdf5.read.H5Read;
import org.scilab.modules.hdf5.write.H5Write;
import org.scilab.modules.javasci.ScilabVariablesJavasci;
import org.scilab.modules.types.GetScilabVariable;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabVariables;
import org.scilab.modules.types.ScilabVariablesHandler;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.VersionMismatchException;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Implement useful methods to easily import or export Scicos data.
 */
// CSOFF: ClassDataAbstractionCoupling
public class RWHandler {
    private static final String CONTEXT = "context";
    private static final String SCS_M = "scs_m";
    private static final Log LOG = LogFactory.getLog(RWHandler.class);

    private static int id = -1;
    private static final Map<Thread, ScilabType> map = new HashMap<Thread, ScilabType>();
    
    private final String h5File;

    /**
     * Construct a new instance with the file path.
     * 
     * @param hdf5file
     *            the file path.
     */
    public RWHandler(String hdf5file) {
        h5File = hdf5file;
    }

    /*
     * Utilities
     */
	private ScilabType getScilabVariable(String variableName) {
		final Thread t = Thread.currentThread();
		
		if (id == -1) {
			id = ScilabVariables
					.addScilabVariablesHandler(new ScilabVariablesHandler() {
						@Override
						public void handle(ScilabType var) {
							map.put(t, var);
						}
					});
		}

		GetScilabVariable.getScilabVariable(variableName, 1, id);
		final ScilabType data = map.get(t);
		map.remove(t);
		return data;
	}
    
    /*
     * Read methods
     */

    /**
     * Decode an Xcos block
     * 
     * @return the decoded block
     * @throws ScicosFormatException
     *             on decoding error
     */
    public BasicBlock readBlock() throws ScicosFormatException {
        return readBlock(null);
    }

    /**
     * Decode an Xcos block into an instance
     * 
     * @param into
     *            the instance to update
     * @return the updated instance.
     * @throws ScicosFormatException
     *             on decoding error
     */
	public BasicBlock readBlock(BasicBlock into) throws ScicosFormatException {
		final BlockElement element = new BlockElement();
		BasicBlock instance;

		if (LOG.isTraceEnabled()) {
			LOG.trace("Reading block from " + h5File);
		}

		final ScilabType data = getScilabVariable(SCS_M);
		instance = element.decode(data, into);
		
		StyleMap style = new StyleMap(instance.getInterfaceFunctionName());
		style.putAll(instance.getStyle());
		instance.setStyle(style.toString());

		if (LOG.isTraceEnabled()) {
			LOG.trace("End of reading block from " + h5File);
		}

		return instance;
	}

    /**
     * Decode an evaluated Xcos context
     * 
     * @return the decoded context
     */
    public Map<String, String> readContext() {
        final ScilabList list;
        final Map<String, String> result = new LinkedHashMap<String, String>();

        if (LOG.isTraceEnabled()) {
            LOG.trace("Reading context from " + h5File);
        }

        final ScilabType data = getScilabVariable(CONTEXT);
        if (data instanceof ScilabList) {
        	list = (ScilabList) data;
        } else {
        	list = new ScilabList();
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

        if (LOG.isTraceEnabled()) {
            LOG.trace("End of reading context from " + h5File);
        }

        return result;
    }

    /**
     * Decode an Xcos diagram from an H5 file
     * 
     * @return the decoded diagram
     * @throws VersionMismatchException
     *             when the diagram version mismatch
     */
    public XcosDiagram readDiagram() throws VersionMismatchException {
        return readDiagram(null);
    }

    /**
     * Decode an Xcos diagram from an H5 file in place.
     * 
     * @param instance
     *            the previously allocated diagram where to decode.
     * @return the decoded diagram
     */
    public XcosDiagram readDiagram(XcosDiagram instance) {
        final DiagramElement element = new DiagramElement();

        XcosDiagram diagram;
        if (instance == null) {
            diagram = new XcosDiagram();
        } else {
            diagram = instance;
        }

        diagram.getModel().beginUpdate();

        if (LOG.isTraceEnabled()) {
            LOG.trace("Reading diagram from " + h5File);
        }

        try {
        	
    		final ScilabType data = getScilabVariable(SCS_M);
    		instance = element.decode(data, diagram);
        } catch (ScicosFormatException e) {
            if (e instanceof VersionMismatchException) {
                /*
                 * On version mismatch alert the user but the current instance
                 * contains the partially decoded data so continue.
                 */
                diagram.error(XcosMessages.UNKNOW_VERSION
                        + ((VersionMismatchException) e).getWrongVersion()
                        + "\n" + XcosMessages.TRY_TO_CONTINUE);
            } else {
                // rethrow
                throw new RuntimeException(e);
            }
        } finally {
            diagram.getModel().endUpdate();
        }

        if (LOG.isTraceEnabled()) {
            LOG.trace("End of reading diagram from " + h5File);
        }

        return diagram;
    }

    /*
     * Write methods
     */

    /**
     * Encode an Xcos block
     * 
     * @param block
     *            the block
     */
    public void writeBlock(BasicBlock block) {
        final BlockElement element = new BlockElement();
        final ScilabType data = element.encode(block, null);

        if (LOG.isTraceEnabled()) {
            LOG.trace("Writing block to " + h5File);
        }

        try {
            int fileId = H5Write.createFile(h5File);

            H5Write.writeInDataSet(fileId, SCS_M, data);

            H5Write.closeFile(fileId);
        } catch (HDF5Exception e) {
            LOG.error(e);
        } catch (java.lang.NullPointerException e) {
            LOG.error(e);
            LOG.debug(data);
        } catch (java.lang.IndexOutOfBoundsException e) {
            LOG.error(e);
            LOG.debug(data);
        }

        if (LOG.isTraceEnabled()) {
            LOG.trace("End of writing block to " + h5File);
        }
    }

    /**
     * Encode an Xcos context
     * 
     * @param context
     *            the context
     */
    public void writeContext(String[] context) {
        final ScilabString string = new ScilabString(context);

        if (LOG.isTraceEnabled()) {
            LOG.trace("Writing context to " + h5File);
        }

        try {
            int fileId = H5Write.createFile(h5File);

            H5Write.writeInDataSet(fileId, CONTEXT, string);

            H5Write.closeFile(fileId);
        } catch (HDF5Exception e) {
            LOG.error(e);
        }

        if (LOG.isTraceEnabled()) {
            LOG.trace("End of writing context to " + h5File);
        }
    }

    /**
     * Encode an Xcos diagram
     * 
     * @param diagram
     *            the block
     */
    public void writeDiagram(XcosDiagram diagram) {
        final DiagramElement element = new DiagramElement();
        final ScilabType data = element.encode(diagram, null);

        if (LOG.isTraceEnabled()) {
            LOG.trace("Writing diagram to " + h5File);
        }

        try {
            int fileId = H5Write.createFile(h5File);

            H5Write.writeInDataSet(fileId, SCS_M, data);

            H5Write.closeFile(fileId);
        } catch (HDF5Exception e) {
            // important error which need a backtrace.
            LOG.error(e);
            e.printStackTrace();
        } catch (java.lang.NullPointerException e) {
            LOG.error(e);
            LOG.debug(data);
        } catch (java.lang.IndexOutOfBoundsException e) {
            LOG.error(e);
            LOG.debug(data);
        }

        if (LOG.isTraceEnabled()) {
            LOG.trace("End of writing diagram to " + h5File);
        }
    }
}
// CSON: ClassDataAbstractionCoupling
