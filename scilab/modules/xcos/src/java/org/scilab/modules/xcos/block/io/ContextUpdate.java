/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.block.io;

import java.io.File;
import java.io.IOException;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.graph.utils.ScilabInterpreterManagement;
import org.scilab.modules.graph.utils.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.types.scilabTypes.ScilabDouble;
import org.scilab.modules.types.scilabTypes.ScilabList;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.io.scicos.H5RWHandler;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.utils.XcosConstants;

/**
 * Abstract super class of all blocks which need to be updated after a context
 * change.
 */
public abstract class ContextUpdate extends BasicBlock {

    /**
     * This enum represent all the subclasses of ContextUpdate .
     *
     * It is used to easily loop over a BasicBlock I/O blocks
     */
    public enum IOBlocks {
	EventInBlock(EventInBlock.class, ControlPort.class),
	EventOutBlock(EventOutBlock.class, CommandPort.class),
	ExplicitInBlock(ExplicitInBlock.class, ExplicitInputPort.class),
	ExplicitOutBlock(ExplicitOutBlock.class, ExplicitOutputPort.class),
	ImplicitInBlock(ImplicitInBlock.class, ImplicitInputPort.class),
	ImplicitOutBlock(ImplicitOutBlock.class, ImplicitOutputPort.class);
	
	private final Class< ? extends ContextUpdate> ioBlock;
	private final Class< ? extends BasicPort> port;
	
	/**
	 * @param ioBlock input/output block
	 * @param port the associated port class
	 */
	private IOBlocks(Class< ? extends ContextUpdate> ioBlock, Class< ? extends BasicPort> port) {
	    this.ioBlock = ioBlock;
	    this.port = port;
	}
	
	/**
	 * @return referenced class
	 */
	public Class< ? extends ContextUpdate> getReferencedClass() {
	    return ioBlock;
	}
	
	/**
	 * @return the port referenced class
	 */
	public Class< ? extends BasicPort> getReferencedPortClass() {
	    return port;
	}
	
    }
    
	/**
	 * Constructor
	 */
	public ContextUpdate() {
		super();
	}

	/**
	 * @param label block labek
	 */
	protected ContextUpdate(String label) {
		this();
		setValue(label);
	}

	/**
	 * Initialize the block with the default values
	 */
	@Override
	protected void setDefaultValues() {
		super.setDefaultValues();
		setNbZerosCrossing(new ScilabDouble(0));
		setNmode(new ScilabDouble(0));
		setODState(new ScilabList());
		setValue(1);
	}
    
    /**
     * Update the block with the associated context
     * @param context new context
     */
    public void onContextChange(String[] context) {
		try {
			final File tempOutput = File.createTempFile(INTERNAL_FILE_PREFIX, INTERNAL_FILE_EXTENSION, XcosConstants.TMPDIR);
			final File tempInput = File.createTempFile(INTERNAL_FILE_PREFIX, INTERNAL_FILE_EXTENSION, XcosConstants.TMPDIR);
			final File tempContext = File.createTempFile(INTERNAL_FILE_PREFIX, INTERNAL_FILE_EXTENSION, XcosConstants.TMPDIR);
			
		    // Write block
			new H5RWHandler(tempOutput).writeBlock(this);
		    // Write context
			new H5RWHandler(tempContext).writeContext(context);
	
		    String cmd = ScilabInterpreterManagement.buildCall("xcosBlockEval", 
		    		tempOutput.getAbsolutePath(),
		    		tempInput.getAbsolutePath(),
		    		getInterfaceFunctionName(),
		    		tempContext.getAbsolutePath());
	
			try {
				ScilabInterpreterManagement.synchronousScilabExec(cmd);
			} catch (InterpreterException e) {
				LogFactory.getLog(ContextUpdate.class).error(e);
			}
			BasicBlock modifiedBlock = new H5RWHandler(tempInput).readBlock();
			updateBlockSettings(modifiedBlock);
		    
		    tempInput.delete();
		    tempOutput.delete();
		    tempContext.delete();
			
		} catch (IOException e) {
			LogFactory.getLog(ContextUpdate.class).error(e);
		}
    }
}
