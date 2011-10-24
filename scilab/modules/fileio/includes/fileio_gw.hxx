/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __FILEIO_GW_HXX__
#define __FILEIO_GW_HXX__

#include "cpp_gateway_prototype.hxx"

#ifdef _MSC_VER
	#if FILEIO_GW_EXPORTS
		#define EXTERN_FILEIO_GW __declspec (dllexport)
	#else
		#define EXTERN_FILEIO_GW __declspec (dllimport)
	#endif
#else
	#define EXTERN_FILEIO_GW
#endif

class FileioModule
{
private :
    FileioModule() {};
    ~FileioModule() {};

public :
	EXTERN_FILEIO_GW static int Load();
};

CPP_GATEWAY_PROTOTYPE(sci_mopen, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_mclose, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_mput, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_mget, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_mputl, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_mgetl, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_pathconvert, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_isdir, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_filesep, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_fileparts, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_findfiles, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_basename, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_isfile, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_fileinfo, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_deletefile, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_getshortpathname, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_getlongpathname, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_fileext, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_createdir, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_fullpath, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_removedir, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_meof, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_mputstr, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_mgetstr, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_mseek, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_mtell, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_newest, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_merror, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_mclearerr, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_get_absolute_file_path, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_getrelativefilename, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_pathsep, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_mfprintf, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_mscanf, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_mfscanf, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_msscanf, EXTERN_FILEIO_GW);
CPP_GATEWAY_PROTOTYPE(sci_getdrives, EXTERN_FILEIO_GW);

#endif /* !__FILEIO_GW_HXX__ */
