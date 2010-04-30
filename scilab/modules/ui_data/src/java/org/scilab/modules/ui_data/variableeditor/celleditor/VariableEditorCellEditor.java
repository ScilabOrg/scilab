package org.scilab.modules.ui_data.variableeditor.celleditor;

import static org.scilab.modules.graph.utils.ScilabInterpreterManagement.asynchronousScilabExec;
import static org.scilab.modules.graph.utils.ScilabInterpreterManagement.buildCall;

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.text.ParseException;

import javax.swing.AbstractAction;
import javax.swing.DefaultCellEditor;
import javax.swing.JFormattedTextField;
import javax.swing.JTable;
import javax.swing.KeyStroke;

import org.scilab.modules.graph.utils.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.ui_data.variableeditor.ScilabVariableEditor;

/**
 * class VariableEditorCellEditor
 * Use when editing cells, call Scilab interpreter.
 */
public class VariableEditorCellEditor extends DefaultCellEditor {

	/**
	 * 
	 */
	private static final long serialVersionUID = 3866054279955696250L;
	
	private int row;
	private int col;
	
	/**
	 * Constructor
	 */
	public VariableEditorCellEditor() {
		super(new JFormattedTextField());
		// TODO Auto-generated constructor stub
		

		final JFormattedTextField textField = (JFormattedTextField) getComponent();
		textField.setFocusLostBehavior(JFormattedTextField.PERSIST);
		textField.getInputMap().put(KeyStroke .getKeyStroke(
				KeyEvent.VK_ENTER, 0),
		"check");
		textField.getInputMap().put(KeyStroke .getKeyStroke(
				KeyEvent.VK_TAB, 0),
		"check");
		textField.getActionMap().put("check", new AbstractAction() {
			public void actionPerformed(ActionEvent e) {
				
				final StringBuilder command = new StringBuilder();
				String variableName = ScilabVariableEditor.getVariableEditor().getVariablename();
		        String data = String.valueOf(textField.getText());
		       
		        String cellInVariable = variableName + "(" + row + "," + col + ")";
		        String cmdInExecStr = cellInVariable + " = " + data;
		        command.append("temp = " + buildCall("execstr", cmdInExecStr, "errcatch"));
		        command.append("updateEditvarValue(\"" + variableName + "\"," + row + "," + col + "," + cellInVariable + ",temp);");
				
				final ActionListener action = new ActionListener() {

					public void actionPerformed(ActionEvent e) {

					}
				};
		        
				try {
					asynchronousScilabExec(action, command.toString());
				} catch (InterpreterException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				
				try {
					textField.commitEdit();
					textField.postActionEvent();
				} catch (ParseException e1) {
					e1.printStackTrace();
				}
				
			}
		});
	}

	/**
	 * {@inheritDoc}
	 */
	public Component getTableCellEditorComponent(JTable table, Object value,
			boolean isSelected, int row, int col) {
		this.row = row + 1; // +1 because scilab index start at 1
		this.col = row + 1;
		return super.getTableCellEditorComponent(table, value, isSelected, row, col);
	}

}
