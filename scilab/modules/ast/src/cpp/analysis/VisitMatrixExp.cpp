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

#include "AnalysisVisitor.hxx"
#include "tools.hxx"

namespace analysis
{
    void AnalysisVisitor::visit(ast::MatrixExp & e)
    {
        const ast::exps_t & lines = e.getLines();
        if (lines.empty())
        {
            // empty matrix
            TIType type(getGVN(), TIType::EMPTY);
            e.getDecorator().res = Result(type);
            setResult(e.getDecorator().res);
            return;
        }

        GVN::Value * totalColsRef = nullptr;
        GVN::Value * totalRows = getGVN().getValue(0.);
        TIType::Type baseType = TIType::UNKNOWN;
        bool baseTypeSet = false;
        bool checkDims = true;

        for (auto line : lines)
        {
            const ast::exps_t & columns = static_cast<ast::MatrixLineExp *>(line)->getColumns();
            if (!columns.empty())
            {
                GVN::Value * totalCols = nullptr;
                GVN::Value * rows = nullptr;
                for (auto e : columns)
                {
                    e->accept(*this);
                    Result & res = getResult();
                    TIType & type = res.getType();
		    if (type.type != TIType::EMPTY && type.rows != 0 && type.cols != 0)
                    {		     
                        if (!baseTypeSet)
                        {
                            baseTypeSet = true;
                            baseType = type.type;
                        }
                        else
                        {
                            switch (baseType)
                            {
                            case TIType::DOUBLE:
                                switch (type.type)
                                {
                                case TIType::COMPLEX:
                                    baseType = TIType::COMPLEX;
                                    break;
                                case TIType::DOUBLE:
                                case TIType::BOOLEAN:
                                    break;
                                default:
                                    baseType = TIType::UNKNOWN;
                                }
                                break;
                            case TIType::COMPLEX:
                                switch (type.type)
                                {
                                case TIType::DOUBLE:
                                case TIType::COMPLEX:
                                case TIType::BOOLEAN:
                                    break;
                                default:
                                    baseType = TIType::UNKNOWN;
                                }
                                break;
                            case TIType::BOOLEAN:
                                switch (type.type)
                                {
                                case TIType::DOUBLE:
                                case TIType::COMPLEX:
                                    baseType = type.type;
                                    break;
                                case TIType::BOOLEAN:
                                    break;
                                default:
                                    baseType = TIType::UNKNOWN;
                                }
                                break;
                            default:
                                if (baseType != type.type)
                                {
                                    baseType = TIType::UNKNOWN;
                                }
                            }
                        }

                        if (checkDims)
                        {
                            if (!rows)
                            {
                                rows = type.rows.getValue();
                            }
                            else
                            {
				if (!getCM().check(ConstraintManager::EQUAL, rows, type.rows.getValue()))
				{
				    checkDims = false;
				}
                            }
			    if (!totalCols)
			    {
				totalCols = type.cols.getValue();
			    }
			    else
			    {
				totalCols = getGVN().getValue(OpValue::Kind::PLUS, *totalCols, *type.cols.getValue());
			    }
			}
                    }
                }

                if (checkDims && totalCols)
                {
                    if (!totalColsRef)
                    {
                        totalColsRef = totalCols;
                    }
                    else
                    {
			if (!getCM().check(ConstraintManager::EQUAL, totalCols, totalColsRef))
			{
			    checkDims = false;
			}
                    }

                    if (rows)
                    {
                        totalRows = getGVN().getValue(OpValue::Kind::PLUS, *totalRows, *rows);
                    }
                }
            }
        }

	if (checkDims)
	{
	    if (!totalRows || !totalColsRef || totalRows->poly->isConstant(0) || totalColsRef->poly->isConstant(0))
	    {
		TIType type(getGVN(), TIType::EMPTY);
		e.getDecorator().res = Result(type);
	    }
	    else
	    {
		TIType type(getGVN(), baseType, SymbolicDimension(getGVN(), totalRows), SymbolicDimension(getGVN(), totalColsRef));
		e.getDecorator().res = Result(type);
	    }
	}
	else
	{
	    TIType type(getGVN(), baseType, false, true);
	    e.getDecorator().res = Result(type);
	}
	
        setResult(e.getDecorator().res);
    }
}
