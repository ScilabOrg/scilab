//Generate the Model Configuration File required for 20-sim 4C
//
//Author : Maarten Meijer
//
//

function [nrParameters, nrInitialValues, nrVariables, nrStates, nrRates]=generateMCF();

	//These variables are read by generateModel and generateIvcIpc
	nrModelVariables=0;
	nrParameters=0;
	nrInitialValues=0;
	nrVariables=0;
	nrStates=0;
	nrRates=0;

  // Header and general section
  Code=['<?xml version='"1.0'" encoding='"UTF-8'"?>'
		'<modelConfiguration>'
 		'  <general>'
		'    <name>'+rdnom+'</name>'
		'    <description></description>'
		'    	<creationDate>'+date_time()+'</creationDate>'
		'    <modelDir>'+rpat+'\src</modelDir>'
		'    <version>'
		'        <major>1</major>'
		'        <minor>0</minor>'
		'        <revision>0</revision>'
		'        <patch>0</patch>'
		'    </version>'
		'    <additional></additional>'
 		'  </general>'];
 
  //run section
  Code=[Code
		'  <run>'
		'    <startTime>0.0000000000000000E+000</startTime>'
		'    <finishTime>2.0000000000000000E+000</finishTime>'
		'    <stepSize>1.0000000000000000E-001</stepSize>'
		'  </run>'];
  
  //begin model variables
  Code=[Code
    ''
    ' <modelVariables>'];
	
  //add the solver type to be used as initial value
  Code=[Code
      getModelVariableSection(nrModelVariables, 'solver type', 'initial value', 1, XX_INIT_VALUE_ARRAY_NAME, nrInitialValues, '', '')
      '']
      nrInitialValues=nrInitialValues+1
  nrModelVariables=nrModelVariables+1

  //loop through all blocks within the model and add the variables to be monitored or logged	
  for nrBlocks=1:nblk

	name = string(nrBlocks)+' '+funs(nrBlocks)
  
    //variables
	nout=outptr(nrBlocks+1)-outptr(nrBlocks); //* number of output ports */
	if nout>0 then
		//don't display the output of an actuator here
		if (part(funs(nrBlocks),1:7) ~= 'capteur') then
		Code=[Code
			getModelVariableSection(nrModelVariables, name+'\output', 'variable', '', XX_VARIABLE_ARRAY_NAME, nrVariables, '', '')
			'']
		end
        nrVariables=nrVariables+nout
		nrModelVariables=nrModelVariables+1
	end

    //parameters
    //Real parameters
    if (rpptr(nrBlocks+1)-rpptr(nrBlocks)>0) then
        Code=[Code
            getModelVariableSection(nrModelVariables, name+'\parameter', 'parameter', cpr.sim.rpar(rpptr(nrBlocks):rpptr(nrBlocks+1)-1), XX_PARAMETER_ARRAY_NAME, nrParameters, '', '')
            '']
    	nrParameters=nrParameters +sum(length( cpr.sim.rpar(rpptr(nrBlocks):rpptr(nrBlocks+1)-1) ))
        nrModelVariables=nrModelVariables+1
    end


    //initial values
	if zptr(nrBlocks)<=size(z,'*'),
		Code=[Code 
			getModelVariableSection(nrModelVariables, name+'\work', 'initial value', z(zptr(nrBlocks)), XX_INIT_VALUE_ARRAY_NAME, nrInitialValues, '', '')
        '']
        nrInitialValues=nrInitialValues+1
		nrModelVariables=nrModelVariables+1
	end
    
    if (xptr(nrBlocks+1)-xptr(nrBlocks))~=0
        //states
        Code=[Code
            getModelVariableSection(nrModelVariables, name+'\state', 'state', '', XX_STATE_ARRAY_NAME, (xptr(nrBlocks)-1), '', '')
            '']
    	nrStates=nrStates+1
        nrModelVariables=nrModelVariables+1
    
        //rates
        Code=[Code
            getModelVariableSection(nrModelVariables, name+'\rate', 'rate', '', XX_RATE_ARRAY_NAME, (xptr(nrBlocks)-1), '', '')
            '']
    	nrRates=nrRates+1
        nrModelVariables=nrModelVariables+1
    end
	
	//actuators / outputs
	//outputs are defined as a block within Scilab. Therefore, the input of the output-block is the variable of interest
	if (part(funs(nrBlocks),1:10) == 'actionneur') then	
		//loop through all inputs of the actionneur (actuator)
		for k=1:(inpptr(nrBlocks+1)-inpptr(nrBlocks))
		   lprt=inplnk(inpptr(nrBlocks)-1+k);
		   input_variable=string(-1);
		   
		   //Find the proper output variable by means of the input pointer
		  offset=0;
		  for i=1:lstsize(outtb)
			if(outlnk(outptr(i)-1+size(outtb(i),'*'))  == lprt)
				input_variable = string(offset);
			end
			offset = offset + size(outtb(i),'*')
		  end

		   Code=[Code
				getModelVariableSection(nrModelVariables, 'output '+string(k), 'output', '', XX_VARIABLE_ARRAY_NAME, input_variable, '', '')
				'']
			nrModelVariables=nrModelVariables+1
		end
	end
	 
	//sensors / inputs
	//inputs are defined as a block within Scilab. Therefore, the output of the input-block is the variable of interest
	if (part(funs(nrBlocks),1:7) == 'capteur') then	
		for k=1:(outptr(nrBlocks+1)-outptr(nrBlocks))
		   lprt=outlnk(outptr(nrBlocks)-1+k);
		   Code=[Code
				getModelVariableSection(nrModelVariables, 'input '+string(k), 'input', '', XX_VARIABLE_ARRAY_NAME, string(lprt-1), '', '')
				'']
			nrModelVariables=nrModelVariables+1
		end
	end

  end
  
  //add the object parameters
  if lstsize(cpr.sim.opar)<>0 then
    for i=1:(length(cpr.sim.opptr)-1)
      if cpr.sim.opptr(i+1)-cpr.sim.opptr(i)>0  then

        if size(corinv(i),'*')==1 then
          OO=scs_m.objs(corinv(i));
        else
          path=list('objs');
          for l=cpr.corinv(i)(1:$-1)
            path($+1)=l;
            path($+1)='model';
            path($+1)='rpar';
            path($+1)='objs';
          end
          path($+1)=cpr.corinv(i)($);
          OO=scs_m(path);
        end

        for j=1:cpr.sim.opptr(i+1)-cpr.sim.opptr(i)
		  //TODO: set value of parameter according to cpr.sim.opar(cpr.sim.opptr(i):j-1)
          Code =[Code
                getModelVariableSection(nrModelVariables, string(OO.gui)+'\parameter', 'parameter',... 
                0, XX_PARAMETER_ARRAY_NAME, nrParameters, '', '')]

		
          nrParameters = nrParameters + 1
          nrModelVariables = nrModelVariables + 1
        end
      end
    end
  end
  
  //Add event duration times add the end of the parameters
  for nrBlocks=1:nblk,
	name = string(nrBlocks)+' '+funs(nrBlocks)
  
  	if (cpr.sim.zptr(nrBlocks+1)-cpr.sim.zptr(nrBlocks))>0
    Code=[Code
        getModelVariableSection(nrModelVariables, name+'\event duration', 'parameter', zptr(nrBlocks)-1, XX_PARAMETER_ARRAY_NAME, nrParameters, 'seconds', 'time')
        '']
        nrParameters=nrParameters+1
		nrModelVariables=nrModelVariables+1
	end
  end
		
  //end model variables
  Code=[Code
		' </modelVariables>'
		''];

		
