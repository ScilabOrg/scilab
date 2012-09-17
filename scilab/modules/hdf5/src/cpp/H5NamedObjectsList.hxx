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

#ifndef __H5NAMEDOBJECTSLIST_HXX__
#define __H5NAMEDOBJECTSLIST_HXX__

#include "H5Object.hxx"
#include "H5File.hxx"
#include "H5Link.hxx"
#include "H5ListObject.hxx"

namespace org_modules_hdf5
{

class H5Group;

template <typename T>
class H5NamedObjectsList : public H5ListObject<T>
{

    typedef struct
    {
        union
        {
            unsigned int count;
            const char * name;
        } u;
        int linktype;
        int basetype;
    } OpData;

    int prevPos;
    hsize_t idx;

public :

    H5NamedObjectsList(H5Group & _parent, const int _baseType, const int _linkType, const std::string _baseTypeName) : H5ListObject<T>(_parent), baseType(_baseType), linkType(_linkType), baseTypeName(_baseTypeName), prevPos(0), idx(0) { }

    virtual ~H5NamedObjectsList()
    {

    }

    void setObject(const unsigned int pos, T & obj)
    {

    }

    T & getObject(const int pos)
    {
        return getObject(pos, true);
    }

    inline static herr_t count(hid_t g_id, const char * name, const H5L_info_t * info, void * op_data)
    {
        OpData & opdata = *(OpData *)op_data;

        if ((opdata.linktype != -1 && info->type == opdata.linktype) || (opdata.linktype == -1 && info->type != H5L_TYPE_ERROR))
        {
            H5O_info_t oinfo;
            herr_t err;
            hid_t obj = H5Oopen(g_id, name, H5P_DEFAULT);

            if (obj < 0)
            {
                return (herr_t) - 1;
            }

            err = H5Oget_info(obj, &oinfo);
            H5Oclose(obj);

            if (err < 0)
            {
                return (herr_t) - 2;
            }

            if (oinfo.type == opdata.basetype)
            {
                opdata.u.count++;
            }
        }

        return (herr_t)0;
    }

    inline static herr_t getElement(hid_t g_id, const char * name, const H5L_info_t * info, void * op_data)
    {
        OpData & opdata = *(OpData *)op_data;

        if ((opdata.linktype != -1 && info->type == opdata.linktype) || (opdata.linktype == -1 && info->type != H5L_TYPE_ERROR))
        {
            H5O_info_t oinfo;
            herr_t err;
            hid_t obj = H5Oopen(g_id, name, H5P_DEFAULT);

            if (obj < 0)
            {
                return (herr_t) - 1;
            }

            err = H5Oget_info(obj, &oinfo);
            H5Oclose(obj);

            if (err < 0)
            {
                return (herr_t) - 2;
            }

            if (oinfo.type == opdata.basetype)
            {
                opdata.u.count--;
            }

            if (opdata.u.count == 0)
            {
                opdata.u.name = strdup(name);

                return (herr_t)1;
            }
        }

        return (herr_t)0;
    }

    const unsigned int getSize() const
    {
        hsize_t idx = 0;
        OpData op_data;
        herr_t err;

        op_data.u.count = 0;
        op_data.linktype = linkType;
        op_data.basetype = baseType;

        err = H5Literate(H5Object::getParent().getH5Id(), H5_INDEX_NAME, H5_ITER_INC, &idx, count, &op_data);

        if (err < 0)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot get the number of groups %d."));
        }

        return op_data.u.count;
    }

    std::string dump(std::set<haddr_t> & alreadyVisited, const unsigned int indentLevel) const
    {
        std::ostringstream os;
        const unsigned int size = getSize();

        for (unsigned int i = 0; i < size; i++)
        {
            const T & obj = const_cast<H5NamedObjectsList *>(this)->getObject(i, false);
            os << obj.dump(alreadyVisited, indentLevel);

            delete &obj;
        }

        return os.str();
    }

    virtual std::string toString(const unsigned int indentLevel) const
    {
        std::ostringstream os;
        std::string indentString = H5Object::getIndentString(indentLevel);
        const unsigned int size = getSize();

        os << indentString << _("Filename") << ": " << H5Object::getParent().getFile().getFileName() << std::endl
           << indentString << _("Parent group name") << ": " << H5Object::getParent().getName() << std::endl
           << indentString << _("Parent group path") << ": " << H5Object::getParent().getCompletePath() << std::endl
           << indentString << _("Elements type") << ": " << baseTypeName << std::endl
           << indentString << _("Size") << ": " << size;

        return os.str();
    }

protected :

    const int baseType;
    const int linkType;
    const std::string baseTypeName;

private:

    T & getObject(const int pos, const bool checkPos)
    {
        OpData op_data;
        herr_t err;

        op_data.linktype = linkType;
        op_data.basetype = baseType;

        if (pos < prevPos)
        {
            idx = 0;
            op_data.u.count = pos + 1;
        }
        else
        {
            op_data.u.count = pos - prevPos + 1;
        }

        err = H5Literate(H5Object::getParent().getH5Id(), H5_INDEX_NAME, H5_ITER_INC, &idx, getElement, &op_data);

        if (err > 0)
        {
            prevPos = pos + 1;
            return *new T(H5Object::getParent(), op_data.u.name);
        }
        else
        {
            idx = 0;
            prevPos = 0;
            throw H5Exception(__LINE__, __FILE__, _("Cannot get the number of groups %d %d."), getSize(), pos);
        }
    }

    T & getObject(const std::string & name)
    {
        H5O_info_t info;
        herr_t err;
        H5Object * obj = 0;

        if (H5Lexists(H5Object::getParent().getH5Id(), name.c_str(), H5P_DEFAULT) <= 0)
        {
            throw H5Exception(__LINE__, __FILE__, _("Invalid name: %s."), name.c_str());
        }

        err = H5Oget_info_by_name(H5Object::getParent().getH5Id(), name.c_str(), &info, H5P_DEFAULT);
        if (err < 0)
        {
            throw H5Exception(__LINE__, __FILE__, _("Invalid name: %s."), name.c_str());
        }

        if (info.type == baseType)
        {
            return *new T(H5Object::getParent(), name);
        }

        throw H5Exception(__LINE__, __FILE__, _("Invalid HDF5 object"));
    }
};
}

#endif // __H5NAMEDOBJECTSLIST_HXX__
