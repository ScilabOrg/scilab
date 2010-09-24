package org.scilab.modules.graphic_objects.JoGLView.scilab.drawers;

import org.scilab.forge.scirenderer.core.scenedrawer.IDrawerVisitor;
import org.scilab.forge.scirenderer.impl.opengl.core.renderinginfo.IGLRenderingInfo;
import org.scilab.modules.graphic_objects.figure.ColorMap;

/**
 * @author Pierre Lando
 */
public class AbstractScilabDrawer  {
    public static final String COLORMAP_KEY = "COLORMAP_KEY";

    /**
     * Get the colorMap from the visitor user's objects. 
     * @param visitor the visitor.
     * @return the current colorMap.
     */
    protected ColorMap getColorMap(IDrawerVisitor<IGLRenderingInfo> visitor) {
        return (ColorMap) visitor.getUserObject(COLORMAP_KEY);
    }

    /**
     * Add the colorMap in the visitor user's objects.
     * @param visitor the visitor.
     * @param colorMap the colorMap.
     */
    protected void setColorMap(IDrawerVisitor<IGLRenderingInfo> visitor, ColorMap colorMap) {
        visitor.addUserObject(COLORMAP_KEY, colorMap);
    }
}
