package org.scilab.modules.graphic_objects.xmlloader;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_ANTIALIASING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AUTORESIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AXES_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_BACKGROUND__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_BORDER_OPT_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACKTYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACK__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CLOSEREQUESTFCN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DEFAULT_AXES__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DOCKABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_EVENTHANDLER_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_EVENTHANDLER_NAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FIGURE_NAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FIGURE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_GRID_OPT_GRID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_GRID_OPT_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_ID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_IMMEDIATE_DRAWING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_INFOBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_INFO_MESSAGE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LAYOUT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LINE_THICKNESS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MARGINS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MENUBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MENUBAR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PIXEL_DRAWING_MODE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_RESIZEFCN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_RESIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_ROTATION_TYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TAG__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TITLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TOOLBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TOOLBAR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICONTEXTMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UIMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BACKGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BORDER_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_CHECKBOX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_EDIT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTANGLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTNAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTSIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTUNITS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTWEIGHT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FOREGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_COLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_HIGHLIGHT_IN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_IN_BORDER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_JUSTIFICATION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_OUT_BORDER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_ROUNDED__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_SHADOW_IN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_SHADOW_OUT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_TITLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_TYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GRIDBAG_ANCHOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GRIDBAG_FILL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GRIDBAG_GRID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GRIDBAG_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GRIDBAG_WEIGHT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GROUP_NAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_HORIZONTALALIGNMENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ICON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_IMAGE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LAYER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LISTBOXTOP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LISTBOX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MAX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MIN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_POPUPMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_PUSHBUTTON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_RADIOBUTTON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_RELIEF__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SCROLLABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SLIDERSTEP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SLIDER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TAB__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TEXT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TOOLTIPSTRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_UNITS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VERTICALALIGNMENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_USEDEPRECATEDLF__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VIEWPORT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;

