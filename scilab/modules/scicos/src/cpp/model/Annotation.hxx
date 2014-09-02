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

#ifndef ANNOTATION_HXX_
#define ANNOTATION_HXX_

#include <string>
#include <vector>

#include "Model.hxx"
#include "model/BaseObject.hxx"

namespace org_scilab_modules_scicos
{
namespace model
{

class Annotation: public BaseObject
{
    friend class ::org_scilab_modules_scicos::Model;

private:
    Annotation(): BaseObject(ANNOTATION), parentDiagram(0), geometry(), description(), font(), size(), relatedTo(0) {};
    Annotation(const Annotation& o) : BaseObject(ANNOTATION), parentDiagram(o.parentDiagram), geometry(o.geometry), description(o.description), font(o.font), size(o.size), relatedTo(o.relatedTo) {};
    ~Annotation() {};

    void getDescription(std::string& data) const
    {
        data = description;
    }

    update_status_t setDescription(const std::string& data)
    {
        if (data == description)
        {
            return NO_CHANGES;
        }

        description = data;
        return SUCCESS;
    }

    void getFont(std::string& data) const
    {
        data = font;
    }

    update_status_t setFont(const std::string& data)
    {
        if (data == font)
        {
            return NO_CHANGES;
        }

        font = data;
        return SUCCESS;
    }

    void getFontSize(std::string& data) const
    {
        data = size;
    }

    update_status_t setFontSize(const std::string& data)
    {
        if (data == size)
        {
            return NO_CHANGES;
        }

        size = data;
        return SUCCESS;
    }

    void getGeometry(std::vector<double>& v) const
    {
        geometry.fill(v);
    }

    update_status_t setGeometry(const std::vector<double>& v)
    {
        if (v.size() != 4)
        {
            return FAIL;
        }

        Geometry g = Geometry(v);
        if (g == geometry)
        {
            return NO_CHANGES;
        }

        geometry = g;
        return SUCCESS;
    }

    ScicosID getParentDiagram() const
    {
        return parentDiagram;
    }

    update_status_t setParentDiagram(ScicosID parentDiagram)
    {
        if (this->parentDiagram == parentDiagram)
        {
            return NO_CHANGES;
        }

        this->parentDiagram = parentDiagram;
        return SUCCESS;
    }

    ScicosID getRelatedTo() const
    {
        return relatedTo;
    }

    update_status_t setRelatedTo(ScicosID relatedTo)
    {
        if (this->relatedTo == relatedTo)
        {
            return NO_CHANGES;
        }

        this->relatedTo = relatedTo;
        return SUCCESS;
    }

private:
    ScicosID parentDiagram;
    Geometry geometry;
    std::string description;
    std::string font;
    std::string size;
    ScicosID relatedTo;
};

} /* namespace model */
} /* namespace org_scilab_modules_scicos */

#endif /* ANNOTATION_HXX_ */
