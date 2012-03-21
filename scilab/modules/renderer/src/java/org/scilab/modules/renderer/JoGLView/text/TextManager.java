/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 * Copyright (C) 2012 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.text;

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.TextureManager;
import org.scilab.forge.scirenderer.tranformations.DegenerateMatrixException;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.textObject.Text;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;

import java.awt.Dimension;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_ANGLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;

/**
 *
 * This class manage scilab text entity drawing.
 *
 *
 * TODO, Manage: {text_box, text_box_mode (filled), auto_dimensionning, clip_state, clip_box}
 *
 *
 * @author Pierre Lando
 */
public class TextManager {

    /**
     * The {@see Map} off existing {@see TextEntity}.
     */
    private final Map<String, Texture> spriteMap = new ConcurrentHashMap<String, Texture>();

    /**
     * The used texture manager.
     */
    private final TextureManager textureManager;


    /**
     * Default constructor.
     * @param textureManager the texture manager.
     */
    public TextManager(TextureManager textureManager) {
        this.textureManager = textureManager;
    }

    /**
     * Draw the given Scilab {@see Text} with the given {@see DrawingTools}.
     * @param drawingTools the given {@see DrawingTools}.
     * @param colorMap the current {@see ColorMap}
     * @param text the given Scilab {@see Text}
     * @throws SciRendererException if draw fail.
     */
    public final void draw(final DrawingTools drawingTools, final ColorMap colorMap, final Text text) throws SciRendererException {
        Texture texture = getTexture(colorMap, text);

        Transformation projection = drawingTools.getTransformationManager().getCanvasProjection();

        /* Compute the text's position and draw in window coordinates */
        Vector3d textPosition = computeTextPosition(projection, text, texture.getDataProvider().getTextureSize());

        drawingTools.getTransformationManager().useWindowCoordinate();

        /* The Text object's rotation direction convention is opposite to the standard one, its angle is expressed in degree. */
        drawingTools.draw(texture, AnchorPosition.LOWER_LEFT, textPosition, -Math.toDegrees(text.getFontAngle()));

        drawingTools.getTransformationManager().useSceneCoordinate();

        /* Compute the corners of the text's bounding box in window coordinates */
        Vector3d[] projCorners = computeProjCorners(projection, new Vector3d(text.getPosition()), text.getFontAngle(), texture.getDataProvider().getTextureSize());

        Vector3d[] corners = computeCorners(projection, projCorners);
        Double[] coordinates = cornersToCoordinateArray(corners);

        /* Set the computed coordinates */
        text.setCorners(coordinates);
    }

    /**
     * Computes and returns the position of a Scilab {@see Text} object in window coordinates as a function
     * of its text box mode and text box properties. It is the position of its lower-left corner,
     * which may differ from the text's unmodified lower-left corner position, depending on its text box properties.
     * If the text box mode is set to off, the returned position is the text's unmodified projected position.
     * @param projection the projection from object coordinates to window coordinates.
     * @param text the Scilab {@see Text}.
     * @param textureDimension the text texture's size (in pixels).
     * @return the position of the Scilab {@see Text}'s text box.
     * @throws DegenerateMatrixException if projection fail.
     */
    private Vector3d computeTextPosition(Transformation projection, Text text, Dimension textureDimension) throws DegenerateMatrixException {
        Vector3d textPosition = new Vector3d(text.getPosition());

        textPosition = projection.project(textPosition);

        /* Only manages the centered mode at the moment */
        if (text.getTextBoxMode() == 1) {
            Double[] textBox = text.getTextBox();

            Vector3d textBoxWidth = new Vector3d(textBox[0], 0.0, 0.0);
            Vector3d textBoxHeight = new Vector3d(0.0, textBox[1], 0.0);

            textBoxWidth = projection.projectDirection(textBoxWidth);
            textBoxHeight = projection.projectDirection(textBoxHeight);

            Vector3d[] projCorners = computeProjCorners(textPosition, text.getFontAngle(), textureDimension);

            Vector3d textWidth = projCorners[1].minus(projCorners[0]);
            Vector3d textHeight = projCorners[2].minus(projCorners[0]);

            /* Compute the text box's and text's half-length vectors */
            textBoxWidth = textWidth.getNormalized().times(0.5*textBoxWidth.getX());
            textBoxHeight = textHeight.getNormalized().times(0.5*textBoxHeight.getY());

            textWidth = textWidth.times(0.5);
            textHeight = textHeight.times(0.5);

            /* Compute the actual position from the initial projected position and the half-length vectors */
            textPosition = textPosition.plus(textBoxWidth);
            textPosition = textPosition.plus(textBoxHeight);

            textPosition = textPosition.minus(textWidth);
            textPosition = textPosition.minus(textHeight);
        }

        return textPosition;
    }

