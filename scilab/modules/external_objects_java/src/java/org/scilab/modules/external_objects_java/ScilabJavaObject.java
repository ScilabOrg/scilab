/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - 2011 - Calixte DENIZET <calixte@contrib.scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.external_objects_java;

import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.DoubleBuffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.nio.LongBuffer;
import java.nio.ShortBuffer;
import java.nio.ByteOrder;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;
import java.io.IOException;
import java.util.Arrays;
import java.util.logging.FileHandler;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.logging.SimpleFormatter;

/**
 * Main class to communicate with Scilab via jni interface generated with Giws.
 * @autor Calixte DENIZET
 */
@SuppressWarnings("serial")
public class ScilabJavaObject {

    private static final int INITIALCAPACITY = 1024;
    private static final Map<Class, Integer> unwrappableType = new HashMap<Class, Integer>(51);
    private static final Map<Class, Integer> listBaseType = new HashMap<Class, Integer>(9);
    private static final Class[] returnType = new Class[1];

    static final Map<Class, Class> primTypes = new HashMap<Class, Class>(8);

    private static int currentPos = 1;
    private static FreePlace freePlace = new FreePlace();

    static boolean debug;
    static Logger logger;

    protected static int currentCapacity = INITIALCAPACITY;
    protected static ScilabJavaObject[] arraySJO = new ScilabJavaObject[currentCapacity];

    static {
        primTypes.put(double.class, Double.class);
        primTypes.put(float.class, Float.class);
        primTypes.put(int.class, Integer.class);
        primTypes.put(short.class, Short.class);
        primTypes.put(byte.class, Byte.class);
        primTypes.put(char.class, Character.class);
        primTypes.put(long.class, Long.class);
        primTypes.put(boolean.class, Boolean.class);

        unwrappableType.put(double.class, 2);
        unwrappableType.put(double[].class, 3);
        unwrappableType.put(double[][].class, 4);
        unwrappableType.put(String.class, 5);
        unwrappableType.put(String[].class, 6);
        unwrappableType.put(String[][].class, 7);
        unwrappableType.put(boolean.class, 8);
        unwrappableType.put(boolean[].class, 9);
        unwrappableType.put(boolean[][].class, 10);
        unwrappableType.put(byte.class, 11);
        unwrappableType.put(byte[].class, 12);
        unwrappableType.put(byte[][].class, 13);
        unwrappableType.put(short.class, 17);
        unwrappableType.put(short[].class, 18);
        unwrappableType.put(short[][].class, 19);
        unwrappableType.put(char.class, 20);
        unwrappableType.put(char[].class, 21);
        unwrappableType.put(char[][].class, 22);
        unwrappableType.put(int.class, 23);
        unwrappableType.put(int[].class, 24);
        unwrappableType.put(int[][].class, 25);
        unwrappableType.put(long.class, 29);
        unwrappableType.put(long[].class, 30);
        unwrappableType.put(long[][].class, 31);
        unwrappableType.put(float.class, 35);
        unwrappableType.put(float[].class, 36);
        unwrappableType.put(float[][].class, 37);
        unwrappableType.put(Double.class, 2);
        unwrappableType.put(Double[].class, 3);
        unwrappableType.put(Double[][].class, 4);
        unwrappableType.put(Integer.class, 23);
        unwrappableType.put(Integer[].class, 24);
        unwrappableType.put(Integer[][].class, 25);
        unwrappableType.put(Long.class, 29);
        unwrappableType.put(Long[].class, 30);
        unwrappableType.put(Long[][].class, 31);
        unwrappableType.put(Byte.class, 11);
        unwrappableType.put(Byte[].class, 12);
        unwrappableType.put(Byte[][].class, 13);
        unwrappableType.put(Character.class, 20);
        unwrappableType.put(Character[].class, 21);
        unwrappableType.put(Character[][].class, 22);
        unwrappableType.put(Boolean.class, 8);
        unwrappableType.put(Boolean[].class, 9);
        unwrappableType.put(Boolean[][].class, 10);
        unwrappableType.put(Float.class, 35);
        unwrappableType.put(Float[].class, 36);
        unwrappableType.put(Float[][].class, 37);
        unwrappableType.put(Short.class, 17);
        unwrappableType.put(Short[].class, 18);
        unwrappableType.put(Short[][].class, 19);

        listBaseType.put(Double.class, 3);
        listBaseType.put(Integer.class, 24);
        listBaseType.put(Long.class, 30);
        listBaseType.put(Byte.class, 12);
        listBaseType.put(Character.class, 21);
        listBaseType.put(Boolean.class, 9);
        listBaseType.put(Float.class, 36);
        listBaseType.put(Short.class, 18);
        listBaseType.put(String.class, 6);

        arraySJO[0] = new ScilabJavaObject(null, null);
    }

    protected Map<String, ScilabJavaMethod> methods;
    protected Object object;
    protected Class clazz;
    protected int id;

