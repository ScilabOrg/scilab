/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.datatip;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;
import org.scilab.modules.graphic_objects.textObject.Text;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;

import java.text.DecimalFormat;


public class Datatip extends Text {

    Double[] tipData;
    Boolean use3component;
    DecimalFormat tipTextFormat;


    enum DatatipObjectProperty { TIP_DATA, TIP_ORIENTATION };
    enum TipOrientation { TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT;

         public static TipOrientation intToEnum(Integer i) {
             switch (i) {
                 case 0:
                     return TipOrientation.TOP_LEFT;
                 case 1:
                     return TipOrientation.TOP_RIGHT;
                 case 2:
                     return TipOrientation.BOTTOM_LEFT;
                 case 3:
                     return TipOrientation.BOTTOM_RIGHT;
                 default:
                     return TipOrientation.TOP_RIGHT;
             }
         }

    };

    TipOrientation currentOrientation;

    public Datatip() {
        super();
        tipData = new Double[]{0.0, 0.0, 0.0};
        use3component = false;
        setOrientationAsEnum(TipOrientation.TOP_RIGHT);
        tipTextFormat = new DecimalFormat("#.####");
        setBox(true);
        setLineMode(true);
        setFillMode(true);
        setBackground(-2);
        setClipState(1);

        setMarkMode(true);
        setMarkSize(8);
        setMarkBackground(-1);
        setMarkForeground(-1);
        setMarkStyle(11);
    }

    @Override
    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

	public Object getPropertyFromName(String propertyName) {
        if (propertyName.equals(__GO_DATATIP_DATA__)) {
            return DatatipObjectProperty.TIP_DATA;
        } else if (propertyName.equals(__GO_DATATIP_ORIENTATION__)) {
            return DatatipObjectProperty.TIP_ORIENTATION;
        } else {
            return super.getPropertyFromName(propertyName);
        }
    }

	public Object getProperty(Object property) {
        if (property == DatatipObjectProperty.TIP_DATA) {
            return getTipData();
        } else if (property == DatatipObjectProperty.TIP_ORIENTATION) {
            return getOrientation();
        } else {
            return super.getProperty(property);
        }
    }

	public UpdateStatus setProperty(Object property, Object value) {
		if (property == DatatipObjectProperty.TIP_DATA) {
			setTipData((Double[]) value);
		} else if (property == DatatipObjectProperty.TIP_ORIENTATION) {
			setOrientation((Integer) value);
		} else {
			return super.setProperty(property, value);
		}

		return UpdateStatus.Success;
    }

    public void setTipData(Double[] data) {
        tipData[0] = data[0];
        tipData[1] = data[1];
        tipData[2] = data[2];
        updateText();
        updateTextPosition();
    }

    public Double[] getTipData() {
        Double[] ret = new Double[3];
        ret[0] = tipData[0];
        ret[1] = tipData[1];
        ret[2] = tipData[2];
        return ret;
    }

    public Integer getOrientation() {
        return getOrientationAsEnum().ordinal();
    }

    public TipOrientation getOrientationAsEnum() {
        return currentOrientation;
    }

    public void setOrientation(Integer orientation) {
        currentOrientation = TipOrientation.intToEnum(orientation);
        updateTextPosition();
    }

    public void setOrientationAsEnum(TipOrientation orientation) {
        currentOrientation = orientation;
        updateTextPosition();
    }


    public void updateTextPosition() {
        Double[] corners = getCorners();
        Double yLength = corners[0*3 + 1] - corners[1*3 + 1];
        Double xLength = corners[0*3 + 0] - corners[3*3 + 0];

        Double[] newPosition = getTipData();

        if (currentOrientation == TipOrientation.BOTTOM_LEFT ||
            currentOrientation == TipOrientation.BOTTOM_RIGHT) {
            newPosition[1] += yLength;
        }
        if (currentOrientation == TipOrientation.TOP_LEFT ||
            currentOrientation == TipOrientation.BOTTOM_LEFT) {
            newPosition[0] += xLength;
        }
        setPosition(newPosition);
    }

    void updateText() {
       String[] textArray = new String[]{"X:", "Y:", "Z:"};
       textArray[0] += tipTextFormat.format(tipData[0]);
       textArray[1] += tipTextFormat.format(tipData[1]);
       textArray[2] += tipTextFormat.format(tipData[2]);

       Integer[] dim = new Integer[2];
       dim[0] = use3component ? 3 : 2;
       dim[1] = 1;

       setTextArrayDimensions(dim);
       setTextStrings(textArray);
    }

 	/**
	 * @return Type as String
	 */
	public String getType() {
		return "Datatip";
	}

}
