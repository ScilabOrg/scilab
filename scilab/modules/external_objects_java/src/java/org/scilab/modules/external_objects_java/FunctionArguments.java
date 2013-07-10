/*
 * JIMS ( http://forge.scilab.org/index.php/p/JIMS/ ) - This file is a part of JIMS
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

import java.util.ArrayList;
import java.util.List;
import java.lang.reflect.Method;
import java.lang.reflect.Constructor;

/**
 * A constructor wrapper
 *
 * @author Calixte DENIZET
 */
public class FunctionArguments {

    /**
     * Find a valid method. An argument can be null (thanks to Fabien Viale).
     * @param name the method name
     * @param functions the set of the valid functions in the class
     * @param argsClass the class of the arguments
     * @param args the arguments
     * @return a valid method wrapped in an array or a method and the modified args (variable args) wrapped in an array
     */
    public static final Object[] findMethod(String name, Method[] functions, Class[] argsClass, Object[] args) throws NoSuchMethodException {
        String internName = name.intern();
        Method better = null;
        boolean mustConv = false;
        boolean isVarArgs = false;
        long sqd = Long.MAX_VALUE;
        boolean[] refBools = new boolean[2];
        for (Method f : functions) {
            if (f.getName() == internName) {
                Class[] types = f.getParameterTypes();
                refBools[0] = false;
                refBools[1] = false;
                long d = compareClassArgs(types, argsClass, args, refBools);
                if (d != Long.MIN_VALUE && d < sqd) {
                    sqd = d;
                    better = f;
                    mustConv = refBools[0];
                    isVarArgs = refBools[1];
                }
                if (d == 0) {
                    break;
                }
            }
        }

        if (better != null) {
            if (mustConv) {
                Class[] types = better.getParameterTypes();
                for (int i = 0; i < types.length; i++) {
                    if (types[i] == int.class && argsClass[i] == double.class) {
                        argsClass[i] = int.class;
                        args[i] = ((Double) args[i]).intValue();
                    }
                }
            }
            if (isVarArgs) {
                Class[] types = better.getParameterTypes();
                Object[] o = new Object[args.length - types.length + 1];
                System.arraycopy(args, types.length - 1, o, 0, args.length - types.length + 1);

                Object[] newArgs = new Object[types.length];
                System.arraycopy(args, 0, newArgs, 0, types.length - 1);
                newArgs[types.length - 1] = o;

                return new Object[] {better, newArgs};
            }

            return new Object[] {better};
        }

        throw new NoSuchMethodException("");
    }

    /**
     * Find a valid method. An argument can be null (thanks to Fabien Viale).
     * @param functions the set of the valid constructors in the class
     * @param argsClass the class of the arguments
     * @param args the arguments
     * @return a valid method wrapped in an array or a method and the modified args (variable args) wrapped in an array
     */
    public static final Object[] findConstructor(Constructor[] functions, Class[] argsClass, Object[] args) throws NoSuchMethodException {
        Constructor better = null;
        boolean mustConv = false;
        boolean isVarArgs = false;
        long sqd = Long.MAX_VALUE;
        boolean[] refBools = new boolean[2];
        for (Constructor f : functions) {
            Class[] types = f.getParameterTypes();
            refBools[0] = false;
            refBools[1] = false;
            long d = compareClassArgs(types, argsClass, args, refBools);
            if (d != Long.MIN_VALUE && d < sqd) {
                sqd = d;
                better = f;
                mustConv = refBools[0];
                isVarArgs = refBools[1];
            }

            if (d == 0) {
                break;
            }
        }

        if (better != null) {
            if (mustConv) {
                Class[] types = better.getParameterTypes();
                for (int i = 0; i < types.length; i++) {
                    if (types[i] == int.class && argsClass[i] == double.class) {
                        argsClass[i] = int.class;
                        args[i] = ((Double) args[i]).intValue();
                    }
                }
            }
            if (isVarArgs) {
                Class[] types = better.getParameterTypes();
                Object[] o = new Object[args.length - types.length + 1];
                System.arraycopy(args, types.length - 1, o, 0, args.length - types.length + 1);

                Object[] newArgs = new Object[types.length];
                System.arraycopy(args, 0, newArgs, 0, types.length - 1);
                newArgs[types.length - 1] = o;

                return new Object[] {better, newArgs};
            }

            return new Object[] {better};
        }

        throw new NoSuchMethodException("");
    }

    /**
     * This function calculates the distance between thes method signatures
     * @param A the method signature
     * @param B the class of the passed arguments
     * @param arr array of arguments (used to transform double in int)
     * @param bools references on boolean
     * @return the distance
     */
    private static final long compareClassArgs(Class[] A, Class[] B, Object[] arr, boolean[] bools) {
        if (A.length > B.length) {
            return Long.MIN_VALUE;
        }

        long s = 0;
        int end = A.length;
        if (A.length > 0 && ((A.length < B.length && A[A.length - 1] == Object[].class) || (A.length == B.length && A.length == 1 && A[0] == Object[].class && B[0] != Object[].class))) {
            bools[1] = true;
            end--;
            s = 1 << 40;
        } else if (A.length < B.length) {
            return Long.MIN_VALUE;
        }

        for (int i = 0; i < end; i++) {
            long d = dist(A[i], B[i]);
            if (d == -1) {
                if (A[i] == int.class && B[i] == double.class && ((Double) arr[i]).intValue() == ((Double) arr[i]).doubleValue()) {
                    d = 2048;
                    bools[0] = true;
                } else {
                    return Long.MIN_VALUE;
                }
            }
            s += d * d;
        }

        return s;
    }

    /**
     * Calculate the distance between two classes.
     * @param A a class
     * @param B another class
     * @return the distance
     */
    private static final long dist(Class A, Class B) {
        if (B == null) {
            return 0;
        }

        if (!A.isPrimitive() && B.isPrimitive()) {
            B = ScilabJavaObject.primTypes.get(B);
        }

        if (A == B) {
            return 0;
        }

        if (A.isAssignableFrom(B)) {
            long i = 0;
            do {
                i++;
                B = B.getSuperclass();
            } while (B != null && A.isAssignableFrom(B));

            return i;
        }

        return -1;
    }
}
