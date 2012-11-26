/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.uiwidget;

import java.lang.annotation.Annotation;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Set;
import java.util.TreeSet;

import javax.swing.JPopupMenu;
import javax.swing.SwingUtilities;

import org.scilab.modules.types.ScilabType;
import org.scilab.modules.uiwidget.components.TextData;
import org.scilab.modules.uiwidget.components.UIKeymap;

public final class UIAccessTools {

    public static void add(final UIComponent parent, final UIComponent uicomp) throws UIWidgetException {
        final Object comp = uicomp.getComponent();
        if (uicomp instanceof UIListener) {
            parent.addListener((UIListener) uicomp);
            uicomp.updateDependencies(parent);
        } else if (comp instanceof JPopupMenu) {
            parent.addPopupMenu((JPopupMenu) comp);
            uicomp.updateDependencies(parent);
        } else if (uicomp instanceof TextData) {
            String text = ((TextData) uicomp).getText();
            parent.setProperty("text", text);
            uicomp.updateDependencies(parent);
        } else if (uicomp instanceof UIKeymap) {
            ((UIKeymap) uicomp).update();
            uicomp.updateDependencies(parent);
        } else {
            Object base = parent;
            Method method = method = UIMethodFinder.findAdder(base.getClass(), comp.getClass());

            if (method == null) {
                base = parent.getComponent();
                method = UIMethodFinder.findAdder(base.getClass(), comp.getClass());
            }

            if (method == null) {
                throw new UIWidgetException("Cannot add a " + comp.getClass().getName() + " to a " + parent.getClass().getName());
            }

            final Method m = method;
            final Object b = base;

            execOnEDT(new Runnable() {
                public void run() {
                    try {
                        invokeAdder(m, b, comp);
                        uicomp.updateDependencies(parent);
                    } catch (Exception e) {
                        System.err.println(e);
                    }
                }
            });
        }
    }

    private static void invokeAdder(final Method m, final Object base, final Object obj) throws UIWidgetException {
        try {
            m.invoke(base, obj);
        } catch (Exception e) {
            System.err.println(e);
        }
    }

    public static void setPropertyViaReflection(final Object obj, final String name, final String value) throws UIWidgetException {
        Class clazz = obj.getClass();
        String methName = getSetterName(name);
        final Method method = UIMethodFinder.findSetter(methName, clazz);

        if (method == null) {
            throw new UIWidgetException("Cannot set the attribute " + name + ": No corresponding method found");
        }

        execOnEDT(new Runnable() {
            public void run() {
                try {
                    UIAccessTools.invokeSetter(method, obj, value);
                } catch (Exception e) {
                    System.err.println(e);
                }
            }
        });
    }

    public static void setPropertyViaReflection(final Object obj, final String name, final ScilabType value) throws UIWidgetException {
        Class clazz = obj.getClass();
        String methName = getSetterName(name);
        final Method method = UIMethodFinder.findSetter(methName, clazz);

        if (method == null) {
            throw new UIWidgetException("Cannot set the attribute " + name + ": No corresponding method found");
        }

        UIAccessTools.execOnEDT(new Runnable() {
            public void run() {
                try {
                    invokeSetter(method, obj, value);
                } catch (Exception e) {
                    System.err.println(e);
                }
            }
        });
    }

    public static void invokeSetter(final Method m, final Object obj, final String value) throws UIWidgetException {
        try {
            m.invoke(obj, StringConverters.getObjectFromValue(m.getParameterTypes()[0], value));
        } catch (IllegalAccessException e) {
            // Should not occur
            System.err.println(e);
        } catch (ExceptionInInitializerError e) {
            // Should not occur
            System.err.println(e);
        } catch (IllegalArgumentException e) {
            // Should not occur
            System.err.println(e);
        } catch (NullPointerException e) {
            // Should not occur
            System.err.println(e);
        } catch (InvocationTargetException e) {
            System.err.println(e.getCause());
            e.getCause().printStackTrace();
            throw new UIWidgetException("An exception has been thrown in calling the method " + m.getName() + " in class " + obj.getClass().getName() + ":\n" + e.getCause());
        }
    }

    public static void invokeSetter(final Method m, final Object obj, final ScilabType value) throws UIWidgetException {
        try {
            m.invoke(obj, ScilabTypeConverters.getObjectFromValue(m.getParameterTypes()[0], value));
        } catch (IllegalAccessException e) {
            // Should not occur
            System.err.println(e);
        } catch (ExceptionInInitializerError e) {
            // Should not occur
            System.err.println(e);
        } catch (IllegalArgumentException e) {
            // Should not occur
            System.err.println(e);
        } catch (NullPointerException e) {
            // Should not occur
            System.err.println(e);
        } catch (InvocationTargetException e) {
            //System.err.println(e.getCause());
            //e.getCause().printStackTrace();
            throw new UIWidgetException("An exception has been thrown in calling the method " + m.getName() + " in class " + obj.getClass().getName() + ":\n" + e.getCause());
        }
    }

