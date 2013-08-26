/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.palette.view;

import java.awt.Color;

import javax.swing.DropMode;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTree;
import javax.swing.tree.TreeNode;
import javax.swing.tree.TreePath;
import javax.swing.tree.TreeSelectionModel;

import org.scilab.modules.xcos.palette.PaletteManager;
import org.scilab.modules.xcos.palette.listener.PaletteManagerMouseListener;
import org.scilab.modules.xcos.palette.listener.PaletteManagerTreeSelectionListener;
import org.scilab.modules.xcos.palette.listener.PaletteTreeTransferHandler;
import org.scilab.modules.xcos.utils.XcosConstants;

/**
 * The content pane for the block view
 */
@SuppressWarnings(value = { "serial" })
public class PaletteManagerPanel extends JSplitPane {

    private PaletteManager controller;

    /**
     * Default constructor
     *
     * @param controller
     *            the {@link PaletteManager} instance
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

        TreeNode root = controller.getRoot();
        JTree tree = new JTree(new PaletteTreeModel(root));
        JScrollPane treeScrollPane = new JScrollPane(tree);

        /** Setup tree */
        tree.getSelectionModel().setSelectionMode(
            TreeSelectionModel.SINGLE_TREE_SELECTION);
        tree.addMouseListener(new PaletteManagerMouseListener());
        tree.addTreeSelectionListener(new PaletteManagerTreeSelectionListener(
                                          panel));

        tree.setEditable(false);
        tree.setDragEnabled(true);
        tree.setExpandsSelectedPaths(true);
        tree.setDropMode(DropMode.INSERT);
        tree.setTransferHandler(new PaletteTreeTransferHandler());

        setLeftComponent(treeScrollPane);
        panel.setViewportView(rootPalette);
        setRightComponent(panel);
    }

    /**
     * Init the ScrollPane component
     *
     * @param panel
     *            the component
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
        JTree tree = (JTree) ((JScrollPane) getLeftComponent()).getViewport()
                     .getView();

        /* Tree layout */
        final Object root = tree.getModel().getRoot();
        final Object firstChild = tree.getModel().getChild(root, 0);
        final Object secondChild = tree.getModel().getChild(firstChild, 0);
        tree.setSelectionPath(new TreePath(new Object[] {root, firstChild, secondChild}));

        tree.setRootVisible(false);
        tree.setScrollsOnExpand(true);

        /* Global layout */
        setContinuousLayout(true);
    }
}
