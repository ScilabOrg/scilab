/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

#include "XMLObject.hxx"
#include "XMLList.hxx"

namespace org_modules_xml
{
XMLList::XMLList(): XMLObject(), size(0)
{
    scilabType = XMLLIST;
}

const std::string XMLList::toString() const
{
    std::ostringstream oss;

    oss << "XML List" << std::endl;
    oss << "size: " << getSize();

    return oss.str();
}
}
