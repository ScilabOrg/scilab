(*************************************************************************
 *
 * This code comes from the Openoffice project and has been adapted for
 * Scilab.
 * For now, this script checks the minimal version of Mac OS X
 *
 * Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright 2008 by Sun Microsystems, Inc.
 *
 * OpenOffice.org - a multi-platform office productivity suite
 *
 * $RCSfile: main.applescript,v $
 * $Revision: 1.5 $
 *
 * This file is part of OpenOffice.org.
 *
 * OpenOffice.org is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 3
 * only, as published by the Free Software Foundation.
 *
 * OpenOffice.org is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Lesser General Public License
 * version 3 along with OpenOffice.org.  If not, see
 * <http://www.openoffice.org/license.html>
 * for a copy of the LGPLv3 License.
 *
 *************************************************************************)

on run argv
    tell application  "System Events" -- Mandatory to use tell application otherwis, osascript complains
        activate
        display dialog  "This version of Scilab will probably fail on this system (" & item 1 of argv & "): Scilab requires " & item 2 of argv & "." buttons {"Try anyway", "Quit"} default button "Try anyway"
        if the button returned of the result is "Quit" then
            error number 128
            return false
        else
            return true
        end if
        end tell
end run