import java.io.File;
import java.util.HashMap;
import java.util.StringTokenizer;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.scilab.modules.graphic_objects.builder.Builder;
import org.scilab.modules.graphic_objects.console.Console;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.uicontrol.frame.border.FrameBorderType;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class XMLDomLoader {

    private enum ModelType {
        BOOLEAN, BOOLEAN_ARRAY, DOUBLE, DOUBLE_ARRAY, STRING, STRING_ARRAY, INTEGER, INTEGER_ARRAY;
    };

    private static final int __NODE_SCILABGUI__ = -10;
    private static final int __NODE_OUT__       = -20;
    private static final int __NODE_IN__        = -30;
    private static final int __NODE_TITLE__     = -40;

    private static HashMap<String, Integer> nameToGO = new HashMap<String, Integer>();

    private static HashMap<String, Pair<Integer, ModelType>> figPropToGO = new HashMap<String, Pair<Integer, ModelType>>();
    private static HashMap<String, Pair<Integer, ModelType>> UiPropToGO = new HashMap<String, Pair<Integer, ModelType>>();
    private static HashMap<String, Pair<Integer, ModelType>> BorderPropToGO = new HashMap<String, Pair<Integer, ModelType>>();

    static {
        // init map to convert control name to id
        nameToGO.put("figure", __GO_FIGURE__);
        nameToGO.put("frame", __GO_UI_FRAME__);
        nameToGO.put("layer", __GO_UI_LAYER__);
        nameToGO.put("pushbutton", __GO_UI_PUSHBUTTON__);
        nameToGO.put("text", __GO_UI_TEXT__);
        nameToGO.put("checkbox", __GO_UI_CHECKBOX__);
        nameToGO.put("textfield", __GO_UI_EDIT__);
        nameToGO.put("edit", __GO_UI_EDIT__);
        nameToGO.put("image", __GO_UI_IMAGE__);
        nameToGO.put("picture", __GO_UI_IMAGE__);
        nameToGO.put("listbox", __GO_UI_LISTBOX__);
        nameToGO.put("popupmenu", __GO_UI_POPUPMENU__);
        nameToGO.put("radiobutton", __GO_UI_RADIOBUTTON__);
        nameToGO.put("slider", __GO_UI_SLIDER__);
        nameToGO.put("table", __GO_UI_TABLE__);
        nameToGO.put("tab", __GO_UI_TAB__);

        nameToGO.put("menu", __GO_UIMENU__);
        nameToGO.put("contextmenu", __GO_UICONTEXTMENU__);

        nameToGO.put("borders", __GO_UI_FRAME_BORDER__);
        nameToGO.put("out", __NODE_OUT__);
        nameToGO.put("in", __NODE_IN__);
        nameToGO.put("title", __NODE_TITLE__);

        /* system */
        nameToGO.put("scilabgui", __NODE_SCILABGUI__);

        //fill map of property -> {GO, ModelType}

        //figure property list
        figPropToGO.put("figure_position", new Pair<Integer, ModelType>(__GO_POSITION__, ModelType.INTEGER_ARRAY));
        figPropToGO.put("figure_size", new Pair<Integer, ModelType>(__GO_SIZE__, ModelType.INTEGER_ARRAY));
        figPropToGO.put("axes_size", new Pair<Integer, ModelType>(__GO_AXES_SIZE__, ModelType.INTEGER_ARRAY));
        figPropToGO.put("auto_resize", new Pair<Integer, ModelType>(__GO_AUTORESIZE__, ModelType.BOOLEAN));
        figPropToGO.put("view_port", new Pair<Integer, ModelType>(__GO_VIEWPORT__, ModelType.INTEGER_ARRAY));
        figPropToGO.put("figure_name", new Pair<Integer, ModelType>(__GO_FIGURE_NAME__, ModelType.STRING));
        figPropToGO.put("figure_id", new Pair<Integer, ModelType>(__GO_ID__, ModelType.INTEGER));
        figPropToGO.put("info_message", new Pair<Integer, ModelType>(__GO_INFO_MESSAGE__, ModelType.STRING));
        figPropToGO.put("pixel_drawing_mode", new Pair<Integer, ModelType>(__GO_PIXEL_DRAWING_MODE__, ModelType.STRING));
        figPropToGO.put("anti_aliasing", new Pair<Integer, ModelType>(__GO_ANTIALIASING__, ModelType.BOOLEAN));
        figPropToGO.put("immediate_drawing", new Pair<Integer, ModelType>(__GO_IMMEDIATE_DRAWING__, ModelType.BOOLEAN));
        figPropToGO.put("background", new Pair<Integer, ModelType>(__GO_BACKGROUND__, ModelType.INTEGER));
        figPropToGO.put("visible", new Pair<Integer, ModelType>(__GO_VISIBLE__, ModelType.BOOLEAN));
        figPropToGO.put("rotation_style", new Pair<Integer, ModelType>(__GO_ROTATION_TYPE__, ModelType.STRING));
        figPropToGO.put("event_handler", new Pair<Integer, ModelType>(__GO_EVENTHANDLER_NAME__, ModelType.STRING));
        figPropToGO.put("event_handler_enable", new Pair<Integer, ModelType>(__GO_EVENTHANDLER_ENABLE__, ModelType.BOOLEAN));
        figPropToGO.put("resizefcn", new Pair<Integer, ModelType>(__GO_RESIZEFCN__, ModelType.STRING));
        figPropToGO.put("closerequestfcn", new Pair<Integer, ModelType>(__GO_CLOSEREQUESTFCN__, ModelType.STRING));
        figPropToGO.put("resize", new Pair<Integer, ModelType>(__GO_RESIZE__, ModelType.BOOLEAN));
        figPropToGO.put("toolbar", new Pair<Integer, ModelType>(__GO_TOOLBAR__, ModelType.INTEGER));
        figPropToGO.put("toolbar_visible", new Pair<Integer, ModelType>(__GO_TOOLBAR_VISIBLE__, ModelType.BOOLEAN));
        figPropToGO.put("menubar", new Pair<Integer, ModelType>(__GO_MENUBAR__, ModelType.INTEGER));
        figPropToGO.put("menubar_visible", new Pair<Integer, ModelType>(__GO_MENUBAR_VISIBLE__, ModelType.BOOLEAN));
        figPropToGO.put("infobar_visible", new Pair<Integer, ModelType>(__GO_INFOBAR_VISIBLE__, ModelType.BOOLEAN));
        figPropToGO.put("dockable", new Pair<Integer, ModelType>(__GO_DOCKABLE__, ModelType.BOOLEAN));
        figPropToGO.put("layout", new Pair<Integer, ModelType>(__GO_LAYOUT__, ModelType.INTEGER));
        figPropToGO.put("default_axes", new Pair<Integer, ModelType>(__GO_DEFAULT_AXES__, ModelType.BOOLEAN));
        figPropToGO.put("icon", new Pair<Integer, ModelType>(__GO_UI_ICON__, ModelType.STRING));
        figPropToGO.put("tag", new Pair<Integer, ModelType>(__GO_TAG__, ModelType.STRING));
        figPropToGO.put("grid_opt_grid", new Pair<Integer, ModelType>(__GO_GRID_OPT_GRID__, ModelType.INTEGER_ARRAY));
        figPropToGO.put("grid_opt_padding", new Pair<Integer, ModelType>(__GO_GRID_OPT_PADDING__, ModelType.INTEGER_ARRAY));
        figPropToGO.put("border_opt_padding", new Pair<Integer, ModelType>(__GO_BORDER_OPT_PADDING__, ModelType.INTEGER_ARRAY));

        //uicontrol property list
        UiPropToGO.put("position", new Pair<Integer, ModelType>(__GO_POSITION__, ModelType.INTEGER));
        UiPropToGO.put("backgroundcolor", new Pair<Integer, ModelType>(__GO_UI_BACKGROUNDCOLOR__, ModelType.INTEGER_ARRAY));
        UiPropToGO.put("enable", new Pair<Integer, ModelType>(__GO_UI_ENABLE__, ModelType.BOOLEAN));
        UiPropToGO.put("fontangle", new Pair<Integer, ModelType>(__GO_UI_FONTANGLE__, ModelType.STRING));
        UiPropToGO.put("fontname", new Pair<Integer, ModelType>(__GO_UI_FONTNAME__, ModelType.STRING));
        UiPropToGO.put("fontsize", new Pair<Integer, ModelType>(__GO_UI_FONTSIZE__, ModelType.DOUBLE));
        UiPropToGO.put("fontunits", new Pair<Integer, ModelType>(__GO_UI_FONTUNITS__, ModelType.STRING));
        UiPropToGO.put("fontweight", new Pair<Integer, ModelType>(__GO_UI_FONTWEIGHT__, ModelType.STRING));
        UiPropToGO.put("foregroundcolor", new Pair<Integer, ModelType>(__GO_UI_FOREGROUNDCOLOR__, ModelType.INTEGER_ARRAY));
        UiPropToGO.put("horizontalalignment", new Pair<Integer, ModelType>(__GO_UI_HORIZONTALALIGNMENT__, ModelType.STRING));
        UiPropToGO.put("listboxtop", new Pair<Integer, ModelType>(__GO_UI_LISTBOXTOP__, ModelType.INTEGER_ARRAY));
        UiPropToGO.put("max", new Pair<Integer, ModelType>(__GO_UI_MAX__, ModelType.DOUBLE));
        UiPropToGO.put("min", new Pair<Integer, ModelType>(__GO_UI_MIN__, ModelType.DOUBLE));
        UiPropToGO.put("position", new Pair<Integer, ModelType>(__GO_POSITION__, ModelType.DOUBLE_ARRAY));
        UiPropToGO.put("relief", new Pair<Integer, ModelType>(__GO_UI_RELIEF__, ModelType.STRING));
        UiPropToGO.put("sliderstep", new Pair<Integer, ModelType>(__GO_UI_SLIDERSTEP__, ModelType.DOUBLE_ARRAY));
        UiPropToGO.put("string", new Pair<Integer, ModelType>(__GO_UI_STRING__, ModelType.STRING_ARRAY));
        UiPropToGO.put("tooltipstring", new Pair<Integer, ModelType>(__GO_UI_TOOLTIPSTRING__, ModelType.STRING_ARRAY));
        UiPropToGO.put("units", new Pair<Integer, ModelType>(__GO_UI_UNITS__, ModelType.STRING));
        UiPropToGO.put("value", new Pair<Integer, ModelType>(__GO_UI_VALUE__, ModelType.DOUBLE_ARRAY));
        UiPropToGO.put("verticalalignment", new Pair<Integer, ModelType>(__GO_UI_VERTICALALIGNMENT__, ModelType.STRING));
        UiPropToGO.put("visible", new Pair<Integer, ModelType>(__GO_VISIBLE__, ModelType.BOOLEAN));
        UiPropToGO.put("layout", new Pair<Integer, ModelType>(__GO_LAYOUT__, ModelType.INTEGER));
        UiPropToGO.put("callback", new Pair<Integer, ModelType>(__GO_CALLBACK__, ModelType.STRING));
        UiPropToGO.put("callback_type", new Pair<Integer, ModelType>(__GO_CALLBACKTYPE__, ModelType.INTEGER));
        UiPropToGO.put("gridbaggrid", new Pair<Integer, ModelType>(__GO_UI_GRIDBAG_GRID__, ModelType.INTEGER_ARRAY));
        UiPropToGO.put("gridbagweight", new Pair<Integer, ModelType>(__GO_UI_GRIDBAG_WEIGHT__, ModelType.DOUBLE_ARRAY));
        UiPropToGO.put("gridbagfill", new Pair<Integer, ModelType>(__GO_UI_GRIDBAG_FILL__, ModelType.INTEGER));
        UiPropToGO.put("gridbaganchor", new Pair<Integer, ModelType>(__GO_UI_GRIDBAG_ANCHOR__, ModelType.INTEGER));
        UiPropToGO.put("gridbagpadding", new Pair<Integer, ModelType>(__GO_UI_GRIDBAG_PADDING__, ModelType.INTEGER_ARRAY));
        UiPropToGO.put("borderposition", new Pair<Integer, ModelType>(__GO_UI_BORDER_POSITION__, ModelType.INTEGER));
        UiPropToGO.put("margins", new Pair<Integer, ModelType>(__GO_MARGINS__, ModelType.DOUBLE_ARRAY));
        UiPropToGO.put("groupname", new Pair<Integer, ModelType>(__GO_UI_GROUP_NAME__, ModelType.STRING));
        UiPropToGO.put("icon", new Pair<Integer, ModelType>(__GO_UI_ICON__, ModelType.STRING));
        UiPropToGO.put("scrollable", new Pair<Integer, ModelType>(__GO_UI_SCROLLABLE__, ModelType.BOOLEAN));
        UiPropToGO.put("gridoptgrid", new Pair<Integer, ModelType>(__GO_GRID_OPT_GRID__, ModelType.INTEGER_ARRAY));
        UiPropToGO.put("gridoptpadding", new Pair<Integer, ModelType>(__GO_GRID_OPT_PADDING__, ModelType.INTEGER_ARRAY));
        UiPropToGO.put("borderoptpadding", new Pair<Integer, ModelType>(__GO_BORDER_OPT_PADDING__, ModelType.INTEGER_ARRAY));
        UiPropToGO.put("tag", new Pair<Integer, ModelType>(__GO_TAG__, ModelType.STRING));

        //border property list
        BorderPropToGO.put("style", new Pair<Integer, ModelType>(__GO_UI_FRAME_BORDER_STYLE__, ModelType.INTEGER));
        BorderPropToGO.put("type", new Pair<Integer, ModelType>(__GO_UI_FRAME_BORDER_TYPE__, ModelType.INTEGER));
        BorderPropToGO.put("position", new Pair<Integer, ModelType>(__GO_POSITION__, ModelType.INTEGER));
        BorderPropToGO.put("hlout", new Pair<Integer, ModelType>(__GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, ModelType.STRING));
        BorderPropToGO.put("hlin", new Pair<Integer, ModelType>(__GO_UI_FRAME_BORDER_HIGHLIGHT_IN__, ModelType.STRING));
        BorderPropToGO.put("shadowout", new Pair<Integer, ModelType>(__GO_UI_FRAME_BORDER_SHADOW_OUT__, ModelType.STRING));
        BorderPropToGO.put("shadowin", new Pair<Integer, ModelType>(__GO_UI_FRAME_BORDER_SHADOW_IN__, ModelType.STRING));
        BorderPropToGO.put("position", new Pair<Integer, ModelType>(__GO_UI_FRAME_BORDER_POSITION__, ModelType.INTEGER));
        BorderPropToGO.put("hl", new Pair<Integer, ModelType>(__GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, ModelType.STRING));
        BorderPropToGO.put("shadow", new Pair<Integer, ModelType>(__GO_UI_FRAME_BORDER_SHADOW_OUT__, ModelType.STRING));
        BorderPropToGO.put("color", new Pair<Integer, ModelType>(__GO_UI_FRAME_BORDER_COLOR__, ModelType.STRING));
        BorderPropToGO.put("thickness", new Pair<Integer, ModelType>(__GO_LINE_THICKNESS__, ModelType.INTEGER));
        BorderPropToGO.put("rounded", new Pair<Integer, ModelType>(__GO_UI_FRAME_BORDER_ROUNDED__, ModelType.BOOLEAN));
        BorderPropToGO.put("title", new Pair<Integer, ModelType>(__GO_TITLE__, ModelType.STRING));
        BorderPropToGO.put("justification", new Pair<Integer, ModelType>(__GO_UI_FRAME_BORDER_JUSTIFICATION__, ModelType.INTEGER));
        BorderPropToGO.put("fontangle", new Pair<Integer, ModelType>(__GO_UI_FONTANGLE__, ModelType.STRING));
        BorderPropToGO.put("fontname", new Pair<Integer, ModelType>(__GO_UI_FONTNAME__, ModelType.STRING));
        BorderPropToGO.put("fontsize", new Pair<Integer, ModelType>(__GO_UI_FONTSIZE__, ModelType.INTEGER));
        BorderPropToGO.put("fontweight", new Pair<Integer, ModelType>(__GO_UI_FONTWEIGHT__, ModelType.STRING));

    }

    private String filename = "";
    private Integer figure = 0;

    public XMLDomLoader(String filename) {
        this.filename = filename;
    }

    public int parse() {
        return parse(filename);
    }

    public int parse(String filename) {
        try {
            this.filename = filename;
            File file = new File(filename);

            DocumentBuilder dBuilder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
            Document doc = dBuilder.parse(file);
            doc.getDocumentElement().normalize();

            if (doc.hasChildNodes()) {
                parseNode(0, doc.getChildNodes());
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return figure;
    }

    private void parseNode(Integer parent, NodeList nodes) {
        for (int i = nodes.getLength() - 1 ; i >= 0;  i--) {
            Node childNode = nodes.item(i);
            Integer child = 0;
            boolean makeRelationShip = true;

            // make sure it's element node.
            if (childNode.getNodeType() == Node.ELEMENT_NODE) {
                // get node name and value
                int nodeType = nameToGO.get(childNode.getNodeName());
                switch (nodeType) {
                    case __GO_FIGURE__: {
                        child = createFigure(childNode);
                        figure = child;
                        break;
                    }

                    case __GO_UI_CHECKBOX__:
                    case __GO_UI_EDIT__:
                    case __GO_UI_FRAME__:
                    case __GO_UI_IMAGE__:
                    case __GO_UI_LAYER__:
                    case __GO_UI_LISTBOX__:
                    case __GO_UI_POPUPMENU__:
                    case __GO_UI_PUSHBUTTON__:
                    case __GO_UI_RADIOBUTTON__:
                    case __GO_UI_SLIDER__:
                    case __GO_UI_TABLE__:
                    case __GO_UI_TAB__:
                    case __GO_UI_TEXT__: {
                        child = createUiControl(nodeType, parent, childNode);
                        break;
                    }

                    case __GO_UI_FRAME_BORDER__: {
                        makeRelationShip = false;
                        child = createBorder(childNode);
                        GraphicController.getController().setProperty(parent, __GO_UI_FRAME_BORDER__, child);
                        break;
                    }

                    case __NODE_OUT__:
                    case __NODE_IN__:
                    case __NODE_TITLE__: {
                        makeRelationShip = false;
                        break;
                    }

                    case __NODE_SCILABGUI__ : {
                        //check version
                        Node nodeVersion = childNode.getAttributes().getNamedItem("version");
                        double version = Double.parseDouble(nodeVersion.getNodeValue());
                        if (version > 1.0) {
                            //call version manager ^^
                        }

                        Integer console = Console.getConsole().getIdentifier();
                        //set usedeprecatedskin flag
                        setBooleanAttribute(console, __GO_USEDEPRECATEDLF__, childNode.getAttributes(), "usedeprecatedskin");
                        break;
                    }

                    case __GO_UICONTEXTMENU__:
                    case __GO_UIMENU__: {
                        break;
                    }


                    default: {
                        break;
                    }
                }

                if (makeRelationShip && parent != 0 && child != 0) {
                    GraphicController.getController().setGraphicObjectRelationship(parent, child);
                }

                if (childNode.hasChildNodes()) {
                    parseNode(child, childNode.getChildNodes());
                }

            }
        }
    }

    private Integer createBorder(Node node) {
        GraphicController controller = GraphicController.getController();
        Integer uib = controller.askObject(GraphicObject.getTypeFromName(__GO_UI_FRAME_BORDER__));

        NamedNodeMap attr = node.getAttributes();

        Node styleNode = attr.getNamedItem("style");
        if (styleNode != null) {
            Integer style = FrameBorderType.stringToEnum(styleNode.getNodeValue()).ordinal();
            controller.setProperty(uib, __GO_UI_FRAME_BORDER_STYLE__, style);
            attr.removeNamedItem("style");
        }

        for (int i = 0 ; i < attr.getLength() ; i++) {
            Node prop = attr.item(i);
            Pair<Integer, ModelType> pair = BorderPropToGO.get(prop.getNodeName());

            ModelType modelType = pair.getSecond();
            switch (modelType) {
                case BOOLEAN:
                    controller.setProperty(uib, pair.getFirst(), getAttributeAsBoolean(prop.getNodeValue()));
                    break;
                case BOOLEAN_ARRAY :
                    controller.setProperty(uib, pair.getFirst(), getAttributeAsBooleanArray(prop.getNodeValue()));
                    break;
                case DOUBLE:
                    controller.setProperty(uib, pair.getFirst(), getAttributeAsDouble(prop.getNodeValue()));
                    break;
                case DOUBLE_ARRAY:
                    controller.setProperty(uib, pair.getFirst(), getAttributeAsDoubleArray(prop.getNodeValue()));
                    break;
                case INTEGER:
                    controller.setProperty(uib, pair.getFirst(), getAttributeAsInteger(prop.getNodeValue()));
                    break;
                case INTEGER_ARRAY:
                    controller.setProperty(uib, pair.getFirst(), getAttributeAsIntegerArray(prop.getNodeValue()));
                    break;
                case STRING:
                    controller.setProperty(uib, pair.getFirst(), getAttributeAsString(prop.getNodeValue()));
                    break;
                case STRING_ARRAY:
                    controller.setProperty(uib, pair.getFirst(), getAttributeAsStringArray(prop.getNodeValue()));
                    break;
                default:
                    System.out.println("missing type");
                    break;
            }
        }

        //        Node tempnode = attr.getNamedItem("style");
        //        String s = getAttributeAsString(tempnode.getNodeValue());
        //        FrameBorderType style = FrameBorderType.stringToEnum(s);
        //
        //        GraphicController.getController().setProperty(uib, __GO_UI_FRAME_BORDER_STYLE__, style.ordinal());
        //        switch(style) {
        //            case BEVEL:
        //                setIntegerAttribute(uib, __GO_UI_FRAME_BORDER_TYPE__, attr, "type");
        //                setStringAttribute(uib, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, attr, "hlout");
        //                setStringAttribute(uib, __GO_UI_FRAME_BORDER_HIGHLIGHT_IN__, attr, "hlin");
        //                setStringAttribute(uib, __GO_UI_FRAME_BORDER_SHADOW_OUT__, attr, "shadowout");
        //                setStringAttribute(uib, __GO_UI_FRAME_BORDER_SHADOW_IN__, attr, "shadowin");
        //                break;
        //            case COMPOUND: {
        //                NodeList children = node.getChildNodes();
        //                if (children.getLength() == 2) {
        //                    for (int i = 0; i < children.getLength(); i++) {
        //                        Node child = children.item(i);
        //                        Integer id = createBorder(child);
        //                        if (child.getNodeName().equals("in")) {
        //                            GraphicController.getController().setProperty(uib, __GO_UI_FRAME_BORDER_IN_BORDER__, id);
        //                        } else if (child.getNodeName().equals("out")) {
        //                            GraphicController.getController().setProperty(uib, __GO_UI_FRAME_BORDER_OUT_BORDER__, id);
        //                        }
        //                    }
        //                }
        //                break;
        //            }
        //            case EMPTY:
        //                setIntegerAttribute(uib, __GO_UI_FRAME_BORDER_POSITION__, attr, "position");
        //                break;
        //            case ETCHED:
        //                setIntegerAttribute(uib, __GO_UI_FRAME_BORDER_TYPE__, attr, "type");
        //                setStringAttribute(uib, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, attr, "hl");
        //                setStringAttribute(uib, __GO_UI_FRAME_BORDER_SHADOW_OUT__, attr, "shadow");
        //                break;
        //            case LINE:
        //                setStringAttribute(uib, __GO_UI_FRAME_BORDER_COLOR__, attr, "color");
        //                setIntegerAttribute(uib, __GO_LINE_THICKNESS__, attr, "thickness");
        //                setBooleanAttribute(uib, __GO_UI_FRAME_BORDER_ROUNDED__, attr, "rounded");
        //                break;
        //            case MATTE:
        //                setIntegerAttribute(uib, __GO_UI_FRAME_BORDER_POSITION__, attr, "position");
        //                setStringAttribute(uib, __GO_UI_FRAME_BORDER_COLOR__, attr, "color");
        //                break;
        //            case SOFTBEVEL:
        //                setIntegerAttribute(uib, __GO_UI_FRAME_BORDER_TYPE__, attr, "type");
        //                setStringAttribute(uib, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, attr, "hlout");
        //                setStringAttribute(uib, __GO_UI_FRAME_BORDER_HIGHLIGHT_IN__, attr, "hlin");
        //                setStringAttribute(uib, __GO_UI_FRAME_BORDER_SHADOW_OUT__, attr, "shadowout");
        //                setStringAttribute(uib, __GO_UI_FRAME_BORDER_SHADOW_IN__, attr, "shadowin");
        //                break;
        //            case TITLED: {
        //                setStringAttribute(uib, __GO_TITLE__, attr, "title");
        //                setIntegerAttribute(uib, __GO_UI_FRAME_BORDER_JUSTIFICATION__, attr, "justification");
        //                setIntegerAttribute(uib, __GO_UI_FRAME_BORDER_POSITION__, attr, "position");
        //                setStringAttribute(uib, __GO_UI_FONTNAME__, attr, "fontname");
        //                setStringAttribute(uib, __GO_UI_FONTANGLE__, attr, "fontangle");
        //                setIntegerAttribute(uib, __GO_UI_FONTSIZE__, attr, "fontsize");
        //                setStringAttribute(uib, __GO_UI_FONTWEIGHT__, attr, "fontweight");
        //                setStringAttribute(uib, __GO_UI_FRAME_BORDER_COLOR__, attr, "color");
        //                NodeList children = node.getChildNodes();
        //
        //                if(children.getLength() == 1) {
        //                    Integer id = createBorder(children.item(0));
        //                    GraphicController.getController().setProperty(uib, __GO_UI_FRAME_BORDER_TITLE__, id);
        //                }
        //                break;
        //            }
        //            case NONE:
        //            default:
        //                break;
        //
        //        }
        //
        return uib;
    }

    private Integer createFigure(Node node) {
        GraphicController controller = GraphicController.getController();
        Boolean dockable = true;
        Integer menubar = 1;
        Integer toolbar = 1;
        Boolean default_axes = true;
        Boolean visible = true;

        NamedNodeMap attr = node.getAttributes();
        //dockable
        Node tempnode = attr.getNamedItem("dockable");
        if (tempnode != null) {
            dockable = getAttributeAsBoolean(tempnode.getNodeValue());
            attr.removeNamedItem("dockable");
        }
        //menubar
        tempnode = attr.getNamedItem("menubar");
        if (tempnode != null) {
            menubar = getAttributeAsInteger(tempnode.getNodeValue());
            attr.removeNamedItem("menubar");
        }
        //toolbar
        tempnode = attr.getNamedItem("toolbar");
        if (tempnode != null) {
            toolbar = getAttributeAsInteger(tempnode.getNodeValue());
            attr.removeNamedItem("toolbar");
        }
        //default_axes
        tempnode = attr.getNamedItem("default_axes");
        if (tempnode != null) {
            default_axes = getAttributeAsBoolean(tempnode.getNodeValue());
            attr.removeNamedItem("default_axes");
        }
        //visible
        tempnode = attr.getNamedItem("visible");
        if (tempnode != null) {
            visible = getAttributeAsBoolean(tempnode.getNodeValue());
            attr.removeNamedItem("visible");
        }

        Integer fig = Builder.createFigure(dockable, menubar, toolbar, default_axes, visible);

        for (int i = 0 ; i < attr.getLength() ; i++) {
            Node prop = attr.item(i);
            Pair<Integer, ModelType> pair = figPropToGO.get(prop.getNodeName());

            ModelType type = pair.getSecond();
            switch (type) {
                case BOOLEAN:
                    controller.setProperty(fig, pair.getFirst(), getAttributeAsBoolean(prop.getNodeValue()));
                    break;
                case BOOLEAN_ARRAY:
                    controller.setProperty(fig, pair.getFirst(), getAttributeAsBooleanArray(prop.getNodeValue()));
                    break;
                case DOUBLE:
                    controller.setProperty(fig, pair.getFirst(), getAttributeAsDouble(prop.getNodeValue()));
                    break;
                case DOUBLE_ARRAY:
                    controller.setProperty(fig, pair.getFirst(), getAttributeAsDoubleArray(prop.getNodeValue()));
                    break;
                case INTEGER:
                    controller.setProperty(fig, pair.getFirst(), getAttributeAsInteger(prop.getNodeValue()));
                    break;
                case INTEGER_ARRAY:
                    controller.setProperty(fig, pair.getFirst(), getAttributeAsIntegerArray(prop.getNodeValue()));
                    break;
                case STRING:
                    controller.setProperty(fig, pair.getFirst(), getAttributeAsString(prop.getNodeValue()));
                    break;
                case STRING_ARRAY:
                    controller.setProperty(fig, pair.getFirst(), getAttributeAsStringArray(prop.getNodeValue()));
                    break;
                default:
                    System.out.println("missing type");
                    break;
            }
        }

        return fig;
    }

    private Integer createUiControl(Integer type, Integer parent, Node node) {
        GraphicController controller = GraphicController.getController();
        NamedNodeMap attr = node.getAttributes();
        Integer uic = GraphicController.getController().askObject(GraphicObject.getTypeFromName(type));

        for (int i = 0 ; i < attr.getLength() ; i++) {
            Node prop = attr.item(i);
            Pair<Integer, ModelType> pair = UiPropToGO.get(prop.getNodeName());

            ModelType modelType = pair.getSecond();
            switch (modelType) {
                case BOOLEAN:
                    controller.setProperty(uic, pair.getFirst(), getAttributeAsBoolean(prop.getNodeValue()));
                    break;
                case BOOLEAN_ARRAY :
                    controller.setProperty(uic, pair.getFirst(), getAttributeAsBooleanArray(prop.getNodeValue()));
                    break;
                case DOUBLE:
                    controller.setProperty(uic, pair.getFirst(), getAttributeAsDouble(prop.getNodeValue()));
                    break;
                case DOUBLE_ARRAY:
                    controller.setProperty(uic, pair.getFirst(), getAttributeAsDoubleArray(prop.getNodeValue()));
                    break;
                case INTEGER:
                    controller.setProperty(uic, pair.getFirst(), getAttributeAsInteger(prop.getNodeValue()));
                    break;
                case INTEGER_ARRAY:
                    controller.setProperty(uic, pair.getFirst(), getAttributeAsIntegerArray(prop.getNodeValue()));
                    break;
                case STRING:
                    controller.setProperty(uic, pair.getFirst(), getAttributeAsString(prop.getNodeValue()));
                    break;
                case STRING_ARRAY:
                    controller.setProperty(uic, pair.getFirst(), getAttributeAsStringArray(prop.getNodeValue()));
                    break;
                default:
                    System.out.println("missing type");
                    break;
            }
        }

        return uic;
    }

    private Boolean getAttributeAsBoolean(String val) {
        if (val == null) {
            return null;
        }

        if (val.equals("on")) {
            return true;
        }

        if (val.equals("off")) {
            return false;
        }

        return false;
    }

    private void setBooleanAttribute(Integer id, int property, NamedNodeMap attributes, String attr) {
        Node node = attributes.getNamedItem(attr);
        if (node != null) {
            Boolean b = getAttributeAsBoolean(node.getNodeValue());
            if (b != null) {
                GraphicController.getController().setProperty(id, property, b);
            }
        }
    }

    private Double getAttributeAsDouble(String val) {
        if (val == null) {
            return null;
        }

        return Double.parseDouble(val);
    }

    private void setDoubleAttribute(Integer id, int property, NamedNodeMap attributes, String attr) {
        Node node = attributes.getNamedItem(attr);
        if (node != null) {
            Double d = getAttributeAsDouble(node.getNodeValue());
            if (d != null) {
                GraphicController.getController().setProperty(id, property, d);
            }
        }
    }

    private Integer getAttributeAsInteger(String val) {
        if (val == null) {
            return null;
        }

        return Integer.parseInt(val);
    }

    private void setIntegerAttribute(Integer id, int property, NamedNodeMap attributes, String attr) {
        Node node = attributes.getNamedItem(attr);
        if (node != null) {
            Integer i = getAttributeAsInteger(node.getNodeValue());
            if (i != null) {
                GraphicController.getController().setProperty(id, property, i);
            }
        }
    }

    private String getAttributeAsString(String val) {
        return val;
    }

    private void setStringAttribute(Integer id, int property, NamedNodeMap attributes, String attr) {
        Node node = attributes.getNamedItem(attr);
        if (node != null) {
            String s = getAttributeAsString(node.getNodeValue());
            if (s != null) {
                GraphicController.getController().setProperty(id, property, s);
            }
        }
    }

    private Boolean[] getAttributeAsBooleanArray(String val) {

        if (val == null) {
            return new Boolean[] {};
        }

        StringTokenizer token = new StringTokenizer(val, ",");

        Boolean[] b = new Boolean[token.countTokens()];
        int i = 0;
        while (token.hasMoreElements()) {
            String s = token.nextToken();
            if (s.equals("on")) {
                b[i++] = true;
            } else {
                b[i++] = false;
            }
        }

        return b;
    }

    private void setBooleanArrayAttribute(Integer id, int property, NamedNodeMap attributes, String attr) {
        Node node = attributes.getNamedItem(attr);
        if (node != null) {
            Boolean[] b = getAttributeAsBooleanArray(node.getNodeValue());
            if (b != null) {
                GraphicController.getController().setProperty(id, property, b);
            }
        }
    }

    private Double[] getAttributeAsDoubleArray(String val) {

        if (val == null) {
            return new Double[] {};
        }

        StringTokenizer token = new StringTokenizer(val, ",");

        Double[] d = new Double[token.countTokens()];
        int i = 0;
        while (token.hasMoreElements()) {
            String s = token.nextToken();
            d[i++] = Double.parseDouble(s);
        }

        return d;
    }

    private void setDoubleArrayAttribute(Integer id, int property, NamedNodeMap attributes, String attr) {
        Node node = attributes.getNamedItem(attr);
        if (node != null) {
            Double[] d = getAttributeAsDoubleArray(node.getNodeValue());
            if (d != null) {
                GraphicController.getController().setProperty(id, property, d);
            }
        }
    }

    private Integer[] getAttributeAsIntegerArray(String val) {

        if (val == null) {
            return new Integer[] {};
        }

        StringTokenizer token = new StringTokenizer(val, ",");

        Integer[] integer = new Integer[token.countTokens()];
        int i = 0;
        while (token.hasMoreElements()) {
            String s = token.nextToken();
            integer[i++] = Integer.parseInt(s);
        }

        return integer;
    }

    private void setIntegerArrayAttribute(Integer id, int property, NamedNodeMap attributes, String attr) {
        Node node = attributes.getNamedItem(attr);
        if (node != null) {
            Integer[] i = getAttributeAsIntegerArray(node.getNodeValue());
            if (i != null) {
                GraphicController.getController().setProperty(id, property, i);
            }
        }
    }

    private String[] getAttributeAsStringArray(String val) {

        if (val == null) {
            return null;
        }

        StringTokenizer token = new StringTokenizer(val, ",");

        String[] s = new String[token.countTokens()];
        int i = 0;
        while (token.hasMoreElements()) {
            s[i++] = token.nextToken();
        }

        return s;
    }

    private void setStringArrayAttribute(Integer id, int property, NamedNodeMap attributes, String attr) {
        Node node = attributes.getNamedItem(attr);
        if (node != null) {
            String[] s = getAttributeAsStringArray(node.getNodeValue());
            if (s != null) {
                GraphicController.getController().setProperty(id, property, s);
            }
        }
    }
}
