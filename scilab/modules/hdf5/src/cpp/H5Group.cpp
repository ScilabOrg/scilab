/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "H5Group.hxx"
#include "H5File.hxx"
#include "H5SoftLinksList.hxx"
#include "H5LinksList.hxx"
#include "H5GroupsList.hxx"
#include "H5DatasetsList.hxx"
#include "H5TypesList.hxx"
#include "H5Link.hxx"

namespace org_modules_hdf5
{

void H5Group::init()
{
    group = H5Gopen(getParent().getH5Id(), name.c_str(), H5P_DEFAULT);
    if (group < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot open the group %s."), name.c_str());
    }
}

H5Group::H5Group(H5Object & _parent, const std::string & _name) : H5Object(_parent), name(_name)
{
    init();
}

H5Group::H5Group(H5Object & _parent, const char * _name) : H5Object(_parent), name(std::string(_name))
{
    init();
}

H5Group::H5Group(H5Object & _parent, hid_t _group, const char * _name) : H5Object(_parent), group(_group), name(std::string(_name))
{
    // TODO: mettre qque chose la dedans
}

H5Group::H5Group(H5Object & _parent, hid_t _group, const std::string & _name) : H5Object(_parent), group(_group), name(_name)
{
    // TODO: mettre qque chose la dedans
}

H5Group::~H5Group()
{
    if (group >= 0)
    {
        H5Gclose(group);
    }
}

H5LinksList & H5Group::getLinks()
{
    return *new H5LinksList(*this);
}

H5NamedObjectsList<H5SoftLink> & H5Group::getSoftLinks()
{
    return *new H5NamedObjectsList<H5SoftLink>(*this, H5O_TYPE_GROUP, H5L_TYPE_SOFT, "H5 Soft Link");
}

H5NamedObjectsList<H5ExternalLink> & H5Group::getExternalLinks()
{
    return *new H5NamedObjectsList<H5ExternalLink>(*this, H5O_TYPE_GROUP, H5L_TYPE_EXTERNAL, "H5 External Link");
}

H5GroupsList & H5Group::getGroups()
{
    return *new H5GroupsList(*this);
}

H5NamedObjectsList<H5Group> & H5Group::getHardGroups()
{
    return *new H5NamedObjectsList<H5Group>(*this, H5O_TYPE_GROUP, H5L_TYPE_HARD, "H5 Group");
}

H5NamedObjectsList<H5Type> & H5Group::getHardTypes()
{
    return *new H5NamedObjectsList<H5Type>(*this, H5O_TYPE_NAMED_DATATYPE, H5L_TYPE_HARD, "H5 Type");
}

H5NamedObjectsList<H5Dataset> & H5Group::getHardDatasets()
{
    return *new H5NamedObjectsList<H5Dataset>(*this, H5O_TYPE_DATASET, H5L_TYPE_HARD, "H5 Dataset");
}

H5DatasetsList & H5Group::getDatasets()
{
    return *new H5DatasetsList(*this);
}

H5TypesList & H5Group::getTypes()
{
    return *new H5TypesList(*this);
}

std::string H5Group::getCompletePath() const
{
    std::string name = getName();
    if (name == "/")
    {
        return "/";
    }

    return H5Object::getCompletePath();
}

void H5Group::getAccessibleAttribute(const std::string & _name, const int pos, void * pvApiCtx) const
{
    SciErr err;
    std::string lower(_name);

    if (_name.empty())
    {
        throw H5Exception(__LINE__, __FILE__, _("Invalid field: "));
    }

    try
    {
        H5Object & obj = H5Object::getObject(*const_cast<H5Group *>(this), group, _name);
        obj.createOnScilabStack(pos, pvApiCtx);
        return;
    }
    catch (H5Exception & e) { }

    std::transform(_name.begin(), _name.end(), lower.begin(), tolower);

    if (lower == "attributes")
    {
        const H5AttributesList & attrs = const_cast<H5Group *>(this)->getAttributes();
        attrs.createOnScilabStack(pos, pvApiCtx);

        return;
    }
    else if (lower == "groups")
    {
        const H5GroupsList & groups = const_cast<H5Group *>(this)->getGroups();
        groups.createOnScilabStack(pos, pvApiCtx);

        return;
    }
    else if (lower == "datasets")
    {
        const H5DatasetsList & sets = const_cast<H5Group *>(this)->getDatasets();
        sets.createOnScilabStack(pos, pvApiCtx);

        return;
    }
    else if (lower == "types")
    {
        const H5TypesList & types = const_cast<H5Group *>(this)->getTypes();
        types.createOnScilabStack(pos, pvApiCtx);

        return;
    }
    else if (lower == "links")
    {
        std::vector<std::string> names;
        std::vector<std::string> types;
        std::vector<std::string> linkstype;
        std::vector<const char *> _str;
        H5Object::getLinksInfo(*this, names, types, linkstype);
        _str.reserve(names.size() * 3);

        for (unsigned int i = 0; i < names.size(); i++)
        {
            _str.push_back(names[i].c_str());
        }
        for (unsigned int i = 0; i < names.size(); i++)
        {
            _str.push_back(linkstype[i].c_str());
        }
        for (unsigned int i = 0; i < names.size(); i++)
        {
            _str.push_back(types[i].c_str());
        }

        err = createMatrixOfString(pvApiCtx, pos, names.size(), 3, &(_str[0]));
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a column of strings on the stack."));
        }

        return;
    }

