/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.utils;

import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;

import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.xcos.graph.PaletteDiagram;
import org.scilab.modules.xcos.graph.XcosDiagram;

/**
 * Customize the default component
 *
 * @see java.awt.Component
 */
public class PaletteComponent extends ScilabComponent {

    private static final long serialVersionUID = 1592816691330208090L;

    /**
     * Construct the component with the associated graph
     *
     * @param graph
     *            The associated graph
     */
    public PaletteComponent(XcosDiagram graph) {
        super(graph);

        addComponentListener(new ComponentListener() {

            @Override
            public void componentShown(ComponentEvent arg0) {
                if (getGraph() instanceof PaletteDiagram) {
                    ((PaletteDiagram) getGraph()).updateDiagram(getSize()
                            .getWidth());
                }
            }

            @Override
            public void componentResized(ComponentEvent arg0) {
                if (getGraph() instanceof PaletteDiagram) {
                    ((PaletteDiagram) getGraph()).updateDiagram(getSize()
                            .getWidth());
                }
            }

            @Override
            public void componentMoved(ComponentEvent arg0) {
            }

            @Override
            public void componentHidden(ComponentEvent arg0) {
            }
        });
    }

    /**
     * @return The filename
     */
    @Override
    public String toString() {
        if (getGraph() instanceof PaletteDiagram) {
            String fileName = ((PaletteDiagram) getGraph()).getName();
            return fileName.substring(0, fileName.lastIndexOf('.'));
        } else {
            return super.toString();
        }
    }
}
