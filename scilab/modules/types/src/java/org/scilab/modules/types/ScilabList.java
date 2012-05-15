/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2011-2011 - DIGITEO - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.types;

import java.io.IOException;
import java.io.ObjectInput;
import java.io.ObjectOutput;
import java.util.ArrayList;
import java.util.Collection;

/**
 * This class provides a representation on the Scilab List datatype<br>
 * <br>
 * This class is {@link java.io.Serializable} and any modification could impact
 * load and store of data (Xcos files, Javasci saved data, etc...).<br>
 * <br>
 * Example:<br />
 * <code>
 * ScilabList data = new ScilabList();<br />
 * data.add(new ScilabString("hello"));<br />
 * data.add(new ScilabDouble(2));<br />
 * </code>
 *
 * @see org.scilab.modules.javasci.Scilab
 */
public class ScilabList extends ScilabGenericList {

    private static final long serialVersionUID = 6884293176289980909L;
    private static final ScilabTypeEnum type = ScilabTypeEnum.sci_list;

    /**
     * Construct an empty Scilab list.
     */
    public ScilabList() {
        super();
    }

    /**
     * Construct a list containing the elements of the specified collection, in
     * the order that they are returned by the specified collection's iterator.
     *
     * @param c
     *            the collection whose elements are to be placed into this list.
     */
    public ScilabList(Collection <? extends ScilabType > c) {
        super(c);
    }

    /**
     * Construct an empty mlist.
     *
     * Note that the first element of this collection is the header used by
     * Scilab to find each field name.
     */
    protected ScilabList(String varName) {
        super(varName);
    }

    /**
     * Return the type of Scilab
     *
     * @return the type of Scilab
     * @since 5.4.0
     */
    @Override
    public ScilabTypeEnum getType() {
        return type;
    }



    /**
     * Display the representation in the Scilab language of the type<br />
     * Note that the representation can be copied/pasted straight into Scilab
     *
     * @return the pretty-printed data
     * @see java.util.AbstractCollection#toString()
     */
    @Override
    public String toString() {
        return super.toString("list");
    } 
}
