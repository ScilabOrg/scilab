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

#ifndef ANNOTATION_HXX_
#define ANNOTATION_HXX_

#include <cassert>
#include <string>

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
    Annotation(): BaseObject(ANNOTATION), parentDiagram(0), relatedTo(0) {};
    Annotation(const Annotation& o) : BaseObject(ANNOTATION), parentDiagram(o.parentDiagram), relatedTo(o.relatedTo) {};
    ~Annotation() {};

    const std::string& getDescription() const
    {
        return description;
    }

    void setDescription(const std::string& description)
    {
        this->description = description;
    }

    void getGeometry(size_t* len, double** data) const
    {
        *len = 4;
        *data = new double[4];

        *data[0] = geometry.x;
        *data[1] = geometry.y;
        *data[2] = geometry.width;
        *data[3] = geometry.height;
    }

    void setGeometry(size_t len, double* data)
    {
        assert(len == 4);

        geometry.x = data[0];
        geometry.y = data[1];
        geometry.width = data[2];
        geometry.height = data[3];
    }

    ScicosID getParentDiagram() const
    {
        return parentDiagram;
    }

    void setParentDiagram(ScicosID parentDiagram)
    {
        this->parentDiagram = parentDiagram;
    }

    ScicosID getRelatedTo() const
    {
        return relatedTo;
    }

    void setRelatedTo(ScicosID relatedTo)
    {
        this->relatedTo = relatedTo;
    }

private:
    ScicosID parentDiagram;
    Geometry geometry;
    std::string description;
    ScicosID relatedTo;
};

} /* namespace model */
} /* namespace org_scilab_modules_scicos */

#endif /* ANNOTATION_HXX_ */
