/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.uiwidget;

import java.awt.BorderLayout;
import java.awt.CardLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Image;
import java.awt.Insets;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.image.BufferedImage;
import java.net.URL;
import java.net.MalformedURLException;
import java.io.File;
import java.text.DateFormat;
import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.text.NumberFormat;
import java.text.ParseException;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import javax.imageio.ImageIO;
import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.KeyStroke;
import javax.swing.border.Border;

import org.scilab.modules.uiwidget.components.UILabel;
import org.scilab.modules.uiwidget.components.UIMenuItem;
import org.scilab.modules.uiwidget.components.UITab;
import org.scilab.modules.uiwidget.components.UISplit;
import org.scilab.modules.uiwidget.components.UIButton;
import org.scilab.modules.uiwidget.components.UIToolBar;
import org.scilab.modules.uiwidget.components.UITools;
import org.scilab.modules.uiwidget.components.UIPanel;
import org.scilab.modules.uiwidget.components.UISlider;
import org.scilab.modules.uiwidget.components.UIProgressBar;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.gui.events.callback.ScilabCallBack;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

public final class StringConverters {

    private static final Map<Class, StringConverter> converters = new HashMap<Class, StringConverter>();
    private static final DateFormat dateformat = DateFormat.getDateInstance(DateFormat.LONG);

    public static final void registerConverter(final Class clazz, final StringConverter converter) {
        converters.put(clazz, converter);
    }

    public static final StringConverter getConverter(final Class clazz) {
        return converters.get(clazz);
    }

    public static final boolean containConverter(final Class clazz) {
        return converters.containsKey(clazz);
    }

    public static final <T> T getObjectFromValue(final Class<T> clazz, final String value) {
        final StringConverter converter = converters.get(clazz);
        if (converter == null) {
            // throw
        }

        return (T) converter.convert(value);
    }

    public static final <T> T getObjectFromValue(final Class<T> clazz, final String value, final T defaultValue) {
        final StringConverter converter = converters.get(clazz);
        if (converter == null) {
            // throw
        }

        return (value == null || value.isEmpty()) ? defaultValue : (T) converter.convert(value);
    }

    /**
     * Interface to implement to parse an attribute String into a Java object
     */
    public static interface StringConverter {

        /**
         * Parse a string
         * @param str the string to parse
         * @return the corresponding Object
         */
        public Object convert(String str);
    }

