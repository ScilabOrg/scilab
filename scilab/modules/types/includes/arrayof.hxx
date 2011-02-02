/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __ARRAYOF_HXX__
#define __ARRAYOF_HXX__

//#include <vector>
//#include <iostream>
//#include <string>
//#include "anytype.hxx"
#include "types.hxx"

extern "C"
{
	#include "core_math.h"
}

namespace types
{

    //commom function
    int GetVarMaxDim(types::InternalType *_pIT, int _iCurrentDim, int _iMaxDim);
    int compute_tuples(int* _piCountDim, int _iDims, int _iCurrentDim, int* _piIndex);
    int check_indexes_arguments(InternalType* _pRef, typed_list* _pArgsIn, typed_list* _pArgsOut, int* _piMaxDim, int* _piCountDim);
    InternalType* create_empty_double();
    InternalType* create_double_vector(int _iSize);

    template <typename T>
    class ArrayOf : public GenericType
    {
    protected :
        bool                    m_bComplex;
        T*                      m_pRealData;
        T*                      m_pImgData;

                                ArrayOf() : GenericType(), m_bComplex(false), m_pRealData(NULL), m_pImgData(NULL) {}
        virtual                 ~ArrayOf() {}

        /*internal constructor*/
        void create(int* _piDims, int _iDims, T** _pRealData, T** _pImgData)
        {
            //try
            //{
                m_iSize     = 1;
                m_iDims     = _iDims;
                m_piDims    = new int[m_iDims];

                for(int i = 0 ; i < m_iDims ; i++)
                {
                    m_piDims[i] = _piDims[i];
                    m_iSize *= m_piDims[i];
                }

                if(_pRealData)
                {
                    m_pRealData = new T[m_iSize];
                    *_pRealData = m_pRealData;
                }
                else
                {
                    m_pRealData = NULL;
                }

                if(_pImgData)
                {
                    m_pImgData = new T[m_iSize];
                    *_pImgData = m_pImgData;
                    m_bComplex = true;
                }
                else
                {
                    m_pImgData = NULL;
                }

                m_iSizeMax = m_iSize;
                m_iRows = m_piDims[0];
                m_iCols = m_piDims[1];
            //}
            //catch (std::bad_alloc &e)
            //{
            //    wchar_t message[bsiz];
            //    os_swprintf(message, bsiz, _W("Can not allocate %.2f MB memory.\n"),  (double) ((double)
            //        m_piDims[0] * (double) m_piDims[1] * sizeof(T)) / 1.e6);
            //    throw(ast::ScilabError(message));
            //}
        }
        

        /*clean values array*/
        void delete_real()
	    {
		    if(m_pRealData != NULL)
		    {
			    delete[] m_pRealData;
			    m_pRealData = NULL;
		    }
	    }
        
        void delete_img(bool _bSetReal = false)
	    {
		    if(m_pImgData != NULL)
		    {
			    delete[] m_pImgData;
			    m_pImgData = NULL;
			    m_bComplex = !_bSetReal;
		    }
	    }
	    
        void delete_all(bool _bSetReal = false)
	    {
		    delete_real();
		    delete_img(_bSetReal);
	    }
        

        virtual T               get_null_value() = 0;
        virtual ArrayOf<T>*     create_empty(int _iDims, int* _piDims, bool _bComplex = false) = 0;
        
    public :

        virtual bool isVector() //only one dim must be != 1
        {
            bool bFirstChance = false;

            for(int i = 0 ; i < m_iDims ; i++)
            {
                if(m_piDims[i] != 1)
                {
                    if(bFirstChance == true)
                    {
                        return false;
                    }
                    else
                    {
                        bFirstChance = true;
                    }
                }
            }
            return true;
        }

        virtual bool isComplex()
        {
            return m_bComplex;
        }

        virtual bool isScalar() //2 dims and each dim == 1
        {
            if(m_iDims == 2 && m_piDims[0] == 1 && m_piDims[1] == 1)
            {
                return true;
            }
            return false;
        }

