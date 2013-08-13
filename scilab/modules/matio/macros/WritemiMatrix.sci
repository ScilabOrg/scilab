// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// Copyright (C) ???? - INRIA - Serge STEER
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function WritemiMatrix(fd,value,ArrayName)
    // Save variables in a Matlab binary file
    // This function has been developed following the 'MAT-File Format' description:
    // www.mathworks.com/access/helpdesk/help/pdf_doc/matlab/matfile_format.pdf

    TagSize=8; // 8 bytes
    ArrayFlagSize=8; // 8 bytes

    // Position is saved to come back here after writing
    SavePosBefore=mtell(fd);

    // Save space for TAG
    WriteEmptyTag(fd);

    // Position is saved to compute number of written bytes
    NumberOfBytes=mtell(fd);

    // Save space for ARRAY FLAGS
    WriteEmptyArrayFlags(fd);

    // Compute array dimensions
    if type(value)==10 then
        WriteDimensionArray(fd,size(mstr2sci(value)));
    else
        WriteDimensionArray(fd,size(value));
    end

    // Write variable name
    WriteArrayName(fd,ArrayName);

    Flags=[0 0 0];

    if type(value)==1 then // DOUBLE value
        value=matrix(value,1,-1);
        Flags(1)=bool2s(~isreal(value));
        Class=DoubleClass;
        NnzMax=0;
        WriteSimpleElement(fd,real(value),miDOUBLE);
        if Flags(1) then
            WriteSimpleElement(fd,imag(value),miDOUBLE);
        end
    elseif type(value)==10 then // CHARACTER STRING value
        if size(value,"*")==1 then
            value=matrix(ascii(mstr2sci(value)),1,-1);
            Flags(1)=0;
            Class=CharClass;
            NnzMax=0;
            WriteSimpleElement(fd,value,miUINT16);
        else
            warning(gettext("Scilab string matrix saved as Matlab Cell."));
            sz=size(value);
            value=matrix(value,1,-1);
            entries=list()
            for k=1:size(value,2)
                entries(k)=value(k);
            end
            value=mlist(["ce","dims","entries"],int32(sz),entries)
            mseek(SavePosBefore,fd);
            WritemiMatrix(fd,value,ArrayName);
            return
        end
    elseif type(value)==8 then // INTEGER value
        value=matrix(value,1,-1);
        Flags(1)=0;
        NnzMax=0;
        select typeof(value)
        case "int8"
            Class=Int8Class;
            WriteSimpleElement(fd,value,miINT8);
        case "uint8"
            Class=Uint8Class;
            WriteSimpleElement(fd,value,miUINT8);
        case "int16"
            Class=Int16Class;
            WriteSimpleElement(fd,value,miINT16);
        case "uint16"
            Class=Uint16Class;
            WriteSimpleElement(fd,value,miUINT16);
        case "int32"
            Class=Int32Class;
            WriteSimpleElement(fd,value,miINT32);
        case "uint32"
            Class=Uint32Class;
            WriteSimpleElement(fd,value,miUINT32);
        else
            error(msprintf(gettext("Unknown integer type: %s."),typeof(value)));
        end
    elseif type(value)==17 then // MLIST used ofr CELLS and STRUCTS
        Flags(1)=0;
        NnzMax=0;
        select typeof(value)
        case "ce" // CELL
            Class=CellClass;
            for k=1:lstsize(value.entries)
                WritemiMatrix(fd,value(k).entries,"");
            end
        case "st" // STRUCT
            Class=StructClass;
            Fnams=getfield(1,value);
            Fnams(1:2)=[];
            FieldNameLength=32;
            WriteSimpleElement(fd,FieldNameLength,miINT32);

            NumberOfFields=size(Fnams,2);
            FieldNames=[]
            for k=1:NumberOfFields
                FieldNames=[FieldNames ascii(Fnams(k)) zeros(1,FieldNameLength-length(Fnams(k)))];
            end

            WriteSimpleElement(fd,FieldNames,miINT8);
            if prod(size(value))==1 then
                for k=1:NumberOfFields
                    WritemiMatrix(fd,value(Fnams(k)),"");
                end
            else
                for i=1:prod(size(value))
                    for k=1:NumberOfFields
                        WritemiMatrix(fd,value(i)(Fnams(k)),"");
                    end
                end
            end
        else
            error(msprintf(gettext("%s mlist type not yet implemented."),typeof(value)));
        end
    elseif or(type(value)==[5,7]) then // SPARSE matrices
        if type(value)==5 then // Scilab sparse is converted to Matlab sparse
            value=mtlb_sparse(value);
        end
        Class=SparseClass;
        [ij,v,mn]=spget(value);
        RowIndex=ij(:,1)-1;
        col=ij(:,2);
        NnzMax=length(RowIndex);

        WriteSimpleElement(fd,RowIndex,miINT32);

        ColumnIndex=col(1);
        for k=1:size(col,"*")-1
            if col(k)<>col(k+1) then
                ColumnIndex=[ColumnIndex;col(k+1)]
            end
        end

        ptr=0;
        for k=1:size(ColumnIndex,"*")
            ptr=[ptr;size(find(col==ColumnIndex(k)),"*")]
        end
        ColumnIndex=cumsum(ptr);

        WriteSimpleElement(fd,ColumnIndex,miINT32);

        Flags(1)=bool2s(~isreal(v));
        WriteSimpleElement(fd,real(v),miDOUBLE);
        if Flags(1) then
            WriteSimpleElement(fd,imag(v),miDOUBLE);
        end
    else
        error(msprintf(gettext("%s not yet implemented."),typeof(value)));
    end

    SavePosAfter=mtell(fd);

    NumberOfBytes=SavePosAfter-NumberOfBytes

    // Update tag
    WriteTag(fd,miMatrix,NumberOfBytes);

    mseek(SavePosBefore+TagSize+TagSize+ArrayFlagSize,fd);

    // Update array flags
    WriteArrayFlags(fd,Flags,Class,NnzMax);

    mseek(SavePosAfter,fd);
