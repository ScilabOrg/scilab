/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.gui.plotbrowser;

import java.util.List;
import java.util.ListIterator;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.utils.ClosingOperationsManager;

/**
* Class to create PlotBrowser instances
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class PlotBrowserTab {

    public static SwingScilabDockablePanel getPlotBrowserInstance(Integer objectID) {
        final SwingScilabDockablePanel plotbrowser = PlotBrowser
                                           .createPlotBrowserTab(objectID);
        ScilabTabFactory.getInstance().addToCache(plotbrowser);

        ClosingOperationsManager.registerClosingOperation(plotbrowser,
        new ClosingOperationsManager.ClosingOperation() {

            //@Override
            public int canClose() {
                return 1;
            }

            //@Override
            public void destroy() {
                PlotBrowser.closePlotBrowser();
            }

            //@Override
            public String askForClosing(final List<SwingScilabDockablePanel> list) {
                return null;
            }

            //@Override
            public void updateDependencies(List<SwingScilabDockablePanel> list,
            ListIterator<SwingScilabDockablePanel> it) {
            }
        });

        ClosingOperationsManager.addDependencyWithRoot(plotbrowser);

        return plotbrowser;
    }
}