    /**
     * Constructor
     * @param obj the Java Object to wrap
     */
    public ScilabJavaObject(Object obj) {
        this(obj, obj == null ? null : obj.getClass());
    }

    /**
     * Constructor
     * @param obj the Java Object to wrap
     * @param clazz the Java Object class
     */
    public ScilabJavaObject(Object obj, Class clazz) {
        this.object = obj;
        this.clazz = clazz;

        if (obj != null) {
            int fp = freePlace.getFreePlace();
            if (fp == -1) {
                this.id = currentPos;
                ++currentPos;
            } else {
                this.id = fp;
            }

            arraySJO[this.id] = this;

            if (debug) {
                logger.log(Level.INFO, "Object creation with id=" + this.id + " and class=" + clazz.toString());
            }

            if (currentPos >= currentCapacity) {
                currentCapacity = currentCapacity * 2;
                ScilabJavaObject[] arr = new ScilabJavaObject[currentCapacity];
                System.arraycopy(arraySJO, 0, arr, 0, currentPos);
                arraySJO = arr;
                if (debug) {
                    logger.log(Level.INFO, "Scope copy");
                }
            }

            this.methods = ScilabJavaMethod.getMethods(clazz);
        } else {
            if (debug) {
                logger.log(Level.INFO, "Object creation with id=0");
            }
            this.id = 0;
        }
    }

    /**
     * Do nothing !
     */
    public static final void initScilabJavaObject() { }

    /**
     * @param filename the log file
     */
    public static final void enableTrace(String filename) throws ScilabJavaException {
        debug = true;
        logger = Logger.getLogger("JIMS");

        try {
            FileHandler fh = new FileHandler(filename, true);
            logger.addHandler(fh);
            logger.setLevel(Level.ALL);
            SimpleFormatter formatter = new SimpleFormatter();
            fh.setFormatter(formatter);
        } catch (SecurityException e) {
            debug = false;
            throw new ScilabJavaException("A security exception has been thrown:\n" + e);
        } catch (IOException e) {
            debug = false;
            throw new ScilabJavaException("I/O problem:\n" + e);
        }
    }

    public static final void writeLog(String s) {
        if (debug) {
            logger.log(Level.INFO, s);
        }
    }

    /**
     * Disable trace
     */
    public static final void disableTrace() {
        if (debug) {
            logger.removeHandler(logger.getHandlers()[0]);
            debug = false;
        }
    }

    /**
     * {@inheritedDoc}
     */
    public String toString() {
        if (object == null) {
            return "null";
        }

        String str = object.toString();
        if (str != null) {
            return str;
        }

        return "Instance of " + object.getClass() + " with hashcode " + System.identityHashCode(object);
    }

    /**
     * Create a new identical reference to a java object
     * @return A deep copy of this {@link ScilabJavaObject}
     */
    @Override
    protected ScilabJavaObject clone() {
        return new ScilabJavaObject(object, clazz);
    }

    /**
     * @param id the Java Object id
     * @return the string to represent this object
     */
    public static final String getRepresentation(final int id) {
        if (arraySJO[id] != null) {
            return arraySJO[id].toString();
        }

        return "Invalid Java object";
    }

    /**
     * @param id the Java Object id
     * @return true if the object is valid
     */
    public static final boolean isValidJavaObject(final int id) {
        return id == 0 || (id > 0 && arraySJO[id] != null);
    }

    /**
     * @param id the Java Object id
     * @param index an array of index
     * @return the id of the element at position given by index in id
     */
    public static final int getArrayElement(final int id, final int[] index) throws ScilabJavaException {
        if (id > 0) {
            if (debug) {
                StringBuffer buf = new StringBuffer();
                buf.append("(");
                if (index.length > 0) {
                    int i = 0;
                    for (; i < index.length - 1; i++) {
                        buf.append(Integer.toString(index[i]));
                        buf.append(",");
                    }
                    buf.append(Integer.toString(index[i]));
                }
                buf.append(")");
                logger.log(Level.INFO, "Get array element: array id=" + id + " at position " + buf.toString());
            }

            if (arraySJO[id] == null) {
                throw new ScilabJavaException("Invalid Java object");
            }

            return new ScilabJavaObject(ScilabJavaArray.get(arraySJO[id].object, index)).id;
        }
        throw new ScilabJavaException("null is not an array");
    }

    /**
     * @param id the Java Object id
     * @param index an array of index
     * @param idArg the id of an element to put at the position given by index
     */
    public static final void setArrayElement(final int id, final int[] index, final int idArg) throws ScilabJavaException {
        if (id > 0) {
            if (debug) {
                StringBuffer buf = new StringBuffer();
                buf.append("(");
                if (index.length > 0) {
                    int i = 0;
                    for (; i < index.length - 1; i++) {
                        buf.append(Integer.toString(index[i]));
                        buf.append(",");
                    }
                    buf.append(Integer.toString(index[i]));
                }
                buf.append(")");
                logger.log(Level.INFO, "Set array element: array id=" + id + " at position " + buf.toString() + " and element id=" + idArg);
            }

            if (arraySJO[id] == null) {
                throw new ScilabJavaException("Invalid Java object");
            }

            ScilabJavaArray.set(arraySJO[id].object, index, arraySJO[idArg].object);
        } else {
            throw new ScilabJavaException("null is not an array");
        }
    }

