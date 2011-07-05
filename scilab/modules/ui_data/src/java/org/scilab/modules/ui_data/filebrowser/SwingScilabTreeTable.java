/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.filebrowser;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Insets;
import java.awt.Point;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.File;
import java.text.DateFormat;
import java.util.Date;
import java.util.Enumeration;
import java.util.regex.Pattern;

import javax.swing.AbstractCellEditor;
import javax.swing.ActionMap;
import javax.swing.InputMap;
import javax.swing.JLabel;
import javax.swing.JTable;
import javax.swing.JTree;
import javax.swing.Timer;
import javax.swing.border.AbstractBorder;
import javax.swing.border.Border;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableCellEditor;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.TableColumn;
import javax.swing.tree.TreePath;

import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.ui_data.filebrowser.actions.ChangeCWDAction;
import org.scilab.modules.ui_data.filebrowser.actions.EditFileWithDefaultAppAction;
import org.scilab.modules.ui_data.filebrowser.actions.ExecuteFileInConsoleAction;
import org.scilab.modules.ui_data.filebrowser.actions.ExecuteFileInXcosAction;
import org.scilab.modules.ui_data.filebrowser.actions.LoadFileAsGraphAction;
import org.scilab.modules.ui_data.filebrowser.actions.LoadFileInScilabAction;
import org.scilab.modules.ui_data.filebrowser.actions.OpenFileInSciNotesAction;
import org.scilab.modules.ui_data.filebrowser.actions.OpenFileWithDefaultAppAction;
import org.scilab.modules.ui_data.filebrowser.actions.ValidateAction;

/**
 * The tree table model abstract implementation
 * @author Calixte DENIZET
 */
public class SwingScilabTreeTable extends JTable {

    private static final Insets INSETS = new Insets(0, 2, 0, 0);
    private static final DateFormat DATEFORMAT = DateFormat.getDateTimeInstance(DateFormat.SHORT, DateFormat.MEDIUM);

    private static final Border BORDER = new AbstractBorder() {
            public void paintBorder(Component c, Graphics g, int x, int y, int width, int height) {
                g.setColor(Color.LIGHT_GRAY);
                g.drawLine(x, y, x, y + height);
            }

            public Insets getBorderInsets(Component c) {
                return INSETS;
            }

            public Insets getBorderInsets(Component c, Insets insets) {
                return INSETS;
            }
        };

    private Timer refreshTimer;
    private static int refreshTime = 1000;

    private boolean initialization = true;

    protected ScilabTreeTableCellRenderer tree;
    protected ScilabFileSelectorComboBox combobox;
    protected ScilabFileBrowserHistory history;

