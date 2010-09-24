package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.appearance.solidcolor;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.AbstractGraphicController;

/**
 * Author: Lando Pierre
 * Date: june 29, 2010
 */
public class SolidColorController extends AbstractGraphicController<ISolidColorModel> implements ISolidColorController {

    public SolidColorController() {
        this (new SolidColorModelImp());
    }

    public SolidColorController(float r, float g, float b) {
        this (new SolidColorModelImp(r, g, b));
    }

    public SolidColorController(float r, float g, float b, float a) {
        this (new SolidColorModelImp(r, g, b, a));
    }

    public SolidColorController(ISolidColorModel model) {
        super(model);
    }

    
    public void setColor(float r, float g, float b) {
        getModel().setColor(r,g,b);
        fireUpdate();
    }

    
    public void setColor(float r, float g, float b, float a) {
        getModel().setColor(r,g,b,a);
        fireUpdate();
    }

    
    public void setColor(ISolidColorModel color) {
        getModel().setColor(color);
        fireUpdate();
    }

    
    public void setRed(float r) {
        if (getRed() != r) {
            getModel().setRed(r);
            fireUpdate();
        }
    }

    
    public void setGreen(float g) {
        if (getGreen() != g) {
            getModel().setGreen(g);
            fireUpdate();
        }
    }

    
    public void setBlue(float b) {
        if (getBlue() != b) {
            getModel().setBlue(b);
            fireUpdate();
        }
    }

    
    public void setAlpha(float a) {
        if (getAlpha() != a) {
            getModel().setAlpha(a);
            fireUpdate();
        }
    }

    
    public float getRed() {
        return getModel().getRed();
    }

    
    public float getGreen() {
        return getModel().getGreen();
    }

    
    public float getBlue() {
        return getModel().getBlue();
    }

    
    public float getAlpha() {
        return getModel().getAlpha();
    }
}
