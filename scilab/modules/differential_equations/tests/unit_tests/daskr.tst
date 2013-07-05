// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2013 - Paul Bignier
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING,
// which you should have received as part of this distribution.
// The terms are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// =============================================================================

//C-----------------------------------------------------------------------
//C First problem.
//C The initial value problem is..
//C   DY/DT = ((2*LOG(Y) + 8)/T - 5)*Y,  Y(1) = 1,  1 .LE. T .LE. 6
//C The solution is  Y(T) = EXP(-T**2 + 5*T - 4), YPRIME(1) = 3
//C The two root functions are..
//C   G1 = ((2*LOG(Y)+8)/T - 5)*Y (= DY/DT)  (with root at T = 2.5),
//C   G2 = LOG(Y) - 2.2491  (with roots at T = 2.47 and 2.53)
//C-----------------------------------------------------------------------
y0=1;t=2:6;t0=1;y0d=3;
info=list([],0,[],[],[],0,[],1,[],0,1,[],[],1);
atol=1.d-6;rtol=0;ng=2;
[yy,nn]=daskr([y0,y0d],t0,t,atol,rtol,'res1',ng,'gr1',info,'psol1','pjac1');
if abs(nn(1)-2.47)>0.001 then pause,end
y0=yy(2,2);y0d=yy(3,2);t0=nn(1);t=[3,4,5,6];
[yy,nn]=daskr([y0,y0d],t0,t,atol,rtol,'res1',ng,'gr1',info,'psol1','pjac1');
if abs(nn(1)-2.5)>0.001 then pause,end
y0=yy(2,1);y0d=yy(3,1);t0=nn(1);t=[3,4,5,6];
info=list([],0,[],[],[],0,[],0,[],0,0,[],[],1);
[yy,nn]=daskr([y0,y0d],t0,t,atol,rtol,'res1',ng,'gr1',info);
if abs(nn(1)-2.53)>0.001 then pause,end

// Same problem, but using macro for the derivative evaluation function 'res1'
deff('[delta,ires]=res1(t,y,ydot)','ires=0;delta=ydot-((2.*log(y)+8)./t-5).*y')
deff('[rts]=gr1(t,y,yd)','rts=[((2*log(y)+8)/t-5)*y;log(y)-2.2491]')

y0=1;t=2:6;t0=1;y0d=3;
atol=1.d-6;rtol=0;ng=2;
[yy,nn]=daskr([y0,y0d],t0,t,atol,rtol,res1,ng,gr1,info);
if abs(nn(1)-2.47)>0.001 then pause,end
y0=yy(2,2);y0d=yy(3,2);t0=nn(1);t=[3,4,5,6];
[yy,nn]=daskr([y0,y0d],t0,t,atol,rtol,res1,ng,gr1,info);
if abs(nn(1)-2.5)>0.001 then pause,end
y0=yy(2,1);y0d=yy(3,1);t0=nn(1);t=[3,4,5,6];
[yy,nn]=daskr([y0,y0d],t0,t,atol,rtol,res1,ng,gr1,info);
if abs(nn(1)-2.53)>0.001 then pause,end

// Same problem, but using macros for the preconditioner evaluation and application functions 'pjac' and 'psol'
// pjac uses the macro res1 defined above.
function [wp, iwp, ires] = pjac(neq, t, y, ydot, h, cj, rewt, savr)
    ires = 0;
    SQuround = 1.490D-08;
    tx = t;
    nrow = 0;
    e = zeros(1, neq);
    wp = zeros(neq*neq, 1);
    iwp = zeros(neq*neq, 2);
    for i=1:neq
        del = max(SQuround*max(abs(y(i)), abs(h*ydot(i))), 1/rewt(i))
        if h*ydot(i) < 0 then del = -del; end
        ysave = y(i);
        ypsave = ydot(i);
        y(i) = y(i) + del;
        ydot(i) = ydot(i) + cj*del;
        [e ires]=res1(tx, y, ydot);
        if ires < 0 then
            ires = -1;
            return;
        end
        delinv = 1/del;
        for j=1:neq
            wp(nrow+j) = delinv*(e(j)-savr(j));
            if isnan(wp(nrow+j)) then
                ires = -1;
                return;
            end
            iwp(nrow+j, 1) = i;
            iwp(nrow+j, 2) = j;
        end
        nrow = nrow + neq;
        y(i) = ysave;
        ydot(i) = ypsave;
    end
    clear e
endfunction
function [r, ier] = psol(wp, iwp, b)
    ier = 0;
    //Compute the LU factorization of R.
    sp = sparse(iwp, wp);
    [h, rk] = lufact(sp);
    //Solve the system LU*X = b
    r = lusolve(h, b);
    ludel(h);
    clear [sp rk]
endfunction

