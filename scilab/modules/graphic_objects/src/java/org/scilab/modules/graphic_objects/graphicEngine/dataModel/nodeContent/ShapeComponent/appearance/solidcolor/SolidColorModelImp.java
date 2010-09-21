package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.appearance.solidcolor;

import org.scilab.modules.graphic_objects.graphicEngine.drawer.AbstractDrawableModel;

/**
 * Author: Lando Pierre
 * Date: june 18, 2010
 */
// TODO : this is not a drawable.
public class SolidColorModelImp extends AbstractDrawableModel implements ISolidColorModel {
    private float r = ISolidColorModel.defaultRed;
    private float g = ISolidColorModel.defaultGreen;
    private float b = ISolidColorModel.defaultBlue;
    private float a = ISolidColorModel.defaultAlpha;

    public SolidColorModelImp() {}

    public SolidColorModelImp(float r, float g, float b) {
        setColor(r, g, b);
    }

    public SolidColorModelImp(float r, float g, float b, float a) {
        setColor(r, g, b, a);
    }

    
    public void setColor(float r, float g, float b) {
        this.r = r;
        this.g = g;
        this.b = b;
        this.a = defaultAlpha;
    }

    
    public void setColor(float r, float g, float b, float a) {
        this.r = r;
        this.g = g;
        this.b = b;
        this.a = a;
    }

    
    public void setColor(ISolidColorModel color) {
        setColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
    }

    
    public void setRed(float r) {
        this.r = r;
    }

    
    public void setGreen(float g) {
        this.g = g;
    }

    
    public void setBlue(float b) {
        this.b = b;
    }

    
    public void setAlpha(float a) {
        this.a = a;
    }

    
    public float getRed() {
        return r;
    }

    
    public float getGreen() {
        return g;
    }

    
    public float getBlue() {
        return b;
    }

    
    public float getAlpha() {
        return a;
    }

    
    public String getType() {
        return type;
    }
}