        virtual void complex_set(bool _bComplex)
	    {
		    if(_bComplex == false)
		    {
			    if(isComplex() == true)
			    {
				    delete_img(!_bComplex);
				    m_bComplex = false;
			    }
		    }
		    else // _bComplex == true
		    {
			    if(isComplex() == false)
			    {
				    m_pImgData = new T[m_iSize];
				    memset(m_pImgData, 0x00, sizeof(T) * m_iSize);
				    m_bComplex = true;
			    }
		    }
	    }
        
        bool set(int _iPos, T _data)
        {
            if(m_pRealData == NULL || _iPos >= m_iSize)
            {
                return false;
            }
            m_pRealData[_iPos] = _data;
            return true;
        }
        
        bool set(int _iRows, int _iCols, T _data)
        {
            int piIndexes[2] = {_iRows, _iCols};
            return set(get_index(piIndexes), _data);
        }
        
        bool set(T* _pdata)
        {
            if(m_pRealData == NULL)
            {
                return false;
            }

            for(int i = 0 ; i < m_iSize ; i++)
            {
                m_pRealData[i] = _pdata[i];
            }
            return true;
        }
        
        bool set(const T* _pdata)
        {
            if(m_pRealData == NULL)
            {
                return false;
            }

            for(int i = 0 ; i < m_iSize ; i++)
            {
                m_pRealData[i] = _pdata[i];
            }
            return true;
        }
        
        T* get() const
        {
            return m_pRealData;
        }
        
        T get(int _iPos)
        {
            if(m_pRealData)
            {
                return m_pRealData[_iPos];
            }
            return T();
        }
        
        T get(int _iRows, int _iCols)
        {
            int piIndexes[2] = {_iRows, _iCols};
            return get(get_index(piIndexes));
        }

        /*internal function to manage img part*/
        bool set_img(int _iPos, T _data)
        {
            if(isComplex() == false || m_pImgData == NULL || _iPos >= m_iSize)
            {
                return false;
            }
            m_pImgData[_iPos] = _data;
            return true;
        }

        
        bool set_img(int _iRows, int _iCols, T _data)
        {
            int piIndexes[2] = {_iRows, _iCols};
            return set_img(get_index(piIndexes), _data);
        }

        bool set_img(T* _pdata)
        {
            if(m_pRealData == NULL)
            {
                return false;
            }

            for(int i = 0 ; i < m_iSize ; i++)
            {
                m_pRealData[i] = _pdata[i];
            }
            return true;
        }

        
        bool set_img(const T* _pdata)
        {
            if(m_pRealData == NULL)
            {
                return false;
            }

            for(int i = 0 ; i < m_iSize ; i++)
            {
                m_pRealData[i] = _pdata[i];
            }
            return true;
        }

        T* get_img() const
        {
            return m_pImgData;
        }
        
        T get_img(int _iPos)
        {
            if(m_pImgData)
            {
                return m_pImgData[_iPos];
            }
            return T();
        }
        
        T get_img(int _iRows, int _iCols)
        {
            int piIndexes[2] = {_iRows, _iCols};
            return get_img(get_index(piIndexes));
        }
        

