/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes;

import java.util.Map;
import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;
import java.awt.Color;
import java.awt.Font;

import javax.swing.text.ViewFactory;
import javax.swing.text.View;
import javax.swing.text.Element;

import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;

/**
 * The class ScilabContext provides a context to render a Scilab's document.
 * @author Calixte DENIZET
 */
public class ScilabContext implements ViewFactory {

    /**
     * Contains the colors of the different tokens
     */
    Color[] tokenColors;

    /**
     * Contains the fonts of the different tokens
     */
    Font[] tokenFonts;

    /**
     * Contains the attrib (underline or stroke) of the different tokens
     */
    int[] tokenAttrib;

    private View view;
    private boolean plain;
    private List<Integer> typeToDefault = new ArrayList();

    /**
     * The constructor
     */
    public ScilabContext() {
        super();
        genColors();
        genFonts();
        genAttributes();
    }

    /**
     * Constructor
     * @param plain boolean for a plain view
     */
     public ScilabContext(boolean plain) {
        this();
        this.plain = plain;
    }

    /**
     * Generate an attribute for a type of keyword
     * @param keyword the name can be found in scinotesConfiguration.xml
     * @param type the type to use
     */
    public void genAttribute(String keyword, int type) {
        tokenAttrib[ScilabLexerConstants.TOKENS.get(keyword)] = type;
        tokenAttrib[ScilabLexerConstants.OSKEYWORD] = tokenAttrib[ScilabLexerConstants.SKEYWORD];
        if (ScilabLexerConstants.TOKENS.get(keyword) == ScilabLexerConstants.DEFAULT) {
            for (Integer i : typeToDefault) {
                tokenAttrib[i] = tokenAttrib[0];
            }
        }
    }

    /**
     * Generate attributes to use to render the document
     */
    public void genAttributes() {
        tokenAttrib = new int[ScilabLexerConstants.NUMBEROFTOKENS];
        Map map = ConfigSciNotesManager.getAllAttributes();
        Iterator it = map.keySet().iterator();
        while (it.hasNext()) {
            String tokenType = (String) it.next();
            tokenAttrib[ScilabLexerConstants.TOKENS.get(tokenType)] = ((Integer) map.get(tokenType)).intValue();
        }

        for (Integer i : typeToDefault) {
            tokenAttrib[i] = tokenAttrib[0];
        }
        tokenAttrib[ScilabLexerConstants.OSKEYWORD] = tokenAttrib[ScilabLexerConstants.SKEYWORD];
    }

    /**
     * Generate the colors to use to render the document
     */
    public void genColors() {
        tokenColors = new Color[ScilabLexerConstants.NUMBEROFTOKENS];
        Map map = ConfigSciNotesManager.getAllForegroundColors();
        Iterator it = map.keySet().iterator();
        while (it.hasNext()) {
            String tokenType = (String) it.next();
            tokenColors[ScilabLexerConstants.TOKENS.get(tokenType)] = (Color) map.get(tokenType);
        }

        tokenColors[ScilabLexerConstants.OSKEYWORD] = Color.BLACK;

        typeToDefault.clear();
        for (int i = 0; i < tokenColors.length; i++) {
            if (tokenColors[i] == null) {
                tokenColors[i] = tokenColors[0];
                typeToDefault.add(i);
            }
        }

        /* Special case : Scilab's developers in comments */
        tokenColors[ScilabLexerConstants.AUTHORS] = tokenColors[ScilabLexerConstants.COMMENT];
        tokenColors[ScilabLexerConstants.OSKEYWORD] = tokenColors[ScilabLexerConstants.SKEYWORD];
    }

    /**
     * Generate a color for a type of keyword
     * @param name the name can be found in scinotesConfiguration.xml
     * @param color the color to use
     */
    public void genColors(String name, Color color) {
        Map map = ConfigSciNotesManager.getAllForegroundColors();
        if (tokenColors == null) {
            genColors();
        }

        tokenColors[ScilabLexerConstants.TOKENS.get(name)] = color;
        tokenColors[ScilabLexerConstants.AUTHORS] = tokenColors[ScilabLexerConstants.COMMENT];
        tokenColors[ScilabLexerConstants.OSKEYWORD] = tokenColors[ScilabLexerConstants.SKEYWORD];

        if (ScilabLexerConstants.TOKENS.get(name) == ScilabLexerConstants.DEFAULT) {
            for (Integer i : typeToDefault) {
                tokenColors[i] = tokenColors[0];
            }
        }
    }

    /**
     * Generate a font for a type of keyword
     * @param name the name can be found in scinotesConfiguration.xml
     * @param type the type to use
     */
    public void genFont(String name, int type) {
        Font font = tokenFonts[ScilabLexerConstants.TOKENS.get(name)];
        int style = font.getStyle();
        switch (type) {
        case -2 :
            font = font.deriveFont(style & ~Font.ITALIC);
            break;
        case -1 :
            font = font.deriveFont(style & ~Font.BOLD);
            break;
        case 1 :
            font = font.deriveFont(style | Font.BOLD);
            break;
        case 2 :
            font = font.deriveFont(style | Font.ITALIC);
            break;
        default :
        }

        tokenFonts[ScilabLexerConstants.TOKENS.get(name)] = font;
        tokenFonts[ScilabLexerConstants.OSKEYWORD] = tokenFonts[ScilabLexerConstants.SKEYWORD];
        if (ScilabLexerConstants.TOKENS.get(name) == ScilabLexerConstants.DEFAULT) {
            for (Integer i : typeToDefault) {
                tokenFonts[i] = tokenFonts[0];
            }
        }
    }

    /**
     * Generate the fonts to use to render the document
     */
    public void genFonts() {
        genFonts(null);
    }

    /**
     * Generate the fonts to use to render the document
     * @param font the base font to use
     */
    public void genFonts(Font font) {
        Map map;
        if (font != null) {
            map = ConfigSciNotesManager.getAllFontStyle(font);
        } else {
            map = ConfigSciNotesManager.getAllFontStyle();
        }

        tokenFonts = new Font[ScilabLexerConstants.NUMBEROFTOKENS];

        Iterator it = map.keySet().iterator();
        while (it.hasNext()) {
            String tokenType = (String) it.next();
            tokenFonts[ScilabLexerConstants.TOKENS.get(tokenType)] = (Font) map.get(tokenType);
        }

        for (int i = 0; i < tokenFonts.length; i++) {
            if (tokenFonts[i] == null) {
                tokenFonts[i] = tokenFonts[0];
            }
        }

        /* Special case : Scilab's developers in comments */
        Font c = tokenFonts[ScilabLexerConstants.COMMENT];
        int style = c.getStyle();
        if (c.isBold()) {
            tokenFonts[ScilabLexerConstants.AUTHORS] = c.deriveFont(style ^ Font.BOLD);
        } else {
            tokenFonts[ScilabLexerConstants.AUTHORS] = c.deriveFont(style | Font.BOLD);
        }

        tokenFonts[ScilabLexerConstants.OSKEYWORD] = tokenFonts[ScilabLexerConstants.SKEYWORD];
    }

    /**
     * @return the view to use to render the document
     */
    public View getCurrentView() {
        return view;
    }

    /**
     * Create a view with a given element
     * @param elem the Element to view
     * @return the view associated with the element
     */
    public View create(Element elem) {
        if (plain) {
            view = new ScilabPlainView(elem, this);
            ((ScilabPlainView) view).setDefaultTabRepresentation();
        } else {
            view = new ScilabView(elem, this);
            ((ScilabView) view).setDefaultTabRepresentation();
        }
        return view;
    }
}
