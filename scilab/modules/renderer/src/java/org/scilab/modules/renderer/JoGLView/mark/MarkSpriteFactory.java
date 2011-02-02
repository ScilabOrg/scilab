/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.mark;

import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.sprite.Sprite;
import org.scilab.forge.scirenderer.sprite.SpriteDrawingTools;
import org.scilab.modules.graphic_objects.contouredObject.Mark;
import org.scilab.modules.graphic_objects.figure.ColorMap;

/**
 * @author Pierre Lando
 */
public class MarkSpriteFactory {

    /**
     * Return a mark sprite corresponding to the given scilab mark.
     *
     * @param mark the scilab mark.
     * @param colorMap the scilab color map.
     * @return a mark sprite corresponding to the given scilab mark.
     */
    public static Sprite getMarkSprite(Mark mark, ColorMap colorMap) {

        final Appearance appearance = new Appearance();
        Color backgroundColor = new Color(colorMap.getScilabColor(mark.getBackground()));
        Color foregroundColor = new Color(colorMap.getScilabColor(mark.getForeground()));

        appearance.setFillColor(backgroundColor);
        appearance.setLineColor(foregroundColor);
        int finalSize;
        if (mark.getMarkSizeUnit() == Mark.MarkSizeUnitType.TABULATED) {
            finalSize = (8 + 2 * mark.getSize());
        } else {
            finalSize = mark.getSize();
        }

        switch (mark.getStyle()) {
            case  0: return new DotSprite(foregroundColor, finalSize);
            case  1: return new PlusSprite(appearance, finalSize);
            case  2: return new CrossSprite(appearance, finalSize);
            case  3: return new StarSprite(appearance, finalSize);
            case  4: return new FilledDiamondSprite(foregroundColor, finalSize);

            case 10: return new AsteriskSprite(appearance, finalSize);
            case 11: return new SquareSprite(appearance, finalSize);

            default:
                return new PlusSprite(appearance, finalSize);
        }
    }

    /**
     * Abstract class for all scilab mark sprites.
     */
    private static abstract class ScilabSprite implements Sprite {
        protected final Appearance appearance;
        protected final int size;

        public ScilabSprite(Appearance appearance, int size) {
            this.appearance = appearance;
            this.size = size;
        }

        @Override
        public int getWidth() {
            return size + 2;
        }

        @Override
        public int getHeight() {
            return size + 2;
        }
    }

    /**
     * Dot sprite
     * Scilab ID = 0
     */
    private static class DotSprite implements Sprite {
        private final Color color;
        private final int size;


        public DotSprite(Color color, int size) {
            this.color = color;
            this.size = size;
        }

        @Override
        public void draw(SpriteDrawingTools spriteDrawingTools) {
            spriteDrawingTools.fillDisc(0, 0, size, color);
        }

        @Override
        public int getWidth() {
            return size + 2;
        }

        @Override
        public int getHeight() {
            return size + 2;
        }
    }

    /**
     * Plus sprite
     * Scilab ID = 1
     */
    private static class PlusSprite extends ScilabSprite {
        private final int[] coordinate1;
        private final int[] coordinate2;

        public PlusSprite(Appearance appearance, int size) {
            super(appearance, size);
            int r = size / 2;
            coordinate1 = new int[] {-r, 0, r, 0};
            coordinate2 = new int[] { 0,-r, 0, r};
        }

        @Override
        public void draw(SpriteDrawingTools spriteDrawingTools) {
            spriteDrawingTools.drawPolyline(coordinate1, appearance);
            spriteDrawingTools.drawPolyline(coordinate2, appearance);
        }
    }

    /**
     * Cross sprite
     * Scilab ID = 2
     */
    private static class CrossSprite extends ScilabSprite {
        private final int[] coordinate1;
        private final int[] coordinate2;

        public CrossSprite(Appearance appearance, int size) {
            super(appearance, size);
            int r = (int) (size * Math.sqrt(2.0) / 2.0);
            coordinate1 = new int[] {-r,-r, r, r};
            coordinate2 = new int[] {-r, r,+r,-r};
        }

        @Override
        public void draw(SpriteDrawingTools spriteDrawingTools) {
            spriteDrawingTools.drawPolyline(coordinate1, appearance);
            spriteDrawingTools.drawPolyline(coordinate2, appearance);
        }
    }

    /**
     * Star sprite
     * Scilab ID = 3
     */
    private static class StarSprite extends ScilabSprite {
        private final int[] coordinate1;
        private final int[] coordinate2;

        public StarSprite(Appearance appearance, int size) {
            super(appearance, size);
            int r = size / 2;
            coordinate1 = new int[] {-r, 0, r, 0};
            coordinate2 = new int[] { 0,-r, 0, r};
        }

        @Override
        public void draw(SpriteDrawingTools spriteDrawingTools) {
            spriteDrawingTools.fillDisc(0, 0, size, appearance.getFillColor());
            spriteDrawingTools.drawCircle(0, 0, size, appearance);
            spriteDrawingTools.drawPolyline(coordinate1, appearance);
            spriteDrawingTools.drawPolyline(coordinate2, appearance);
        }
    }

    /**
     * Filled diamond sprite
     * Scilab ID = 4
     */
    private static class FilledDiamondSprite implements Sprite {
        private final Appearance appearance;
        private final int[] coordinates;
        private final int size;

        public FilledDiamondSprite(Color color, int size) {
            this.size = size;
            int r = size / 2;

            appearance = new Appearance();
            appearance.setFillColor(color);
            appearance.setLineColor(color);

            coordinates = new int[] {-r,0, 0,-r, +r,0, 0,+r};
        }

        @Override
        public void draw(SpriteDrawingTools spriteDrawingTools) {
            spriteDrawingTools.drawPolygon(coordinates, appearance);
        }

        @Override
        public int getWidth() {
            return size + 2;
        }

        @Override
        public int getHeight() {
            return size + 2;
        }
    }

    /**
     * Asterisk sprite
     * Scilab ID = 10
     */
    private static class AsteriskSprite extends ScilabSprite {
        private final int[] coordinate1;
        private final int[] coordinate2;
        private final int[] coordinate3;
        private final int[] coordinate4;

        public AsteriskSprite(Appearance appearance, int size) {
            super(appearance, size);
            int r = size / 2;
            coordinate1 = new int[] {-r, 0, r, 0};
            coordinate2 = new int[] { 0,-r, 0, r};
            r = (int) (r*Math.sqrt(2.0) / 2.0);
            coordinate3 = new int[] {-r,-r, r, r};
            coordinate4 = new int[] {-r, r,+r,-r};
        }

        @Override
        public void draw(SpriteDrawingTools spriteDrawingTools) {
            spriteDrawingTools.drawPolyline(coordinate1, appearance);
            spriteDrawingTools.drawPolyline(coordinate2, appearance);
            spriteDrawingTools.drawPolyline(coordinate3, appearance);
            spriteDrawingTools.drawPolyline(coordinate4, appearance);
        }
    }

    /**
     * Square sprite
     * Scilab ID = 11
     */
    private static class SquareSprite extends ScilabSprite {
        private final int[] coordinate;

        public SquareSprite(Appearance appearance, int size) {
            super(appearance, size);
            int r = size / 2;
            coordinate = new int[] {-r,-r, +r,-r, +r,+r, -r,+r};
        }

        @Override
        public void draw(SpriteDrawingTools spriteDrawingTools) {
            spriteDrawingTools.drawPolygon(coordinate, appearance);
        }
    }
}
