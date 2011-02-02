/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include <sstream>
#include <math.h>
#include "arrayof.hxx"

#include "core_math.h"
#include "tostring_common.hxx"

extern "C"
{
    #include "elem_common.h"
}

std::wstring printInLinePoly(types::Poly* _pPoly, std::wstring _stVar, int _iPrecision, int _iLineLen);
std::wstring printDouble(types::Double* _pD, int _iPrecision, int _iLineLen);
long long convert_input(types::InternalType* _poIT);
unsigned long long convert_unsigned_input(types::InternalType* _poIT);

namespace types
{
    ImplicitList::~ImplicitList()
    {
        if(isDeletable() == true)
        {
            if(m_poStart)
            {
                m_poStart->DecreaseRef();
                if(m_poStart->isDeletable())
                {
                    delete m_poStart;
                }
            }

            if(m_poStep)
            {
                m_poStep->DecreaseRef();
                if(m_poStep->isDeletable())
                {
                    delete m_poStep;
                }
            }

            if(m_poEnd)
            {
                m_poEnd->DecreaseRef();
                if(m_poEnd->isDeletable())
                {
                    delete m_poEnd;
                }
            }
        }
    }

    ImplicitList::ImplicitList()
    {
        m_bComputed = false;
    }

    ImplicitList::ImplicitList(InternalType* _poStart, InternalType* _poStep, InternalType* _poEnd)
    {
        m_iSize = -1;
        m_eOutSubType = Int::Type8;
        m_eOutType = RealGeneric;
        setStart(_poStart);
        setStep(_poStep);
        setEnd(_poEnd);
        compute();
    }

    InternalType* ImplicitList::clone()
    {
        return new ImplicitList(m_poStart, m_poStep, m_poEnd);
    }

    InternalType::RealType ImplicitList::getStartType()
    {
        return m_poStart->getType();
    }

    InternalType::RealType ImplicitList::getStepType()
    {
        return m_poStep->getType();
    }

    InternalType::RealType ImplicitList::getEndType()
    {
        return m_poEnd->getType();
    }

    InternalType* ImplicitList::getStart()
    {
        return m_poStart;
    }

    void ImplicitList::setStart(InternalType *_poIT)
    {
        _poIT->IncreaseRef();
        m_poStart = _poIT;
        if(m_poStart != NULL)
        {
            m_eStartType = m_poStart->getType();
        }
        m_bComputed = false;
    }

    InternalType* ImplicitList::getStep()
    {
        return m_poStep;
    }

    void ImplicitList::setStep(InternalType *_poIT)
    {
        _poIT->IncreaseRef();
        m_poStep = _poIT;
        if(m_poStep != NULL)
        {
            m_eStepType = m_poStep->getType();
        }
        m_bComputed = false;
    }

    InternalType* ImplicitList::getEnd()
    {
        return m_poEnd;
    }

    void ImplicitList::setEnd(InternalType* _poIT)
    {
        _poIT->IncreaseRef();
        m_poEnd = _poIT;
        if(m_poEnd != NULL)
        {
            m_eEndType = m_poEnd->getType();
        }
        m_bComputed = false;
    }

    long long ImplicitList::getSize()
    {
        return m_iSize;
    }

