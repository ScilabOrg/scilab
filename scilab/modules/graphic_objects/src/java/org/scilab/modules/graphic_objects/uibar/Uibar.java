/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.uibar;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MESSAGE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MESSAGE_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;

/**
 * @author Vincent COUVERT
 */
public class Uibar extends GraphicObject {

    private String[] message = {""};
    private int value;

    /**
     * All uibar properties
     */
    private enum UibarProperty {
        MESSAGE,
        MESSAGE_SIZE,
        VALUE
    };

    /**
     * Constructor
     */
    public Uibar() {
        super();
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(String propertyName) {
        Object property = null;
        if (propertyName.equals(__GO_UI_MESSAGE__)) {
            property = UibarProperty.MESSAGE;
        } else if (propertyName.equals(__GO_UI_MESSAGE_SIZE__)) {
            property = UibarProperty.MESSAGE_SIZE;
        } else if (propertyName.equals(__GO_UI_VALUE__)) {
            property = UibarProperty.VALUE;
        } else {
            property = super.getPropertyFromName(propertyName);
        }
        return property;
    }

    /**
     * Fast property get method
     * @param property the property to get
     * @return the property value
     */
    public Object getProperty(Object property) {
        Object propertyValue = null;
        if (property == UibarProperty.MESSAGE) {
            propertyValue = getMessage();
        } else if (property == UibarProperty.MESSAGE_SIZE) {
            propertyValue = getMessage().length;
        } else if (property == UibarProperty.VALUE) {
            propertyValue = getValue();
        } else {
            propertyValue = super.getProperty(property); 
        }
        return propertyValue;
    }

    /**
     * Fast property set method
     * @param property the property to set
     * @param value the property value
     * @return true if the property has been set, false otherwise
     */
    public UpdateStatus setProperty(Object property, Object value) {
        if (property == UibarProperty.MESSAGE) {
            setMessage((String[]) value);
        } else if (property == UibarProperty.VALUE) {
            setValue((Integer) value);
        } else {
            return super.setProperty(property, value);
        }

        return UpdateStatus.Success;
    }

    /**
     * Get the uibar message
     * @return the message 
     */
    public String[] getMessage() {
        return this.message;
    }

    /**
     * Set the uibar message
     * @param message the message
     */
    public void setMessage(String[] message) {
        this.message = message;
    }

    /**
     * Get the uibar value
     * @return the value 
     */
    public Integer getValue() {
        return this.value;
    }

    /**
     * Set the uibar value
     * @param value the value
     */
    public void setValue(Integer value) {
        this.value = value;
    }

    /**
     * Accept method
     * @param visitor the visitor
     * @see org.scilab.modules.graphic_objects.graphicObject.GraphicObject#accept(org.scilab.modules.graphic_objects.graphicObject.Visitor)
     */
    public void accept(Visitor visitor) {
    }
}
