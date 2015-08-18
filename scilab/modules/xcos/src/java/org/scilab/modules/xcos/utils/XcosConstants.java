/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement David
 * Copyright (C) 2015 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.utils;

import java.awt.Dimension;

import org.scilab.modules.graph.utils.ScilabGraphConstants;

/**
 * Contains all the constants used trough the source code.
 */
public final class XcosConstants extends ScilabGraphConstants {

    /** Define the default block width on the palette */
    public static final int PALETTE_BLOCK_WIDTH = 100;
    /** Define the default block height on the palette */
    public static final int PALETTE_BLOCK_HEIGHT = 100;

    /** Define the default block height on the palette */
    public static final double PALETTE_BLOCK_ICON_RATIO = 1.5;

    /** Define the default block horizontal margin on the palette */
    public static final int PALETTE_HMARGIN = 5;
    /** Define the default block vertical margin on the palette */
    public static final int PALETTE_VMARGIN = 5;

    /** Define the maximum number of char that might be represented as style */
    public static final int MAX_CHAR_IN_STYLE = 24;

    /** Define the maximum number of blocks on the "Recently Used Blocks" palette */
    public static final int MAX_RECENTLY_USED_BLOCKS = 12;

    /** Define the history length */
    public static final int HISTORY_LENGTH = 20;

    /** Define the maximum number of hits to be displayed */
    public static final int MAX_HITS = 150;

    /** the size of the palette block **/
    public enum PaletteBlockSize {
        /** tiny **/
        TINY,
        /** small size **/
        SMALL,
        /** normal size **/
        NORMAL,
        /** large size **/
        LARGE,
        /** Extra large size **/
        XLARGE;

        private static final Dimension DIM_TINY = new Dimension(50, 50);
        private static final Dimension DIM_SMALL = new Dimension(75, 75);
        private static final Dimension DIM_NORMAL = new Dimension(100, 100);
        private static final Dimension DIM_LARGE = new Dimension(120, 120);
        private static final Dimension DIM_XLARGE = new Dimension(140, 140);

        /**
         * @return font size
         */
        public int getFontSize() {
            switch (this) {
                case TINY:
                    return 10;
                case SMALL:
                    return 11;
                case NORMAL:
                    return 12;
                case LARGE:
                    return 13;
                case XLARGE:
                    return 13;
                default:
                    return 12;
            }
        }

        /**
         * @return maximum icon height
         */
        public int getMaxIconHeight() {
            // 65% of the frame height
            return (int) (getBlockDimension().height * 0.65f);
        }

        /**
         * @return maximum icon width
         */
        public int getMaxIconWidth() {
            // 65% of the frame width
            return (int) (getBlockDimension().width * 0.65f);
        }

        /**
         * @return block dimension (width and height)
         */
        public Dimension getBlockDimension() {
            switch (this) {
                case TINY:
                    return DIM_TINY;
                case SMALL:
                    return DIM_SMALL;
                case NORMAL:
                    return DIM_NORMAL;
                case LARGE:
                    return DIM_LARGE;
                case XLARGE:
                    return DIM_XLARGE;
                default:
                    return DIM_NORMAL;
            }
        }

        /**
         * Get the next size
         * @return enum or null if there is no next
         */
        public PaletteBlockSize next() {
            if (this.ordinal() < values().length - 1) {
                return values()[ordinal() + 1];
            }
            return null;
        }

        /**
         * Get the previous size
         * @return enum or null if there is no previous
         */
        public PaletteBlockSize previous() {
            if (this.ordinal() > 0) {
                return values()[ordinal() - 1];
            }
            return null;
        }
	}

    /* Events */
    /**
     * When a block changed
     */
    public static final String EVENT_BLOCK_UPDATED = "block";
    /** Change event old name */
    public static final String EVENT_CHANGE_OLD = "old";
    /** Change event new name */
    public static final String EVENT_CHANGE_NEW = "new";

    /* SCI environment */
    /** Path from SCI or SCIHOME to the Xcos configuration directory */
    public static final String XCOS_ETC = "/modules/xcos/etc";

    /** This class is a static singleton, thus it must not be instantiated */
    private XcosConstants() {
    }
}
