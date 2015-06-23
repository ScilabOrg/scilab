/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.palette.actions;

import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.lang.reflect.InvocationTargetException;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JTextField;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Search Palettes Action
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class SearchAction extends CommonCallBack {

    private static final long serialVersionUID = 1L;

    private static final String LABEL_SEARCH = XcosMessages.SEARCH;
    private static final String ICON_SEARCH = FindIconHelper.findIcon("system-search");

    private static JButton btnSearch;
    private static JTextField txtSearch;
    private static JPanel searchBar;

    /**
     * Constructor
     */
    public SearchAction() {
        super("");
    }

    /**
     * Create the search bar
     * @return JPanel
     */
    public static JPanel createSearchBar() {
        searchBar = new JPanel();
        searchBar.setLayout(new FlowLayout(FlowLayout.LEFT, 0, 0));

        txtSearch = new JTextField();
        txtSearch.setPreferredSize(new Dimension(150, 22));
        searchBar.add(txtSearch);

        btnSearch = new JButton();
        ScilabLAF.setDefaultProperties(btnSearch);
        btnSearch.setIcon(new ImageIcon(ICON_SEARCH));
        btnSearch.setToolTipText(LABEL_SEARCH);
        btnSearch.addActionListener(getCallBack());
        btnSearch.setFocusable(true);
        btnSearch.setBorderPainted(false);
        searchBar.add(btnSearch);

        return searchBar;
    }

    /**
     * Create a new class instance
     * @return the instance
     */
    private static CommonCallBack getCallBack() {
        CommonCallBack callback = null;
        try {
            callback = SearchAction.class.getConstructor().newInstance();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        }
        return callback;
    }

    /**
     * Action
     * @param e ActionEvent
     */
    public void actionPerformed(ActionEvent e) {
        // TODO
    }

    @Override
    public void callBack() {
    }
}
