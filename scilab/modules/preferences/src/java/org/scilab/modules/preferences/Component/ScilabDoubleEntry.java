/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.preferences.Component;

import java.math.BigDecimal;
import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;

import javax.swing.InputVerifier;
import javax.swing.JFormattedTextField;

import org.scilab.modules.preferences.XChooser;
import org.scilab.modules.preferences.XCommonManager;
import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XConfigManager;
import org.w3c.dom.Node;

/**
 * {@link Entry} component which only allow a Scilab single double data text.
 */
public class ScilabDoubleEntry extends JFormattedTextField implements XComponent, XChooser {

    /*
     * Static things, modify the following lines in case of a format change
     */

    private static final DecimalFormatSymbols FORMAT_SYMBOL = DecimalFormatSymbols.getInstance();
    private static final DecimalFormat SCILAB_FORMAT = new DecimalFormat("0.0####E00", FORMAT_SYMBOL);
    private static final BigDecimal MAX_DOUBLE = BigDecimal.valueOf(Double.MAX_VALUE);

    /**
     * Validate the user entry and format it.
     *
     * Without formatting the entry, bug #7143 appears on jdk6.
     */
    private static final InputVerifier VALIDATE_POSITIVE_DOUBLE = new InputVerifier() {

        @Override
        public boolean verify(javax.swing.JComponent arg0) {
            boolean ret = false;
            JFormattedTextField textField = (JFormattedTextField) arg0;
            try {
                BigDecimal value = new BigDecimal(textField.getText());
                if (value.compareTo(BigDecimal.ZERO) >= 0 && value.compareTo(MAX_DOUBLE) <= 0) {
                    ret = true;
                }
            } catch (NumberFormatException e) {
                return ret;
            }
            return ret;

        };
    };

    /**
     * Initialize static final fields
     */
    static {
        FORMAT_SYMBOL.setDecimalSeparator('.');
        SCILAB_FORMAT.setDecimalFormatSymbols(FORMAT_SYMBOL);
        SCILAB_FORMAT.setParseIntegerOnly(false);
        SCILAB_FORMAT.setParseBigDecimal(true);
    }

    /**
     * Constructor.
     *
     * @param peer
     *            : associated view DOM node.
     */
    public ScilabDoubleEntry(final Node peer) {
        super(SCILAB_FORMAT);
        setValue(new BigDecimal(0.0));
        setInputVerifier(VALIDATE_POSITIVE_DOUBLE);

        refresh(peer);
    }

    /*
     * Actuators
     */

    public final String text() {
        return getText();
    }

    public final void text(final String text) {
        setText(text);
    }

    /*
     * XChooser implementation
     *
     * @see org.scilab.modules.preferences.XChooser
     */

    @Override
    public Object choose() {
        return getText();
    }

    /*
     * XComponent implementation
     *
     * @see org.scilab.modules.preferences.XComponent
     */

    @Override
    public String[] actuators() {
        String[] actuators = { "enable", "text", "editable" };
        return actuators;
    }

    @Override
    public void refresh(Node peer) {
        final String text = XCommonManager.getAttribute(peer, "text");
        final String previous = text();
        if (!text.equals(previous)) {
            text(text);
        }

        final String enable = XConfigManager.getAttribute(peer, "enable", "true");
        setEnabled(enable.equals("true"));
    }
}
