package org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.scilab;

import org.scilab.modules.graphic_objects.JoGLView.nativegl.NativeGL;
import org.scilab.modules.graphic_objects.JoGLView.scilab.ISciObjModel;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glCore.IGLRenderingInfo;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.IGLDrawer;

/**
 * Author: lando
 * Date: 21 sept. 2010
 */
public class PolylineGLDrawer implements IGLDrawer<ISciObjModel> {
  static {
    System.out.println("PATH : "
      + System.getProperty("java.library.path"));
    System.loadLibrary("scigraphic_objects");
  }

    /*
     * Singleton of this class.
     */
    private static final PolylineGLDrawer singleton = new PolylineGLDrawer();

    /**
     * Make constructor private for singleton.
     */
    private PolylineGLDrawer() {
    }

    public static PolylineGLDrawer getDrawer() {
        return singleton;
    }

    public String getType() {
        // TODO : Polyline.getType() en static.
        return "Polyline";
    }

    public void beginDraw(ISciObjModel shape, IGLRenderingInfo renderingInfo) {
        NativeGL.loadGLData(shape.getId());
    }

    public void endDraw(ISciObjModel shape, IGLRenderingInfo renderingInfo) {
    }

}
