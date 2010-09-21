package org.scilab.modules.graphic_objects.util.MVC;

import javax.swing.event.EventListenerList;

/**
 * Author: Lando Pierre
 * Date: june 14, 2010
 */
public abstract class AbstractDataController<dataType extends IDataModel> implements IDataController {
    private final dataType model;

    private final EventListenerList listeners = new EventListenerList();
    private boolean isUpdating = false;

    protected AbstractDataController(dataType model) {
        this.model = model;
    }

    protected dataType getModel() {
        return model;
    }

    
    public void addListener(IDataListener listener) {
        listeners.add(IDataListener.class, listener);
    }

    
    public void removeListener(IDataListener listener) {
        listeners.remove(IDataListener.class, listener);
    }

    
    public void startUpdate() {
        isUpdating = true;
    }

    
    public void endUpdate(IDataEvent event) {
        isUpdating = false;
        fireUpdate(event);
    }

    
    public void endUpdate() {
        endUpdate(new ValueChangedEvent());
    }


    protected void fireUpdate(IDataEvent event) {
        if (!isUpdating) {
            for (IDataListener listener : getListeners()) {
                listener.update(this, event);
            }
        }
    }

    protected void fireUpdate() {
        fireUpdate(new ValueChangedEvent<dataType>());
    }

    protected IDataListener[] getListeners() {
        return listeners.getListeners(IDataListener.class);
    }
}
