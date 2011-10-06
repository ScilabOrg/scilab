/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FIGURE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PARENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_CENTIMETERS_UNITS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_INCHES_UNITS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_NORMALIZED_UNITS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_PIXELS_UNITS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_POINTS_UNITS__;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.bridge.CallScilabBridge;
import org.scilab.modules.gui.widget.Widget;

/**
 * Class used to convert uicontrols positions according to units property
 * @author Vincent COUVERT
 */
public final class UnitsConverter {

    private static double POINT_PER_INCH = 72.0;
    private static double CM_PER_INCH = 2.54;


    public enum UicontrolUnits { /* Needed in SwingScilabWidget */
        POINTS,
        NORMALIZED,
        INCHES,
        CENTIMETERS,
        PIXELS
    };

    public static UicontrolUnits stringToUnitsEnum(String style) {
        if (style.equals(__GO_UI_POINTS_UNITS__)) {
            return UicontrolUnits.POINTS;
        } else if (style.equals(__GO_UI_NORMALIZED_UNITS__)) {
            return UicontrolUnits.NORMALIZED;
        } else if (style.equals(__GO_UI_INCHES_UNITS__)) {
            return UicontrolUnits.INCHES;
        } else if (style.equals(__GO_UI_CENTIMETERS_UNITS__)) {
            return UicontrolUnits.CENTIMETERS;
        } else if (style.equals(__GO_UI_PIXELS_UNITS__)) {
            return UicontrolUnits.PIXELS;
        } else {
            return null;
        }
    }

    public static double convertFromPoint(double value, UicontrolUnits newUnit, Widget uicontrol, boolean widthAsRef) {
        int refSize = 0;

        String parentId;
        String parentType = null;
        double[] parentSize;

        SwingViewObject uicontrolAsView = (SwingViewObject) uicontrol;
        String widgetType = (String) GraphicController.getController().getProperty(uicontrolAsView.getId(), __GO_TYPE__);

        /* Get the component height from java */
        if(widgetType.equals(__GO_FIGURE__)) { /* Figure */
            if (widthAsRef) {
                refSize = (int) CallScilabBridge.getScreenWidth();
            } else {
                refSize = (int) CallScilabBridge.getScreenHeight();
            }
            newUnit = UicontrolUnits.PIXELS;
        } else { /* Uicontrol */
            parentId = (String) GraphicController.getController().getProperty(uicontrolAsView.getId(), __GO_PARENT__);
            if (parentId == null && newUnit == UicontrolUnits.NORMALIZED) { /* Parent not yet set */
                return 0.0;
            }
            parentType = (String) GraphicController.getController().getProperty(parentId, __GO_TYPE__);
        }

        switch(newUnit) {
        case POINTS:
            return (double) value;
        case NORMALIZED:
            if(parentType.equals(__GO_FIGURE__)) { /* Figure */
                parentSize = (double[]) GraphicController.getController().getProperty(uicontrolAsView.getId(), __GO_SIZE__);
                if (widthAsRef) {
                    refSize = (int) parentSize[0];
                } else {
                    refSize = (int) parentSize[1];
                }
            } else { /* Frame */
                parentSize = (double[]) GraphicController.getController().getProperty(uicontrolAsView.getId(), __GO_POSITION__);

                if (widthAsRef) {
                    refSize = (int) parentSize[2];
                } else {
                    refSize = (int) parentSize[3];
                }
            }
            return ((double) value) / (refSize * POINT_PER_INCH / CallScilabBridge.getScreenResolution());
        case INCHES:
            return ((double) value) / POINT_PER_INCH;
        case CENTIMETERS:
            return ((double) value) * CM_PER_INCH / POINT_PER_INCH;
        case PIXELS:
            return ((double) value) * CallScilabBridge.getScreenResolution() / POINT_PER_INCH;
        default:
            return 0.0;
        }
    }

