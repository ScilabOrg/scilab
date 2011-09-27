package org.scilab.modules.xcos.codegeneration;

import java.util.List;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Options", propOrder = { "option" })
public class Options {

	@XmlElement(nillable = true)
	private List<Option> option;
	
	
	/**
	 * Default constructor
	 */
	public Options() {
	}
	
	/**
	 * Get Option on specified list index location
	 * @param i
	 * @return Selected Option
	 */
	public Option getSpecifiedOption(int i) {
		return option.get(i);
	}
	
	public int getNumberOfOptions() {
		return option.size();
	}

}
