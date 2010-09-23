package org.scilab.modules.graphic_objects.graphicEngine.renderingZone.layout;

import org.scilab.modules.graphic_objects.graphicEngine.core.IRenderingInfo;

/**
 * Author: Lando Pierre
 * Date: june 14, 2010
 */
public interface ILayoutPolicy {

    void setRenderingInfo(IRenderingInfo renderingInfo);

    float getAspectRatio();
}
