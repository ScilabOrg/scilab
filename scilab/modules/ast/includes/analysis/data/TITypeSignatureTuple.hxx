/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __TITYPE_SIGNATURE_TUPLE_HXX__
#define __TITYPE_SIGNATURE_TUPLE_HXX__

#include <vector>

#include "tools.hxx"
#include "TIType.hxx"

namespace analysis
{

struct TITypeSignature
{
    TIType::Type type;
    bool scalar;

    TITypeSignature(const TIType::Type _type, const bool _scalar) : type(_type), scalar(_scalar) { }

    friend std::wostream & operator<<(std::wostream & out, const TITypeSignature & signature)
    {
        out << L"[ty:" << TIType::toString(signature.type)
            << L", sc:" << (signature.scalar ? L"T" : L"F")
            << L"]";

        return out;
    }
};

struct TITypeSignatureTuple
{
    std::vector<TITypeSignature> types;

    TITypeSignatureTuple() { }

    TITypeSignatureTuple(const std::vector<TIType> & _types)
    {
        types.reserve(_types.size());
        for (const auto & type : _types)
        {
            types.emplace_back(type.type, type.isscalar());
        }
    }

    inline std::vector<TITypeSignature> & getTypes()
    {
        return types;
    }

    inline const std::vector<TITypeSignature> & getTypes() const
    {
        return types;
    }

    inline void emplace(const TIType & t)
    {
        types.emplace_back(t.type, t.isscalar());
    }

    inline std::size_t size() const
    {
        return types.size();
    }

    inline std::size_t hash() const
    {
        if (types.empty())
        {
            return 0;
        }
        else
        {
            std::size_t seed = 0;
            for (const auto & tts : types)
            {
                seed = tools::hash_combine(seed, tts.type, tts.scalar);
            }
            return seed;
        }
    }

    friend std::wostream & operator<<(std::wostream & out, const TITypeSignatureTuple & tuple)
    {
        tools::printSet(tuple.types, out);
        return out;
    }

    inline bool operator==(const TITypeSignatureTuple & R) const
    {
        if (types.size() == R.types.size())
        {
            for (std::vector<TITypeSignature>::const_iterator i = types.begin(), j = R.types.begin(), e = types.end(); i != e; ++i, ++j)
            {
                if (i->type != j->type || i->scalar != j->scalar)
                {
                    return false;
                }
            }
            return true;
        }

        return false;
    }

    struct Hash
    {
        inline std::size_t operator()(const TITypeSignatureTuple & tuple) const
        {
            return tuple.hash();
        }
    };

    struct Eq
    {
        inline bool operator()(const TITypeSignatureTuple & L, const TITypeSignatureTuple & R) const
        {
            return L == R;
        }
    };
};

} // namespace analysis

#endif // __TITYPE_SIGNATURE_TUPLE_HXX__
