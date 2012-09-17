/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __H5BASICDATA_HXX__
#define __H5BASICDATA_HXX__

#include "H5Data.hxx"
#include "H5Object.hxx"
#include "H5DataConverter.hxx"

#define __SCILAB_STACK_CREATOR__(U,NAME) static void create(void * pvApiCtx, const int position, const int rows, const int cols, U * ptr, int * list, const int listPosition) \
    {                                                                   \
        SciErr err;                                                     \
        if (list)                                                       \
        {                                                               \
            err = createMatrixOf##NAME##InList(pvApiCtx, position, list, listPosition, rows, cols, ptr); \
        }                                                               \
        else                                                            \
        {                                                               \
            err = createMatrixOf##NAME(pvApiCtx, position, rows, cols, ptr); \
        }                                                               \
        if (err.iErr)                                                   \
        {           printError(&err, 0);				\
            throw H5Exception(__LINE__, __FILE__, "Cannot allocate memory"); \
        }                                                               \
    }

#define __SCILAB_STACK_ALLOCATOR__(U,NAME) static void alloc(void * pvApiCtx, const int position, const int rows, const int cols, int * list, const int listPosition, U ** ptr) \
    {                                                                   \
        SciErr err;                                                     \
        if (list)                                                       \
        {                                                               \
            err = allocMatrixOf##NAME##InList(pvApiCtx, position, list, listPosition, rows, cols, ptr); \
        }                                                               \
        else                                                            \
        {                                                               \
            err = allocMatrixOf##NAME(pvApiCtx, position, rows, cols, ptr); \
        }                                                               \
        if (err.iErr)                                                   \
        {           printError(&err, 0);                                                            \
            throw H5Exception(__LINE__, __FILE__, "Cannot allocate memory"); \
        }                                                               \
    }

#define __SCILAB_ALLOCATORS_CREATORS__(U,NAME) __SCILAB_STACK_CREATOR__(U,NAME) \
    __SCILAB_STACK_ALLOCATOR__(U,NAME)


namespace org_modules_hdf5
{
    template<typename T>
    class H5BasicData : public H5Data
    {

    protected:

        T * transformedData;

    public:

        H5BasicData(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, T * _data, const hsize_t _stride = -1, const size_t _offset = 0, const bool _dataOwner = true) : H5Data(_parent, _totalSize, _dataSize, _ndims, _dims, static_cast<void *>(_data), _stride, _offset, _dataOwner), transformedData(0)
            {

            }

        virtual ~H5BasicData()
            {
                if (transformedData)
                {
                    delete[] transformedData;
                }
            }

	virtual void printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const
	    {
		os << static_cast<T *>(getData())[pos];
	    }

        virtual void copyData(T * dest) const
            {
                if (dest)
                {
                    if (stride == -1)
                    {
                        memcpy(static_cast<void *>(dest), data, totalSize * sizeof(T));
                    }
                    else if (transformedData)
                    {
                        memcpy(static_cast<void *>(dest), static_cast<void *>(transformedData), totalSize * sizeof(T));
                    }
                    else
                    {
                        char * cdata = static_cast<char *>(data) + offset;
                        for (int i = 0; i < totalSize; i++)
                        {
                            dest[i] = *((T *)cdata);
                            cdata += stride;
                        }
                    }
                }
                else
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot copy data to an empty pointer"));
                }
            }

        virtual void * getData() const
            {
                if (stride == -1)
                {
                    return data;
                }
                else
                {
                    if (!transformedData)
                    {
			T * dest = new T[totalSize];
                        copyData(dest);
			const_cast<H5BasicData *>(this)->transformedData = dest;
                    }

                    return static_cast<void *>(transformedData);
                }
            }

        virtual void toScilab(void * pvApiCtx, const int lhsPosition, int * parentList = 0, const int listPosition = 0) const
            {
                SciErr err;
                T * newData = 0;
                if (ndims == 1)
                {
                    alloc(pvApiCtx, lhsPosition, 1, *dims, parentList, listPosition, &newData);
                    copyData(newData);
                }
                else
                {
                    if (ndims == 2)
                    {
                        alloc(pvApiCtx, lhsPosition, dims[0], dims[1], parentList, listPosition, &newData);
                        H5DataConverter::C2FHypermatrix(2, dims, 0, static_cast<T *>(getData()), newData);
                    }
                    else
                    {
                        int * list = getHypermatrix(pvApiCtx, lhsPosition, parentList, listPosition);
			alloc(pvApiCtx, lhsPosition, 1, totalSize, list, 3, &newData);
                        H5DataConverter::C2FHypermatrix(ndims, dims, totalSize, static_cast<T *>(getData()), newData);
                    }
                }
            }

        virtual std::string dump(std::set<haddr_t> & alreadyVisited, const unsigned int indentLevel) const
            {
                return H5DataConverter::dump(alreadyVisited, indentLevel, ndims, dims, *this);
            }

        __SCILAB_ALLOCATORS_CREATORS__(double,Double)
        __SCILAB_ALLOCATORS_CREATORS__(char,Integer8)
        __SCILAB_ALLOCATORS_CREATORS__(unsigned char,UnsignedInteger8)
        __SCILAB_ALLOCATORS_CREATORS__(short,Integer16)
        __SCILAB_ALLOCATORS_CREATORS__(unsigned short,UnsignedInteger16)
        __SCILAB_ALLOCATORS_CREATORS__(int,Integer32)
        __SCILAB_ALLOCATORS_CREATORS__(unsigned int,UnsignedInteger32)

#ifdef  _SCILAB_INT64__
        __SCILAB_ALLOCATORS_CREATORS__(long long,Integer64)
        __SCILAB_ALLOCATORS_CREATORS__(unsigned long long,UnsignedInteger64)
#endif

        __SCILAB_STACK_CREATOR__(char *,String)
    };
}


#undef __SCILAB_STACK_CREATOR__
#undef __SCILAB_STACK_ALLOCATOR__
#undef __SCILAB_LIST_CREATOR__
#undef __SCILAB_LIST_ALLOCATOR__
#undef __SCILAB_ALLOCATORS_CREATORS__

#endif // __H5BASICDATA_HXX__
