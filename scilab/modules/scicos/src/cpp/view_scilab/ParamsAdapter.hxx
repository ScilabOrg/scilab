/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
 *
 */

#ifndef PARAMSADAPTER_HXX_
#define PARAMSADAPTER_HXX_

#include <string>

#include "internal.hxx"

#include "BaseAdapter.hxx"
#include "model/Diagram.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

class ParamsAdapter : public BaseAdapter<ParamsAdapter, org_scilab_modules_scicos::model::Diagram>
{
public:
    ParamsAdapter();
    ParamsAdapter(const Controller& c, org_scilab_modules_scicos::model::Diagram* adaptee);
    ~ParamsAdapter();

    static const std::wstring getSharedTypeStr()
    {
        return L"params";
    }

    std::wstring getTypeStr();
    std::wstring getShortTypeStr();

    types::InternalType* getDocContent() const;
    void setDocContent(types::InternalType* v);

private:
    types::InternalType* doc_content;
};

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* PARAMSADAPTER_HXX_ */
