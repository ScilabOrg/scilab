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

import java.io.File;
import java.text.DecimalFormat;
import java.text.DateFormat;

import javax.swing.SwingWorker;
import javax.swing.tree.TreePath;

import org.scilab.modules.ui_data.utils.UiDataMessages;

/**
 * The tree table model abstract implementation
 * @author Calixte DENIZET
 */
public class ScilabFileBrowserModel extends AbstractScilabTreeTableModel implements ScilabTreeTableModel {

    private static final String[] names = {UiDataMessages.NAME_COLUMN,
                                           UiDataMessages.SIZE_COLUMN,
                                           UiDataMessages.TYPE_COLUMN,
                                           UiDataMessages.LASTMODIF_COLUMN};

    private static final Class[] types = {ScilabTreeTableModel.class,
                                          FileSize.class,
                                          String.class,
                                          String.class};

    private static final FileSize MINUSONE = new FileSize(-1);

    private static final DateFormat dateFormat = DateFormat.getDateTimeInstance(DateFormat.SHORT, DateFormat.MEDIUM);

    private int order = 1;
    private String baseDir = "";
    private FileNode parent;

    public ScilabFileBrowserModel() {
        super();
        setRoot(new RootNode());
    }

    public void setBaseDir(final String baseDir, final SwingScilabTreeTable stt) {
        this.baseDir = baseDir;
File f = new File(baseDir);
                setRoot(new FileNode(f));
                File parentFile = f.getParentFile();
                if (parentFile != null) {
                    parent = new ParentNode(parentFile);
                } else {
                    parent = null;
                }
		fireTreeStructureChanged(ScilabFileBrowserModel.this, new TreePath(root).getPath(), null, null);
		stt.reload();
        SwingWorker worker = new SwingWorker<Void, Void>() {
            protected Void doInBackground() throws Exception {
                

                return null;
            }

            protected void done() {

            }
        };
        worker.execute();
    }

    public String getBaseDir() {
        return baseDir;
    }

    protected File getFile(Object node) {
        FileNode fileNode = (FileNode) node;
        return fileNode.getFile();
    }

    protected Object[] getChildren(Object node) {
        FileNode fileNode = (FileNode) node;
        return fileNode.getChildren();
    }

    public int getChildCount(Object node) {
        int count = ((FileNode) node).getChildrenCount();
        if (parent == null || node != getRoot()) {
            return count;
        }
        return count + 1;
    }

    public Object getChild(Object node, int i) {
        Object ret;
        if (node == getRoot()) {
            if (parent == null) {
                ret = getChildren(node)[i];
            } else {
                if (i == 0) {
                    ret = parent;
                } else {
                    ret = getChildren(node)[i - 1];
                }
            }
        } else {
            ret = getChildren(node)[i];
        }

        return ret;
    }

    public boolean isLeaf(Object node) {
        return ((FileNode) node).isLeaf();
    }

    public int getColumnCount() {
        return names.length;
    }

    public String getColumnName(int column) {
        return names[column];
    }

    public Class getColumnClass(int column) {
        return types[column];
    }

    public Object getValueAt(Object node, int column) {
        File file = getFile(node);
        try {
            switch(column) {
            case 0:
                return file.getName();
            case 1:
                return file.isFile() ? new FileSize((int) file.length()) : MINUSONE;
            case 2:
                if (file.isFile()) {
                    String ext = FileUtils.getFileExtension(file);
                    if (ext.isEmpty()) {
                        return UiDataMessages.FILE;
                    } else {
                        return String.format(UiDataMessages.FILETYPE, FileUtils.getFileExtension(file));
                    }
                } else {
                    return UiDataMessages.DIRECTORY;
                }
            case 3:
                return dateFormat.format(file.lastModified());
            }
        }
        catch (SecurityException se) { }

        return null;
    }

    public class RootNode extends FileNode {

        public RootNode() {
            super(new File(""));
        }

        protected Object[] getChildren() {
            if (children == null) {
                try {
                    File[] files = File.listRoots();
                    if (files != null) {
                        children = orderFiles(order, files);
                    }
                } catch (SecurityException se) { }
            }

            return children;
        }
    }

    public class ParentNode extends FileNode {

        public ParentNode(File f) {
            super(f);
        }

        public boolean isLeaf() {
            return true;
        }

        public String toString() {
            return "..";
        }
    }

    public static class FileSize {

        int size;

        FileSize(int size) {
            this.size = size;
        }

        public String toString() {
            if (size < 0) {
                return "";
            }

            if (size >= 0 && size < 1000) {
                return size + " B";
            }

            DecimalFormat df = new DecimalFormat("#.#");
            if (size >= 1000 && size < 1000000) {
                return df.format(((float) size) / 1000f).toString() + " KB";
            }

            if (size >= 1000000 && size < 1000000000) {
                return df.format(((float) size) / 1000000f).toString() + " MB";
            }

            return df.format(((float) size) / 1000000000f).toString() + " GB";
        }
    }
}