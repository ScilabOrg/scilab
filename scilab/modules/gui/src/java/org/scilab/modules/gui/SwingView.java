/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui;

import org.flexdock.docking.Dockable;
import org.flexdock.docking.DockingManager;
import org.flexdock.docking.activation.ActiveDockableTracker;
import org.flexdock.view.View;
import org.scilab.modules.graphic_objects.console.Console;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;
import org.scilab.modules.gui.bridge.checkbox.SwingScilabCheckBox;
import org.scilab.modules.gui.bridge.checkboxmenuitem.SwingScilabCheckBoxMenuItem;
import org.scilab.modules.gui.bridge.console.SwingScilabConsole;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.bridge.editbox.SwingScilabEditBox;
import org.scilab.modules.gui.bridge.frame.SwingScilabFrame;
import org.scilab.modules.gui.bridge.imagerenderer.SwingScilabImageRenderer;
import org.scilab.modules.gui.bridge.label.SwingScilabLabel;
import org.scilab.modules.gui.bridge.listbox.SwingScilabListBox;
import org.scilab.modules.gui.bridge.menu.SwingScilabMenu;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.bridge.popupmenu.SwingScilabPopupMenu;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.bridge.radiobutton.SwingScilabRadioButton;
import org.scilab.modules.gui.bridge.slider.SwingScilabSlider;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.uitable.SwingScilabUiTable;
import org.scilab.modules.gui.bridge.waitbar.SwingScilabWaitBar;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.ToolBarBuilder;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.gui.widget.Widget;

import javax.swing.JComponent;
import javax.swing.JSeparator;
import java.awt.Component;
import java.awt.Container;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.UUID;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;
import static org.scilab.modules.gui.utils.Debug.DEBUG;

/**
 * @author Bruno JOFRET
 * @author Vincent COUVERT
 */
public final class SwingView implements GraphicView {

    public static final String NULLUUID = new UUID(0L, 0L).toString();

    private static SwingView me;

    private Map<String, TypedObject> allObjects;

    /**
     * Constructor
     */
    private SwingView() {
        GraphicController.getController().register(this);
        allObjects = Collections.synchronizedMap(new HashMap<String, TypedObject>());
    }

    public static void registerSwingView() {
        DEBUG("SwingView", "calling registerSwingView()");
        if (me == null) {
            me = new SwingView();
        }
    }

    public static SwingViewObject getFromId(String id) {
        TypedObject typedObject = me.allObjects.get(id);

        if (typedObject == null) {
            return null;
        }

        return typedObject.getValue();
    }

    private enum UielementType {
        Console,
        CheckBox,
        Edit,
        Frame,
        Figure,
        ImageRenderer,
        ListBox,
        PopupMenu,
        Progressbar,
        PushButton,
        RadioButton,
        Slider,
        Table,
        Text,
        Uimenu,
        UiParentMenu,
        UiChildMenu,
        UiCheckedMenu,
        UiContextMenu,
        Waitbar
    }

    private class TypedObject {
        private UielementType   _type;
        private SwingViewObject _value;
        private Set<String> _children;

        public TypedObject(UielementType _type, SwingViewObject _value) {
            this._type = _type;
            this._value = _value;
            this._children = Collections.synchronizedSet(new HashSet<String>());
        }

        public UielementType getType() {
            return _type;
        }

        public SwingViewObject getValue() {
            return _value;
        }

        public Set<String> getChildren() {
            return _children;
        }

        public void addChild(String childUID) {
            _children.add(childUID);
        }

        public void removeChild(String childUID) {
            _children.remove(childUID);
        }

        public boolean hasChild(String childUID) {
            return _children.contains(childUID);
        }
    };

    @Override
    public void createObject(String id) {

        boolean isValid = (Boolean) GraphicController.getController().getProperty(id, __GO_VALID__);
        if (!isValid) {
            return;
        }

        String objectType = (String) GraphicController.getController().getProperty(id, __GO_TYPE__);
        DEBUG("SwingWiew", "Object Created : " + id + "with type : " + objectType);
        if (objectType.equals(__GO_FIGURE__)
                || objectType.equals(__GO_UICONTEXTMENU__)
                || objectType.equals(__GO_UIMENU__)
                || objectType.equals(__GO_CONSOLE__)
                || objectType.equals(__GO_PROGRESSIONBAR__)
                || objectType.equals(__GO_WAITBAR__)) {
            allObjects.put(id, CreateObjectFromType(objectType, id));
            return;
        }

        if (objectType.equals(__GO_UICONTROL__)) {
            String style = (String) GraphicController.getController().getProperty(id, __GO_STYLE__);
            DEBUG("SwingView", "__GO_STYLE__(" + style + ")");
            if (style != null) {
                allObjects.put(id, CreateObjectFromType(style, id));
            } else {
                allObjects.put(id, null);
            }
        }
    }

