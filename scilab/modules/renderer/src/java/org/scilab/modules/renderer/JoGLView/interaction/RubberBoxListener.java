/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.interaction;

import java.awt.Point;
import java.util.EventListener;

/**
 * @author Pierre Lando
 */
public interface RubberBoxListener extends EventListener {

    /**
     * Notification for a rubber box end.
     * @param firstPoint the first corner of the rubber box.
     * @param secondPoint the second corner of the rubber box.
     */
    void rubberBoxEnd(Point firstPoint, Point secondPoint);

    /** Notification for a rubber box stop */
    void rubberBoxStopped();
}
