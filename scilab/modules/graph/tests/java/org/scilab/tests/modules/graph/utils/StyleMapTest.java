/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.tests.modules.graph.utils;

import org.scilab.modules.graph.utils.StyleMap;
import org.junit.*;

/**
 * Test the {@link StyleMap} class
 */
public class StyleMapTest {
    private static final String light = "key=value;BlockName;";
    private static final String duplicate = "BlockName;key=value;key=value;BlockName";

    @Test
    public void emptyString() {
        StyleMap map = new StyleMap("");
        assert map.entrySet().size() == 0;

        String str = map.toString();
        assert str.compareTo("") == 0;
    }

    @Test
    public void lightString() {
        StyleMap map = new StyleMap(light);
        assert map.containsKey("BlockName");
        assert map.get("BlockName") == null;
        assert map.containsKey("key");
        assert map.get("key").compareTo("value") == 0;

        String str = map.toString();
        assert areEquals(new StyleMap(str), map);
    }

    @Test
    public void withoutFinalSemiColon() {
        StyleMap map = new StyleMap(light.substring(0, light.length() - 1));
        assert map.containsKey("BlockName");
        assert map.get("BlockName") == null;
        assert map.containsKey("key");
        assert map.get("key").compareTo("value") == 0;

        String str = map.toString();
        assert areEquals(new StyleMap(str), new StyleMap(light));
    }

    @Test
    public void duplicateKeyValues() {
        StyleMap map = new StyleMap(duplicate);
        assert map.containsKey("BlockName");
        assert map.get("BlockName") == null;
        assert map.containsKey("key");
        assert map.get("key").compareTo("value") == 0;

        String str = map.toString();
        assert areEquals(new StyleMap(str), new StyleMap(light));
    }

    /**
     * @param m1
     *            the first map to compare
     * @param m2
     *            the second map to compare
     * @return Return true if m1 and m2 contains the same keys and values, false
     *         otherwise.
     */
    private boolean areEquals(StyleMap m1, StyleMap m2) {
        for (String key : m1.keySet()) {
            if (m2.containsKey(key)) {
                if (m1.get(key) != null) {
                    if (m1.get(key).compareTo(m2.get(key)) != 0) {
                        return false;
                    }
                }
            } else {
                return false;
            }
        }

        return true;
    }
}
