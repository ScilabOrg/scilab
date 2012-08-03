/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.editor;


import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.event.MouseEvent;
import java.awt.event.KeyEvent;
import java.awt.Component;
import javax.swing.JOptionPane;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;

import org.scilab.modules.gui.editor.ScilabClipboard;
import org.scilab.modules.gui.editor.SystemClipboard;
import org.scilab.modules.gui.editor.PolylineHandler;
import org.scilab.modules.gui.editor.LabelHandler;
import org.scilab.modules.gui.editor.LegendHandler;
import org.scilab.modules.gui.editor.action.EditorHistory;
import org.scilab.modules.gui.editor.action.ActionDelete;
import org.scilab.modules.gui.editor.action.ActionLegend;
import org.scilab.modules.gui.editor.action.ActionMove;
import org.scilab.modules.gui.editor.action.ActionPaste;
import org.scilab.modules.gui.editor.action.ActionTextEdit;
import org.scilab.modules.gui.ged.Inspector;
import org.scilab.modules.localization.Messages;



/**
* Point and click figure editor.
*
* Provides polyline selection by mouse click,
* copy, cut, paste, delete, hide, unhide polylines
* by popup menus and keyboard shortcuts.
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/

public class Editor {

    JPopupMenu menu;
    JMenuItem copy, cut, paste, delete, clear, hide, unhide, clipboardCopy, labelX, labelY, labelZ, insert, remove, ged, editdata, undo, redo;
    JMenu labels, legends;

    EntityPicker.LegendInfo selectedLegend = null;
    String selected = null;
    String figureUid = null;
    Integer oriColor = 0;
    Integer[] lastClick = { 0, 0 };
    Integer[] dragClick = { 0, 0 };
    EntityPicker entityPicker;
    DataEditor dataEditor;
    boolean dataModifyEnabled = false;
    boolean dataEditEnabled = false;
    EditorHistory editorHistory;

    Component dialogComponent;

    enum SelectionType {POLYLINE, LEGEND, SURFACE};
    SelectionType selectedType;

    public Editor() {
        init();
        setSelected(null);
        setFigure(null);
        editorHistory = new EditorHistory();
        entityPicker = new EntityPicker();
        dataEditor = new DataEditor();
        dataEditor.setLeaveAction(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                leaveDataEditor();
            }
        });
    }

    /**
     * Enable / disable data modify.
     * @param b true to enable, false to disable.
     */
    public void setDataModifyEnabled(boolean b) {
        dataModifyEnabled = b;
        if (!dataModifyEnabled) {
            leaveDataEditor();
        } else if (getSelected() != null) {
            enterDataEditor();
        }
    }

    /**
     * Returns if the data modify is enabled or not.
     * @return True if enabled, false otherwise.
     */
    public boolean isDataModifyEnabled() {
        return dataModifyEnabled;
    }

    /**
     * On right mouse click, set popup menu visible.
     * @param event MouseEvent to retrieve click positon in figure.
     */
    public void onRightMouseClick(MouseEvent event) {
        if (!dataEditEnabled) {
            boolean b = ScilabClipboard.getInstance().canPaste();
            paste.setEnabled(b);

            boolean notBlank = AxesHandler.isAxesNotBlank(figureUid);
            clipboardCopy.setEnabled(notBlank);

            undo.setEnabled(editorHistory.isUndoEnabled());
            redo.setEnabled(editorHistory.isRedoEnabled());

            menu.show(event.getComponent(), event.getX(), event.getY());
            lastClick[0] = event.getX();
            lastClick[1] = event.getY();
            dialogComponent = (Component)event.getComponent();
        } else {
            dataEditor.onRightClick(event);
        }
    }

    /**
     * Check if the user clicked over a polyline.
     *
     * @param event the mouse event.
     */
    public void onLeftMouseDown(MouseEvent event) {

        lastClick[0] = event.getX();
        lastClick[1] = event.getY();

        if (!dataEditEnabled) {
            switch (event.getClickCount()) {
                case 1:
                    /*try pick a legend*/
                    selectedLegend = entityPicker.pickLegend(figureUid, lastClick);
                    if (selectedLegend != null) {
                        selectedType = SelectionType.LEGEND;
                        setSelected(selectedLegend.legend);
                    } else {
                        /*try pick a polyline*/
                        String picked = entityPicker.pick(figureUid, lastClick[0], lastClick[1]);
                        if (picked != null) {
                            selectedType = SelectionType.POLYLINE;
                            setSelected(picked);
                        } else {
                            selectedType = SelectionType.SURFACE;
                            setSelected(entityPicker.pickSurface(figureUid, lastClick));
                        }
                    }
                    break;
                case 2:
                    /*there is a polyline selected? if yes start dataEditor*/
                    if (selected != null && selectedType == SelectionType.POLYLINE && dataModifyEnabled) {
                        enterDataEditor();
                    }
                    /*on double click over a legend or label open dialog*/
                    else if (selectedLegend != null) {
                        onClickInsert(selectedLegend.polyline);
                    } else {
                        /*try pick a label and open the dialog*/
                        onClickLabel(entityPicker.pickLabel(figureUid, lastClick));
                    }
                    break;
                default:
                    break;
            }
        } else { /*data editor is enabled, pass event to it*/
            switch (event.getClickCount()) {
                case 1:
                    dataEditor.onLeftMouseDown(event);
                    break;
                case 2:
                    dataEditor.onLeftDoubleClick(event);
                    break;
                default:
                    break;
            }
        }
        dragClick[0] = lastClick[0];
        dragClick[1] = lastClick[1];
    }

    public void onLeftMouseRelease(MouseEvent event) {

        String object = getSelected();
        if (dataModifyEnabled && !dataEditEnabled && object != null) {
            editorHistory.addAction(new ActionMove(object, lastClick, dragClick, (selectedType == SelectionType.LEGEND)));
        } else {
            dataEditor.onLeftMouseRelease(event);
        }
    }

    /**
     * On drag move the selected object, if dataEditor
     * is enabled pass event to it.
     * @param event The mouse event.
     */
    public void onMouseDragged(MouseEvent event) {
        Integer[] newClick = { event.getX(), event.getY() };
        if (dataModifyEnabled) {
            if (!dataEditEnabled) {
                String objUID = getSelected();
                if (objUID != null) {
                    if (selectedType == SelectionType.LEGEND) {
                        LegendHandler.dragLegend(objUID, dragClick, newClick);
                    } else if (selectedType == SelectionType.POLYLINE) {
                        PolylineHandler.getInstance().dragPolyline(objUID, dragClick, newClick);
                    }
                }
            } else {
                dataEditor.onDrag(dragClick, newClick);
            }
        }
        dragClick[0] = newClick[0];
        dragClick[1] = newClick[1];
    }

    /**
     * Handles keyboard input
     * @param event The Key event.
     */
    void onKeyPressed(KeyEvent event) {

        if (event.isControlDown()) {
            switch (event.getKeyCode()) {
                case KeyEvent.VK_C:
                    if (getSelected() != null) {
                        onClickCopy();
                    }
                    break;
                case KeyEvent.VK_V:
                    if (ScilabClipboard.getInstance().canPaste()) {
                        onClickPaste();
                    }
                    break;
                case KeyEvent.VK_X:
                    onClickCut();
                    break;
                case KeyEvent.VK_Z:
                    if (editorHistory.isUndoEnabled()) {
                        onClickUndo();
                    }
                    break;
                default:
                    break;
            }
        } else {
            switch (event.getKeyCode()) {
                case KeyEvent.VK_DELETE:
                    onClickDelete();
                    break;
                case KeyEvent.VK_ESCAPE:
                    leaveDataEditor();
                    break;
                default:
                    break;
            }
        }
    }

    /**
     * Initializes the popup menu.
     *
     * Create the popup menu and all popup menu items
     * and set the properly action for each menu item.
     */
    public void init() {
        menu = new JPopupMenu();
        labels = new JMenu(Messages.gettext("Label"));
        legends = new JMenu(Messages.gettext("Legend"));

        copy = new JMenuItem(Messages.gettext("Copy"));
        copy.setToolTipText(Messages.gettext("Copy selected curve."));
        cut = new JMenuItem(Messages.gettext("Cut"));
        cut.setToolTipText(Messages.gettext("Cut selected curve."));
        paste = new JMenuItem(Messages.gettext("Paste"));
        paste.setToolTipText(Messages.gettext("Paste copied curve to this figure."));
        delete = new JMenuItem(Messages.gettext("Delete"));
        delete.setToolTipText(Messages.gettext("Delete selected curve."));
        clear = new JMenuItem(Messages.gettext("Clear"));
        clear.setToolTipText(Messages.gettext("Clear figure."));
        hide = new JMenuItem(Messages.gettext("Hide"));
        hide.setToolTipText(Messages.gettext("Hide selected curve."));
        unhide = new JMenuItem(Messages.gettext("Unhide all"));
        unhide.setToolTipText(Messages.gettext("Unhide all curves."));
        clipboardCopy = new JMenuItem(Messages.gettext("Copy to Clipboard"));
        clipboardCopy.setToolTipText(Messages.gettext("Copy figure to system clipboard."));
        labelX = new JMenuItem(Messages.gettext("Label X"));
        labelX.setToolTipText(Messages.gettext("Insert a label in X axis"));
        labelY = new JMenuItem(Messages.gettext("Label Y"));
        labelY.setToolTipText(Messages.gettext("Insert a label in Y axis"));
        labelZ = new JMenuItem(Messages.gettext("Label Z"));
        labelZ.setToolTipText(Messages.gettext("Insert a label in Z axis"));
        insert = new JMenuItem(Messages.gettext("Insert"));
        insert.setToolTipText(Messages.gettext("Insert a legend to current selected item"));
        remove = new JMenuItem(Messages.gettext("Remove"));
        remove.setToolTipText(Messages.gettext("Remove a legend of current selected item"));
        ged = new JMenuItem(Messages.gettext("Open Quick Editor"));
        ged.setToolTipText(Messages.gettext("Initialize the graphics editor"));
        editdata = new JMenuItem(Messages.gettext("Edit curve data"));
        editdata.setToolTipText(Messages.gettext("Enables curve data modify"));
        undo = new JMenuItem(Messages.gettext("Undo"));
        undo.setToolTipText(Messages.gettext("Undo last action"));
        redo = new JMenuItem(Messages.gettext("Redo"));
        redo.setToolTipText(Messages.gettext("Redo last undo action"));

        copy.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickCopy();
            }
        });

        cut.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickCut();
            }
        });

        paste.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickPaste();
            }
        });

        delete.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickDelete();
            }
        });

        clear.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickClear();
            }
        });

        hide.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickHide();
            }
        });

        unhide.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickUnhide();
            }
        });

        clipboardCopy.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickCCopy();
            }
        });

        labelX.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickLabel(AxesHandler.axisTo.__X__);
            }
        });

        labelY.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickLabel(AxesHandler.axisTo.__Y__);
            }
        });

        labelZ.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickLabel(AxesHandler.axisTo.__Z__);
            }
        });

        insert.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickInsert(getSelected());
            }
        });

        remove.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickRemove();
            }
        });

        ged.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickGED();
            }
        });

        editdata.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                setDataModifyEnabled(true);
            }
        });

        undo.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickUndo();
            }
        });

        redo.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickRedo();
            }
        });


        labels.add(labelX);
        labels.add(labelY);
        labels.add(labelZ);
        legends.add(insert);
        legends.add(remove);
        menu.add(copy);
        menu.add(cut);
        menu.add(paste);
        menu.addSeparator();
        menu.add(delete);
        menu.add(clear);
        menu.add(undo);
        menu.add(redo);
        menu.addSeparator();
        menu.add(hide);
        menu.add(unhide);
        menu.addSeparator();
        menu.add(clipboardCopy);
        menu.addSeparator();
        menu.add(labels);
        menu.add(legends);
        menu.add(editdata);
        menu.addSeparator();
        menu.add(ged);
    }


    /**
     * Set the current selected object, change its color,
     * if there is a previous selected object restores its color,
     * enable/disable popup menu items if there is/isn't
     * a object selected.
     *
     * @param uid object unique identifier. Null uid unselect previous selection.
     */
    public void setSelected(String uid) {

        if (CommonHandler.objectExists(selected)) {
            CommonHandler.setColor(selected, oriColor);
        }

        selected = uid;

        if (selected != null) {
            oriColor = CommonHandler.setColor(selected, -3);

            boolean spl = selectedType == SelectionType.SURFACE || selectedType == SelectionType.POLYLINE || selectedType == SelectionType.LEGEND;
            boolean sp = selectedType == SelectionType.SURFACE || selectedType == SelectionType.POLYLINE;
            boolean p = selectedType == SelectionType.POLYLINE;
            /* Enable delete if object is surface or polyline or legend*/
            delete.setEnabled(true && spl);
            /* Enable copy, cut, hide if object is surface or polyline*/
            copy.setEnabled(true && sp);
            cut.setEnabled(true && sp);
            hide.setEnabled(true && sp);
            /* Enable editdata, add legend if object is  polyline*/
            legends.setEnabled(true && p);
            editdata.setEnabled(true && p);
        } else {
            copy.setEnabled(false);
            cut.setEnabled(false);
            delete.setEnabled(false);
            hide.setEnabled(false);
            legends.setEnabled(false);
            editdata.setEnabled(false);
        }
    }

    /**
    * Get current color of the object line/mark.
    *
    * @return Returns the current color of the object.
    */
    public Integer getOriColor() {
        return oriColor;
    }

    /**
    * Set current color of the object line/mark.
    *
    * @param newScilabColor Color selected by user.
    */
    public void setOriColor(Integer newScilabColor) {
        oriColor = newScilabColor;
    }


    /**
     * Returns selected object unique identifier.
     * @return selected object uid or null if there isn't any selected.
     */
    public String getSelected() {
        if (CommonHandler.objectExists(selected)) {
            return selected;
        } else {
            setSelected(null);
            return null;
        }
    }

    /**
     * Set the figure uid wich the editor belongs.
     * @param uid Figure unique identifier.
     */
    public void setFigure(String uid) {
        figureUid = uid;
    }

    /**
     * Get the figure uid wich the editor belongs.
     * @return figure uid.
     */
    public String getFigureUid() {
        return figureUid;
    }

    /**
    * Implements copy menu item action(Callback).
    */
    public void onClickCopy() {
        if (selectedType != SelectionType.LEGEND) {
            ScilabClipboard.getInstance().copy(getSelected());
            ScilabClipboard.getInstance().setCopiedColor(oriColor);
        }
    }

    /**
     * Implements paste menu item action(Callback).
     */
    public void onClickPaste() {

        String currentObject, newObject, currentParent, newParent;
        boolean isDuplicated = false;

        currentObject = ScilabClipboard.getInstance().getCurrentObject();
        currentParent = CommonHandler.getParent(currentObject);
        String oldFigure = CommonHandler.getParentFigure(currentObject);
        if (!CommonHandler.cmpColorMap(figureUid, oldFigure)) {
            String msg =  "The colormap from source figure seems to be different from the destination figure." +
                          "\nThis may influence the final appearence from the object." +
                          "\nDo you want copy the color map too?";
            int i = JOptionPane.showConfirmDialog(dialogComponent, msg, "Warning", JOptionPane.YES_NO_OPTION);

            if (i == JOptionPane.YES_OPTION) {
                CommonHandler.cloneColorMap(oldFigure, figureUid);
            }
        }

        AxesHandler.pasteRotationAngles(currentObject, figureUid, lastClick);

        newObject = ScilabClipboard.getInstance().paste(figureUid, lastClick);
        newParent = CommonHandler.getParent(newObject);
        if (newObject == currentObject) {
            isDuplicated = false;
        }
        editorHistory.addAction(new ActionPaste(newObject, currentParent, newParent, isDuplicated));
    }

    /**
     * Implements cut menu item action
     */
    public void onClickCut() {
        String s = getSelected();
        if (s != null && selectedType != SelectionType.LEGEND) {
            setSelected(null);
            ScilabClipboard.getInstance().cut(s);
            ScilabClipboard.getInstance().setCopiedColor(oriColor);
        }
    }

    /**
     * Implements delete menu item action(Callback).
     */
    public void onClickDelete() {
        String toDelete = getSelected();
        if (toDelete != null) {
            setSelected(null);
            editorHistory.addAction(new ActionDelete(toDelete, CommonHandler.getParent(toDelete)));
            CommonHandler.cut(toDelete);
        }
    }

    /**
    * Implements clear menu item action(Callback).
    */
    public void onClickClear() {
        setSelected(null);
        String axesTo = AxesHandler.clickedAxes(figureUid, lastClick);
        if (axesTo != null) {
            PolylineHandler.getInstance().deleteAll(axesTo);
        }
    }

    /**
    * Implements hide menu item action(Callback).
    */
    public void onClickHide() {
        if (getSelected() != null) {
            CommonHandler.setVisible(selected, false);
            setSelected(null);
        }
    }

    /**
     * Implements unhide menu item action(Callback).
     */
    public void onClickUnhide() {
        CommonHandler.unhideAll(figureUid);
    }

    /**
     * Implements clipboard copy menu item action(Callback).
     */
    public void onClickCCopy() {
        SystemClipboard.copyToSysClipboard(figureUid);
    }

    /**
     * Implements label insert action(Callback).
     * @param axis axis number.
     */
    public void onClickLabel(AxesHandler.axisTo axis) {

        String axes = AxesHandler.clickedAxes(figureUid, lastClick);
        if (axes != null && axis != null) {
            String text = LabelHandler.getLabelText(axes, axis);
            String s = (String)JOptionPane.showInputDialog(
                           dialogComponent,
                           Messages.gettext("Enter the text"),
                           Messages.gettext("Set label text"),
                           JOptionPane.PLAIN_MESSAGE,
                           null,
                           null,
                           text);
            if (s != null) {
                String tmp[] = {s};
                String[] oldText = {text};
                String label = LabelHandler.setLabel(axes, tmp, axis);
                editorHistory.addAction(new ActionTextEdit(label, oldText, tmp));
            }
        }
    }

    /**
     * Implements legend insert action(Callback).
     * @param polyline Polyline to be inserted in the legend.
     */
    public void onClickInsert(String polyline) {

        String axes = AxesHandler.clickedAxes(figureUid, lastClick);
        if (axes != null) {
            String text = LegendHandler.getLegendText(axes, polyline);
            String s = (String)JOptionPane.showInputDialog(
                           dialogComponent,
                           Messages.gettext("Enter the text"),
                           Messages.gettext("Set legend text"),
                           JOptionPane.PLAIN_MESSAGE,
                           null,
                           null,
                           text);
            if (s != null) {
                String legend = LegendHandler.searchLegend(axes);
                String[] links = LegendHandler.getLinks(legend);
                String[] texts = LegendHandler.getText(legend);
                Double[] position = LegendHandler.getPosition(legend);
                LegendHandler.setLegend(axes, polyline, s);
                editorHistory.addAction(new ActionLegend(axes, links, texts, position));
            }
        }
    }

    /**
     * Implements legend remove action(Callback).
     */
    public void onClickRemove() {

        String axesTo = AxesHandler.clickedAxes(figureUid, lastClick);
        String legend = LegendHandler.searchLegend(axesTo);
        String[] links = LegendHandler.getLinks(legend);
        String[] text = LegendHandler.getText(legend);
        Double[] position = LegendHandler.getPosition(legend);
        LegendHandler.removeLegend(axesTo, selected);
        editorHistory.addAction(new ActionLegend(axesTo, links, text, position));
    }

    /**
     * Enter data editor mode.
     */
    public void enterDataEditor() {
        if (!dataEditEnabled && selectedType == SelectionType.POLYLINE) {
            dataEditor.beginEdit(selected);
            dataEditEnabled = true;
        }
    }

    /**
     * Leave data editor mode.
     */
    public void leaveDataEditor() {
        if (dataEditEnabled) {
            dataEditor.endEdit();
            dataEditEnabled = false;
        }
    }

    /**
    * Starts the GED with the property of the Figure.
    */
    public void onClickGED() {
        Inspector.getInspector("Figure" , figureUid);
    }

    /**
    * Implements Undo action(callBAck)
    */
    public void onClickUndo() {
        editorHistory.undo();
    }

    /**
    * Implements Redo action(callBack)
    */
    public void onClickRedo() {
        editorHistory.redo();
    }
}
