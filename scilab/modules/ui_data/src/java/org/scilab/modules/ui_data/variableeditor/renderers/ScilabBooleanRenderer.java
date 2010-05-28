package org.scilab.modules.ui_data.variableeditor.renderers;

import javax.swing.table.DefaultTableCellRenderer;

public class ScilabBooleanRenderer extends DefaultTableCellRenderer {
	
	public  ScilabBooleanRenderer() {
		super();
	}

	@Override
	protected void setValue(Object value) {
		if (value == null){
			super.setValue("");
		} else if (value.toString().equals("")) {
			super.setValue(0);
		} else if (value.toString() == "false") {
			super.setValue(0);
		} else {
			super.setValue(1);
		}
	}
	
}
