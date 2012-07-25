package org.scilab.modules.gui.editor.action;

public interface Action {

    void undo();
    void redo();
    boolean exists();
    void dispose();
}
