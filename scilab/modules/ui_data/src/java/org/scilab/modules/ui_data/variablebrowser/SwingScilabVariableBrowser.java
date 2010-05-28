/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.ui_data.variablebrowser;

import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.asynchronousScilabExec;

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.ui_data.datatable.SwingTableModel;

/**
 * Swing implementation of Scilab Variable browser
 * uses JTable
 */
public final class SwingScilabVariableBrowser extends SwingScilabTab implements Tab, SimpleVariableBrowser {

	private static final long serialVersionUID = 2169382559550113917L;
	private SwingTableModel<Object> dataModel;
	private JTable table;

	/**
	 * Create a JTable with data Model.
	 * @param columnsName : Titles of JTable columns.
	 */
	public SwingScilabVariableBrowser(String[] columnsName) {
		super(Messages.gettext("Variable Browser"));

		dataModel = new SwingTableModel<Object>(columnsName);

		table = new JTable(dataModel);
		table.setFillsViewportHeight(true);
		table.setAutoResizeMode(CENTER);
		table.setAutoCreateRowSorter(true);

		table.addMouseListener(new BrowseVarMouseListener());
		// Mouse selection mode
		table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

		JScrollPane scrollPane = new JScrollPane(table);
		table.setBackground(Color.WHITE);
		setContentPane(scrollPane);
	}

	/**
	 * {@inheritDoc}
	 */
	public void addInfoBar(TextBox infoBarToAdd) {
		setInfoBar(infoBarToAdd);
	}

	/**
	 * {@inheritDoc}
	 */
	public void addMenuBar(MenuBar menuBarToAdd) {
		setMenuBar(menuBarToAdd);
	}

	/**
	 * {@inheritDoc}
	 */
	public void addToolBar(ToolBar toolBarToAdd) {
		setToolBar(toolBarToAdd);
	}

	/**
	 * {@inheritDoc}
	 */
	public void setColumnNames(String[] columnNames) {
		dataModel.setColumnNames(columnNames);
	}

	/**
	 * {@inheritDoc}
	 */
	public void setData(Object[][] data) {
		dataModel.setData(data);
	}

	/**
	 * {@inheritDoc}
	 */
	public SimpleTab getAsSimpleTab() {
		return this;
	}

	/**
	 * {@inheritDoc}
	 */
	public Window getParentWindow() {
		// TODO Auto-generated method stub
		return null;
	}


	/**
	 * MouseListener inner class
	 */
	private class BrowseVarMouseListener implements MouseListener {

		/**
		 * 
		 */
		public BrowseVarMouseListener() {
			
		}

		public void mouseClicked(MouseEvent e) {
	
			if (e.getClickCount() >= 2) {

				String variableName = ((JTable)e.getSource()).getValueAt(((JTable)e.getSource()).getSelectedRow(), 1).toString();
				final ActionListener action = new ActionListener() {

					public void actionPerformed(ActionEvent e) {

					}
				};

				try {
					asynchronousScilabExec(action, "editvar(\""+variableName+"\")");
				} catch (InterpreterException e1) {
					System.err.println("An error in the interpreter has been catched: " + e1.getLocalizedMessage()); 
				}
			
			}

		}

		public void mouseEntered(MouseEvent e) {
		}

		public void mouseExited(MouseEvent e) {
		}

		public void mousePressed(MouseEvent e) {
		}

		public void mouseReleased(MouseEvent e) {

		}
	}
	

}
