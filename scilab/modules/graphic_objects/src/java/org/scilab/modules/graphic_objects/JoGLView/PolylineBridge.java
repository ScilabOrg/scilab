package org.scilab.modules.graphic_objects.JoGLView;

import org.scilab.modules.graphic_objects.util.FACTORY.IFactory;

/**
 * Author: lando
 * Date: 20 sept. 2010
 */
public class PolylineBridge implements SciBridge {

    private PolylineBridge(String id) {

    }


    public static IFactory<PolylineBridge> getFactory() {
        return singleton;
    }

    private static IFactory<PolylineBridge> singleton = new IFactory<PolylineBridge> () {

        // TODO remove that.
        public PolylineBridge createStuff() {
            return null;
        }

        public PolylineBridge createStuff(Object object) {
            if (object instanceof String) {
                return new PolylineBridge((String) object);
            } else {
                // TODO : ??
                return null;
            }
        }
    };
}