//modelVariable storage	
  storageSize = (nrParameters + nrVariables + nrStates + nrRates )*8;
  Code=[Code
		'  <storage>'
		'    <name>ivc_'+rdnom+'</name>'
		'    <size>'+string( storageSize )+'</size>'
		'    <tables>'
                  getTableSection(XX_PARAMETER_ARRAY_NAME, nrParameters)
                  getTableSection(XX_VARIABLE_ARRAY_NAME, nrVariables)
                  getTableSection(XX_INIT_VALUE_ARRAY_NAME, nrInitialValues)
                  getTableSection(XX_STATE_ARRAY_NAME, nrStates)
                  getTableSection(XX_RATE_ARRAY_NAME, nrRates)
		'    </tables>'
		'  </storage>'
		'  <icons />'
		'</modelConfiguration>'];
		
  //Create Model Configuration File
  ierr=execstr('mputl(Code,rpat+''/src/ModelConfiguration.xml'')','errcatch')
  if ierr<>0 then
    message(lasterror())
  end
endfunction

//***********************************************************************
//
//The section underneath contains helper functions
//
//***********************************************************************/

//Generate the model variable section by means of the input data
function txt=getModelVariableSection(id, name, vartype, value, storage_table, storage_index, unit, quantity)
        txt=['    <modelVariable id='"'+string(id)+''">'
		'        <name>'+name+'</name>'
		'        <kind>'+vartype+'</kind>']
		
                  //is scalar? and is not '' then
              if value~='' then
		if sum(length(value))==1 then
		  txt=[txt
		  '        <value>'+string(value)+'</value>']
                  else
		    strvalue=''
		    //create the vector or matrix value
		    for i=1:size(value, 2)
		      for j=1:size(value, 1)
		        if j~=size(value, 1)
		          strvalue=strvalue+string(value(j,i))+', '
		        else
		          strvalue=strvalue+string(value(j,i))
		        end
		      end
		      if i~=size(value, 2)
		        strvalue=strvalue+'; '
		      end
		    end

                      txt=[txt
		  '        <value>'+strvalue+'</value>'
		  '        <size>'
		  '          <columns>'+string(size(value, 1))+'</columns>'
		  '          <rows>'+string(size(value, 2))+'</rows>'
		  '        </size>']
		end
              end
		
	if(unit<>'' & quantity <>'')
	txt=[txt
		'        <physical>'
		'          <unit>'
		'            <name>'+unit+'</name>'
		'          </unit>'
		'          <quantity>'+quantity+'</quantity>'
		'        </physical>'];
	end
			
	txt=[txt
		'        <storage>'
		'          <name>'+storage_table+'</name>'
		'          <index>'+string(storage_index)+'</index>'
		'        </storage>'
		'    </modelVariable>'];
endfunction  

function txt=getTableSection(tbl_name, tbl_count)
	txt=['        <table>'
		'            <name>'+tbl_name+'</name>'
		'            <size>'+string( tbl_count * 8 )+'</size>'
		'            <kind>real</kind>'
		'            <kindSize>8</kindSize>'
		'            <count>'+string( tbl_count )+'</count>'
		'        </table>']
endfunction
