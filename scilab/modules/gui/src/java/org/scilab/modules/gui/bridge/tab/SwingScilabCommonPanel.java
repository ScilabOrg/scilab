/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.gui.bridge.tab;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AUTORESIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AXES_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_BORDER_OPT_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACK__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CHILDREN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_EVENTHANDLER_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_EVENTHANDLER_NAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_GRID_OPT_GRID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_GRID_OPT_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_ID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_INFOBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_INFO_MESSAGE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LAYOUT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MENUBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_NAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_RESIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TOOLBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICHECKEDMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICHILDMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UIMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UIPARENTMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;

import javax.swing.JLayeredPane;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicModel.GraphicModel;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;
import org.scilab.modules.graphic_objects.utils.LayoutType;
import org.scilab.modules.gui.SwingView;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.bridge.frame.SwingScilabFrame;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.events.callback.ScilabCloseCallBack;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.ToolBarBuilder;

public class SwingScilabCommonPanel {

    public static final String GRAPHICS_TOOLBAR_DESCRIPTOR = System.getenv("SCI") + "/modules/gui/etc/graphics_toolbar.xml";

    /**
     * Update the tab after a modification of its properties
     * @param property the property name
     * @param value the property value
     * @see org.scilab.modules.gui.SwingViewObject#update(java.lang.String, java.lang.Object)
     */
    protected static void update(SwingScilabPanel component, int property, Object value) {
        String name;
        Integer figureId;
        switch (property) {
            case  __GO_NAME__ :
                name = ((String) value);
                figureId = (Integer) GraphicController.getController().getProperty(component.getId(), __GO_ID__);
                updateTitle(component, name, figureId);
                break;
            case __GO_ID__ :
                /* Update title */
                figureId = ((Integer) value);
                name = (String) GraphicController.getController().getProperty(component.getId(), __GO_NAME__);
                updateTitle(component, name, figureId);

                /** Update tool bar */
                component.setToolBar(ToolBarBuilder.buildToolBar(GRAPHICS_TOOLBAR_DESCRIPTOR, figureId));
                SwingScilabWindow parentWindow = SwingScilabWindow.allScilabWindows.get(component.getParentWindowId());

                /* Update callback */
                String closingCommand =
                    "if (get_figure_handle(" + figureId + ") <> []) then"
                    +      "  if (get(get_figure_handle(" + figureId + "), 'event_handler_enable') == 'on') then"
                    +      "    execstr(get(get_figure_handle(" + figureId + "), 'event_handler')+'(" + figureId + ", -1, -1, -1000)', 'errcatch', 'm');"
                    +      "  end;"
                    +      "  delete(get_figure_handle(" + figureId + "));"
                    +      "end;";
                component.setCallback(null);
                component.setCallback(ScilabCloseCallBack.create(component.getId(), closingCommand));
                /* Update menus callback */
                Integer[] children = (Integer[]) GraphicController.getController().getProperty(component.getId(), __GO_CHILDREN__);
                updateChildrenCallbacks(children, figureId);
                break;
            case __GO_SIZE__ :
                Integer[] size = (Integer[]) value;
                SwingScilabWindow figure = SwingScilabWindow.allScilabWindows.get(component.getParentWindowId());
                Size oldFigureSize = figure.getDims();
                figure.setDims(new Size(size[0], size[1]));
                int deltaFigureX = size[0] - oldFigureSize.getWidth();
                int deltaFigureY = size[1] - oldFigureSize.getHeight();
                if ( oldFigureSize.getWidth() != 0 && oldFigureSize.getHeight() != 0
                        && ((oldFigureSize.getWidth() != size[0]) || (oldFigureSize.getHeight() != size[1]))
                        && ((Boolean) GraphicController.getController().getProperty(component.getId(), __GO_AUTORESIZE__))
                   ) {
                    Integer[] axesSize = (Integer[]) GraphicController.getController().getProperty(component.getId(), __GO_AXES_SIZE__);
                    Integer[] newAxesSize = {axesSize[0] + deltaFigureX, axesSize[1] + deltaFigureY};
                    GraphicController.getController().setProperty(component.getId(), __GO_AXES_SIZE__, newAxesSize);
                }
                break;
            case __GO_POSITION__ :
                Integer[] position = (Integer[]) value;
                SwingScilabWindow.allScilabWindows.get(component.getParentWindowId()).setPosition(new Position(position[0], position[1]));
                break;
            case __GO_AXES_SIZE__ :
                Integer[] axesSize = (Integer[]) value;
                Dimension oldAxesSize = component.getContentPane().getSize();
                if ( oldAxesSize.getWidth() != 0 && oldAxesSize.getHeight() != 0
                        && ((oldAxesSize.getWidth() != axesSize[0]) || (oldAxesSize.getHeight() != axesSize[1]))
                        && ((Boolean) GraphicController.getController().getProperty(component.getId(), __GO_AUTORESIZE__))
                   ) {
                    // TODO manage tabs when there are docked (do not change the window size if more than one tab docked)
                    int deltaX = axesSize[0] - (int) oldAxesSize.getWidth();
                    int deltaY = axesSize[1] - (int) oldAxesSize.getHeight();
                    Size parentWindowSize = SwingScilabWindow.allScilabWindows.get(component.getParentWindowId()).getDims();
                    SwingScilabWindow.allScilabWindows.get(component.getParentWindowId()).setDims(
                        new Size(parentWindowSize.getWidth() + deltaX, parentWindowSize.getHeight() + deltaY));
                    Integer figureSize[] = {parentWindowSize.getWidth() + deltaX, parentWindowSize.getHeight() + deltaY};
                    GraphicController.getController().setProperty(component.getId(), __GO_SIZE__, figureSize);
                }
                break;
            case __GO_INFO_MESSAGE__ :
                if (component.getInfoBar() != null) {
                    component.getInfoBar().setText((String) value);
                }
                break;
            case __GO_EVENTHANDLER_ENABLE__ :
                Boolean enabled = (Boolean) GraphicController.getController().getProperty(component.getId(), __GO_EVENTHANDLER_ENABLE__);
                component. setEventHandlerEnabled(enabled);
                break;
            case __GO_EVENTHANDLER_NAME__ :
                String eventHandlerName = (String) GraphicController.getController().getProperty(component.getId(), __GO_EVENTHANDLER_NAME__);
                component.setEventHandler(eventHandlerName);
                break;
            case __GO_VISIBLE__ :
                component.getContentPane().setVisible((Boolean) value);
                break;
            case __GO_INFOBAR_VISIBLE__ :
                component.getInfoBar().setVisible((Boolean) value);
                break;
            case __GO_TOOLBAR_VISIBLE__ :
                component.getToolBar().setVisible((Boolean) value);
                break;
            case __GO_MENUBAR_VISIBLE__ :
                component.getMenuBar().setVisible((Boolean) value);
                break;
            case __GO_RESIZE__ :
                component.getParentWindow().setResizable((Boolean) value);
                break;
            case __GO_LAYOUT__ :
                LayoutType newLayout = LayoutType.intToEnum((Integer) value);
                switch (newLayout) {
                    case BORDER : {
                        Integer[] padding = (Integer[]) GraphicController.getController().getProperty(component.getId(), __GO_BORDER_OPT_PADDING__);
                        component.getWidgetPane().setLayout(new BorderLayout(padding[0], padding[1]));
                        component.getWidgetPane().setLayout(new BorderLayout());
                        break;
                    }
                    case GRIDBAG : {
                        component.getWidgetPane().setLayout(new GridBagLayout());
                        break;
                    }
                    case GRID : {
                        Integer[] padding = (Integer[]) GraphicController.getController().getProperty(component.getId(), __GO_GRID_OPT_PADDING__);
                        Integer[] grid = (Integer[]) GraphicController.getController().getProperty(component.getId(), __GO_GRID_OPT_GRID__);
                        if (grid[0] == 0 && grid[1] == 0) {
                            grid[0] = 1;
                        }
                        component.getWidgetPane().setLayout(new GridLayout(grid[0], grid[1], padding[0], padding[1]));
                        break;
                    }
                    case NONE :
                    default:
                        component.getWidgetPane().setLayout(null);
                        break;
                }
                break;
            case __GO_GRID_OPT_PADDING__ :
            case __GO_GRID_OPT_GRID__ : {
                Integer layout = (Integer) GraphicController.getController().getProperty(component.getId(), __GO_LAYOUT__);
                LayoutType layoutType = LayoutType.intToEnum(layout);

                if (layoutType != LayoutType.GRID) {
                    break;
                }

                Integer[] padding = (Integer[]) GraphicController.getController().getProperty(component.getId(), __GO_GRID_OPT_PADDING__);
                Integer[] grid = (Integer[]) GraphicController.getController().getProperty(component.getId(), __GO_GRID_OPT_GRID__);
                if (grid[0] == 0 && grid[1] == 0) {
                    grid[0] = 1;
                }
                component.getWidgetPane().setLayout(new GridLayout(grid[0], grid[1], padding[0], padding[1]));
                break;
            }
            case __GO_BORDER_OPT_PADDING__ : {
                Integer layout = (Integer) GraphicController.getController().getProperty(component.getId(), __GO_LAYOUT__);
                LayoutType layoutType = LayoutType.intToEnum(layout);

                if (layoutType != LayoutType.BORDER) {
                    break;
                }

                Integer[] padding = (Integer[]) GraphicController.getController().getProperty(component.getId(), __GO_BORDER_OPT_PADDING__);
                component.getWidgetPane().setLayout(new BorderLayout(padding[0], padding[1]));
                component.getWidgetPane().setLayout(new BorderLayout());
            }
        }
    }

