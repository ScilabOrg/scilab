/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Sylvestre LEDRU
 *  Copyright (C) 2012 - Scilab Enterprises - Sylvestre LEDRU
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.types;

import org.scilab.modules.localization.Messages;

public class ScilabTypeEnumDescription {

    public static String getTypeDescriptionFromId(int type) {
        try {
            switch (ScilabTypeEnum.swigToEnum(type)) {
                case sci_matrix:
                    return Messages.gettext("Double");
                case sci_poly:
                    return Messages.gettext("Polynomial");
                case sci_boolean:
                    return Messages.gettext("Boolean");
                case sci_sparse:
                    return Messages.gettext("Sparse");
                case sci_boolean_sparse:
                    return Messages.gettext("Boolean Sparse");
                case sci_matlab_sparse:
                    return Messages.gettext("Matlab Sparse");
                case sci_ints:
                    return Messages.gettext("Integer");
                case sci_handles:
                    return Messages.gettext("Graphic handle");
                case sci_strings:
                    return Messages.gettext("String");
                case sci_u_function:
                    return Messages.gettext("User function");
                case sci_c_function:
                    return Messages.gettext("Compiled function");
                case sci_lib:
                    return Messages.gettext("Function library");
                case sci_list:
                    return Messages.gettext("List");
                case sci_tlist:
                    return Messages.gettext("Tlist");
                case sci_mlist:
                    return Messages.gettext("Mlist");
                case sci_pointer:
                    return Messages.gettext("Pointer");
                case sci_implicit_poly:
                    return Messages.gettext("Implicit polynomial");
                case sci_intrinsic_function:
                    return Messages.gettext("Intrinsic function");
            }
            return Messages.gettext("Unknown datatype");

        } catch (IllegalArgumentException e) {
            return Messages.gettext("Unknown datatype");
        }
    }

    /*
     * Return the description of the mlist code ('ce' => 'cell', 'st => 'struct', etc)
     * @param shortCode the short code
     * @return the full description
     */
    public static String getListTypeDescription (String shortCode) {
        // Once we switch to Java 7, we can replace that by a String switch
        if (shortCode.equals("ce")) {
            shortCode = "cell";
        } else {
            if (shortCode.equals("st")) {
                shortCode = "struct";
            } else {
                if (shortCode.equals("fptr")) {
                    shortCode = "built-in";
                }
            }
        }
        return shortCode;
    }
}