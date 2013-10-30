/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.uiwidget.components;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;

import javax.swing.ImageIcon;
import javax.swing.JLabel;

import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.uiwidget.StringConverters;
import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

public class Image extends ScilabWidget {

    private double[] scale;
    private double[] shear;
    private double angle;
    private ImageIcon baseIcon;

    public Image(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        JLabel label = new JLabel();
        label.setOpaque(true);
        label.setBackground(Color.WHITE);

        return label;
    }

    public void setString(String[][] strs) {
        JLabel label = (JLabel) getModifiableComponent();
        String s = getSingleString(strs);
        if (s != null) {
            StringConverters.StringConverter converter = StringConverters.getConverter(ImageIcon.class);
            ImageIcon icon = (ImageIcon) converter.convert(s);
            label.setIcon(icon);
            baseIcon = icon;
            super.setString(strs);
        }
    }

    public void initialize() {
        super.initialize();
        setValue(new double[] {1, 1, 0, 0, 0});
        try {
            setRelief(ScilabRelief.RAISED);
        } catch (UIWidgetException e) { }
    }

    public void setValue(double[] value) {
        super.setValue(value);
        if (value != null) {
	    double[] v = new double[]{1, 1, 0, 0, 0};
	    for (int i = 0; i < Math.min(value.length, 5); i++) {
		v[i] = value[i];
	    }
            JLabel label = (JLabel) getModifiableComponent();
            if (v[0] == 1 && v[1] == 1 && v[1] == 0 && v[1] == 0 && v[1] == 0 && label.getIcon() != baseIcon) {
                label.setIcon(baseIcon);
            } else if (baseIcon != null) {
		int w = (int) (baseIcon.getIconWidth() * v[0]);
		int h = (int) (baseIcon.getIconHeight() * v[1]);
                AffineTransform transf = AffineTransform.getScaleInstance(v[0], v[1]);
                transf.shear(v[2], v[3]);
                transf.rotate(Math.toRadians(v[4]), w / 2, h / 2);

                BufferedImage newimg = new BufferedImage(w, h, BufferedImage.TRANSLUCENT);
                Graphics2D g2d = newimg.createGraphics();
		g2d.fillRect(0, 0, w, h);
                g2d.drawImage(baseIcon.getImage(), transf, label);
                g2d.dispose();

                label.setIcon(new ImageIcon(newimg));
            }
        }
    }
}