    /**
     * Update the title of the Tab
     * @param figureName figure_name property
     * @param figureId figure_id property
     */
    private static void updateTitle(SwingScilabPanel component, String figureName, Integer figureId) {
        if ((figureName != null) && (figureId != null)) {
            String figureTitle = figureName.replaceFirst("%d", figureId.toString());
            component.setName(figureTitle);
        }
    }

    /**
     * Update the menus callbacks when they are linked to the figure ID
     * @param children the children UID
     * @param parentFigureId the figure ID
     */
    private static void updateChildrenCallbacks(Integer[] children, int parentFigureId) {
        for (int kChild = 0; kChild < children.length; kChild++) {
            Integer childType = (Integer) GraphicController.getController().getProperty(children[kChild], __GO_TYPE__);
            if (childType != null && (
                        childType == __GO_UIMENU__
                        || childType == __GO_UIPARENTMENU__
                        || childType == __GO_UICHILDMENU__
                        || childType == __GO_UICHECKEDMENU__)) {
                String cb = (String) GraphicController.getController().getProperty(children[kChild], __GO_CALLBACK__);
                SwingView.getFromId(children[kChild]).update(__GO_CALLBACK__, replaceFigureID(cb, parentFigureId));
                Integer[] menuChildren = (Integer[]) GraphicController.getController().getProperty(children[kChild], __GO_CHILDREN__);
                updateChildrenCallbacks(menuChildren, parentFigureId);
            }
        }
    }

