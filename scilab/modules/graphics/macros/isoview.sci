// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2010 - DIGITEO - Pierre LANDO <pierre.lando@scilab.org>
// Copyright (C) 2015 - Samuel GOUGEON
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function []=isoview(varargin)
    // setting default plotting to be isoview

    rhs = argn(2)
    rhs0 = rhs

    // isoview action to be performed
    action = ""
    if rhs>0 & typeof(varargin($))=="string" then
        tmp = convstr(varargin($))
        if tmp(1)=="on"
            action = "on"
        elseif tmp(1)=="off"
            action = "off"
        end
        varargin($) = null()
        rhs = rhs-1
    end

    // Set of axes to be processed
    if rhs==0 | type(varargin($))~=9
        a = gca()
    else // varargin($) is a graphics handle
        a = varargin($)
        a0 = a
        while a.type~="Axes" & a.type~="Figure"
            a = a.parent
        end
        if a0.type=="Figure" then
            // All the axes of the figure must be isoviewed
            a = a.children
            a = a(find(a.type=="Axes"))
            //the given figure has no associated axes => do nothing
            if a==[]
                return
            end
        elseif a.type=="Figure"
            //the given handle has no associated axes ancestor => do nothing
            return
        end
        varargin($) = null()
    end

    if rhs==3 | rhs>8 then
        msg = _("%s: Wrong number of input argument(s): 0, 1, 2, 4 to 8 arguments expected.\n")
        error(msprintf(msg, "isoview"));
    end

    nb = lstsize(varargin)
    if rhs>3
        for i = 1:nb
            if type(varargin(i)) <> 1 then
                msg = _("%s: Wrong type for input argument #%d: A real expected.\n")
                error(msprintf(msg, "isoview", i))
            end
        end;
        [xmin xmax ymin ymax] = varargin(1:4)
        if nb>4
            zmin = varargin(5)
            if nb>5
                zmax = varargin(6)
            end
        end
    end

    // -------------------------
    for i = 1:size(a,"*")
        tmp = a(i).isoview
        if action=="on" | (action=="" & tmp=="off")
            a(i).isoview = "on"
        else
            a(i).isoview = "off"
        end
        a(i).clip_state = "clipgrf"
        if rhs>3 then
            b = a(i).data_bounds
            if ~isnan(xmin), b(1) = xmin, end
            if ~isnan(xmax), b(2) = xmax, end
            if ~isnan(ymin), b(3) = ymin, end
            if ~isnan(ymax), b(4) = ymax, end
            if a(i).view=="3d"
                if nb>4 & ~isnan(zmin), b(5) = zmin, end
                if nb>5 & ~isnan(zmax), b(6) = zmax, end
            end
            a(i).data_bounds = b
        end
    end
endfunction
