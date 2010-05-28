package org.scilab.modules.ui_data.variableeditor.renderers;

import javax.swing.table.DefaultTableCellRenderer;

public class ScilabDoubleRenderer extends DefaultTableCellRenderer {
	
	public  ScilabDoubleRenderer() {
		super();
	}

	@Override
	protected void setValue(Object value) {
		if (value == null){
			super.setValue("");
		} else if (value.toString().equals("")) {
			super.setValue(0.0);
		} else {
			super.setValue(value);
		}
	}
	
}