y0=1;t=2:6;t0=1;y0d=3;
info=list([],0,[],[],[],0,[],1,[],0,1,[],[],1);
atol=1.d-6;rtol=0;ng=2;
[yy,nn]=daskr([y0,y0d],t0,t,atol,rtol,res1,ng,'gr1',info,psol,pjac);
if abs(nn(1)-2.47)>0.001 then pause,end
y0=yy(2,2);y0d=yy(3,2);t0=nn(1);t=[3,4,5,6];
[yy,nn]=daskr([y0,y0d],t0,t,atol,rtol,res1,ng,'gr1',info,psol,pjac);
if abs(nn(1)-2.5)>0.001 then pause,end
y0=yy(2,1);y0d=yy(3,1);t0=nn(1);t=[3,4,5,6];
[yy,nn]=daskr([y0,y0d],t0,t,atol,rtol,res1,ng,'gr1',info,psol,pjac);
if abs(nn(1)-2.53)>0.001 then pause,end

//C
//C-----------------------------------------------------------------------
//C Second problem (Van Der Pol oscillator).
//C The initial value problem is..
//C   DY1/DT = Y2,  DY2/DT = 100*(1 - Y1**2)*Y2 - Y1,
//C   Y1(0) = 2,  Y2(0) = 0,  0 .LE. T .LE. 200
//C   Y1PRIME(0) = 0, Y2PRIME(0) = -2
//C The root function is  G = Y1.
//C An analytic solution is not known, but the zeros of Y1 are known
//C to 15 figures for purposes of checking the accuracy.
//C-----------------------------------------------------------------------
info=list([],0,[],[],[],0,[],0,[],0,0,[],[],1);
rtol=[1.d-6;1.d-6];atol=[1.d-6;1.d-4];
t0=0;y0=[2;0];y0d=[0;-2];t=[20:20:200];ng=1;
[yy,nn]=daskr([y0,y0d],t0,t,atol,rtol,'res2','jac2',ng,'gr2',info);
if abs(nn(1)-81.163512)>0.001 then pause,end

deff('[delta,ires]=res2(t,y,ydot)',...
'ires=0;y1=y(1),y2=y(2),delta=[ydot-[y2;100*(1-y1*y1)*y2-y1]]')
[yy,nn]=daskr([y0,y0d],t0,t,atol,rtol,res2,'jac2',ng,'gr2',info);
deff('J=jac2(t,y,ydot,c)','y1=y(1);y2=y(2);J=[c,-1;200*y1*y2+1,c-100*(1-y1*y1)]')
[yy,nn]=daskr([y0,y0d],t0,t,atol,rtol,res2,jac2,ng,'gr2',info);
deff('s=gr2(t,y,yd)','s=y(1)')
[yy,nn]=daskr([y0,y0d],t0,t,atol,rtol,res2,jac2,ng,gr2,info);

// Same problem, with psol and pjac example routines

info=list([],0,[],[],[],0,[],1,[],0,1,[],[],1);
[yy,nn]=daskr([y0,y0d],t0,t,atol,rtol,res2,jac2,ng,'gr2',info,'psol1','pjac1');
if abs(nn(1)-81.163512)>0.009 then pause,end
deff('s=gr2(t,y,yd)','s=y(1)')
[yy,nn]=daskr([y0,y0d],t0,t,atol,rtol,res2,jac2,ng,gr2,info,'psol1','pjac1');
if abs(nn(1)-81.163512)>0.009 then pause,end

// Same problem, with psol and pjac macros

// Redefine pjac to use res2
function [wp, iwp, ires] = pjac(neq, t, y, ydot, h, cj, rewt, savr)
    ires = 0;
    SQuround = 1.490D-08;
    tx = t;
    nrow = 0;
    e = zeros(1, neq);
    wp = zeros(neq*neq, 1);
    iwp = zeros(neq*neq, 2);
    for i=1:neq
        del = max(SQuround*max(abs(y(i)), abs(h*ydot(i))), 1/rewt(i))
        if h*ydot(i) < 0 then del = -del; end
        ysave = y(i);
        ypsave = ydot(i);
        y(i) = y(i) + del;
        ydot(i) = ydot(i) + cj*del;
        [e ires]=res2(tx, y, ydot);
        if ires < 0 then return; end
        delinv = 1/del;
        for j=1:neq
            wp(nrow+j) = delinv*(e(j)-savr(j));
            iwp(nrow+j,1) = i;
            iwp(nrow+j,2) = j;
        end
        nrow = nrow + neq;
        y(i) = ysave;
        ydot(i) = ypsave;
    end
endfunction
[yy,nn]=daskr([y0,y0d],t0,t,atol,rtol,res2,jac2,ng,'gr2',info,psol,pjac);
if abs(nn(1)-81.163512)>0.003 then pause,end
deff('s=gr2(t,y,yd)','s=y(1)')
[yy,nn]=daskr([y0,y0d],t0,t,atol,rtol,res2,jac2,ng,gr2,info,psol,pjac);
if abs(nn(1)-81.163512)>0.003 then pause,end
info=list([],0,[],[],[],0,[],0,[],0,0,[],[],1);