        InternalType* insert(typed_list* _pArgs, InternalType* _pSource)
        {
            bool bNeedToResize  = false;
            int iDims           = _pArgs->size();
            typed_list pArg;

            int* piMaxDim       = new int[iDims];
            int* piCountDim     = new int[iDims];

            //on case of resize
            int* piNewDims      = NULL;
            int iNewDims        = 0;

            //evaluate each argument and replace by appropriate value and compute the count of combinations
            int iSeqCount = check_indexes_arguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
            if(iSeqCount == 0)
            {
                return this;
            }

            if(iDims >= m_iDims)
            {//all case are good, we can do almost everything
                //check if resize is needed
                if(iDims > m_iDims)
                {
                    bNeedToResize = true;
                }
                else //_iDims == m_iDims
                {
                    for(int i = 0 ; i < m_iDims ; i++)
                    {
                        if(m_piDims[i] < piMaxDim[i])
                        {
                            bNeedToResize = true;
                            break;
                        }
                    }
                }

                //determine new dimension of the matrix
                if(bNeedToResize == true)
                {
                    iNewDims = iDims;
                    piNewDims = new int[iNewDims];
                    for(int i = 0 ; i < m_iDims ; i++)
                    {
                        piNewDims[i] = Max(piMaxDim[i], m_piDims[i]);
                    }

                    for(int i = m_iDims ; i < iNewDims ;i++)
                    {
                        piNewDims[i] = piMaxDim[i];
                    }
                }
            }
            else // _iDims < m_iDims
            {
                //each index before last index must be in range of his dimension
                //and last given dimension can not be > prod(last dimensions)
                for(int i = 0 ; i < iDims - 1 ; i++)
                {
                    //indexes are always doubles
                    double* pIdx = static_cast<double*>(pArg[i]->getAs<ArrayOf>()->get());
                    for(int j = 0 ; j < pArg[i]->getAs<ArrayOf>()->size_get() ; j++)
                    {
                        if(pIdx[j] >= m_piDims[i])
                        {
                            return false;
                        }
                    }
                }

                //check last dim
                int iMaxLastDim = GetVarMaxDim(this, iDims - 1, iDims);
                double* pIdx = static_cast<double*>(pArg[pArg.size() - 1]->getAs<ArrayOf>()->get());
                for(int i = 0 ; i < pArg[pArg.size() - 1]->getAs<ArrayOf>()->size_get() ; i++)
                {
                    if(pIdx[i] > iMaxLastDim)
                    {
                        return false;
                    }
                }
            }

            //before resize, check input dimension
            if(_pSource->getAs<ArrayOf>()->isScalar() == false)
            {
                if(_pSource->getAs<ArrayOf>()->size_get() != iSeqCount)
                {
                    return NULL;
                }
            }

            if(bNeedToResize)
            {
                bool bPass = resize(piNewDims, iNewDims);
                if(bPass == false)
                {
                    return NULL;
                }
            }
            else
            {
                piNewDims   = m_piDims;
                iNewDims    = m_iDims;
            }

            //update complexity
            if(_pSource->getAs<ArrayOf>()->isComplex() && isComplex() == false)
            {
                complex_set(true);
            }

            int* piIndex    = new int[_pArgs->size()];
            int* piCoord    = new int[_pArgs->size()];
            int* piViewDims = new int[iDims];
            memset(piIndex, 0x00, sizeof(int) * _pArgs->size());

            //convert  current dimension to view dimension
            for(int i = 0 ; i < iDims ; i++)
            {
                piViewDims[i] = GetVarMaxDim(this, i, iDims);
            }

            T* pRealData    = _pSource->getAs<ArrayOf>()->get();
            T* pImgData     = _pSource->getAs<ArrayOf>()->get_img();
            bool bComplex   = _pSource->getAs<ArrayOf>()->isComplex();

            for(int i = 0 ; i < iSeqCount ; i++)
            {
                compute_tuples(piCountDim, _pArgs->size(), _pArgs->size() - 1, piIndex);

                //std::cout << "[";
                for(int j = 0 ; j < (int)_pArgs->size() ; j++)
                {
                    piCoord[j] = static_cast<int>(pArg[j]->getAs<ArrayOf>()->get(piIndex[j]) - 1);
                    //std::cout << piCoord[j] << " ";
                }

                //std::cout << "]" << std::endl;

                int iPos = get_index_with_dims(piCoord, piViewDims, iDims);

                if(_pSource->getAs<ArrayOf>()->isScalar())
                {//element-wise insertion
                    set(iPos, pRealData[0]);
                    if(bComplex)
                    {
                        set_img(iPos, pImgData[0]);
                    }
                }
                else
                {
                    set(iPos, pRealData[i]);
                    if(bComplex)
                    {
                        set_img(iPos, pImgData[i]);
                    }
                }

                //update index
                piIndex[0]++;
           }

            if(bNeedToResize)
            {
                delete[] piNewDims;
            }
            
            delete[] piMaxDim;
            delete[] piCountDim;
            delete[] piIndex;
            delete[] piCoord;
            delete[] piViewDims;

            return this;
        }
        