    /**
     * Computes and returns the corners of a {@see Text} object's bounding box, in window coordinates.
     * The returned corners are in the following order: lower-left, lower-right, upper-left and upper-right.
     * @param canvasProj the projection from object coordinates to window coordinates.
     * @param position the text's position in object coordinates.
     * @param fontAngle the text's font angle (radians).
     * @param textureDimension the text texture's dimension (in pixels).
     * @return the corners' window coordinates (4-element array).
     * @throws DegenerateMatrixException if projection fail.
     */
    private Vector3d[] computeProjCorners(Transformation canvasProj, Vector3d position, double fontAngle, Dimension textureDimension) throws DegenerateMatrixException {
        Vector3d[] projCorners = new Vector3d[4];

        position = canvasProj.project(position);

        projCorners = computeProjCorners(position, fontAngle, textureDimension);

        return projCorners;
    }

    /**
     * Computes and returns the corners of a {@see Text} object's bounding box, in window coordinates.
     * The returned corners are in the following order: lower-left, lower-right, upper-left and upper-right.
     * @param projPosition the text's position in window coordinates.
     * @param fontAngle the text's font angle (radians).
     * @param textureDimension the text texture's dimension (in pixels).
     * @return the corners' window coordinates (4-element array).
     * @throws DegenerateMatrixException if projection fail.
     */
    private Vector3d[] computeProjCorners(Vector3d projPosition, double fontAngle, Dimension textureDimension) throws DegenerateMatrixException {
        Vector3d[] projCorners = new Vector3d[4];

        /*
         * Should be -fontAngle, as positive angle values are measured clockwise for texts.
         * Apparently uses the same convention as the texts (clockwise positive directions).
         * To be verified.
         */
        Transformation projRotation = TransformationFactory.getRotationTransformation(180.0*fontAngle/Math.PI, 0.0, 0.0, 1.0);

        projCorners[0] = projPosition;

        Vector3d width = new Vector3d(textureDimension.getWidth(), 0.0, 0.0);
        Vector3d height = new Vector3d(0.0, textureDimension.getHeight(), 0.0);

        width = projRotation.projectDirection(width);
        height = projRotation.projectDirection(height);

        projCorners[1] = projCorners[0].plus(width);
        projCorners[2] = projCorners[0].plus(height);
        projCorners[3] = projCorners[2].plus(width);

        return projCorners;
    }

    /**
     * Computes and returns the corners (in object coordinates) of a text's bounding box.
     * @param projection the projection from object coordinates to window coordinates.
     * @param projCorners the corners of the text's bounding box in window coordinates (4-element array).
     * @return the corners of the text's bounding box in object coordinates (4-element array).
     */
    private Vector3d[] computeCorners(Transformation projection, Vector3d[] projCorners) {
        Vector3d[] corners = new Vector3d[4];

        corners[0] = projection.unproject(projCorners[0]);
        corners[1] = projection.unproject(projCorners[1]);
        corners[2] = projection.unproject(projCorners[2]);
        corners[3] = projection.unproject(projCorners[3]);

        return corners;
    }

    /**
     * Returns the positions of a bounding box's corners as an array of (x,y,z) coordinate triplets.
     * The output corners are reordered to match their order in the {@see Text} object's
     * equivalent array, respectively: lower-left, lower-right, upper-left, upper-right in the input array,
     * starting from the lower-left and going in clockwise order in the returned array.
     * @param corners of the bounding box (4-element array).
     * @return the corners' coordinates (12-element array).
     */
    private Double[] cornersToCoordinateArray(Vector3d[] corners) {
        Double[] coordinates = new Double[12];
        coordinates[0] = corners[0].getX();
        coordinates[1] = corners[0].getY();
        coordinates[2] = corners[0].getZ();

        coordinates[3] = corners[2].getX();
        coordinates[4] = corners[2].getY();
        coordinates[5] = corners[2].getZ();

        coordinates[6] = corners[3].getX();
        coordinates[7] = corners[3].getY();
        coordinates[8] = corners[3].getZ();

        coordinates[9] = corners[1].getX();
        coordinates[10] = corners[1].getY();
        coordinates[11] = corners[1].getZ();

        return coordinates;
    }


