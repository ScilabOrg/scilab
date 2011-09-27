package org.scilab.modules.xcos.codegeneration;

import java.io.File;

import javax.xml.XMLConstants;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.UnmarshalException;
import javax.xml.bind.Unmarshaller;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.xcos.configuration.ConfigurationManager;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.xml.sax.SAXException;

/**
 * 
 * @author maarten
 * Datastorage for CodegenerationTarget. Is responsible for loading the XML CodegenerationTargets.
 * 
 */
public class CodegenerationTargets {

	private CodegenerationTarget[] targets;
	
	/**
	 * Default constructor
	 * Initializes all CodegenerationTargets found on the system
	 */
	public CodegenerationTargets() {
		targets = initializeCodegenerationTargets();
	}

	
	/**
	 * Get all Code generation targets found in SCI/contrib/codegeneration directory
	 * 
	 * @return Returns CodeGenerationTarget array  
	 */
	public CodegenerationTarget[] initializeCodegenerationTargets(){
		CodegenerationTarget[] targetsFound;
		
		//initialize Folder-searching variables
	    File folder = new File(ScilabConstants.SCI.getAbsoluteFile() + XcosConstants.XCOS_CODEGENERATION);
	    File[] listOfFiles = folder.listFiles();
	    File xmlTargetDescription;
	    
	    //initialize Marshal variables
	    Schema schema;
	    Unmarshaller unmarshaller;
	    JAXBContext jaxbContext;
		String schemaPath = ScilabConstants.SCI.getAbsolutePath() + XcosConstants.XCOS_ETC + 
				File.separator+ "CodeGenerationTarget.xsd";
		
		//setup unmarshaller for org.scilab.modules.xcos.codegeneration
		try {
			jaxbContext = JAXBContext.newInstance("org.scilab.modules.xcos.codegeneration");
			unmarshaller = jaxbContext.createUnmarshaller();
		} catch (JAXBException ex) {
			LogFactory.getLog(ConfigurationManager.class).error(
					"base configuration file corrupted.\n"
					+ ex);
			
			//return an empty array
			return new CodegenerationTarget[0];
		}
		
		//Validate CodeGeneration.xsd
		try {
			schema = SchemaFactory.newInstance(
			XMLConstants.W3C_XML_SCHEMA_NS_URI).newSchema(
			new File(schemaPath));
			unmarshaller.setSchema(schema);
		} catch (SAXException e) {
			LogFactory.getLog(ConfigurationManager.class).error(
			"Unable to validate " + schemaPath + e);
			
			//return an empty array
			return new CodegenerationTarget[0];
		}
		
	    //Scan the contrib/codegeneration directory for code generation targets
		//Index i loops through all directories found in the codegeneration directory
		//Index j registers the existing XML files
		
		//Initialize the targetsFound array with the absolute upperbound
		targetsFound = new CodegenerationTarget[listOfFiles.length];
		
	    int j=0;
	    for(int i=0; i<listOfFiles.length; i++ ) {
	      
	      //load xmlfile
		  xmlTargetDescription = new File(ScilabConstants.SCI.getAbsoluteFile() + XcosConstants.XCOS_CODEGENERATION
					+ File.separator + listOfFiles[i].getName() + File.separator 
					+ listOfFiles[i].getName() + ".xml");
		  
		  //Check of directory and xml-file exist
	      if (listOfFiles[i].isDirectory() && xmlTargetDescription.exists() ) {
	    	  
	  	    //Unmarshal the XML-codegeneration target file to targets array
			try {
				//Load xml-description of target to java object
				targetsFound[j] = (CodegenerationTarget) unmarshaller.unmarshal(xmlTargetDescription);
				j++;
			} catch(UnmarshalException e) {	
				//If the target is invalid, then effectively don't increment the index
				j--;
			} catch (JAXBException e) {
				e.printStackTrace();
			} 
	      }
	    }
	    
	    //Only return the valid targets, which are the first j items within targetsFound
	    CodegenerationTarget[] validTargets = new CodegenerationTarget[j];
	    for (int i=0; i<j; i++) validTargets[i] = targetsFound[i];
	    
	    return validTargets;
	}
	
	public CodegenerationTarget getCodegenerationTarget(int index) {
		return targets[index];
	}
	
	public int getNumberOfTargets() {
		return targets.length;
	}
	
}
