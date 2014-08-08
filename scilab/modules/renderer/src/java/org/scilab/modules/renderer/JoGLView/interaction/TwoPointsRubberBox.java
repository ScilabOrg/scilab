/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.modules.renderer.JoGLView.interaction;

import org.scilab.modules.renderer.JoGLView.DrawerVisitor;

/**
 * @author Bruno JOFRET
 */
public class TwoPointsRubberBox extends RubberBox implements PointRubberBox {

    /**
     * Default constructor.
     *
     * @param drawerVisitor parent drawer visitor.
     */
    public TwoPointsRubberBox(DrawerVisitor drawerVisitor) {
        super(drawerVisitor);
    }

    public double[] getResults() {
        double[][] factors = axes.getScaleTranslateFactors();
        double result[] = {
            mouseButton - 1,
            (Math.min(firstPoint.getX(), secondPoint.getX()) - factors[1][0]) / factors[0][0],
            (Math.max(firstPoint.getY(), secondPoint.getY()) - factors[1][1]) / factors[0][1],
            (Math.max(firstPoint.getZ(), secondPoint.getZ()) - factors[1][2]) / factors[0][2],
            (Math.abs(firstPoint.getX() - secondPoint.getX())) / factors[0][0],
            (Math.abs(firstPoint.getY() - secondPoint.getY())) / factors[0][1],
            (Math.abs(firstPoint.getZ() - secondPoint.getZ())) / factors[0][2]
        };

        return result;
    }
}