    static {
        converters.put(int.class, new StringConverter() {
            public Integer convert(String str) {
                if (str == null || str.isEmpty()) {
                    return Integer.MAX_VALUE;
                }
                try {
                    return Integer.parseInt(str);
                } catch (NumberFormatException e) {
                    try {
                        return (int) Double.parseDouble(str);
                    } catch (NumberFormatException ee) {
                        return new Integer(0);
                    }
                }
            }
        });
        converters.put(char.class, new StringConverter() {
            public Object convert(String str) {
                if (str.length() > 0) {
                    return str.charAt(0);
                } else {
                    return new Character((char) 0);
                }
            }
        });
        converters.put(byte.class, new StringConverter() {
            public Object convert(String str) {
                try {
                    return Byte.parseByte(str);
                } catch (NumberFormatException e) {
                    try {
                        return (byte) Double.parseDouble(str);
                    } catch (NumberFormatException ee) {
                        return new Byte((byte) 0);
                    }
                }
            }
        });
        converters.put(short.class, new StringConverter() {
            public Object convert(String str) {
                try {
                    return Short.parseShort(str);
                } catch (NumberFormatException e) {
                    try {
                        return (short) Double.parseDouble(str);
                    } catch (NumberFormatException ee) {
                        return new Short((short) 0);
                    }
                }
            }
        });
        converters.put(double.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return Double.NaN;
                }

                try {
                    return Double.parseDouble(str);
                } catch (NumberFormatException ee) {
                    return new Double((double) 0);
                }
            }
        });
        converters.put(float.class, new StringConverter() {
            public Object convert(String str) {
                try {
                    return Float.parseFloat(str);
                } catch (NumberFormatException ee) {
                    return new Float((float) 0);
                }
            }
        });
        converters.put(boolean.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return false;
                }

                str = str.toLowerCase();
                if (str.equals("on")) {
                    return true;
                }

                if (str.equals("off")) {
                    return false;
                }

                return Boolean.parseBoolean(str);
            }
        });
        converters.put(long.class, new StringConverter() {
            public Object convert(String str) {
                try {
                    return Long.parseLong(str);
                } catch (NumberFormatException e) {
                    try {
                        return (long) Double.parseDouble(str);
                    } catch (NumberFormatException ee) {
                        return new Long((long) 0);
                    }
                }
            }
        });
        converters.put(String.class, new StringConverter() {
            public Object convert(String str) {
                return str;
            }
        });
        converters.put(Color.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null) {
                    return null;
                }

                try {
                    return Color.decode(str);
                } catch (NumberFormatException e) {
                    return Color.BLACK;
                }
            }
        });
        converters.put(KeyStroke.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                String[] toks = str.split("[ +]");
                StringBuilder buffer = new StringBuilder();
                for (int i = 0; i < toks.length - 1; i++) {
                    buffer.append(toks[i].toLowerCase());
                    buffer.append(" ");
                }
                buffer.append(toks[toks.length - 1].toUpperCase());
                return KeyStroke.getKeyStroke(buffer.toString());
            }
        });
        converters.put(ImageIcon.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                if (str.lastIndexOf('.') == -1) {
                    String path = ScilabSwingUtilities.findIcon(str);
                    if (path != null) {
                        try {
                            BufferedImage img = ImageIO.read(new File(path));
                            return new ImageIcon(img);
                        } catch (Exception e) {
                            return null;
                        }
                    }
                }

                try {
                    URL url = new URL(str);
                    BufferedImage img = ImageIO.read(url);
                    return new ImageIcon(img);
                } catch (Exception e) { }

                try {
                    BufferedImage img = ImageIO.read(new File(UIWidgetTools.getFile(str).getAbsolutePath()));
                    return new ImageIcon(img);
                } catch (Exception e) {
                    return null;
                }
            }
        });
        converters.put(Icon.class, converters.get(ImageIcon.class));
        converters.put(Image.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                if (str.lastIndexOf('.') == -1) {
                    String path = ScilabSwingUtilities.findIcon(str);
                    if (path != null) {
                        try {
                            return ImageIO.read(new File(path));
                        } catch (Exception e) {
                            return null;
                        }
                    }
                }

                try {
                    URL url = new URL(str);
                    return ImageIO.read(url);
                } catch (Exception e) { }

                try {
                    return ImageIO.read(new File(UIWidgetTools.getFile(str).getAbsolutePath()));
                } catch (Exception e) {
                    return null;
                }
            }
        });
        converters.put(UILabel.Alignment.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return UILabel.Alignment.CENTER;
                }

                str = str.toLowerCase();

                if (str.equals("leading")) {
                    return UILabel.Alignment.LEADING;
                } else if (str.equals("left")) {
                    return UILabel.Alignment.LEFT;
                } else if (str.equals("right")) {
                    return UILabel.Alignment.RIGHT;
                } else if (str.equals("trailing")) {
                    return UILabel.Alignment.TRAILING;
                } else {
                    return UILabel.Alignment.CENTER;
                }
            }
        });
        converters.put(UIButton.Alignment.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return UIButton.Alignment.CENTER;
                }

                str = str.toLowerCase();

                if (str.equals("leading")) {
                    return UIButton.Alignment.LEADING;
                } else if (str.equals("left")) {
                    return UIButton.Alignment.LEFT;
                } else if (str.equals("right")) {
                    return UIButton.Alignment.RIGHT;
                } else if (str.equals("trailing")) {
                    return UIButton.Alignment.TRAILING;
                } else {
                    return UIButton.Alignment.CENTER;
                }
            }
        });
        converters.put(UITab.Placement.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return UITab.Placement.TOP;
                }

                str = str.toLowerCase();

                if (str.equals("bottom")) {
                    return UITab.Placement.BOTTOM;
                } else if (str.equals("left")) {
                    return UITab.Placement.LEFT;
                } else if (str.equals("right")) {
                    return UITab.Placement.RIGHT;
                } else {
                    return UITab.Placement.TOP;
                }
            }
        });
        converters.put(UISplit.Orientation.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return UISplit.Orientation.HORIZONTAL;
                }

                str = str.toLowerCase();

                if (str.equals("v") || str.equals("vertical")) {
                    return UISplit.Orientation.VERTICAL;
                } else {
                    return UISplit.Orientation.HORIZONTAL;
                }
            }
        });
        converters.put(UIToolBar.Orientation.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return UIToolBar.Orientation.HORIZONTAL;
                }

                str = str.toLowerCase();

                if (str.equals("v") || str.equals("vertical")) {
                    return UIToolBar.Orientation.VERTICAL;
                } else {
                    return UIToolBar.Orientation.HORIZONTAL;
                }
            }
        });
        converters.put(UISlider.Orientation.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return UISlider.Orientation.HORIZONTAL;
                }

                str = str.toLowerCase();

                if (str.equals("v") || str.equals("vertical")) {
                    return UISlider.Orientation.VERTICAL;
                } else {
                    return UISlider.Orientation.HORIZONTAL;
                }
            }
        });
        converters.put(UIProgressBar.Orientation.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return UIProgressBar.Orientation.HORIZONTAL;
                }

                str = str.toLowerCase();

                if (str.equals("v") || str.equals("vertical")) {
                    return UIProgressBar.Orientation.VERTICAL;
                } else {
                    return UIProgressBar.Orientation.HORIZONTAL;
                }
            }
        });
        converters.put(UIButton.Relief.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return UIButton.Relief.NORMAL;
                }

                str = str.toLowerCase();
                if (str.equals("none")) {
                    return UIButton.Relief.NONE;
                } else if (str.equals("half")) {
                    return UIButton.Relief.HALF;
                } else {
                    return UIButton.Relief.NORMAL;
                }
            }
        });
        converters.put(UIButton.Relief.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return UIButton.Relief.NORMAL;
                }

                str = str.toLowerCase();
                if (str.equals("none")) {
                    return UIButton.Relief.NONE;
                } else if (str.equals("half")) {
                    return UIButton.Relief.HALF;
                } else {
                    return UIButton.Relief.NORMAL;
                }
            }
        });
        converters.put(Border.class, new StringConverter() {
            public Object convert(String str) {
                return UIBorderFactory.getBorder(str);
            }
        });
        converters.put(Date.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }
                try {
                    return dateformat.parse(str);
                } catch (ParseException e) {
                    return null;
                }
            }
        });
        converters.put(Action.class, new StringConverter() {
            public Object convert(final String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }
                return new AbstractAction() {
                    public void actionPerformed(ActionEvent e) {
                        InterpreterManagement.requestScilabExec(str + "()");
                    }
                };
            }
        });
        converters.put(UITools.FontWeight.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                str = str.toLowerCase();
                return UITools.mapWeight.get(str);
            }
        });
        converters.put(UIPanel.ImageFill.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return UIPanel.ImageFill.CENTER;
                }

                str = str.toLowerCase();
                if (str.equals("fit")) {
                    return UIPanel.ImageFill.FIT;
                } else if (str.equals("repeat")) {
                    return UIPanel.ImageFill.REPEAT;
                } else {
                    return UIPanel.ImageFill.CENTER;
                }
            }
        });
        converters.put(UIMenuItem.MenuType.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return UIMenuItem.MenuType.NORMAL;
                }

                str = str.toLowerCase();
                if (str.equals("radio")) {
                    return UIMenuItem.MenuType.RADIO;
                } else if (str.equals("check")) {
                    return UIMenuItem.MenuType.CHECK;
                } else {
                    return UIMenuItem.MenuType.NORMAL;
                }
            }
        });
        converters.put(Insets.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                int[] ints = new int[4];
                String[] toks = str.split("[ ,;]");
                for (int i = 0; i < toks.length && i < 4; i++) {
                    try {
                        ints[i] = Integer.parseInt(toks[i]);
                    } catch (NumberFormatException e) {
                        try {
                            ints[i] = (int) Double.parseDouble(toks[i]);
                        } catch (NumberFormatException ee) {
                            ints[i] = 0;
                        }
                    }
                }

                return new Insets(ints[0], ints[1], ints[2], ints[3]);
            }
        });
        converters.put(Rectangle.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                int[] ints = new int[4];
                String[] toks = str.split("[ ,;]");
                for (int i = 0; i < toks.length && i < 4; i++) {
                    try {
                        ints[i] = Integer.parseInt(toks[i]);
                    } catch (NumberFormatException e) {
                        try {
                            ints[i] = (int) Double.parseDouble(toks[i]);
                        } catch (NumberFormatException ee) {
                            ints[i] = 0;;
                        }
                    }
                }

                return new Rectangle(ints[0], ints[1], ints[2], ints[3]);
            }
        });
        converters.put(Dimension.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                int[] ints = new int[2];
                String[] toks = str.split("[ ,;]");
                for (int i = 0; i < toks.length && i < 2; i++) {
                    try {
                        ints[i] = Integer.parseInt(toks[i]);
                    } catch (NumberFormatException e) {
                        try {
                            ints[i] = (int) Double.parseDouble(toks[i]);
                        } catch (NumberFormatException ee) {
                            ints[i] = 0;;
                        }
                    }
                }

                return new Dimension(ints[0], ints[1]);
            }
        });
        converters.put(Point.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                int[] ints = new int[2];
                String[] toks = str.split("[ ,;]");
                for (int i = 0; i < toks.length && i < 2; i++) {
                    try {
                        ints[i] = Integer.parseInt(toks[i]);
                    } catch (NumberFormatException e) {
                        try {
                            ints[i] = (int) Double.parseDouble(toks[i]);
                        } catch (NumberFormatException ee) {
                            ints[i] = 0;;
                        }
                    }
                }

                return new Point(ints[0], ints[1]);
            }
        });
        converters.put(int[].class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                String[] toks = str.split("[ ,;]");
                int[] ints = new int[toks.length];
                for (int i = 0; i < toks.length; i++) {
                    try {
                        ints[i] = Integer.parseInt(toks[i]);
                    } catch (NumberFormatException e) {
                        try {
                            ints[i] = (int) Double.parseDouble(toks[i]);
                        } catch (NumberFormatException ee) {
                            ints[i] = 0;
                        }
                    }
                }

                return ints;
            }
        });
        converters.put(double[].class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                String[] toks = str.split("[ ,;]");
                double[] dbls = new double[toks.length];
                for (int i = 0; i < toks.length; i++) {
                    try {
                        dbls[i] = Double.parseDouble(toks[i]);
                    } catch (NumberFormatException e) {
                        dbls[i] = 0;
                    }
                }

                return dbls;
            }
        });
        converters.put(String[].class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                return new String[] { str };
            }
        });
        converters.put(Position.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return new Position(0, 0);
                }

                int[] ints = new int[2];
                String[] toks = str.split("[ ,;]");
                for (int i = 0; i < toks.length && i < 2; i++) {
                    try {
                        ints[i] = Integer.parseInt(toks[i]);
                    } catch (NumberFormatException e) {
                        try {
                            ints[i] = (int) Double.parseDouble(toks[i]);
                        } catch (NumberFormatException ee) {
                            ints[i] = 0;;
                        }
                    }
                }

                return new Position(ints[0], ints[1]);
            }
        });
        converters.put(Size.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return new Size(0, 0);
                }

                int[] ints = new int[2];
                String[] toks = str.split("[ ,;]");
                for (int i = 0; i < toks.length && i < 2; i++) {
                    try {
                        ints[i] = Integer.parseInt(toks[i]);
                    } catch (NumberFormatException e) {
                        try {
                            ints[i] = (int) Double.parseDouble(toks[i]);
                        } catch (NumberFormatException ee) {
                            ints[i] = 0;;
                        }
                    }
                }

                return new Size(ints[0], ints[1]);
            }
        });
        converters.put(CommonCallBack.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null) {
                    return null;
                }

                return ScilabCallBack.create(str);
            }
        });
        converters.put(DecimalFormat.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null) {
                    return null;
                }

                DecimalFormat format;
                if (str.isEmpty()) {
                    format = new DecimalFormat();
                }

                try {
                    format = new DecimalFormat(str);
                } catch (IllegalArgumentException e) {
                    format = (DecimalFormat) NumberFormat.getNumberInstance();
                }

                DecimalFormatSymbols symbols = format.getDecimalFormatSymbols();
                symbols.setDecimalSeparator('.');
                symbols.setGroupingSeparator('\u00A0');

                format.setDecimalFormatSymbols(symbols);

                return format;
            }
        });
        converters.put(UIComponent.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                return UILocator.get(str);
            }
        });
    }
}
