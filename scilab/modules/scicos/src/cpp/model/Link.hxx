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

#ifndef LINK_HXX_
#define LINK_HXX_

#include <string>
#include <vector>

#include "Model.hxx"
#include "model/BaseObject.hxx"

namespace org_scilab_modules_scicos
{
namespace model
{

class Link: public BaseObject
{
    friend class ::org_scilab_modules_scicos::Model;

private:
    Link() : BaseObject(LINK), parentDiagram(0), sourcePort(0), destinationPort(0), controlPoints(),
        label(), thick(), ct() {};
    Link(const Link& o) : BaseObject(LINK), parentDiagram(o.parentDiagram), sourcePort(o.sourcePort), destinationPort(o.destinationPort),
        controlPoints(o.controlPoints), label(o.label), thick(o.thick), ct(o.ct) {};
    ~Link() {}

    ScicosID getParentDiagram() const
    {
        return parentDiagram;
    }

    void setParentDiagram(ScicosID parentDiagram)
    {
        this->parentDiagram = parentDiagram;
    }

    void getControlPoints(std::vector<double>& v) const
    {
        v = controlPoints;
    }

    update_status_t setControlPoints(const std::vector<double>& v)
    {
        if (v == controlPoints)
        {
            return NO_CHANGES;
        }

        controlPoints = v;
        return SUCCESS;
    }

    void getLabel(std::string& data) const
    {
        data = label;
    }

    update_status_t setLabel(const std::string& data)
    {
        if (data == label)
        {
            return NO_CHANGES;
        }

        label = data;
        return SUCCESS;
    }

    void getThick(std::vector<double>& v) const
    {
        v = thick;
    }

    update_status_t setThick(const std::vector<double>& v)
    {
        if (v.size() != 2)
        {
            return FAIL;
        }

        if (v == thick)
        {
            return NO_CHANGES;
        }

        thick = v;
        return SUCCESS;
    }

    void getCT(std::vector<int>& v) const
    {
        v = ct;
    }

    update_status_t setCT(const std::vector<int>& v)
    {
        if (v.size() != 2)
        {
            return FAIL;
        }

        if (v == ct)
        {
            return NO_CHANGES;
        }

        ct = v;
        return SUCCESS;
    }

    ScicosID getDestinationPort() const
    {
        return destinationPort;
    }

    void setDestinationPort(ScicosID destinationPort)
    {
        this->destinationPort = destinationPort;
    }

    ScicosID getSourcePort() const
    {
        return sourcePort;
    }

    void setSourcePort(ScicosID sourcePort)
    {
        this->sourcePort = sourcePort;
    }

private:
    ScicosID parentDiagram;

    ScicosID sourcePort;
    ScicosID destinationPort;

    // used to store, user-defined control points
    std::vector<double> controlPoints;

    std::string label;
    std::vector<double> thick;
    std::vector<int> ct;
};

} /* namespace model */
} /* namespace org_scilab_modules_scicos */

#endif /* LINK_HXX_ */
