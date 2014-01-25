package org.scilab.modules.graphic_objects.uicontrol.frame.border;

public enum FrameBorderType {
    NONE, LINE, BEVEL, SOFTBEVEL, ETCHED, TITLED, EMPTY, COMPOUND, MATTE;

    public static FrameBorderType intToEnum(Integer value) {
        switch (value) {
            default :
            case 0 :
                return NONE;
            case 1 :
                return LINE;
            case 2 :
                return BEVEL;
            case 3 :
                return SOFTBEVEL;
            case 4 :
                return ETCHED;
            case 5 :
                return TITLED;
            case 6 :
                return EMPTY;
            case 7 :
                return COMPOUND;
            case 8 :
                return MATTE;
        }
    }
}
