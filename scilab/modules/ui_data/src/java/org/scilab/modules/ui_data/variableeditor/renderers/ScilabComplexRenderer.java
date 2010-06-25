package org.scilab.modules.ui_data.variableeditor.renderers;

import javax.swing.table.DefaultTableCellRenderer;

public class ScilabComplexRenderer extends DefaultTableCellRenderer {
	
	public  ScilabComplexRenderer() {
		super();
	}

	@Override
	protected void setValue(Object value) {
		if (value == null){
			super.setValue("");
		} else if (value.toString().equals("")) {
			super.setValue("0.0");
		} else {
        	Double[] oldValue = (Double[]) value; 
        	if (oldValue[1] != 0.0) {
        		value = oldValue[0] +" + " + oldValue[1] + "i.";
        	} else {
        		value = oldValue[0];
        	}
			super.setValue(value);
		}
	}
	
}