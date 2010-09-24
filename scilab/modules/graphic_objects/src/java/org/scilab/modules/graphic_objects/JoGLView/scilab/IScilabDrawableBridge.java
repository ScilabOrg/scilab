package org.scilab.modules.graphic_objects.JoGLView.scilab;

import org.scilab.forge.scirenderer.core.drawable.IDrawable;

/**
 * @author Pierre Lando
 */
public interface IScilabDrawableBridge extends IDrawable {

    /**
     * Return the id of this drawable. 
     * @return the id of this drawable.
     */
    String getId();
}
