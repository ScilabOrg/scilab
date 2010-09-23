package org.scilab.modules.graphic_objects.graphicEngine.core.array;

import org.scilab.modules.graphic_objects.graphicEngine.core.IRenderingInfo;

import java.nio.FloatBuffer;

/**
 * Author: Lando Pierre
 * Date: june 7, 2010
 *
 * Data should be 4-aligned.
 */
public interface INormalArray<renderingInfoType extends IRenderingInfo> {

    /**
     * @param data of this vertex array.
     */
    public void setData(FloatBuffer data);

    /**
     * Bind this vertex array has current vertex array.
     * @param renderingInfo needed to interact with graphic engine.
     * @throws UnBindableException if graphic engine refuse to bind this array.
     */
    void bind(renderingInfoType renderingInfo) throws UnBindableException;

    /**
     * @return the getSize of this index array.
     */
    int size();
}