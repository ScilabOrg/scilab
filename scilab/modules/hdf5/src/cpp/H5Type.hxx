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

#ifndef __H5TYPE_HXX__
#define __H5TYPE_HXX__

#include "H5Object.hxx"
#include "H5File.hxx"

namespace org_modules_hdf5
{
    
    class H5Type : public H5Object
    {
	hid_t type;
	const std::string name;

    public:

	H5Type(H5Object & _parent, hid_t type);
	H5Type(H5Object & _parent, const char * name);
	H5Type(H5Object & _parent, const std::string & name);
	H5Type(H5Object & _parent, hid_t type, const char * name);
	H5Type(H5Object & _parent, hid_t type, const std::string & name);

	virtual ~H5Type();

	virtual hid_t getH5Id() const { return type; }
	virtual const std::string & getName() const { return name; }
	virtual std::string getClassName() const;
	virtual std::string getTypeName() const;
	virtual unsigned int getTypeSize() const;
	virtual std::string getNativeTypeName() const;
	virtual unsigned int getNativeTypeSize() const;

	virtual void getAccessibleAttribute(const std::string & _name, const int pos, void * pvApiCtx) const;
	virtual std::string dump(std::set<haddr_t> & alreadyVisited, const unsigned int indentLevel) const;
	virtual std::string toString(const unsigned int indentLevel) const;

	static std::string getNameFromType(hid_t type);

    private:

	void init();
    };
}

#endif // __H5TYPE_HXX__
