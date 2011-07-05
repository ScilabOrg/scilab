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

import javax.swing.Icon;

import org.scilab.modules.ui_data.utils.UiDataMessages;

/**
 * The tree table model abstract implementation
 * @author Calixte DENIZET
 */
public class ScilabFileBrowserModel extends AbstractScilabTreeTableModel implements ScilabTreeTableModel {

    private static String[] names = {UiDataMessages.NAME_COLUMN,
				     UiDataMessages.SIZE_COLUMN,
				     UiDataMessages.TYPE_COLUMN,
				     UiDataMessages.LASTMODIF_COLUMN};

    private static Class[] types = {ScilabTreeTableModel.class,
                                    FileSize.class,
                                    String.class,
                                    String.class};

    private static final FileSize MINUSONE = new FileSize(-1);
    
    private static final FileFilter FILTER = new FileFilter() {
	    
	    public boolean accept(File f) {
		return !f.isHidden() && f.canRead();
	    }
	};
    
    private static final DateFormat dateFormat = DateFormat.getDateTimeInstance(DateFormat.SHORT, DateFormat.MEDIUM);

    public ScilabFileBrowserModel() {
        super(new RootNode());//new FileNode(new File(File.separator)));
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

    public static class FileNode {

        protected File file;
	protected Icon icon;
        protected Object[] children;
	protected long lastModified;
	protected boolean canRead;
	protected boolean canWrite;

        public FileNode(File file) {
            this.file = file;
	    this.lastModified = file.lastModified();
	    this.canRead = file.canRead();
	    this.canWrite = file.canWrite();
        }

        /**
         * Returns the the string to be used to display this leaf in the JTree.
         */
        public String toString() {
            return file.getName();
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
			Arrays.sort(files);
			children = new FileNode[files.length];
			for(int i = 0; i < files.length; i++) {
			    children[i] = new FileNode(files[i]);
			}
		    }
		} catch (SecurityException se) { }
	    }
	    
	    return children;
        }

	public void reset() {
	    children = null;
	    icon = null;
	    this.lastModified = file.lastModified();
	}
    }

    public static class RootNode extends FileNode {
	
	public RootNode() {
	    super(new File(""));//helo"));
	}

	protected Object[] getChildren() {
            if (children == null) {
		try {
		    File[] files = File.listRoots();
		    if (files != null) {
			Arrays.sort(files);
			children = new FileNode[files.length];
			for(int i = 0; i < files.length; i++) {
			    children[i] = new FileNode(files[i]);
			}
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