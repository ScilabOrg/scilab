/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.interaction;

import org.scilab.modules.renderer.JoGLView.DrawerVisitor;

/**
 * @author Bruno JOFRET
 */
public class PointRubberBox extends RubberBox {

	/**
	 * Default constructor.
	 *
	 * @param drawerVisitor parent drawer visitor.
	 */
	public PointRubberBox(DrawerVisitor drawerVisitor) {
		super(drawerVisitor);
		status = Status.WAIT_POINT_A;
	}

	public double[] getResults() {
		double result[] = {
				mouseButton - 1,
				Math.min(firstPoint.getX(), secondPoint.getX()),
				Math.max(firstPoint.getY(), secondPoint.getY()),
				Math.max(firstPoint.getZ(), secondPoint.getZ()),
				Math.abs(firstPoint.getX() - secondPoint.getX()),
				Math.abs(firstPoint.getY() - secondPoint.getY()),
				Math.abs(firstPoint.getZ() - secondPoint.getZ())
		};

		return result;
	}
}