    private UielementType StyleToEnum(String style) {
        DEBUG("SwingView", "StyleToEnum(" + style + ")");
        if (style.equals(__GO_FIGURE__)) {
            return UielementType.Figure;
        } else if (style.equals(__GO_CONSOLE__)) {
            return UielementType.Console;
        } else if (style.equals(__GO_UI_CHECKBOX__)) {
            return UielementType.CheckBox;
        } else if (style.equals(__GO_UI_EDIT__)) {
            return UielementType.Edit;
        } else if (style.equals(__GO_UI_FRAME__)) {
            return UielementType.Frame;
        } else if (style.equals(__GO_UI_IMAGERENDERER__)) {
            return UielementType.ImageRenderer;
        } else if (style.equals(__GO_UI_LISTBOX__)) {
            return UielementType.ListBox;
        } else if (style.equals(__GO_UI_POPUPMENU__)) {
            return UielementType.PopupMenu;
        } else if (style.equals(__GO_UI_PUSHBUTTON__)) {
            return UielementType.PushButton;
        } else if (style.equals(__GO_UI_RADIOBUTTON__)) {
            return UielementType.RadioButton;
        } else if (style.equals(__GO_UI_SLIDER__)) {
            return UielementType.Slider;
        } else if (style.equals(__GO_UI_TABLE__)) {
            return UielementType.Table;
        } else if (style.equals(__GO_UI_TEXT__)) {
            return UielementType.Text;
        } else if (style.equals(__GO_UIMENU__)) {
            return UielementType.UiChildMenu;
        } else if (style.equals(__GO_UIPARENTMENU__)) {
            return UielementType.UiParentMenu;
        } else if (style.equals(__GO_UICHILDMENU__)) {
            return UielementType.UiChildMenu;
        } else if (style.equals(__GO_UICHECKEDMENU__)) {
            return UielementType.UiCheckedMenu;
        } else if (style.equals(__GO_PROGRESSIONBAR__)) {
            return UielementType.Progressbar;
        } else if (style.equals(__GO_WAITBAR__)) {
            return UielementType.Waitbar;
        } else if (style.equals(__GO_UICONTEXTMENU__)) {
            return UielementType.UiContextMenu;
        }
        return null;
    }

    private TypedObject CreateObjectFromType(String type, String id) {
        UielementType enumType = StyleToEnum(type);
        return new TypedObject(enumType, CreateObjectFromType(enumType, id));
    }

