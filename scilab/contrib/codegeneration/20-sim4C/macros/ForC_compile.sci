//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>
//                      - Ramine Nikoukhah <ramine.nikoukhah@inria.fr>
//                      - Rachid Djenidi
//
//                      - Scilab 5 update by Simone Mannori 
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
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

//**---------------------------------------------------------------------------------------------------------------------------------



//
//16/06/07 Author : ?, A. Layec
//
function [Code,actt,proto]=call_actuator(i)
  nin=inpptr(i+1)-inpptr(i);  //** number of input ports
  nout=outptr(i+1)-outptr(i); //** number of output ports

  if funtyp(i)==0 then
    if nin==0 then
      uk    = 0;
      nuk_1 = 0;
      nuk_2 = 0;
      uk_t  = 1;
      //Code($+1)=+'  args[0]=(double *)outtbptr[0]);';
    else
      uk    = inplnk(inpptr(i));
      nuk_1 = size(outtb(uk),1);
      nuk_2 = size(outtb(uk),2);
      uk_t  = mat2scs_c_nb(outtb(uk));
      //Code($+1)=' args[0]=('+mat2scs_c_ptr(outtb(uk))+' *)outtbptr['+string(uk-1)+'];';
    end
  end
  //pour la fonction gui ont a : num de bloc l'adresse dans
  //nouveau  z et la taille du port
  actt=[i uk nuk_1 nuk_2 uk_t bllst(i).ipar]

  Code($+1)='block_'+rdnom+'['+string(i-1)+'].nevprt=nevprt;'

  Code=['/* Call of actuator (blk nb '+string(i)+') */'
        Code;
        'nport = '+string(nbact)+';';
        rdnom+'_actuator(&flag, &nport, &block_'+rdnom+'['+string(i-1)+'].nevprt, told, '+..
        '('+mat2scs_c_ptr(outtb(uk))+' *)outtbptr['+string(uk-1)+'], &nrd_'+string(nuk_1)+', &nrd_'+..
        string(nuk_2)+', &nrd_'+string(uk_t)+',bbb);'];

  proto='void '+rdnom+'_actuator('+..
        'int *, int *, int *, double *, void *, int *, int *,int *,int);'
  proto=cformatline(proto,70);
endfunction

//CallBlock : generate C calling sequence
//            of a scicos block
//
//inputs : bk   : bloc index
//         pt   :
//         flag :block_'+rdnom+'[
//
//output : txt  :
//
//16/06/07 Authors : R.Nikoukhah, A.Layec
function txt=call_block4(bk)
//   if flag==2 & ((zptr(bk+1)-zptr(bk))+(ozptr(bk+1)-ozptr(bk))==0 | pt<=0) then
//     return // block without discrete state or continuously activated
//            // If work allocated in the absence of z and oz, it does not work
//   end

  //**
  nx=xptr(bk+1)-xptr(bk);
  nz=zptr(bk+1)-zptr(bk);
  nrpar=rpptr(bk+1)-rpptr(bk);
  nipar=ipptr(bk+1)-ipptr(bk);
  nin=inpptr(bk+1)-inpptr(bk);  //* number of input ports */
  nout=outptr(bk+1)-outptr(bk); //* number of output ports */

  //**
  //l'adresse du pointeur de ipar
  if nipar<>0 then ipar=ipptr(bk), else ipar=1;end
  //l'adresse du pointeur de rpar
  if nrpar<>0 then rpar=rpptr(bk), else rpar=1; end
  //l'adresse du pointeur de z attention -1 pas sur
  if nz<>0 then z=zptr(bk)-1, else z=0;end
  //l'adresse du pointeur de x
  if nx<>0 then x=xptr(bk)-1, else x=0;end

  //**
  ftyp=funtyp(bk)
  if ftyp>2000 then ftyp=ftyp-2000,end
  if ftyp>1000 then ftyp=ftyp-1000,end

  //** check function type
  if ftyp < 0 then //** ifthenelse eselect blocks
      txt = [];
      return;
  else
    if (ftyp<>0 & ftyp<>1 & ftyp<>2 & ftyp<>3 & ftyp<>4) then
      disp("types other than 0,1,2,3 or 4 are not supported.")
      txt = [];
      return;
    end
  end

  //** add comment
  txt=[get_comment('call_blk',list(funs(bk),funtyp(bk),bk));]

  //** write nevprt activation
