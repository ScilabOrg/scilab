package org.scilab.modules.graphic_objects.xmlloader;

import java.awt.GridBagConstraints;
import java.util.HashMap;
import java.util.Map;
import java.util.StringTokenizer;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;

public class XmlTools {
    public static final Map<String, Integer> GridBagConstants = new HashMap<String, Integer>();

    static {
        GridBagConstants.put("northwest", GridBagConstraints.NORTHWEST);
        GridBagConstants.put("nw", GridBagConstraints.NORTHWEST);
        GridBagConstants.put("north", GridBagConstraints.NORTH);
        GridBagConstants.put("n", GridBagConstraints.NORTH);
        GridBagConstants.put("northeast", GridBagConstraints.NORTHEAST);
        GridBagConstants.put("ne", GridBagConstraints.NORTHEAST);
        GridBagConstants.put("west", GridBagConstraints.WEST);
        GridBagConstants.put("w", GridBagConstraints.WEST);
        GridBagConstants.put("center", GridBagConstraints.CENTER);
        GridBagConstants.put("c", GridBagConstraints.CENTER);
        GridBagConstants.put("east", GridBagConstraints.EAST);
        GridBagConstants.put("e", GridBagConstraints.EAST);
        GridBagConstants.put("southwest", GridBagConstraints.SOUTHWEST);
        GridBagConstants.put("sw", GridBagConstraints.SOUTHWEST);
        GridBagConstants.put("south", GridBagConstraints.SOUTH);
        GridBagConstants.put("s", GridBagConstraints.SOUTH);
        GridBagConstants.put("southeast", GridBagConstraints.SOUTHEAST);
        GridBagConstants.put("se", GridBagConstraints.SOUTHEAST);
        GridBagConstants.put("first_line_start", GridBagConstraints.FIRST_LINE_START);
        GridBagConstants.put("page_start", GridBagConstraints.PAGE_START);
        GridBagConstants.put("first_line_end", GridBagConstraints.FIRST_LINE_END);
        GridBagConstants.put("line_start", GridBagConstraints.LINE_START);
        GridBagConstants.put("line_end", GridBagConstraints.LINE_END);
        GridBagConstants.put("last_line_start", GridBagConstraints.LAST_LINE_START);
        GridBagConstants.put("page_end", GridBagConstraints.PAGE_END);
        GridBagConstants.put("last_line_end", GridBagConstraints.LAST_LINE_END);
        GridBagConstants.put("baseline", GridBagConstraints.BASELINE);
        GridBagConstants.put("baseline_leading", GridBagConstraints.BASELINE_LEADING);
        GridBagConstants.put("baseline_trailing", GridBagConstraints.BASELINE_TRAILING);
        GridBagConstants.put("above_baseline", GridBagConstraints.ABOVE_BASELINE);
        GridBagConstants.put("above_baseline_leading", GridBagConstraints.ABOVE_BASELINE_LEADING);
        GridBagConstants.put("above_baseline_trailing", GridBagConstraints.ABOVE_BASELINE_TRAILING);
        GridBagConstants.put("below_baseline", GridBagConstraints.BELOW_BASELINE);
        GridBagConstants.put("below_baseline_leading", GridBagConstraints.BELOW_BASELINE_LEADING);
        GridBagConstants.put("below_baseline_trailing", GridBagConstraints.BELOW_BASELINE_TRAILING);
        GridBagConstants.put("none", GridBagConstraints.NONE);
        GridBagConstants.put("horizontal", GridBagConstraints.HORIZONTAL);
        GridBagConstants.put("h", GridBagConstraints.HORIZONTAL);
        GridBagConstants.put("vertical", GridBagConstraints.VERTICAL);
        GridBagConstants.put("v", GridBagConstraints.VERTICAL);
        GridBagConstants.put("both", GridBagConstraints.BOTH);
        GridBagConstants.put("b", GridBagConstraints.BOTH);
    }

    public static Boolean setPropAsDouble(Integer uid, Integer prop, String propString) {
        if (propString == null || propString.equals("")) {
            return false;
        }

        GraphicController.getController().setProperty(uid, prop, Double.parseDouble(propString));
        return true;
    }

    public static Boolean setPropAsDoubleVector(Integer uid, Integer prop, Map<String, String> map, String[] propString) {
        if (propString == null || propString.length == 0) {
            return false;
        }

        Double[] values = new Double[propString.length];
        for (int i = 0 ; i < propString.length ; i++) {
            String value = map.get(propString[i]);
            if (value == null || value.equals("")) {
                values[i] = 0.0;
            } else {
                values[i] = Double.parseDouble(value);
            }
        }

        GraphicController.getController().setProperty(uid, prop, values);
        return true;
    }