endfunction

function fd=open_matfile_wb(fil)
    // Copyright INRIA
    // Opens a file in 'w+b' mode
    // VC
    fil=stripblanks(fil)
    fd=mopen(fil,"w+b",0)
endfunction

function swap=write_matfile_header(fd)
    // Copyright INRIA
    // Write the mat file header information
    // VC

    head=gettext("MATLAB 5.0 MAT-file, Generated by Scilab");
    head=head+part(" ",1:(124-length(head)));
    mput(ascii(head),"uc",fd);

    version=[1 0];
    mput(version,"uc",fd);

    endian_indicator=ascii(["M" "I"]);
    mput(endian_indicator,"uc",fd);

    // Character are read just after to get endian
    // Because mput swap automatically bytes
    // if endian not given when writing
    mseek(mtell(fd)-2,fd);
    IM_MI=mget(2,"uc",fd);
    if and(IM_MI==[73,77]) then // little endian file
        swap="l"
    elseif and(IM_MI==[77,73]) then // big endian file
        swap="b"
    else
        error(gettext("Error while writing MI."));
    end
    // Following call to mseek is needed under Windows
    // to set file pointer after reading
    mseek(0,fd,"cur");
endfunction

function WriteEmptyTag(fd)
    // Copyright INRIA
    // Reserve space for a tag
    // VC

    for k=1:TagSize
        mput(0,"uc",fd);
    end
endfunction

function WriteEmptyArrayFlags(fd)
    // Copyright INRIA
    // Reserve space for an array flag
    // VC

    for k=1:ArrayFlagSize+TagSize
        mput(0,"uc",fd);
    end
endfunction

function WriteArrayFlags(fd,Flags,Class,NnzMax)
    // Copyright INRIA
    // Write an array flag
    // VC

    WriteTag(fd,miUINT32,ArrayFlagSize);

    mseek(mtell(fd)-ArrayFlagSize,fd);

    Flags=[0 Flags(3:-1:1)];

    B=[0 0 0 0];
    B(3)=bits2byte(Flags);
    B(4)=Class;
    mput(B,"uc",fd);

    mput(NnzMax,md_i,fd);
