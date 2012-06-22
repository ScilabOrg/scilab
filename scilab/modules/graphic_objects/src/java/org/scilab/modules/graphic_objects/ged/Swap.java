/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos Cardinot
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.graphic_objects.ged;

import javax.swing.JPanel;


/**
* Manager object exchange.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class Swap {
    public Swap(String select, String objectID) {
         if ("axes".equals(select)) {
            Inspector.pReceive.removeAll();
            JPanel received = new Axes();
            received.setBounds(2, 0, 348, 780);
            received.setVisible(true);
            Inspector.pReceive.add(received);
            Inspector.pReceive.setPreferredSize(new java.awt.Dimension(370, 780));
            Inspector.selectaux = select;
        } else {
            if ("curve".equals(select)) {
                Inspector.pReceive.removeAll();
                JPanel received = new Curve(objectID);
                received.setBounds(2, 0, 348, 300);
                received.setVisible(true);
                Inspector.pReceive.add(received);
                Inspector.pReceive.setPreferredSize(new java.awt.Dimension(370, 300));
                Inspector.selectaux = select;

            } else {
                Inspector.pReceive.removeAll();
                JPanel received = new Figure(objectID);
                received.setBounds(2, 0, 348, 600);
                received.setVisible(true);
                Inspector.pReceive.add(received);
                Inspector.pReceive.setPreferredSize(new java.awt.Dimension(370, 600));
                Inspector.selectaux = select;
            }
        }
   }
}
