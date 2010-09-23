package org.scilab.modules.graphic_objects.util.MVC;

import java.util.EventListener;

/**
 * Author: Lando Pierre
 * Date: june 14, 2010
 */
public interface IDataListener extends EventListener {

    public void update(IDataController sources, IDataEvent event);
}
