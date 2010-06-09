/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <iostream>

#ifdef _MSC_VER

#include "windows.h"
#define putenv _putenv

#else

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define  DIRMODE	0777

#endif

#include "setenvvar.hxx"
#include "context.hxx"
#include "configvariable.hxx"
#include "sci_home.h"
#include "sci_path.h"

using namespace std;


/*--------------------------------------------------------------------------*/
/**
* Les variables d'environnements SCI, and some others
* sont définies directement dans scilab
* scilex peut donc etre executé seul
*/

/*--------------------------------------------------------------------------*/
/**
* Define SCI and some others Environments variables
*/
void SetScilabEnvironment(void)
{
    //create SCI, SCIHOME, home, ...
    defineSCI();
    defineSCIHOME();
}

#ifdef _MSC_VER
void SciEnvForWindows(void)
{
	char *SCIPathName = 0;

	//SCIPathName = getScilabDirectory(true);

	/* Correction Bug 1579 */
	if (!IsTheGoodShell())
	{
		if ( (!Set_Shell()) || (!IsTheGoodShell()))
		{
			cout << "Please modify ""ComSpec"" environment variable." << endl << "cmd.exe on W2K and more." << endl;
		}
	}

//	SetScilabEnvironmentVariables(SCIPathName);
	if(SCIPathName)
	{
		delete[] SCIPathName;
		SCIPathName = NULL;
	}
}
#else
int SciEnvForOthers(void)
{
	int ierr,iflag=0;
	int lbuf=PATH_MAX;
	char *buf = new char[PATH_MAX];
	if (buf)
	{
		getenvc(&ierr,(char*)"SCI",buf,&lbuf,&iflag);

		if ( ierr== 1) 
		{
			cerr << "SCI environment variable not defined." << endl;
			exit(1);
		}
		setSCI(buf);
		delete[] buf;
		buf = NULL;
	}
	
	return 0;
}
#endif

#ifdef _MSC_VER
/*--------------------------------------------------------------------------*/
bool IsTheGoodShell(void)
{
	bool bOK=false;
	char shellCmd[PATH_MAX];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	strcpy(shellCmd,"");
	strcpy(fname,"");
	GetEnvironmentVariable("ComSpec", shellCmd, PATH_MAX);
	_splitpath(shellCmd,drive,dir,fname,ext);

	if (_stricmp(fname,"cmd")==0) bOK=true;

	return bOK;
}

/*--------------------------------------------------------------------------*/
bool Set_Shell(void)
{
	bool bOK=false;
	char env[_MAX_DRIVE+_MAX_DIR+_MAX_FNAME+_MAX_EXT+10];
	char *WINDIRPATH=NULL;

	WINDIRPATH=getenv ("SystemRoot");
	sprintf(env,"ComSpec=%s\\system32\\cmd.exe",WINDIRPATH);

	if (putenv (env))
	{
		bOK=false;
	}
	else
	{
		bOK=true;
	}

	if(WINDIRPATH)
	{
		delete[] WINDIRPATH;
		WINDIRPATH = NULL;
	}
	return bOK;
}
#endif

bool slashToAntislash(char *pathunix,char *pathwindows)
{
	return convertSlash(pathunix,pathwindows,true);
}
/*--------------------------------------------------------------------------*/
bool AntislashToSlash(char *pathwindows,char *pathunix)
{
	return convertSlash(pathwindows,pathunix,false);
}
/*--------------------------------------------------------------------------*/
bool convertSlash(char *path_in,char *path_out,bool slashToAntislash)
{
	bool bOK = false;
	if ( (path_in) && (path_out) )
	{
		int i=0;
		strcpy(path_out,path_in);
		for (i=0;i<(int)strlen(path_out);i++)
		{
			if ( slashToAntislash )
			{
				if (path_in[i] == UNIX_SEPATATOR)
				{
					path_out[i] = WINDOWS_SEPATATOR;
					bOK = true;
				}
			}
			else
			{
				if (path_in[i] == WINDOWS_SEPATATOR)
				{
					path_out[i] = UNIX_SEPATATOR;
					bOK = true;
				}
			}
		}
	}
	else bOK = false;

	return bOK;
}

