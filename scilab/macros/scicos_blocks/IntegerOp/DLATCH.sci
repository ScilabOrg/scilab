function [x,y,typ]=DLATCH(job,arg1,arg2)
// Copyright INRIA
  x=[];y=[],typ=[]
  select job
   case 'plot' then
    standard_draw(arg1)
   case 'getinputs' then
    [x,y,typ]=standard_inputs(arg1)
   case 'getoutputs' then
    [x,y,typ]=standard_outputs(arg1)
   case 'getorigin' then
    [x,y]=standard_origin(arg1)
   case 'set' then
    x=arg1
   case 'define' then
    logical_op=LOGICAL_OP('define')
    logical_op.graphics.orig=[319.32798,270.04579]
    logical_op.graphics.sz=[50,40]
    logical_op.graphics.flip=%t
    logical_op.graphics.exprs=['1';'5']
    logical_op.graphics.pin=17
    logical_op.graphics.pout=10
    logical_op.model.ipar=5
    
    output_port1=OUT_f('define')
    output_port1.graphics.orig=[330.07269,240.05129]
    output_port1.graphics.sz=[20,20]
    output_port1.graphics.flip=%t
    output_port1.graphics.exprs='1'
    output_port1.graphics.pin=16
    output_port1.model.ipar=1

    output_port2=OUT_f('define')
    output_port2.graphics.orig=[400.12806,279.49209]
    output_port2.graphics.sz=[20,20]
    output_port2.graphics.flip=%t
    output_port2.graphics.exprs='2'
    output_port2.graphics.pin=10
    output_port2.model.ipar=2

    input_port1=IN_f('define')
    input_port1.graphics.orig=[184.04191,240.00295]
    input_port1.graphics.sz=[20,20]
    input_port1.graphics.flip=%t
    input_port1.graphics.exprs='1'
    input_port1.graphics.pout=13
    input_port1.model.ipar=1

    input_port2=IN_f('define')
    input_port2.graphics.orig=[184.06519,309.97352]
    input_port2.graphics.sz=[20,20]
    input_port2.graphics.flip=%t
    input_port2.graphics.exprs='2'
    input_port2.graphics.pout=12
    input_port2.model.ipar=2

    ifthel=IFTHEL_f('define')
    ifthel.graphics.orig=[230.26359,289.96451]
    ifthel.graphics.sz=[60,60]
    ifthel.graphics.flip=%t
    ifthel.graphics.exprs=['1';'1']
    ifthel.graphics.pin=19
    ifthel.graphics.pein=11
    ifthel.graphics.peout=[7;0]

    samphold=SAMPHOLD('define')
    samphold.graphics.orig=[230.63124,230.43473]
    samphold.graphics.sz=[40,40]
    samphold.graphics.flip=%t
    samphold.graphics.pin=13
    samphold.graphics.pout=14
    samphold.graphics.pein=7

    split1=SPLIT_f('define')
    split1.graphics.orig=[210.77258;319.97009]
    split1.graphics.sz=[0.3333333,0.3333333]
    split1.graphics.flip=%t
    split1.graphics.pin=12
    split1.graphics.pout=[19;20]
    
    extr_act= Extract_Activation('define')
    extr_act.graphics.orig=[229.8431,371.07558]
    extr_act.graphics.sz=[60,40]
    extr_act.graphics.flip=%t
    extr_act.graphics.pin=20
    extr_act.graphics.peout=11

    split2=SPLIT_f('define')
    split2.graphics.orig=[309.78689;250.2042]
    split2.graphics.sz=[0.3333333,0.3333333]
    split2.graphics.flip=%t
    split2.graphics.pin=14
    split2.graphics.pout=[16;17]

    gr_i=list(['[x,y,typ]=standard_inputs(o) ';
	  'dd=sz(1)/8,de=5.5*sz(1)/8';
	  'xstring(orig(1)+dd,y(1)-4,''D'')';
	  'xstring(orig(1)+dd,y(2)-4,''C'')';
	  'xstring(orig(1)+de,y(1)-4,''Q'')';
	  'xstring(orig(1)+de,y(2)-4,''!Q'')';],8)
    diagram=scicos_diagram();
    diagram.objs(1)=input_port1
    diagram.objs(2)=output_port1
    diagram.objs(3)=input_port2
    diagram.objs(4)=output_port2
    diagram.objs(5)=ifthel
    diagram.objs(6)=samphold
    diagram.objs(7)=scicos_link(xx=[250.26359;250.63124],yy=[284.25022;276.14902],..
				ct=[5,-1],from=[5,1],to=[6,1])  
    diagram.objs(8)=extr_act
    diagram.objs(9)=logical_op
    diagram.objs(10)=scicos_link(xx=[377.89941;400.12806],..
				yy=[290.04579;289.49209],..
				ct=[1,1],from=[9,1],to=[4,1]) 
    diagram.objs(11)=scicos_link(xx=[259.8431;260.26359],..
				yy=[365.3613;355.6788],..
				ct=[5,-1],from=[8,1],to=[5,1]) 
    diagram.objs(12)=scicos_link(xx=[204.06519;210.77258],..
				yy=[319.97352;319.97009],..
				ct=[1,1],from=[3,1],to=[18,1]) 
    diagram.objs(13)=scicos_link(xx=[204.04191;222.05981],..
				yy=[250.00295;250.43473],..
				ct=[1,1],from=[1,1],to=[6,1]) 
    diagram.objs(14)=scicos_link(xx=[279.20267;300.25216;309.78689],..
				yy=[250.43473;250.27607;250.2042],..
				ct=[1,1],from=[6,1],to=[15,1])
    diagram.objs(15)=split2
    diagram.objs(16)=scicos_link(xx=[309.78689;330.07269],..
				yy=[250.2042;250.05129],..
				ct=[1,1],from=[15,1],to=[2,1])
    diagram.objs(17)=scicos_link(xx=[309.78689;309.78689;310.75655],..
				yy=[250.2042;290.04579;290.04579],..
				ct=[1,1],from=[15,2],to=[9,1])  
    diagram.objs(18)=split1
    diagram.objs(19)=scicos_link(xx=[210.77258;221.69216],..
				yy=[319.97009;319.96451],..
				ct=[1,1],from=[18,1],to=[5,1]) 
    diagram.objs(20)=scicos_link(xx=[210.77258;210.77258;221.27167],..
				yy=[319.97009;391.07558;391.07558],..
				ct=[1,1],from=[18,2],to=[8,1]) 

    x=scicos_block()
    x.gui='DLATCH'
    x.graphics.sz=[2,3]
    x.graphics.gr_i=gr_i
    x.graphics.pin=[0;0]
    x.graphics.pout=[0;0]
    x.model.sim='csuper'
    x.model.in=[-1;-1]
    x.model.in2=[]
    x.model.out=[-1;-1]
    x.model.out2=[]
    x.model.blocktype='h'
    x.model.firing=%f
    x.model.dep_ut=[%f %f]
    x.model.rpar=diagram
    x.graphics.id="DLATCH"
  end
endfunction
