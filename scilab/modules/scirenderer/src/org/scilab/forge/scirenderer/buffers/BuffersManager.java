/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

package org.scilab.forge.scirenderer.buffers;

import java.util.Collection;

/**
 * Interface for a buffers manager.
 * @author Pierre Lando
 */
public interface BuffersManager {

    /**
     * Create an elements buffer.
     * @return a new elements buffer.
     */
    ElementsBuffer createElementsBuffer();

    /**
     * Create an indices buffer.
     * @return a new indices buffer.
     */
    IndicesBuffer createIndicesBuffer();

    /**
     * Release all resources used by the given buffer.
     * @param buffer the given buffer.
     */
    void dispose(DataBuffer buffer);

    /**
     * Release all resources used by the given buffers.
     * @param buffers the given buffers.
     */
    void dispose(Collection <? extends DataBuffer > buffers);
}
