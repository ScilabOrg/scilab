/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import javax.swing.JColorChooser;

import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.VertexSelectionDependantAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxUtils;

/**
 * Change the color of multiple blocks
 */
@SuppressWarnings(value = { "serial" })
public final class BorderColorAction extends VertexSelectionDependantAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.BORDER_COLOR;
    /** Icon name of the action */
    public static final String SMALL_ICON = "draw-brush";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = 0;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = 0;

    /**
     * @param scilabGraph
     *            graph
     */
    public BorderColorAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * @param scilabGraph
     *            graph
     * @return menu item
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, BorderColorAction.class);
    }

    /**
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

        Object[] selectedCells = graph.getSelectionCells();

        // if no cells are selected : Do nothing
        if (selectedCells.length == 0) {
            return;
        }

        // Get the selected cells statistics values
        Map<String, Integer> colorStats = new HashMap<String, Integer>();
        for (Object object : selectedCells) {
            String color = (String) graph.getCellStyle(object).get(
                               mxConstants.STYLE_STROKECOLOR);
            if (colorStats.containsKey(color)) {
                colorStats.put(color, colorStats.get(color) + 1);
            } else {
                colorStats.put(color, 1);
            }
        }

        // Getting the most present color
        String color = "#FF0000";
        int max = 0;
        for (Entry<String, Integer> entry : colorStats.entrySet()) {
            final int current = entry.getValue();
            if (current > max) {
                color = entry.getKey();
                max = current;
            }
        }

        // Apply the most common color as the default color
        Color newColor = JColorChooser.showDialog(getGraph(null)
                         .getAsComponent(), NAME, mxUtils.parseColor(color));

        if (newColor != null) {
            graph.setCellStyles(mxConstants.STYLE_STROKECOLOR,
                                mxUtils.hexString(newColor));
        }
    }
}
