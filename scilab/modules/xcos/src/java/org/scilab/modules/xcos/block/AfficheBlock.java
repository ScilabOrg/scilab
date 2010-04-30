/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.xcos.block;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Collections;
import java.util.Map;
import java.util.WeakHashMap;

import javax.swing.Timer;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.graph.utils.ScilabExported;
import org.scilab.modules.types.scilabTypes.ScilabDouble;
import org.scilab.modules.types.scilabTypes.ScilabList;
import org.scilab.modules.types.scilabTypes.ScilabType;

/**
 * Implement the AFFICH_m block
 */
public final class AfficheBlock extends BasicBlock {

	private static final long serialVersionUID = 6874403612919831380L;

	/**
	 * Map any id to an affiche block instance.
	 * 
	 * This property is linked to the affich2.cpp native implementation
	 */
	private static final Map<Integer, AfficheBlock> INSTANCES = Collections
			.synchronizedMap(new WeakHashMap<Integer, AfficheBlock>());
	
	private static class UpdateValueListener implements ActionListener {
		private final AfficheBlock block;
		private String[][] data;
		
		/**
		 * Default constructor 
		 */
		public UpdateValueListener(AfficheBlock block) {
			this.block = block;
		}
		
		/**
		 * @param data the data to set
		 */
		public synchronized void setData(String[][] data) {
			this.data = data;
		}
		
		/**
		 * Calculate an print the new String value.
		 * 
		 * @param e the current event
		 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
		 */
		@Override
		public synchronized void actionPerformed(ActionEvent e) {
			/*
			 * Construct a String representation of the values.
			 */
			final StringBuilder blockResult = new StringBuilder();
			final int iRows = data.length;
			final int iCols = data.length;

			for (int i = 0; i < iRows; i++) {
				for (int j = 0; j < iCols; j++) {
					if (iCols != 0) {
						blockResult.append("  ");
					}

					blockResult.append(data[i][j]);
				}
				blockResult.append(System.getProperty("line.separator"));
			}
			
			/*
			 * Update and refresh the values
			 */
			block.setValue(blockResult.toString());
			block.getParentDiagram().refresh();
			
			LogFactory.getLog(UpdateValueListener.class).trace(data[0][0]);
		}		
	}
	
	private transient final Timer printTimer;  
	private transient final UpdateValueListener updateAction;
	
	/** Default constructor */
	public AfficheBlock() {
		super();
		
		updateAction = new UpdateValueListener(this);
		printTimer = new Timer(200, updateAction);
		printTimer.setRepeats(false);
	}

	/**
	 * Constructor with label
	 * 
	 * @param value
	 *            the default value.
	 */
	protected AfficheBlock(String value) {
		this();
		setValue(value);
	}

	/**
	 * Assign a value to an AfficheBlock instance
	 * 
	 * @param uid
	 *            the block id
	 * @param value
	 *            the value to set.
	 */
	@ScilabExported(module = "scicos_blocks", filename = "Blocks.giws.xml")
	public static void setValue(final int uid, final String[][] value) {
		if (value.length == 0 || value[0].length == 0) {
			throw new IllegalArgumentException(
					"value is not a non-empty String matrix (String[][])");
		}
		
		final AfficheBlock block = INSTANCES.get(uid);
		
		block.updateAction.setData(value);
		if (!block.printTimer.isRunning()) {
			block.printTimer.start();
		}
	}

	/**
	 * @return the instance UID.
	 */
	@Deprecated
	public synchronized int getHashCode() {
		return hashCode();
	}
	
	/**
	 * @return The scilab formated object parameters
	 */
	@Override
	public ScilabType getObjectsParameters() {
		int id = hashCode();

		/*
		 * As hashCode() may return an already existing id, we need to change it
		 * in this case.
		 * 
		 * see http://java.sun.com/javase/6/docs/api/java/lang/Object.html#hashCode()
		 */
		while (INSTANCES.containsKey(id) && INSTANCES.get(id) != this) {
			 id++;
		}
		
		INSTANCES.put(id, this);
		
		ScilabList list = new ScilabList();
		list.add(new ScilabDouble(id));
		return list;
	}

}
