/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Pierre GRADIT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.utils.Component;

import org.scilab.modules.gui.utils.XCommonManager;
import org.scilab.modules.gui.utils.XComponent;
import org.scilab.modules.gui.utils.XConfigManager;
import javax.swing.JLabel;
import java.awt.Color;
import java.awt.Font;

import org.w3c.dom.Node;

/** Implementation of Label compliant with extended management.
*
* @author Pierre GRADIT
*
*/
public class Label extends JLabel implements XComponent {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = -4842434795956015957L;

    /** Define the set of actuators.
    *
    * @return array of actuator names.
    */
    public final String [] actuators() {
        String [] actuators = {"text", "foreground", "background"};
        return actuators;
    }

    /** Constructor.
    *
    * @param peer : associated view DOM node.
    */
    public Label(final Node peer) {
        super();

        String text = XConfigManager.getAttribute(peer , "text");
        text(text);

        String background = XConfigManager.getAttribute(peer , "background");
        background(background);

        String foreground = XConfigManager.getAttribute(peer , "foreground");
        foreground(foreground);

        String fontFamily = XConfigManager.getAttribute(peer , "font-family", fontFamily());
        fontFamily(fontFamily);

        String fontFace = XConfigManager.getAttribute(peer , "font-face", fontFace());
        fontFace(fontFace);

        String fontSize = XConfigManager.getAttribute(peer , "font-size", fontSize());
        fontSize(fontSize);

    }

    /** Refresh the component by the use of actuators.
    *
    * @param peer the corresponding view DOM node
    */
    public final void refresh(final Node peer) {

        String text = XConfigManager.getAttribute(peer , "text");
        if (!text.equals(text())) {
            text(text);
        }

        String background = XConfigManager.getAttribute(peer , "background");
        if (!background.equals(foreground())) {
            background(background);
        }

        String foreground = XConfigManager.getAttribute(peer , "foreground");
        if (!foreground.equals(foreground())) {
            foreground(foreground);
        }

        String fontFamily = XConfigManager.getAttribute(peer , "font-family", fontFamily());
        if (!fontFamily.equals(fontFamily())) {
            fontFamily(fontFamily);
        }

        String fontFace = XConfigManager.getAttribute(peer , "font-face", fontFace());
        if (!fontFace.equals(fontFace())) {
            foreground(fontFace);
        }

        String fontSize = XConfigManager.getAttribute(peer , "font-size", fontSize());
        if (!fontSize.equals(fontSize())) {
        	fontSize(fontSize);
        }

    }

    /** Sensor for 'text' attribute.
    *
    * @return the attribute value.
    */
    public final String text() {
        return getText();
    }

    /** Sensor for 'foreground' attribute.
    *
    * @return the attribute value.
    */
    public final String foreground() {
        if (getForeground().equals(getParent().getForeground())) {
            return XConfigManager.NAV;
        } else {
            return XConfigManager.getColor(getForeground());
        }
    }

    /** Sensor for 'background' attribute.
    *
    * @return the attribute value.
    */
    public final String background() {
        if (getForeground().equals(getParent().getBackground())) {
            return XConfigManager.NAV;
        } else {
            return XConfigManager.getColor(getBackground());
        }
    }

    /** Sensor for 'font-family' attribute.
    *
    * @return the attribute value.
    */
    public final String fontFamily() {
        return getFont().getFamily();
    }

    /** Sensor for 'font-size' attribute.
    *
    * @return the attribute value.
    */
    public final String fontSize() {
        return "" + getFont().getSize();
    }

    /** Sensor for 'font-face' attribute.
    *
    * @return the attribute value.
    */
    public final String fontFace() {
        switch (getFont().getStyle()) {
        case Font.PLAIN :  return "plain";
        case Font.ITALIC : return "italic";
        case Font.BOLD :   return "bold";
        case 3 :           return "bold italic";
        }
        return XCommonManager.NAV;
    }

    /** Actuator for 'font-family' attribute.
    *
    * @return the attribute value.
    */
    public final void fontFamily(String fontFamily) {
        Font font = new Font(
            fontFamily,
            getFont().getStyle(),
            getFont().getSize());
        setFont(font);
    }

    /** Actuator for 'font-size' attribute.
    *
    * @return the attribute value.
    */
    public final void fontSize(String fontSize) {
        Font font = new Font(
            getFont().getFamily(), 
            getFont().getStyle(), 
            Integer.parseInt(fontSize));
        setFont(font);
    }

    /** Accelerator for 'font-face' actuator.
     * 
     * @param xFace
     * @return
     */
    private int getStyle(String xFace) {
        if (xFace.equals("plain"))       return Font.PLAIN;
        if (xFace.equals("italic"))      return Font.ITALIC;
        if (xFace.equals("bold"))        return Font.BOLD;
        if (xFace.equals("bold italic")) return Font.BOLD + Font.ITALIC;
        return -1;
    }
    /** Actuator for 'font-face' attribute.
    *
    * @return the attribute value.
    */
    public final void fontFace(String fontFace) {
        Font font = new Font(
                getFont().getFamily(), 
                getStyle(fontFace), 
                getFont().getSize());
        setFont(font);
    }

    /** Actuator for 'text' attribute.
    *
    * @param text : the attribute value.
    */
    public final void text(final String text) {
        setText(text);
    }

    /** Actuator for 'foreground' attribute.
    *
    * @param foreground : the attribute value.
    */
    public final void foreground(final String foreground) {
        if (foreground.equals(XConfigManager.NAV)) {
            setForeground(null);
        } else {
            Color color = XConfigManager.getColor(foreground);
            //System.err.println("foreground:" + actuator + " = " + color);
            setForeground(color);
        }
    }

    /** Actuator for 'background' attribute.
    *
    * @param background : the attribute value.
    */
    public final void background(final String background) {
        if (background.equals(XConfigManager.NAV)) {
            setOpaque(false);
            setBackground(null);
        } else {
            Color color = XConfigManager.getColor(background);
            //System.err.println("background:" + actuator + " = " + color);
            setOpaque(true);
            setBackground(color);
        }
    }

   /** Developer serialization method.
    *
    * @return equivalent signature.
    */
    public final String toString() {
        String signature = "Label";

        if (!text().equals(XConfigManager.NAV)) {
            signature += " text='" + text() + "'";
        }
        if (!background().equals(XConfigManager.NAV)) {
            signature += " background='" + background() + "'";
        }
        if (!foreground().equals(XConfigManager.NAV)) {
            signature += " foreground='" + foreground() + "'";
        }
        if (!fontFamily().equals(XConfigManager.NAV)) {
            signature += " font-family='" + fontFamily() + "'";
        }
        if (!fontFace().equals(XConfigManager.NAV)) {
            signature += " font-face='" + fontFace() + "'";
        }
        if (!fontSize().equals(XConfigManager.NAV)) {
            signature += " font-size='" + fontSize() + "'";
        }
        return signature;
    }
}

