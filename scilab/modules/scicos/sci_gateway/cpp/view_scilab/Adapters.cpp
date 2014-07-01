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

#include <algorithm>

#include "Adapters.hxx"

#include "BlockAdapter.hxx"
#include "GraphicsAdapter.hxx"
#include "ModelAdapter.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

Adapters Adapters::m_instance = Adapters();

Adapters::Adapters()
{

    /*
     * Create the supported data types list
     */
    adapters = adapters_t();
    insert_adapter_klass(view_scilab::BlockAdapter::getSharedTypeStr(), BLOCK_ADAPTER);
    insert_adapter_klass(view_scilab::GraphicsAdapter::getSharedTypeStr(), GRAPHIC_ADAPTER);
    insert_adapter_klass(view_scilab::ModelAdapter::getSharedTypeStr(), MODEL_ADAPTER);
}

Adapters::~Adapters()
{

}

void Adapters::insert_adapter_klass(const std::wstring& n, adapters_index_t kind)
{
    adapters_t::iterator it = std::lower_bound(adapters.begin(), adapters.end(), n);
    adapters.insert(it, adapter_t(n, kind));
};

Adapters::adapters_index_t Adapters::lookup_by_typename(const std::wstring& name)
{
    adapters_t::iterator it = std::lower_bound(adapters.begin(), adapters.end(), name);
    if (it != adapters.end())
    {
        return it->kind;
    }
    return INVALID_ADAPTER;
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
