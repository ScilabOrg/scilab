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
import java.util.Arrays;
import java.util.Comparator;
import java.util.regex.Pattern;

import javax.swing.Icon;
import javax.swing.SortOrder;

import org.scilab.modules.commons.CommonFileUtils;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.core.Scilab;
import org.scilab.modules.ui_data.utils.UiDataMessages;

public class FileNode {

    private static final int NAMEORDER = 1;
    private static final int SIZEORDER = 2;
    private static final int DATEORDER = 3;
    private static final int TYPEORDER = 4;

    private final FileFilter FILTER = new FileFilter() {
            public boolean accept(File f) {
                return (f.isFile() || f.isDirectory()) && !f.isHidden() && f.canRead() && (pat == null || pat.matcher(f.getName()).matches());
            }
        };

    private static final File userHomeDir = new File(ScilabConstants.USERHOME);
    private static boolean isWindows = Scilab.isWindowsPlateform();

    protected File file;
    protected Icon icon;
    protected Object[] children;
    protected File[] childrenFiles;
    protected long lastModified;
    protected boolean canRead;
    protected boolean canWrite;
    private boolean userHome;
    private boolean isEmpty;
    private int order = 1;
    private Pattern pat;

    public FileNode(File file) {
        this.file = file;
        init();
    }

    public void setFilter(Pattern pat) {
	this.pat = pat;
    }

    public void setOrder(int order) {
        this.order = order;
    }

    public void setOrder(int order, boolean ascending) {
        int sign = ascending ? +1 : -1;
        setOrder(sign * order);
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

    private void init() {
        this.lastModified = file.lastModified();
        this.canRead = file.canRead();
        this.canWrite = file.canWrite();
        this.userHome = file.equals(userHomeDir);
        this.isEmpty = file.isDirectory() && CommonFileUtils.isEmptyDirectory(file.getAbsolutePath()) == 1;
        if (!isEmpty) {
            this.childrenFiles = file.listFiles(FILTER);
        }
    }

    protected static FileNode[] orderFiles(int order, File[] files) {
        final Comparator alpha;
        if (isWindows) {
            alpha = new Comparator<File>() {
                public int compare(File f1, File f2) {
                    if ((f1.isFile() && f2.isFile()) || (f1.isDirectory() && f2.isDirectory())) {
                        int diff = f1.getName().compareToIgnoreCase(f2.getName());
                        if (diff == 0) {
                            diff = f1.getName().compareTo(f2.getName());
                        }
                        return diff;
                    } else if (f1.isFile()) {
                        return 1;
                    }
                    return -1;
                }

                public boolean equals(Object obj) {
                    return false;
                }
            };
        } else {
            alpha = new Comparator<File>() {
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
        }

        switch (Math.abs(order)) {
        case SIZEORDER :
            Arrays.sort(files, new Comparator<File>() {
                    public int compare(File f1, File f2) {
                        int diff = new Long(f1.length()).compareTo(f2.length());
                        if (diff == 0 || f1.isDirectory() || f2.isDirectory()) {
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
        case DATEORDER :
            Arrays.sort(files, new Comparator<File>() {
                    public int compare(File f1, File f2) {
                        int diff = new Long(f1.lastModified()).compareTo(f2.lastModified());
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

    public boolean isUserHome() {
        return userHome;
    }

    public boolean hasChanged() {
        return file.lastModified() > lastModified
            || file.canRead() != canRead
            || file.canWrite() != canWrite
            || isEmpty != (file.isDirectory() && CommonFileUtils.isEmptyDirectory(file.getAbsolutePath()) == 1);
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
        return file.isFile() || getChildrenCount() == 0;
    }

    public int getChildrenCount() {
        if (childrenFiles != null) {
            return childrenFiles.length;
        }

        return 0;
    }

    protected Object[] getChildren() {
        if (children == null && file.isDirectory()) {
            try {
                if (childrenFiles != null) {
                    children = orderFiles(order, childrenFiles);
                }
            } catch (SecurityException se) { }
        }

        return children;
    }

    public void reset() {
        children = null;
        icon = null;
        childrenFiles = null;
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
