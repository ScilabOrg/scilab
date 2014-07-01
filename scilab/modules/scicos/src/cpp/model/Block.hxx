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

#ifndef BLOCK_HXX_
#define BLOCK_HXX_

#include <string>
#include <vector>
#include <bitset>
#include <utility>

#include "model/BaseObject.hxx"
#include "model/Link.hxx"

namespace org_scilab_modules_scicos
{
namespace model
{

struct Geometry
{
    double x;
    double y;
    double width;
    double height;
};

struct Parameter
{
    // FIXME: list the possible parameters kind, name, and so on
    double foo;
};

//FIXME: should reuse Scilab datatypes descriptors
struct Datatype
{
    int datatype_id;
    int rows;
    int collumns;
};

struct Descriptor
{
    std::string functionName;
    int functionApi;

    // FIXME: should encode all possible values for dep_ut and blocktype
    std::bitset<4> schedulingProperties;
};

class Block;
struct Port
{

    Port(const Datatype* dataType, const Block* sourceBlock, unsigned int portNumber) :
        dataType(dataType), sourceBlock(sourceBlock), portNumber(portNumber)
    {
    }

    const Datatype* dataType;
    const Block* sourceBlock;
    unsigned int portNumber;

    std::vector<Link*> connectedSignals;
};

class Block: BaseObject
{

public:
    Block(const std::string& uid) :
        BaseObject(uid), portReference(0)
    {
    }

    const std::wstring& get_typename() const;
    void set_typename(const std::wstring& n) const;
    const Geometry& get_geometry() const;
    void set_geometry(const Geometry& g) const;

private:
    std::wstring _typename;
    Geometry geometry;

    Descriptor sim;

    std::vector<Port> in;
    std::vector<Port> out;
    std::vector<Port> ein;
    std::vector<Port> eout;

    Parameter parameters;

    /**
     * SuperBlock: the blocks, links and so on contained into this block
     */
    std::vector<BaseObject*> children;

    /**
     * I/O Blocks: the corresponding parent port
     */
    Port* portReference;
};

} /* namespace model */
} /* namespace org_scilab_modules_scicos */

#endif /* BLOCK_HXX_ */
