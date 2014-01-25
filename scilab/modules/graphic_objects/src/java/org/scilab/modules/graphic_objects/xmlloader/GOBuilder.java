package org.scilab.modules.graphic_objects.xmlloader;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DOCKABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_INFOBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LAYOUT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MENUBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_NAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_RESIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TAG__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TOOLBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BORDER_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;

import java.util.Map;

import org.scilab.modules.graphic_objects.builder.Builder;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;
import org.scilab.modules.graphic_objects.utils.LayoutType;
import org.xml.sax.Attributes;

public class GOBuilder {
    public static Integer figureBuilder(GraphicController controller, Attributes attributes) {
        Integer fig = Builder.createNewFigureWithAxes();
        String item = null;

        try {
            Map<String, String> map = null;
            // hide toolbar
            controller.setProperty(fig, __GO_TOOLBAR_VISIBLE__, false);

            // hide menubar
            controller.setProperty(fig, __GO_MENUBAR_VISIBLE__, false);

            // hide infobar
            controller.setProperty(fig, __GO_INFOBAR_VISIBLE__, false);

            // remove dock
            controller.setProperty(fig, __GO_DOCKABLE__, false);

            // id
            XmlTools.setPropAsString(fig, __GO_TAG__, attributes.getValue("id"));

            // visible
            XmlTools.setPropAsBoolean(fig, __GO_VISIBLE__,
                                      attributes.getValue("visible"));

            // position
            Integer[] position = (Integer[]) controller.getProperty(fig, __GO_POSITION__);
            // posX
            item = attributes.getValue("posX");
            if (item != null) {
                position[0] = Integer.parseInt(item);
            }

            // posY
            item = attributes.getValue("posY");
            if (item != null) {
                position[1] = Integer.parseInt(item);
            }

            // set position
            controller.setProperty(fig, __GO_POSITION__, position);

            Integer[] size = (Integer[]) controller.getProperty(fig, __GO_SIZE__);
            // width
            item = attributes.getValue("width");
            if (item != null) {
                size[0] = Integer.parseInt(item);
            }

            // height
            item = attributes.getValue("height");
            if (item != null) {
                size[1] = Integer.parseInt(item);
            }

            // set size
            controller.setProperty(fig, __GO_SIZE__, size);

            // title
            XmlTools.setPropAsString(fig, __GO_NAME__, attributes.getValue("title"));

            // icon
            // XmlTools.setPropAsString(fig, __GO_ICON__,
            // attributes.getValue("icon"));

            // resizable
            XmlTools.setPropAsBoolean(fig, __GO_RESIZE__, attributes.getValue("resizable"));

            // layout
            item = attributes.getValue("layout");
            if (item != null) {
                map = CSSParser.parseLine(item);
                item = map.get("name");
                controller.setProperty(fig, __GO_LAYOUT__, LayoutType.stringToEnum(item).ordinal());
            }
        } catch (CSSParserException e) {
            e.printStackTrace();
        }

        return fig;
    }

    public static Integer uicontrolBuilder(GraphicController controller, int type, Attributes attributes, int parent) {
        Integer uic = controller.askObject(GraphicObject.getTypeFromName(type));
        return uicontrolUpdater(controller, uic, attributes, parent);
    }

    public static Integer uicontrolUpdater(GraphicController controller, int uic, Attributes attributes, int parent) {
        return uicontrolUpdater(controller, uic, attributes, parent, null);
    }

    public static Integer uicontrolUpdater(GraphicController controller, int uic, Attributes attributes, int parent, String fromModel) {
        String item = null;

        try {
            Map<String, String> map = null;
            //Common attributes

            //id
            XmlTools.setPropAsString(uic, __GO_TAG__, attributes.getValue("id"));


            //layout
            item = attributes.getValue("layout");
            if (item != null) {
                map = CSSParser.parseLine(item);
                item = map.get("name");

                LayoutType layout = LayoutType.stringToEnum(item);
                controller.setProperty(uic, __GO_LAYOUT__, layout.ordinal());
            }

            //constraints

            //get parent layout
            LayoutType layout = LayoutType.NONE;
            if (parent != 0) {
                layout = LayoutType.intToEnum((Integer)controller.getProperty(parent, __GO_LAYOUT__));
            }

            if (layout != LayoutType.NONE) {
                item = attributes.getValue("constraint");
                if (item == null && fromModel != null) {
                    //use value from model if property is not in "declaration"
                    item = fromModel;
                }

                if (item != null) {
                    map = CSSParser.parseLine(item);
                } else if (map != null) {
                    map.clear();
                }

                switch (layout) {
                    case BORDER:
                        item = XmlTools.getFromMap(map, "position");
                        controller.setProperty(uic, __GO_UI_BORDER_POSITION__, Uicontrol.BorderLayoutType.stringToEnum(item).ordinal());
                        break;
                    case GRID:

                        //String[] propList = new String[]{"",""};
                        //XmlTools.setPropAsDoubleVector(uic, __GO_UI_GRID_CONSTRAINTS__, map, new String[]{});
                        break;
                    case GRIDBAG:
                        //pos = item.indexOf("row");
                        break;
                    default:
                        break;
                }
            }
        } catch (CSSParserException e) {
            e.printStackTrace();
        }

        return uic;
    }
}
