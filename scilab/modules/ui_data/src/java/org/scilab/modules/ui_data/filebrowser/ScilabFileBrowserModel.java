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
import java.io.FileFilter;
import java.text.DecimalFormat;
import java.text.DateFormat;
import java.util.Arrays;
import java.util.Comparator;

import javax.swing.Icon;
import javax.swing.SortOrder;

import org.scilab.modules.ui_data.utils.UiDataMessages;

/**
 * The tree table model abstract implementation
 * @author Calixte DENIZET
 */
public class ScilabFileBrowserModel extends AbstractScilabTreeTableModel implements ScilabTreeTableModel {

    private static final int NAMEORDER = 1;
    private static final int SIZEORDER = 2;
    private static final int DATEORDER = 3;
    private static final int TYPEORDER = 4;

    private static final String[] names = {UiDataMessages.NAME_COLUMN,
                                           UiDataMessages.SIZE_COLUMN,
                                           UiDataMessages.TYPE_COLUMN,
                                           UiDataMessages.LASTMODIF_COLUMN};

    private static final Class[] types = {ScilabTreeTableModel.class,
                                          FileSize.class,
                                          String.class,
                                          String.class};

    private static final FileSize MINUSONE = new FileSize(-1);

    private static final FileFilter FILTER = new FileFilter() {
            public boolean accept(File f) {
                return (f.isFile() || f.isDirectory()) && !f.isHidden() && f.canRead();
            }
        };

    private static final DateFormat dateFormat = DateFormat.getDateTimeInstance(DateFormat.SHORT, DateFormat.MEDIUM);

    private int order = 1;

    public ScilabFileBrowserModel() {
        super();
        setRoot(new RootNode());//new FileNode(new File(File.separator)));
    }

    public void setOrder(int order) {
        this.order = order;
    }

    public void setOrder(int order, boolean ascending) {
        int sign = ascending ? +1 : -1;
        this.order = sign * order;
    }

    public SortOrder getOrder() {
        return order > 0 ? SortOrder.ASCENDING : SortOrder.DESCENDING;
    }

    public void toggleSortOrder(String nameColumn) {
        if (nameColumn.equals(UiDataMessages.NAME_COLUMN)) {
            if (Math.abs(order) == NAMEORDER) {
                setOrder(-order);
            } else {
                setOrder(NAMEORDER);
            }
        } else if (nameColumn.equals(UiDataMessages.TYPE_COLUMN)) {
            if (Math.abs(order) == TYPEORDER) {
                setOrder(-order);
            } else {
                setOrder(TYPEORDER);
            }
        } else if (nameColumn.equals(UiDataMessages.SIZE_COLUMN)) {
            if (Math.abs(order) == SIZEORDER) {
                setOrder(-order);
            } else {
                setOrder(SIZEORDER);
            }
        } else if (nameColumn.equals(UiDataMessages.LASTMODIF_COLUMN)) {
            if (Math.abs(order) == DATEORDER) {
                setOrder(-order);
            } else {
                setOrder(DATEORDER);
            }
        }
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
        Object[] children = getChildren(node);
        if (children == null) {
            return 0;
        } else {
            return children.length;
        }
    }

    public Object getChild(Object node, int i) {
        return getChildren(node)[i];
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

    private final FileNode[] orderFiles(int order, File[] files) {
        final Comparator alpha = new Comparator<File>() {
            public int compare(File f1, File f2) {
                if ((f1.isFile() && f2.isFile()) || (f1.isDirectory() && f2.isDirectory())) {
                    return f1.getName().compareTo(f2.getName());
                } else if (f1.isFile()) {
                    return 1;
                }
                return -1;
            }

            public boolean equals(Object obj) {
                return false;
            }
        };

        switch (Math.abs(order)) {
        case SIZEORDER :
            Arrays.sort(files, new Comparator<File>() {
                    public int compare(File f1, File f2) {
                        long diff = f1.length() - f2.length();
                        if (diff == 0 || f1.isDirectory() || f2.isDirectory()) {
                            return alpha.compare(f1, f2);
                        } else {
                            return (int) diff;
                        }
                    }

                    public boolean equals(Object obj) {
                        return false;
                    }
                });
            break;
        case DATEORDER :
            Arrays.sort(files, new Comparator<File>() {
                    public int compare(File f1, File f2) {
                        long diff = f1.lastModified() - f2.lastModified();
                        if (diff == 0) {
                            return alpha.compare(f1, f2);
                        } else {
                            return (int) diff;
                        }
                    }

                    public boolean equals(Object obj) {
                        return false;
                    }
                });
            break;
        case TYPEORDER :
            Arrays.sort(files, new Comparator<File>() {
                    public int compare(File f1, File f2) {
                        int diff = FileUtils.getFileExtension(f1).compareTo(FileUtils.getFileExtension(f2));
                        if (diff == 0) {
                            return alpha.compare(f1, f2);
                        } else {
                            return diff;
                        }
                    }

                    public boolean equals(Object obj) {
                        return false;
                    }
                });
            break;
        default :
            Arrays.sort(files, alpha);
        }

        FileNode[] nodes = new FileNode[files.length];
        if (order > 0) {
            for (int i = 0; i < files.length; i++) {
                nodes[i] = new FileNode(files[i]);
            }
        } else {
            for (int i = 0; i < files.length; i++) {
                nodes[i] = new FileNode(files[files.length - 1 - i]);
            }
        }

        return nodes;
    }

    public class FileNode {

        protected File file;
        protected Icon icon;
        protected Object[] children;
        protected long lastModified;
        protected boolean canRead;
        protected boolean canWrite;

        public FileNode(File file) {
            this.file = file;
            init();
        }

        private void init() {
            this.lastModified = file.lastModified();
            this.canRead = file.canRead();
            this.canWrite = file.canWrite();
        }

        /**
         * Returns the the string to be used to display this leaf in the JTree.
         */
        public String toString() {
            String name = file.getName();
            if (name.isEmpty()) {
                name = "/";
            }
            return name;
        }

        public File getFile() {
            return file;
        }

        public boolean hasChanged() {
            return file.lastModified() > lastModified || file.canRead() != canRead || file.canWrite() != canWrite;
        }

        public long getLastModified() {
            return lastModified;
        }

        public Icon getIcon() {
            if (icon == null) {
                icon = FileUtils.getIconForFile(file);
            }

            return icon;
        }

        public boolean isLeaf() {
            Object[] c = getChildren();
            return file.isFile() || c == null || c.length == 0;
        }

        protected Object[] getChildren() {
            if (children == null && file.isDirectory()) {
                try {
                    File[] files = file.listFiles(FILTER);
                    if (files != null) {
                        children = orderFiles(order, files);
                    }
                } catch (SecurityException se) { }
            }

            return children;
        }

        public void reset() {
            children = null;
            icon = null;
            init();
        }

        public void resetChildren() {
            children = null;
        }

        public boolean equals(Object o) {
            return (o instanceof FileNode) && ((FileNode) o).file.equals(file);
        }

	public int hashCode() {
	    return file.hashCode();
	}
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