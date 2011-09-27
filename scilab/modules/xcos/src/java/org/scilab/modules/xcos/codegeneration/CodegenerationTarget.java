package org.scilab.modules.xcos.codegeneration;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlType;

@XmlRootElement(name = "codegenerationtarget")
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "CodegenerationTarget", propOrder = { "name", "macroDirectory", "macroName", "description", "options" })
public class CodegenerationTarget {

	@XmlElement(required = true)
	String name;
	
	@XmlElement(required = true)
	String macroDirectory;
	
	@XmlElement(required = true)
	String macroName;
	
	@XmlElement(required = true)
	String description;
	
	@XmlElement(nillable = true)
	Options options;
	
	
	/**
	 * Default constructor
	 */
	public CodegenerationTarget() {
	}
	
	public String getName() {
		return name;
	}
	
	public String getMacroDirectory() {
		return macroDirectory;
	}
	
	public String getMacroName() {
		return macroName;
	}
	
	public String getDescription() {
		return description;
	}
	
	public Options getOptions() {
		return options;
	}
	
	public int getNumberOfOptions() {
		return options.getNumberOfOptions();
	}
	
	public int getNumberOfDirectoryTypeOptions() {
		int j=0;
		for(int i=0; i<options.getNumberOfOptions(); i++) {
			if(options.getSpecifiedOption(i).getType()=="DIRECTORY") j++;
		}
		return j;
	}
	
	public void setName(String argName) {
		name = argName;
	}
	
	public void setDescription(String argDescription) {
		description = argDescription;
	}
}