    public static double convertToPoint(double value, UicontrolUnits oldUnit, Widget uicontrol, boolean widthAsRef) {
        int refSize = 0;

        String parentId;
        String parentType = null;
        double[] parentSize;

        SwingViewObject uicontrolAsView = (SwingViewObject) uicontrol;
        String widgetType = (String) GraphicController.getController().getProperty(uicontrolAsView.getId(), __GO_TYPE__);

        /* Get the component height from java */
        if (widgetType.equals(__GO_FIGURE__)) { /* Figure */
            /* The parent is the screen */
            if (widthAsRef)	{
                refSize = (int) CallScilabBridge.getScreenWidth();
            } else {
                refSize = (int) CallScilabBridge.getScreenHeight();
            }
            oldUnit = UicontrolUnits.PIXELS;
        } else { /* Uicontrol */
            parentId = (String) GraphicController.getController().getProperty(uicontrolAsView.getId(), __GO_PARENT__);
            if (parentId == null || parentId.equals("") || oldUnit == UicontrolUnits.NORMALIZED) { /* Parent not yet set */
                return 0;
            }
            parentType = (String) GraphicController.getController().getProperty(parentId, __GO_TYPE__);
        }

        switch(oldUnit) {
        case POINTS:
            return (int) value;
        case NORMALIZED:
            if (parentType.equals(__GO_FIGURE__)) { /* Figure */
                parentSize = (double[]) GraphicController.getController().getProperty(uicontrolAsView.getId(), __GO_SIZE__);

                if (widthAsRef) {
                    refSize = (int) parentSize[0];
                } else {
                    refSize = (int) parentSize[1];
                }
            } else { /* Frame */
                parentSize = (double[]) GraphicController.getController().getProperty(uicontrolAsView.getId(), __GO_POSITION__);

                if (widthAsRef) {
                    refSize = (int) parentSize[2];
                } else {
                    refSize = (int) parentSize[3];
                }
            }
            return (int) (value * refSize * POINT_PER_INCH / CallScilabBridge.getScreenResolution());
        case INCHES:
            return (int) (value * POINT_PER_INCH);
        case CENTIMETERS:
            return (int) (value * POINT_PER_INCH / CM_PER_INCH);
        case PIXELS:
            return (int) (value * POINT_PER_INCH / CallScilabBridge.getScreenResolution());
        default:
            return 0;
        }
    }

    public static double convertFromPixel(int value, UicontrolUnits newUnit, Widget uicontrol, boolean widthAsRef) {

        SwingViewObject uicontrolAsView = (SwingViewObject) uicontrol;
        String widgetType = (String) GraphicController.getController().getProperty(uicontrolAsView.getId(), __GO_TYPE__);

        if ((uicontrol != null) && (widgetType.equals(__GO_FIGURE__))) {/* Uicontrol figure */
            newUnit = UicontrolUnits.PIXELS;
        }

        if (newUnit == UicontrolUnits.PIXELS) {
            return (double) value;
        } else {
            return convertFromPoint(convertToPoint(value, UicontrolUnits.PIXELS, uicontrol, widthAsRef), newUnit, uicontrol, widthAsRef);
        }
    }

    public static int convertToPixel(double value, UicontrolUnits oldUnit, Widget uicontrol, boolean widthAsRef) {

        SwingViewObject uicontrolAsView = (SwingViewObject) uicontrol;
        String widgetType = (String) GraphicController.getController().getProperty(uicontrolAsView.getId(), __GO_TYPE__);

        if ((uicontrol != null) && (widgetType.equals(__GO_FIGURE__))) { /* Uicontrol figure */
            oldUnit = UicontrolUnits.PIXELS;
        }

        if (oldUnit == UicontrolUnits.PIXELS) {
            return (int) value;
        } else {
            return (int) convertFromPoint(convertToPoint(value, oldUnit, uicontrol, widthAsRef), UicontrolUnits.PIXELS, uicontrol, widthAsRef);
        }
    }

    public static Double[] convertPositionToPixels(Double[] position, UicontrolUnits oldUnit, Widget uicontrol, boolean widthAsRef) {
        for (int i = 0; i<4; i++) {
            position[i] = new Double(convertToPixel(position[i].doubleValue(), oldUnit, uicontrol, widthAsRef));
        }
        return position;
    }

}
