/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - 2015 - Clement DAVID <clement.david@scilab-enterprises.com>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.modules.external_objects_java;

import java.lang.reflect.Constructor;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabType;

/**
 * Converters to manage org.scilab.modules.types.*
 */
public class ScilabJavaType {

    private static final Map<Class<?>, Class<? extends ScilabType>> PRIMITIVES_MAP;

    static {
        PRIMITIVES_MAP = new HashMap<>();
        PRIMITIVES_MAP.put(double.class, ScilabDouble.class);
        PRIMITIVES_MAP.put(Double.class, ScilabDouble.class);
        PRIMITIVES_MAP.put(long.class, ScilabInteger.class);
        PRIMITIVES_MAP.put(Long.class, ScilabInteger.class);
        PRIMITIVES_MAP.put(int.class, ScilabInteger.class);
        PRIMITIVES_MAP.put(Integer.class, ScilabInteger.class);
        PRIMITIVES_MAP.put(short.class, ScilabInteger.class);
        PRIMITIVES_MAP.put(Short.class, ScilabInteger.class);
        PRIMITIVES_MAP.put(byte.class, ScilabInteger.class);
        PRIMITIVES_MAP.put(Byte.class, ScilabInteger.class);
        PRIMITIVES_MAP.put(boolean.class, ScilabBoolean.class);
        PRIMITIVES_MAP.put(Boolean.class, ScilabBoolean.class);
        // ScilabString is handled on the code as a String object is not a primitive
    }

    private static Class<? extends ScilabType> lookForScilabClass(Class<?> from) {
        Class<? extends ScilabType> scilabClass;

        // try to detect vector or matrices
        Class base = ScilabJavaArray.getArrayBaseType(from);
        scilabClass = PRIMITIVES_MAP.get(base);

        // try to detect a String, a String array or multi-array
        if (scilabClass == null && String.class == base) {
            scilabClass = ScilabString.class;
        }

        // try to detect a buffer
        if (scilabClass == null) {
            base = ScilabJavaArray.bufferMappings.get(from);
            scilabClass = PRIMITIVES_MAP.get(base);
        }

        // try to map to list / mlist / tlist
        if (scilabClass == null) {
            boolean isMap = Map.class.isAssignableFrom(from);
            boolean isList = List.class.isAssignableFrom(from);

            // FIXME handle TList and MList
            if (isMap) {
                // scilabClass = ScilabMList.class;
                scilabClass = null;
            } else if (isList) {
                scilabClass = ScilabList.class;
            } else {
                scilabClass = null;
            }
        }
        return scilabClass;
    }

    /*
     * convert a Java native array to a ScilabType
     */

    public static boolean canConvertTo(Class<?> from, Class<? extends ScilabType> to) {
        Class<? extends ScilabType> scilabClass = lookForScilabClass(from);

        if (scilabClass == null) {
            return false;
        }
        return to.isAssignableFrom(scilabClass);
    }

    public static Object convertTo(Object original, Class<? extends ScilabType> to) {
        Class<? extends ScilabType> scilabClass = lookForScilabClass(original.getClass());

        if (List.class.isAssignableFrom(original.getClass())) {
            // recursively convert is necessary
            List list = (List) original;

            List<ScilabType> convertedValues = new ArrayList<ScilabType>(list.size());
            for (Object e : list) {
                Object value;
                if (e instanceof ScilabJavaObject) {
                    value = ((ScilabJavaObject) e).object;
                } else {
                    value = e;
                }
                convertedValues.add((ScilabType) FunctionArguments.convert(value, ScilabType.class));
            }

            if (scilabClass == ScilabList.class) {
                return new ScilabList(convertedValues);
            } else {
                // FIXME manage TList and MList
                return null;
            }
        } else if (ScilabString.class == scilabClass) {
            // manual management of String
            Object converted = original;
            Class valueClass = original.getClass();

            // increase dimension of a vector
            if (valueClass.isArray() && !valueClass.getComponentType().isArray()) {
                valueClass = String[][].class;
                converted = ScilabJavaArray.toBiDim(converted);
            }

            try {
                Constructor<? extends ScilabType> cstr = scilabClass.getConstructor(valueClass);
                return cstr.newInstance(converted);
            } catch (ReflectiveOperationException e) {
                // hmm something goes wrong, ignore it
            }

        } else {
            // this is a matrix or vector default-ing to the constructor
            Object converted = ScilabJavaArray.toPrimitive(original);

            // if this is a native boxed type, look for the un-boxed constructor
            Class valueClass = converted.getClass();
            if (!valueClass.isArray()) {
                valueClass = ScilabJavaArray.mappings.get(valueClass);
            } else if (!valueClass.getComponentType().isArray()) {
                converted = ScilabJavaArray.toBiDim(converted);
                valueClass = converted.getClass();
            }

            try {
                Constructor<? extends ScilabType> cstr = scilabClass.getConstructor(valueClass);
                return cstr.newInstance(converted);
            } catch (ReflectiveOperationException e) {
                // hmm something goes wrong, ignore it
            }
        }

        return null;
    }

    /*
     * convert a ScilabType to a Java native array
     */

    public static boolean canConvertFrom(Class<? extends ScilabType> from, Class<?> to) {
        // firstly, try to detect vector or matrices
        Class<?> validPrimitive = null;
        for (Map.Entry<Class<?>, Class<? extends ScilabType>> e : PRIMITIVES_MAP.entrySet()) {
            if (e.getValue() == from) {
                validPrimitive = e.getKey();
                break;
            }
        }

        if (validPrimitive == null) {
            boolean isValidVector = to.isArray() && PRIMITIVES_MAP.containsKey(to.getComponentType());
            boolean isValidMatrix = to.isArray() && to.getComponentType().isArray() && PRIMITIVES_MAP.containsKey(to.getComponentType().getComponentType());

            return isValidVector || isValidMatrix;
        }

        // secondly, detect the mlist / tlist / list case
        boolean isScilabType = from.equals(ScilabList.class) || from.equals(ScilabTList.class) || from.equals(ScilabMList.class);
        if (isScilabType) {
            // FIXME handle this case
            return false;
        }

        return false;
    }

    public static Object convertFrom(ScilabType original, Class<?> to) {
        // firstly, handle to detect vector or matrices
        boolean isValidVector = to.isArray() && PRIMITIVES_MAP.containsKey(to.getComponentType());
        boolean isValidMatrix = to.isArray() && to.getComponentType().isArray() && PRIMITIVES_MAP.containsKey(to.getComponentType().getComponentType());

        if (isValidVector) {
            Class<?> nativeClass = to.getComponentType();
            if (original instanceof ScilabDouble) {
                ScilabDouble scilabValue = (ScilabDouble) original;
                double[] data = new double[scilabValue.getHeight() * scilabValue.getWidth()];
                for (int i = 0; i < scilabValue.getHeight(); i++)
                    for (int j = 0; j < scilabValue.getWidth(); j++) {
                        data[i * scilabValue.getWidth() + j] = scilabValue.getRealElement(i, j);
                    }
                return data;
            }
        }

        return null;
    }

}
