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

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Log all model's modification to a logger and to stderr
 */
public class LoggerView extends View {

    Logger logger;

    /**
     * Default constructor
     */
    public LoggerView() {
        logger = Logger.getLogger(Xcos.class.getSimpleName());
    }

    /* (non-Javadoc)
     * @see org.scilab.modules.xcos.View#objectCreated(long, org.scilab.modules.xcos.Kind)
     */
    @Override
    public void objectCreated(long uid, Kind kind) {
        logger.log(Level.INFO, "objectCreated", new Object [] {uid, kind});
    }

    /* (non-Javadoc)
     * @see org.scilab.modules.xcos.View#objectDeleted(long, org.scilab.modules.xcos.Kind)
     */
    @Override
    public void objectDeleted(long uid, Kind kind) {
        logger.log(Level.INFO, "objectDeleted", new Object [] {uid, kind});
    }

    /* (non-Javadoc)
     * @see org.scilab.modules.xcos.View#propertyUpdated(long, org.scilab.modules.xcos.Kind, org.scilab.modules.xcos.ObjectProperties, org.scilab.modules.xcos.UpdateStatus)
     */
    @Override
    public void propertyUpdated(long uid, Kind kind, ObjectProperties property, UpdateStatus status) {
        logger.log(Level.INFO, "propertyUpdated", new Object [] {uid, kind, property, status});
    }
}