    /**
     * @param id the Java Object id
     * @return the accessibles methods and fields corresponding to the given path
     */
    public static final String[] getCompletion(final int id, final String[] fieldPath) throws ScilabJavaException {
        if (id > 0) {
            if (debug) {
                logger.log(Level.INFO, "Get accessible methods and fields in object id=" + id + " with path " + Arrays.deepToString(fieldPath));
            }

            if (arraySJO[id] == null) {
                throw new ScilabJavaException("Invalid Java object");
            }

            Class clazz = arraySJO[id].clazz;
            final boolean isClass = arraySJO[id].object == clazz;

            if (fieldPath.length == 0) {
                return getFieldsAndMethods(clazz, isClass);
            }

            if (isClass) {
                // We have a class object
                try {
                    Field f = clazz.getField(fieldPath[0]);
                    int modifiers = f.getModifiers();
                    if (Modifier.isPublic(modifiers) && Modifier.isStatic(modifiers)) {
                        clazz = f.getType();
                    } else {
                        return new String[0];
                    }
                } catch (Exception e) {
                    return new String[0];
                }
            }

            for (int i = (isClass ? 1 : 0); i < fieldPath.length; i++) {
                try {
                    Field f = clazz.getField(fieldPath[i]);
                    if (Modifier.isPublic(f.getModifiers())) {
                        clazz = f.getType();
                    } else {
                        return new String[0];
                    }
                } catch (Exception e) {
                    return new String[0];
                }
            }

            return getFieldsAndMethods(clazz, false);
        } else {
            return new String[0];
        }
    }

    /**
     * Get fields and methods in a Class
     * @param clazz the base class
     * @return names
     */
    private static final String[] getFieldsAndMethods(final Class clazz, final boolean staticOnly) {
        if (clazz.isArray()) {
            return new String[] {"length"};
        }

        try {
            final Field[] fs = clazz.getFields();
            final Method[] ms = clazz.getMethods();

            Set<String> set = new TreeSet<String>();
            for (Field f : fs) {
                final int modifiers = f.getModifiers();
                if (Modifier.isPublic(modifiers) && (!staticOnly || Modifier.isStatic(modifiers))) {
                    set.add(f.getName());
                }
            }

            for (Method m : ms) {
                final int modifiers = m.getModifiers();
                if (Modifier.isPublic(modifiers) && (!staticOnly || Modifier.isStatic(modifiers))) {
                    set.add(m.getName());
                }
            }

            return set.toArray(new String[set.size()]);
        } catch (Exception e) {
            return new String[0];
        }
    }

    /**
     * @param id the Java Object id
     * @return the accessibles methods in the object represented by id
     */
    public static final String[] getAccessibleMethods(final int id) throws ScilabJavaException {
        if (id > 0) {
            if (debug) {
                logger.log(Level.INFO, "Get accessible methods in object id=" + id);
            }

            if (arraySJO[id] == null) {
                throw new ScilabJavaException("Invalid Java object");
            }
            return arraySJO[id].methods.keySet().toArray(new String[0]);
        } else {
            return new String[0];
        }
    }

    /**
     * @param id the Java Object id
     * @return the accessibles fields in the object represented by id
     */
    public static final String[] getAccessibleFields(final int id) throws ScilabJavaException {
        if (id > 0) {
            if (debug) {
                logger.log(Level.INFO, "Get accessible fields in object id=" + id);
            }

            if (arraySJO[id] == null) {
                throw new ScilabJavaException("Invalid Java object");
            }

            if (arraySJO[id].clazz.isArray()) {
                return new String[] {"length"};
            }

            final Field[] f = arraySJO[id].clazz.getFields();
            final String[] sf = new String[f.length];
            for (int i = 0; i < f.length; i++) {
                if (Modifier.isPublic(f[i].getModifiers())) {
                    sf[i] = f[i].getName();
                }
            }
            return sf;
        } else {
            return new String[0];
        }
    }

    /**
     * @param id the Java Object id
     * @return the class name of the object represented by id
     */
    public static final String getClassName(final int id) throws ScilabJavaException {
        if (id > 0) {
            if (debug) {
                logger.log(Level.INFO, "Get class name of object id=" + id);
            }

            if (arraySJO[id] == null) {
                throw new ScilabJavaException("Invalid Java object");
            }

            return arraySJO[id].clazz.getName();
        } else {
            return "null";
        }
    }

