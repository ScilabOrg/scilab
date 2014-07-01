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

#ifndef BLOCKADAPTER_HXX_
#define BLOCKADAPTER_HXX_

#include <sstream>
#include <string>
#include <vector>

#include "BaseAdapter.hxx"
#include "model/Block.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

class BlockAdapter : public BaseAdapter<BlockAdapter, org_scilab_modules_scicos::model::Block>
{
public:
    BlockAdapter(const BlockAdapter& o);
    BlockAdapter(org_scilab_modules_scicos::model::Block* o);
    ~BlockAdapter();

    bool toString(std::wostringstream& ostr)
    {
        ostr << L"BlockAdapter.hxx: Dunno what to display there" ;
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
        return L"Block";
    }

    inline types::InternalType* get_doc_content() const
    {
        return doc_content;
    };
    inline void set_doc_content(types::InternalType* v)
    {
        delete doc_content;
        doc_content = v->clone();
    };

private:
    types::InternalType* doc_content;
};

} /* view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* BLOCKADAPTER_HXX_ */
