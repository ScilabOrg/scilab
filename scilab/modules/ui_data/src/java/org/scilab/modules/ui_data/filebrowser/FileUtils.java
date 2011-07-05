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

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import javax.swing.Icon;
import javax.swing.ImageIcon;

public final class FileUtils {

    private static final Icon SCILABICON = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/16x16/filebrowser/icon_scilab.png");
    private static final Icon PDFICON = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/16x16/filebrowser/application-pdf.png");
    private static final Icon BINARYICON = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/16x16/filebrowser/binary.png");
    private static final Icon IMAGEICON = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/16x16/filebrowser/image-x-generic.png");
    private static final Icon TEXTICON = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/16x16/filebrowser/text-x-generic.png");
    private static final Icon HTMLICON = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/16x16/filebrowser/text-html.png");
    private static final Icon XMLICON = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/16x16/filebrowser/text-xml.png");
    private static final Icon CSVICON = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/16x16/filebrowser/text-csv.png");
    private static final Icon DIRICON = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/16x16/filebrowser/folder.png");
    private static final Icon OPENDIRICON = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/16x16/filebrowser/folder-open.png");
    private static final Icon READONLYICON = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/16x16/filebrowser/emblem-readonly.png");
    private static final Icon CLOSEDUSERHOMEICON = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/16x16/filebrowser/user-home.png");
    private static final Icon OPENUSERHOMEICON = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/16x16/filebrowser/open-user-home.png");

    private static final Map<String, Icon> extToIcon = new HashMap<String, Icon>();

    static {
        extToIcon.put("sce", SCILABICON);
        extToIcon.put("sci", SCILABICON);
        extToIcon.put("xcos", SCILABICON);
        extToIcon.put("bin", SCILABICON);
        extToIcon.put("cos", SCILABICON);
        extToIcon.put("cosf", SCILABICON);
        extToIcon.put("dem", SCILABICON);
        extToIcon.put("graph", SCILABICON);
        extToIcon.put("sav", SCILABICON);
        extToIcon.put("tst", SCILABICON);
        extToIcon.put("pdf", PDFICON);
        extToIcon.put("png", IMAGEICON);
        extToIcon.put("gif", IMAGEICON);
        extToIcon.put("jpg", IMAGEICON);
        extToIcon.put("jpeg", IMAGEICON);
        extToIcon.put("html", HTMLICON);
        extToIcon.put("htm", HTMLICON);
        extToIcon.put("xml", XMLICON);
        extToIcon.put("csv", CSVICON);
    }

    public static String getFileExtension(File f) {
        if (f.isFile()) {
            String name = f.getName();
            int pos = name.lastIndexOf(".");
            if (pos != -1) {
                return name.substring(pos + 1, name.length());
            }
        }

        return "";
    }

    /**
     * @param file the file to test
     * @return true if it is a binary file
     */
    public static boolean isBinaryFile(File f) {
        if (f.isFile() && f.canRead()) {
            try {
                BufferedReader reader = new BufferedReader(new FileReader(f));
                char[] buffer = new char[256];
                int len = reader.read(buffer, 0, 256);
                reader.close();
                int i = 0;
                if (len != -1) {
                    for (;i < len && buffer[i] != '\0'; i++);
                }

                return len != -1 && i != len;
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return false;
    }

    public static Icon getOpenDirIcon() {
        return OPENDIRICON;
    }

    public static Icon getClosedDirIcon() {
        return DIRICON;
    }

    public static Icon getClosedUserHomeIcon() {
        return CLOSEDUSERHOMEICON;
    }

    public static Icon getOpenUserHomeIcon() {
        return OPENUSERHOMEICON;
    }

    public static Icon getIconForFile(File f) {
        if (f.isDirectory()) {
            return DIRICON;
        }
	
	if (f.canRead() && !f.canWrite()) {
	    return READONLYICON;
	}

        String ext = getFileExtension(f).toLowerCase();
        Icon icon = extToIcon.get(ext);
        if (icon != null) {
            return icon;
        }

        if (f.isFile() && isBinaryFile(f)) {
            return BINARYICON;
        }

        return TEXTICON;
    }



}
