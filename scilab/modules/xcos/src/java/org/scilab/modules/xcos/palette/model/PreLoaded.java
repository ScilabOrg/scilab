/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.palette.model;

import java.util.ArrayList;
import java.util.List;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlTransient;
import javax.xml.bind.annotation.XmlType;

/**
 * <p>
 * A Preloaded palette contains a number of preloaded blocks (
 * {@link PaletteBlock}).
 *
 * <p>
 * The following schema fragment specifies the expected content contained within
 * this class.
 *
 * <pre>
 * &lt;complexType name="PreLoaded">
 *   &lt;complexContent>
 *     &lt;extension base="{}Palette">
 *       &lt;sequence>
 *         &lt;element name="block" type="{}PaletteBlock" maxOccurs="unbounded" minOccurs="0"/>
 *       &lt;/sequence>
 *     &lt;/extension>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 *
 *
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "PreLoaded", propOrder = { "block" })
public class PreLoaded extends Palette {

    /**
     * Not marshalled {@link PreLoaded} class
     */
    @XmlTransient
    public static class Dynamic extends PreLoaded {
        /**
         * Default constructor.
         */
        public Dynamic() {
        }
    }

    @XmlElement(nillable = true)
    private List<PaletteBlock> block;

    /**
     * Default constructor
     */
    public PreLoaded() {
    }

    /**
     * Gets the value of the block property.
     *
     * <p>
     * This accessor method returns a reference to the live list, not a
     * snapshot. Therefore any modification you make to the returned list will
     * be present inside the JAXB object. This is why there is not a
     * <CODE>set</CODE> method for the block property.
     *
     * <p>
     * For example, to add a new item, do as follows:
     *
     * <pre>
     * getBlock().add(newItem);
     * </pre>
     *
     *
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link PaletteBlock }
     *
     * @return the non-null block list
     */
    public List<PaletteBlock> getBlock() {
        if (block == null) {
            block = new ArrayList<PaletteBlock>();
        }
        return block;
    }

}
