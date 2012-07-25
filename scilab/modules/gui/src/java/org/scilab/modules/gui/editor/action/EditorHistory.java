package org.scilab.modules.gui.editor.action;

import java.util.LinkedList;
import java.util.ArrayDeque;
import java.util.Iterator;


public class EditorHistory implements ActionHistory {


    LinkedList<Action> history;

    Integer head;
    private final Integer MAX_HISTORY_SIZE = 20;

    public EditorHistory() {

        /** initialize the history list */
        history = new LinkedList<Action>();
    }

    /**
    * Add an action to list at the position where is the pointer
    * and remove all action that are over the last added
    *
    * @param action the action to add
    */
    public void addAction(Action action) {

    	if (head == null) {
		    head = 0;
        }
        while(head > 0) {
            /*same as removeFirst without throwing exception if the deque is empty*/
            Action a = history.pollFirst();
            a.dispose();
            --head;
    	}
    	
    	/*history reached max capacity? remove oldest action*/
        if(history.size() == MAX_HISTORY_SIZE) {
            Action a = history.pollLast();
            a.dispose();
    	}
        history.addFirst(action);
    }

    /**
    * Undo the action at head
    */
    public void undo() {

        Action action = history.get(head);
        if (action.exists()) {
            action.undo();
            ++head;
        } else {
            history.remove(head);
        }
    }

    /**
    * Redo the action at position 'put'
    */
    public void redo() {

        if (head > 0) {
            --head;
            Action action = history.get(head);

            if (action.exists()) {
                action.redo();
            } else {
            	Action a = history.get(head);
            	a.dispose();
                history.remove(head);
            }

        }
    }

    public void removeAction() {
        history.pollLast();
    }

    /**
    * Clean the list and delete all objects involved
    */
    public void dispose() {

        Iterator i = history.iterator();
        while (i.hasNext()) {
            Action action = (Action)i.next();
            action.dispose();
        }
        history.clear();
    }

    public boolean isUndoEnabled() {

        if (history.size() > 0 && head < (history.size() - 1) && head >= 0) {
            return true;
        }
        return false;
    }

    public boolean isRedoEnabled() {

        if (history.size() > 0 && head > 0 && head < history.size()) {
            return true;
        }
        return false;
    }
}
