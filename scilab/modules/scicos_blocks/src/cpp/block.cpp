/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

#include "block.hxx"

extern "C"
{
#include "scicos_block4.h"
}

namespace blocks
{
    /**
     * Translate execution to the related functions.
     *
     * @param flag current execution flag
     * @return an error number or 0
     */
    int Block::exec(scicos_flag flag)
    {
        int err;
    
        switch(flag) {
            case DerivativeState:
                err = derivateState();
                break;
            case OutputUpdate:
                err = updateOutput();
                break;
            case StateUpdate:
                err = updateState();
                break;
            case OutputEventTiming:
                err = outputEventTiming();
                break;
            case Initialization:
                err = initialize();
                break;
            case Ending:
                err = end();
                break;
            case ReInitialization:
                err = reInitialize();
                break;
            case ZeroCrossing:
                err = onZeroCrossing();
                break;
            default:
                err = -1;
                break;
        }
        
        return err;
    }
    
    /**
     * \brief default empty implementation
     * \return 0
     */
    int Block::derivateState() {
        return 0;
    }

    /**
     * \brief default empty implementation
     * \return 0
     */
    int Block::updateOutput() {
        return 0;
    }

    /**
     * \brief default empty implementation
     * \return 0
     */
    int Block::updateState() {
        return 0;
    }

    /**
     * \brief default empty implementation
     * \return 0
     */
    int Block::outputEventTiming() {
        return 0;
    }

    /**
     * \brief default empty implementation
     * \return 0
     */
    int Block::initialize() {
        return 0;
    }

    /**
     * \brief default empty implementation
     * \return 0
     */
    int Block::end() {
        return 0;
    }

    /**
     * \brief default empty implementation
     * \return 0
     */
    int Block::reInitialize() {
        return 0;
    }
    
    /**
     * \brief default empty implementation
     * \return 0
     */
    int Block::onZeroCrossing() {
        return 0;
    }
}