    /**
     * @param id the Java Object id
     * @param fieldName the field name to set
     * @param idArg the id of the element to set
     */
    public static final void setField(final int id, final String fieldName, final int idarg) throws ScilabJavaException {
        if (id > 0) {
            Field f = null;
            try {
                if (debug) {
                    logger.log(Level.INFO, "Set field \'" + fieldName + "\' in object id=" + id + " with value id=" + idarg);
                }

                if (arraySJO[id] == null) {
                    throw new ScilabJavaException("Invalid Java object");
                }

                f = arraySJO[id].clazz.getField(fieldName);
                if (Modifier.isPublic(f.getModifiers())) {
                    try {
                        f.set(arraySJO[id].object, arraySJO[idarg].object);
                    } catch (IllegalArgumentException e) {
                        if (f != null && f.getType() == int.class && arraySJO[idarg].clazz == double.class
                                && ((Double) arraySJO[idarg].object).intValue() == ((Double) arraySJO[idarg].object).doubleValue()) {
                            f.set(arraySJO[id].object, ((Double) arraySJO[idarg].object).intValue());
                        } else {
                            throw e;
                        }
                    }
                } else {
                    String setter = "set" + Character.toUpperCase(fieldName.charAt(0)) + fieldName.substring(1);
                    if (arraySJO[id].methods.containsKey(setter)) {
                        arraySJO[id].methods.get(setter).invoke(arraySJO[id].object, returnType, new int[] {idarg});
                    } else {
                        throw new ScilabJavaException("No method " + setter + " in object " + getClassName(id));
                    }
                }
            } catch (NoSuchFieldException e) {
                throw new ScilabJavaException("No field " + fieldName + " in object " + getClassName(id));
            } catch (IllegalArgumentException e) {
                throw new ScilabJavaException("Bad argument value for field " + fieldName + " in object " + getClassName(id));
            } catch (IllegalAccessException e) {
                throw new ScilabJavaException("Cannot access to the field " + fieldName + " in object " + getClassName(id));
            }
        } else {
            throw new ScilabJavaException("null is not an object");
        }
    }

    /**
     * @param id the Java Object id
     * @param fieldName the field name to set
     * @return the id of the got object
     */
    public static final int getField(final int id, final String fieldName) throws ScilabJavaException {
        if (id > 0) {
            try {
                if (debug) {
                    logger.log(Level.INFO, "Get field \'" + fieldName + "\' in object id=" + id);
                }

                if (arraySJO[id] == null) {
                    throw new ScilabJavaException("Invalid Java object");
                }

                if (arraySJO[id].clazz.isArray() && fieldName.equals("length")) {
                    return new ScilabJavaObject(Array.getLength(arraySJO[id].object), int.class).id;
                }

                if (arraySJO[id].object == arraySJO[id].clazz && fieldName.equals("class")) {
                    return new ScilabJavaObject(arraySJO[id].object, arraySJO[id].object.getClass()).id;
                }

                final Field f = arraySJO[id].clazz.getField(fieldName);
                if (Modifier.isPublic(f.getModifiers())) {
                    final Class cl = f.getType();
                    if (cl == int.class) {
                        return new ScilabJavaObject(f.getInt(arraySJO[id].object), int.class).id;
                    } else if (cl == double.class) {
                        return new ScilabJavaObject(f.getDouble(arraySJO[id].object), double.class).id;
                    } else if (cl == boolean.class) {
                        return new ScilabJavaObject(f.getBoolean(arraySJO[id].object), boolean.class).id;
                    } else if (cl == short.class) {
                        return new ScilabJavaObject(f.getShort(arraySJO[id].object), short.class).id;
                    } else if (cl == char.class) {
                        return new ScilabJavaObject(f.getChar(arraySJO[id].object), char.class).id;
                    } else if (cl == float.class) {
                        return new ScilabJavaObject(f.getFloat(arraySJO[id].object), float.class).id;
                    } else if (cl == byte.class) {
                        return new ScilabJavaObject(f.getByte(arraySJO[id].object), byte.class).id;
                    } else if (cl == long.class) {
                        return new ScilabJavaObject(f.getLong(arraySJO[id].object), long.class).id;
                    }

                    return new ScilabJavaObject(f.get(arraySJO[id].object)).id;
                } else {
                    final String fieldStr = Character.toUpperCase(fieldName.charAt(0)) + fieldName.substring(1);
                    final String getter = "get" + fieldStr;
                    final String iser = "is" + fieldStr;

                    if (arraySJO[id].methods.containsKey(getter)) {
                        return new ScilabJavaObject(arraySJO[id].methods.get(getter).invoke(arraySJO[id].object, returnType, new int[] {}), returnType[0]).id;
                    } else if (Boolean.class.equals(f.getClass()) && arraySJO[id].methods.containsKey(iser)) {
                        return new ScilabJavaObject(arraySJO[id].methods.get(iser).invoke(arraySJO[id].object, returnType, new int[] {}), returnType[0]).id;
                    } else {
                        throw new ScilabJavaException("No method " + getter + " in object " + getClassName(id));
                    }
                }
            } catch (NoSuchFieldException e) {
                throw new ScilabJavaException("No field " + fieldName + " in object " + getClassName(id));
            } catch (IllegalArgumentException e) {
                throw new ScilabJavaException("Bad argument value for field " + fieldName + " in object " + getClassName(id));
            } catch (IllegalAccessException e) {
                throw new ScilabJavaException("Cannot access to the field " + fieldName + " in object " + getClassName(id));
            }
        } else {
            throw new ScilabJavaException("null is not an object");
        }
    }

