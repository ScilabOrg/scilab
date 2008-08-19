/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class which draw centered text  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.textDrawing;


import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.textRendering.SciTextRenderer;


/**
 * Class which draw centered text.
 * @author Jean-Baptiste Silvy
 */
public class CenteredAlignedTextGL implements TextAlignementStrategy {

	/**
	 * Default constructor
	 */
	public CenteredAlignedTextGL() {
		
	}
	
	/**
	 * Draw the text matrix inside a position matrix using a specific text renderer.
	 * @param gl current OpenGL pipeline
	 * @param renderer renderer to use for drawing
	 * @param text matrix of strings
	 * @param positionMatrix matrix of positions.
	 * @param angle angle of the text to draw
	 */
	public void drawTextContent(GL gl, SciTextRenderer renderer, StringMatrixGL text,
			                    TextGrid positionMatrix, double angle) {
		renderer.begin3DRendering();
		for (int i = 0; i < text.getNbRow(); i++) {
			for (int j = 0; j < text.getNbCol(); j++) {
				Vector3D[] curCell = positionMatrix.getCellCoordinates(i, j);
				// put the middle of the string in the middle of the cell
				double xCoord = ((curCell[2].getX() + curCell[1].getX()) - text.getStringWidth(i, j)) / 2.0;
				double yCoord = curCell[1].getY() - (curCell[1].getY() - curCell[0].getY()) * TextGrid.EXTEND_FACTOR_Y / 2.0;
				renderer.draw3D(gl, text.getMatrixElement(i, j), xCoord,
						        yCoord, curCell[1].getZ(), angle);
			}
		}
		
		renderer.end3DRendering();

	}

}
