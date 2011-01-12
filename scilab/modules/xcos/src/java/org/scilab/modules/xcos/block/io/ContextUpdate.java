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

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.io.IOException;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.EnumMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Pattern;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.io.scicos.H5RWHandler;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosEvent;

import com.mxgraph.model.mxGraphModel;
import com.mxgraph.model.mxICell;
import com.mxgraph.model.mxIGraphModel;
import com.mxgraph.util.mxEventObject;

/**
 * Common class for the SuperBlock I/O blocks (represent ports)
 */
public abstract class ContextUpdate extends BasicBlock {

	private static final Log LOG_LOCAL = LogFactory.getLog(ContextUpdate.class);
	private static final long serialVersionUID = 6076826729067963560L;

	/**
	 * Implement a listener which update the value and refresh the view when the
	 * index of the port change.
	 */
	private static final class IndexChangeAdapter implements PropertyChangeListener, Serializable {
		private static IndexChangeAdapter instance;
		
		/**
		 * Default constructor.
		 */
		private IndexChangeAdapter() { }
		
		/**
		 * @return the instance
		 */
		public static synchronized IndexChangeAdapter getInstance() {
			if (instance == null) {
				instance = new IndexChangeAdapter();
			}
			return instance;
		}
		
		/**
		 * Update the value and refresh the graph view.
		 * 
		 * @param evt the event
		 * @see java.beans.PropertyChangeListener#propertyChange(java.beans.PropertyChangeEvent)
		 */
		@Override
		public void propertyChange(PropertyChangeEvent evt) {
			ScilabType data = (ScilabType) evt.getNewValue();
			ContextUpdate ioBlock = (ContextUpdate) evt.getSource();
			
			if (!data.isEmpty()) {
				int newIndex = (int) ((ScilabDouble) data).getRealPart()[0][0];
				
				int oldIndex;
				if (evt.getOldValue() instanceof ScilabDouble
						&& !((ScilabDouble) evt.getOldValue()).isEmpty()) {
					oldIndex = (int) ((ScilabDouble) evt.getOldValue()).getRealPart()[0][0];
				} else {
					oldIndex = -1;
				}
				
				ioBlock.setValue(newIndex);
				
				if (ioBlock.getParentDiagram() != null) {
					ioBlock.getParentDiagram().fireEvent(
							new mxEventObject(XcosEvent.IO_PORT_VALUE_UPDATED,
									"block", ioBlock, "oldIndex", oldIndex,
									"newIndex", newIndex));
				}
			}
		}
		
	}
	
	/**
	 * Implement a listener to update the {@link ContextUpdate#isContextDependent} flag.
	 */
	private static final class ExprsChangeAdapter implements PropertyChangeListener, Serializable {
		private static final Pattern INTEGER_PATTERN = Pattern.compile("\\d+");

		private static ExprsChangeAdapter instance;
		
		/**
		 * Default constructor
		 */
		public ExprsChangeAdapter() { }
		
		/**
		 * @return the shared instance
		 */
		public static ExprsChangeAdapter getInstance() {
			if (instance == null) {
				instance = new ExprsChangeAdapter();
			}
			return instance;
		}
		
		/**
		 * isContextDependant field
		 * 
		 * @param evt the event
		 * @see java.beans.PropertyChangeListener#propertyChange(java.beans.PropertyChangeEvent)
		 */
		@Override
		public void propertyChange(final PropertyChangeEvent evt) {
			final ScilabType data = (ScilabType) evt.getNewValue();
			final ContextUpdate ioBlock = (ContextUpdate) evt.getSource();
			
			if (!data.isEmpty()) {
				final String newIndex = ((ScilabString) data).getData()[0][0];
				
				if (!INTEGER_PATTERN.matcher(newIndex).matches()) {
					ioBlock.isContextDependent = true;
				} else {
					ioBlock.isContextDependent = false;
				}
			}
		}
	}
	
	/**
	 * This enum represent all the subclasses of ContextUpdate .
	 * 
	 * It is used to easily loop over a BasicBlock I/O blocks
	 */
	public static enum IOBlocks {
		/** Map a control port to an event input block */
		EventInBlock(EventInBlock.class, ControlPort.class),
		/** Map a command port to an event output block */
		EventOutBlock(EventOutBlock.class, CommandPort.class),
		/** Map an explicit input port to an explicit input block */
		ExplicitInBlock(ExplicitInBlock.class, ExplicitInputPort.class),
		/** Map an explicit output port to an explicit output block */
		ExplicitOutBlock(ExplicitOutBlock.class, ExplicitOutputPort.class),
		/** Map an implicit input port to an implicit input block */
		ImplicitInBlock(ImplicitInBlock.class, ImplicitInputPort.class),
		/** Map an implicit output port to an implicit output block */
		ImplicitOutBlock(ImplicitOutBlock.class, ImplicitOutputPort.class);

		private final Class< ? extends ContextUpdate> ioBlock;
		private final Class< ? extends BasicPort> port;

