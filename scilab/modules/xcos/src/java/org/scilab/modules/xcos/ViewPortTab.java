package org.scilab.modules.xcos;

import java.util.UUID;

import org.flexdock.docking.Dockable;
import org.flexdock.docking.DockingManager;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.swing.mxGraphOutline;

public final class ViewPortTab extends SwingScilabTab implements Tab {
    private ViewPortTab(XcosDiagram graph, String uuid) {
        super(XcosMessages.VIEWPORT, uuid);

        graph.setViewPortTab(uuid);
        setWindowIcon(Xcos.ICON.getImage());

        initComponents(graph);
    }

    /*
     * Static API for Tabs
     */

    /**
     * Get the viewport for a graph.
     * 
     * @param graph
     *            the graph
     * @return the view port
     */
    public static ViewPortTab get(XcosDiagram graph) {
        final String uuid = graph.getViewPortTab();
        if (uuid == null) {
            return null;
        }
        return (ViewPortTab) Xcos.getInstance().getTab(uuid);
    }

    /**
     * Restore or create the viewport tab for the graph
     * 
     * @param graph
     *            the graph
     */
    public static void restore(XcosDiagram graph) {
        String uuid = graph.getViewPortTab();
        if (uuid == null) {
            uuid = UUID.randomUUID().toString();
        }

        ViewPortTab tab = new ViewPortTab(graph, uuid);
        tab.createDefaultWindow().setVisible(true);

        tab.setCurrent();
        ScilabTabFactory.getInstance().addToCache(tab);
        ClosingOperationsManager.addDependency(
                (SwingScilabTab) XcosTab.get(graph), (SwingScilabTab) tab);
    }

    /**
     * Close the viewport tab for the graph
     * 
     * @param graph
     *            the graph
     */
    public static void close(XcosDiagram graph) {
        final String uuid = graph.getViewPortTab();
        if (uuid == null) {
            return;
        }

        WindowsConfigurationManager.removeDependency(uuid);

        final ViewPortTab tab = (ViewPortTab) ScilabTabFactory.getInstance()
                .getFromCache(uuid);
        if (tab != null) {
            final boolean isDocked = DockingManager.isDocked((Dockable) tab);

            WindowsConfigurationManager.saveTabProperties(tab, isDocked);
            if (isDocked) {
                DockingManager.undock((Dockable) tab);
            }

            tab.close();
            ScilabTabFactory.getInstance().removeFromCache(uuid);
        }
    }

    /*
     * Specific implementation
     */

    private void initComponents(XcosDiagram graph) {
        final mxGraphOutline outline = new mxGraphOutline(
                graph.getAsComponent());
        outline.setDrawLabels(true);

        setContentPane(outline);
    }

    private Window createDefaultWindow() {
        final Window win = ScilabWindow.createWindow();

        win.addTab(this);
        return win;
    }

    /*
     * Implement Tab
     */

    @Deprecated
    @Override
    public void addToolBar(ToolBar toolBarToAdd) {
        // no toolbar
    }

    @Deprecated
    @Override
    public void addMenuBar(MenuBar menuBarToAdd) {
        // no menubar
    }

    @Deprecated
    @Override
    public void addInfoBar(TextBox infoBarToAdd) {
        // no infobar
    }

    @Deprecated
    @Override
    public SimpleTab getAsSimpleTab() {
        return this;
    }

    @Deprecated
    @Override
    public Window getParentWindow() {
        return createDefaultWindow();
    }
}
