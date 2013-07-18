/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.helptools;

import java.io.IOException;

import org.xml.sax.SAXException;

public class PDFConverter extends ContainerConverter {

    public PDFConverter(SciDocMain sciDocMain) {
        super(sciDocMain.getOutputDirectory(), sciDocMain.getLanguage());
        // TODO Auto-generated constructor stub
    }

    @Override
    public void convert() throws SAXException, IOException {
        // TODO Auto-generated method stub

    }

    @Override
    public void install() throws IOException {
        // TODO Auto-generated method stub

    }

}
