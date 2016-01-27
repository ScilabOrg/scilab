/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Pierre GRADIT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
 *
 */

package org.scilab.modules.preferences;

import org.w3c.dom.Node;

/** XComponent implements scilab components compliant with XManagement.
 *
 * @author Pierre GRADIT
 */
public interface XComponent {

    // CAUTION : Layout information have to be set in constructor!

    /** Define the set of actuators.
     *
     * @return array of actuator names.
     */
    String [] actuators();

    /** Refresh the component by the use of actuators.
     *
     * @param peer the corresponding view DOM node
     */
    void refresh(Node peer);
}


