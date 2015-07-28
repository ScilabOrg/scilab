/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos;

/**
 * Default view for the Xcos java side.
 */
public class XcosView extends View {

    /**
     * To retain the id on object creation
     */
    private long retainedId;

    public XcosView() {

    }

    /**
     * @return the id
     */
    public long getId() {
        return retainedId;
    }

    /**
     * @param id the id to set
     */
    public void setId(long id) {
        this.retainedId = id;
    }

    @Override
    public void objectCreated(long uid, Kind k) {
        setId(uid);
        //System.out.println("XcosView::objectCreated(" + uid + ", " + k + ")");
    }

    @Override
    public void objectReferenced(long uid, Kind k) {
        //System.out.println("XcosView::objectReferenced(" + uid + ", " + k + ")");
    }

    @Override
    public void objectUnreferenced(long uid, Kind k) {
        //System.out.println("XcosView::objectUnreferenced(" + uid + ", " + k + ")");
    }

    @Override
    public void objectDeleted(long uid, Kind k) {
        //System.out.println("XcosView::objectDeleted(" + uid + ", " + k + ")");
    }

    @Override
    public void propertyUpdated(long uid, Kind k, ObjectProperties p, UpdateStatus u) {
        //System.out.println("XcosView::propertyUpdated(" + uid + ", " + k + ", " + p + "): " + u);
    }
}
