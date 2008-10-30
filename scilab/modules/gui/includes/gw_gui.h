/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GW_GUI__
#define __GW_GUI__
/*--------------------------------------------------------------------------*/
#include "machine.h"
/*--------------------------------------------------------------------------*/
int gw_gui(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_x_dialog)(char *fname,unsigned long fname_len);
int C2F(sci_x_choose)(char *fname,unsigned long fname_len);
int C2F(sci_x_mdialog)(char *fname,unsigned long fname_len);
int C2F(sci_x_choice)(char *fname,unsigned long fname_len);
int sci_delmenu(char *fname,unsigned long fname_len);
int sci_setmenu(char *fname,unsigned long fname_len);
int sci_unsetmenu(char *fname,unsigned long fname_len);
int C2F(sci_xgetfile)(char *fname,unsigned long fname_len);
int sci_uigetdir(char *fname,unsigned long l);
#include "sci_raise_window.h"
int sci_getlookandfeel( char * fname, unsigned long fname_len ) ;
int sci_getinstalledlookandfeels( char * fname, unsigned long fname_len ) ;
int sci_setlookandfeel( char * fname, unsigned long fname_len ) ;
int C2F(sci_ClipBoard)(char *fname,unsigned long l);
int sci_toolbar(char *fname,unsigned long l);
int sci_uicontrol(char *fname,unsigned long l);
int sci_uimenu(char *fname,unsigned long l);
int C2F(sci_x_choose_modeless)(char *fname,unsigned long fname_len);
int sci_messagebox(char *fname,unsigned long l);
int sci_waitbar(char *fname,unsigned long l);
int sci_progressionbar(char *fname,unsigned long l);
int sci_helpbrowser(char *fname,unsigned long l);
int sci_uigetfont(char *fname,unsigned long l);
int sci_uigetcolor(char *fname,unsigned long l);
int sci_getcallbackobject(char *fname,unsigned long fname_len);
#include "sci_uicontextmenu.h"
#include "sci_uiwait.h"
#include "sci_mpopup.h"
int sci_printfigure(char *fname,unsigned long fname_len);
#include "sci_exportUI.h"
int sci_printsetupbox(char *fname,unsigned long l);
int sci_toprint(char *fname,unsigned long l);
int sci_usecanvas(char *fname,unsigned long l);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_GUI__ */
/*--------------------------------------------------------------------------*/