    private SwingViewObject CreateObjectFromType(UielementType type, String id) {
        switch (type) {
            case CheckBox:
                SwingScilabCheckBox checkBox = new SwingScilabCheckBox();
                checkBox.setId(id);
                setDefaultProperties(checkBox, id);
                return checkBox;
            case Console:
                Console console = (Console) GraphicController.getController().getObjectFromId(id);
                if (console.getScilabMode() == Console.ScilabMode.STD) {
                    WindowsConfigurationManager.restoreUUID(NULLUUID);
                    SwingScilabConsole sciConsole = ((SwingScilabConsole) ScilabConsole.getConsole().getAsSimpleConsole());
                    SwingScilabTab consoleTab = (SwingScilabTab) sciConsole.getParent();
                    consoleTab.setId(id);
                    return consoleTab;
                } else {
                    return null;
                }
            case Edit:
                SwingScilabEditBox edit = new SwingScilabEditBox();
                edit.setId(id);
                setDefaultProperties(edit, id);
                return edit;
            case Figure:
                Figure figure = (Figure) GraphicController.getController().getObjectFromId(id);
                String figureTitle = figure.getName();
                Integer figureId = figure.getId();
                if ((figureTitle != null) && (figureId != null)) {
                    figureTitle = figureTitle.replaceFirst("%d", figureId.toString());
                }

                SwingScilabWindow window = new SwingScilabWindow();

                window.setTitle(figureTitle);
                /* TOOLBAR */
                ToolBar toolBar = ToolBarBuilder.buildToolBar(SwingScilabTab.GRAPHICS_TOOLBAR_DESCRIPTOR, figureId);
                /* INFOBAR */
                TextBox infoBar = ScilabTextBox.createTextBox();

                SwingScilabTab tab = new SwingScilabTab(figureTitle, figureId, figure);
                tab.setId(id);

                tab.setMenuBar(ScilabMenuBar.createMenuBar());
                tab.setToolBar(toolBar);
                tab.setInfoBar(ScilabTextBox.createTextBox());
                window.addMenuBar(tab.getMenuBar());
                window.addToolBar(tab.getToolBar());
                window.addInfoBar(tab.getInfoBar());

                tab.setWindowIcon("graphic-window");

                tab.setParentWindowId(window.getId());

                DockingManager.dock(tab, window.getDockingPort());
                ActiveDockableTracker.requestDockableActivation(tab);

                window.setVisible(true);
                tab.setVisible(true);
                tab.setName(figureTitle);

                String infoMessage = figure.getInfoMessage();
                if ((infoMessage == null) || (infoMessage.length() == 0)) {
                    infoBar.setText("");
                } else {
                    infoBar.setText(infoMessage);
                }
                tab.update(__GO_SIZE__, (Integer[]) GraphicController.getController().getProperty(id, __GO_SIZE__));
                tab.update(__GO_POSITION__, (Integer[]) GraphicController.getController().getProperty(id, __GO_POSITION__));
                tab.update(__GO_AXES_SIZE__, (Integer[]) GraphicController.getController().getProperty(id, __GO_AXES_SIZE__));
                // TODO set other default properties
                return tab;
            case Frame:
                SwingScilabFrame frame = new SwingScilabFrame();
                frame.setId(id);
                setDefaultProperties(frame, id);
                return frame;
            case ImageRenderer:
                SwingScilabImageRenderer imageRenderer = new SwingScilabImageRenderer();
                imageRenderer.setId(id);
                return imageRenderer;
            case ListBox:
                SwingScilabListBox listBox = new SwingScilabListBox();
                listBox.setId(id);
                return listBox;
            case PopupMenu:
                SwingScilabPopupMenu popupMenu = new SwingScilabPopupMenu();
                popupMenu.setId(id);
                setDefaultProperties(popupMenu, id);
                return popupMenu;
            case Progressbar:
                SwingScilabWaitBar progressbar = new SwingScilabWaitBar();
                progressbar.setIndeterminateMode(true);
                progressbar.setId(id);
                return progressbar;
            case PushButton:
                SwingScilabPushButton pushButton = new SwingScilabPushButton();
                pushButton.setId(id);
                setDefaultProperties(pushButton, id);
                return pushButton;
            case RadioButton:
                SwingScilabRadioButton radioButton = new SwingScilabRadioButton();
                radioButton.setId(id);
                setDefaultProperties(radioButton, id);
                return radioButton;
            case Slider:
                SwingScilabSlider slider = new SwingScilabSlider();
                slider.setId(id);
                setDefaultProperties(slider, id);
                return slider;
            case Table:
                SwingScilabUiTable table = new SwingScilabUiTable();
                table.setId(id);
                return table;
            case Text:
                SwingScilabLabel text = new SwingScilabLabel();
                text.setId(id);
                setDefaultProperties(text, id);
                return text;
            case Uimenu:
                throw new UnsupportedOperationException();
            case UiParentMenu: /* SwingView internal type */
                SwingScilabMenu parentMenu = new SwingScilabMenu();
                parentMenu.setId(id);
                setMenuDefaultProperties(parentMenu, id);
                return parentMenu;
            case UiChildMenu: /* SwingView internal type */
                SwingScilabMenuItem childMenu = new SwingScilabMenuItem();
                childMenu.setId(id);
                setMenuDefaultProperties(childMenu, id);
                return childMenu;
            case UiCheckedMenu: /* SwingView internal type */
                SwingScilabCheckBoxMenuItem checkedMenu = new SwingScilabCheckBoxMenuItem(false);
                checkedMenu.setId(id);
                setMenuDefaultProperties(checkedMenu, id);
                return checkedMenu;
            case UiContextMenu:
                SwingScilabContextMenu contextMenu = new SwingScilabContextMenu();
                contextMenu.setId(id);
                return contextMenu;
            case Waitbar:
                SwingScilabWaitBar waitbar = new SwingScilabWaitBar();
                waitbar.setIndeterminateMode(false);
                waitbar.setId(id);
                return waitbar;
            default:
                return null;
        }
    }

    /**
     * Initialize all poperties according to model
     * @param uiMenuObject the uimenu
     * @param id the uimenu id
     */
    private void setMenuDefaultProperties(Widget uiMenuObject, String id) {
        SwingViewMenu.update(uiMenuObject, __GO_CHILDREN__,
                             (String[]) GraphicController.getController().getProperty(id, __GO_CHILDREN__));
        SwingViewMenu.update(uiMenuObject, __GO_CALLBACK__,
                             (String) GraphicController.getController().getProperty(id, __GO_CALLBACK__));
        SwingViewMenu.update(uiMenuObject, __GO_CALLBACKTYPE__,
                             (Integer) GraphicController.getController().getProperty(id, __GO_CALLBACKTYPE__));
        SwingViewMenu.update(uiMenuObject, __GO_UI_CHECKED__,
                             (Boolean) GraphicController.getController().getProperty(id, __GO_UI_CHECKED__));
        SwingViewMenu.update(uiMenuObject, __GO_UI_ENABLE__,
                             (Boolean) GraphicController.getController().getProperty(id, __GO_UI_ENABLE__));
        SwingViewMenu.update(uiMenuObject, __GO_UI_FOREGROUNDCOLOR__,
                             (Double[]) GraphicController.getController().getProperty(id, __GO_UI_FOREGROUNDCOLOR__));
        SwingViewMenu.update(uiMenuObject, __GO_UI_LABEL__,
                             (String) GraphicController.getController().getProperty(id, __GO_UI_LABEL__));
        SwingViewMenu.update(uiMenuObject, __GO_UI_ICON__,
                             (String) GraphicController.getController().getProperty(id, __GO_UI_ICON__));
    }

