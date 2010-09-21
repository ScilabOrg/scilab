package org.scilab.modules.graphic_objects.util.FACTORY;

import java.util.HashMap;
import java.util.Map;

/**
 * Author: Lando Pierre
 * Date: june 15, 2010
 */
public class AbstractFactoryManager <objectType> implements IFactoryManager<objectType> {
    private final Map<Object, IFactory<? extends objectType>> map = new HashMap<Object, IFactory<? extends objectType>>();

    public void registerFactory(Object type, IFactory<? extends objectType> factory) {
        map.put(type, factory);
    }

    public IFactory<? extends objectType> getFactory(Object type) {
        return map.get(type);
    }

    public objectType createStuff(Object type) {
        IFactory<? extends objectType> factory = getFactory(type);
        if (factory != null) {
            return factory.createStuff();
        } else {
            return null;
        }
    }


    public objectType createStuff(String type, Object object) {
        IFactory<? extends objectType> factory = getFactory(type);
        if (factory != null) {
            return factory.createStuff(object);
        } else {
            return null;
        }
    }
}
