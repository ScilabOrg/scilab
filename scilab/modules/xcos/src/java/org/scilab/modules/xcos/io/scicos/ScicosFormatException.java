/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.io.scicos;




/**
 * Default exception for a Xcos - Scicos communication
 */
public abstract class ScicosFormatException extends Exception {
	private final String field;
	
	/**
	 * Used when the {@link Element} cannot be used to decode/encode the instance.
	 */
	public static class WrongElementException extends ScicosFormatException {
		
		/**
		 * Default constructor
		 */
		public WrongElementException() {
			super();
			
			printStackTrace();
		}
	}
	
	/**
	 * Used when the expected data cannot be casted safely.
	 */
	public static class WrongTypeException extends ScicosFormatException {
		
		private final Class expected;
		private final Class getting;
		
		/**
		 * Default constructor
		 * 
		 * @param expected the expected class type
		 * @param getting the serialized klass type
		 */
		public WrongTypeException(Class expected, Class getting) {
			super();
			
			this.expected = expected;
			this.getting = getting;
			
			printStackTrace();
		}
		
		/**
		 * @return the error message
		 * @see java.lang.Throwable#getMessage()
		 */
		@Override
		public String getMessage() {
			return "Expecting " + expected + " but getting " + getting;
		}
	}
	
	/**
	 * Used when the expected data are not well formatted.
	 */
	public static class WrongStructureException extends ScicosFormatException {
		/**
		 * Default constructor
		 */
		public WrongStructureException() {
			super();
			
			printStackTrace();
		}
	}
	
	/**
	 * Used when the data doesn't have a valid version number.
	 */
	public static class VersionMismatchException extends ScicosFormatException {
		private final String wrongVersion;
		
		/**
		 * Default constructor
		 * @param wrongVersion the wrong version number.
		 */
		public VersionMismatchException(String wrongVersion) {
			this.wrongVersion = wrongVersion;
		}
		
		/**
		 * @return the wrong version number
		 */
		public String getWrongVersion() {
			return wrongVersion;
		}
	}
	
	/**
	 * Default constructor
	 */
	protected ScicosFormatException() {
		this.field = null;
	}
	
	/**
	 * Default constructor
	 * @param field path to the erroneous field
	 */
	public ScicosFormatException(String field) {
		this.field = field;
		
		printStackTrace();
	}

	/**
	 * @return the erroneous field
	 */
	public String getField() {
		return field;
	}
}
