/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.ui_data.newsfeed.actions;

import org.scilab.modules.ui_data.newsfeed.NewsFeedTab;
import org.scilab.modules.ui_data.newsfeed.NewsFeedUIMessages;

import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabKeyStroke;
import org.scilab.modules.commons.gui.ScilabLAF;

import javax.swing.ImageIcon;
import javax.swing.JButton;

public class UpdateAction extends CommonCallBack {

    private static final String LABEL = NewsFeedUIMessages.NEWS_FEED_UPDATE_LABEL;
    private static final String ICON = FindIconHelper.findIcon("view-refresh");
    private static final char MNEMONIC = 'R';
    private static final String KEY = "OSSCKEY R";


    public UpdateAction() {
        super("");
    }

    public static MenuItem createMenuItem() {
        MenuItem menuItem = ScilabMenuItem.createMenuItem();
        menuItem.setText(LABEL);
        menuItem.setMnemonic(MNEMONIC);
        menuItem.setCallback(getCallBack());
        ((SwingScilabMenuItem) menuItem.getAsSimpleMenuItem()).setAccelerator(ScilabKeyStroke.getKeyStroke(KEY));
        return menuItem;
    }

    public static JButton createPushButton() {
        JButton pushButton = new JButton();
        ScilabLAF.setDefaultProperties(pushButton);
        pushButton.setIcon(new ImageIcon(ICON));
        pushButton.setToolTipText(LABEL);
        pushButton.addActionListener(getCallBack());
        return pushButton;
    }

    public static CommonCallBack getCallBack() {
        CommonCallBack callback = null;
        try {
            callback = UpdateAction.class.getConstructor().newInstance();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return callback;
    }

    public void callBack() {
        NewsFeedTab.getInstance().updateNewsFeed();
    }
}
