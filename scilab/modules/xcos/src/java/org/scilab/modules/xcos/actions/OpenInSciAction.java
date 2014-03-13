/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.actions;

import java.awt.event.ActionEvent;
import java.io.File;
import java.io.IOException;

import javax.swing.JButton;

import org.scilab.modules.commons.CommonFileUtils;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * File opening management
 */
@SuppressWarnings(value = { "serial" })
public final class OpenInSciAction extends DefaultAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.OPEN_IN;
    /** Icon name of the action */
    public static final String SMALL_ICON = "document-open-sci";
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
    public OpenInSciAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Create a menu to add in Scilab Graph menu bar
     *
     * @param scilabGraph
     *            associated Scilab Graph
     * @return the menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, OpenInSciAction.class);
    }

    /**
     * Create a button to add in Scilab Graph tool bar
     *
     * @param scilabGraph
     *            associated Scilab Graph
     * @return the button
     */
    public static JButton createButton(ScilabGraph scilabGraph) {
        return createButton(scilabGraph, OpenInSciAction.class);
    }

    /**
     * @param e
     *            parameter
     * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        final SwingScilabFileChooser fc = OpenAction.createFileChooser();

        /* Configure the file chooser */
        OpenAction.configureFileFilters(fc);
        fc.setCurrentDirectory(new File(CommonFileUtils.getCWD()));

        try {
            OpenAction.displayAndOpen(fc, getGraph(e).getAsComponent());
        } catch (IOException e1) {
            e1.printStackTrace();
        }
    }
}
