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

package org.scilab.modules.gui.utils;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import javax.swing.Icon;
import javax.swing.JComponent;

import org.scilab.forge.jlatexmath.TeXConstants;
import org.scilab.forge.jlatexmath.TeXFormula;
import org.scilab.forge.jlatexmath.TeXIcon;
import org.scilab.forge.jlatexmath.ParseException;

/**
 * Utilities with JLaTeXMath.
 * @author Calixte DENIZET
 */
public class ScilabJLaTeXMathUtilities {

    /**
     * @param component where to set a LaTeX icon
     * @param text to use for the menu, if it's enclosed between '$' then it's interpreted as
     * a LaTeX string.
     */
    public static boolean setText(JComponent component, String text) {
        boolean b = false;
        if (text != null && text.length() != 0 && text.startsWith("$") && text.endsWith("$")) {
            try {
                TeXFormula formula = new TeXFormula(text.substring(1, text.length() - 1));
                int fontSize = component.getFont().getSize();
                setIcon(component, formula.createTeXIcon(TeXConstants.STYLE_DISPLAY, fontSize));
                b = true;
            } catch (ParseException e) {
            } catch (InvocationTargetException e) {
                e.printStackTrace();
            }
        }

        return b;
    }

    /**
     * @param component where to set a LaTeX icon
     * @param icon to set
     */
    private static void setIcon(JComponent component, Icon icon) throws InvocationTargetException {
        try {
            Class clazz = component.getClass();
            Method method = clazz.getMethod("setIcon", new Class[]{Icon.class});
            method.invoke(component, new Object[]{icon});
        } catch (NoSuchMethodException e) {
            throw new InvocationTargetException(e, "No valid method setIcon");
        } catch (IllegalAccessException e) {
            throw new InvocationTargetException(e, "The method setIcon must be public");
        } catch (InvocationTargetException e) {
            throw new InvocationTargetException(e, "The method setIcon threw an exception");
        }
    }
}
