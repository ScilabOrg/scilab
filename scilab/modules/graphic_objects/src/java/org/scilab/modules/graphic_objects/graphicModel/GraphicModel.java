package org.scilab.modules.graphic_objects.graphicModel;

import java.rmi.server.UID;
import java.util.HashMap;
import java.util.Map;

import org.scilab.modules.graphic_objects.*;

//import demo.mvc.model.INetworkObject.Type;

import org.scilab.modules.graphic_objects.GraphicObject.Type;

/**
 * GraphicModel class
 * @author Bruno Jofret
 */
public class GraphicModel {

    private void DEBUG(String message) {
    	System.err.println("[DEBUG] Model."+message);
    }

    private void LOG(String message) {
    	System.err.println("[LOG] Model."+message);
    }
    
    private Map<UID, GraphicObject> allObjects = new HashMap<UID, GraphicObject>();
    
    private static GraphicModel me = null;
    
    /**
     * Default constructor
     */
    private GraphicModel() { }
  
    /**
     * Returns the model
     * @return the model
     */
    public static GraphicModel getModel() {
	if (me == null) {
	    me = new GraphicModel();
	}
	
	return me;
    }
    
    /**
     * @param id the id of the object to get
     * @return the object
     */
    public Object getObjectFromId(UID id) {
    	return allObjects.get(id);
    }

    /**
     * Returns a null property
     * @param id the id of the object
     * @param property the property name
     * @return the property
     */
    public Object getNullProperty(UID id, String property){
    	GraphicObject object = allObjects.get(id);
    	return object.getNullProperty(property);
    }

    /**
     * Returns a property
     * @param id the id of the object
     * @param property the property name 
     * @return the property value
     */
    public Object getProperty(UID id, String property) {
	//LOG("getProperty id = " + id + " property = " + property);
	GraphicObject object = allObjects.get(id);
	return object.getProperty(property);
    }

    /**
     * Sets a property
     * @param id object id
     * @param property property name
     * @param value property value
     */
    public void setProperty(UID id, String property, Object value) {
	//LOG("setProperty id = " + id + " property = " + property + " value = " + value);
	GraphicObject object = allObjects.get(id);
	object.setProperty(property, value);
    }

    /**
     * Fast property get
     * @param id object id
     * @param property property name
     * @return property value
     */
    public Object getPropertyFast(UID id, String property) {
    	GraphicObject object = allObjects.get(id);
    	Object propertyType = object.getPropertyFromName(property);
    	return object.getPropertyFast(propertyType);
    }
    
    /**
     * Fast property set
     * @param id object id
     * @param property name
     * @param value property value
     */
    public void setPropertyFast(UID id, String property, Object value) {
    	GraphicObject object = allObjects.get(id);
    	Object propertyType = object.getPropertyFromName(property);
    	object.setPropertyFast(propertyType, value);
    }
   
    /**
     * Experimental fast property set 
     * @param id object id
     * @param property property name
     * @param value property value
     */
    public void setPropertyFastHack(UID id, String property, Object value) {
    	GraphicObject object = allObjects.get(id);
    	Object propertyType = object.getPropertyFromName2(property);
    	object.setPropertyFastHack(propertyType, value);
    }

    /**
     * Fast property get version 2
     * @param id object id
     * @param property property name
     * @return property value
     */
    public Object getPropertyFast2(UID id, String property) {
    	GraphicObject object = allObjects.get(id);
    	Object propertyType = object.getPropertyFromName2(property);
    	return object.getPropertyFast2(propertyType);
    }
    
    /**
     * Fast property set version 2
     * @param id object id
     * @param property property name
     * @param value property value
     */
    public void setPropertyFast2(UID id, String property, Object value) {
    	GraphicObject object = allObjects.get(id);
    	Object propertyType = object.getPropertyFromName2(property);
    	object.setPropertyFast2(propertyType, value);
    }

    /**
     * Creates an object
     * @param id object id
     * @param type object type
     * @return the created object's id
     */
    public UID createObject(UID id, GraphicObject.Type type) {
    	GraphicObject object = createTypedObject(type);
    	allObjects.put(id, object);
	
    	return id;
    }


    /**
     * Creates a typed object
     * @param type the object type
     * @return the created typed object
     */
    private GraphicObject createTypedObject(Type type) {
    	GraphicObject createdObject;
    	
		switch (type) {
	
		case ARC:
			System.out.format("ARC created\n");
			createdObject =  new Arc();
			break;
		case AXES:
			System.out.format("AXES created\n");
			createdObject =  new Axes();
			break;
		case AXIS:
			System.out.format("AXIS created\n");
			createdObject =  new Axis();
			break;
		case CHAMP:
			System.out.format("CHAMP created\n");
			createdObject =  new Champ();
			break;
		case COMPOUND:
			System.out.format("COMPOUND created\n");
			createdObject =  new Compound();
			break;
		case FAC3D:
			System.out.format("FAC3D created\n");
			createdObject =  new Fac3d();
			break;
		case FEC:
			System.out.format("FEC created\n");
			createdObject =  new Fec();
			break;
		case FIGURE:
			System.out.format("FIGURE created\n");
			createdObject =  new Figure();
			break;
		case GRAYPLOT:
			System.out.format("GRAYPLOT created\n");
			createdObject =  new Grayplot();
			break;
		case LABEL:
			System.out.format("LABEL created\n");
			createdObject =  new Label();
			break;
		case LEGEND:
			System.out.format("LEGEND created\n");
			createdObject =  new Legend();
			break;
		case MATPLOT:
			System.out.format("MATPLOT created\n");
			createdObject =  new Matplot();
			break;
		case PLOT3D:
			System.out.format("PLOT3D created\n");
			createdObject =  new Plot3d();
			break;
		case POLYLINE:
			System.out.format("POLYLINE created\n");
			createdObject =  new Polyline();
			break;
		case RECTANGLE:
			System.out.format("RECTANGLE created\n");
			createdObject =  new Rectangle();
			break;
		case SEGS:
			System.out.format("SEGS created\n");
			createdObject =  new Segs();
			break;
		case TEXT:
			System.out.format("TEXT created\n");
			createdObject =  new Text();
			break;
		case UNKNOWNOBJECT:
			System.err.format("No object created\n");
			createdObject =  null;
			break;
		default:
			System.err.format("No object created\n");
		    createdObject =  null;
		    break;
		}

		return createdObject;
    }
   
    /**
     * Deletes an object
     * @param id object id
     */
    public void deleteObject(UID id) {
    	allObjects.remove(id);
    	System.err.format("# remaining objects: %d\n", allObjects.size());
    }
    
    
}