    /**
     * Initialize all poperties according to model
     * @param uiControlObject the uicontrol
     * @param id the uicontrol id
     */
    private void setDefaultProperties(Widget uiControlObject, String id) {
        SwingViewWidget.update(uiControlObject, __GO_UI_BACKGROUNDCOLOR__,
                               (Double[]) GraphicController.getController().getProperty(id, __GO_UI_BACKGROUNDCOLOR__));
        SwingViewWidget.update(uiControlObject, __GO_UI_ENABLE__,
                               (Boolean) GraphicController.getController().getProperty(id, __GO_UI_ENABLE__));
        SwingViewWidget.update(uiControlObject, __GO_UI_ENABLE__,
                               (Boolean) GraphicController.getController().getProperty(id, __GO_UI_ENABLE__));
        SwingViewWidget.update(uiControlObject, __GO_UI_FONTANGLE__,
                               (String) GraphicController.getController().getProperty(id, __GO_UI_FONTANGLE__));
        SwingViewWidget.update(uiControlObject, __GO_UI_FONTNAME__,
                               (String) GraphicController.getController().getProperty(id, __GO_UI_FONTNAME__));
        SwingViewWidget.update(uiControlObject, __GO_UI_FONTSIZE__,
                               (Double) GraphicController.getController().getProperty(id, __GO_UI_FONTSIZE__));
        SwingViewWidget.update(uiControlObject, __GO_UI_FONTWEIGHT__,
                               (String) GraphicController.getController().getProperty(id, __GO_UI_FONTWEIGHT__));
        SwingViewWidget.update(uiControlObject, __GO_UI_FOREGROUNDCOLOR__,
                               (Double[]) GraphicController.getController().getProperty(id, __GO_UI_FOREGROUNDCOLOR__));
        SwingViewWidget.update(uiControlObject, __GO_UI_HORIZONTALALIGNMENT__,
                               (String) GraphicController.getController().getProperty(id, __GO_UI_HORIZONTALALIGNMENT__));
        SwingViewWidget.update(uiControlObject, __GO_UI_RELIEF__,
                               (String) GraphicController.getController().getProperty(id, __GO_UI_RELIEF__));
        SwingViewWidget.update(uiControlObject, __GO_UI_STRING__,
                               (String[]) GraphicController.getController().getProperty(id, __GO_UI_STRING__));
        SwingViewWidget.update(uiControlObject, __GO_UI_VERTICALALIGNMENT__,
                               (String) GraphicController.getController().getProperty(id, __GO_UI_VERTICALALIGNMENT__));
        SwingViewWidget.update(uiControlObject, __GO_POSITION__,
                               (Double[]) GraphicController.getController().getProperty(id, __GO_POSITION__));
        SwingViewWidget.update(uiControlObject, __GO_VISIBLE__,
                               (Boolean) GraphicController.getController().getProperty(id, __GO_VISIBLE__));
    }

    @Override
    public void deleteObject(String id) {
        TypedObject requestedObject = allObjects.get(id);
        if (requestedObject != null) {
            switch (requestedObject.getType()) {
                case Figure:
                    SwingScilabTab tab = (SwingScilabTab) requestedObject.getValue();
                    DockingManager.close(tab);
                    DockingManager.unregisterDockable((Dockable) tab);
                    tab.close();
                    break;
                case Progressbar:
                case Waitbar:
                    SwingScilabWaitBar bar = (SwingScilabWaitBar) requestedObject.getValue();
                    bar.close();
                    break;
                default:
                    // Nothing to do
                    // uicontrol case: the object is destroyed when its parent updates its children
                    break;
            }
        }
    }

