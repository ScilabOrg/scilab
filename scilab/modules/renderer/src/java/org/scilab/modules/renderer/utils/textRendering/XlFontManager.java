/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.renderer.utils.textRendering;

/**
 * Class to manage FontManager with xlfont in scilab
 * @author Allan CORNET
 *
 */

public class XlFontManager {

    private static FontManager sciFontManager;

    /**
     * default constructor
     */
    public XlFontManager() {
        sciFontManager = FontManager.getSciFontManager();
    }

    /**
     * Returns fonts name of available fonts on system
     * @return array of string
     */
    public String[] getAvailableFontsName() {
        return sciFontManager.getAvailableFontsName();
    }

    /**
     * Returns number of available fonts
     * @return number of available fonts
     */
    public int getSizeAvailableFontsName() {
        return sciFontManager.getSizeAvailableFontsName();
    }

    /**
     * Checks if font name is available
     * @param fontname  font name
     * @return true or false
     */
    public boolean isAvailableFontName(String fontname) {
        return sciFontManager.isAvailableFontName(fontname);
    }

    /**
     * Returns fonts name installed
     * @return array of String
     */
    public String[] getInstalledFontsName() {
        return sciFontManager.getInstalledFontsName();
    }

    /**
     * Returns number of installed fonts
     * @return number of installed fonts
     */
    public int getSizeInstalledFontsName() {
        return sciFontManager.getSizeInstalledFontsName();
    }

    /**
     * add font
     * @param fontName font name
     * @return index
     */
    public int addFont(String fontName) {
        return sciFontManager.addFont(fontName);
    }

    /**
     * add font loaded from a filename
     * @param fontFilename String filename
     * @return index
     */
    public int addFontFromFilename(String fontFilename) {
        return sciFontManager.addFontFromFilename(fontFilename);
    }

    /**
     * Change Font
     * @param index index of font to replace
     * @param fontName font name
     * @return index
     */
    public int changeFont(int index, String fontName) {
        return sciFontManager.changeFont(index, fontName);
    }

    /**
     * Change Font loaded from a filename
     * @param index index of font to replace
     * @param fontFilename filename
     * @return index
     */
    public int changeFontFromFilename(int index, String fontFilename) {
        return sciFontManager.changeFontFromFilename(index, fontFilename);
    }

    /**
     * Change Font
     * @param index index of font to replace
     * @param fontName  font name
     * @param isBold true or false
     * @param isItalic true or false
     * @return index
     */
    public int changeFontWithProperty(int index, String fontName, boolean isBold, boolean isItalic) {
        return sciFontManager.changeFont(index, fontName, isBold, isItalic);
    }

    /**
     * reset XlFontManager with initial value
     */
    public void resetXlFontManager() {
        sciFontManager.initializeFontManager();
    }

}
