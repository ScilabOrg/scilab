package org.scilab.modules.xcos.codegeneration;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlEnum;
import javax.xml.bind.annotation.XmlType;
import javax.xml.bind.annotation.XmlValue;


@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name="Option")
public class Option {
	
	@XmlValue
	private String name;
	
	@XmlAttribute
	private Type type;
	
	//TODO: find a way to feed modelspecific default values to this attribute
	//private String value;
	
	/**
	 * Default constructor
	 */
	public Option() {
	}
	
	@XmlEnum(String.class)
	public enum Type {
		DIRECTORY, TEXT
	}

	public String getName() {
		return name;
	}
	
	public String getType() {
		return type.name();
	}
	
	public String getValue() {
		//return value;
		return "";
	}
	
	public void setValue(String argValue) {
		//value = argValue;
	}
}
