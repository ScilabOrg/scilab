/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.graph.swing;

import java.awt.GraphicsEnvironment;
import java.awt.MouseInfo;

import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.PasteAction;
import org.scilab.modules.graph.actions.RedoAction;
import org.scilab.modules.graph.actions.SelectAllAction;
import org.scilab.modules.graph.actions.UndoAction;
import org.scilab.modules.graph.actions.ZoomInAction;
import org.scilab.modules.graph.actions.ZoomOutAction;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenu;
import org.scilab.modules.xcos.actions.DiagramBackgroundAction;
import org.scilab.modules.xcos.actions.SetContextAction;
import org.scilab.modules.xcos.actions.SetupAction;
import org.scilab.modules.xcos.actions.XcosDocumentationAction;
import org.scilab.modules.xcos.block.actions.ShowParentAction;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.swing.handler.ConnectionHandler;
import org.scilab.modules.xcos.graph.swing.handler.GraphHandler;
import org.scilab.modules.xcos.graph.swing.handler.SelectionCellsHandler;

import com.mxgraph.swing.handler.mxConnectionHandler;
import com.mxgraph.swing.handler.mxGraphHandler;
import com.mxgraph.swing.handler.mxSelectionCellsHandler;

/**
 * Implement a specific {@link com.mxgraph.swing.mxGraphComponent} for an Xcos
 * diagram.
 */
@SuppressWarnings(value = { "serial" })
public class GraphComponent extends ScilabComponent {

    /**
     * Default constructor.
     *
     * @param graph
     *            the graph
     */
    public GraphComponent(XcosDiagram graph) {
        super(graph);
    }

    /**
     * @return a new {@link ConnectionHandler} instance
     * @see com.mxgraph.swing.mxGraphComponent#createConnectionHandler()
     */
    @Override
    protected mxConnectionHandler createConnectionHandler() {
        return new ConnectionHandler(this);
    }

    /**
     * @return a new {@link GraphHandler} instance
     * @see com.mxgraph.swing.mxGraphComponent#createGraphHandler()
     */
    @Override
    protected mxGraphHandler createGraphHandler() {
        if (GraphicsEnvironment.isHeadless()) {
            return null;
        }

        return new GraphHandler(this);
    }

    @Override
    protected mxSelectionCellsHandler createSelectionCellsHandler() {
        return new SelectionCellsHandler(this);
    }

    /**
     * Display the context menu on the current component
     */
    public void displayContextMenu() {
        ContextMenu menu = ScilabContextMenu.createContextMenu();

        menu.add(UndoAction.undoMenu((ScilabGraph) getGraph()));
        menu.add(RedoAction.redoMenu((ScilabGraph) getGraph()));
        menu.add(PasteAction.pasteMenu((ScilabGraph) getGraph()));
        menu.add(SelectAllAction.createMenu((ScilabGraph) getGraph()));
        /*---*/
        menu.getAsSimpleContextMenu().addSeparator();
        /*---*/
        menu.add(SetContextAction.createMenu((ScilabGraph) getGraph()));
        menu.add(SetupAction.createMenu((ScilabGraph) getGraph()));

        if (getGraph() instanceof SuperBlockDiagram) {
            /*---*/
            menu.getAsSimpleContextMenu().addSeparator();
            /*---*/
            menu.add(ShowParentAction.createMenu((ScilabGraph) getGraph()));
        }
        /*---*/
        menu.getAsSimpleContextMenu().addSeparator();
        /*---*/
        menu.add(ZoomInAction.zoominMenu((ScilabGraph) getGraph()));
        menu.add(ZoomOutAction.zoomoutMenu((ScilabGraph) getGraph()));
        /*---*/
        menu.getAsSimpleContextMenu().addSeparator();
        /*---*/
        menu.add(DiagramBackgroundAction.createMenu((ScilabGraph) getGraph()));
        /*---*/
        menu.getAsSimpleContextMenu().addSeparator();
        /*---*/
        menu.add(XcosDocumentationAction.createMenu((ScilabGraph) getGraph()));

        ((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(MouseInfo.getPointerInfo().getLocation().x, MouseInfo.getPointerInfo()
                .getLocation().y);

        menu.setVisible(true);
    }
}
