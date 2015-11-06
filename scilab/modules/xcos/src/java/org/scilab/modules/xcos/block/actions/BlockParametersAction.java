/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.block.actions;

import java.awt.Cursor;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.VertexSelectionDependantAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfScicosID;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.model.BlockInterFunction;
import org.scilab.modules.xcos.graph.model.XcosCell;
import org.scilab.modules.xcos.graph.model.XcosCellFactory;
import org.scilab.modules.xcos.io.scicos.ScilabDirectHandler;
import org.scilab.modules.xcos.utils.XcosMessages;

import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.buildCall;
import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.asynchronousScilabExec;

/**
 * Open dialog to set block parameters
 */
@SuppressWarnings(value = { "serial" })
public class BlockParametersAction extends VertexSelectionDependantAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.BLOCK_PARAMETERS;
    /** Icon name of the action */
    public static final String SMALL_ICON = "applications-system";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_B;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit()
            .getMenuShortcutKeyMask();

    /**
     * Constructor
     *
     * @param scilabGraph
     *            associated diagram
     */
    public BlockParametersAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Menu for diagram menubar
     *
     * @param scilabGraph
     *            associated diagram
     * @return the menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, BlockParametersAction.class);
    }

    /**
     * @param e
     *            parameter
     * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        actionPerformed();
    }

    public void actionPerformed() {
        XcosDiagram graph = (XcosDiagram) getGraph(null);
        Object selectedCell = graph.getSelectionCell();
        if (selectedCell != null && selectedCell instanceof XcosCell) {
            XcosCell cell = (XcosCell) selectedCell;

            if (cell.getKind() != Kind.BLOCK) {
                return;
            }

            JavaController controller = new JavaController();

            String[] interfaceFunction = new String[1];
            controller.getObjectProperty(cell.getUID(), cell.getKind(), ObjectProperties.INTERFACE_FUNCTION, interfaceFunction);

            BlockInterFunction func = XcosCellFactory.lookForInterfunction(interfaceFunction[0]);
            if (func.equals(BlockInterFunction.SUPER_f)) {
                // this is a super-block, open it
                XcosDiagram sub = new XcosDiagram(cell.getUID(), cell.getKind());

                XcosTab.restore(sub, true);
                Xcos.getInstance().addDiagram(sub);
            } else {
                BasicBlock block = (BasicBlock) cell;
                // prevent to open twice
                if (block.isLocked()) {
                    return;
                }

                graph.setCellsLocked(true);
                graph.getAsComponent().getGraphControl().setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));

                try {

                    final ActionListener action = new ActionListener() {
                        @Override
                        public void actionPerformed(ActionEvent e) {

                            graph.getView().clear(this, true, true);

                            // Now read new Block
                            graph.getModel().beginUpdate();
                            try {
                                block.updateBlockSettings(XcosCellFactory.createBlock(XcosCellFactory.getLastCreated()));
                            } finally {
                                graph.getModel().endUpdate();
                                block.setLocked(false);

                                graph.getAsComponent().getGraphControl().setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
                                graph.setCellsLocked(false);
                            }
                        }
                    };

                    ScilabDirectHandler handler = ScilabDirectHandler.acquire();
                    try {
                        handler.writeContext(graph.getContext());
                    } finally {
                        handler.release();
                    }

                    block.setLocked(true);
                    String blk = buildCall("scicos_new", Long.toHexString(cell.getUID()));
                    String xcosBlockInterface = buildCall("xcosBlockInterface", interfaceFunction[0].toCharArray(), "set", blk.toCharArray(), ScilabDirectHandler.CONTEXT.toCharArray());

                    asynchronousScilabExec(action, "xcosCellCreated", xcosBlockInterface.toCharArray());
                } catch (InterpreterException ex) {
                    block.setLocked(false);

                    graph.getAsComponent().getGraphControl().setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
                    graph.setCellsLocked(false);
                }
            }
        }
    }
}