//   nclock=abs(pt);
  txt=[txt;
       'block_'+rdnom+'['+string(bk-1)+'].nevprt=nevprt;']

  select ftyp
    //** zero funtyp
    case 0 then
      //**** input/output addresses definition ****//
      if nin>1 then
        for k=1:nin
          uk=inplnk(inpptr(bk)-1+k);
          nuk=size(outtb(uk),'*');
          TYPE=mat2scs_c_ptr(outtb(uk));//scilab index start from 1
          txt=[txt;
               'rdouttb['+string(k-1)+']=('+TYPE+' *)'+XX_VARPREFIX+'outtbptr['+string(uk-1)+'];']
        end
        txt=[txt;
             'args[0]=&(rdouttb[0]);']
      elseif nin==0
        uk=0;
        nuk=0;
        txt=[txt;
             'args[0]=(SCSREAL_COP *)'+XX_VARPREFIX+'outtbptr[0];']
      else
        uk=inplnk(inpptr(bk));
        nuk=size(outtb(uk),'*');
        TYPE=mat2scs_c_ptr(outtb(uk));//scilab index start from 1
        txt=[txt;
             'args[0]=('+TYPE+' *)'+XX_VARPREFIX+'outtbptr['+string(uk-1)+'];']
      end

      if nout>1 then
        for k=1:nout
          yk=outlnk(outptr(bk)-1+k);
          nyk=size(outtb(yk),'*');
          TYPE=mat2scs_c_ptr(outtb(yk));//scilab index start from 1
          txt=[txt;
               'rdouttb['+string(k+nin-1)+']=('+TYPE+' *)'+XX_VARPREFIX+'outtbptr['+string(yk-1)+'];'];
        end
        txt=[txt;
             'args[1]=&(rdouttb['+string(nin)+']);'];
      elseif nout==0
        yk=0;
        nyk=0;
        txt=[txt;
             'args[1]=(SCSREAL_COP *)'+XX_VARPREFIX+'outtbptr[0];'];
      else
        yk=outlnk(outptr(bk));
        nyk=size(outtb(yk),'*'),;
        TYPE=mat2scs_c_ptr(outtb(yk));//scilab index start from 1
        txt=[txt;
             'args[1]=('+TYPE+' *)'+XX_VARPREFIX+'outtbptr['+string(yk-1)+'];'];
      end
      //*******************************************//

      //*********** call seq definition ***********//
      txtc=['(&flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,told,block_'+rdnom+'['+string(bk-1)+'].xd, \';
            'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
            'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
            '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
            'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar, \';
            '(double *)args[0],&nrd_'+string(nuk)+',(double *)args[1],&nrd_'+string(nyk)+');'];
      if (funtyp(bk)>2000 & funtyp(bk)<3000)
        blank = get_blank(funs(bk)+'( ');
        txtc(1) = funs(bk)+txtc(1);
      elseif (funtyp(bk)<2000)
        txtc(1) = 'C2F('+funs(bk)+')'+txtc(1);
        blank = get_blank('C2F('+funs(bk)+') ');
      end
      txtc(2:$) = blank + txtc(2:$);
      txt = [txt;txtc];
      //*******************************************//


    //**
    case 1 then
      //**** input/output addresses definition ****//
//       if nin>=1 then
//         for k=1:nin
//           uk=inplnk(inpptr(i)-1+k);
//           nuk=size(outtb(uk),'*');
//         end
//       end
//       if nout>=1 then
//         for k=1:nout
//           yk=outlnk(outptr(i)-1+k);
//           nyk=size(outtb(yk),'*');
//         end
//       end
      //*******************************************//

      //*********** call seq definition ***********//
      txtc=['(&flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,told,block_'+rdnom+'['+string(bk-1)+'].xd, \';
            'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
            'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
            '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
            'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar'];
      if (funtyp(bk)>2000 & funtyp(bk)<3000)
        blank = get_blank(funs(bk)+'( ');
        txtc(1) = funs(bk)+txtc(1);
      elseif (funtyp(bk)<2000)
        txtc(1) = 'C2F('+funs(bk)+')'+txtc(1);
        blank = get_blank('C2F('+funs(bk)+') ');
      end
      if nin>=1 | nout>=1 then
        txtc($)=txtc($)+', \'
        txtc=[txtc;'']
        if nin>=1 then
          for k=1:nin
            uk=inplnk(inpptr(bk)-1+k);
            nuk=size(outtb(uk),'*');
            txtc($)=txtc($)+'(SCSREAL_COP *)'+XX_VARPREFIX+'outtbptr['+string(uk-1)+'],&nrd_'+string(nuk)+',';
          end
          txtc($)=part(txtc($),1:length(txtc($))-1); //remove last ,
        end
        if nout>=1 then
          if nin>=1 then
            txtc($)=txtc($)+', \'
            txtc=[txtc;'']
          end
          for k=1:nout
            yk=outlnk(outptr(bk)-1+k);
            nyk=size(outtb(yk),'*');
            txtc($)=txtc($)+'(SCSREAL_COP *)'+XX_VARPREFIX+'outtbptr['+string(yk-1)+'],&nrd_'+string(nyk)+',';
          end
          txtc($)=part(txtc($),1:length(txtc($))-1); //remove last ,
        end
      end

      if ztyp(bk) then
        txtc($)=txtc($)+', \'
        txtc=[txtc;'w,&nrd_0);'];
      else
        txtc($)=txtc($)+');';
      end

      txtc(2:$) = blank + txtc(2:$);
      txt = [txt;txtc];
      //*******************************************//

    //**
    case 2 then

      //*********** call seq definition ***********//
      txtc=[funs(bk)+'(&flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,told,block_'+rdnom+'['+string(bk-1)+'].xd, \';
            'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
            'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
            '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
            'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar, \';
            '(double **)block_'+rdnom+'['+string(bk-1)+'].inptr,block_'+rdnom+'['+string(bk-1)+'].insz,&block_'+rdnom+'['+string(bk-1)+'].nin, \';
            '(double **)block_'+rdnom+'['+string(bk-1)+'].outptr,block_'+rdnom+'['+string(bk-1)+'].outsz, &block_'+rdnom+'['+string(bk-1)+'].nout'];
      if ~ztyp(bk) then
        txtc($)=txtc($)+');';
      else
        txtc($)=txtc($)+', \';
        txtc=[txtc;
              'block_'+rdnom+'['+string(bk-1)+'].g,&block_'+rdnom+'['+string(bk-1)+'].ng);']
      end
      blank = get_blank(funs(bk)+'( ');
      txtc(2:$) = blank + txtc(2:$);
      txt = [txt;txtc];
      //*******************************************//

    //**
    case 4 then
      txt=[txt;
           funs(bk)+'(&block_'+rdnom+'['+string(bk-1)+'],flag);'];

  end
