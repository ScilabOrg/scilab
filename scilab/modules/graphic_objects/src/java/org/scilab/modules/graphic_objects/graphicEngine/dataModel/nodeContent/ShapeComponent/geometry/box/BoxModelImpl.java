package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.box;

import org.scilab.modules.graphic_objects.graphicEngine.drawer.AbstractDrawableModel;
import org.scilab.modules.graphic_objects.graphicEngine.maths.IVector3f;
import org.scilab.modules.graphic_objects.graphicEngine.maths.Vector3f;

/**
 * Author: Lando Pierre
 * Date: june 14, 2010
 */
public class BoxModelImpl extends AbstractDrawableModel implements IBoxModel {
    private IVector3f size = new Vector3f(IBoxModel.defaultSize);
    private Boolean solidness = IBoxModel.defaultSolidness;

    
    public void setSize(IVector3f size) {
        this.size = size;
    }

    
    public IVector3f getSize() {
        return size;
    }

    
    public void setSolid(boolean solidness) {
        this.solidness = solidness;
    }

    
    public boolean getSolid() {
        return solidness;
    }

    
    public String getType() {
        return type;
    }
}