    /**
     * Update the data if needed.
     * @param id the modified object.
     * @param property the changed property.
     */
    public void update(String id, String property) {
        if (!__GO_POSITION__.equals(property) && !__GO_FONT_ANGLE__.equals(property)) {
            dispose(id);
        }
    }

    /**
     * Return the SciRenderer {@see Texture} corresponding to the given Scilab {@see Text}.
     * @param colorMap the current color map.
     * @param text the given Scilab {@see Text}.
     * @return the SciRenderer {@see Texture} corresponding to the given Scilab {@see Text}.
     */
    private Texture getTexture(final ColorMap colorMap, final Text text) {
        Texture texture = spriteMap.get(text.getIdentifier());
        if (texture == null) {
            texture = createSprite(colorMap, text);
            spriteMap.put(text.getIdentifier(), texture);
        }
        return texture;
    }

    /**
     * Returns the dimensions of the SciRenderer {@see Texture} corresponding to the given Scilab {@see Text}.
     * The dimensions are in pixels (width, height).
     * @param colorMap the current color map.
     * @param text the given Scilab {@see Text}.
     * @return the texture's dimensions (2-element array)
     */
    private Dimension getSpriteDims(final ColorMap colorMap, final Text text) {
        Texture texture = spriteMap.get(text.getIdentifier());
        if (texture == null) {
            TextSpriteDrawer spriteDrawer = new TextSpriteDrawer(colorMap, text);
            return spriteDrawer.getTextureSize();
        } else {
            return texture.getDataProvider().getTextureSize();
        }
    }

    /**
     * Create a texture for the given text object.
     * @param colorMap the current colormap.
     * @param textObject the given text object.
     * @return a new texture for the given text object.
     */
    private Texture createSprite(final ColorMap colorMap, final Text textObject) {
        TextSpriteDrawer spriteDrawer = new TextSpriteDrawer(colorMap, textObject);
        Texture texture = textureManager.createTexture();
        texture.setDrawer(spriteDrawer);
        return texture;
    }

    /**
     * Dispose the texture corresponding to the given id.
     * @param id the given id.
     */
    public void dispose(String id) {
        Texture texture = spriteMap.get(id);
        if (texture != null) {
            textureManager.dispose(texture);
            spriteMap.remove(id);
        }
    }

    /**
     * Dispose all the text sprites.
     */
    public void disposeAll() {
        textureManager.dispose(spriteMap.values());
        spriteMap.clear();
    }

    /**
     * Computes and updates the corners of the given Scilab {@see Text}.
     * @param text the given Scilab {@see Text}.
     */
    public static void updateTextCorners(Text text) {
        Vector3d[] projCorners = null;

        DrawerVisitor currentVisitor = DrawerVisitor.getVisitor(text.getParentFigure());
        Transformation currentProj = currentVisitor.getAxesDrawer().getProjection(text.getParentAxes());

        Dimension spriteDims = currentVisitor.getTextManager().getSpriteDims(currentVisitor.getColorMap(), text);
        Vector3d textPosition = new Vector3d(text.getPosition());

        /* Compute the corners */
        try {
            textPosition = currentVisitor.getTextManager().computeTextPosition(currentProj, text, spriteDims);
            projCorners = currentVisitor.getTextManager().computeProjCorners(textPosition, text.getFontAngle(), spriteDims);
        } catch (DegenerateMatrixException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        Vector3d[] corners = currentVisitor.getTextManager().computeCorners(currentProj, projCorners);
        Double[] coordinates = currentVisitor.getTextManager().cornersToCoordinateArray(corners);

        /* Set the computed coordinates */
        text.setCorners(coordinates);
    }
}