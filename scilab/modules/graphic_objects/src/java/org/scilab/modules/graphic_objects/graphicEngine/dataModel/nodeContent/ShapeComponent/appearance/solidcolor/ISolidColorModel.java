package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.appearance.solidcolor;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.appearance.IAppearanceModel;


/**
 * Author: Lando Pierre
 * Date: june 18, 2010
 *
 * Model of solid color.
 *
 */
public interface ISolidColorModel extends IAppearanceModel {
    final String type = "SolidColor";
    final float defaultRed   = .4f;
    final float defaultGreen = .5f;
    final float defaultBlue  = .6f;
    final float defaultAlpha  = 1f;

    public void setColor(float r, float g, float b);
    public void setColor(float r, float g, float b, float a);
    public void setColor(ISolidColorModel color);

    public void setRed(float r);
    public void setGreen(float g);
    public void setBlue(float b);
    public void setAlpha(float a);

    public float getRed();
    public float getGreen();
    public float getBlue();
    public float getAlpha();

}
