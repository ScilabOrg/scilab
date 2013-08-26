/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.gui.utils;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.awt.print.PageFormat;
import java.awt.print.Printable;
import java.awt.print.PrinterException;
import java.awt.print.PrinterJob;

import javax.print.attribute.PrintRequestAttributeSet;

/**
 * This class allow us to print a figure using Java2D
 * @author Sylvestre Koumar
 *
 */
public class ScilabPrint implements Printable {

    private static final int PRINT_MARGIN = 3;

    private BufferedImage canvasDump;
    private PrinterJob pj;

    /**
     * Default Constructor
     * Calling print job to print the BufferedImage
     * @param canvasDump BufferedImage
     * @param printerJob PrinterJob
     * @param scilabPageFormat PrintRequestAttributeSet
     */
    public ScilabPrint(BufferedImage canvasDump, PrinterJob printerJob, PrintRequestAttributeSet scilabPageFormat) {
        this.canvasDump = canvasDump;
        this.pj = printerJob;
        PageFormat pf = pj.defaultPage();

        //setting the page format
        //		Paper paper = new Paper();
        //		double margin = PRINT_MARGIN;
        //		paper.setImageableArea(margin, margin, paper.getWidth() - margin * 2, paper.getHeight()
        //				- margin * 2);
        //		pf.setPaper(paper);

        pj.setPrintable(this, pf);
        try {
            pj.print(scilabPageFormat);
        } catch (PrinterException pe) {
            System.out.println("Printing Error");
            System.err.println(pe.toString());
        }
    }

    /**
     * Printing by converting the buffered image to Graphics2D
     * @param g Graphics
     * @param pf PageFormat
     * @param page int
     * @return Printable.PAGE_EXISTS if the printing instruction exist
     * @throws PrinterException indicate that an exceptional condition
     * has occurred in the print system
     */
    public int print(Graphics g, PageFormat pf, int page)
    throws PrinterException {
        if (page >= 1) {
            return Printable.NO_SUCH_PAGE;
        }

        ((Graphics2D) g).translate(pf.getImageableX(), pf.getImageableY());

        AffineTransform at = AffineTransform.getTranslateInstance(0, 0);
        ((Graphics2D) g).drawRenderedImage(canvasDump, at);

        return Printable.PAGE_EXISTS;
    }

}