    public static Boolean setPropAsDoubleVector(Integer uid, Integer prop, String propString, Integer size) {
        if (propString == null || propString.equals("")) {
            return false;
        }

        Double[] values = new Double[size];

        StringTokenizer tokens = new StringTokenizer(propString, ",");
        if (tokens.countTokens() > size) {
            return false;
        }

        int i = 0;
        while (tokens.hasMoreTokens()) {
            values[i++] = Double.parseDouble(tokens.nextToken());
        }

        GraphicController.getController().setProperty(uid, prop, values);
        return true;
    }

    public static Boolean setPropAsBoolean(Integer uid, Integer prop, String propString) {
        if (propString == null || propString.equals("")) {
            return false;
        }

        GraphicController.getController().setProperty(uid, prop, Boolean.parseBoolean(propString));
        return true;
    }

    public static Boolean setPropAsBooleanVector(Integer uid, Integer prop, String propString, Integer size) {
        if (propString == null || propString.equals("")) {
            return false;
        }

        Boolean[] values = new Boolean[size];

        StringTokenizer tokens = new StringTokenizer(propString, ",");
        if (tokens.countTokens() > size) {
            return false;
        }

        int i = 0;
        while (tokens.hasMoreTokens()) {
            values[i++] = Boolean.parseBoolean(tokens.nextToken());
        }

        GraphicController.getController().setProperty(uid, prop, values);
        return true;
    }

    public static Boolean setPropAsInteger(Integer uid, Integer prop, String propString) {
        if (propString == null || propString.equals("")) {
            return false;
        }

        GraphicController.getController().setProperty(uid, prop, Integer.parseInt(propString));
        return true;
    }

    public static Boolean setPropAsIntegerVector(Integer uid, Integer prop, String propString, Integer size) {
        if (propString == null || propString.equals("")) {
            return false;
        }

        Integer[] values = new Integer[size];

        StringTokenizer tokens = new StringTokenizer(propString, ",");
        if (tokens.countTokens() > size) {
            return false;
        }

        int i = 0;
        while (tokens.hasMoreTokens()) {
            values[i++] = Integer.parseInt(tokens.nextToken());
        }

        GraphicController.getController().setProperty(uid, prop, values);
        return true;
    }

    public static Boolean setPropAsString(Integer uid, Integer prop, String propString) {
        if (propString == null || propString.equals("")) {
            return false;
        }

        GraphicController.getController().setProperty(uid, prop, propString);
        return true;
    }

    public static Boolean setPropAsStringVector(Integer uid, Integer prop, String propString, Integer size) {
        if (propString == null || propString.equals("")) {
            return false;
        }

        String[] values = new String[size];

        StringTokenizer tokens = new StringTokenizer(propString, ",");
        if (tokens.countTokens() > size) {
            return false;
        }

        int i = 0;
        while (tokens.hasMoreTokens()) {
            values[i++] = tokens.nextToken();
        }

        GraphicController.getController().setProperty(uid, prop, values);
        return true;
    }

    public static Double[] getGridconstraints(Map<String, String> map) {
        Double[] ret = new Double[2];
        return ret;
    }

    public static Double[] getGridbagconstraints(Map<String, String> map) {
        Double[] ret = new Double[6];

        Double value = 0.0;
        String item = null;
        int i = 0;

        //weightx
        item = map.get("weightx");
        if (item != null) {
            value = Double.parseDouble(item);
            ret[i++] = value;
        }

        //weighty
        item = map.get("weighty");
        if (item != null) {
            value = Double.parseDouble(item);
            ret[i++] = value;
        }

        //ipadx
        item = map.get("ipadx");
        if (item != null) {
            value = Double.parseDouble(item);
            ret[i++] = value;
        }

        //ipady
        item = map.get("ipady");
        if (item != null) {
            value = Double.parseDouble(item);
            ret[i++] = value;
        }

        //fill ( horizontal, vertical, both, none ? )
        item = map.get("fill");
        if (item != null) {
            value = (double)GridBagConstants.get(item);
            ret[i++] = value;
        }

        //anchors ( horizontal, vertical, both, none ? )
        item = map.get("anchor");
        if (item != null) {
            value = (double)GridBagConstants.get(item);
            ret[i++] = value;
        }

        return ret;
    }

    public static String getFromMap(Map<String, String> map, String field) {
        if (map != null) {
            return map.get(field);
        }
        return null;
    }
}