    bool ImplicitList::compute()
    {
        if(m_bComputed == true)
        {
            return true;
        }

        m_iSize = -1;
        if(isComputable() == true)
        {
            m_iSize = 0;
            if(m_eOutType == RealDouble)
            {
                double dblStart	= m_poStart->getAs<Double>()->getReal(0,0);
                double dblStep	= m_poStep->getAs<Double>()->getReal(0,0);
                double dblEnd   = m_poEnd->getAs<Double>()->getReal(0,0);

                if(dblStep > 0)
                {
                    m_iSize = static_cast<long long>(floor((dblEnd - dblStart) / dblStep)) + 1;
                }
                else if(dblStep < 0)
                {
                    m_iSize = static_cast<long long>(floor((dblStart - dblEnd) / -dblStep)) + 1;
                }
                else
                {
                    m_iSize = 0;
                }

                if(m_iSize < 0)
                {
                    m_iSize = 0;
                }
//                m_iSize = static_cast<long long>(floor(fabs(dblEnd - dblStart) / fabs(dblStep))) + 1;
            }
            else //m_eOutType == RealInt
            {
                if(m_eOutSubType > Int::Type64)//Unsigned
                {
                    unsigned long long ullStart = convert_unsigned_input(m_poStart);
                    unsigned long long ullStep	= convert_unsigned_input(m_poStep);
                    unsigned long long ullEnd   = convert_unsigned_input(m_poEnd);

#ifdef _MSC_VER
                    m_iSize = static_cast<long long>(floor(static_cast<double>(_abs64(ullEnd - ullStart) / _abs64(ullStep)) )) + 1;
#else
                    m_iSize = static_cast<long long>(floor(static_cast<double>(llabs(ullEnd - ullStart) / llabs(ullStep)) )) + 1;
#endif
                }
                else //Signed
                {
                    long long llStart   = convert_input(m_poStart);
                    long long llStep    = convert_input(m_poStep);
                    long long llEnd     = convert_input(m_poEnd);

#ifdef _MSC_VER
                    m_iSize = static_cast<long long>(floor( static_cast<double>(_abs64(llEnd - llStart) / _abs64(llStep)) )) + 1;
#else
                    m_iSize = static_cast<long long>(floor( static_cast<double>(llabs(llEnd - llStart) / llabs(llStep)) )) + 1;
#endif
                }
            }
            m_bComputed = true;
            return true;
        }
        else
        {
            return false;
        }
    }

    bool ImplicitList::isComputable()
    {
        if(m_eStartType != RealDouble && m_eStartType != RealInt)
        {
            return false;
        }

        if(m_eStepType != RealDouble && m_eStepType != RealInt)
        {
            return false;
        }

        if(m_eEndType != RealDouble && m_eEndType != RealInt)
        {
            return false;
        }

        //"compute" output type
        m_eOutType = RealGeneric; //not defined type
        if(m_eStartType == RealInt)
        {
            m_eOutType      = RealInt;
            m_eOutSubType   = m_poStart->getAsInt()->getIntType();
        }
        else if(m_eStepType == RealInt)
        {
            m_eOutType      = RealInt;
            m_eOutSubType   = m_poEnd->getAsInt()->getIntType();
        }
        else if(m_eEndType == RealInt)
        {
            m_eOutType      = RealInt;
            m_eOutSubType   = m_poEnd->getAsInt()->getIntType();
        }
        else
        {
            m_eOutType      = RealDouble;
        }

        return true;
    }

    std::wstring ImplicitList::toString(int _iPrecision, int _iLineLen)
    {
        if(isComputable())
        {
            return extractFullMatrix()->toString(_iPrecision, _iLineLen);
        }
        else
        {
            std::wostringstream ostr;
            ostr << L" ";
            if(m_eStartType == RealDouble)
            {
                Double *pD = m_poStart->getAs<Double>();
                ostr << printDouble(pD, _iPrecision, _iLineLen);
            }
            else //MatrixPoly
            {
                MatrixPoly* pMP = m_poStart->getAsPoly();
                ostr << printInLinePoly(pMP->getPoly(0,0), pMP->getVariableName(), _iPrecision, _iLineLen);
            }

            ostr << L":";

            if(m_eStepType == RealDouble)
            {
                Double *pD = m_poStep->getAs<Double>();
                ostr << printDouble(pD, _iPrecision, _iLineLen);
            }
            else //MatrixPoly
            {
                MatrixPoly* pMP = m_poStep->getAsPoly();
                ostr << printInLinePoly(pMP->getPoly(0,0), pMP->getVariableName(), _iPrecision, _iLineLen);
            }

            ostr << L":";

            if(m_eEndType == RealDouble)
            {
                Double *pD = m_poEnd->getAs<Double>();
                ostr << printDouble(pD, _iPrecision, _iLineLen);
            }
            else //MatrixPoly
            {
                MatrixPoly* pMP = m_poEnd->getAsPoly();
                ostr << printInLinePoly(pMP->getPoly(0,0), pMP->getVariableName(), _iPrecision, _iLineLen);
            }
            ostr << std::endl;
            return ostr.str();
        }
    }

    InternalType::RealType ImplicitList::getOutputType()
    {
        return m_eOutType;
    }

    Int::IntType ImplicitList::getOutputSubType()
    {
        return m_eOutSubType;
    }

    double ImplicitList::extractValueInDouble(int _iOccur)
    {
        double dblStart		= m_poStart->getAs<Double>()->getReal(0,0);
        double dblStep		= m_poStep->getAs<Double>()->getReal(0,0);
        return dblStart + _iOccur * dblStep;
    }

