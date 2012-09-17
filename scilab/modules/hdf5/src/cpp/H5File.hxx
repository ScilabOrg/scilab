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

#ifndef __H5FILE_HXX__
#define __H5FILE_HXX__

#include "H5Object.hxx"

namespace org_modules_hdf5
{
    class H5Group;

    class H5File : public H5Object
    {
	const std::string filename;
	const std::string path;
	const bool readonly;
	hid_t file;

    public :
	
	H5File(const char * _filename, const char * _path = "/", const bool _readonly = false);
	H5File(const std::string & _filename, const std::string & _path = "/", const bool _readonly = false);
	
	~H5File();

	H5Group & getRootGroup();
	hid_t getH5Id() const { return file; }
	virtual std::string getCompletePath() const;
	const std::string & getFileName() const { return filename; }
	const std::string & getRootPath() const { return path; }
	hsize_t getFileSize() const;
	void getFileHDF5Version(unsigned int * out) const;

	virtual void getAccessibleAttribute(const std::string & name, const int pos, void * pvApiCtx) const;

	virtual std::string toString(const unsigned int indentLevel) const;
	virtual std::string dump(std::set<haddr_t> & alreadyVisited, const unsigned int indentLevel = 0) const;

    private :
	
	void init();

    };
}

#endif // __H5FILE_HXX__
