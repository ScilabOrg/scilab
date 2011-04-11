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
#ifndef BLOCK_HXX
#define BLOCK_HXX

extern "C"
{
#include "scicos_block4.h"
}

namespace blocks
{
    class Block
    {
        public :
            /**
             * \brief Construct with parameters
             * 
             * \example
             * void tows_c(scicos_block *block, scicos_flag flag) {
             *    blocks::ToWorkspace(block).exec(flag);
             * }
             */
            Block(scicos_block *block);

            /**
             * Execute the current instance for a flag.
             */
            int exec(scicos_flag flag);

        protected :
            // YaSp: should be private with type accessor
			/**
			 * Contains data for the current instance
			 */
			scicos_block*   block;

            virtual int derivateState();
            virtual int updateOutput();
            virtual int updateState();
            virtual int outputEventTiming();
            virtual int initialize();
            virtual int end();
            virtual int reInitialize();
            virtual int onZeroCrossing();
    };
}

#endif /* !BLOCK_HXX */
