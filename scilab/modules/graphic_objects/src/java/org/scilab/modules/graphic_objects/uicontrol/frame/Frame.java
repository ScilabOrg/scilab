/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graphic_objects.uicontrol.frame;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME__;

import javax.swing.UIDefaults;
import javax.swing.UIManager;

import org.scilab.modules.graphic_objects.axes.AxesContainer;
import org.scilab.modules.graphic_objects.console.Console;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicModel.GraphicModel;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;

/**
 * @author Vincent COUVERT
 */
public class Frame extends Uicontrol implements AxesContainer {

    /**
     * Constructor
     */
    public Frame() {
        super();
        setStyle(__GO_UI_FRAME__);
        if (Console.getConsole().getUseDeprecatedLF()) {
            setRelief(RELIEF_RIDGE);
        } else {
            UIDefaults defaults = UIManager.getDefaults();

            //font
            setFont(defaults.getFont("Panel.font"));

            //h-alignment
            setHorizontalAlignment("left");

            //v-alignement
            setVerticalAlignment("middle");
        }
    }

    public Integer getAntialiasing() {
        return 0;
    }

    public Integer getBackground() {
        Figure figure = (Figure) GraphicModel.getModel().getObjectFromId(getParentFigure());
        if (figure != null) {
            return figure.getBackground();
        }
        return -2;
    }

    public ColorMap getColorMap() {
        Figure figure = (Figure) GraphicModel.getModel().getObjectFromId(getParentFigure());
        if (figure != null) {
            return figure.getColorMap();
        }

        return new ColorMap();
    }

    public Integer[] getAxesSize() {
        Double[] pos = getUiPosition();
        return new Integer[] {pos[2].intValue(), pos[3].intValue()};
    }

}
