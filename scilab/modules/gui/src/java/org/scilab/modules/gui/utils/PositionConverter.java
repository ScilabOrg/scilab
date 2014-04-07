/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import java.awt.Container;
import java.awt.Dimension;
import java.awt.Point;

/**
 * Class used to convert Scilab position to/from Java position
 * @author Vincent COUVERT
 */
public final class PositionConverter {

    /**
     * Constructor
     */
    private PositionConverter() {
        throw new UnsupportedOperationException();
    }

    /**
     * Convert Scilab position to Java position
     * @param scilabPosition Scilab position
     * @param objectSize the size of the object
     * @param parent the parent
     * @return the Java position of the object
     */
    public static Position scilabToJava(Position scilabPosition, Size objectSize, Container parent) {

        if (parent == null) { // Parent not yet set
            return scilabPosition;
        }

        //		SwingScilabTab parentTab = (SwingScilabTab) parent;
        //		JPanel canvas = (JPanel) parentTab.getContentPane();
        //
        //		int xOffset = (parentTab.getDims().getWidth() - canvas.getWidth()) / 2; /* Border Width */
        //		int yOffset = parentTab.getTitlebar().getHeight();
        //
        //		int xCoord = scilabPosition.getX() + xOffset;
        //		int yCoord = canvas.getHeight() - scilabPosition.getY() - objectSize.getHeight() + yOffset + xOffset;
        //
        //		return new Position(xCoord, yCoord);
        //SwingScilabAxes parentAxes = (SwingScilabAxes) parent;
        int xCoord = scilabPosition.getX();
        int yCoord = parent.getHeight() - scilabPosition.getY() - objectSize.getHeight();

        return new Position(xCoord, yCoord);
    }

    /**
     * Convert Java position to Scilab position
     * @param javaPosition Java position
     * @param objectSize the size of the object
     * @param parent the parent
     * @return the Scilab position of the object
     */
    public static Position javaToScilab(Point javaPosition, Dimension objectSize, Container parent) {

        if (parent == null) { // Parent not yet set
            return new Position(javaPosition.x, javaPosition.y);
        }

        //		SwingScilabTab parentTab = (SwingScilabTab) parent;
        //		JPanel canvas = (JPanel) parentTab.getContentPane();
        //
        //		int xOffset = (parentTab.getDims().getWidth() - canvas.getWidth()) / 2; /* Border Width */
        //		int yOffset = parentTab.getTitlebar().getHeight();
        //
        //		int xCoord = javaPosition.x - xOffset;
        //		int yCoord = canvas.getHeight() - javaPosition.y - objectSize.height + yOffset + xOffset;
        //
        //		return new Position(xCoord, yCoord);

        
        int xCoord = javaPosition.x;
        int yCoord = parent.getHeight() - javaPosition.y - objectSize.height;

        return new Position(xCoord, yCoord);
    }
}
