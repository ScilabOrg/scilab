/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bernard HUGUENEY
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef PARALLEL_SHAREDDATA_HXX
#define PARALLEL_SHAREDDATA_HXX

#include "internal.hxx"

#define CONST


namespace parallel
{
    /*
     * Base class for dynamic dispatching of shared memory buffers to copy results across processes.
     */
    struct SharedData
    {
        /*
         * read data into the shared memory buffer
         * @param b ptr to the first element of an array of bools indicating if we must read the data
         * for the corresponding slice (we cannot template on an iterator because the member function is virtual).
         * @param e ptr to the past the end element of said array.
         * @param d reference to a Scilab variable from which we will read data.
         */
        template<typename In>
        void readSlices(In b, In e, types::InternalType CONST& i)
        {
            for(std::size_t c(0); b != e; ++b, ++c)
            {
                if(*b)
                {
                    this->readSlice(c, i); // dynamic dispatching to the derived implementation
                }
            }
        }
        virtual void readSlice(std::size_t c, types::InternalType CONST& i)=0;
        /*
         * write data to fill a Scilab var
         * @param b ptr to the first element of an array of bools indicating if we must read the data
         * for the corresponding slice (we cannot template on an iterator because the member function is virtual).
         * @param e ptr to the past the end element of said array.
         * @param d reference to a Scilab variable from which we will read data.
         */
        virtual void write(types::InternalType & d)=0;
        /*
         * polymorphic destructor to release the allocated shared memory buffer(s)
         */
        virtual ~SharedData(){};
        /*
         * Factory static function to create the appropriate shared buffer according to the runtime type
         * of a given Scilab var.
         */
        static SharedData* create(types::InternalType*);
    };
}
#endif
