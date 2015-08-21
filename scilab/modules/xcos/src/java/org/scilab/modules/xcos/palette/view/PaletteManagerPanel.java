/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2015 - Marcos CARDINOT
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
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Toolkit;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;
import java.util.ArrayList;
import java.util.List;

import javax.swing.DropMode;
import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTree;
import javax.swing.tree.TreeNode;
import javax.swing.tree.TreePath;
import javax.swing.tree.TreeSelectionModel;

import org.scilab.modules.xcos.graph.PaletteDiagram;
import org.scilab.modules.xcos.palette.PaletteManager;
import org.scilab.modules.xcos.palette.actions.NavigationAction;
import org.scilab.modules.xcos.palette.actions.ZoomAction;
import org.scilab.modules.xcos.palette.listener.PaletteManagerMouseListener;
import org.scilab.modules.xcos.palette.listener.PaletteManagerTreeSelectionListener;
import org.scilab.modules.xcos.palette.listener.PaletteManagerTreeWillExpandListener;
import org.scilab.modules.xcos.palette.listener.PaletteTreeTransferHandler;
import org.scilab.modules.xcos.palette.model.Category;
import org.scilab.modules.xcos.palette.model.Custom;
import org.scilab.modules.xcos.palette.model.PaletteBlock;
import org.scilab.modules.xcos.palette.model.PaletteNode;
import org.scilab.modules.xcos.palette.model.PreLoaded;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosConstants.PaletteBlockSize;

/**
 * The content pane for the block view
 */
@SuppressWarnings(value = { "serial" })
public class PaletteManagerPanel extends JSplitPane {

    /** Name of tree node for recently used blocks **/
    public static final String RECENTLY_USED_BLOCKS = "Recently Used Blocks";

    private static XcosConstants.PaletteBlockSize currentSize;
    private PaletteManager controller;
    private CustomMouseWheelListener mouseWheelListener;
    private JTree tree;
    private PaletteDiagram diagramInstance;
    private List<TreePath> historyNext;
    private List<TreePath> historyPrev;
    private TreePath currentPath;

    /**
     * Default constructor
     *
     * @param controller
     *            the {@link PaletteManager} instance
     */
    public PaletteManagerPanel(PaletteManager controller) {
        super(JSplitPane.HORIZONTAL_SPLIT);
        this.controller = controller;
        this.mouseWheelListener = new CustomMouseWheelListener();
        this.historyNext = new ArrayList<TreePath>();
        this.historyPrev = new ArrayList<TreePath>();
        this.currentPath = null;
        currentSize = XcosConstants.PaletteBlockSize.NORMAL;
        fillUpContentPane();
    }

