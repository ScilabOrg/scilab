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

#ifndef DIAGRAM_HXX_
#define DIAGRAM_HXX_

#include "model/BaseObject.hxx"
#include "model/Block.hxx"
#include "model/Link.hxx"

namespace org_scilab_modules_scicos
{
namespace model
{

struct SimulationConfig
{
    double final_time;

    // FIXME: add more properties there
};

class Diagram: BaseObject
{

public:
    Diagram(const std::string& uid) : BaseObject(uid) {};

private:

    SimulationConfig properties;

    std::vector<BaseObject*> children;
    std::vector<Datatype*> datatypes;
};

} /* namespace model */
} /* namespace org_scilab_modules_scicos */

#endif /* DIAGRAM_HXX_ */
