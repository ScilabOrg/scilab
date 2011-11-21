/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.graph;

import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import org.scilab.modules.xcos.Xcos;

/**
 * Compare two diagrams per kind.
 * 
 * Is used to sort diagram list per distance to the root diagram.
 */
public class DiagramComparator implements Comparator<XcosDiagram> {

    public static void sort(List<XcosDiagram> diagrams) {
        Collections.sort(diagrams, new DiagramComparator());
    }

    public static void reverse(List<XcosDiagram> diagrams) {
        Collections.sort(diagrams,
                Collections.reverseOrder(new DiagramComparator()));
    }

    public DiagramComparator() {
    }

    @Override
    public int compare(XcosDiagram o1, XcosDiagram o2) {
        final int o1Relative;
        if (o1 instanceof SuperBlockDiagram) {
            final XcosDiagram parent = Xcos.findParent(((SuperBlockDiagram) o1)
                    .getContainer());
            o1Relative = compare(o1, parent) + 1;
        } else {
            o1Relative = 0;
        }

        final int o2Relative;
        if (o2 instanceof SuperBlockDiagram) {
            final XcosDiagram parent = Xcos.findParent(((SuperBlockDiagram) o1)
                    .getContainer());
            o2Relative = compare(parent, o2) - 1;
        } else {
            o2Relative = 0;
        }

        return o1Relative - o2Relative;
    }

}
