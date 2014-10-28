/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef LINKADAPTER_HXX_
#define LINKADAPTER_HXX_

#include <string>

#include "BaseAdapter.hxx"

#include "model/Link.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

class LinkAdapter : public BaseAdapter<LinkAdapter, org_scilab_modules_scicos::model::Link>
{
public:
    LinkAdapter(std::shared_ptr<org_scilab_modules_scicos::model::Link> adaptee);
    LinkAdapter(const LinkAdapter& adapter);
    ~LinkAdapter();

    static const std::wstring getSharedTypeStr()
    {
        return L"Link";
    }

    std::wstring getTypeStr();
    std::wstring getShortTypeStr();

    std::vector<double> getFrom() const;
    bool setFrom(const std::vector<double>& v, Controller& controller, const bool model_level = true);
    std::vector<double> getTo() const;
    bool setTo(const std::vector<double>& v, Controller& controller, const bool model_level = true);

private:
    std::vector<double> from_content;
    std::vector<double> to_content;
};

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* MODELADAPTER_HXX_ */
