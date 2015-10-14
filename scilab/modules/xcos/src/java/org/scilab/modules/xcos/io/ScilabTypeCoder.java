/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.io;

import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.VectorOfDouble;

/**
 * Encode and decode using a var2vec / vec2var compatible encoding.
 *
 * <p>
 * This encoder is used to store arbitrary data to the model. Usually some properties are hard to map to both Java and C++ STL type system, using a shared encoding let us provide an implementation
 * whatever the language is without sharing too low-level information.
 */
public class ScilabTypeCoder {

    public ScilabTypeCoder() {
    }

    /*
     * var2vec implementation
     */
    public VectorOfDouble var2vec(ScilabType var) {
        VectorOfDouble vec = new VectorOfDouble();

        switch (var.getType()) {
            case sci_boolean:
                break;
            case sci_boolean_sparse:
                break;
            case sci_ints:
                break;
            case sci_list:
                break;
            case sci_matrix:
                break;
            case sci_mlist:
                break;
            case sci_poly:
                break;
            case sci_strings:
                break;
            case sci_tlist:
                break;
            default:
                break;
        }

        return vec;
    }

    /*
     * vec2var implementation
     */

    public ScilabType vec2var(VectorOfDouble vec) {
        return new ScilabDouble();
    }
}
