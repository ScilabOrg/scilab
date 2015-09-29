/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos;

import org.scilab.modules.graph.utils.StyleMap;

/**
 * Update the source block when the interface function change.
 */
public final class UpdateStyleFromInterfunctionAdapter extends XcosViewListener {
    public UpdateStyleFromInterfunctionAdapter() { }

    /**
     * Put the interfunction at the start of the style map to preserve
     * style modification.
     *
     * oldStyle="SUPER_f;fillColor=red" newStyle="DSUPER;fillColor=red"
     *
     * and not newStyle="fillColor=red;DSUPER"
     */
    @Override
    public void propertyUpdated(long uid, Kind kind, ObjectProperties property, UpdateStatus status) {
        if (status != UpdateStatus.SUCCESS) {
            return;
        }

        // prevent any unrelated property change, safety code
        if (property != ObjectProperties.INTERFACE_FUNCTION && property != ObjectProperties.STYLE) {
            return;
        }

        JavaController controller = new JavaController();

        String[] interfaceFunction = new String[1];
        controller.getObjectProperty(uid, kind, ObjectProperties.INTERFACE_FUNCTION, interfaceFunction);

        String[] style = new String[1];
        controller.getObjectProperty(uid, kind, ObjectProperties.STYLE, style);


        final StyleMap styleMap = new StyleMap(interfaceFunction[0]);
        styleMap.putAll(style[0]);

        controller.setObjectProperty(uid, kind, ObjectProperties.STYLE, styleMap.toString());
    }
}
