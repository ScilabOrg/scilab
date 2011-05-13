package org.scilab.modules.graph.actions;

import java.awt.Dialog;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.lang.reflect.InvocationTargetException;

import javax.swing.JDialog;
import javax.swing.JList;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.OneBlockDependantAction;
import org.scilab.modules.graph.utils.ScilabGraphMessages;
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
import com.mxgraph.view.mxGraph;

public class LayoutAction extends OneBlockDependantAction {
	/** Name of the action */
	public static final String NAME = ScilabGraphMessages.LAYOUT;
	/** Icon name of the action */
	public static final String SMALL_ICON = "edit-cut.png";
	/** Mnemonic key of the action */
	public static final int MNEMONIC_KEY = KeyEvent.VK_L;
	/** Accelerator key for the action */
	public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();
	
	/**
	 * Create a new layout action used to render the layout the entire grapĥ
	 * @param scilabGraph
	 */
	public LayoutAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
	}

	/**
	 * Create a button for a graph toolbar
	 * @param scilabGraph corresponding Scilab Graph
	 * @return the button
	 */
	public static PushButton createButton(ScilabGraph scilabGraph) {
		return createButton(scilabGraph, LayoutAction.class);
	}
	
	/**
	 * Execute layout !!!
	 * 
	 * {@inheritDoc}
	 */
	@Override
	public void actionPerformed(final ActionEvent e) {
		JDialog dialog = new JDialog();
		JList list = new JList( new Class[] {
			mxCircleLayout.class,
			mxCompactTreeLayout.class,
			mxEdgeLabelLayout.class,
			mxFastOrganicLayout.class,
			mxHierarchicalLayout.class,
			mxOrganicLayout.class,
			mxOrthogonalLayout.class,
			mxParallelEdgeLayout.class,
			mxPartitionLayout.class,
			mxStackLayout.class,
		});
		
		list.addListSelectionListener(new ListSelectionListener() {
			
			@Override
			public void valueChanged(ListSelectionEvent ev) {
				Class<? extends mxGraphLayout> klass = (Class<? extends mxGraphLayout>) ((JList) ev
						.getSource()).getModel()
						.getElementAt(ev.getFirstIndex());
				
				try {
					mxGraphLayout layout = klass.getConstructor(mxGraph.class).newInstance(LayoutAction.this.getGraph(e));
					layout.execute(getGraph(e).getDefaultParent());
					
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
		});
		
		java.awt.Container contentPane = dialog.getContentPane();
		contentPane.add(list, java.awt.BorderLayout.CENTER);
		
		dialog.pack();
		dialog.setVisible(true);
	}

}
