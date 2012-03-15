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
}
