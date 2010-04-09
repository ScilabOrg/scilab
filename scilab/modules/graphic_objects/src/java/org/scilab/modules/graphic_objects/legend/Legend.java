package org.scilab.modules.graphic_objects.legend;

import java.util.ArrayList;

import org.scilab.modules.graphic_objects.polyline.Polyline;
import org.scilab.modules.graphic_objects.textObject.ClippableTextObject;

/**
 * Legend class
 * @author juliachs
 */
public class Legend extends ClippableTextObject {
	/** Legend properties names */
	private enum LegendProperty { LINKS, LEGENDLOCATION, POSITION };
	
	/** Legend location */
	private enum LegendLocation { IN_UPPER_RIGHT, IN_UPPER_LEFT, IN_LOWER_RIGHT, IN_LOWER_LEFT,
		OUT_UPPER_RIGHT, OUT_UPPER_LEFT, OUT_LOWER_RIGHT, OUT_LOWER_LEFT,
		UPPER_CAPTION, LOWER_CAPTION, BY_COORDINATES };
	
	/** List of the polylines referred to */
	private ArrayList <Polyline> links;

	/** Legend location */
	private LegendLocation legendLocation;

	/** 2D position relative to the parent axes bounds */
	private double[] position;

	/** Constructor */
	public Legend() {
		super();
		this.links = null;
		this.legendLocation = null;
		position = new double[2];
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp;

		if (propertyName.equals("Links")) {
			returnedProp =  LegendProperty.LINKS;
		} else if (propertyName.equals("LegendLocation")) {
			returnedProp =  LegendProperty.LEGENDLOCATION;
		} else if (propertyName.equals("Position")) {
			returnedProp =  LegendProperty.POSITION;
		} else {
			returnedProp = super.getPropertyFromName(propertyName);
		}
		
		return (Object) returnedProp;
	}

	/**
	 * Fast property get method
	 * @param property the property to get
	 * @return the property value
	 */
	public Object getPropertyFast(Object property) {
		Object returnedProp;
	
		if (property == LegendProperty.LINKS) {
			returnedProp = getLinks();
		} else if (property == LegendProperty.LEGENDLOCATION) {
			returnedProp = getLegendLocation();
		} else if (property == LegendProperty.POSITION) {
			returnedProp = getPosition();
		} else {
			returnedProp = super.getPropertyFast(property);	
		}

		return returnedProp;
	}

	/**
	 * Fast property set method
	 * @param property the property to set
	 * @param value the property value
	 */
	public void setPropertyFast(Object property, Object value) {

		if (property == LegendProperty.LINKS) {
			setLinks((ArrayList<Polyline>) value);
		} else if (property == LegendProperty.LEGENDLOCATION) {
			setLegendLocation((LegendLocation) value);
		} else if (property == LegendProperty.POSITION) {
			setPosition((Double[]) value);
		} else {
			super.setPropertyFast(property, value);
		}
	}

	/**
	 * @return the legendLocation
	 */
	public LegendLocation getLegendLocation() {
		return legendLocation;
	}

	/**
	 * @param legendLocation the legendLocation to set
	 */
	public void setLegendLocation(LegendLocation legendLocation) {
		this.legendLocation = legendLocation;
	}

	/**
	 * @return the links
	 */
	public ArrayList<Polyline> getLinks() {
		return links;
	}

	/**
	 * @param links the links to set
	 */
	public void setLinks(ArrayList<Polyline> links) {
		this.links = links;
	}

	/**
	 * @return the position
	 */
	public Double[] getPosition() {
		Double[] retPosition = new Double[2];
		retPosition[0] = new Double(position[0]);
		retPosition[1] = new Double(position[1]);

		return retPosition;
	}

	/**
	 * @param position the position to set
	 */
	public void setPosition(Double[] position) {
		this.position[0] = position[0].doubleValue();
		this.position[1] = position[1].doubleValue();
	}
	
	//	 Methods: to be done
}