        static InternalType* insert_new(typed_list* _pArgs, InternalType* _pSource)
	    {
            typed_list pArg;
		    InternalType *pOut  = NULL;
            ArrayOf<T>* pSource = _pSource->getAs<ArrayOf>();

            int iDims           = _pArgs->size();
            int* piMaxDim       = new int[iDims];
            int* piCountDim     = new int[iDims];
            bool bComplex       = pSource->isComplex();
            bool bUndefine      = false;

            //evaluate each argument and replace by appropriate value and compute the count of combinations
            int iSeqCount = check_indexes_arguments(NULL, _pArgs, &pArg, piMaxDim, piCountDim);
            if(iSeqCount == 0)
            {
                return create_empty_double();
            }

            if(iSeqCount < 0)
            {
                iSeqCount = -iSeqCount;
                bUndefine = true;
            }

            if(bUndefine)
            {//manage : and $ in creation by insertion
                int iSource         = 0;
                int *piSourceDims   = pSource->get_dims_array();

                for(int i = 0 ; i < iDims ; i++)
                {
                    if(pArg[i] == NULL)
                    {//undefine value
                        piMaxDim[i]     = piSourceDims[iSource];
                        piCountDim[i]   = piSourceDims[iSource];
                        iSource++;

                        //replace pArg value by the new one
                        pArg[i] = create_double_vector(piMaxDim[i]);;
                    }
                    //else
                    //{
                    //    piMaxDim[i] = piCountDim[i];
                    //}
                }
            }

            //remove last dimension at size 1
            //remove last dimension if are == 1
            for(int i = (iDims - 1) ; i >= 2 ; i--)
            {
                if(piMaxDim[i] == 1)
                {
                    iDims--;
                    pArg.pop_back();
                }
                else
                {
                    break;
                }
            }


            if(iDims == 1)
            {
                if(pSource->cols_get() == 1)
                {
                    int piRealDim[2] = {piCountDim[0], 1};
                    pOut = pSource->create_empty(2, piRealDim, bComplex);
                }
                else
                {//rows == 1
                    int piRealDim[2] = {1, piCountDim[0]};
                    pOut = pSource->create_empty(2, piRealDim, bComplex);
                }
            }
            else
            {
                pOut = pSource->create_empty(iDims, piMaxDim, bComplex);
            }

            //fill with null item
            T* pRealData = pOut->getAs<ArrayOf>()->get();
            T* pImgData = pOut->getAs<ArrayOf>()->get_img();

            for(int i = 0 ; i < pOut->getAs<ArrayOf>()->size_get() ; i++)
            {
                pRealData[i] = pSource->get_null_value();
                if(bComplex)
                {
                    pImgData[i] = pSource->get_null_value();
                }
            }

            //insert values in new matrix
            InternalType* pOut2 = pOut->getAs<ArrayOf>()->insert(&pArg, _pSource);
            if(pOut != pOut2)
            {
                delete pOut;
            }

            return pOut2;
	    }
        
