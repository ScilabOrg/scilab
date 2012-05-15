/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012-2012 - Scilab Enterprises - Bruno JOFRET
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

public abstract class ScilabGenericList extends ScilabType {

    protected ArrayList<ScilabType> data;
    private static final int VERSION = 0;
    
    public ScilabGenericList() {
        super();
        data = new ArrayList<ScilabType>();
    }
    
    public ScilabGenericList(String varName) {
        super(varName, false);
        data = new ArrayList<ScilabType>();
    }
    
    public ScilabGenericList(Collection<? extends ScilabType> c) {
        data = new ArrayList<ScilabType>(c);
    }
    
    public ScilabGenericList(int i) {
        data = new ArrayList<ScilabType>(i);
    }

    public boolean isEmpty() {
        return data.isEmpty();
    }
    
    public boolean equals(Object obj) {
        return data.equals(obj);
    }
    
    protected ArrayList<ScilabType> getData() {
        return data;
    }
    
    /**
     * @return 1 when there is data on the list, 0 otherwise.
     * @see org.scilab.modules.types.ScilabType#getWidth()
     */
    @Override
    public int getWidth() {
        if (isEmpty()) {
            return 0;
        }
        return data.size();
    }
    
    /**
     * @return 1 when there is data on the list, 0 otherwise.
     * @see org.scilab.modules.types.ScilabType#getHeight()
     */
    public int getHeight() {
        if (isEmpty()) {
            return 0;
        }
        return 1;
    }

    /**
     * Get a serialized list; The format is the following: i) returned[0] is an
     * array of integers containing the Scilab type (ScilabTypeEunm) of the
     * different elements of the list. ii) returned[i] for i&gt;=1 contains the
     * serialized form of each items.
     *
     * @return a serialized SiclabList/
     */
    public Object[] getSerializedObject() {
        int[] types = new int[data.size()];
        Object[] items = new Object[types.length + 1];

        for (int i = 0; i < types.length; i++) {
            ScilabType var = data.get(i);
            types[i] = var.getType().swigValue();
            items[i + 1] = var.getSerializedObject();
        }
        items[0] = types;

        return items;
    }

    @Override
    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {
        int version = in.readInt();
        switch (version) {
            case 0:
                int size = in.readInt();
                data.ensureCapacity(size + 1);
                ArrayList list = (ArrayList) data;
                for (int i = 0; i < size; i++) {
                    list.add(in.readObject());
                }
                setVarName((String) in.readObject());
                break;
            default:
                throw new ClassNotFoundException("A class ScilabList with a version " + version + " does not exists");
        }
    }

    @Override
    public void writeExternal(ObjectOutput out) throws IOException {
        out.writeInt(VERSION);
        out.writeInt(data.size());
        for (Object var : (ArrayList<ScilabType>) data) {
            out.writeObject(var);
        }
        out.writeObject(getVarName());
    }
    
    /**
     * Display the representation in the Scilab language of the type<br />
     * Note that the representation can be copied/pasted straight into Scilab
     *
     * @return the pretty-printed data
     * @see java.util.AbstractCollection#toString()
     */
    protected String toString(String scilabListType) {
        StringBuffer result = new StringBuffer();
        if (isEmpty()) {
            result.append(scilabListType+"()");
            return result.toString();
        }

        result.append(scilabListType+"(");
        for (int i = 0; i < data.size(); i++) {
            result.append(data.get(i));
            if (i != data.size() - 1) {
                result.append(", ");
            }
        }
        result.append(")");

        return result.toString();
    }

    protected void add(ScilabType scilabValue) {
        data.add(scilabValue);
    }

    public void addAll(Collection<? extends ScilabType> c) {
       data.addAll(c);
    }
}
