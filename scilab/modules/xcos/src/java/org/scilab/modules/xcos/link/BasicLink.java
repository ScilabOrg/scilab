/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.link;

import java.awt.MouseInfo;
import java.awt.geom.Line2D;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.ScilabGraphUniqueObject;
import org.scilab.modules.graph.actions.DeleteAction;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenu;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.xcos.actions.EditFormatAction;
import org.scilab.modules.xcos.block.actions.BorderColorAction;
import org.scilab.modules.xcos.link.actions.StyleHorizontalAction;
import org.scilab.modules.xcos.link.actions.StyleStraightAction;
import org.scilab.modules.xcos.link.actions.StyleVerticalAction;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxPoint;

/**
 * Root base class for links.
 * 
 * A link is always oriented from Input to Output or from Command to Control.
 */
// CSOFF: ClassDataAbstractionCoupling
public abstract class BasicLink extends ScilabGraphUniqueObject {
    private transient int ordering;

    /**
     * Default constructor
     * 
     * @param style
     *            The style to use for this link
     */
    public BasicLink(String style) {
        super();
        setVertex(false);
        setEdge(true);
        setStyle(style);
    }

    /**
     * @param ordering
     *            a unique order number per instance
     */
    public void setOrdering(int ordering) {
        this.ordering = ordering;
    }

    /**
     * @return the unique order number per instance
     */
    public int getOrdering() {
        return ordering;
    }

    /**
     * @param index
     *            the point index to be removed
     */
    public void removePoint(int index) {
        if (getGeometry() == null || getGeometry().getPoints() == null) {
            return;
        }
        if (index < getGeometry().getPoints().size()) {
            getGeometry().getPoints().remove(index);
        }
    }

    /** @return the number of points in this link */
    public int getPointCount() {
        if (getGeometry() == null || getGeometry().getPoints() == null) {
            return 0;
        }
        return getGeometry().getPoints().size();
    }

    /**
     * Split the link points into a before and after collections
     * 
     * @param split
     *            the split point
     * @param before
     *            all points before
     * @param after
     *            all points at and after the split
     */
    public void splitPoints(mxPoint split, List<mxPoint> before,
            List<mxPoint> after) {
        if (getGeometry() == null || getGeometry().getPoints() == null) {
            return;
        }

        final mxPoint start = new mxPoint(getSource().getParent().getGeometry()
                .getX()
                + getSource().getGeometry().getX(), getSource().getParent()
                .getGeometry().getY()
                + getSource().getGeometry().getY());
        final mxPoint end = new mxPoint(getTarget().getParent().getGeometry()
                .getX()
                + getTarget().getGeometry().getX(), getTarget().getParent()
                .getGeometry().getY()
                + getTarget().getGeometry().getY());

        final ArrayList<mxPoint> work = new ArrayList<mxPoint>();
        work.add(start);
        work.addAll(getGeometry().getPoints());
        work.add(end);

        double minDist = Double.MAX_VALUE;
        int minDistIndex = 0;
        for (int i = 1; i < work.size(); i++) {
            final mxPoint p1 = work.get(i - 1);
            final mxPoint p2 = work.get(i);

            final double ptSegDistSq = Line2D.ptSegDistSq(p1.getX(), p1.getY(),
                    p2.getX(), p2.getY(), split.getX(), split.getY());

            if (ptSegDistSq < minDist) {
                minDist = ptSegDistSq;
                minDistIndex = i - 1;
            }
        }

        if (minDistIndex <= 0) {
            // first segment
            after.addAll(getGeometry().getPoints());
        } else if (minDistIndex >= work.size() - 2) {
            // last segment
            before.addAll(getGeometry().getPoints());
        } else {
            // between two points
            before.addAll(work.subList(1, minDistIndex + 1));
            after.addAll(work.subList(minDistIndex + 1, work.size() - 1));
        }
    }

    /** @return The scicos color and type values */
    public abstract double[][] getColorAndType();

    /**
     * Open the contextual menu of the link
     * 
     * @param graph
     *            The associated graph
     */
    public void openContextMenu(ScilabGraph graph) {
        ContextMenu menu = ScilabContextMenu.createContextMenu();
        menu.add(DeleteAction.createMenu(graph));
        /*--- */
        menu.getAsSimpleContextMenu().addSeparator();
        /*--- */
        Menu format = ScilabMenu.createMenu();
        format.setText(XcosMessages.FORMAT);
        format.add(BorderColorAction.createMenu(graph));
        format.add(EditFormatAction.createMenu(graph));
        menu.add(format);
        /*--- */
        menu.getAsSimpleContextMenu().addSeparator();
        /*--- */
        Menu linkStyle = ScilabMenu.createMenu();
        linkStyle.setText(XcosMessages.LINK_STYLE);
        linkStyle.add(StyleHorizontalAction.createMenu(graph));
        linkStyle.add(StyleStraightAction.createMenu(graph));
        linkStyle.add(StyleVerticalAction.createMenu(graph));

        menu.add(linkStyle);

        ((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(
                MouseInfo.getPointerInfo().getLocation().x, MouseInfo
                        .getPointerInfo().getLocation().y);

        menu.setVisible(true);
    }

    /** Invert the source and target of the link */
    public void invertDirection() {
        // invert source and destination and all points.
        mxICell linkSource = getSource();
        mxICell linkTarget = getTarget();
        List<mxPoint> points = getGeometry().getPoints();

        setSource(linkTarget);
        setTarget(linkSource);

        if (points != null) {
            Collections.reverse(points);
        }
    }

    /*
     * Overriden methods from jgraphx
     */

    /**
     * @return always true
     * @see com.mxgraph.model.mxCell#isConnectable()
     * @see org.scilab.modules.xcos.graph.XcosDiagram#isValidSource(Object)
     */
    @Override
    public boolean isConnectable() {
        return true;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        final StringBuilder str = new StringBuilder();
        final String linkSep = " -> ";

        if (getChildCount() > 0) {
            // append the label
            str.append(getChildAt(0).getValue());
        } else {
            str.append(getClass().getSimpleName());
        }
        str.append(": ");

        str.append(source);
        str.append(linkSep);

        List<mxPoint> points = getGeometry().getPoints();
        if (points != null) {
            for (mxPoint p : points) {
                str.append("[" + p.getX() + ", " + p.getY() + "]");
                str.append(linkSep);
            }
        }

        str.append(target);
        return str.toString();
    }
}
// CSON: ClassDataAbstractionCoupling
