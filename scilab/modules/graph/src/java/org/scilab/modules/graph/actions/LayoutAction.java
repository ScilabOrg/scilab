package org.scilab.modules.graph.actions;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.lang.reflect.InvocationTargetException;

import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.OneBlockDependantAction;
import org.scilab.modules.graph.utils.ScilabGraphMessages;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;

import com.mxgraph.layout.mxCircleLayout;
import com.mxgraph.layout.mxCompactTreeLayout;
import com.mxgraph.layout.mxEdgeLabelLayout;
import com.mxgraph.layout.mxFastOrganicLayout;
import com.mxgraph.layout.mxGraphLayout;
import com.mxgraph.layout.mxOrganicLayout;
import com.mxgraph.layout.mxParallelEdgeLayout;
import com.mxgraph.layout.mxPartitionLayout;
import com.mxgraph.layout.mxStackLayout;
import com.mxgraph.layout.hierarchical.mxHierarchicalLayout;
import com.mxgraph.layout.orthogonal.mxOrthogonalLayout;
import com.mxgraph.swing.util.mxMorphing;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxEventSource.mxIEventListener;
import com.mxgraph.view.mxGraph;

public class LayoutAction extends OneBlockDependantAction {
    /** Name of the action */
    public static final String NAME = ScilabGraphMessages.LAYOUT;
    /** Icon name of the action */
    public static final String SMALL_ICON = "edit-cut";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_L;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

    /**
     * Create a new layout action used to render the layout the entire grapĥ
     * 
     * @param scilabGraph
     */
    public LayoutAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Menu for diagram menubar
     * 
     * @param scilabGraph
     *            associated diagram
     * @return the menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, LayoutAction.class);
    }

    /**
     * Create a button for a graph toolbar
     * 
     * @param scilabGraph
     *            corresponding Scilab Graph
     * @return the button
     */
    public static PushButton createButton(ScilabGraph scilabGraph) {
        return createButton(scilabGraph, LayoutAction.class);
    }

    private static class ListListener implements ListSelectionListener {
        final ScilabGraph graph;

        public ListListener(final ScilabGraph graph) {
            this.graph = graph;
        }

        @Override
        public void valueChanged(ListSelectionEvent ev) {
            Class<? extends mxGraphLayout> klass = (Class<? extends mxGraphLayout>) ((JList) ev.getSource()).getModel().getElementAt(ev.getFirstIndex());

            try {
                mxGraphLayout layout = klass.getConstructor(mxGraph.class).newInstance(graph);
                Object cell = graph.getSelectionCell();

                if (cell == null || graph.getModel().getChildCount(cell) == 0) {
                    cell = graph.getDefaultParent();
                }

                graph.getModel().beginUpdate();
                try {
                    layout.execute(cell);
                } finally {
                    mxMorphing morph = new mxMorphing(graph.getAsComponent(), 20, 1.2, 20);

                    morph.addListener(mxEvent.DONE, new mxIEventListener() {

                        @Override
                        public void invoke(Object sender, mxEventObject evt) {
                            graph.getModel().endUpdate();
                        }

                    });

                    morph.startAnimation();
                }

            } catch (IllegalArgumentException e1) {
                e1.printStackTrace();
            } catch (SecurityException e1) {
                e1.printStackTrace();
            } catch (InstantiationException e1) {
                e1.printStackTrace();
            } catch (IllegalAccessException e1) {
                e1.printStackTrace();
            } catch (InvocationTargetException e1) {
                e1.printStackTrace();
            } catch (NoSuchMethodException e1) {
                e1.printStackTrace();
            }
        }
    }

    /**
     * Execute layout !!!
     * 
     * {@inheritDoc}
     */
    @Override
    public void actionPerformed(final ActionEvent e) {
        final JList list = new JList(new Class[] { mxCircleLayout.class, mxCompactTreeLayout.class, mxEdgeLabelLayout.class, mxFastOrganicLayout.class,
                mxHierarchicalLayout.class, mxOrganicLayout.class, mxOrthogonalLayout.class, mxParallelEdgeLayout.class, mxPartitionLayout.class,
                mxStackLayout.class, });

        final ScilabGraph graph = getGraph(e);
        list.addListSelectionListener(new ListListener(graph));
        JOptionPane.showMessageDialog(graph.getAsComponent(), list);
    }
}