    /**
     * @param id the Java Object id
     * @param fieldName the field name to set
     * @return the type of the field in object represented by id:
     *         * -1 for unknown
     *         * 0 for a method
     *         * 1 for a field
     */
    public static final int getFieldType(final int id, final String fieldName) {
        if (id > 0 && arraySJO[id] != null) {
            if (debug) {
                logger.log(Level.INFO, "Get field type of \'" + fieldName + "\' in object id=" + id);
            }

            if (arraySJO[id].methods.containsKey(fieldName)) {
                return 0;
            }
            try {
                if (arraySJO[id].clazz.isArray()) {
                    if (fieldName.equals("length")) {
                        return 1;
                    } else {
                        return -1;
                    }
                }

                if (arraySJO[id].object == arraySJO[id].clazz && fieldName.equals("class")) {
                    return 1;
                }

                Field f = arraySJO[id].clazz.getField(fieldName);
                if (Modifier.isPublic(f.getModifiers())) {
                    return 1;
                } else {
                    final String fieldStr = Character.toUpperCase(fieldName.charAt(0)) + fieldName.substring(1);
                    final String getter = "get" + fieldStr;
                    final String iser = "is" + fieldStr;
                    final String setter = "set" + fieldStr;
                    if (arraySJO[id].methods.containsKey(setter) && (arraySJO[id].methods.containsKey(getter) || Boolean.class.equals(f.getClass()) && arraySJO[id].methods.containsKey(iser))) {
                        return 1;
                    } else {
                        return -1;
                    }
                }
            } catch (NoSuchFieldException e) {
                return -1;
            } catch (IllegalArgumentException e) {
                return -1;
            }
        }

        return -1;
    }

    /**
     * @param id the Java Object id
     * @param methName the method name to invoke
     * @param args an array containing the id of the arguments
     * @return the id of the invocation result
     */
    public static final int invoke(final int id, final String methName, final int[] args) throws ScilabJavaException {
        if (id > 0) {
            if (debug) {
                StringBuffer buf = new StringBuffer();
                buf.append("(");
                if (args.length > 0) {
                    int i = 0;
                    for (; i < args.length - 1; i++) {
                        buf.append(Integer.toString(args[i]));
                        buf.append(",");
                    }
                    buf.append(Integer.toString(args[i]));
                }
                buf.append(")");
                logger.log(Level.INFO, "Invoke method \'" + methName + "\' in object id=" + id + " with arguments id=" + buf.toString());
            }

            if (arraySJO[id] != null) {
                Object ret = arraySJO[id].methods.get(methName).invoke(arraySJO[id].object, returnType, args);
                if (ret == null && returnType[0] == Void.TYPE) {
                    return -1;
                } else {
                    return new ScilabJavaObject(ret, returnType[0]).id;
                }
            }
            throw new ScilabJavaException("Invalid Java object");
        } else {
            throw new ScilabJavaException("null is not an object");
        }
    }

    /**
     * @param id the Java Object id
     * @param args an array containing the id of the arguments
     * @return the id of the invocation result
     */
    public static final int extract(final int id, final int[] args) throws ScilabJavaException {
        if (id > 0) {
            if (debug) {
                StringBuffer buf = new StringBuffer();
                buf.append("(");
                if (args.length > 0) {
                    int i = 0;
                    for (; i < args.length - 1; i++) {
                        buf.append(Integer.toString(args[i]));
                        buf.append(",");
                    }
                    buf.append(Integer.toString(args[i]));
                }
                buf.append(")");
                logger.log(Level.INFO, "Extract in object id=" + id + " with arguments id=" + buf.toString());
            }

            for (int i = 0; i < args.length; i++) {
                if (args[i] < 0 || arraySJO[args[i]] == null) {
                    throw new ScilabJavaException("Invalid Java object at position " + i);
                }
            }

            if (arraySJO[id] != null) {
                Object o = arraySJO[id].object;
                for (int i = 0; i < args.length; i++) {
                    Object a = args[i] == 0 ? null : arraySJO[args[i]].object;
                    if (o instanceof Map) {
                        o = ((Map) o).get(a);
                    } else if (o instanceof List) {
                        List l = (List) o;
                        int pos = -1;
                        if (a instanceof Double) {
                            pos = ((Double) a).intValue();
                        } else if (a instanceof Integer) {
                            pos = ((Integer) a).intValue();
                        } else {
                            pos = l.indexOf(a);
                        }
                        if (pos >= 0 || pos < l.size()) {
                            o = l.get(pos);
                        } else {
                            throw new ScilabJavaException("Cannot get object at position " + (i + 1));
                        }
                    } else if (o.getClass().isArray()) {
                        int pos = -1;
                        if (a instanceof Double) {
                            pos = ((Double) a).intValue();
                        } else if (a instanceof Integer) {
                            pos = ((Integer) a).intValue();
                        }

                        o = ScilabJavaArray.get(o, new int[] {pos});
                    } else {
                        throw new ScilabJavaException("Invalid field " + (a == null ? "null" : a.toString()));
                    }
                }

                if (o == null) {
                    return 0;
                }

                return new ScilabJavaObject(o).id;
            }
            throw new ScilabJavaException("Invalid Java object");
        } else {
            throw new ScilabJavaException("null is not an object");
        }
    }

