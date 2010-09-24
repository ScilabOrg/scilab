package org.scilab.modules.graphic_objects.util.FACTORY;

/**
 * Author: Lando Pierre
 * Date: june 15, 2010
 *
 * Interface of a factory that create object of given type ('objectType')
 */
public interface IFactory <objectType> {

    /**
     * @return a new instance of a stuff object.
     */
    objectType createStuff();

    /**
     * @param object the parameters for the new stuff
     * @return a new instance of a stuff object.
     */
    objectType createStuff(Object object);
}
