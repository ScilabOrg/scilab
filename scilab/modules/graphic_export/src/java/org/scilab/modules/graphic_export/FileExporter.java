/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - Calixte Denizet
 * desc : Static class used to create file export of graphic figures
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_export;

import java.lang.reflect.Method;

import java.awt.Color;
import java.io.File;
import java.io.IOException;
import org.scilab.modules.commons.ScilabCommonsUtils;
import org.scilab.modules.renderer.FigureMapper;
import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.implementation.g2d.G2DCanvasFactory;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import java.awt.Graphics2D;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;

/**
 * Static class used to create file export of graphic figures
 * @author Jean-Baptiste Silvy
 */
public class FileExporter {

    /** Export waiting message */
    private static final String exportingMessage = "Exporting figure, please wait...";

    /** The id used on classpath.xml to load vectorial export JARs */
    private static final String CLASSPATH_PDF_PS_EPS_EXPORT_NAME = "pdf_ps_eps_graphic_export";
    private static final String CLASSPATH_SVG_EXPORT_NAME = "svg_graphic_export";

    /**
     * Default constructor
     */
    protected FileExporter() { }

    /**
     * Export a figure into a file
     * @param figureIndex index of the figure to export
     * @param fileName name of the file to create
     * @param fileType kind of the file
     * @param jpegCompressionQuality the JPEG compression quality
     * @param fileOrientation orientation of the file
     * @return 0 if everything worked fine, a non null integer if an exception occured
     *         depending on the kind of error
     */
    public static String fileExport(int figureIndex, String fileName, int fileType, float jpegCompressionQuality, int fileOrientation) {
//	export(figureIndex, fileName, fileType, jpegCompressionQuality, fileOrientation);
	return "";
    }

    public static void fileExport2(String uid, String fileName) {
	Figure figure = (Figure) GraphicController.getController().getObjectFromId(uid);
	Integer[] dims = figure.getAxesSize();
	BufferedImage image = new BufferedImage(dims[0], dims[1], BufferedImage.TYPE_INT_ARGB);
	Graphics2D g2d = image.createGraphics();
	g2d.setColor(java.awt.Color.WHITE);
	g2d.fillRect(0, 0, dims[0], dims[1]);
	Canvas canvas = G2DCanvasFactory.createCanvas(g2d, dims[0], dims[1]);
	DrawerVisitor visitor = new DrawerVisitor(null, canvas, figure);
	canvas.setMainDrawer(visitor);
	canvas.redraw();
	GraphicController.getController().unregister(visitor);
	
	
	File file = new File(fileName);
	try {
	    ImageIO.write(image, "png", file.getAbsoluteFile());
	} catch (IOException ex) { }
	g2d.dispose();
    }
}