    @Override
    public void updateObject(String id, String property) {
        TypedObject registeredObject = allObjects.get(id);
        DEBUG("SwingView", "Update" + property);

        /* On uicontrol style is set after object creation */
        if (registeredObject == null && property.equals(__GO_STYLE__)) {
            String style = (String) GraphicController.getController().getProperty(id, __GO_STYLE__);
            allObjects.put(id, CreateObjectFromType(style, id));
        }

        /* Children list update */
        if (registeredObject != null && property.equals(__GO_CHILDREN__)) {
            String[] newChildren = (String[]) GraphicController.getController().getProperty(id, __GO_CHILDREN__);
            String type = (String) GraphicController.getController().getProperty(id, __GO_TYPE__);

            /*
             * FIGURE CHILDREN UPDATE
             */
            if (type.equals(__GO_FIGURE__)) {
                updateFigureChildren(id, newChildren);
            }

            /*
             * CONSOLE CHILDREN UPDATE
             */
            if (type.equals(__GO_CONSOLE__)) {
                updateConsoleChildren(id, newChildren);
            }

            /*
             * MENU CHILDREN UPDATE
             */
            if (type.equals(__GO_UIMENU__)) {
                updateMenuChildren(id, newChildren);
            }

            /*
             * CONTEXTMENU CHILDREN UPDATE
             */
            if (type.equals(__GO_UICONTEXTMENU__)) {
                updateContextMenuChildren(id, newChildren);
            }
        }

        /*
         * When the CHECKED property is updated for a UIMENU,
         * the object is converted to a SwingScilabCheckBoxMenuItem is not already of this type
         */
        if (registeredObject != null && property.equals(__GO_UI_CHECKED__)) {
            String type = (String) GraphicController.getController().getProperty(id, __GO_TYPE__);
            if (type.equals(__GO_UIMENU__)) {
                TypedObject updatedObject = allObjects.get(id);
                switch (updatedObject.getType()) {
                    case UiParentMenu:
                        allObjects.put(id, CreateObjectFromType(__GO_UICHECKEDMENU__, id));
                        registeredObject = allObjects.get(id);
                        break;
                    case UiChildMenu:
                        String parentId = (String) GraphicController.getController().getProperty(id, __GO_PARENT__);
                        int index = ((SwingScilabMenu) allObjects.get(parentId).getValue())
                                    .getComponentZOrder((SwingScilabMenuItem) allObjects.get(id).getValue());
                        ((SwingScilabMenu) allObjects.get(parentId).getValue()).remove((SwingScilabMenuItem) allObjects.get(id).getValue());
                        allObjects.put(id, CreateObjectFromType(__GO_UICHECKEDMENU__, id));
                        registeredObject = allObjects.get(id);
                        ((SwingScilabMenu) allObjects.get(parentId).getValue()).add((SwingScilabCheckBoxMenuItem) allObjects.get(id).getValue(), index);
                        return;
                    default:
                        break;
                }
            }
        }

        /*
         * When the SEPARATOR property is updated for a UIMENU,
         * When the property is set to TRUE: A separator is added if not already done
         * When the property is set to FALSE: The previous separator is removed if it exists
         */
        if (registeredObject != null && property.equals(__GO_UI_SEPARATOR__)) {
            String type = (String) GraphicController.getController().getProperty(id, __GO_TYPE__);
            if (type.equals(__GO_UIMENU__)) {
                String parentId = (String) GraphicController.getController().getProperty(id, __GO_PARENT__);
                int menuPosition = -1;
                Component[] allChildren =  ((SwingScilabMenu) allObjects.get(parentId).getValue()).getMenuComponents();
                for (int kChild = 0; kChild < allChildren.length; kChild++) {
                    if (allChildren[kChild] == (Component) allObjects.get(id).getValue()) {
                        menuPosition = kChild;
                        break;
                    }
                }
                if (menuPosition < 0) {
                    return;
                }
                boolean newSeparatorMode = (Boolean) GraphicController.getController().getProperty(id, __GO_UI_SEPARATOR__);
                if (newSeparatorMode) { // Add a separator above the menu
                    ((SwingScilabMenu) allObjects.get(parentId).getValue()).insertSeparator(menuPosition);
                } else { // Remove the separator above the menu (if there is one)
                    if (menuPosition == 0) { // There is no object above the menu
                        return;
                    }
                    if (((SwingScilabMenu) allObjects.get(parentId).getValue()).getComponent(menuPosition - 1) instanceof JSeparator) {
                        ((SwingScilabMenu) allObjects.get(parentId).getValue()).remove(menuPosition - 1);
                    }
                }
            }
        }

        if (registeredObject != null) {
            SwingViewObject swingObject = registeredObject.getValue();
            if (swingObject != null) {
                swingObject.update(property, GraphicController.getController().getProperty(id, property));
            }
        }
    }

