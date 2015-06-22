/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Chenfeng ZHU
 */
package org.scilab.modules.xcos.link.actions;

import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.scilab.modules.xcos.utils.XcosRoute;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxGraphModel;
import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxPoint;

/**
 * Implement the set link optimal action.
 */
@SuppressWarnings(value = { "serial" })
public class StyleOptimalAction extends StyleAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.LINK_STYLE_OPTIMAL;
    /** Icon name of the action */
    public static final String SMALL_ICON = "";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_O;

    public StyleOptimalAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * @param scilabGraph
     * @return menu item
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, StyleOptimalAction.class);
    }

    /**
     * @param e
     * @see org.scilab.modules.xcos.link.actions.StyleAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        final XcosDiagram graph = (XcosDiagram) getGraph(e);

        // action disabled when the cell is edited
        final ScilabComponent comp = ((ScilabComponent) graph.getAsComponent());
        if (comp.isEditing()) {
            return;
        }

        final Object[] links = getLinks();

        this.updateLinkOptimal(graph, links);

        // graph.getModel().beginUpdate();
        // try {
        // graph.setCellStyles(mxConstants.STYLE_NOEDGESTYLE, "1", links);
        //
        // reset(graph, links);
        // } finally {
        // graph.getModel().endUpdate();
        // }
    }

    public void updateLinkOptimal(XcosDiagram graph, Object[] links) {
        for (Object o : links) {
            if (o instanceof BasicLink) {
                BasicLink link = (BasicLink) o;
                if (link.isEdge()) {
                    this.updateRoute(link, graph);
                }
            }
        }
    }

    /**
     * Update the Edge.
     *
     * @param cell
     * @param graph
     */
    protected void updateRoute(BasicLink cell, XcosDiagram graph) {
        Object[] all = graph.getChildCells(graph.getDefaultParent());
        mxICell sourceCell = cell.getSource();
        mxICell targetCell = cell.getTarget();
        all = removeMyself(all, cell, sourceCell, targetCell);
        if (sourceCell != null && targetCell != null) {
            graph.getModel().beginUpdate();
            try {
                List<mxPoint> list = findTurningPoints(cell, all, graph);
                if (list == null) {
                    graph.setCellStyle("", new Object[] { cell });
                    // when it is "straight", use straight or horizontal line.
                    graph.setCellStyles(mxConstants.STYLE_NOEDGESTYLE, "0",
                                        new Object[] { cell });
                    graph.setCellStyles(mxConstants.STYLE_EDGE, mxConstants.EDGESTYLE_ELBOW,
                                        new Object[] { cell });
                    graph.setCellStyles(mxConstants.STYLE_ELBOW, mxConstants.ELBOW_HORIZONTAL,
                                        new Object[] { cell });
                    super.reset(graph, cell);
                    // graph.resetEdge(cell);
                } else if (list.size() == 0) {
                    // if no optimal route is found, keep the original one.
                    ;
                } else {
                    mxGeometry geometry = new mxGeometry();
                    geometry.setPoints(list);
                    ((mxGraphModel) (graph.getModel())).setGeometry(cell, geometry);
                }
            } finally {
                graph.getModel().endUpdate();
            }
        }
    }

    /**
     * Get the turning points for the optimal route. If the straight route is
     * the optimal route, return null.
     *
     * @param link
     * @param allCells
     * @return list of turning points
     */
    public List<mxPoint> findTurningPoints(BasicLink link, Object[] allCells, XcosDiagram graph) {
        List<mxPoint> listPoints = new ArrayList<mxPoint>(0);
        mxICell sourceCell = link.getSource();
        mxICell targetCell = link.getTarget();
        if (!(sourceCell instanceof BasicPort) || !(targetCell instanceof BasicPort)) {
            return null;
        }
        BasicPort sourcePort = (BasicPort) sourceCell;
        BasicPort targetPort = (BasicPort) targetCell;
        // mxGeometry srcGeometry = sourcePort.getGeometry();
        // mxGeometry tgtGeometry = targetPort.getGeometry();
        double srcx = graph.getView().getState(sourcePort).getCenterX();
        double srcy = graph.getView().getState(sourcePort).getCenterY();
        double tgtx = graph.getView().getState(targetPort).getCenterX();
        double tgty = graph.getView().getState(targetPort).getCenterY();
        // if two ports are not oblique and not in the same direction,
        // use straight route.
        if ((!XcosRoute.checkOblique(srcx, srcy, tgtx, tgty))
                && !XcosRoute.checkObstacle(srcx, srcy, tgtx, tgty, allCells)) {
            return null;
        }
        mxPoint sourcePoint1 = new mxPoint(srcx, srcy);
        mxPoint targetPoint1 = new mxPoint(tgtx, tgty);
        this.getPointAwayPort(sourcePoint1, sourcePort);
        this.getPointAwayPort(targetPoint1, targetPort);
        listPoints.add(sourcePoint1);
        XcosRoute route = new XcosRoute();
        List<mxPoint> listRoute = route.getRoute(sourcePoint1, new mxPoint(
                                      (srcx + sourcePoint1.getX()) / 2, (srcy + sourcePoint1.getY()) / 2),
                                  targetPoint1, allCells);
        Collections.reverse(listRoute);
        listPoints.addAll(listRoute);
        listPoints.add(targetPoint1);
        return listPoints;
    }

    public void getPointAwayPort(mxPoint point, BasicPort port) {
        double distance = XcosRoute.BEAUTY_DISTANCE;
        switch (port.getOrientation()) {
            case EAST:
                point.setX(point.getX() + distance);
                return;
            case SOUTH:
                point.setY(point.getY() - distance);
                return;
            case WEST:
                point.setX(point.getX() - distance);
                return;
            case NORTH:
                point.setY(point.getY() + distance);
                return;
        }
    }

    /**
     * Remove the relative cells from the array of all.
     *
     * @param all
     * @param me
     * @return a new array of all objects excluding me
     */
    public Object[] removeMyself(Object[] all, Object... me) {
        List<Object> listme = Arrays.asList(me);
        List<Object> listnew = new ArrayList<Object>(0);
        for (Object o : all) {
            if (!listme.contains(o)) {
                listnew.add(o);
            }
        }
        Object[] newAll = listnew.toArray();
        return newAll;
    }

}