    /**
     * @param id the Java Object id
     * @param className the target class name
     * @return the id of the cast result
     */
    public static final int javaCast(final int id, final String className) throws ScilabJavaException {
        if (id > 0) {
            if (debug) {
                logger.log(Level.INFO, "Cast object id=" + id + " to class " + className);
            }

            final int idC = ScilabClassLoader.loadJavaClass(className, false);
            final Class clazz = (Class) arraySJO[idC].object;
            try {
                int idR = new ScilabJavaObject(clazz.cast(arraySJO[id].object), clazz).id;
                removeScilabJavaObject(idC);
                return idR;
            } catch (ClassCastException e) {
                throw new ScilabJavaException("Cannot cast object " + getClassName(id) + " into " + getClassName(idC));
            }
        } else {
            throw new ScilabJavaException("null is not an object");
        }
    }

    /**
     * @param id the Java Object id
     * @param classId the target class id
     * @return the id of the cast result
     */
    public static final int javaCast(final int id, final int classId) throws ScilabJavaException {
        if (id > 0) {
            if (debug) {
                logger.log(Level.INFO, "Cast object id=" + id + " to class with id=" + classId);
            }

            final Class clazz = (Class) arraySJO[classId].object;
            try {
                return new ScilabJavaObject(clazz.cast(arraySJO[id].object), clazz).id;
            } catch (ClassCastException e) {
                throw new ScilabJavaException("Cannot cast object " + getClassName(id) + " into " + getClassName(classId));
            }
        } else {
            throw new ScilabJavaException("null is not an object");
        }
    }

    /**
     * Remove an object.
     * @param id the Java Object id
     */
    public static final void removeScilabJavaObject(final int id) {
        if (id > 0 && arraySJO[id] != null) {
            if (debug) {
                logger.log(Level.INFO, "Remove object id=" + id + " (" + java.util.Arrays.deepToString(new Object[] {freePlace.getCurrentFreePlace()})  + ")");
            }
            freePlace.addFreePlace(id);
            if (arraySJO[id] instanceof ScilabJavaClass) {
                ScilabClassLoader.removeID(id);
            }
            arraySJO[id].methods = null;
            arraySJO[id] = null;
        }
    }

    /**
     * Set the limit of a direct buffer to 0 to make it unusable.
     * @param id the Java Object id
     */
    public static final void limitDirectBuffer(final int id) {
        if (id > 0 && arraySJO[id] != null && (arraySJO[id].object instanceof Buffer)) {
            if (debug) {
                logger.log(Level.INFO, "Limit direct buffer with id=" + id);
            }

            ((Buffer) arraySJO[id].object).limit(0);
        }
    }