        ArrayOf<T>* extract(typed_list* _pArgs)
	    {
		    ArrayOf<T> *pOut    = NULL;
            int iDims           = _pArgs->size();
            typed_list pArg;

            int* piMaxDim       = new int[iDims];
            int* piCountDim     = new int[iDims];

            //evaluate each argument and replace by appropriate value and compute the count of combinations
            int iSeqCount = check_indexes_arguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
            if(iSeqCount == 0)
            {
                return create_empty_double()->getAs<Double>();
            }

            if(iDims < m_iDims)
            {
                for(int i = 0 ; i < iDims ; i++)
                {
                    int iDimToCheck = 0;
                    if(i == (iDims - 1))
                    {
                        iDimToCheck = GetVarMaxDim(this, i, iDims);
                    }
                    else
                    {
                        iDimToCheck = m_piDims[i];
                    }

                    if(piMaxDim[i] > iDimToCheck)
                    {
                        return NULL;
                    }
                }
            }
            else if(iDims < m_iDims)
            {
                for(int i = m_iDims ; i < iDims ; i++)
                {
                    if(piMaxDim[i] > 1)
                    {
                        return false;
                    }
                }
            }
            else
            {//pArg.size() == m_iDims
                //check MaxDim
                for(int i = 0 ; i < m_iDims ; i++)
                {
                    if(piMaxDim[i] > m_piDims[i])
                    {//exrtact must be in dimension limits
                        return false;
                    }
                }
            }

            //remove last dimension if are == 1
            for(int i = (iDims - 1) ; i >= 2 ; i--)
            {
                if(piCountDim[i] == 1)
                {
                    (iDims)--;
                }
                else
                {
                    break;
                }
            }

            //vector
            if(iDims == 1)
            {
                if(piCountDim[0] == 0)
                {
                    pOut = create_empty_double()->getAs<Double>();
                }
                else
                {
                    //two cases, depends of original matrix/vector
                    if((*_pArgs)[0]->getAs<Colon>() == NULL && m_iDims == 2 && m_piDims[1] != 1)
                    {//special case for row vector
                        int piRealDim[2] = {1, piCountDim[0]};
                        pOut = create_empty(2, piRealDim, isComplex());
                    }
                    else
                    {
                        int piRealDim[2] = {piCountDim[0], 1};
                        pOut = create_empty(2, piRealDim, isComplex());
                    }
                }
            }
            else
            {//matrix
                pOut = create_empty(iDims, piCountDim, isComplex());
            }

            T* pReal        = pOut->get();
            T* pImg         = pOut->get_img();
            int* piIndex    = new int[_pArgs->size()];
            int* piCoord    = new int[_pArgs->size()];
            int* piViewDims = new int[iDims];
            memset(piIndex, 0x00, sizeof(int) * _pArgs->size());

            for(int i = 0 ; i < iDims ; i++)
            {
                piViewDims[i] = GetVarMaxDim(this, i, iDims);
            }

            for(int i = 0 ; i < iSeqCount ; i++)
            {
                //increment last dimension
                compute_tuples(piCountDim, _pArgs->size(), _pArgs->size() - 1, piIndex);

                //std::cout << "[";
                for(int j = 0 ; j < (int)_pArgs->size() ; j++)
                {
                    piCoord[j] = static_cast<int>(pArg[j]->getAs<ArrayOf>()->get(piIndex[j]) - 1);
                    //std::cout << piCoord[j] << " ";
                }

                //std::cout << "]" << std::endl;

                int iPos = 0;
                //put vlaue in the new matrix
                if((int)_pArgs->size() < m_iDims)
                {//compute index based on viewed matrix
                    iPos = get_index_with_dims(piCoord, piViewDims, iDims);
                }
                else
                {//compute vector index
                    iPos = get_index(piCoord);
                }

                //convert flat dimension to 0
                for(int j = 0 ; j < iDims ; j++)
                {
                    if(piCountDim[j] == 1)
                    {
                        piCoord[j] = 0;
                    }
                }

                //int iNewPos = get_index_with_dims(piCoord, piCountDim, iDims);

                pReal[i] = get(iPos);
                if(isComplex())
                {
                    pImg[i] = get_img(iPos);
                }


                piIndex[0]++;
            }

            //free allocated data
            for(int i = 0 ; i < iDims ; i++)
            {
                if(pArg[i] != (*_pArgs)[i])
                {
                    delete pArg[i];
                }
            }

            delete[] piIndex;
            delete[] piCoord;
            delete[] piViewDims;
            delete[] piMaxDim;
            delete[] piCountDim;

            return pOut;
	    }
        
        bool resize(int _iNewRows, int _iNewCols)
        {
            int piDims[2] = {_iNewRows, _iNewCols};
            return resize(piDims, 2);
        }
        
