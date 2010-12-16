/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/**
 ** \file symbol.hh
 ** \brief Define class Symbol.
 */

#ifndef SYMBOL_HH
#define SYMBOL_HH

#include <set>
#include <list>
#include <string>
#include <iostream>

#include <map>

#define UNORDERED_SYMBOLS_MAP

#ifdef UNORDERED_SYMBOLS_MAP
#include <tr1/unordered_map>
#endif

#define INTERN_NAMES

#ifdef INTERN_NAMES
#include <boost/flyweight.hpp>
#include <memory>
#endif

#ifdef UNORDERED_SYMBOLS_MAP
#ifdef INTERN_NAMES
namespace std{
    namespace tr1 {
        template<typename T>
        struct hash< ::boost::flyweights::flyweight<T> > : std::unary_function< ::boost::flyweights::flyweight<T>, std::size_t> {
            std::size_t operator()(::boost::flyweights::flyweight<T> const& fw) const { return reinterpret_cast<std::size_t>(&fw.get());}
        };
    }
}
#endif
#endif

namespace types { class InternalType; };
#include "export_symbol.h"

namespace symbol
{


#ifdef INTERN_NAMES
typedef ::boost::flyweights::flyweight<std::wstring> symbol_t;

template<typename Fw>
struct comp_fw : std::binary_function<Fw, Fw, bool>{
    bool operator()(Fw const& a1, Fw const& a2) const { return &a1.get() < &a2.get() ;}
};
typedef comp_fw<symbol_t> comp_t;

#else
typedef std::wstring symbol_t;
typedef std::less<symbol_t> comp_t;
#endif



#ifdef UNORDERED_SYMBOLS_MAP
    typedef std::tr1::unordered_map<symbol_t, types::InternalType*> map_t;
#else
    typedef std::map<symbol_t, types::InternalType*, comp_t> map_t;
#endif


  /** \brief Define class Symbol.
   **
   ** Map any wstring to a unique reference.
   ** This allows to avoid an "strcmp()" style comparison of strings:
   ** reference comparison is much faster.
   */
  class EXTERN_SYMBOL Symbol
  {
    /** \brief Define the type "set of wstring". */
    typedef std::set < std::wstring > string_set_type;
    /** \brief Define the type used for the size of wstring set. */
    typedef string_set_type::size_type size_type;

    /** \name Ctor & Dtor.
     ** \{ */
  public:
    /** \brief Construct a Symbol (explicit).
     ** \param s referenced wstring */
    explicit Symbol (const std::wstring &s);
    /** \} */


    /** \name Accessors.
     ** \{ */
  public:
    /** \brief Return the wstring referenced by this Symbol. */
    const std::wstring& name_get () const;
    /** \brief Return the number of referenced strings. */
    static size_type map_size ();
    /** \} */


    /** \name Operators.
     ** \{ */
  public:
    /** \brief Compare two Symbol for equality.
     ** \param rhs Symbol to compare with. */
    bool operator== (const Symbol &rhs) const;
    /** \brief Compare two Symbol for inequality.
     ** \param rhs Symbol to compare with. */
    bool operator!= (const Symbol &rhs) const;
    /** \brief Compare two Symbol for order.
     ** \param rhs Symbol to compare with. */
    bool operator<  (const Symbol &rhs) const;
    /** \} */


  private:
    /** \brief Static (global to all instance of Symbol) strings container. */
    static string_set_type _set;
    /** \brief Pointer to the node containing the unique referenced wstring. */
    const string_set_type::const_iterator _set_node;
  };

  /** \brief Intercept output stream redirection.
   ** \param ostr the destination output stream
   ** \param the a reference to the symbol to redirect
   */
  EXTERN_SYMBOL std::ostream& operator<< (std::ostream &ostr, const Symbol &the);

  /** Typedef for the list of Symbol */
  typedef std::list<const Symbol*> symbols_t;
}

#endif // !SYMBOL_HH