		/**
		 * @param ioBlock
		 *            input/output block
		 * @param port
		 *            the associated port class
		 */
		private IOBlocks(Class< ? extends ContextUpdate> ioBlock,
				Class< ? extends BasicPort> port) {
			this.ioBlock = ioBlock;
			this.port = port;
		}
		
		/**
		 * Get all the port of the SuperBlock parent.
		 * 
		 * @param parent
		 *            the parent
		 * @return the port list mapped by port type
		 */
		public static Map<IOBlocks, List<mxICell>> getAllPorts(SuperBlock parent) {
			final EnumMap<IOBlocks, List<mxICell>> ret = new EnumMap<IOBlocks, List<mxICell>>(
					IOBlocks.class);

			/* Allocation */
			for (IOBlocks b : IOBlocks.values()) {
				ret.put(b, new ArrayList<mxICell>());
			}

			/* Loop all over the children */
			final int childCount = parent.getChildCount();

			for (int i = 0; i < childCount; i++) {
				final mxICell child = parent.getChildAt(i);

				/* if compatible add it to the list */
				for (IOBlocks b : IOBlocks.values()) {
					if (child.getClass().equals(b.getReferencedPortClass())) {
						ret.get(b).add(child);
					}
				}
			}

			return ret;
		}
		
		public static List<mxICell> getPorts(SuperBlock parent, Class<? extends ContextUpdate> klass) {
			List<mxICell> ret = new ArrayList<mxICell>();
			
			/* Get the corresponding klass */
			Class< ? extends BasicPort> portKlass = null; 
			for (IOBlocks b : IOBlocks.values()) {
				if (b.getReferencedClass().equals(klass)) {
					portKlass = b.getReferencedPortClass();
					break;
				}
			}
			
			/* Loop all over the children */
			final int childCount = parent.getChildCount();

			for (int i = 0; i < childCount; i++) {
				final mxICell child = parent.getChildAt(i);
				if (portKlass.isInstance(child)) {
					ret.add(child);
				}
			}
			
			return ret;
		}
		
		/**
		 * Get all the I/O blocks of the SuperBlock parent.
		 * 
		 * @param parent
		 *            the parent
		 * @return the port list mapped by port type
		 */
		public static Map<IOBlocks, List<mxICell>> getAllBlocks(SuperBlock parent) {
			final EnumMap<IOBlocks, List<mxICell>> ret = new EnumMap<IOBlocks, List<mxICell>>(
					IOBlocks.class);

			final SuperBlockDiagram graph = parent.getChild();
			if (graph == null) {
				parent.createChildDiagram();
			}
			
			/* Allocation */
			for (IOBlocks b : IOBlocks.values()) {
				ret.put(b, new ArrayList<mxICell>());
			}

			/* Loop all over the children */
			final Object defaultParent = graph.getDefaultParent();
			final mxIGraphModel defaultModel = graph.getModel();
			final Object[] children = mxGraphModel.getChildCells(
					defaultModel, defaultParent, true, false);
			
			for (Object element : children) {
				final mxICell child = (mxICell) element;

				/* if compatible add it to the list */
				for (IOBlocks b : IOBlocks.values()) {
					if (child.getClass().equals(b.getReferencedClass())) {
						ret.get(b).add(child);
					}
				}
			}

			return ret;
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
	
	private transient boolean isContextDependent;
	
	/**
	 * Constructor.
	 */
	public ContextUpdate() {
		super();
		
		getParametersPCS().addPropertyChangeListener(INTEGER_PARAMETERS,
				IndexChangeAdapter.getInstance());
		getParametersPCS().addPropertyChangeListener(EXPRS,
				ExprsChangeAdapter.getInstance());
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
	 * @param context
	 *            new context
	 */
	public void onContextChange(String[] context) {
		// prevent to open twice
		if (isLocked()) {
			return;
		}

		// do not evaluate context is the block is not context dependent.
		if (!isContextDependent) {
			return;
		}
		
		LOG_LOCAL.trace("Update the I/O value from the context");
		
		File tempOutput;
		File tempInput;
		File tempContext;
		try {
			tempInput = FileUtils.createTempFile();
			tempInput.deleteOnExit();

			// Write scs_m
			tempOutput = exportBlockStruct();
			// Write context
			tempContext = exportContext(context);

			String cmd = ScilabInterpreterManagement.buildCall("xcosBlockEval",
					tempOutput.getAbsolutePath(), tempInput.getAbsolutePath(),
					getInterfaceFunctionName().toCharArray(), tempContext.getAbsolutePath());

			try {
				ScilabInterpreterManagement.synchronousScilabExec(cmd);
			} catch (InterpreterException e) {
				e.printStackTrace();
			}
			BasicBlock modifiedBlock = new H5RWHandler(tempInput).readBlock();
			updateBlockSettings(modifiedBlock);

		} catch (IOException e) {
			LOG_LOCAL.error(e);
		}
	}
}