        bool resize(int* _piDims, int _iDims)
        {
            if(_iDims == m_iDims)
            {
                bool bChange = false;
                for(int i = 0 ; i < _iDims ; i++)
                {
                    if(m_piDims[i] != _piDims[i])
                    {
                        bChange = true;
                        break;
                    }
                }

                if(bChange == false)
                {//nothing to do
                    return true;
                }
            }

            //alloc new data array
            T* pRealData = NULL;
            T* pImgData = NULL;

            int iNewSize = 0;
            if(isComplex())
            {
                iNewSize = get_max_size(_piDims, _iDims);
                if(m_iSizeMax < iNewSize)
                {
                    //alloc 10% bigger than asked to prevent future resize
                    m_iSizeMax = static_cast<int>(iNewSize * 1.1);
                    pRealData = new T[m_iSizeMax];
                    pImgData = new T[m_iSizeMax];

                    //set value to (null) value
                    for(int i = 0 ; i < m_iSizeMax ; i++)
                    {
                        pRealData[i] = get_null_value();
                        pImgData[i] = get_null_value();
                    }

                    //copy values into new one
                    int* piIndexes = new int[m_iDims];
                    for(int i = 0 ; i < m_iSize ; i++)
                    {
                        get_indexes(i, piIndexes);
                        int iNewIdx = get_index_with_dims(piIndexes, _piDims, _iDims);
                        pRealData[iNewIdx] = m_pRealData[i];
                        pImgData[iNewIdx] = m_pImgData[i];
                    }

                    delete[] piIndexes;
                    //delete all array
                    delete[] m_pRealData;
                    delete[] m_pImgData;
                    //replace old array by new one
                    m_pRealData	= pRealData;
                    m_pImgData	= pImgData;
                }
                else
                {
                    //check if only the last dims change
                    bool bNonLastDimChange = false;
                    for(int i = 0 ; i < (m_iDims - 1) ; i++)
                    {
                        if(m_piDims[i] != _piDims[i])
                        {
                            bNonLastDimChange = true;
                            break;
                        }
                    }

                    //if vector or if row dimension not change, we don't need to shift data
                    if(m_iDims != _iDims || (!isVector() && bNonLastDimChange))
                    {
                        //copy values into new one
                        int* piIndexes = new int[m_iDims];
                        for(int i = m_iSize - 1 ; i >= 0  ; i--)
                        {
                            get_indexes(i, piIndexes);
                            int iNewIdx = get_index_with_dims(piIndexes, _piDims, _iDims);
                            m_pRealData[iNewIdx] = m_pRealData[i];
                            m_pImgData[iNewIdx] = m_pImgData[i];
                            if(iNewIdx != i)
                            {
                                m_pRealData[i] = 0;
                                m_pImgData[i] = 0;
                            }
                        }
                        delete[] piIndexes;
                    }
                }
            }
            else
            {
                iNewSize = get_max_size(_piDims, _iDims);
                if(m_iSizeMax < iNewSize)
                {
                    //alloc 10% bigger than asked to prevent future resize
                    m_iSizeMax = static_cast<int>(iNewSize * 1.1);
                    pRealData = new T[m_iSizeMax];

                    //set value to (null) value
                    for(int i = 0 ; i < m_iSizeMax ; i++)
                    {
                        pRealData[i] = get_null_value();
                    }

                    //copy values into new one
                    int* piIndexes = new int[_iDims];
                    for(int i = 0 ; i < _iDims ; i++)
                    {
                        piIndexes[i] = 0;
                    }

                    for(int i = 0 ; i < m_iSize ; i++)
                    {
                        get_indexes(i, piIndexes);
                        int iNewIdx = get_index_with_dims(piIndexes, _piDims, _iDims);
                        pRealData[iNewIdx] = m_pRealData[i];
                    }

                    delete[] piIndexes;
                    //delete all array
                    delete[] m_pRealData;
                    //replace old array by new one
                    m_pRealData	= pRealData;
                }
                else
                {
                    //check if only the last dims change
                    bool bNonLastDimChange = false;
                    for(int i = 0 ; i < (m_iDims - 1) ; i++)
                    {
                        if(m_piDims[i] != _piDims[i])
                        {
                            bNonLastDimChange = true;
                            break;
                        }
                    }

                    //if vector or if row dimension not change, we don't need to shift data
                    if(m_iDims != _iDims || (!isVector() && bNonLastDimChange))
                    {
                        //copy values into new one
                        int* piIndexes = new int[m_iDims];
                        for(int i = m_iSize - 1 ; i >= 0  ; i--)
                        {
                            get_indexes(i, piIndexes);
                            int iNewIdx = get_index_with_dims(piIndexes, _piDims, _iDims);
                            m_pRealData[iNewIdx] = m_pRealData[i];
                            if(iNewIdx != i)
                            {
                                m_pRealData[i] = 0;
                            }
                        }
                        delete[] piIndexes;
                    }
                }
            }

            if(_iDims != m_iDims)
            {
                int* piDims = new int[_iDims];
                for(int i = 0 ; i < _iDims ; i++)
                {
                    piDims[i] = _piDims[i];
                }
                delete[] m_piDims;
                m_piDims = piDims;
                m_iDims = _iDims;
            }
            else
            {
                for(int i = 0 ; i < m_iDims ; i++)
                {
                    m_piDims[i] = _piDims[i];
                }
            }
            m_iRows = m_piDims[0];
            m_iCols	= m_piDims[1];
            m_iSize = iNewSize;
            return true;
        }
        
