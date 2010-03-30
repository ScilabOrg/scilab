/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

package org.scilab.modules.hdf5.scilabTypes;

/**
 * This class provides a wrapping on the Scilab String datatype
 */
public class ScilabString implements ScilabType {

	private String[][] data;

    public ScilabString() {
		data = null;
    }
    
    public ScilabString(String[][] data) {
		this.data = data;
    }
    
    public ScilabString(String[] data) {
		if(data == null || data.length == 0) {
			this.data = new String[1][1];
			this.data[0][0] = "";
		} else {
			this.data = new String[1][data.length];
			for (int i = 0 ; i < data.length ; i++){
				this.data[0][i] = data[i] ;
			}
		}
    }
    
    public ScilabString(String string) {
		if(string == null) {
			System.err.println("string == null");
		}
		this.data = new String[1][1];
		this.data[0][0] = string;
    }

    public void setData(String[][] data) {
		this.data = data;
    }

    public String[][] getData() {
		return data;
    }
    
    public int getHeight() {
		if (data == null) {
			return 0;
		}
		return data.length;
	}

	public int getWidth() {
		if (data == null) {
			return 0;
		}
		return data[0].length;
	}
    
    public boolean isEmpty(){
    	return (data == null);
    }

	public String toString() {
		StringBuffer result = new StringBuffer();
		if (isEmpty()) {
			result.append("[]");
			return result.toString();
		}

		result.append("[");
		for (int i = 0 ; i < getHeight() ; ++i) {
			for (int j = 0 ; j < getWidth() ; ++j) {

				result.append('"');
				result.append(getData()[i][j]);
				result.append('"');
				
				if (j != getWidth() - 1) {
					result.append(", ");
				}
			}
			if (i != getHeight() - 1) {
				result.append(" ; ");
			}
		}
		result.append("]");
		return result.toString();
	}
}
