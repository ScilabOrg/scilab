package org.scilab.modules.xcos.utils;

import org.scilab.modules.graph.utils.ScilabGraphConstants;

/**
 * Contains all the constants used trough the source code.
 */
public final class XcosConstants extends ScilabGraphConstants {

    /** Define the default block width on the palette */
    public static final int PALETTE_BLOCK_WIDTH = 100;
    /** Define the default block height on the palette */
    public static final int PALETTE_BLOCK_HEIGHT = 100;

    /** Define the default block horizontal margin on the palette */
    public static final int PALETTE_HMARGIN = 5;
    /** Define the default block vertical margin on the palette */
    public static final int PALETTE_VMARGIN = 5;
    
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
    private XcosConstants() { }
}
