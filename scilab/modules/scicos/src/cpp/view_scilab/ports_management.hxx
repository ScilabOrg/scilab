/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef PORTS_MANAGEMENT_HXX_
#define PORTS_MANAGEMENT_HXX_

#include <string>
#include <vector>

#include "double.hxx"
#include "string.hxx"

#include "Controller.hxx"
#include "GraphicsAdapter.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

static types::Double* get_ports(const GraphicsAdapter& adaptor, object_properties_t p, const Controller& controller);

static types::String* get_ports_property(const GraphicsAdapter& adaptor, object_properties_t p, const Controller& controller, object_properties_t p2);

static bool create_ports(const GraphicsAdapter& adaptor, types::InternalType* v, object_properties_t p, Controller& controller);

static bool set_ports_property(const GraphicsAdapter& adaptor, types::InternalType* v, object_properties_t p, Controller& controller, object_properties_t p2);

} /* view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* PORTS_MANAGEMENT_HXX_ */
