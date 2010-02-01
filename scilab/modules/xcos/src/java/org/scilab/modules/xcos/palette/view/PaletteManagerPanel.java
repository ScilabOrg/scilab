/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.palette.view;

import java.awt.Color;

import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTree;
import javax.swing.tree.TreeSelectionModel;

import org.scilab.modules.xcos.palette.PaletteManager;
import org.scilab.modules.xcos.palette.listener.PaletteManagerMouseListener;
import org.scilab.modules.xcos.palette.listener.PaletteManagerTreeSelectionListener;
import org.scilab.modules.xcos.palette.model.PaletteManagerModel;
import org.scilab.modules.xcos.utils.XcosConstants;

/**
 * The content pane for the block view
 */
public class PaletteManagerPanel extends JSplitPane {

	private static final double DEFAULT_WEIGHT = 0;
	private PaletteManager controller;
	
	/**
	 * Default constructor
	 * @param controller the {@link PaletteManager} instance
	 */
	public PaletteManagerPanel(PaletteManager controller) {
		super(JSplitPane.HORIZONTAL_SPLIT);
		this.controller = controller;
		fillUpContentPane();
	}
	
	/**
	 * Fill up the content pane
	 */
	private void fillUpContentPane() {
		/** Default instances */
		JScrollPane panel = new JScrollPane();
		initJScrollPane(panel);
		
		// Set default left component
		JPanel rootPalette = new JPanel();
		
		PaletteManagerModel model = controller.getModel();
		JTree tree = new JTree(model.getTreeModel());
		
		/** Setup tree */
		tree.getSelectionModel().setSelectionMode(
				TreeSelectionModel.SINGLE_TREE_SELECTION);
		tree.addMouseListener(new PaletteManagerMouseListener());
		tree.addTreeSelectionListener(new PaletteManagerTreeSelectionListener(panel));
		
		setLeftComponent(tree);
		panel.setViewportView(rootPalette);
		setRightComponent(panel);
	}

	/**
	 * Init the ScrollPane component
	 * @param panel the component
	 */
	private void initJScrollPane(JScrollPane panel) {
		panel.setBackground(Color.WHITE);
		panel.getVerticalScrollBar().setBlockIncrement(
				XcosConstants.PALETTE_BLOCK_HEIGHT
						+ XcosConstants.PALETTE_VMARGIN);
		panel.getVerticalScrollBar().setUnitIncrement(
				XcosConstants.PALETTE_BLOCK_HEIGHT
						+ XcosConstants.PALETTE_VMARGIN);

		panel.getHorizontalScrollBar().setBlockIncrement(
				XcosConstants.PALETTE_BLOCK_WIDTH
						+ XcosConstants.PALETTE_HMARGIN);
		panel.getHorizontalScrollBar().setUnitIncrement(
				XcosConstants.PALETTE_BLOCK_WIDTH
						+ XcosConstants.PALETTE_HMARGIN);
	}

	/**
	 * Setup the default layout 
	 */
	public void performStartUpLayout() {
		JTree tree = (JTree) getLeftComponent();
		
		/* Tree layout*/
		tree.expandRow(0);
		tree.setMinimumSize(tree.getPreferredSize());
		tree.setSelectionRow(1);
		tree.setRootVisible(true);
		
		/* Global layout */
		setResizeWeight(DEFAULT_WEIGHT);
		setContinuousLayout(true);
	}
}