    /**
     * Remove all the objects and start a garbage collection
     */
    public static final void garbageCollect() {
        if (debug) {
            logger.log(Level.INFO, "Garbage collection");
        }
        currentPos = 1;
        currentCapacity = INITIALCAPACITY;
        arraySJO = new ScilabJavaObject[currentCapacity];
        arraySJO[0] = new ScilabJavaObject(null, null);
        freePlace = new FreePlace();
        ScilabClassLoader.clazz.clear();
        System.gc();
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrapAsDirectByteBuffer(final ByteBuffer x) {
        x.order(ByteOrder.nativeOrder());
        return new ScilabJavaObject(x, ByteBuffer.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrapAsDirectDoubleBuffer(final ByteBuffer x) {
        x.order(ByteOrder.nativeOrder());
        return new ScilabJavaObject(x.asDoubleBuffer(), DoubleBuffer.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrapAsDirectIntBuffer(final ByteBuffer x) {
        x.order(ByteOrder.nativeOrder());
        return new ScilabJavaObject(x.asIntBuffer(), IntBuffer.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrapAsDirectCharBuffer(final ByteBuffer x) {
        x.order(ByteOrder.nativeOrder());
        return new ScilabJavaObject(x.asCharBuffer(), CharBuffer.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrapAsDirectFloatBuffer(final ByteBuffer x) {
        x.order(ByteOrder.nativeOrder());
        return new ScilabJavaObject(x.asFloatBuffer(), FloatBuffer.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrapAsDirectLongBuffer(final ByteBuffer x) {
        x.order(ByteOrder.nativeOrder());
        return new ScilabJavaObject(x.asLongBuffer(), LongBuffer.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrapAsDirectShortBuffer(final ByteBuffer x) {
        x.order(ByteOrder.nativeOrder());
        return new ScilabJavaObject(x.asShortBuffer(), ShortBuffer.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final double x) {
        return new ScilabJavaObject(x, double.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final double[] x) {
        return new ScilabJavaObject(x, double[].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final double[][] x) {
        return new ScilabJavaObject(x, double[][].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final int x) {
        return new ScilabJavaObject(x, int.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final int[] x) {
        return new ScilabJavaObject(x, int[].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final int[][] x) {
        return new ScilabJavaObject(x, int[][].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final long x) {
        return new ScilabJavaObject(x, long.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final long[] x) {
        return new ScilabJavaObject(x, long[].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final long[][] x) {
        return new ScilabJavaObject(x, long[][].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final byte x) {
        return new ScilabJavaObject(x, byte.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final byte[] x) {
        return new ScilabJavaObject(x, byte[].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final byte[][] x) {
        return new ScilabJavaObject(x, byte[][].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final short x) {
        return new ScilabJavaObject(x, short.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final short[] x) {
        return new ScilabJavaObject(x, short[].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final short[][] x) {
        return new ScilabJavaObject(x, short[][].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final String x) {
        return new ScilabJavaObject(x, String.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final String[] x) {
        return new ScilabJavaObject(x, String[].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final String[][] x) {
        return new ScilabJavaObject(x, String[][].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final boolean x) {
        return new ScilabJavaObject(x, boolean.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final boolean[] x) {
        return new ScilabJavaObject(x, boolean[].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final boolean[][] x) {
        return new ScilabJavaObject(x, boolean[][].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final char x) {
        return new ScilabJavaObject(x, char.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final char[] x) {
        return new ScilabJavaObject(x, char[].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final char[][] x) {
        return new ScilabJavaObject(x, char[][].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final float x) {
        return new ScilabJavaObject(x, float.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final float[] x) {
        return new ScilabJavaObject(x, float[].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static final int wrap(final float[][] x) {
        return new ScilabJavaObject(x, float[][].class).id;
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final double unwrapDouble(final int id) {
        return ((Double) (arraySJO[id].object)).doubleValue();
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final double[] unwrapRowDouble(final int id) {
        if (arraySJO[id].object instanceof List) {
            return ScilabJavaArray.toDoubleArray((List<Double>) arraySJO[id].object);
        } else if (arraySJO[id].object instanceof Double[]) {
            return ScilabJavaArray.toPrimitive((Double[]) arraySJO[id].object);
        }

        return (double[]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final double[][] unwrapMatDouble(final int id) {
        if (arraySJO[id].object instanceof Double[][]) {
            Object o = ScilabJavaArray.toPrimitive(arraySJO[id].object);
            return (double[][]) ScilabJavaArray.toPrimitive(arraySJO[id].object);
        }

        return (double[][]) arraySJO[id].object;
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final int unwrapInt(final int id) {
        return ((Integer) (arraySJO[id].object)).intValue();
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final int[] unwrapRowInt(final int id) {
        if (arraySJO[id].object instanceof List) {
            return ScilabJavaArray.toIntArray((List<Integer>) arraySJO[id].object);
        } else if (arraySJO[id].object instanceof Integer[]) {
            return ScilabJavaArray.toPrimitive((Integer[]) arraySJO[id].object);
        }

        return (int[]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final int[][] unwrapMatInt(final int id) {
        if (arraySJO[id].object instanceof Integer[][]) {
            return (int[][]) ScilabJavaArray.toPrimitive(arraySJO[id].object);
        }

        return (int[][]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final short unwrapShort(final int id) {
        return ((Short) (arraySJO[id].object)).shortValue();
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final short[] unwrapRowShort(final int id) {
        if (arraySJO[id].object instanceof List) {
            return ScilabJavaArray.toShortArray((List<Short>) arraySJO[id].object);
        } else if (arraySJO[id].object instanceof Short[]) {
            return ScilabJavaArray.toPrimitive((Short[]) arraySJO[id].object);
        }

        return (short[]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final short[][] unwrapMatShort(final int id) {
        if (arraySJO[id].object instanceof Short[][]) {
            return (short[][]) ScilabJavaArray.toPrimitive(arraySJO[id].object);
        }

        return (short[][]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final byte unwrapByte(final int id) {
        return ((Byte) (arraySJO[id].object)).byteValue();
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final byte[] unwrapRowByte(final int id) {
        if (arraySJO[id].object instanceof List) {
            return ScilabJavaArray.toByteArray((List<Byte>) arraySJO[id].object);
        } else if (arraySJO[id].object instanceof Byte[]) {
            return ScilabJavaArray.toPrimitive((Byte[]) arraySJO[id].object);
        }

        return (byte[]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final byte[][] unwrapMatByte(final int id) {
        if (arraySJO[id].object instanceof Byte[][]) {
            return (byte[][]) ScilabJavaArray.toPrimitive(arraySJO[id].object);
        }

        return (byte[][]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final String unwrapString(final int id) {
        return (String) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final String[] unwrapRowString(final int id) {
        if (arraySJO[id].object instanceof List) {
            return ScilabJavaArray.toStringArray((List<String>) arraySJO[id].object);
        }

        return (String[]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final String[][] unwrapMatString(final int id) {
        return (String[][]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final boolean unwrapBoolean(final int id) {
        return (Boolean) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final boolean[] unwrapRowBoolean(final int id) {
        if (arraySJO[id].object instanceof List) {
            return ScilabJavaArray.toBooleanArray((List<Boolean>) arraySJO[id].object);
        } else if (arraySJO[id].object instanceof Boolean[]) {
            return ScilabJavaArray.toPrimitive((Boolean[]) arraySJO[id].object);
        }

        return (boolean[]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final boolean[][] unwrapMatBoolean(final int id) {
        if (arraySJO[id].object instanceof Boolean[][]) {
            return (boolean[][]) ScilabJavaArray.toPrimitive(arraySJO[id].object);
        }

        return (boolean[][]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final char unwrapChar(final int id) {
        return (Character) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final char[] unwrapRowChar(final int id) {
        if (arraySJO[id].object instanceof List) {
            return ScilabJavaArray.toCharArray((List<Character>) arraySJO[id].object);
        } else if (arraySJO[id].object instanceof Character[]) {
            return ScilabJavaArray.toPrimitive((Character[]) arraySJO[id].object);
        }

        return (char[]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final char[][] unwrapMatChar(final int id) {
        if (arraySJO[id].object instanceof Character[][]) {
            return (char[][]) ScilabJavaArray.toPrimitive(arraySJO[id].object);
        }

        return (char[][]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final float unwrapFloat(final int id) {
        return (Float) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final float[] unwrapRowFloat(final int id) {
        if (arraySJO[id].object instanceof List) {
            return ScilabJavaArray.toFloatArray((List<Float>) arraySJO[id].object);
        } else if (arraySJO[id].object instanceof Double[]) {
            return ScilabJavaArray.toPrimitive((Float[]) arraySJO[id].object);
        }

        return (float[]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final float[][] unwrapMatFloat(final int id) {
        if (arraySJO[id].object instanceof Float[][]) {
            return (float[][]) ScilabJavaArray.toPrimitive(arraySJO[id].object);
        }

        return (float[][]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final long unwrapLong(final int id) {
        return (Long) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final long[] unwrapRowLong(final int id) {
        if (arraySJO[id].object instanceof List) {
            return ScilabJavaArray.toLongArray((List<Long>) arraySJO[id].object);
        } else if (arraySJO[id].object instanceof Long[]) {
            return ScilabJavaArray.toPrimitive((Long[]) arraySJO[id].object);
        }

        return (long[]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static final long[][] unwrapMatLong(final int id) {
        if (arraySJO[id].object instanceof Long[][]) {
            return (long[][]) ScilabJavaArray.toPrimitive(arraySJO[id].object);
        }

        return (long[][]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the identifier of the unwrappable type or -1 if nothing
     */
    public static final int isUnwrappable(final int id) {
        Integer t = unwrappableType.get(arraySJO[id].clazz);
        if (t != null) {
            return t;
        } else if (arraySJO[id].object instanceof List) {
            List l = (List) arraySJO[id].object;
            if (l.size() == 0 || l.get(0) == null) {
                return -1;
            } else {
                Class cl = l.get(0).getClass();
                Integer i = listBaseType.get(cl);
                if (i != null) {
                    for (Object o : l) {
                        if (o.getClass() != cl) {
                            return -1;
                        }
                    }
                }

                if (i == null) {
                    return -1;
                }

                return i;
            }
        } else {
            return -1;
        }
    }

    /**
     * Inner class to manage the free places
     */
    private static final class FreePlace {

        int[] fp;
        int currentPos;

        FreePlace() {
            this.fp = new int[INITIALCAPACITY];
            currentPos = -1;
        }

        final void addFreePlace(final int n) {
            if (currentPos == fp.length - 1) {
                int[] newFp = new int[(int) (1.5 * fp.length)];
                System.arraycopy(fp, 0, newFp, 0, fp.length);
                fp = newFp;
            }
            fp[++currentPos] = n;
        }

        final int getFreePlace() {
            if (currentPos == -1) {
                return -1;
            }
            return fp[currentPos--];
        }

        final int[] getCurrentFreePlace() {
            if (currentPos == -1) {
                return null;
            }

            int[] ret = new int[currentPos + 1];
            System.arraycopy(fp, 0, ret, 0, ret.length);

            return ret;
        }
    }
}