    public static Object getPropertyViaReflection(final Object obj, final String name) throws UIWidgetException {
        Class clazz = obj.getClass();
        String methName = getGetterName(name);
        Method method = UIMethodFinder.findGetter(methName, clazz);
        if (method == null) {
            methName = UIAccessTools.getIsGetterName(name);
            method = UIMethodFinder.findGetter(methName, clazz);
        }

        if (method == null) {
            throw new UIWidgetException("Cannot get the attribute " + name + ": No corresponding method found");
        }

        if (SwingUtilities.isEventDispatchThread()) {
            return invokeGetter(method, obj);
        } else {
            final Method m = method;
            final Object[] ptr = new Object[1];

            try {
                SwingUtilities.invokeAndWait(new Runnable() {
                    public void run() {
                        try {
                            ptr[0] = invokeGetter(m, obj);
                        } catch (Exception e) {
                            System.err.println(e);
                        }
                    }
                });
            } catch (InterruptedException e) {
                throw new UIWidgetException("Error in getting property " + name);
            } catch (InvocationTargetException e) {
                throw new UIWidgetException("Error in getting property " + name + ":\n" + e.getMessage());
            }

            return ptr[0];
        }
    }

    public static Object invokeGetter(final Method m, final Object obj) throws UIWidgetException {
        try {
            return m.invoke(obj);
        } catch (IllegalAccessException e) {
            // Should not occur
            System.err.println(e);
        } catch (ExceptionInInitializerError e) {
            // Should not occur
            System.err.println(e);
        } catch (IllegalArgumentException e) {
            // Should not occur
            System.err.println(e);
        } catch (NullPointerException e) {
            // Should not occur
            System.err.println(e);
        } catch (InvocationTargetException e) {
            //System.err.println(e.getCause());
            //e.getCause().printStackTrace();
            throw new UIWidgetException("An exception has been thrown in calling the method " + m.getName() + " in class " + obj.getClass().getName() + ":\n" + e.getCause());
        }

        return null;
    }

    public static Set<String> createNewInstance(final UIComponent uicomp,  final ConvertableMap attributes) throws UIWidgetException {
        final Set<String> uselessAttrs = new TreeSet<String>(attributes.keySet());
        Method method = UIMethodFinder.findNewer(uicomp.getClass());
        Annotation annotation = null;
        Object[] args = null;

        if (method == null) {
            try {
                method = uicomp.getClass().getMethod("newInstance");
            } catch (NoSuchMethodException e) { }

            if (!method.isAccessible()) {
                method.setAccessible(true);
            }
        } else {
            annotation = method.getAnnotation(UIComponentAnnotation.class);
        }

        if (annotation != null) {
            String[] argsName = ((UIComponentAnnotation) annotation).attributes();
            Class[] argsType = method.getParameterTypes();

            if (argsName.length != argsType.length) {
                throw new UIWidgetException("Invalid annotation: the number of attributes must be the same as the number of arguments");
            }

            args = new Object[argsType.length];

            for (int i = 0; i < args.length; i++) {
                args[i] = attributes.get(argsType[i], argsName[i]);
                uselessAttrs.remove(argsName[i]);
            }
        } else {
            args = new Object[0];
        }

        final Object[] fargs = args;
        final Method m = method;

        if (SwingUtilities.isEventDispatchThread()) {
            try {
                uicomp.setComponent(m.invoke(uicomp, fargs));
            } catch (Exception e) {
                System.err.println(e);
            }
        } else {
            try {
                SwingUtilities.invokeAndWait(new Runnable() {
                    public void run() {
                        try {
                            uicomp.setComponent(m.invoke(uicomp, fargs));
                        } catch (Exception e) {
                            System.err.println(e);
                        }
                    }
                });
            } catch (Exception e) {
                System.err.println(e);
            }
        }

        return uselessAttrs;
    }

    public static void execOnEDT(final Runnable runnable) {
        if (SwingUtilities.isEventDispatchThread()) {
            runnable.run();
        } else {
            SwingUtilities.invokeLater(runnable);
        }
    }

    public static String getSetterName(final String key) throws UIWidgetException {
        return getMethodName("set", key);
    }

    public static String getGetterName(final String key) throws UIWidgetException {
        return getMethodName("get", key);
    }

    public static String getIsGetterName(final String key) throws UIWidgetException {
        return getMethodName("is", key);
    }

    public static String getMethodName(final String prefix, final String key) throws UIWidgetException {
        if (key == null || key.isEmpty()) {
            throw new UIWidgetException("Invalid attribute name: cannot be empty");
        }
        StringBuilder buffer = new StringBuilder(32);
        char[] chars = key.toCharArray();

        buffer.append(prefix).append(Character.toUpperCase(chars[0]));
        for (int i = 1; i < chars.length; i++) {
            if (chars[i] == '-') {
                buffer.append(Character.toUpperCase(chars[i + 1]));
                i++;
            } else {
                buffer.append(chars[i]);
            }
        }

        return buffer.toString();
    }
}