//           Hot Restart

[yy,nn,hotd]=daskr([y0,y0d],t0,t,atol,rtol,'res2','jac2',ng,'gr2',info);
t01=nn(1);t=100:20:200;[pp,qq]=size(yy);y01=yy(2:3,qq);y0d1=yy(3:4,qq);
[yy,nn,hotd]=daskr([y01,y0d1],t01,t,atol,rtol,'res2','jac2',ng,'gr2',info,hotd);
if abs(nn(1)-162.57763)>0.004 then pause,end

//Test of Dynamic link (Require f77!)
//         1 making the routines
res22=[...
'      SUBROUTINE RES22(T,Y,YDOT,DELTA,IRES,RPAR,IPAR)';
'      IMPLICIT DOUBLE PRECISION (A-H,O-Z)';
'      INTEGER NEQ';
'      DIMENSION Y(*), YDOT(*), DELTA(*)';
'      NEQ=2';
'      CALL F2(NEQ,T,Y,DELTA)';
'      DO 10 I = 1,NEQ';
'         DELTA(I) = YDOT(I) - DELTA(I)';
' 10   CONTINUE';
'      RETURN';
'      END';
'      SUBROUTINE F2 (NEQ, T, Y, YDOT)';
'      IMPLICIT DOUBLE PRECISION (A-H,O-Z)';
'      INTEGER NEQ';
'      DOUBLE PRECISION T, Y, YDOT';
'      DIMENSION Y(*), YDOT(*)';
'      YDOT(1) = Y(2)';
'      YDOT(2) = 100.0D0*(1.0D0 - Y(1)*Y(1))*Y(2) - Y(1)';
'      RETURN';
'      END';]

jac22=[...
'      SUBROUTINE JAC22 (T, Y, ydot, PD, CJ, RPAR, IPAR)';
' ';
'      IMPLICIT DOUBLE PRECISION (A-H,O-Z)';
'      INTEGER  NROWPD';
'      DOUBLE PRECISION T, Y, PD';
'      PARAMETER (NROWPD=2)';
'      DIMENSION Y(2), PD(NROWPD,2)';
'      PD(1,1) = 0.0D0';
'      PD(1,2) = 1.0D0';
'      PD(2,1) = -200.0D0*Y(1)*Y(2) - 1.0D0';
'      PD(2,2) = 100.0D0*(1.0D0 - Y(1)*Y(1))';
'      PD(1,1) = CJ - PD(1,1)';
'      PD(1,2) =    - PD(1,2)';
'      PD(2,1) =    - PD(2,1)';
'      PD(2,2) = CJ - PD(2,2)';
'      RETURN';
'      END';]


gr22=[...
'      SUBROUTINE GR22 (NEQ, T, Y, NG, GROOT, RPAR, IPAR)';
'      IMPLICIT DOUBLE PRECISION (A-H,O-Z)';
'      INTEGER NEQ, NG';
'      DOUBLE PRECISION T, Y, GROOT';
'      DIMENSION Y(*), GROOT(*)';
'      GROOT(1) = Y(1)';
'      RETURN';
'      END';]

//Uncomment lines below: link may be machine dependent if some f77 libs are
//needed for linking
//unix_g('cd /tmp;rm -f /tmp/res22.f');unix_g('cd /tmp;rm -f /tmp/gr22.f');
//unix_g('cd /tmp;rm -f /tmp/jac22.f');
//write('/tmp/res22.f',res22);write('/tmp/gr22.f',gr22);write('/tmp/jac22.f',jac22)
//unix_g("cd /tmp;make /tmp/res22.o");unix_g('cd /tmp;make /tmp/gr22.o');
//unix_g('cd /tmp;make /tmp/jac22.o');
//          2  Linking the routines
//link('/tmp/res22.o','res22');link('/tmp/jac22.o','jac22');link('/tmp/gr22.o','gr22')
//rtol=[1.d-6;1.d-6];atol=[1.d-6;1.d-4];
//t0=0;y0=[2;0];y0d=[0;-2];t=[20:20:200];ng=1;
//          3 Calling the routines by dasrt
//[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,'res22','jac22',ng,'gr22',info);
// hot restart
//[yy,nn,hotd]=dasrt([y0,y0d],t0,t,atol,rtol,'res22','jac22',ng,'gr22',info);
//t01=nn(1);t=100:20:200;[pp,qq]=size(yy);y01=yy(2:3,qq);y0d1=yy(3:4,qq);
//[yy,nn,hotd]=dasrt([y01,y0d1],t01,t,atol,rtol,'res22','jac22',ng,'gr22',info,hotd);
