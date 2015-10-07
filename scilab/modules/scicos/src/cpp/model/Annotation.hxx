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

#include "utilities.hxx"
#include "Model.hxx"
#include "model/BaseObject.hxx"

namespace org_scilab_modules_scicos
{
namespace model
{

class Annotation: public BaseObject
{
public:
    Annotation(): BaseObject(ANNOTATION), m_parentDiagram(ScicosID()), m_parentBlock(ScicosID()), m_geometry{0, 0, 2, 1},
        m_description("Text"), m_font("2"), m_font_size("1"), m_style(""), m_relatedTo(ScicosID()) {};
    Annotation(const Annotation& o) : BaseObject(ANNOTATION), m_parentDiagram(o.m_parentDiagram), m_parentBlock(o.m_parentBlock), m_geometry(o.m_geometry),
        m_description(o.m_description), m_font(o.m_font), m_font_size(o.m_font_size), m_style(o.m_style), m_relatedTo(o.m_relatedTo) {};
    ~Annotation() = default;

private:
    friend class ::org_scilab_modules_scicos::Model;

    void getDescription(std::string& data) const
    {
        data = m_description;
    }

    update_status_t setDescription(const std::string& data)
    {
        if (data == m_description)
        {
            return NO_CHANGES;
        }

        m_description = data;
        return SUCCESS;
    }

    void getFont(std::string& data) const
    {
        data = m_font;
    }

    update_status_t setFont(const std::string& data)
    {
        if (data == m_font)
        {
            return NO_CHANGES;
        }

        m_font = data;
        return SUCCESS;
    }

    void getFontSize(std::string& data) const
    {
        data = m_font_size;
    }

    update_status_t setFontSize(const std::string& data)
    {
        if (data == m_font_size)
        {
            return NO_CHANGES;
        }

        m_font_size = data;
        return SUCCESS;
    }

    void getStyle(std::string& data) const
    {
        data = m_style;
    }

    update_status_t setStyle(const std::string& data)
    {
        if (data == m_style)
        {
            return NO_CHANGES;
        }

        m_style = data;
        return SUCCESS;
    }

    void getGeometry(std::vector<double>& v) const
    {
        m_geometry.fill(v);
    }

    update_status_t setGeometry(const std::vector<double>& v)
    {
        if (v.size() != 4)
        {
            return FAIL;
        }

        Geometry g = Geometry(v);
        if (g == m_geometry)
        {
            return NO_CHANGES;
        }

        m_geometry = g;
        return SUCCESS;
    }

    void getParentDiagram(ScicosID& v) const
    {
        v = m_parentDiagram;
    }

    update_status_t setParentDiagram(const ScicosID v)
    {
        if (v == m_parentDiagram)
        {
            return NO_CHANGES;
        }

        m_parentDiagram = v;
        return SUCCESS;
    }

    void getParentBlock(ScicosID& v) const
    {
        v = m_parentBlock;
    }

    update_status_t setParentBlock(const ScicosID v)
    {
        if (v == m_parentBlock)
        {
            return NO_CHANGES;
        }

        m_parentBlock = v;
        return SUCCESS;
    }

    ScicosID getRelatedTo() const
    {
        return m_relatedTo;
    }

    update_status_t setRelatedTo(ScicosID relatedTo)
    {
        if (this->m_relatedTo == relatedTo)
        {
            return NO_CHANGES;
        }

        this->m_relatedTo = relatedTo;
        return SUCCESS;
    }

private:
    ScicosID m_parentDiagram;
    ScicosID m_parentBlock;

    Geometry m_geometry;
    std::string m_description;
    std::string m_font;
    std::string m_font_size;
    std::string m_style;
    ScicosID m_relatedTo;
};

} /* namespace model */
} /* namespace org_scilab_modules_scicos */

#endif /* ANNOTATION_HXX_ */
