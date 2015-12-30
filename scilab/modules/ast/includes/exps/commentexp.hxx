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

#ifndef AST_COMMENTEXP_HXX
#define AST_COMMENTEXP_HXX

#include "constexp.hxx"

namespace ast
{
/** \brief Abstract an string Expression node.
**
** \b Example: string*/
class CommentExp : public ConstExp
{
public:
    CommentExp (const Location& location,
                std::string* comment)
        : ConstExp (location),
          _comment (comment)
    {
    }
    /** \brief Destroy an string Exp node.
    **
    ** Delete value (see constructor). */
    virtual ~CommentExp ()
    {
        delete _comment;
    }
    /** \} */

    virtual CommentExp* clone()
    {
        CommentExp* cloned = new CommentExp(getLocation(), new std::string(getComment()));
        cloned->setVerbose(isVerbose());
        return cloned;
    }

    virtual bool equal(const Exp & e) const
    {
        return e.getType() == COMMENTEXP;
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
    /** \brief Return the comment (read only). */
    std::string &getComment() const
    {
        return *_comment;
    }
    /** \} */

    virtual ExpType getType() const
    {
        return COMMENTEXP;
    }
    inline bool isCommentExp() const
    {
        return true;
    }
protected:
    std::string* _comment;
};

} // namespace ast

#endif // !AST_COMMENTEXP_HXX