    /**
     * Replace pattern [SCILAB_FIGURE_ID] by the figure index
     * @param initialString string read in XML file
     * @param parentFigureId the figure ID
     * @return callback string
     */
    private static String replaceFigureID(String initialString, Integer parentFigureId) {
        return initialString.replaceAll("\\[SCILAB_FIGURE_ID\\]", Integer.toString(parentFigureId));
    }

    /**
     * Add a SwingViewObject (from SwingView.java) to container and returns its index
     * @param member the member to add
     */
    protected static void addMember(SwingScilabPanel component, SwingViewObject member) {
        //member.get
        Uicontrol uicontrol = (Uicontrol) GraphicModel.getModel().getObjectFromId(member.getId());
        if (component.getWidgetPane().getLayout() instanceof BorderLayout) {
            switch (uicontrol.getBorderPositionAsEnum()) {
                case BOTTOM:
                    component.getWidgetPane().add((Component) member, BorderLayout.SOUTH);
                    break;
                case TOP:
                    component.getWidgetPane().add((Component) member, BorderLayout.NORTH);
                    break;
                case LEFT:
                    component.getWidgetPane().add((Component) member, BorderLayout.WEST);
                    break;
                case RIGHT:
                    component.getWidgetPane().add((Component) member, BorderLayout.EAST);
                    break;
                case CENTER:
                    component.getWidgetPane().add((Component) member, BorderLayout.CENTER);
                    break;
                default:
                    break;
            }
        } else if (component.getWidgetPane().getLayout() instanceof GridBagLayout) {
            GridBagConstraints constraints = new GridBagConstraints();

            // Grid
            Integer[] grid = uicontrol.getGridBagGrid();
            constraints.gridx = grid[0];
            constraints.gridy = grid[1];
            constraints.gridwidth = grid[2];
            constraints.gridheight = grid[3];

            // Weight
            Double[] weight = uicontrol.getGridBagWeight();
            constraints.weightx = weight[0];
            constraints.weighty = weight[1];

            // Anchor
            switch (uicontrol.getGridBagAnchorAsEnum()) {
                case LEFT :
                    constraints.anchor = GridBagConstraints.EAST;
                    break;
                case UPPER :
                    constraints.anchor = GridBagConstraints.NORTH;
                    break;
                case LOWER:
                    constraints.anchor = GridBagConstraints.SOUTH;
                    break;
                case LOWER_LEFT:
                    constraints.anchor = GridBagConstraints.SOUTHEAST;
                    break;
                case LOWER_RIGHT:
                    constraints.anchor = GridBagConstraints.SOUTHWEST;
                    break;
                case RIGHT:
                    constraints.anchor = GridBagConstraints.WEST;
                    break;
                case UPPER_LEFT:
                    constraints.anchor = GridBagConstraints.NORTHEAST;
                    break;
                case UPPER_RIGHT:
                    constraints.anchor = GridBagConstraints.NORTHWEST;
                    break;
                case CENTER :
                default :
                    constraints.anchor = GridBagConstraints.CENTER;
                    break;
            }

            // Fill
            switch (uicontrol.getGridBagFillAsEnum()) {
                case BOTH :
                    constraints.fill = GridBagConstraints.BOTH;
                    break;
                case HORIZONTAL:
                    constraints.fill = GridBagConstraints.HORIZONTAL;
                    break;
                case VERTICAL:
                    constraints.fill = GridBagConstraints.VERTICAL;
                    break;
                case NONE:
                default:
                    constraints.fill = GridBagConstraints.NONE;
                    break;
            }

            // Insets
            // TODO : add Insets

            // Padding
            Integer[] padding = uicontrol.getGridBagPadding();
            constraints.ipadx = padding[0];
            constraints.ipady = padding[1];

            component.getWidgetPane().add((Component) member, constraints);
            component.getWidgetPane().revalidate();
        } else {
            if (member instanceof SwingScilabFrame) {
                component.getWidgetPane().add((Component) member, JLayeredPane.FRAME_CONTENT_LAYER);
            } else {
                component.getWidgetPane().add((Component) member, JLayeredPane.DEFAULT_LAYER + 1);
            }
        }
    }
}