    long long ImplicitList::extractValueInInteger(int _iOccur)
    {
        return convert_input(m_poStart) + _iOccur * convert_input(m_poStep);
    }

    //extract single value in a InternalType
    InternalType* ImplicitList::extractValue(int _iOccur)
    {
        InternalType* pIT = NULL;
        if(compute())
        {
            if(m_eOutType == RealInt)
            {
                Int *pI	= NULL;
                pI = Int::createInt(1,1, m_eOutSubType);
                pI->data_set(0,0, convert_input(m_poStart) + _iOccur * convert_input(m_poStep));
                pIT	= pI;
            }
            else //RealDouble
            {
                double dblStart		= m_poStart->getAs<Double>()->getReal(0,0);
                double dblStep		= m_poStep->getAs<Double>()->getReal(0,0);
                Double* pD				= new Double(dblStart + _iOccur * dblStep);
                pIT = pD;
            }
        }
        return pIT;
    }

    //extract matrix in a Internaltype
    InternalType* ImplicitList::extractFullMatrix()
    {
        InternalType* pIT = NULL;
        if(compute())
        {
            if(m_eOutType == RealInt)
            {
                Int *pI	= Int::createInt(1, static_cast<int>(m_iSize), m_eOutSubType);

                for(int i = 0 ; i < m_iSize ; i++)
                {
                    pI->data_set(i, convert_input(m_poStart) + convert_input(m_poStep) * i);
                }

                pIT	= pI;
            }
            else //RealDouble
            {
                Double* pD = new Double(1, static_cast<int>(m_iSize));
                extractFullMatrix(pD->getReal());
                pIT = pD;
            }
        }
        return pIT;
    }

    template<typename T>
    void ImplicitList::extractFullMatrix(T *_pT)
    {
        T tStart = static_cast<T>(convert_input(m_poStart));
        T tStep	= static_cast<T>(convert_input(m_poStep));

        for(int i = 0 ; i < m_iSize ; i++)
        {
            _pT[i] = tStart + tStep * i;
        }
    }
}

std::wstring printInLinePoly(types::Poly* _pPoly, std::wstring _stVar, int _iPrecision, int _iLineLen)
{
    std::wostringstream ostr;
    for(int i = 0 ; i < _pPoly->getRank() ; i++)
    {
        double dbl = _pPoly->getCoef()->getReal()[i];
        if(dbl != 0)
        {
            int iWidth = 0, iPrec = 0;
            bool bFP = false; // FloatingPoint
            getDoubleFormat(dbl, _iPrecision, &iWidth, &iPrec, &bFP);
            addDoubleValue(&ostr, dbl, iWidth, iPrec, ostr.str().size() != 0, i == 0, false);
            if(i != 0)
            {
                ostr <<_stVar;
                if(i > 1)
                {
                    ostr << "^" << i;
                }
            }
        }
    }
    return ostr.str();
}

std::wstring printDouble(types::Double* _pD, int _iPrecision, int _iLineLen)
{
    std::wostringstream ostr;
    int iWidth = 0, iPrec = 0;
    bool bFP = false; // FloatingPoint
    getDoubleFormat(_pD->getReal(0,0), _iPrecision, &iWidth, &iPrec, &bFP);
    addDoubleValue(&ostr, _pD->getReal(0,0), iWidth, iPrec, false, true, false);
    return ostr.str();
}

long long convert_input(types::InternalType* _poIT)
{
    long long llValue = 0;
    switch(_poIT->getType())
    {
    case types::GenericType::RealDouble :
        llValue = static_cast<long long>(_poIT->getAs<types::Double>()->getReal(0,0));
        break;
    case types::GenericType::RealInt :
        llValue = static_cast<long long>(_poIT->getAsInt()->data_get(0,0));
        break;
    default:
        // FIXME : Trigger an error ??
        break;
    }
    return llValue;
}

unsigned long long convert_unsigned_input(types::InternalType* _poIT)
{
    unsigned long long ullValue = 0;
    switch(_poIT->getType())
    {
    case types::GenericType::RealDouble :
        ullValue = static_cast<unsigned long long>(_poIT->getAs<types::Double>()->getReal(0,0));
        break;
    case types::GenericType::RealInt :
        ullValue = static_cast<unsigned long long>(_poIT->getAsInt()->data_get(0,0));
        break;
    default:
        // FIXME : Trigger an error ??
        break;
    }
    return ullValue;
}