    /**
     * Update the figure children (called by generic updateObject method)
     * @param id the id of the figure
     * @param newChildren the new children IDs list
     */
    private void updateFigureChildren(String id, String[] newChildren) {
        TypedObject updatedObject = allObjects.get(id);
        Container updatedComponent = (SwingScilabTab) updatedObject.getValue();
        boolean needRevalidate = false;

        // Add new children
        for (String childId : newChildren) {
            if (!updatedObject.hasChild(childId)) {

                // Add the child
                updatedObject.addChild(childId);

                String childType = (String) GraphicController.getController().getProperty(childId, __GO_TYPE__);

                /* Add an uicontrol */
                if (childType.equals(__GO_UICONTROL__)) {
                    ((SwingScilabTab) updatedComponent).addMember(allObjects.get(childId).getValue());
                    needRevalidate = true;
                }

                /* Add an uimenu */
                if (childType.equals(__GO_UIMENU__)) {
                    TypedObject childAsTypedObject = allObjects.get(childId);
                    switch (childAsTypedObject.getType()) {
                        case UiChildMenu:
                        case UiCheckedMenu:
                            allObjects.put(childId, CreateObjectFromType(__GO_UIPARENTMENU__, childId));
                            ((Container) ((SwingScilabTab) updatedComponent).getMenuBar().getAsSimpleMenuBar()).add((SwingScilabMenu) allObjects.get(childId).getValue());
                            break;
                        default: /* UiParentMenu */
                            ((Container) ((SwingScilabTab) updatedComponent).getMenuBar().getAsSimpleMenuBar()).add((SwingScilabMenu) allObjects.get(childId).getValue());
                            break;
                    }
                    needRevalidate = true;
                }
            }
        }

        // Remove children which have been deleted
        Set<String> newChildrenSet = new HashSet<String>(Arrays.asList(newChildren));
        // Clone the children set to avoid concurrent accesses
        String[] oldChildrenSet = (String[]) updatedObject.getChildren().toArray(new String[updatedObject.getChildren().size()]);
        for (String childId : oldChildrenSet) {
            if (!newChildrenSet.contains(childId)) {

                // Remove the child
                updatedObject.removeChild(childId);

                String childType = (String) GraphicController.getController().getProperty(childId, __GO_TYPE__);

                /* Remove an uicontrol */
                if (childType.equals(__GO_UICONTROL__)) {
                    updatedComponent.remove((Component) allObjects.get(childId).getValue());
                    needRevalidate = true;
                }

                /* Remove an uimenu */
                if (childType.equals(__GO_UIMENU__)) {
                    ((Container) ((SwingScilabTab) updatedComponent).getMenuBar().getAsSimpleMenuBar()).remove((SwingScilabMenu) allObjects.get(childId).getValue());
                    needRevalidate = true;
                }
            }
        }
        if (needRevalidate && updatedComponent != null) {
            ((View) updatedComponent).revalidate();
        }
    }

    /**
     * Update the Console menus (called by generic updateObject method)
     * @param id the id of the console object
     * @param newChildren the new children IDs list
     */
    private void updateConsoleChildren(String id, String[] newChildren) {
        TypedObject updatedObject = allObjects.get(id);
        Container updatedComponent = (SwingScilabTab) updatedObject.getValue();
        boolean needRevalidate = false;

        // Add new children
        for (String childId : newChildren) {
            if (!updatedObject.hasChild(childId)) {

                // Add the child
                updatedObject.addChild(childId);

                String childType = (String) GraphicController.getController().getProperty(childId, __GO_TYPE__);

                if (childType.equals(__GO_UIMENU__)) {
                    TypedObject childAsTypedObject = allObjects.get(childId);
                    switch (childAsTypedObject.getType()) {
                        case UiChildMenu:
                        case UiCheckedMenu:
                            allObjects.put(childId, CreateObjectFromType(__GO_UIPARENTMENU__, childId));
                            ((Container) ((SwingScilabTab) updatedObject.getValue()).getMenuBar().getAsSimpleMenuBar()).add((SwingScilabMenu) allObjects.get(childId).getValue());
                            break;
                        default: /* UiParentMenu */
                            ((Container) ((SwingScilabTab) updatedObject.getValue()).getMenuBar().getAsSimpleMenuBar()).add((SwingScilabMenu) allObjects.get(childId).getValue());
                            break;
                    }
                    needRevalidate = true;
                }
            }
        }

        // Remove children which have been deleted
        Set<String> newChildrenSet = new HashSet<String>(Arrays.asList(newChildren));
        Object[] updatedObjectChildren = updatedObject.getChildren().toArray();
        for (int i = 0 ; i < updatedObjectChildren.length ; ++i) {
            String childId = (String) updatedObjectChildren[i];
            if (!newChildrenSet.contains(childId)) {

                // Remove the child
                updatedObject.removeChild(childId);

                String childType = (String) GraphicController.getController().getProperty(childId, __GO_TYPE__);

                if (childType.equals(__GO_UIMENU__)) {
                    ((Container) ((SwingScilabTab) allObjects.get(id).getValue()).getMenuBar().getAsSimpleMenuBar()).remove((SwingScilabMenu) allObjects.get(childId).getValue());
                    needRevalidate = true;
                }
            }
        }
        if (needRevalidate && updatedComponent != null) {
            updatedComponent.validate();
        }

    }