        /* ArrayOf */
        template <typename V>
        V*                      getAs(void) {return dynamic_cast<V*>(this);}

        /*dimensions functions*/
        int get_index(int* _piIndexes)
        {
            int idx = 0;
            int iMult = 1;
            for(int i = 0 ; i < m_iDims ; i++)
            {
                idx += _piIndexes[i] * iMult;
                iMult *= m_piDims[i];
            }
            return idx;
        }
        
        int get_index_with_dims(int* _piIndexes, int* _piDims, int _iDims)
        {
            int idx = 0;
            int iMult = 1;
            for(int i = 0 ; i < _iDims ; i++)
            {
                idx += _piIndexes[i] * iMult;
                iMult *= _piDims[i];
            }
            return idx;
        }
        
        void get_indexes(int _iIndex, int* _piIndexes)
        {
            get_indexes_with_dims(_iIndex, _piIndexes, m_piDims, m_iDims);
        }

        void get_indexes_with_dims(int _iIndex, int* _piIndexes, int* _piDims, int _iDims)
        {
            int iMul = 1;
            for(int i = 0 ; i < _iDims ; i++)
            {
                _piIndexes[i] = (int)(_iIndex / iMul) % _piDims[i];
                iMul *= _piDims[i];
            }
            //matrix [2,4,3]
            //index = 12 ( 0,2,1) = 1 * 4 * 2 + 2 * 2 + 0 = 12
            //loop 1
            // (12 / 1) % 2 -> 0
            //loop 2
            // (12 / 2) % 4 -> 2
            //loop 3
            // (12 / 8) % 3 -> 1

            //matrix [3,4,3]
            //index = 22 
            //loop 1
            // (22 / 1) % 3 -> 1
            //loop 2
            // (22 / 3) % 4 -> 3
            //loop 3
            // (22 / 12) % 3 -> 1

            //matrix [3,4,3]
            //index = 35 
            //loop 1
            // (35 / 1) % 3 -> 2
            //loop 2
            // (35 / 3) % 4 -> 3
            //loop 3
            // (35 / 12) % 3 -> 2
        }

        ArrayOf<T>* get_col_value(int _iPos)
	    {
		    ArrayOf<T>* pOut = NULL;
		    if(_iPos < m_iCols)
		    {
                int piDims[2] = {m_iRows, 1};
                pOut = create_empty(2, piDims, isComplex());
                T* pReal    = pOut->get();
                T* pImg     = pOut->get_img();
                for(int i = 0 ; i < m_iRows ; i++)
                {
                    pReal[i] = get(i, _iPos);
                }

                if(isComplex())
			    {
                    for(int i = 0 ; i < m_iRows ; i++)
                    {
                        pImg[i] = get_img(i, _iPos);
                    }
			    }
		    }
		    return pOut;
	    }	


    };

}

#include "double.hxx"
#include "implicitlist.hxx"
#include "matrixpoly.hxx"
#include "bool.hxx"
#include "colon.hxx"
#include "string.hxx"
#endif /* !__ARRAYOF_HXX__ */