    H5Object::getAccessibleAttribute(_name, pos, pvApiCtx);
}

std::string H5Group::dump(std::set<haddr_t> & alreadyVisited, const unsigned int indentLevel) const
{
    haddr_t addr = this->getAddr();
    if (alreadyVisited.find(addr) != alreadyVisited.end())
    {
        return "";
    }
    else
    {
        alreadyVisited.insert(addr);
    }

    std::ostringstream os;
    H5AttributesList & attrs = const_cast<H5Group *>(this)->getAttributes();
    H5NamedObjectsList<H5SoftLink> & softlinks = const_cast<H5Group *>(this)->getSoftLinks();
    H5NamedObjectsList<H5ExternalLink> & externallinks = const_cast<H5Group *>(this)->getExternalLinks();
    H5NamedObjectsList<H5Group> & hardgroups = const_cast<H5Group *>(this)->getHardGroups();
    H5NamedObjectsList<H5Type> & hardtypes = const_cast<H5Group *>(this)->getHardTypes();
    H5NamedObjectsList<H5Dataset> & harddatasets = const_cast<H5Group *>(this)->getHardDatasets();

    os << H5Object::getIndentString(indentLevel) << "GROUP \"" << name << "\" {" << std::endl
       << attrs.dump(alreadyVisited, indentLevel + 1)
       << hardtypes.dump(alreadyVisited, indentLevel + 1)
       << harddatasets.dump(alreadyVisited, indentLevel + 1)
       << hardgroups.dump(alreadyVisited, indentLevel + 1)
       << softlinks.dump(alreadyVisited, indentLevel + 1)
       << externallinks.dump(alreadyVisited, indentLevel + 1)
       << H5Object::getIndentString(indentLevel) << "}" << std::endl;

    delete &attrs;
    delete &softlinks;
    delete &externallinks;
    delete &hardgroups;
    delete &hardtypes;
    delete &harddatasets;

    return os.str();
}

std::string H5Group::toString(const unsigned int indentLevel) const
{
    std::ostringstream os;
    std::string indentString = H5Object::getIndentString(indentLevel + 1);
    const H5GroupsList & groups = const_cast<H5Group *>(this)->getGroups();
    const H5DatasetsList & datasets = const_cast<H5Group *>(this)->getDatasets();
    const H5TypesList & types = const_cast<H5Group *>(this)->getTypes();
    const H5AttributesList & attrs = const_cast<H5Group *>(this)->getAttributes();

    os << H5Object::getIndentString(indentLevel) << "HDF5 Group" << std::endl
       << indentString << _("Filename") << ": " << getFile().getFileName() << std::endl
       << indentString << _("Group name") << ": " << name << std::endl
       << indentString << _("Group path") << ": " << getCompletePath() << std::endl
       << indentString << _("Attributes") << ": [1 x " << attrs.getSize() << "]" << std::endl
       << indentString << _("Groups") << ": [1 x " << groups.getSize() << "]" << std::endl
       << indentString << _("Datasets") << ": [1 x " << datasets.getSize() << "]" << std::endl
       << indentString << _("Types") << ": [1 x " << types.getSize() << "]";

    delete &groups;
    delete &datasets;
    delete &types;
    delete &attrs;

    return os.str();
}
}
