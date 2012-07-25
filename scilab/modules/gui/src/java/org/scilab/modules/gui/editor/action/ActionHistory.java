package org.scilab.modules.gui.editor.action;

public interface ActionHistory {


    void undo();
    void redo();

    void addAction(Action action);
    void removeAction();

    void dispose();
}
