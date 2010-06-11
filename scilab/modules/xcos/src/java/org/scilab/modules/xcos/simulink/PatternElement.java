/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Jerzy Zagorski
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.simulink;
import java.util.Iterator;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBElement;
import javax.xml.bind.JAXBException;
import javax.xml.bind.UnmarshalException;
import javax.xml.bind.Unmarshaller;
import javax.xml.transform.stream.StreamSource;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.jvm.utils.ScilabConstants;
import org.scilab.modules.xcos.simulink.patterns.Block;
import org.scilab.modules.xcos.simulink.patterns.BlockPallete;
import org.scilab.modules.xcos.simulink.patterns.Int2StrParameter;
import org.scilab.modules.xcos.simulink.patterns.IntegerParameter;
import org.scilab.modules.xcos.simulink.patterns.SimpleParameter;
import org.scilab.modules.xcos.simulink.patterns.Str2IntParameter;
import org.scilab.modules.xcos.simulink.patterns.Str2IntValueMap;
import org.scilab.modules.xcos.simulink.patterns.StringParameter;

public class PatternElement {
	private static final Log LOG = LogFactory.getLog(PatternElement.class);
	JAXBContext jc;
	Unmarshaller u;
	BlockPallete blocks;
	
	public PatternElement(){
	try{
		JAXBContext jc = JAXBContext.newInstance( "org.scilab.modules.xcos.simulink.patterns" );
		Unmarshaller u = jc.createUnmarshaller();
		//FIXME: now you have to manually copy compatibility pattern to SCIHOME
		JAXBElement<BlockPallete> element = u.unmarshal(
				new StreamSource(ScilabConstants.SCIHOME + "/simulinkImportBlocks.xml"),
				BlockPallete.class );
		blocks = element.getValue();
	} catch( UnmarshalException ue ) {
		LogFactory.getLog(PatternElement.class).error(ue);
	} catch( JAXBException je ) {
		LogFactory.getLog(PatternElement.class).error(je);
	}
	}
	
	/**
	 * Function without target functionality yet
	 * @param String paramName;
	 * @return respondend Xcos parameterName
	 */
	public String decode(String simulinkBlockName, String paramName, String simulinkValue){
		try{
			Iterator<Block> blockIter = blocks.getBlock().iterator();
			while (blockIter.hasNext()){
				Block block = blockIter.next();
				if(block.getSim().contentEquals(simulinkBlockName)){
					/*
					 * checking simple parameters
					 */
					Iterator<SimpleParameter> simpleParamIter = block.getSimple().iterator();
					while (simpleParamIter.hasNext()){
						SimpleParameter simpleParam = simpleParamIter.next();
						if(simpleParam.getSim().contentEquals(paramName)){
							return simpleParam.getXCos();
						}
					}
					/*
					 * checking integer parameters
					 */
					Iterator<IntegerParameter> integerParamIter = block.getInteger().iterator();
					while (integerParamIter.hasNext()){
						IntegerParameter integerParam = integerParamIter.next();
						if(integerParam.getSim().contentEquals(paramName)){
							return integerParam.getXCos();
						}
					}
					/*
					 * checking string parameters
					 */
					Iterator<StringParameter> stringParamIter = block.getString().iterator();
					while (stringParamIter.hasNext()){
						StringParameter stringParam = stringParamIter.next();
						if(stringParam.getSim().contentEquals(paramName)){
							return stringParam.getXCos();
						}
					}
					/*
					 * checking str2int parameters
					 */
					Iterator<Str2IntParameter> str2intParamIter = block.getStr2Int().iterator();
					while (str2intParamIter.hasNext()){
						Str2IntParameter str2intParam = str2intParamIter.next();
						if(str2intParam.getSim().contentEquals(paramName)){
							String returnValue = str2intParam.getXCos() + ": ";
							Iterator<Str2IntValueMap> valueMapIter = str2intParam.getParMap().iterator();
							while (valueMapIter.hasNext()){
								Str2IntValueMap valueMap = valueMapIter.next();
								if(valueMap.getSimVal().contentEquals(simulinkValue)){
									returnValue +=  valueMap.getXCosVal();
								}
							}
							return returnValue;
						}
					}
					/*
					 * checking int2str parameters
					 */
					Iterator<Int2StrParameter> int2strParamIter = block.getInt2Str().iterator();
					while (int2strParamIter.hasNext()){
						Int2StrParameter int2strParam = int2strParamIter.next();
						if(int2strParam.getSim().contentEquals(paramName)){
							return int2strParam.getXCos();
						}
					}
					return ".";
				}
			}
		} catch(Exception e1) {
			LogFactory.getLog(PatternElement.class).error(e1);
		}
		return "";
	}
	
	/**
	 * Functions printPattern is created for debugging compatibility patterns
	 */
	public void printPattern() {
		if (LOG.isTraceEnabled()) {
			try{
				Iterator<Block> blockIter = blocks.getBlock().iterator();
				while (blockIter.hasNext()){
					Block block = blockIter.next();
					if(LOG.isTraceEnabled()){
						LOG.trace("From: " + block.getSim() + 
								" to: " + block.getXCos() + 
								" with: " + block.getDsc());
					}
					/*
					 * printing simple parameters
					 */
					Iterator<SimpleParameter> simpleParamIter = block.getSimple().iterator();
					while (simpleParamIter.hasNext()){
						SimpleParameter simpleParam = simpleParamIter.next();
						if(LOG.isTraceEnabled()){
							LOG.trace("  From: " + simpleParam.getSim() + 
									" to: " + simpleParam.getXCos() + 
									" with: " + simpleParam.getDsc());
						}
					}
					/*
					 * printing integer parameters
					 */
					Iterator<IntegerParameter> integerParamIter = block.getInteger().iterator();
					while (integerParamIter.hasNext()){
						IntegerParameter integerParam = integerParamIter.next();
						if(LOG.isTraceEnabled()){
							LOG.trace("  From: " + integerParam.getSim() + 
									" to: " + integerParam.getXCos() + 
									" with: " + integerParam.getDsc());
						}
					}
					/*
					 * printing string parameters
					 */
					Iterator<StringParameter> stringParamIter = block.getString().iterator();
					while (stringParamIter.hasNext()){
						StringParameter stringParam = stringParamIter.next();
						if(LOG.isTraceEnabled()){
							LOG.trace("  From: " + stringParam.getSim() + 
									" to: " + stringParam.getXCos() + 
									" with: " + stringParam.getDsc());
						}
					}
					/*
					 * printing str2int parameters
					 */
					Iterator<Str2IntParameter> str2intParamIter = block.getStr2Int().iterator();
					while (str2intParamIter.hasNext()){
						Str2IntParameter str2intParam = str2intParamIter.next();
						if(LOG.isTraceEnabled()){
							LOG.trace("  From: " + str2intParam.getSim() + 
									" to: " + str2intParam.getXCos() + 
									" with: " + str2intParam.getDsc());
						}
					}
					/*
					 * printing int2str parameters
					 */
					Iterator<Int2StrParameter> int2strParamIter = block.getInt2Str().iterator();
					while (int2strParamIter.hasNext()){
						Int2StrParameter int2strParam = int2strParamIter.next();
						if(LOG.isTraceEnabled()){
							LOG.trace("  From: " + int2strParam.getSim() + 
									" to: " + int2strParam.getXCos() + 
									" with: " + int2strParam.getDsc());
						}
					}
				}
			} catch(Exception e1) {
				LogFactory.getLog(PatternElement.class).error(e1);
			}
		}
	}
}
