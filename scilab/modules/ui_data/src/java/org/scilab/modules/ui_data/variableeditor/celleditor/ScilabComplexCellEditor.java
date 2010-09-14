package org.scilab.modules.ui_data.variableeditor.celleditor;

import java.awt.Component;

import javax.swing.JTable;

public class ScilabComplexCellEditor extends ScilabGenericCellEditor {
 
    private static final long serialVersionUID = 3866044279955696250L;
    
    /**
     * {@inheritDoc}
     */
    public String getDataAsScilabString(Object value) {
        String newValue = "";
        if (value != null && value instanceof Double[]) {
	    newValue = convertComplex((Double[]) value);
        } else if (value instanceof String) {
	    newValue = (String) value;
	}

	return newValue;
    }

    /**
     * Convert a finite double, %nan or +/-%inf
     * @param d the double to convert
     */
    protected static String convertDouble(Double d) {
	if (d.isNaN()) {
	    return "%nan";
	} else if (d.isInfinite()) {
	    if (d.doubleValue() < 0) {
		return "-%inf";
	    } else {
		return "%inf";
	    }
	} else {
	    return d.toString();
	}
    }
    
    /**
     * Convert a complex into a Scilab expression
     * @param comp the complex
     * @return the string expression
     */
    private static String convertComplex(Double[] comp) {
	String re = "";
	String im = "";
	if (comp[0] == 0 && comp[1] == 0) {
	    re = "0";
	} else {
	    if (comp[1] != 0) {
		if (comp[1] == 1) {
		    im = "+%i";
		} else if (comp[1] == -1) {
		    im = "-%i";
		} else if (comp[1] < 0) {
		    im = convertDouble(comp[1]) + "*%i";
		} else {
		    im = "+" + convertDouble(comp[1]) + "*%i";
		}
	    }
	    if (comp[0] != 0) {
		re = convertDouble(comp[0]);
	    } else if (im.startsWith("+")) {
		im = im.substring(1);
	    }
	}
	
	return re + im;
    }
}