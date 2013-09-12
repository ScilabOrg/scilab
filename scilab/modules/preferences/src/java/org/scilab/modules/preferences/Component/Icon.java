/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Pierre GRADIT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.preferences.Component;

import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XConfigManager;
import javax.swing.JLabel;
import javax.swing.ImageIcon;
import org.w3c.dom.Node;

//TODO import org.scilab.modules.gui.bridge.label.SwingScilabLabel;
// - setIcon() and getIcon() not reachable...

/** Implementation of Label compliant with extended management.
*
* @author Pierre GRADIT
*
*/
public class Icon extends JLabel implements XComponent {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = 3026097634733924562L;

    /** Path for icons.
    *
    */
    public static final   String   SCILAB_XICONS  = System.getenv("SCI")
            + "/modules/gui/images/icons/16x16/actions/";

    public static final   String   SCILAB_ICONS  = System.getenv("SCI")
            + "/modules/gui/images/icons/";

    /** Define the set of actuators.
    *
    * @return array of actuator names.
    */
    public final String [] actuators() {
        String [] actuators = {"src"};
        return actuators;
    }

    /** Constructor.
    *
    * @param peer : associated view DOM node.
    */
    public Icon(final Node peer) {
        super();

        String src = XConfigManager.getAttribute(peer , "src");
        src(src);
    }

    /** Refresh the component by the use of actuators.
    *
    * @param peer the corresponding view DOM node
    */
    public final void refresh(final Node peer) {

        String src = XConfigManager.getAttribute(peer , "src");
        if (!src.equals(src())) {
            src(src);
        }
    }

    /** Sensor for 'src' attribute.
    *
    * @return the attribute value.
    */
    public final String src() {
        ImageIcon icon = (ImageIcon) getIcon();
        return icon.getDescription();
    }

    /** Actuator for 'src' attribute.
    *
    * @param src : the attribute value.
    */
    public final void src(final String src) {
        ImageIcon icon = new ImageIcon(SCILAB_XICONS + src);
        if (icon.getImageLoadStatus() == 4) {
            icon = new ImageIcon(SCILAB_ICONS + src);
        }
        icon.setDescription(src);
        setIcon(icon);
    }

    /** Developer serialization method.
    *
    * @return equivalent signature.
    */
    public final String toString() {
        String signature = "Icon";

        return signature;
    }
}