    /**
     * Fill up the content pane
     */
    private void fillUpContentPane() {
        // Default instances
        JScrollPane panel = new JScrollPane();
        panel.setBackground(Color.WHITE);
        setUpScrollBar(panel, currentSize);

        // Set default left component
        JPanel rootPalette = new JPanel();

        TreeNode root = controller.getRoot();
        tree = new JTree(new PaletteTreeModel(root));
        JScrollPane treeScrollPane = new JScrollPane(tree);

        // Setup tree
        tree.getSelectionModel().setSelectionMode(
            TreeSelectionModel.SINGLE_TREE_SELECTION);
        tree.addMouseListener(new PaletteManagerMouseListener());
        tree.addTreeWillExpandListener(new PaletteManagerTreeWillExpandListener());
        tree.addTreeSelectionListener(new PaletteManagerTreeSelectionListener(
                                          this, panel));

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
     * Get the current PaletteBlockSize
     * @return current PaletteBlockSize
     */
    public static PaletteBlockSize getCurrentSize() {
        return currentSize;
    }

    /**
     * Adds a PaletteBlock to the list of recently used blocks. 
     * @param block PaletteBlock
     */
    public void addRecentltyUsedBlock(PaletteBlock block) {
        PaletteNode currentNode = (PaletteNode) tree.getLastSelectedPathComponent();
        if (currentNode != null && currentNode.getName().equals(RECENTLY_USED_BLOCKS)) {
            return;
        }

        final Category root = PaletteManager.getInstance().getRoot();
        List<PaletteNode> nodes = root.getNode();

        PreLoaded p = null;
        for (PaletteNode n : nodes) {
            if (n.getName().equals(RECENTLY_USED_BLOCKS) && n instanceof PreLoaded) {
                p = (PreLoaded) n;
                break;
            }
        }

        if (p == null) {
            p = new PreLoaded();
            p.setName(RECENTLY_USED_BLOCKS);
            p.setEnable(true);
            root.getNode().add(p);
            p.setParent(root);
            PaletteNode.refreshView(root, currentNode);
        }

        List<PaletteBlock> blocks = p.getBlock();
        for (PaletteBlock b : blocks) {
            if (b.getName().equals(block.getName())) {
                return;
            }
        }

        if (blocks.size() >= XcosConstants.MAX_RECENTLY_USED_BLOCKS) {
            blocks.remove(XcosConstants.MAX_RECENTLY_USED_BLOCKS - 1);
        }
        blocks.add(0, block);
    }

    /**
     * Zoom
     * @param newSize new paletteblocksize enum
     */
    // CSOFF: CyclomaticComplexity
    private void zoom(PaletteBlockSize newSize) {
        if (newSize == null || newSize == currentSize) {
            return;
        }

        currentSize = newSize;

        try {
            JScrollPane jspR = (JScrollPane) this.getRightComponent();
            final Dimension dimension = jspR.getPreferredSize();
            setUpScrollBar(jspR, newSize);

            // check what's being displayed on the right panel
            PaletteNode node = (PaletteNode) tree.getLastSelectedPathComponent();
            if (node instanceof PreLoaded || node == null) {
                Component c = jspR.getViewport().getComponent(0);
                String cName = c.getName();
                PaletteView pview;
                if (cName.equals("PaletteView")) {
                    pview = (PaletteView) c;
                } else if (cName.equals("PaletteSearchView")) {
                    PaletteSearchView sview = (PaletteSearchView) c;
                    pview = (PaletteView) sview.getComponent(1);
                } else {
                    return;
                }

                Component[] blockViews = pview.getComponents();
                for (Component component : blockViews) {
                    PaletteBlockView bview = (PaletteBlockView) component;
                    bview.initComponents();
                }
                pview.revalidate();
            } else if (node instanceof Custom) {
                jspR = openDiagramAsPal(node);
                jspR.setPreferredSize(dimension);
                this.setRightComponent(jspR);
            } else {
                return;
            }

            // update the status of both zoom buttons
            if (newSize.next() == null) {
                // is it the last zoom-in level?
                ZoomAction.setEnabledZoomIn(false);
                ZoomAction.setEnabledZoomOut(true);
            } else if (newSize.previous() == null) {
                // is it the last zoom-out level?
                ZoomAction.setEnabledZoomIn(true);
                ZoomAction.setEnabledZoomOut(false);
            } else {
                ZoomAction.setEnabledZoomIn(true);
                ZoomAction.setEnabledZoomOut(true);
            }

            jspR.revalidate();
        } catch (NullPointerException e) {
            e.printStackTrace();
        }
    }

    /** zoom in **/
    public void zoomIn() {
        zoom(currentSize.next());
    }

    /** zoom out **/
    public void zoomOut() {
        zoom(currentSize.previous());
    }

    /**
     * Open a diagram as a palette.
     * @param node PaletteNode
     * @return a JScrollPane with the diagram
     */
    public JScrollPane openDiagramAsPal(PaletteNode node) {
        String path = ((Custom) node).getPath().getEvaluatedPath();
        this.diagramInstance = new PaletteDiagram();
        this.diagramInstance.openDiagramAsPal(path);
        return this.diagramInstance.getAsComponent();
    }

    /**
     * Setup the ScrollPane component
     * @param jsp The component
     * @param pbs PaletteBlockSize
     */
    private void setUpScrollBar(JScrollPane jsp, PaletteBlockSize pbs) {
        // vertical
        jsp.getVerticalScrollBar().setBlockIncrement(
            pbs.getBlockDimension().height
            + XcosConstants.PALETTE_VMARGIN);
        jsp.getVerticalScrollBar().setUnitIncrement(
            pbs.getBlockDimension().height
            + XcosConstants.PALETTE_VMARGIN);
        // horizontal
        jsp.getHorizontalScrollBar().setBlockIncrement(
            pbs.getBlockDimension().width
            + XcosConstants.PALETTE_HMARGIN);
        jsp.getHorizontalScrollBar().setUnitIncrement(
            pbs.getBlockDimension().width
            + XcosConstants.PALETTE_HMARGIN);

        mouseWheelListener.setVerticalScrollBar(jsp.getVerticalScrollBar());

        // removes the mouse wheel listeners
        MouseWheelListener[] mwls = jsp.getMouseWheelListeners();
        for (MouseWheelListener mwl : mwls) {
            jsp.removeMouseWheelListener(mwl);
        }
        // adds the CustomMouseWheelListener
        jsp.addMouseWheelListener(mouseWheelListener);
    }

    /**
     * Setup the default layout
     */
    public void performStartUpLayout() {
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

    /**
     * Updates the history.
     */
    public void updateHistory() {
        TreePath lastPath = currentPath;
        currentPath = tree.getSelectionPath();

        if (lastPath != null && !currentPath.equals(lastPath)) {

            if (historyPrev.isEmpty()) {
                historyPrev.add(lastPath);
            } else {
                int prevPathIdx = historyPrev.size() - 1;
                // going backwards?
                if (currentPath.equals(historyPrev.get(prevPathIdx))) {
                    historyNext.add(lastPath);
                    historyPrev.remove(prevPathIdx);
                    updateHistorySettings();
                    return;
                } else {
                    // it seems to be a new path, just store it!
                    // Soon we'll check if we are just going forward
                    // or if it's really a new path!
                    historyPrev.add(lastPath);
                }
            }

            if (!historyNext.isEmpty()) {
                int nextPathIdx = historyNext.size() - 1;
                // going forward?
                if (currentPath.equals(historyNext.get(nextPathIdx))) {
                    historyNext.remove(nextPathIdx);
                } else {
                    // it is really a new path!
                    // make sure it's on the top of the history!
                    historyNext.clear();
                }
            }
            updateHistorySettings();
        }
    }

    /**
     * Update the status of the buttons and the history length.
     */
    private void updateHistorySettings() {
        if (historyNext.size() + historyPrev.size() > XcosConstants.HISTORY_LENGTH) {
            historyPrev.remove(0);
        }
        NavigationAction.setEnabledNext(historyNext.size() > 0);
        NavigationAction.setEnabledPrev(historyPrev.size() > 0);
    }

    /**
     * Go to the next path (history).
     */
    public void goNext() {
        try {
            tree.setSelectionPath(historyNext.get(historyNext.size() - 1));
        } catch (ArrayIndexOutOfBoundsException err) {
            NavigationAction.setEnabledNext(false);
        }
    }

    /**
     * Go to the previous path (history).
     */
    public void goPrevious() {
        try {
            tree.setSelectionPath(historyPrev.get(historyPrev.size() - 1));
        } catch (ArrayIndexOutOfBoundsException err) {
            NavigationAction.setEnabledPrev(false);
        }
    }

    /**
     * Implement custom mouse handling for the zoom
     */
    private static final class CustomMouseWheelListener implements MouseWheelListener {
        private static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();
        private JScrollBar verticalScrollBar;

        /**
         * Default constructor
         */
        public CustomMouseWheelListener() {
        }

        /**
         * Set the vertical scrollbar
         * It's important to update the unit increment
         * @param verticalScrollBar JScrollBar
         */
        public void setVerticalScrollBar(JScrollBar verticalScrollBar) {
            this.verticalScrollBar = verticalScrollBar;
        }

        /**
         * When the wheel is used
         * @param e The parameters
         * @see java.awt.event.MouseWheelListener#mouseWheelMoved(java.awt.event.MouseWheelEvent)
         */
        @Override
        public void mouseWheelMoved(MouseWheelEvent e) {
            if (verticalScrollBar == null) {
                // nothing to do!
                return;
            }

            if (e.getModifiers() == ACCELERATOR_KEY) {
                if (e.getWheelRotation() < 0) {
                    PaletteManagerView.get().getPanel().zoomIn();
                } else if (e.getWheelRotation() > 0) {
                    PaletteManagerView.get().getPanel().zoomOut();
                }
            } else {
                int i = verticalScrollBar.getValue();
                if (e.getWheelRotation() < 0) {
                    i -= verticalScrollBar.getUnitIncrement();
                } else {
                    i += verticalScrollBar.getUnitIncrement();
                }
                verticalScrollBar.setValue(i);
            }
        }
    }
}
