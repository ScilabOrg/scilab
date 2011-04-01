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
import org.scilab.forge.scirenderer.sprite.SpriteDrawer;
import org.scilab.forge.scirenderer.sprite.SpriteDrawingTools;
import org.scilab.forge.scirenderer.sprite.SpriteManager;
import org.scilab.modules.graphic_objects.contouredObject.Mark;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;

/**
 * @author Pierre Lando
 */
public class MarkSpriteFactory {

    /**
     * Return a mark sprite corresponding to the given scilab mark.
     *
     *
     * @param spriteManager the current sprite manager.
     * @param mark the scilab mark.
     * @param colorMap the scilab color map.
     * @return a mark sprite corresponding to the given scilab mark.
     */
    public static Sprite getMarkSprite(SpriteManager spriteManager, Mark mark, ColorMap colorMap) {
        int finalSize;

        /**
         * Compute the sprite pixel size.
         */
        if (mark.getMarkSizeUnit() == Mark.MarkSizeUnitType.TABULATED) {
            finalSize = (8 + 2 * mark.getSize());
        } else {
            finalSize = mark.getSize();
        }

        /**
         * Add a margin such that (0, 0) was pixel aligned.
         */
        int margin;
        if (finalSize % 2 == 0) {
            margin = 3;
        } else {
            margin = 2;
        }

        Sprite sprite = spriteManager.createSprite(finalSize + margin, finalSize + margin);
        sprite.setDrawer(getSpriteDrawer(mark, finalSize, colorMap));

        return sprite;
    }

    /**
     * Return the sprite drawer corresponding to the given mark.
     * @param mark the mark to draw.
     * @param finalSize the final mark size.
     * @param colorMap the scilab colormap to use.
     * @return the sprite drawer corresponding to the given mark.
     */
    private static SpriteDrawer getSpriteDrawer(Mark mark, int finalSize, ColorMap colorMap) {

        final Appearance appearance = new Appearance();
        Color backgroundColor = ColorFactory.createColor(colorMap, mark.getBackground());
        Color foregroundColor = ColorFactory.createColor(colorMap, mark.getForeground());

        appearance.setFillColor(backgroundColor);
        appearance.setLineColor(foregroundColor);

        switch (mark.getStyle()) {
            case  0: return new DotSpriteDrawer(foregroundColor, finalSize);
            case  1: return new PlusSpriteDrawer(appearance, finalSize);
            case  2: return new CrossSpriteDrawer(appearance, finalSize);
            case  3: return new StarSpriteDrawer(appearance, finalSize);
            case  4: return new FilledDiamondSpriteDrawer(foregroundColor, finalSize);
            case  5: return new DiamondSpriteDrawer(appearance, finalSize);
            case  6: return new TriangleUpSpriteDrawer(appearance, finalSize);
            case  7: return new TriangleDownSpriteDrawer(appearance, finalSize);
            case  8: return new DiamondPlusSpriteDrawer(appearance, finalSize);
            case  9: return new CircleSpriteDrawer(appearance, finalSize);
            case 10: return new AsteriskSpriteDrawer(appearance, finalSize);
            case 11: return new SquareSpriteDrawer(appearance, finalSize);
            case 12: return new TriangleRightSpriteDrawer(appearance, finalSize);
            case 13: return new TriangleLeftSpriteDrawer(appearance, finalSize);
            case 14: return new PentagramSpriteDrawer(appearance, finalSize);

            default:
                return new PlusSpriteDrawer(appearance, finalSize);
        }
    }

    /**
     * Abstract class for all scilab mark sprites.
     */
    private static abstract class ScilabSpriteDrawer implements SpriteDrawer {
        protected final Appearance appearance;
        protected final int size;

        public ScilabSpriteDrawer(Appearance appearance, int size) {
            this.appearance = appearance;
            this.size = size;
        }

        @Override
        public OriginPosition getOriginPosition() {
            return OriginPosition.CENTER;
        }
    }

    /**
     * Dot sprite
     * Scilab ID = 0
     */
    private static class DotSpriteDrawer implements SpriteDrawer {
        private final Color color;
        private final int size;


        public DotSpriteDrawer(Color color, int size) {
            this.color = color;
            this.size = size;
        }

        @Override
        public void draw(SpriteDrawingTools spriteDrawingTools) {
            spriteDrawingTools.fillDisc(0, 0, size, color);
        }

        @Override
        public OriginPosition getOriginPosition() {
            return OriginPosition.CENTER;
        }
    }