    public SwingScilabTreeTable(ScilabTreeTableModel treeTableModel, ScilabFileSelectorComboBox combobox) {
        super();
        this.combobox = combobox;
        combobox.setTreeTable(this);
        history = new ScilabFileBrowserHistory(this);
        tree = new ScilabTreeTableCellRenderer(this, treeTableModel);
        super.setModel(new ScilabTreeTableModelAdapter(treeTableModel, tree));

        // Install the tree editor renderer and editor.
        setDefaultRenderer(ScilabTreeTableModel.class, tree);
	setDefaultRenderer(Date.class, new DefaultTableCellRenderer() {
		{
		    setHorizontalTextPosition(DefaultTableCellRenderer.LEFT);
		}
	    
	    public Component getTableCellRendererComponent(JTable table, Object value, boolean selected, boolean focus, int row, int col) {
		JLabel label = (JLabel) super.getTableCellRendererComponent(table, value, selected, focus, row, col);
		label.setText(DATEFORMAT.format((Date) value));
		if (col == 1) {
		    label.setBorder(BORDER);
		}
		return label;
	    }
	});
	setDefaultRenderer(ScilabFileBrowserModel.FileSize.class, new DefaultTableCellRenderer() {
		{
		    setHorizontalTextPosition(DefaultTableCellRenderer.LEFT);
		}
	    
	    public Component getTableCellRendererComponent(JTable table, Object value, boolean selected, boolean focus, int row, int col) {
		Component c = super.getTableCellRendererComponent(table, value, selected, focus, row, col);
		if (col == 1) {
		    JLabel jl = (JLabel) c;
		    jl.setBorder(BORDER);
		}
		return c;
	    }
	});
	setDefaultRenderer(String.class, new DefaultTableCellRenderer() {
		{
		    setHorizontalTextPosition(DefaultTableCellRenderer.LEFT);
		}
	    
	    public Component getTableCellRendererComponent(JTable table, Object value, boolean selected, boolean focus, int row, int col) {
		Component c = super.getTableCellRendererComponent(table, value, selected, focus, row, col);
		if (col == 1) {
		    JLabel jl = (JLabel) c;
		    jl.setBorder(BORDER);
		}
		return c;
	    }
	});
	
	
        setShowGrid(false);
	setFillsViewportHeight(true);
        setIntercellSpacing(new Dimension(0, 0));
        setRowSorter(new FileBrowserRowSorter(tree, this));
        setAutoResizeMode(AUTO_RESIZE_NEXT_COLUMN);

        addMouseListener(new MouseAdapter() {
                public void mousePressed(MouseEvent e) {
                    int col = columnAtPoint(new Point(e.getX(), e.getY()));
                    if (getColumnClass(col) == ScilabTreeTableModel.class) {
                        tree.dispatchEvent(e);
                    }
                }
            });

	initActions();
        refreshTimer = new Timer(refreshTime, new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    tree.startRefresh();
                }
            });
    }

    public PushButton getNextButton() {
	return history.getNextButton();
    }

    public PushButton getPreviousButton() {
	return history.getPreviousButton();
    }

    public static void setRefreshTime(int time) {
	refreshTime = time;
    }

    public ScilabFileSelectorComboBox getComboBox() {
        return combobox;
    }

    public String[] getSelectedPaths() {
        int[] rows = getSelectedRows();
        String[] paths = new String[rows.length];
        for (int i = 0; i < rows.length; i++) {
            TreePath path = tree.getPathForRow(rows[i]);
            FileNode fn = (FileNode) path.getLastPathComponent();
            paths[i] = fn.getFile().getAbsolutePath();
        }

        return paths;
    }

    public File[] getSelectedFiles() {
        int[] rows = getSelectedRows();
        File[] files = new File[rows.length];
        for (int i = 0; i < rows.length; i++) {
            TreePath path = tree.getPathForRow(rows[i]);
            FileNode fn = (FileNode) path.getLastPathComponent();
            files[i] = fn.getFile();
        }

        return files;
    }

    public int getRowHeight(int row) {
        return getRowHeight();
    }

    public boolean isOpaque() {
        return false;
    }

    public void setBaseDir(String baseDir) {
	setBaseDir(baseDir, true);
    }

    public void setBaseDir(String baseDir, boolean addInHistory) {
        ScilabFileBrowserModel model = (ScilabFileBrowserModel) tree.getModel();
        combobox.setBaseDir(baseDir);
	File f = new File(baseDir);
        if (!baseDir.equals(model.getBaseDir()) && f.exists() && f.isDirectory() && f.canRead()) {
            refreshTimer.stop();
            tree.stopRefresh();
	    if (addInHistory) {
		history.addPathInHistory(baseDir);
	    }
            tree.collapsePath(new TreePath(model.getRoot()));
            model.setBaseDir(baseDir, this);
        }
    }

    public void setFilter(Pattern pat) {
	ScilabFileBrowserModel model = (ScilabFileBrowserModel) tree.getModel();
	refreshTimer.stop();
	tree.stopRefresh();
	tree.collapsePath(new TreePath(model.getRoot()));
	model.setFilter(pat);

	TreePath rootPath = new TreePath(model.getRoot());
        Enumeration<TreePath> en = tree.getExpandedDescendants(rootPath);
	model.fireTreeStructureChanged(model, rootPath.getPath(), null, null);
	if (en != null) {
	    while (en.hasMoreElements()) {
		tree.expandPath(en.nextElement());
	    }
	}
	reload();
    }

    public void reload() {
        ScilabFileBrowserModel model = (ScilabFileBrowserModel) tree.getModel();
        tree.setModel(model);
        tree.setRowHeight(getRowHeight());
        tree.setLargeModel(true);
        TreePath path = new TreePath(model.getRoot());
        tree.collapsePath(path);
        tree.expandPath(path);
        editingRow = 0;
        refreshTimer.start();
    }

    /* Workaround for BasicTableUI anomaly. Make sure the UI never tries to
     * paint the editor. The UI currently uses different techniques to
     * paint the renderers and editors and overriding setBounds() below
     * is not the right thing to do for an editor. Returning -1 for the
     * editing row in this case, ensures the editor is never painted.
     */
    public int getEditingRow() {
        if (getColumnClass(editingColumn) == ScilabTreeTableModel.class) {
            return -1;
        } else {
            return editingRow;
        }
    }

    private void initActions() {
        ActionMap actions = getActionMap();
        actions.put("scinotes", new OpenFileInSciNotesAction(this));
        actions.put("xcos", new ExecuteFileInXcosAction(this));
        actions.put("console", new ExecuteFileInConsoleAction(this));
        actions.put("load", new LoadFileInScilabAction(this));
        actions.put("graph", new LoadFileAsGraphAction(this));
        actions.put("cwd", new ChangeCWDAction(this));
        if (EditFileWithDefaultAppAction.isSupported()) {
            actions.put("edit", new EditFileWithDefaultAppAction(this));
        }
        if (OpenFileWithDefaultAppAction.isSupported()) {
            actions.put("open", new OpenFileWithDefaultAppAction(this));
        }
        actions.put("validate", new ValidateAction(this));

        combobox.setAction((CallBack) actions.get("cwd"));
        InputMap map = getInputMap();
        //map.put(
    }
}
