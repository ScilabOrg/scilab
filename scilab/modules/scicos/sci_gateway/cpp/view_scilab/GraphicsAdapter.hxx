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

#ifndef GRAPHICSADAPTER_HXX_
#define GRAPHICSADAPTER_HXX_

#include <sstream>
#include <string>
#include <vector>
#include <utility>

#include "BaseAdapter.hxx"
#include "model/Block.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

class GraphicsAdapter : public BaseAdapter<GraphicsAdapter, org_scilab_modules_scicos::model::Block>
{
public:
    GraphicsAdapter(const GraphicsAdapter& o);
    GraphicsAdapter(org_scilab_modules_scicos::model::Block* o);
    ~GraphicsAdapter();

    bool toString(std::wostringstream& ostr)
    {
        ostr << L"GraphicsAdapter.hxx: Dunno what to display there" << std::endl;
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
        return L"graphics";
    }
};

} /* view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* GRAPHICSADAPTER_HXX_ */
