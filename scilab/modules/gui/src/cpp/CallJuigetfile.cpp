/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - DIGITEO - Sylvestre KOUMAR
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "CallJuigetfile.hxx"
#include "Juigetfile.hxx"
#include "BOOL.h"

using namespace org_scilab_modules_gui_filechooser;

void CallJuigetfile(char ** mask, char ** description, int maskSize, char * initialDirectory, char * titleBox, bool multipleSelection)
{
	if (description == NULL)
	{
		Juigetfile::uigetfile(getScilabJavaVM(), mask, maskSize, NULL, 0, initialDirectory, titleBox, multipleSelection);
	}
	else
	{
		Juigetfile::uigetfile(getScilabJavaVM(), mask, maskSize, description, maskSize, initialDirectory, titleBox, multipleSelection);
	}
}

void CallJuigetfileOnlyWithMask(char ** mask, char ** description, int maskSize)
{
	if (description == NULL)
	{
		Juigetfile::uigetfile(getScilabJavaVM(), mask, maskSize, NULL, 0);
	}
	else
	{
		Juigetfile::uigetfile(getScilabJavaVM(), mask, maskSize, description, maskSize);
	}  
}

void CallJuigetfileWithMaskAndInitialdirectory(char ** mask, char ** description, int maskSize, char * initialDirectory)
{
	if (description == NULL)
	{
		Juigetfile::uigetfile(getScilabJavaVM(), mask, maskSize, NULL, 0, initialDirectory);
	}
	else
	{
		Juigetfile::uigetfile(getScilabJavaVM(), mask, maskSize, description, maskSize, initialDirectory);
	}
}

void CallJuigetfileWithoutMultipleSelection(char ** mask, char ** description, int maskSize, char * initialDirectory, char * titleBox)
{
	if (description == NULL)
	{
		Juigetfile::uigetfile(getScilabJavaVM(), mask, maskSize, NULL, 0, initialDirectory, titleBox);
	}
	else
	{
		Juigetfile::uigetfile(getScilabJavaVM(), mask, maskSize, description, maskSize, initialDirectory, titleBox);
	}
}

char * getJuigetfileTitleBox()
{
  return Juigetfile::getTitleBox(getScilabJavaVM());
}

char ** getJuigetfileMask()
{
  return Juigetfile::getMask(getScilabJavaVM());
}

char * getJuigetfileInitialDirectory()
{
  return Juigetfile::getInitialDirectory(getScilabJavaVM());
}

int getJuigetfileSelectionSize()
{
  return Juigetfile::getSelectionSize(getScilabJavaVM());
}

char ** getJuigetfileSelection()
{
  return Juigetfile::getSelection(getScilabJavaVM());
}

bool getJuigetfileMultipleSelection()
{
  return Juigetfile::isMultipleSelection(getScilabJavaVM());
}

char * getJuigetfileSelectionPathName()
{
  return Juigetfile::getSelectionPathName(getScilabJavaVM());
}

int getJuigetfileFilterIndex()
{
  return Juigetfile::getFilterIndex(getScilabJavaVM());
}

char * getJuigetfileMenuCallback()
{
  return Juigetfile::getMenuCallback(getScilabJavaVM());
}


void deleteMenuCallBack(char * menuCallBack)
{
  delete[] menuCallBack;
}
