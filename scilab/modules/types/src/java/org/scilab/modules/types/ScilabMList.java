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

import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

/**
 * This class provides a representation on the Scilab MList datatype<br>
 * <br>
 * This class is {@link java.io.Serializable} and any modification could impact
 * load and store of data (Xcos files, Javasci saved data, etc...).<br>
 * <br>
 * Example:<br />
 * <code>
 * ScilabMList data = new ScilabMList();<br />
 * data.add(new ScilabString("hello"));<br />
 * data.add(new ScilabDouble(2));<br />
 * </code>
 *
 * @see org.scilab.modules.javasci.Scilab
 */
public class ScilabMList extends ScilabGenericList {

    private static final long serialVersionUID = 3224510024213901841L;
    private static final ScilabTypeEnum type = ScilabTypeEnum.sci_mlist;

    /**
     * Construct an empty mlist.
     *
     * Note that the first element of this collection is the header used by
     * Scilab to find each field name.
     */
    public ScilabMList() {
        super();
    }

    /**
     * Construct an empty mlist.
     *
     * Note that the first element of this collection is the header used by
     * Scilab to find each field name.
     */
    protected ScilabMList(String varName) {
        super(varName);
    }

    /**
     * Construct an mlist with a specified header.
     *
     * @param types
     *            field names used by the accessors.
     */
    public ScilabMList(String[] types) {
        super();
        String[][] typesData = new String[1][types.length];
        typesData[0] = types;
        super.add(new ScilabString(typesData));
    }

    /**
     * Construct a tlist containing the elements of the specified collection, in
     * the order that they are returned by the specified collection's iterator.
     *
     * @param names
     *            field names, used by the accessors.
     * @param c
     *            the collection whose elements are to be placed into this
     *            mlist.
     */
    public ScilabMList(String[] names, Collection <? extends ScilabType > c) {
        super(c.size() + 1);

        String[][] namesData = new String[1][names.length];
        namesData[0] = names;
        super.add(new ScilabString(namesData));

        super.addAll(c);
    }

    /**
     * Get a map between the fields name and the associated ScilabType objects
     *
     * @return the map
     */
    public Map<String, ScilabType> getMListFields() {
        Map<String, ScilabType> map = new HashMap<String, ScilabType>();
        if (isEmpty()) {
            return map;
        }

        ScilabString mat = (ScilabString) data.get(0);
        if (mat.isEmpty()) {
            return map;
        }

        String[] fields = mat.getData()[0];
        for (int i = 1; i < fields.length; i++) {
            if (i < data.size()) {
                map.put(fields[i], data.get(i));
            } else {
                map.put(fields[i], null);
            }
        }

        return map;
    }

    /**
     * Get the mlist type or null if none
     *
     * @return the mlist type
     */
    public String getMListType() {
        if (isEmpty()) {
            return null;
        }

        ScilabString mat = (ScilabString) data.get(0);
        if (mat.isEmpty()) {
            return null;
        }

        return mat.getData()[0][0];
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
        return super.toString("mlist");
    }
}
