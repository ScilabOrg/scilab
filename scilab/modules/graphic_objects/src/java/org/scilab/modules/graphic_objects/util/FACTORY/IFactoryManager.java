package org.scilab.modules.graphic_objects.util.FACTORY;

/**
 * Author: Lando Pierre
 * Date: june 15, 2010
 *
 * Interface of a factory manager.
 * A factory manager is a set of factory that create object derived from the given type ('objectType').
 */
public interface IFactoryManager <objectType> {

    /**
     * Add a factory to this manager.
     * @param type the type of object this factory make.
     * @param factory the factory.
     */
    void registerFactory(Object type, IFactory<? extends objectType> factory);

    /**
     * @param type the type of object the returned factory should make.
     * @return a factory that make object of the given type.
     */
    IFactory<? extends objectType> getFactory(Object type);

    /**
     * @param type the type of the returned object.
     * @return a new object of given type, created by the corresponding factory. 
     */
    objectType createStuff(Object type);
}
