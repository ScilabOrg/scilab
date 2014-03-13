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

package org.scilab.modules.xcos.actions;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.awt.print.PageFormat;
import java.awt.print.Paper;
import java.awt.print.PrinterException;
import java.awt.print.PrinterJob;

import javax.swing.JButton;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.swing.mxGraphComponent;

/**
 * Diagram printing management
 */
@SuppressWarnings(value = { "serial" })
public final class PrintAction extends DefaultAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.PRINT;
    /** Icon name of the action */
    public static final String SMALL_ICON = "document-print";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_P;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

    /** The default page margin */
    private static final int DEFAULT_MARGIN = 36;

    /**
     * Constructor
     *
     * @param scilabGraph
     *            associated diagram
     */
    public PrintAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Print menu creation
     *
     * @param scilabGraph
     *            associated diagram
     * @return the menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, PrintAction.class);
    }

    /**
     * Print button creation
     *
     * @param scilabGraph
     *            associated diagram
     * @return the button
     */
    public static JButton createButton(ScilabGraph scilabGraph) {
        return createButton(scilabGraph, PrintAction.class);
    }

    /**
     * @param e
     *            parameter
     * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        mxGraphComponent graphComponent = getGraph(null).getAsComponent();
        PrinterJob pj = PrinterJob.getPrinterJob();

        if (pj.printDialog()) {
            PageFormat pf = graphComponent.getPageFormat();
            Paper paper = new Paper();
            double margin = DEFAULT_MARGIN;
            paper.setImageableArea(margin, margin, paper.getWidth() - margin * 2, paper.getHeight() - margin * 2);
            pf.setPaper(paper);
            pj.setPrintable(graphComponent, pf);

            try {
                pj.print();
            } catch (PrinterException e2) {
                System.out.println(e2);
            }
        }

    }
}