    /**
     * Plus sprite
     * Scilab ID = 1
     */
    private static class PlusSpriteDrawer extends ScilabSpriteDrawer {
        private final int[] coordinate1;
        private final int[] coordinate2;

        public PlusSpriteDrawer(Appearance appearance, int size) {
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
    private static class CrossSpriteDrawer extends ScilabSpriteDrawer {
        private final int[] coordinate1;
        private final int[] coordinate2;

        public CrossSpriteDrawer(Appearance appearance, int size) {
            super(appearance, size);
            int r = (int) (size * Math.sqrt(2.0) / 2.0);
            coordinate1 = new int[] {-r, -r, r,  r};
            coordinate2 = new int[] {-r,  r, r, -r};
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
    private static class StarSpriteDrawer extends ScilabSpriteDrawer {
        private final int[] coordinate1;
        private final int[] coordinate2;

        public StarSpriteDrawer(Appearance appearance, int size) {
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
    private static class FilledDiamondSpriteDrawer implements SpriteDrawer {
        private final Appearance appearance;
        private final int[] coordinates;
        private final int size;

        public FilledDiamondSpriteDrawer(Color color, int size) {
            this.size = size;
            int r = size / 2;

            appearance = new Appearance();
            appearance.setFillColor(color);
            appearance.setLineColor(color);

            coordinates = new int[] {-r,0, 0,-r, +r,0, 0,+r};
        }

        @Override
        public void draw(SpriteDrawingTools spriteDrawingTools) {
            spriteDrawingTools.fillPolygon(coordinates, appearance);
        }

        @Override
        public OriginPosition getOriginPosition() {
            return OriginPosition.CENTER;
        }
    }

    /**
     * Abstract class for all the triangle mark sprites.
     */
    private static abstract class TriangleSpriteDrawer extends ScilabSpriteDrawer {
        protected final int r;
        protected final int basex;
        protected final int basey;
        protected int[] coordinates;

        public TriangleSpriteDrawer(Appearance appearance, int size) {
            /* To do: check whether modifying size is correct */
            super(appearance, (int)(Math.sqrt(2.0)*(double)size));
            double tmpr = (double)(size/2);

            r = (int) (tmpr);

            basex = (int) (tmpr * 0.5 * Math.sqrt(3));
            basey = (int) (tmpr * 0.5);
        }

        @Override
        public void draw(SpriteDrawingTools spriteDrawingTools) {
            spriteDrawingTools.fillPolygon(coordinates, appearance);
        }
    }

    /**
     * Diamond sprite
     * Scilab ID = 5
     */
    private static class DiamondSpriteDrawer extends ScilabSpriteDrawer {
        protected final int[] coordinates;

        public DiamondSpriteDrawer(Appearance appearance, int size) {
            super(appearance, size);
            int r = size / 2;

            coordinates = new int[] {-r,0, 0,-r, r,0, 0,r};
        }

        @Override
        public void draw(SpriteDrawingTools spriteDrawingTools) {
            spriteDrawingTools.fillPolygon(coordinates, appearance);
        }
    }

    /**
     * Triangle up sprite
     * Scilab ID = 6
     */
    private static class TriangleUpSpriteDrawer extends TriangleSpriteDrawer {
        public TriangleUpSpriteDrawer(Appearance appearance, int size) {
            super(appearance, size);

            coordinates = new int[] {-basex,basey, 0,-r, basex,basey};
        }
    }

    /**
     * Triangle down sprite
     * Scilab ID = 7
     */
    private static class TriangleDownSpriteDrawer extends TriangleSpriteDrawer {
        public TriangleDownSpriteDrawer(Appearance appearance, int size) {
            super(appearance, size);

            coordinates = new int[] {-basex,-basey, basex,-basey, 0,r};
        }
    }

    /**
     * Diamond plus sprite
     * Scilab ID = 8
     */
    private static class DiamondPlusSpriteDrawer extends DiamondSpriteDrawer {
        private final int[] coordinate1;
        private final int[] coordinate2;

        public DiamondPlusSpriteDrawer(Appearance appearance, int size) {
            super(appearance, size);
            coordinate1 = new int[] {coordinates[0], coordinates[1], coordinates[4], coordinates[5]};
            coordinate2 = new int[] {coordinates[2], coordinates[3], coordinates[6], coordinates[7]};
        }

        @Override
        public void draw(SpriteDrawingTools spriteDrawingTools) {
            super.draw(spriteDrawingTools);
            spriteDrawingTools.drawPolyline(coordinate1, appearance);
            spriteDrawingTools.drawPolyline(coordinate2, appearance);
        }
    }

    /**
     * Circle sprite
     * Scilab ID = 9
     */
    private static class CircleSpriteDrawer extends ScilabSpriteDrawer {
        private final int[] coordinate1;
        private final int[] coordinate2;

        public CircleSpriteDrawer(Appearance appearance, int size) {
            super(appearance, size);
            int r = size / 2;
            coordinate1 = new int[] {-r, 0, r, 0};
            coordinate2 = new int[] { 0,-r, 0, r};
        }

        @Override
        public void draw(SpriteDrawingTools spriteDrawingTools) {
            spriteDrawingTools.fillDisc(0, 0, size, appearance.getFillColor());
            spriteDrawingTools.drawCircle(0, 0, size, appearance);
        }
    }

    /**
     * Asterisk sprite
     * Scilab ID = 10
     */
    private static class AsteriskSpriteDrawer extends ScilabSpriteDrawer {
        private final int[] coordinate1;
        private final int[] coordinate2;
        private final int[] coordinate3;
        private final int[] coordinate4;

        public AsteriskSpriteDrawer(Appearance appearance, int size) {
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
    private static class SquareSpriteDrawer extends ScilabSpriteDrawer {
        private final int[] coordinate;

        public SquareSpriteDrawer(Appearance appearance, int size) {
            super(appearance, size);
            int r = size / 2;
            coordinate = new int[] {-r,-r, +r,-r, +r,+r, -r,+r};
        }

        @Override
        public void draw(SpriteDrawingTools spriteDrawingTools) {
            spriteDrawingTools.fillPolygon(coordinate, appearance);
        }
    }

    /**
     * Triangle right sprite
     * Scilab ID = 12
     */
    private static class TriangleRightSpriteDrawer extends TriangleSpriteDrawer {
        public TriangleRightSpriteDrawer(Appearance appearance, int size) {
            super(appearance, size);

            coordinates = new int[] {-basey,basex, -basey,-basex, r,0};
        }
    }

    /**
     * Triangle left sprite
     * Scilab ID = 13
     */
    private static class TriangleLeftSpriteDrawer extends TriangleSpriteDrawer {
        public TriangleLeftSpriteDrawer(Appearance appearance, int size) {
            super(appearance, size);

            coordinates = new int[] {basey,basex, -r,0, basey,-basex};
        }
    }

    /**
     * Pentagram sprite (five-pointed star)
     * Scilab ID = 14
     */
    private static class PentagramSpriteDrawer extends ScilabSpriteDrawer {
        private final int[] coordinates;

        private static final double COS_PI_OVER_10 = Math.cos(Math.PI/10.0);
        private static final double SIN_PI_OVER_10 = Math.sin(Math.PI/10.0);
        private static final double COS_3_PI_OVER_10 = Math.cos(3.0*Math.PI/10.0);
        private static final double SIN_3_PI_OVER_10 = Math.sin(3.0*Math.PI/10.0);

        public PentagramSpriteDrawer(Appearance appearance, int size) {
            /* To do: check whether modifying size is correct */
            super(appearance, (int)(Math.sqrt(2.0)*(double)size));

            int r = size / 2;

            double tmpy = SIN_PI_OVER_10*(double)r;

            int x = (int)(COS_PI_OVER_10*(double)r);
            int y = (int)(SIN_PI_OVER_10*(double)r);

            int x2 = (int)(COS_3_PI_OVER_10*(double)r);
            int y2 = (int)(SIN_3_PI_OVER_10*(double)r);

            double tmpinnr = tmpy / SIN_3_PI_OVER_10;


            int xinn = (int)(tmpinnr * COS_3_PI_OVER_10);
            int yinn = y;

            int xinn2 = (int)(tmpinnr * COS_PI_OVER_10);
            int yinn2 = (int)(tmpinnr * SIN_PI_OVER_10);

            int innr = (int)tmpinnr;

            coordinates = new int[] {-x2,y2, -xinn2,yinn2, -x,-y, -xinn,-yinn, 0,-r, xinn,-yinn, x,-y, xinn2,yinn2, x2,y2, 0,innr};
        }

        @Override
        public void draw(SpriteDrawingTools spriteDrawingTools) {
            spriteDrawingTools.fillPolygon(coordinates, appearance);
        }
    }
}