endfunction

function WriteDimensionArray(fd,dims)
    // Copyright INRIA
    // Write dimensions of an array
    // VC

    WriteSimpleElement(fd,dims,miINT32);
endfunction

function WriteArrayName(fd,ArrayName)
    // Copyright INRIA
    // Write name of an array
    // VC

    WriteSimpleElement(fd,ascii(ArrayName),miINT8);
endfunction

function WriteTag(fd,DataType,NumberOfBytes,Compressed)
    // Copyright INRIA
    // Write a tag
    // VC

    SavePos=mtell(fd);

    if argn(2)==3 then
        Compressed=%F;
    end
    Compressed=NumberOfBytes<=4;

    if Compressed then
        mseek(SavePos-NumberOfBytes-TagSize/2,fd);
        mput(NumberOfBytes,md_s,fd);
        mput(DataType,md_s,fd);
    else
        mseek(SavePos-NumberOfBytes-TagSize,fd);
        mput(DataType,md_i,fd);
        mput(NumberOfBytes,md_i,fd);
    end

    mseek(SavePos,fd);
endfunction

function WriteSimpleElement(fd,value,DataType)
    // Copyright INRIA
    // Write an element in file
    // VC

    // If data is of double type
    // and made of integer values
    // then it is writen in an INT* format to save space
    if DataType==miDOUBLE & and(double(int(value))==value) then
        if min(value)>=0 & max(value)<=255 then // min and max value for int8
            DataType=miUINT8;
        elseif min(value)>=-128 & max(value)<=127 then // min and max value for int8
            DataType=miINT8;
            //miINT8 replaced by miINT16 due to an error somewhere (matlab or
            //scilab?) the generated file gives incorrect result in Matlab!
            //example:
            //  scilab var=-40;savematfile('foosci.mat','var','-mat','-v6');
            //  matlab load foosci.mat;var
            DataType=miINT16;

        elseif min(value)>=0 & max(value)<=65535 then // min and max value for int16
            DataType=miUINT16;
        elseif min(value)>=-32768 & max(value)<=32767 then // min and max value for int16
            DataType=miINT16;
        elseif min(value)>=0 & max(value)<=4294967295 then // min and max value for int32
            DataType=miINT32;
        elseif min(value)>=-2147483648 & max(value)<=2147483647 then // min and max value for int32
            DataType=miINT32;
        end
    end

    NumberOfValues=length(value);

    WriteEmptyTag(fd);

    select DataType
    case miDOUBLE
        NumberOfBytes=NumberOfValues*8;
        fmt=md_d;
    case miINT8
        NumberOfBytes=NumberOfValues;
        fmt="c";
    case miUINT8
        NumberOfBytes=NumberOfValues;
        fmt="uc";
    case miINT16
        NumberOfBytes=NumberOfValues*2;
        fmt=md_s;
    case miUINT16
        NumberOfBytes=NumberOfValues*2;
        fmt="u"+md_s;
    case miINT32
        NumberOfBytes=NumberOfValues*4;
        fmt=md_i;
    case miUINT32
        NumberOfBytes=NumberOfValues*4;
        fmt="u"+md_i;
    else
        error(msprintf(gettext("Error while writing MI."),string(DataType)));
    end

    Compressed=NumberOfBytes<=4;
    if Compressed then
        mseek(mtell(fd)-TagSize/2,fd);
    end

    mput(value,fmt,fd);

    WriteTag(fd,DataType,NumberOfBytes);

    WritePaddingBytes(fd);

endfunction

function WritePaddingBytes(fd)
    // Copyright INRIA
    // Write padding bytes to have a number of bytes multiple of 8
    // VC

    np=modulo(8-modulo(mtell(fd),8),8);
    for k=1:np
        mput(0,"uc",fd);
    end
endfunction

function i=bits2byte(b)
    // Copyright INRIA
    // Converts 4-bits value to a byte value
    // VC

    i=b* 2^(0:3)';
endfunction
