/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan SIMON
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.block.actions;

import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.asynchronousScilabExec;
import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.buildCall;
import static org.scilab.modules.xcos.utils.FileUtils.delete;
import static org.scilab.modules.xcos.utils.FileUtils.exists;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.RWHandler;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Generate code for the current graph.
 */
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
        Object selectedObj = getGraph(null).getSelectionCell();
        if (!(selectedObj instanceof SuperBlock)) {
            ((XcosDiagram) getGraph(null))
                    .error(XcosMessages.ERROR_GENERATING_C_CODE);
            return;
        }

        ((XcosDiagram) getGraph(null)).info(XcosMessages.GENERATING_C_CODE);

        final SuperBlock block = (SuperBlock) selectedObj;
        try {
            /*
             * Prepare data
             */
            final String tempOutput = FileUtils.createTempFile();
            final String tempInput = FileUtils.createTempFile();

            /*
             * Export data
             */
            new RWHandler(tempOutput).writeBlock(block);

            /*
             * Prepare command and callback
             */
            String cmd = buildCall("xcosCodeGeneration", tempOutput, tempInput);

            final ActionListener callback = new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent arg0) {

                    if (!exists(tempInput)) {
                        ((XcosDiagram) getGraph(null))
                                .info(XcosMessages.EMPTY_INFO);
                        return;
                    }

                    XcosDiagram graph = block.getParentDiagram();
                    if (graph == null) {
                        block.setParentDiagram(Xcos.findParent(block));
                        graph = block.getParentDiagram();
                        LogFactory.getLog(getClass()).error(
                                "Parent diagram was null");
                    }

                    graph.getModel().beginUpdate();
                    doAction(block, tempInput);
                    graph.getModel().endUpdate();

                    graph.getView().clear(block, true, false);
                    graph.getView().validate();

                    delete(tempOutput);
                    delete(tempInput);

                    ((XcosDiagram) getGraph(null))
                            .info(XcosMessages.EMPTY_INFO);
                }
            };

            /*
             * Execute
             */
            asynchronousScilabExec(callback, cmd);

        } catch (IOException ex) {
            LogFactory.getLog(CodeGenerationAction.class).error(ex);
            ((XcosDiagram) getGraph(null)).info(XcosMessages.EMPTY_INFO);
        } catch (InterpreterException ex) {
            LogFactory.getLog(CodeGenerationAction.class).error(ex);
            ((XcosDiagram) getGraph(null)).info(XcosMessages.EMPTY_INFO);
        }
    }

    /**
     * Callback function
     * 
     * Read the block from the scilab
     * 
     * @param block
     *            The block we are working on
     * @param tempInput
     *            Input file
     */
    private static void doAction(final SuperBlock block, final String tempInput) {
        try {
            BasicBlock modifiedBlock = new RWHandler(tempInput).readBlock();

            block.updateBlockSettings(modifiedBlock);
            block.setInterfaceFunctionName(modifiedBlock
                    .getInterfaceFunctionName());
            block.setSimulationFunctionName(modifiedBlock
                    .getSimulationFunctionName());
            block.setSimulationFunctionType(modifiedBlock
                    .getSimulationFunctionType());
            block.setChild(null);

            block.setStyle(block.getStyle() + ";blockWithLabel");
            block.setValue(block.getSimulationFunctionName());
            BlockPositioning.updateBlockView(block);
        } catch (ScicosFormatException e) {
            LogFactory.getLog(CodeGenerationAction.class).error(e);
        }
    }
}
