package org.scilab.modules.graphic_objects.graphicController;

import java.rmi.server.UID;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

import org.scilab.modules.graphic_objects.graphicModel.GraphicModel;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;

//import demo.mvc.model.Model;
//import demo.mvc.model.INetworkObject.Type;
//import demo.mvc.view.IView;

/**
 * GraphicController class
 * @author Bruno Jofret
 */
public class GraphicController {

    private void DEBUG(String message) {
    	System.err.println("[DEBUG] Controller."+message);
    }

    private void LOG(String message) {
    	System.err.println("[LOG] Controller."+message);
    }

    // TBD, deactivated for now
    //private Set<IView> allViews = new HashSet<IView>();
    
    private static GraphicController me = null; 
    
    /**
     * Default constructor
     */
    private GraphicController() { }
    
    /**
     * Returns the controller
     * @return the controller
     */
    public static GraphicController getController() {
	if (me == null) {
	    me = new GraphicController();
	}
	
	return me;
    }
    
    // TBD, deactivated for now
    /*
    public void register(IView view) {
	allViews.add(view);
    }
    */
    
    /**
     * Creates a UID
     * @return the created UID
     */
    public UID createUID() {
    	return new UID();
    }
    
    /**
     * Returns the object associated to an id 
     * @param id the object id
     * @return the object
     */
    public Object getObjectFromId(UID id) {
    	Object object;
    	object = GraphicModel.getModel().getObjectFromId(id);

    	return object;
    }
    
    /**
     * Fast property set method
     * @param id the object id
     * @param prop the property name
     * @param value the property value
     */
    public void setPropertyFast(UID id, String prop, Object value) {
    	GraphicModel.getModel().setPropertyFast(id, prop, value);
    }

    /**
     * Fast property get method
     * @param id the object id
     * @param prop the property name
     * @return the property value
     */
    public Object getPropertyFast(UID id, String prop) {
    	return GraphicModel.getModel().getPropertyFast(id, prop);
    }

    /**
     * Sets a property
     * @param id the object id
     * @param prop the property name
     * @param value the property value
     */
    public void setProperty(UID id, String prop, Object value) {
	//	LOG("setProperty id = "+id+" prop = "+prop);
    //	System.out.format("setProperty: %s %s\n", prop, value.toString());
    	GraphicModel.getModel().setProperty(id, prop, value);
    	objectUpdate(id);
    }
    
    /**
     * Returns a null property
     * @param id the object id
     * @param prop the property name
     * @return the null property
     */
    public Object getNullProperty(UID id, String prop) {
    	//LOG("getProperty id = "+id+" prop = "+prop);

    	return GraphicModel.getModel().getNullProperty(id, prop);
    }
    
    /**
     * Returns a property
     * @param id the object id
     * @param prop the property name
     * @return the property value
     */
	public Object getProperty(UID id, String prop) {
		//LOG("getProperty id = "+id+" prop = "+prop);
		return GraphicModel.getModel().getProperty(id, prop);
    }
    
	/**
	 * Asks the model to create a new object
	 * @param type the object type
	 * @return the created object's id
	 */
    public UID askObject(Type type) {
    	//LOG("askObject()");
    	UID id = createUID();
    	GraphicModel.getModel().createObject(id, type);
    	objectCreated(id);
	
    	return id;
    }
    
    
    /**
     * Notifies the existing views that an object has been created
     * @param id the created object's id
     */
    public void objectCreated(UID id) {
	//LOG("objectCreated id = "+id);
    	
   	// TBD, deactivated for now
    
   	/*
	Iterator<IView> itr = allViews.iterator();
	while (itr.hasNext()) {
		System.out.format("createObject: %s\n", id.toString());
	    IView currentView = itr.next();
	    currentView.createObject(id);
	}
	objectUpdate(id);
	*/
    }

    /**
     * Notified the existing views that an object has been updated 
     * @param id the updated object's id
     */
    public void objectUpdate(UID id) {
	//LOG("objectUpdate id = "+id);
    	
    // TBD, deactivated for now
    /*
	Iterator<IView> itr = allViews.iterator();
	while (itr.hasNext()) {
	    itr.next().updateObject(id);
	}
	*/
    }


    /**
     * Deletes an object
     * @param id the deleted object's id
     */
    public void deleteObject(UID id) {
	   LOG("deleteObject id = " + id);
	   GraphicModel.getModel().deleteObject(id);
	   
	   // TBD, deactivated for now
	   /*
	   Iterator<IView> itr = allViews.iterator();
		while (itr.hasNext()) {
		    itr.next().deleteObject(id);
		}
		*/
    }
}
