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
 * === LICENSE_END ===
 */

package org.scilab.forge.scirenderer.buffers;

import java.nio.IntBuffer;
import java.util.Collection;

/**
 * @author Pierre Lando
 */
public interface IndicesBuffer extends DataBuffer {

    /**
     * Set the data
     * @param indices the new data.
     */
    void setData(int[] indices);


    /**
     * Set the data
     * @param indices the new data.
     */
    void setData(Collection<Integer> indices);

    /**
     * Set the data.
     * @param indexBuffer the new data.
     */
    void setData(IntBuffer indexBuffer);

    @Override
    IntBuffer getData();


    void clear();
}