/*--------------------------------------------------------------------------*/
void getenvc(int *ierr,char *var,char *buf,int *buflen,int *iflag)
{
	#ifdef _MSC_VER
	if (GetEnvironmentVariable(var,buf,(DWORD)*buflen) == 0)
	{
		if(*iflag == 1)
			cout << "Undefined environment variable " << var << endl;
		*ierr=1;
	}
	else
	{
		*buflen = (int)strlen(buf);
		*ierr=0;
	}
	#else
	char *local;
	if ( (local=getenv(var)) == 0)
	{
		if ( *iflag == 1 ) 
		{
			cout << "Undefined environment variable " << var << endl;
		}
		*ierr=1;
	}
	else
	{
		strncpy(buf,local,*buflen);
		*buflen = strlen(buf);
		*ierr=0;
	}
	#endif
}

char *getshortpathname(const char *longpathname,bool *convertok)
{
	char *ShortName = NULL;

	if (longpathname)
	{
		#ifdef _MSC_VER
		/* first we try to call to know path length */
		int length = GetShortPathName(longpathname, NULL, 0);

		if (length <= 0 ) length = MAX_PATH_SHORT;

		ShortName = new char[length];

		if (ShortName)
		{
			/* second converts path */
			if ( GetShortPathName(longpathname, ShortName, length) )
			{
				*convertok = true;
			}
			else
			{
				/* FAILED */
				strcpy(ShortName, longpathname);
				*convertok = false;
			}
		}
		else
		{
			/* FAILED */
			*convertok = false;
		}
		#else
		/* Linux */
		int length = (int)strlen(longpathname) + 1;
		ShortName = new char[length];
		if(ShortName) 
		{
			strcpy(ShortName, longpathname);
		}
		
		*convertok = false;
		#endif
	}
	else
	{
		/* FAILED */
		*convertok = false;
	}
	return ShortName;
}

bool isdir(const char * path)
{
	bool bOK = false;
#ifndef _MSC_VER
	struct stat buf;
	if (path == NULL) return false;
	if (stat(path, &buf) == 0 && S_ISDIR(buf.st_mode)) bOK=true;
#else
	if (isDrive(path)) return true;
	else
	{
		char *pathTmp = NULL;
		pathTmp = new char[strlen(path) + 1];
		if (pathTmp)
		{
			DWORD attr = 0;
			strcpy(pathTmp,path);
			if ( (pathTmp[strlen(pathTmp)-1]=='\\') || (pathTmp[strlen(pathTmp)-1]=='/') )
			{
				pathTmp[strlen(pathTmp)-1]='\0';
			}
			attr = GetFileAttributes(pathTmp);
			delete[] pathTmp;
			pathTmp = NULL;
			if (attr == INVALID_FILE_ATTRIBUTES) return false;
			return ((attr & FILE_ATTRIBUTE_DIRECTORY) != 0) ? true : false;
		}
	}
#endif
	return bOK;
}

bool createdirectory(const char *path)
{
	bool bOK=false;
#ifndef _MSC_VER
	if  (!isdir(path))
	{
		if (mkdir(path, DIRMODE) == 0) bOK=true;
	}
#else
	if (CreateDirectory(path,NULL)) bOK=true;
#endif
	return bOK;
}

bool isDrive(const char *strname)
{
	#ifdef _MSC_VER
	if (strname)
	{
		if ( ((strlen(strname) == 2) || (strlen(strname) == 3)) && (strname[1]== ':') )
		{
			if (strlen(strname) == 3)
			{
				if ( (strname[2]!= '\\') && (strname[2]!= '/') )
				{
					return false;
				}
			}

			if ( ( strname[0] >= 'A' && strname[0] <= 'Z' ) || ( strname[0] >= 'a' && strname[0] <= 'z' ) )
			{
				return true;
			}
		}
	}
	#endif
	return false;
}

