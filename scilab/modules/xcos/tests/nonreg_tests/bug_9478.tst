// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
// <-- NO CHECK ERROR OUTPUT -->
//
// <-- Non-regression test for bug 9478 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9478
//
// <-- Short Description -->
// depends en u/t field works as a row or a column


scs_m=scicos_diagram(..
      version="scicos4.2",..
      props=scicos_params(..
            wpar=[-31.207834,624.42151,62.495145,459.85324,894,544,84,72,725,544,299,51,1.4],..
            Title=["bug_9478",..
            "/home/davidcl/"],..
            tol=[0.0001;0.000001;1.000D-10;100001;0;0;0],..
            tf=210,..
            context=" ",..
            void1=[],..
            options=tlist(["scsopt","3D","Background","Link","ID","Cmap"],list(%t,33),[8,1],[1,5],..
            list([5,1],[4,1]),[0.8,0.8,0.8]),..
            void2=[],..
            void3=[],..
            doc=list()));
scs_m.objs(1)=scicos_block(..
              gui="CONST_m",..
              graphics=scicos_graphics(..
                       orig=[136.43864,240.71188],..
                       sz=[40,40],..
                       flip=%t,..
                       theta=0,..
                       exprs="0.1",..
                       pin=[],..
                       pout=11,..
                       pein=[],..
                       peout=[],..
                       gr_i=list(..
                       ["dx=sz(1)/5;dy=sz(2)/10;";
                       "w=sz(1)-2*dx;h=sz(2)-2*dy;";
                       "txt=C;";
                       "xstringb(orig(1)+dx,orig(2)+dy,txt,w,h,''fill'');"],8),..
                       id="",..
                       in_implicit=[],..
                       out_implicit="E"),..
              model=scicos_model(..
                       sim=list("cstblk4_m",4),..
                       in=[],..
                       in2=[],..
                       intyp=1,..
                       out=1,..
                       out2=1,..
                       outtyp=1,..
                       evtin=[],..
                       evtout=[],..
                       state=[],..
                       dstate=[],..
                       odstate=list(),..
                       rpar=[],..
                       ipar=[],..
                       opar=list(0.1),..
                       blocktype="d",..
                       firing=[],..
                       dep_ut=[%f;%f],..
                       label="",..
                       nzcross=0,..
                       nmode=0,..
                       equations=list()),..
              doc=list());


tmpFileName = TMPDIR + "/bug_9478_scs_m.sod";

assert_checktrue(export_to_hdf5(tmpFileName, "scs_m"))
assert_checktrue(isfile(tmpFileName))

xcos(tmpFileName)

