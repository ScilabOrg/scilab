package org.scilab.modules.ui_data.variableeditor.renderers;

import javax.swing.table.DefaultTableCellRenderer;

public class ScilabStringRenderer extends DefaultTableCellRenderer {
	
	public  ScilabStringRenderer() {
		super();
	}

	@Override
	protected void setValue(Object value) {
		if (value == null){
			super.setValue("");
		} else {
			value = ((String)value).replace("\"","\"\"");
			value = ((String)value).replace("'","''");
			super.setValue("\""+ value+"\"");
		}
	}
	
}
