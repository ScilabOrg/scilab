package org.scilab.modules.graphic_objects.graphicEngine.renderingZone.layout;

import org.scilab.modules.graphic_objects.graphicEngine.core.IRenderingInfo;

/**
 * Author: Lando Pierre
 * Date: june 14, 2010
 */
public class FullLayoutPolicy implements ILayoutPolicy {
    private IRenderingInfo renderingInfo;

    
    public void setRenderingInfo(IRenderingInfo renderingInfo) {
        this.renderingInfo = renderingInfo;
    }

    
    public float getAspectRatio() {
        return  (float) renderingInfo.getFrameWidth() / (float) renderingInfo.getFrameHeight();
    }
}