endfunction

//
//16/06/07 Author : ?, A. Layec
//
function [Code,capt,proto]=call_sensor(i)
  nin=inpptr(i+1)-inpptr(i); ///* number of input ports */
  nout=outptr(i+1)-outptr(i); ///* number of output ports */

  //declaration des I/O des blocs de type 1
  if funtyp(i)==0 then
    if nout==0 then
      yk    = 0;
      nyk_1 = 0;
      nyk_2 = 0;
      yk_t  = 1;
      //Code($+1)=+'  args[1]=(double *)(outtbptr[0]);';
    else
      yk    = outlnk(outptr(i));
      nyk_1 = size(outtb(yk),1);
      nyk_2 = size(outtb(yk),2);
      yk_t  = mat2scs_c_nb(outtb(yk));
      //Code($+1)='  args[1]=('+mat2scs_c_ptr(outtb(yk))+' *)(outtbptr['+string(yk-1)+']);';
    end

  end
  capt=[i yk nyk_1 nyk_2 yk_t bllst(i).ipar]
  Code($+1)='block_'+rdnom+'['+string(i-1)+'].nevprt=nevprt;'
  Code=['/* Call of sensor (blk nb '+string(i)+') */'
        Code;
        'nport = '+string(nbcap)+';';
        rdnom+'_sensor(&flag, &nport, &block_'+rdnom+'['+string(i-1)+'].nevprt, '+..
        'told, ('+mat2scs_c_ptr(outtb(yk))+' *)(outtbptr['+string(yk-1)+']), &nrd_'+string(nyk_1)+..
        ', &nrd_'+string(nyk_2)+', &nrd_'+string(yk_t)+',aaa);'];
  proto='void '+rdnom+'_sensor('+..
        'int *, int *, int *, double *,void *, int *, int *,int *, int);'
  proto=cformatline(proto,70);
endfunction

function  [ok,XX,alreadyran,flgcdgen,szclkINTemp,freof] = ForC_compile(XX,all_scs_m,numk,alreadyran)
// Transforms a given Scicos discrete and continuous SuperBlock into a C defined Block

  scs_m=XX.model.rpar
  par=scs_m.props;	
  if alreadyran then
    //terminate current simulation
    do_terminate()
    alreadyran=%f
  end
  hname=scs_m.props.title(1) //superblock name
  //***********************************************************
  //Check blocks properties and adapt them if necessary
  //***********************************************************

  IN=[];OUT=[];clkIN=[];clkOUT=[];numa=[];numc=[];
  for i=1:size(scs_m.objs)
    if typeof(scs_m.objs(i))=='Block' then
      if scs_m.objs(i).gui=='CLKOUT_f' then
        ok=%f;%cpr=list()
        message('Superblock should not have any activation output port.')
        return
      elseif scs_m.objs(i).gui=='IN_f' then
        //replace input ports by sensor blocks
        numc=numc+1
        scs_m.objs(i).gui='INPUTPORTEVTS';
        scs_m.objs(i).model.evtin=1
        scs_m.objs(i).model.sim(1)='capteur'+string(numc)
        IN=[IN scs_m.objs(i).model.ipar]
      elseif scs_m.objs(i).gui=='OUT_f' then
        //replace output ports by actuator blocks
        numa=numa+1
        scs_m.objs(i).gui='OUTPUTPORTEVTS';
        scs_m.objs(i).model.sim(1)='actionneur'+string(numa)
        OUT=[OUT  scs_m.objs(i).model.ipar]
      elseif scs_m.objs(i).gui=='CLKINV_f' then
        //replace event input ports by  fictious block
        scs_m.objs(i).gui='EVTGEN_f';
        scs_m.objs(i).model.sim(1)='bidon'
        clkIN=[clkIN scs_m.objs(i).model.ipar];
        //elseif scs_m.objs(i).model.dep_ut(2)==%t then
        //check for time dependency PAS IICI
        //ok=%f;%cpr=list()
        //message('a block have time dependence.')
        //return
      elseif scs_m.objs(i).gui=='CLKOUTV_f' then
        scs_m.objs(i).gui='EVTOUT_f';
        scs_m.objs(i).model.sim(1)='bidon2'
        clkOUT=[clkOUT scs_m.objs(i).model.ipar];
      end
    end
  end

  //Check if input/output ports are numered properly
  IN=-gsort(-IN);
  if or(IN<>[1:size(IN,'*')]) then
    ok=%f;%cpr=list()
    message('Input ports are not numbered properly.')
    return
  end
  OUT=-gsort(-OUT);
  if or(OUT<>[1:size(OUT,'*')]) then
    ok=%f;%cpr=list()
    message('Output ports are not numbered properly.')
    return
  end
  clkIN=-gsort(-clkIN);
  if or(clkIN<>[1:size(clkIN,'*')]) then
    ok=%f;%cpr=list()
    message('Event input ports are not numbered properly.')
    return
  end
  clkOUT=-gsort(-clkOUT);
  if or(clkOUT<>[1:size(clkOUT,'*')]) then
    ok=%f;%cpr=list()
    message('Event output ports are not numbered properly.')
    return
  end

  //Check if there is more than one clock in the diagram
  szclkIN=size(clkIN,2);
  if szclkIN==0 then
    szclkIN=[]
  end
  flgcdgen=szclkIN;
  [bllst,connectmat,clkconnect,cor,corinv,ok,scs_m,flgcdgen,freof]=c_pass1(scs_m,flgcdgen);
  if flgcdgen<> szclkIN
    clkIN=[clkIN flgcdgen]
  end
  szclkINTemp=szclkIN;
  szclkIN=flgcdgen;
 // [bllst,connectmat,clkconnect,cor,corinv,ok]=c_pass1(scs_m);

  //Test for  ALWAYS_ACTIVE sblock (RN -25/06/07)
  ALWAYS_ACTIVE=%f;
  for blki=bllst
    if blki.dep_ut($) then
      ALWAYS_ACTIVE=%t;
      break;
    end
  end
  if ALWAYS_ACTIVE then
    CAPTEURS=[];
    for Ii=1:length(bllst)
      if part(bllst(Ii).sim(1),1:7)=='capteur' then
        bllst(Ii).dep_ut($)=%t
