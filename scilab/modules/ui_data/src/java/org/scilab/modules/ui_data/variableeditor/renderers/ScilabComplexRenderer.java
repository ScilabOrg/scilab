package org.scilab.modules.ui_data.variableeditor.renderers;

import java.text.NumberFormat;

import javax.swing.table.DefaultTableCellRenderer;

public class ScilabComplexRenderer extends DefaultTableCellRenderer {
    
    private static final NumberFormat numberFormat = NumberFormat.getInstance(); 
    
    public ScilabComplexRenderer() {
	super();
    }
    
    protected void setValue(Object value) {
	if (value == null) {
	    super.setValue("");
	} else if (value instanceof String) {
	    super.setValue(value);
	} else {
	    super.setValue(convertComplex((Double[]) value));
	}
    }
    
    public void setMaximumFractionDigits(int newValue) {
	numberFormat.setMaximumFractionDigits(newValue);
    }
	
    /**
     * Convert a finite double, %nan or +/-%inf
     * @param d the double to convert
     */
    protected static String convertDouble(Double d) {
	if (d.isNaN()) {
	    return "Nan";
	} else if (d.isInfinite()) {
	    if (d.doubleValue() < 0) {
		return "-Inf";
	    } else {
		return "Inf";
	    }
	} else {
	    return numberFormat.format(d.doubleValue());
	}
    }

    private static String convertComplex(Double[] comp) {
	String re = "";
	String im = "";
	if (comp[0] == 0 && comp[1] == 0) {
	    re = "0";
	} else {
	    if (comp[1] != 0) {
		if (comp[1] == 1) {
		    im = "+i";
		} else if (comp[1] == -1) {
		    im = "-i";
		} else if (comp[1] < 0) {
		    im = convertDouble(comp[1]) + "i";
		} else {
		    im = "+" + convertDouble(comp[1]) + "i";
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