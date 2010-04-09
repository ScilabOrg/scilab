package org.scilab.modules.graphic_objects.axes;

import java.util.ArrayList;

import org.scilab.modules.graphic_objects.textObject.FormattedText;

/**
 * TicksProperty class
 * @author juliachs
 */
public class TicksProperty {
	/** TicksProperty properties names */
	public enum TicksPropertyProperty { AUTO, LOCATIONS, LABELS, SUBTICKS };
	
	/** Default number of ticks */
	private static final int DEFAULT_NUMBER_OF_TICKS = 20;
	
	/** Specifies whether ticks are automatically computed or not */
	private boolean auto;

	/** Ticks locations along their axis */
	private double[] locations;

	/** Ticks labels */
	private ArrayList <FormattedText> labels;

	/** Number of subticks between two main ticks */
	private int subticks;

	/** Constructor */
	public TicksProperty() {
		auto = false;
		locations = new double[DEFAULT_NUMBER_OF_TICKS];
		labels = new ArrayList<FormattedText>(DEFAULT_NUMBER_OF_TICKS);
		subticks = 0;
	}

	/**
	 * @return the auto
	 */
	public Boolean getAuto() {
		return new Boolean(auto);
	}

	/**
	 * @param auto the auto to set
	 */
	public void setAuto(Boolean auto) {
		this.auto = auto.booleanValue();
	}

	/**
	 * @return the labels
	 */
	public ArrayList<FormattedText> getLabels() {
		return labels;
	}

	/**
	 * @param labels the labels to set
	 */
	public void setLabels(ArrayList<FormattedText> labels) {
		this.labels = labels;
	}

	/**
	 * @return the locations
	 */
	public Double[] getLocations() {
		Double[] retLocations = new Double[locations.length];
		for (int i = 0; i < locations.length; i++) {
			retLocations[i] = new Double(locations[i]);
		}

		return retLocations;
	}

	/**
	 * @param locations the locations to set
	 */
	public void setLocations(Double[] locations) {
		for (int i = 0; i < locations.length; i++) {
			this.locations[i] = locations[i].doubleValue();
		}
	}

	/**
	 * @return the subticks
	 */
	public Integer getSubticks() {
		return new Integer(subticks);
	}

	/**
	 * @param subticks the subticks to set
	 */
	public void setSubticks(Integer subticks) {
		this.subticks = subticks.intValue();
	}

	//	 Methods: to be done
}
