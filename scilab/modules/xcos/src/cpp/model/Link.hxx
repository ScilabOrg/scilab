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
#include <utility>

#include "model/BaseObject.hxx"

namespace org_scilab_modules_xcos
{
namespace model
{

// forward declaration of the Block's port, see Block.hxx for more informations
struct Port;

class Link: BaseObject
{
public:
    Link(const std::string& uid, const Port& dstPort, const Port& srcPort) :
        BaseObject(uid), dstPort(dstPort), srcPort(srcPort)
    {
    }

    inline const Port& getDestination()
    {
        return dstPort;
    }

    inline const Port& getSource()
    {
        return srcPort;
    }

private:
    const Port& dstPort;
    const Port& srcPort;

    // used to store, user-defined control points
    std::vector< std::pair<double, double> > points;
};

} /* namespace model */
} /* namespace org_scilab_modules_xcos */

#endif /* LINK_HXX_ */
