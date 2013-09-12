/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan SIMON
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

import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.asynchronousScilabExec;
import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.buildCall;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.logging.Logger;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException;
import org.scilab.modules.xcos.io.scicos.ScilabDirectHandler;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Generate code for the current graph.
 */
@SuppressWarnings(value = { "serial" })
public class CodeGenerationAction extends SuperBlockSelectedAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.CODE_GENERATION;
    /** Icon name of the action */
    public static final String SMALL_ICON = "";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = 0;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = 0;

    /**
     * Constructor
     *
     * @param scilabGraph
     *            associated Scilab graph
     */
    public CodeGenerationAction(ScilabGraph scilabGraph) {
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
        return createMenu(scilabGraph, CodeGenerationAction.class);
    }

    /**
     * Action !!!
     *
     * @param e
     *            parameter
     * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        final XcosDiagram graph = (XcosDiagram) getGraph(e);

        // action disabled when the cell is edited
        final ScilabComponent comp = ((ScilabComponent) graph.getAsComponent());
        if (comp.isEditing()) {
            return;
        }

        Object selectedObj = graph.getSelectionCell();
        if (!(selectedObj instanceof SuperBlock)) {
            graph.error(XcosMessages.ERROR_GENERATING_C_CODE);
            return;
        }

        graph.info(XcosMessages.GENERATING_C_CODE);

        final SuperBlock block = (SuperBlock) selectedObj;

        final ScilabDirectHandler handler = ScilabDirectHandler.acquire();
        if (handler == null) {
            return;
        }

        try {
            /*
             * Export data
             */
            handler.writeBlock(block);

            /*
             * Prepare command and callback
             */
            String cmd = buildCall("blk = xcosCodeGeneration", ScilabDirectHandler.BLK.toCharArray());

            final ActionListener callback = new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent arg0) {
                    /*
                     * Find the block parent
                     */
                    XcosDiagram parent = block.getParentDiagram();
                    if (parent == null) {
                        block.setParentDiagram(Xcos.findParent(block));
                        parent = block.getParentDiagram();
                        Logger.getLogger(CodeGenerationAction.class.getName()).finest("Parent diagram was null");
                    }

                    /*
                     * Update
                     */
                    parent.getModel().beginUpdate();
                    doAction(block, handler);
                    parent.getModel().endUpdate();

                    parent.getView().clear(block, true, false);
                    parent.getView().validate();

                    graph.info(XcosMessages.EMPTY_INFO);
                }
            };

            /*
             * Execute
             */
            asynchronousScilabExec(callback, cmd);

        } catch (InterpreterException ex) {
            Logger.getLogger(CodeGenerationAction.class.getName()).severe(ex.toString());
            graph.info(XcosMessages.EMPTY_INFO);

            handler.release();
        }
    }

    /**
     * Callback function
     *
     * Read the block from the scilab
     *
     * @param block
     *            The block we are working on
     * @param handler
     *            the handler used to read/write data to/from Scilab
     */
    private static void doAction(final SuperBlock block, final ScilabDirectHandler handler) {
        try {
            try {
                final BasicBlock modifiedBlock = handler.readBlock();
                if (modifiedBlock == null) {
                    return;
                }

                block.updateBlockSettings(modifiedBlock);
                block.setInterfaceFunctionName(modifiedBlock.getInterfaceFunctionName());
                block.setSimulationFunctionName(modifiedBlock.getSimulationFunctionName());
                block.setSimulationFunctionType(modifiedBlock.getSimulationFunctionType());
                block.setChild(null);

                block.setStyle(block.getStyle() + ";blockWithLabel");
                block.setValue(block.getSimulationFunctionName());
                BlockPositioning.updateBlockView(block);
            } catch (ScicosFormatException e) {
                Logger.getLogger(CodeGenerationAction.class.getName()).severe(e.toString());
            }
        } finally {
            handler.release();
        }
    }
}
