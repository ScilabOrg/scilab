//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function [x,y,typ]=scifunc_block(job,arg1,arg2)
    //%Description
    // job=='set'        : block parameters acquisition
    //                    arg1 is block data structure
    //                    x is returned block data structure
    // job=='define'     : corresponding block data structure initialisation
    //                    arg1: name of block parameters acquisition macro
    //                    x   : block data structure
    //%Block data-structure definition
    // bl=list('Block',graphics,model,init,'standard_block')
    //  graphics=list([xo,yo],[l,h],orient,label)
    //          xo          - x coordinate of block origin
    //          yo          - y coordinate of block origin
    //          l           - block width
    //          h           - block height
    //          orient      - boolean, specifies if block is tilded
    //          label       - string block label
    //  model=list(eqns,#input,#output,#clk_input,#clk_output,state,..
    //             rpar,ipar,typ [,firing])
    //          eqns        - function name (in string form if fortran routine)
    //          #input      - vector of input port sizes
    //          #output     - vector of output port sizes
    //          #clk_input  - vector  of clock inputs port sizes
    //          #clk_output - vector  of clock output port sizes
    //          state       - vector (column) of initial condition
    //          rpar        - vector (column) of real parameters
    //          ipar        - vector (column) of integer parameters
    //          typ         - string: 'c' if block is continuous, 'd' if discrete
    //                        'z' if zero-crossing.
    //          firing      - vector of initial output event firing times
    //
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        needcompile=0
        x=arg1
        model=arg1.model;
        graphics=arg1.graphics;
        exprs=graphics.exprs

        if size(exprs(1),"*")==8 then
            exprs(1)(9)="0";
        end
        while %t do
            [ok,i,o,ci,co,xx,z,rpar,auto0,deptime,lab]=scicos_getvalue(..
            ["Set scifunc_block parameters";"only regular blocks supported"],..
            ["input ports sizes";
            "output port sizes";
            "input event ports sizes";
            "output events ports sizes";
            "initial continuous state";
            "initial discrete state";
            "System parameters vector";
            "initial firing vector (<0 for no firing)";
            "is block always active (0:no, 1:yes)"  ],..
            list("vec",-1,"vec",-1,"vec",-1,"vec",-1,"vec",-1,"vec",-1,..
            "vec",-1,"vec","sum(%4)","vec",1),exprs(1))
            if ~ok then
                break,
            end
            exprs(1)=lab
            xx=xx(:);
            z=z(:);
            rpar=rpar(:)
            nrp=prod(size(rpar))
            // create simulator
            i=int(i(:));
            ni=size(i,1);
            o=int(o(:));
            no=size(o,1);
            ci=int(ci(:));
            nci=size(ci,1);
            co=int(co(:));
            nco=size(co,1);
            [ok,tt,dep_ut]=genfunc1(exprs(2),i,o,nci,nco,size(xx,1),size(z,1),..
            nrp,"c")
            dep_ut(2)=(1==deptime)
            if ~ok then
                break,
            end
            [model,graphics,ok]=check_io(model,graphics,i,o,ci,co)
            if ok then
                auto=auto0
                model.state=xx
                model.dstate=z
                model.rpar=rpar
                if model.ipar <> 0 then
                    model.opar=model.ipar;
                    model.ipar=0;
                end
                if or(model.opar<>tt) then
                    needcompile=4,
                end
                model.opar=tt
                model.firing=auto
                model.dep_ut=dep_ut
                x.model=model
                exprs(2)=tt
                graphics.exprs=exprs
                x.graphics=graphics
                break
            end
        end
        needcompile=resume(needcompile)
    case "define" then
        in=1
        out=1
        clkin=[]
        clkout=[]
        x0=[]
        z0=[]
        typ="c"
        auto=[]
        rpar=[]

        model=scicos_model()
        model.sim=list("scifunc",3)
        model.in=in
        model.out=out
        model.evtin=clkin
        model.evtout=clkout
        model.state=x0
        model.dstate=z0
        model.rpar=rpar
        model.ipar=0
        model.opar=list()
        model.blocktype=typ
        model.firing=auto
        model.dep_ut=[%t %f]

        exprs=list([sci2exp(in);sci2exp(out);sci2exp(clkin);sci2exp(clkout);
        strcat(sci2exp(x0));strcat(sci2exp(z0));
        strcat(sci2exp(rpar));sci2exp(auto)],..
        list("y1=sin(u1)"," "," ","y1=sin(u1)"," "," "," "))
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
