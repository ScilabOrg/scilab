/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clément DAVID <clement.david@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.graph.swing.handler;

import java.awt.event.MouseEvent;

import org.scilab.modules.xcos.block.SplitBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.swing.GraphComponent;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.port.BasicPort;

import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.view.mxCellState;
import com.mxgraph.view.mxGraph;


/**
 * Implement a workaround for a jgraphx bug.
 * 
 * @see <a href="http://www.jgraph.org/bugzilla/show_bug.cgi?id=47">http://www.jgraph.org/bugzilla/show_bug.cgi?id=47</a>
 */
public class ConnectPreview extends com.mxgraph.swing.handler.mxConnectPreview {
	
	/**
	 * Default constructor
	 * 
	 * @param graphComponent the default graph component
	 */
	public ConnectPreview(GraphComponent graphComponent) {
		super(graphComponent);
	}
	
	/**
	 * Create a new edge on the graph.
	 * @param startState the startState
	 * @param style the current style
	 * @return the new cell
	 * @see com.mxgraph.swing.handler.mxConnectPreview#createCell(com.mxgraph.view.mxCellState, java.lang.String)
	 */
	@Override
	protected Object createCell(mxCellState startState, String style) {
		final mxGraph graph = graphComponent.getGraph();
		final mxICell cell = ((mxICell) graph.createEdge(null, null, "",
				startState.getCell(), null, style));
		((mxICell) startState.getCell()).insertEdge(cell, true);

		return cell;
	}
	
	/**
	 * Starting a new connection by setting sourceState, startPoint and previewState.
	 * 
	 * @param e the mouse event
	 * @param startState the cell state
	 * @param style the style of the edge
	 * @see com.mxgraph.swing.handler.mxConnectPreview#start(java.awt.event.MouseEvent, com.mxgraph.view.mxCellState, java.lang.String)
	 */
	@Override
	public void start(MouseEvent e, mxCellState startState, String style) {
		final XcosDiagram graph = (XcosDiagram) graphComponent.getGraph();
		final ConnectionHandler handler = (ConnectionHandler) graphComponent.getConnectionHandler();
		
		// Split case
		if (startState.getCell() instanceof BasicLink)  {
			BasicLink lnk = (BasicLink) startState.getCell();
			handler.setResetEnable(false);
			SplitBlock split = graph.addSplitEdge(transformScreenPoint(e.getPoint().getX(), e.getPoint().getY()), lnk);
			handler.setResetEnable(true);
			super.start(e, graph.getView().getState(split), style);
		} else {
			super.start(e, startState, style);
		}
	}
	
	/**
	 * Stop the current link creation
	 * @param commit true if any modification should be performed 
	 * @param e the mouse event
	 */
	@Override
	public Object stop(boolean commit, MouseEvent e) {
		if (!commit && sourceState != null && sourceState.getCell() instanceof SplitBlock) {
			final XcosDiagram graph = (XcosDiagram) graphComponent.getGraph();
			final SplitBlock block = (SplitBlock) sourceState.getCell();
			
			BasicPort src = null;
			BasicPort trg = null;
			if (block.getIn().getEdgeCount() > 0) {
				src = (BasicPort) block.getIn().getEdgeAt(0).getTerminal(true);
			}
			if (block.getIn().getEdgeCount() > 0) {
				trg = (BasicPort) block.getOut1().getEdgeAt(0).getTerminal(false);
			}
			
			graph.getModel().beginUpdate();
			try {
				graph.removeCells(new Object[] {block}, true);
				
				if (src != null && trg != null) {
					graph.connect(src, trg, null);
				}
			} finally {
				graph.getModel().endUpdate();
			}
			
			Object ret = super.stop(false, e);
			
			if (src != null && trg != null) {
				return src.getEdgeAt(0); // the new link
			} else {
				return ret;
			}
		} else {
			return super.stop(commit, e);
		}
	}
}
