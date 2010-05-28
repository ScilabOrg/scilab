package org.scilab.modules.ui_data.variableeditor.renderers;

import javax.swing.table.DefaultTableCellRenderer;

public class RendererFactory {
	
	public static DefaultTableCellRenderer createRenderer(Object[][] data){
		
		if (data instanceof String[][]) {
			return new ScilabStringRenderer();
		} else if (data instanceof Double[][]) {
			return new ScilabDoubleRenderer();
		} else if (data instanceof Boolean[][]) {
			return new ScilabBooleanRenderer();
		}
		return new ScilabStringRenderer();
		
	}

}
