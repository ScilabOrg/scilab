/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef AST_TRANSPOSEEXP_HXX
#define AST_TRANSPOSEEXP_HXX

#include "mathexp.hxx"

namespace ast
{
/** \brief Abstract a Transpose Expression node.
**
** \b Example: ~a */
class TransposeExp : public MathExp
{
public:
    /* \brief Conjugate or not transposition */
    enum Kind
    {
        /** \brief Conjugate transpose */ _Conjugate_,
        /** \brief NonConjugate transpose */ _NonConjugate_
    };
    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct an Transpose Expression node.
    ** \param location scanner position informations
    ** \param exp expression of the Transpose operator
    ** \param kind indicate if should be conjugated or not
    **
    ** \b Example: a'
    */
    TransposeExp (const Location& location,
                  Exp& exp,
                  Kind kind)
        : MathExp (location),
          _conjugate (kind)
    {
        exp.setParent(this);
        _exps[0] = &exp;
    }

    /** \brief Destroy a Transpose Operation Expression node.
    **
    ** Delete expression, see constructor. */
    virtual ~TransposeExp ()
    {
        delete _exps[0];
    }
    /** \} */

    virtual TransposeExp* clone()
    {
        TransposeExp* cloned = new TransposeExp(getLocation(), *getExp().clone(), getConjugate());
        cloned->setVerbose(isVerbose());
        return cloned;
    }

    /** \name Visitors entry point.
    ** \{ */
public:
    /** \brief Accept a const visitor \a v. */
    virtual void accept (Visitor& v)
    {
        v.visit (*this);
    }
    /** \brief Accept a non-const visitor \a v. */
    virtual void accept (ConstVisitor& v) const
    {
        v.visit (*this);
    }
    /** \} */


    /** \name Accessors.
    ** \{ */
public:
    /** \brief Return the expression of the operation (read only) */
    const Exp& getExp() const
    {
        return *_exps[0];
    }
    /** \brief Return the expression of the operation (read and write) */
    Exp& getExp()
    {
        return *_exps[0];
    }
    /** \brief Return the conjugate kind of the transposition */
    Kind getConjugate() const
    {
        return _conjugate;
    }

    virtual ExpType getType()
    {
        return TRANSPOSEEXP;
    }
    inline bool isTransposeExp() const
    {
        return true;
    }
protected:
    Kind _conjugate;
};

} // namespace ast

#endif // !AST_TRANSPOSEEXP_HXX
