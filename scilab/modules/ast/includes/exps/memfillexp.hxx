/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __MEMFILL_EXP_HXX__
#define __MEMFILL_EXP_HXX__

namespace ast
{
    /**
     * \class MemfillExp
     * \brief represent an exp like zeros or ones or anything else...
     * 
     * The expression %pi+zeros(...) is equivalent to fill the matrix with %pi and obviously
     * it is fastest to fill with %pi rather than to fill with zero and then add %pi.
     * This expression comes from an analysis of such expressions.
     *
     */
    class MemfillExp : public OptimizedExp
    {

    public:

        MemfillExp(const Location & location, Exp & value, const exps_t & args) : OptimizedExp(location)
	    {
		_exps.push_back(&value);
		value.setParent(this);
		for (auto arg : args)
		{
		    _exps.push_back(arg);
		    arg->setParent(this);
		}
	    }

        virtual ~MemfillExp()
            {
            }

	virtual MemfillExp * clone() override
	    {
		exps_t args;
		args.reserve(_exps.size() - 1);
		for (exps_t::iterator i = std::next(_exps.begin()), end = _exps.end(); i != end; ++i)
		{
		    args.emplace_back((*i)->clone());
		}
		return new MemfillExp(getLocation(), *getValue().clone(), args);
	    }
	
        virtual void accept(Visitor& v) override
            {
                v.visit(*this);
            }

        virtual void accept(ConstVisitor& v) const override
            {
                v.visit(*this);
            }

	const Exp & getValue() const
	    {
		return *_exps[0];
	    }

	Exp & getValue()
	    {
		return *_exps[0];
	    }

	exps_t getArgs() const
	    {
		return exps_t(std::next(_exps.begin()), _exps.end());
	    }

	exps_t getArgs()
	    {
		return exps_t(std::next(_exps.begin()), _exps.end());
	    }
	
        inline bool isMemfillExp() const
            {
                return true;
            }

	virtual ExpType getType() const
	    {
		return MEMFILLEXP;
	    }
    };

} // namespace ast

#endif // __MEMFILL_EXP_HXX__
