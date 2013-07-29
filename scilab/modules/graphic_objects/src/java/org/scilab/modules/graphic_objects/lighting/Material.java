/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro SOUZA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.lighting;

import org.scilab.modules.graphic_objects.lighting.ColorTriplet;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

public class Material extends ColorTriplet {

    public enum MaterialProperty {COLOR_MATERIAL, SHININES};
    /** Specifies if the sufarce color will be used as diffuse color */
    private Boolean useColorMaterial;

    /** Specular shinines parameter*/
    private Double shinines;


    public Material() {
        super();
        useColorMaterial = true;
        ambient[0] = ambient[1] = ambient[2] = 0.3;
        diffuse[0] = diffuse[1] = diffuse[2] = 1.0;
        specular[0] = specular[1] = specular[2] = 1.0;
        shinines = 10.0;
    }

    public UpdateStatus setColorMaterialMode(Boolean status) {
        if (useColorMaterial != status) {
            useColorMaterial = status;
            return UpdateStatus.Success;
        }
        return UpdateStatus.NoChange;
    }

    public Boolean getColorMaterialMode() {
        return useColorMaterial;
    }
    
    public UpdateStatus setShinines(Double s) {
        if (shinines != s) {
            shinines = s;
            return UpdateStatus.Success;
        }
        return UpdateStatus.NoChange;
    }
    
    public Double getShinines() {
        return shinines;
    }
   
}