//       elseif part(bllst(Ii).sim(1),1:10)=='actionneur' then
//         bllst(Ii).dep_ut($)=%t
      end
    end
  end

  if ~ok then
    message('Sorry: problem in the pre-compilation step.')
    return
  end
  a=[];
  b=[];
  tt=[];
  howclk=[];
  allhowclk=[];
  allhowclk2=[];
  cap=[];
  act=[];

  ///**********************************
  for i=1:size(bllst)
    for j=1:size(bllst)
      if (bllst(i).sim(1)=='actionneur'+string(j)) then
        if tt<>i then
          act=[act;i];
          tt=i;
        end
      elseif (bllst(i).sim(1)=='capteur'+string(j)) then
        if tt<>i then
          cap=[cap;i];
          tt=i;
        end
      elseif (bllst(i).sim(1)=='bidon') then
        if tt<>i then
          allhowclk=[allhowclk;i];
          tt=i;
        end
      elseif (bllst(i).sim(1)=='bidon2') then
        if tt<>i then
          allhowclk2=[allhowclk2;i];
          tt=i;
        end
      end
    end
  end
  ///**********************************

  if szclkIN>1 then
    //replace the N Event inputs by a fictious block with 2^N as many event
    //outputs
    output=ones((2^szclkIN)-1,1)
    bllst($+1)=scicos_model(sim=list('bidon',1),evtout=output,..
                            blocktype='d',..
                            firing=-output',dep_ut=[%f %f])
    corinv(size(bllst))=size(bllst)+1;
    howclk=size(bllst)
    // adjust the links accordingly
    for i=1:(2^szclkIN)-1
      vec=codebinaire(i,szclkIN)
      for j=1:szclkIN
        if vec(j)*allhowclk(j)>=1 then
          for k=1:size(clkconnect,1)
            if clkconnect(k,1)==allhowclk(j) then
              clkconnect=[clkconnect;[howclk i clkconnect(k,3:4)]]
            end
          end
        end
      end
    end
  elseif szclkIN==[]&~ALWAYS_ACTIVE then
    //superblock has no Event input, add a fictious clock
    output=ones((2^(size(cap,'*')))-1,1)
    if (output == []) then
      output=0;
    end
    bllst($+1)=scicos_model(sim=list('bidon',1),evtout=output,..
                            firing=-output,blocktype='d',dep_ut=[%f %f])
    corinv(size(bllst))=size(bllst)+1;
    howclk=size(bllst);
  elseif szclkIN==1  then
    howclk=allhowclk;
  end

  //mise en ordre de clkconnect
  if szclkIN>1 then
    newclkconnect=clkconnect;
    clkconnect=[];
    for i=1:size(newclkconnect,1)-1
      if or(newclkconnect(i,:)<>newclkconnect(i+1,:)) then
        clkconnect=[clkconnect;newclkconnect(i,:)]
      end
    end
    if or(newclkconnect($-1,:)<>newclkconnect($,:)) then
      clkconnect=[clkconnect;newclkconnect($,:)]
    end

    //suppression des blocs bidons
    newclkconnect=clkconnect;nkt=[];
    for i=1:szclkIN
      for k=1:size(newclkconnect,1)
        if newclkconnect(k,1)~=allhowclk(i) then
          nkt=[nkt;newclkconnect(k,:)];
        end
      end
      newclkconnect=nkt;
      nkt=[];
    end
    clkconnect=newclkconnect;
  end

  //**************************************************
  // nouveau clkconnect avec liaisons sur les capteurs
  //**************************************************
  //Test for  ALWAYS_ACTIVE sblock (RN -25/06/07)
  if ~(ALWAYS_ACTIVE&szclkIN==[]) then
    n=size(cap,1)
    for i=1:n
      if szclkIN>1 then
        for j=1:(2^szclkIN)-1
          clkconnect=[clkconnect;[howclk j cap(i) 1]];
        end
      elseif szclkIN==1 then
        clkconnect=[clkconnect;[howclk 1 cap(i) 1]];
      end
    end

    //codage de l'activation des capteurs dans le cas de l'heritage
    for i=1:2^n-1
      vec=codebinaire(i,n);
      for j=1:n
        if (vec(j)==1) then
          clkconnect=[clkconnect;[howclk i cap(j) 1]];
        end
      end
    end
  end

  FIRING=[]
  for i=1:size(allhowclk2,1)
    j = find(clkconnect(:,3)==allhowclk2(i))
    if j<>[] then
      FIRING=[FIRING;bllst(clkconnect(j,1)).firing(clkconnect(j,2))]
    end
  end

  Code_gene_run=[];

  //** OLD GRAPHICS 
  //** %windo=xget('window')

  cpr=c_pass2(bllst,connectmat,clkconnect,cor,corinv)

  if cpr==list() then ok=%f,return, end

  //** Alan's patch 5/07/07: try to solve
  //   which blocks use work
  funs_save=cpr.sim.funs;
  funtyp_save=cpr.sim.funtyp;
  with_work = zeros(cpr.sim.nblk,1)
  for i=1:lstsize(cpr.sim.funs)
    if part(cpr.sim.funs(i),1:10)=='actionneur' then
       cpr.sim.funs(i) ='bidon'
       cpr.sim.funtyp(i) = 1
    elseif part(cpr.sim.funs(i),1:7)=='capteur' then
       cpr.sim.funs(i) ='bidon'
       cpr.sim.funtyp(i) = 1
    end
  end

  //**
  //** retrieve all open Scilab windows with winsid()
  //**

  BeforeCG_WinList = winsid(); 

  ierr=execstr('[state,t]=scicosim(cpr.state,0,0,cpr.sim,'+..
               '''start'',scs_m.props.tol)','errcatch')
  if ierr==0 then
    for i=1:cpr.sim.nblk
       if state.iz(i)<>0 then
          with_work(i)=%t
       end
    end
    ierr=execstr('[state,t]=scicosim(state,0,0,cpr.sim,'+..
                 '''finish'',scs_m.props.tol)','errcatch')
  end

  //**
  //** retrieve all open Scilab windows with winsid
  //** and close the additional windows open since first
  //** 
 
  //** This code does not cover 100% of the possible situations because the user can 
  //** destroy one or more Scicos wins manually during this intermediate phase
  //** This code is 100% functional if the the user does not close manually any win.
  //** TO BE updated in Scilab 5.0

  AfterCG_WinList = winsid();
  
  AfterCG_size = size(AfterCG_WinList); //** matrix
  AfterCG_size = AfterCG_size(2) ; //** vector lenght 

  BeforeCG_size = size(BeforeCG_WinList); //** matrix
  BeforeCG_size = BeforeCG_size(2) ; //** vector lenght

  if (or(AfterCG_WinList<>BeforeCG_WinList)) & (AfterCG_size>BeforeCG_size) then
     //** means that a scope or other scicos object has created some
     //** output window

     DiffCG_Winlist = AfterCG_WinList<>BeforeCG_WinList ; //** T/F mismatch 
     DiffCG_Index = find(DiffCG_Winlist); //** recover the mismatched indexes only 

     for win_idx = DiffCG_Index
         delete( scf( AfterCG_WinList(win_idx) ) ) ; //** clear the spurious windows   
     end 
     
  end  
  //**------------- end of windows cleaning ----------------------------------------

  cpr.sim.funs=funs_save;
  cpr.sim.funtyp=funtyp_save;

  //** OLD GRAPHICS 
  //** xset('window',%windo)

  ///////////////////
  //les pointeurs de cpr :
  x=cpr.state.x;
  z=cpr.state.z;
  outtb=cpr.state.outtb;

  //RN
  zcptr=cpr.sim.zcptr;
  ozptr=cpr.sim.ozptr;
  rpptr=cpr.sim.rpptr;
  ipptr=cpr.sim.ipptr;
  opptr=cpr.sim.opptr;
  funs=cpr.sim.funs;
  xptr=cpr.sim.xptr;
  zptr=cpr.sim.zptr;
  inpptr=cpr.sim.inpptr;
  inplnk=cpr.sim.inplnk;
  outptr=cpr.sim.outptr;
  outlnk=cpr.sim.outlnk;
  //@l@n lnkptr=cpr.sim.lnkptr;
  ordclk=cpr.sim.ordclk;
  funtyp=cpr.sim.funtyp;
  cord=cpr.sim.cord;
  ncord=size(cord,1);
  nblk=cpr.sim.nb;
  ztyp=cpr.sim.ztyp;
  clkptr=cpr.sim.clkptr
  //taille totale de z : nztotal
  nztotal=size(z,1);

  //*******************************
  //Checking if superblock is valid
  //*******************************
  msg=[]
  for i=1:length(funs)-1
    if funtyp(i)==3 then
      msg=[msg;'Type 3 block''s not allowed']
//RN   elseif ztyp(i)<>0 then
    //elseif (zcptr(i+1)-zcptr(i))<>0 then
//
      //msg=[msg;'Active zero crossing block''s not allowed']
    elseif (clkptr(i+1)-clkptr(i))<>0 &funtyp(i)>-1 &funs(i)~='bidon' then
      msg=[msg;'Regular block generating activation not allowed yet']
    end
    if msg<>[] then message(msg),ok=%f,return,end
  end

  //********************************************************
  // Change logical units for readf and writef blocks if any ???
  //********************************************************
  lunit=0
  for d=1:length(funs)
    if funs(d)=='readf'  then
      z(zptr(d)+2)=lunit
      lunit=lunit+1;
    elseif funs(d)=='writef'
      z(zptr(d)+1)=lunit
      lunit=lunit+1;
    end
  end

  //***********************************
  // Get the name of the file
  //***********************************
      disp("size of codegeneration is: "+string(size(codegeneration)));
    rdnom = codegeneration(1);
    disp(rdnom);
    rpat= codegeneration(2);
    disp(rpat);


    dirinfo=fileinfo(rpat)

    if dirinfo==[] then
      [pathrp,fnamerp,extensionrp]=fileparts(rpat)
      ok=mkdir(pathrp,fnamerp+extensionrp)
      if ~ok then 
        messagebox('Directory '+rpat+' cannot be created',"modal","info");
      end
    elseif filetype(dirinfo(2))<>'Directory' then
      ok=%f;
      messagebox(rpat+' is not a directory',"modal","error");
    end

    if stripblanks(rdnom)==emptystr() then 
      ok=%f;
      messagebox('sorry C file name not defined',"modal","error");
    end

  //////////////////////////////////////////////////
  maxnrpar=max(rpptr(2:$)-rpptr(1:$-1))
  maxnipar=max(ipptr(2:$)-ipptr(1:$-1))
  maxnx=max(xptr(2:$)-xptr(1:$-1))
  maxnz=max(zptr(2:$)-zptr(1:$-1))
  maxnin=max(inpptr(2:$)-inpptr(1:$-1))
  maxnout=max(outptr(2:$)-outptr(1:$-1))
  maxdim=[];
  for i=1:lstsize(cpr.state.outtb)
    maxdim=max(size(cpr.state.outtb(i)))
  end
  maxtotal=max([maxnrpar;maxnipar;maxnx;maxnz;maxnin;maxnout;maxdim]);

  //************************************************************************
  //generate the call to the blocks and blocs simulation function prototypes
  //************************************************************************
  wfunclist=list();
  nbcap=0;nbact=0;capt=[];actt=[];Protostalone=[];Protos=[];
  dfuns=[]
  for i=1:length(funs)
    if or(i==act) then //block is an actuator
      nbact=nbact+1;
      [Code,actti,protoi]=call_actuator(i)
      wfunclist($+1)=[Code;'if(flag < 0 ) return(5 - flag);']
      if nbact==1 then Protostalone=[Protostalone;'';protoi],end
      actt=[actt;actti]
    elseif or(i==cap) then //block is a sensor
      nbcap=nbcap+1;
      [Code,capti,protoi]=call_sensor(i)
      wfunclist($+1)=[Code;'if(flag < 0 ) return(5 - flag);']
      if nbcap==1 then Protostalone=[Protostalone;'';protoi] ,end
      capt=[capt;capti]
    elseif funs(i)=='bidon'
      wfunclist($+1)=' '
    elseif funs(i)=='bidon2'
      wfunclist($+1)=' '
    else
      ki=find(funs(i)==dfuns)
      dfuns=[dfuns;funs(i)]
      //[Code,protoi]=call_block(i)
      [Code]=call_block4(i)
      if Code<>[] then
        wfunclist($+1)=[Code;'if(flag < 0 ) return(5 - flag);']
      else
        wfunclist($+1)=' ';
      end
      if ki==[] then
        Protos=[Protos;'';BlockProto(i)];
        Protostalone=[Protostalone;'';BlockProto(i)];
      end
    end
  end

  //**************************************
  //on prend en compte l'ordre des numerotation graphique des
  //capteur et des actionneur
  [junk,index]=gsort(-actt(:,$));
  actt=actt(index,1:$) ;
  [junk,index]=gsort(-capt(:,$));
  capt=capt(index,1:$) ;

  //***************************************************
  //Compute the initial state and outtb (links)  values
  //***************************************************
  //petite modification pour uniformiser les fichiers capteurs
  //et actionneurs ???
  rdcpr=cpr.sim.funs;
  for r=1:length(cap),rdcpr(cap(r))='bidon';end
  for r=1:length(act),rdcpr(act(r))='bidon';end
  Total_rdcpr=cpr.sim;Total_rdcpr.funs=rdcpr;
  //
  tcur=0;
  tf=scs_m.props.tf;
  tolerances=scs_m.props.tol;
  
  z=cpr.state.z;
  outtb=cpr.state.outtb;

  //** Create source directory
  mkdir(rpat+"/src");
  
  //cd to projects source directory
  cd(rpat+'/src');
  
  //** Generate the 20-sim 4C meta model file
  [nrParameters, nrInitialValues, nrVariables, nrStates, nrRates]=generateMCF();
  
  //** Generate main file
  ok=generateMain();
  
  //** Generate model file
  ok=generateModel(nrParameters, nrInitialValues, nrVariables, nrStates, nrRates);
  
  //** Generate 20-sim 4C inter-process communication (IVC IPC)
  ok=generateIvcIpc(nrParameters, nrInitialValues, nrVariables, nrStates, nrRates);
    
  //Copy additional required files from contrib/20-sim4C/src to <target path>/src directory
  //Copy all files direct into src directory
  copyfile(SCI+"/contrib/20-sim4C/src/includes", rpat+"/src");
  
  //Copy the functions used from scicos subdirectory to the /src directory
  //Removed since those functions are in the library
  //for i=1:size(dfuns, '*')
  //	if(isfile(SCI+"/contrib/20-sim4C/src/scicos_blocks/c/"+dfuns(i)+".c"))
  //		copyfile(SCI+"/contrib/20-sim4C/src/scicos_blocks/c/"+dfuns(i)+".c", rpat+"/src/scicos_func_"+dfuns(i)+".c");
  //	else
  //		disp(SCI+"/contrib/20-sim4C/src/scicos_blocks/c/"+dfuns(i)+".c does not exist!");
  //	end
  //end
  
  if ok then
  
    //Start 20-sim 4C
	dos([''"C:\Program Files\20-sim 4C 2.0\bin\20SIMParser.exe'" '"'+rpat+''"']);
	
	//cd back
	cd(rpat);
  end

endfunction

//BlockProto : generate prototype
//            of a scicos block
//
//inputs : bk   : bloc index
//
//output : txt  :
//
//16/06/07 Author : A.Layec
function [txt]=BlockProto(bk)

  nin=inpptr(bk+1)-inpptr(bk);  //* number of input ports */
  nout=outptr(bk+1)-outptr(bk); //* number of output ports */

  //**
  ftyp=funtyp(bk)
  if ftyp>2000 then ftyp=ftyp-2000,end
  if ftyp>1000 then ftyp=ftyp-1000,end

  //** check function type
  if ftyp < 0 then //** ifthenelse eselect blocks
      txt = [];
      return;
  else
    if (ftyp<>0 & ftyp<>1 & ftyp<>2 & ftyp<>3 & ftyp<>4) then
      disp("types other than 0,1,2,3 or 4 are not yet supported.")
      txt = [];
      return;
    end
  end

  //** add comment
  txt=[get_comment('proto_blk',list(funs(bk),funtyp(bk),bk));]

  select ftyp
    //** zero funtyp
    case 0 then

      //*********** prototype definition ***********//
      txtp=['(int *, int *, double *, double *, double *, int *, double *, \';
            ' int *, double *, int *, double *, int *,int *, int *, \';
            ' double *, int *, double *, int *);'];
      if (funtyp(bk)>2000 & funtyp(bk)<3000)
        blank = get_blank('void '+funs(bk)+'(');
        txtp(1) = 'void '+funs(bk)+txtp(1);
      elseif (funtyp(bk)<2000)
        txtp(1) = 'void C2F('+funs(bk)+')'+txtp(1);
        blank = get_blank('void C2F('+funs(bk)+')');
      end
      txtp(2:$) = blank + txtp(2:$);
      txt = [txt;txtp];
      //*******************************************//


    //**
    case 1 then

      //*********** prototype definition ***********//
      txtp=['(int *, int *, double *, double *, double *, int *, double *, \';
            ' int *, double *, int *, double *, int *,int *, int *';]
      if (funtyp(bk)>2000 & funtyp(bk)<3000)
        blank = get_blank('void '+funs(bk)+'(');
        txtp(1) = 'void '+funs(bk)+txtp(1);
      elseif (funtyp(bk)<2000)
        txtp(1) = 'void C2F('+funs(bk)+')'+txtp(1);
        blank = get_blank('void C2F('+funs(bk)+')');
      end
      if nin>=1 | nout>=1 then
        txtp($)=txtp($)+', \'
        txtp=[txtp;'']
        if nin>=1 then
          for k=1:nin
            txtp($)=txtp($)+' double *, int * ,'
          end
          txtp($)=part(txtp($),1:length(txtp($))-1); //remove last ,
        end
        if nout>=1 then
          if nin>=1 then
            txtp($)=txtp($)+', \'
            txtp=[txtp;'']
          end
          for k=1:nout
            txtp($)=txtp($)+' double *, int * ,'
          end
          txtp($)=part(txtp($),1:length(txtp($))-1); //remove last ,
        end
      end

      if ztyp(bk) then
        txtp($)=txtp($)+', \'
        txtp=[txtp;' double *,int *);'];
      else
        txtp($)=txtp($)+');';
      end

      txtp(2:$) = blank + txtp(2:$);
      txt = [txt;txtp];
      //*******************************************//

    //**
    case 2 then

      //*********** prototype definition ***********//

      txtp=['void '+funs(bk)+...
            '(int *, int *, double *, double *, double *, int *, double *, \';
            ' int *, double *, int *, double *, int *, int *, int *, \'
            ' double **, int *, int *, double **,int *, int *'];
      if ~ztyp(bk) then
        txtp($)=txtp($)+');';
      else
        txtp($)=txtp($)+', \';
        txtp=[txtp;
              ' double *,int *);']
      end
      blank = get_blank('void '+funs(bk));
      txtp(2:$) = blank + txtp(2:$);
      txt = [txt;txtp];
      //********************************************//

    //**
    case 4 then
      txt=[txt;
           'void '+funs(bk)+'(scicos_block *, int );'];

  end
endfunction

//utilitary fonction used to format long C instruction
//t : a string containing a C instruction
//l : max line length allowed

//Author : Rachid Djenidi
function t1=cformatline(t ,l)

  sep=[',','+']
  l1=l-2
  t1=[]
  kw=strindex(t,' ')
  nw=0
  if kw<>[] then
    if kw(1)==1 then // there is leading blanks
      k1=find(kw(2:$)-kw(1:$-1)<>1)
      if k1==[] then // there is a single blank
	nw=1
      else
	nw=kw(k1(1))
      end
    end
  end
  t=part(t,nw+1:length(t));
  bl=part(' ',ones(1,nw))
  l1=l-nw;first=%t
  while %t 
    if length(t)<=l then t1=[t1;bl+t],return,end
    k=strindex(t,sep);
    if k==[] then t1=[t1;bl+t],return,end
    k($+1)=length(t)+1 // positions of the commas
    i=find(k(1:$-1)<=l&k(2:$)>l) //nearest left comma (reltively to l)
    if i==[] then i=1,end
    t1=[t1;bl+part(t,1:k(i))]
    t=part(t,k(i)+1:length(t))
    if first then l1=l1-2;bl=bl+'  ';first=%f;end
  end
endfunction

//used in do_compile_superblock
function vec=codebinaire(v,szclkIN)
  vec=zeros(1,szclkIN)
  for i=1:szclkIN
    w=v/2;
    vec(i)=v-2*int(w);
    v=int(w);
  end
endfunction

function t=filetype(m)
  m=int32(m)
  filetypes=['Directory','Character device','Block device',...
             'Regular file','FIFO','Symbolic link','Socket']
  bits=[16384,8192,24576,32768,4096,40960,49152]
  m=int32(m)&int32(61440)
  t=filetypes(find(m==int32(bits)))
endfunction

//get_blank : return blanks with a length
//            of the given input string
//
//input : str : a string
//
//output : txt : blanks
//
//16/06/07 Author : A.Layec
function [txt] = get_blank(str)
 txt='';
 for i=1:length(str)
     txt=txt+' ';
 end
endfunction

// get_comment : return a C comment
//               for generated code
//
//input : typ : a string
//        param : a list
//
//output : a C comment
//
//16/06/07 Author : A.Layec
function [txt]=get_comment(typ,param)
  txt = [];
  select typ
    //** main flag
    case 'flag' then
        select param(1)
          case 0 then
             txt = '/* Continuous state computation */'
          case 1 then
             txt = '/* Output computation */'
          case 2 then
             txt = '/* Discrete state computation */'
          case 3 then
             txt = '/* Output Event computation */'
          case 4 then
             txt = '/* Initialization */'
          case 5 then
             txt = '/* Ending */'
          case 9 then
             txt = '/* Update zero crossing surfaces */'
        end
    //** blocks activated on event number
    case 'ev' then
       txt = '/* Blocks activated on the event number '+string(param(1))+' */'

    //** blk calling sequence
    case 'call_blk' then
        txt = ['/* Call of '''+param(1) + ...
               ''' (type '+string(param(2))+' - blk nb '+...
                    string(param(3))];
        if ztyp(param(3)) then
          txt=txt+' - with zcross) */';
        else
          txt=txt+') */';
        end
    //** proto calling sequence
    case 'proto_blk' then
        txt = ['/* prototype of '''+param(1) + ...
               ''' (type '+string(param(2))];
        if ztyp(param(3)) then
          txt=txt+' - with zcross) */';
        else
          txt=txt+') */';
        end
    //** ifthenelse calling sequence
    case 'ifthenelse_blk' then
        txt = ['/* Call of ''if-then-else'' blk (blk nb '+...
                    string(param(1))+') */']
    //** eventselect calling sequence
    case 'evtselect_blk' then
        txt = ['/* Call of ''event-select'' blk (blk nb '+...
                    string(param(1))+') */']
    //** set block structure
    case 'set_blk' then
        txt = ['/* set blk struc. of '''+param(1) + ...
               ''' (type '+string(param(2))+' - blk nb '+...
                    string(param(3))+') */'];
    //** Update xd vector ptr
    case 'update_xd' then
        txt = ['/* Update xd vector ptr */'];
    //** Update g vector ptr
    case 'update_g' then
        txt = ['/* Update g vector ptr */'];
    else
      break;
  end
endfunction

//mat2c_typ : matrix to C type
//sci2c_ttyp : get the C string of a scicos type
//
//input : outtb : a matrix
//
//output : txt : the string of the C scicos type
//               of the data of outtb
//
//16/06/07 Author : A.Layec
function [txt]=mat2c_typ(outtb)
 select type(outtb)
   //real matrix
   case 1 then
      if isreal(outtb) then
        txt = "double"
      else
        txt = "double"
      end
   //integer matrix
   case 8 then
      select typeof(outtb)
         case 'int32' then
           txt = "long"
         case 'int16' then
           txt = "short"
         case 'int8' then
           txt = "char"
         case 'uint32' then
           txt = "unsigned long"
         case 'uint16' then
           txt = "unsigned short"
         case 'uint8' then
           txt = "unsigned char"
      end
   else
     break;
 end
endfunction

//mat2scs_c_nb  matrix to scicos C number (sci2sci_n)
//
//input : outtb : a matrix
//
//output : c_nb : the scicos C number
//
//16/06/07 Author : A.Layec
function [c_nb]=mat2scs_c_nb(outtb)
 select type(outtb)
   //real matrix
   case 1 then
      if isreal(outtb) then
        c_nb = 10
      else
        c_nb = 11
      end
   //integer matrix
   case 8 then
      select typeof(outtb)
         case 'int32' then
           c_nb = 84
         case 'int16' then
           c_nb = 82
         case 'int8' then
           c_nb = 81
         case 'uint32' then
           c_nb = 814
         case 'uint16' then
           c_nb = 812
         case 'uint8' then
           c_nb = 811
      end
   else
     break;
 end
endfunction

//mat2scs_c_ptr matrix to scicos C ptr (sci2c_typ)
//
//input : outtb : a matrix
//
//output : txt : the string of the C scicos type
//               of the data of outtb
//
//16/06/07 Author : A.Layec
function [txt]=mat2scs_c_ptr(outtb)
 select type(outtb)
   //real matrix
   case 1 then
      if isreal(outtb) then
        txt = "SCSREAL_COP"
      else
        txt = "SCSCOMPLEX_COP"
      end
   //integer matrix
   case 8 then
      select typeof(outtb)
         case 'int32' then
           txt = "SCSINT32_COP"
         case 'int16' then
           txt = "SCSINT16_COP"
         case 'int8' then
           txt = "SCSINT8_COP"
         case 'uint32' then
           txt = "SCSUINT32_COP"
         case 'uint16' then
           txt = "SCSUINT16_COP"
         case 'uint8' then
           txt = "SCSUINT8_COP"
      end
   else
     break;
 end
endfunction

