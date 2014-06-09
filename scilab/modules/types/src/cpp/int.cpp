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

#include "int.hxx"

namespace types
{
    // Specializations
    template<>
    std::wstring Int<char>::getTypeStr()
    {
	return L"int8";
    }

    template<>
    std::wstring Int<short>::getTypeStr()
    {
	return L"int16";
    }

    template<>
    std::wstring Int<int>::getTypeStr()
    {
	return L"int32";
    }

    template<>
    std::wstring Int<long long>::getTypeStr()
    {
	return L"int64";
    }

    template<>
    std::wstring Int<unsigned char>::getTypeStr()
    {
	return L"uint8";
    }

    template<>
    std::wstring Int<unsigned short>::getTypeStr()
    {
	return L"uint16";
    }

    template<>
    std::wstring Int<unsigned int>::getTypeStr()
    {
	return L"uint32";
    }

    template<>
    std::wstring Int<unsigned long long>::getTypeStr()
    {
	return L"uint64";
    }

    template<>
    void Int<char>::whoAmI()
    {
	std::cout << "types::Int8";
    }

    template<>
    void Int<short>::whoAmI()
    {
	std::cout << "types::Int16";
    }

    template<>
    void Int<int>::whoAmI()
    {
	std::cout << "types::Int32";
    }

    template<>
    void Int<long long>::whoAmI()
    {
	std::cout << "types::Int64";
    }

    template<>
    void Int<unsigned char>::whoAmI()
    {
	std::cout << "types::UInt8";
    }

    template<>
    void Int<unsigned short>::whoAmI()
    {
	std::cout << "types::UInt16";
    }

    template<>
    void Int<unsigned int>::whoAmI()
    {
	std::cout << "types::UInt32";
    }

    template<>
    void Int<unsigned long long>::whoAmI()
    {
	std::cout << "types::UInt64";
    }

    template<>
    InternalType::RealType Int<char>::getType()
    {
	return InternalType::RealInt8;
    }

    template<>
    InternalType::RealType Int<short>::getType()
    {
	return InternalType::RealInt16;
    }

    template<>
    InternalType::RealType Int<int>::getType()
    {
	return InternalType::RealInt32;
    }

    template<>
    InternalType::RealType Int<long long>::getType()
    {
	return InternalType::RealInt64;
    }

    template<>
    InternalType::RealType Int<unsigned char>::getType()
    {
	return InternalType::RealUInt8;
    }

    template<>
    InternalType::RealType Int<unsigned short>::getType()
    {
	return InternalType::RealUInt16;
    }

    template<>
    InternalType::RealType Int<unsigned int>::getType()
    {
	return InternalType::RealUInt32;
    }

    template<>
    InternalType::RealType Int<unsigned long long>::getType()
    {
	return InternalType::RealUInt64;
    }
}