    /**
     * Update the children of a menu menus (called by generic updateObject method)
     * @param id the id of the menu object
     * @param newChildren the new children IDs list
     */
    private void updateMenuChildren(String id, String[] newChildren) {
        TypedObject updatedObject = allObjects.get(id);
        Container updatedComponent = null;
        boolean needRevalidate = false;
        String parentId = null;
        int updatedObjectPosition = 0;

        // Add new children
        for (String childId : newChildren) {
            String childType = (String) GraphicController.getController().getProperty(childId, __GO_TYPE__);
            if (childType.equals(__GO_UIMENU__)) {
                if (!updatedObject.hasChild(childId)) {

                    // Add the child
                    updatedObject.addChild(childId);

                    TypedObject childAsTypedObject = allObjects.get(childId);
                    Object addedChild = allObjects.get(childId).getValue();
                    parentId = (String) GraphicController.getController().getProperty(id, __GO_PARENT__);
                    JComponent parent = (JComponent) allObjects.get(parentId).getValue();
                    switch (updatedObject.getType()) {
                        case UiChildMenu:
                            updatedComponent = (SwingScilabMenuItem) updatedObject.getValue();
                            switch (childAsTypedObject.getType()) {
                                case UiChildMenu:
                                    /* Replace the item by a parent menu */
                                    updatedObjectPosition = parent.getComponentZOrder((SwingScilabMenuItem) allObjects.get(id).getValue());
                                    parent.remove((SwingScilabMenuItem) allObjects.get(id).getValue());
                                    allObjects.put(id, CreateObjectFromType(__GO_UIPARENTMENU__, id));
                                    parent.add((SwingScilabMenu) allObjects.get(id).getValue(), updatedObjectPosition);
                                    /* Update the created menu */
                                    ((SwingScilabMenu) allObjects.get(id).getValue()).add((SwingScilabMenuItem) addedChild);
                                    break;
                                case UiCheckedMenu:
                                    /* Replace the item by a parent menu */
                                    updatedObjectPosition = parent.getComponentZOrder((SwingScilabCheckBoxMenuItem) allObjects.get(id).getValue());
                                    parent.remove((SwingScilabCheckBoxMenuItem) allObjects.get(id).getValue());
                                    allObjects.put(id, CreateObjectFromType(__GO_UIPARENTMENU__, id));
                                    parent.add((SwingScilabMenu) allObjects.get(id).getValue(), updatedObjectPosition);
                                    /* Update the created menu */
                                    ((SwingScilabMenu) allObjects.get(id).getValue()).add((SwingScilabCheckBoxMenuItem) addedChild);
                                    break;
                                default: /* UiParentMenu */
                                    ((SwingScilabMenu) allObjects.get(id).getValue()).add((SwingScilabMenu) addedChild);
                                    break;
                            }
                            break;
                        case UiCheckedMenu:
                            updatedComponent = (SwingScilabCheckBoxMenuItem) updatedObject.getValue();
                            switch (childAsTypedObject.getType()) {
                                case UiChildMenu:
                                    /* Replace the item by a parent menu */
                                    updatedObjectPosition = parent.getComponentZOrder((SwingScilabCheckBoxMenuItem) allObjects.get(id).getValue());
                                    parent.remove((SwingScilabCheckBoxMenuItem) allObjects.get(id).getValue());
                                    allObjects.put(id, CreateObjectFromType(__GO_UIPARENTMENU__, id));
                                    parent.add((SwingScilabMenu) allObjects.get(id).getValue(), updatedObjectPosition);
                                    /* Update the created menu */
                                    ((SwingScilabMenu) allObjects.get(id).getValue()).add((SwingScilabMenuItem) allObjects.get(childId).getValue());
                                    break;
                                case UiCheckedMenu:
                                    /* Replace the item by a parent menu */
                                    updatedObjectPosition = parent.getComponentZOrder((SwingScilabCheckBoxMenuItem) allObjects.get(id).getValue());
                                    parent.remove((SwingScilabCheckBoxMenuItem) allObjects.get(id).getValue());
                                    allObjects.put(id, CreateObjectFromType(__GO_UIPARENTMENU__, id));
                                    parent.add((SwingScilabMenu) allObjects.get(id).getValue(), updatedObjectPosition);
                                    /* Update the created menu */
                                    ((SwingScilabMenu) allObjects.get(id).getValue()).add((SwingScilabCheckBoxMenuItem) allObjects.get(childId).getValue());
                                    break;
                                default: /* UiParentMenu */
                                    ((SwingScilabMenu) allObjects.get(id).getValue()).add((SwingScilabMenu) allObjects.get(childId).getValue());
                                    break;
                            }
                            break;
                        default: /* UiParentMenu */
                            SwingScilabMenu updatedMenu = (SwingScilabMenu) updatedObject.getValue();
                            updatedComponent = updatedMenu;
                            switch (childAsTypedObject.getType()) {
                                case UiChildMenu:
                                    updatedMenu.add((SwingScilabMenuItem) allObjects.get(childId).getValue());
                                    break;
                                case UiCheckedMenu:
                                    updatedMenu.add((SwingScilabCheckBoxMenuItem) allObjects.get(childId).getValue());
                                    break;
                                default: /* UiParentMenu */
                                    updatedMenu.add((SwingScilabMenu) allObjects.get(childId).getValue());
                                    break;
                            }
                            break;
                    }
                    needRevalidate = true;
                }
            }
        }
        // Remove children which have been deleted
        Set<String> newChildrenSet = new HashSet<String>(Arrays.asList(newChildren));
        for (String childId : updatedObject.getChildren()) {
            if (!newChildrenSet.contains(childId)) {
                // Remove the child
                updatedObject.removeChild(childId);
                updatedComponent = (SwingScilabMenu) updatedObject.getValue();

                TypedObject childAsTypedObject = allObjects.get(childId);
                SwingScilabMenu updatedMenu = (SwingScilabMenu) allObjects.get(id).getValue();
                Object removedMenu = allObjects.get(childId).getValue();

                switch (childAsTypedObject.getType()) {
                    case UiChildMenu:
                        updatedMenu.remove((SwingScilabMenuItem) removedMenu);
                        break;
                    case UiCheckedMenu:
                        updatedMenu.remove((SwingScilabCheckBoxMenuItem) removedMenu);
                        break;
                    default:
                        updatedMenu.remove((SwingScilabMenu) removedMenu);
                        break;
                }
                needRevalidate = true;
            }
        }
        if (needRevalidate && updatedComponent != null) {
            updatedComponent.validate();
        }
    }
    /**
     * Update the children of a contextmenu (called by generic updateObject method)
     * @param id the id of the contextmenu object
     * @param newChildren the new children IDs list
     */
    private void updateContextMenuChildren(String id, String[] newChildren) {
        TypedObject updatedObject = allObjects.get(id);
        Container updatedComponent = null;
        boolean needRevalidate = false;

        // Add new children
        for (String childId : newChildren) {
            String childType = (String) GraphicController.getController().getProperty(childId, __GO_TYPE__);
            if (childType.equals(__GO_UIMENU__)) {
                if (!updatedObject.hasChild(childId)) {

                    // Add the child
                    updatedObject.addChild(childId);

                    TypedObject childAsTypedObject = allObjects.get(childId);
                    SwingScilabContextMenu updatedMenu = (SwingScilabContextMenu) updatedObject.getValue();
                    updatedComponent = updatedMenu;
                    switch (childAsTypedObject.getType()) {
                        case UiChildMenu:
                            updatedMenu.add((SwingScilabMenuItem) allObjects.get(childId).getValue());
                            break;
                        case UiCheckedMenu:
                            updatedMenu.add((SwingScilabCheckBoxMenuItem) allObjects.get(childId).getValue());
                            break;
                        default: /* UiParentMenu */
                            updatedMenu.add((SwingScilabMenu) allObjects.get(childId).getValue());
                            break;
                    }
                    needRevalidate = true;
                }
            }
        }
        // Remove children which have been deleted
        Set<String> newChildrenSet = new HashSet<String>(Arrays.asList(newChildren));
        for (String childId : updatedObject.getChildren()) {
            if (!newChildrenSet.contains(childId)) {
                // Remove the child
                updatedObject.removeChild(childId);
                updatedComponent = (SwingScilabMenu) updatedObject.getValue();

                TypedObject childAsTypedObject = allObjects.get(childId);
                SwingScilabContextMenu updatedMenu = (SwingScilabContextMenu) allObjects.get(id).getValue();
                Object removedMenu = allObjects.get(childId).getValue();

                switch (childAsTypedObject.getType()) {
                    case UiChildMenu:
                        updatedMenu.remove((SwingScilabMenuItem) removedMenu);
                        break;
                    case UiCheckedMenu:
                        updatedMenu.remove((SwingScilabCheckBoxMenuItem) removedMenu);
                        break;
                    default:
                        updatedMenu.remove((SwingScilabMenu) removedMenu);
                        break;
                }
                needRevalidate = true;
            }
        }
        if (needRevalidate && updatedComponent != null) {
            updatedComponent.validate();
        }
    }
}
