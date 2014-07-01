/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef MODELADAPTER_HXX_
#define MODELADAPTER_HXX_

#include <sstream>

#include "BaseAdapter.hxx"

#include "model/Block.hxx"

namespace org_scilab_modules_xcos
{
namespace view_scilab
{

class ModelAdapter : public BaseAdapter<ModelAdapter, org_scilab_modules_xcos::model::Block>
{
public:
    ModelAdapter(const ModelAdapter& o);
    ModelAdapter(org_scilab_modules_xcos::model::Block* o);
    ~ModelAdapter();

    bool toString(std::wostringstream& ostr)
    {
        ostr << L"ModelAdapter.hxx: Dunno what to display there" << std::endl;
        return true;
    }

    std::wstring getTypeStr()
    {
        return getSharedTypeStr();
    }
    std::wstring getShortTypeStr()
    {
        return getSharedTypeStr();
    }

    static const std::wstring getSharedTypeStr()
    {
        return L"model";
    }

};

} /* view_scilab */
} /* namespace org_scilab_modules_xcos */

#endif /* MODELADAPTER_HXX_ */
