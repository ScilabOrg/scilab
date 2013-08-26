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

package org.scilab.modules.xcos.palette.actions;

import java.awt.event.ActionEvent;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.xcos.palette.PaletteManager;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Palette Tab visibility management
 */
@SuppressWarnings(value = { "serial" })
public final class ViewPaletteBrowserAction extends DefaultAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.PALETTE_BROWSER;
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
     *            associated Scilab Graph
     */
    public ViewPaletteBrowserAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Create checkbox menu for the graph menu bar
     *
     * @param scilabGraph
     *            associated Scilab Graph
     * @return the menu
     */
    public static CheckBoxMenuItem createCheckBoxMenu(ScilabGraph scilabGraph) {
        final CheckBoxMenuItem menu = createCheckBoxMenu(scilabGraph,
                                      ViewPaletteBrowserAction.class);
        PaletteManager.getInstance().addPropertyChangeListener(
        new PropertyChangeListener() {
            @Override
            public void propertyChange(PropertyChangeEvent evt) {
                if ("visible".equals(evt.getPropertyName())) {
                    menu.setChecked((Boolean) evt.getNewValue());
                }
            }
        });

        menu.setChecked(PaletteManager.isVisible());
        return menu;
    }

    /**
     * @param e
     *            parameter
     * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        PaletteManager.setVisible(!PaletteManager.isVisible());
    }
}
