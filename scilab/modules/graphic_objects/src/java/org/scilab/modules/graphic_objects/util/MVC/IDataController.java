package org.scilab.modules.graphic_objects.util.MVC;

/**
 * Author: Lando Pierre
 * Date: june 14, 2010
 */
public interface IDataController extends IDataModel {

    /**
     * @param listener will no be notified of change of the data handled by this data controller.
     */
    public void addListener(IDataListener listener);

    /** 
     * @param listener will no longer be notified of change of the data handled by this data controller.
     */
    public void removeListener(IDataListener listener);

    /**
     * Notify controller a lot of value will change and it's necessary to wait before fire update to listener.
     */
    public void startUpdate();

    /**
     * Notify controller change are done and fire update to listener can be done.
     * @param event the event send to listener.
     */
    public void endUpdate(IDataEvent event);

    /**
     * Notify controller change are done and fire update to listener can be done.
     */
    public void endUpdate();
}