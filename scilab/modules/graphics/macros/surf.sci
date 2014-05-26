// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Fabrice Leray
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function surf(varargin)
    [lhs,rhs]=argn(0);

    if ~rhs
        Z= [  0.0001    0.0013    0.0053   -0.0299   -0.1809   -0.2465   -0.1100   -0.0168   -0.0008   -0.0000; ..
        0.0005    0.0089    0.0259   -0.3673   -1.8670   -2.4736   -1.0866   -0.1602   -0.0067    0.0000; ..
        0.0004    0.0214    0.1739   -0.3147   -4.0919   -6.4101   -2.7589   -0.2779    0.0131    0.0020; ..
        -0.0088   -0.0871    0.0364    1.8559    1.4995   -2.2171   -0.2729    0.8368    0.2016    0.0130; ..
        -0.0308   -0.4313   -1.7334   -0.1148    3.0731    0.4444    2.6145    2.4410    0.4877    0.0301; ..
        -0.0336   -0.4990   -2.3552   -2.1722    0.8856   -0.0531    2.6416    2.4064    0.4771    0.0294; ..
        -0.0137   -0.1967   -0.8083    0.2289    3.3983    3.1955    2.4338    1.2129    0.2108    0.0125; ..
        -0.0014   -0.0017    0.3189    2.7414    7.1622    7.1361    3.1242    0.6633    0.0674    0.0030; ..
        0.0002    0.0104    0.1733    1.0852    2.6741    2.6725    1.1119    0.1973    0.0152    0.0005; ..
        0.0000    0.0012    0.0183    0.1099    0.2684    0.2683    0.1107    0.0190    0.0014    0.0000];
        f=gcf();
        f.color_map = jetcolormap(64);
        surf(Z,"edgeco","b","marker","d","markersiz",9,"markeredg","red","markerfac","k");
        return;
    end

    X=[];
    Y=[];
    Z=[];
    C=[];

    CurColor = 0; // current color used if no color specified via LineSpec
    // nor PropertyName

    ListArg = varargin;

    //detect and set the current axes now:
    if type(ListArg(1)) == 9
        hdle = ListArg(1);
        if (hdle.type == "Axes")
            sca(ListArg(1));
            ListArg(1) = null(); // remove this parameter from the list
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: An ''Axes'' handle expected.\n"), "surf", 1));
            return;
        end
    end


    nv = size(ListArg)


    T=[];
    d=[];

    typeOfPlot = "surf";
    //given_data = 2;

    for k=1:nv
        T(k,1) = type(ListArg(k))
    end

    given_data = 0;
    P1 = 0;

    for i=1:nv
        if T(i) == 1
            given_data = given_data +1;
        else
            P1 = i;  // Position of the first PropertyName field
            break;
        end
    end


    // delay the drawing commands
    // smart drawlater
    current_figure=gcf();
    cur_draw_mode = current_figure.immediate_drawing;
    current_figure.immediate_drawing = "off";

    colormap_size = size(current_figure.color_map,1);

    if given_data == 1 //surf(Z) with Z giving us data + color info.
        // ---------------------------------------------------------- //

        if or(size(ListArg(1))==1)
            ResetFigureDDM(current_figure, cur_draw_mode);
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A matrix of size greater than %d-by-%d expected.\n"), "surf", 1,  2, 2));
            return;
        end

        X = 1:size(ListArg(1),2);
        Y = 1:size(ListArg(1),1);
        Z = ListArg(1)'; // here a transposition is needed
        C = Z;

        [XX,YY,ZZ] = genfac3d(X,Y,Z);
        CC = ZZ; // Add a color matrix based on Z values

    elseif given_data == 2 //surf(Z,COLOR)
        // ---------------------------------------------------------- //
        if or(size(ListArg(1))==1)
            ResetFigureDDM(current_figure, cur_draw_mode);
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A matrix of size greater than %d-by-%d expected.\n"), "surf", 1,  2, 2));
            return;
        end

        if ((size(ListArg(1)) <> size(ListArg(2))) & (size(ListArg(1))-1 <> size(ListArg(2))))
            ResetFigureDDM(current_figure, cur_draw_mode);
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A %d-by-%d or %d-by-%d matrix expected.\n"),..
            "surf", 2, size(ListArg(1),1), size(ListArg(1),2), size(ListArg(1),1) - 1, size(ListArg(1),2) -1 ));
            return;
        end

        X = 1:size(ListArg(1),2);
        Y = 1:size(ListArg(1),1);
        Z = ListArg(1)'; // here a transposition is needed
        C = ListArg(2)';

        [XX,YY,ZZ] = genfac3d(X,Y,Z);

        if (size(ListArg(2)) == size(ListArg(1))) // color number == zdata number
            [XX,YY,CC] = genfac3d(X,Y,C);     // CC must be a color matrix of size nf x n
        elseif ((size(ListArg(2))) == size(ListArg(1))-1) // color number -1 == zdata number => ONLY flat mode can be enabled
            Ctmp=[];
            Ctmp = [C [C(:,$)]] ;
            Ctmp = [Ctmp; Ctmp($,:)];
            [XX,YY,CC] = genfac3d(X,Y,Ctmp);     // CC must be a color matrix of size nf x n
        end

    elseif given_data == 3 //surf(X,Y,Z) with Z giving us data + color info.
        // ---------------------------------------------------------- //

        X = ListArg(1)
        Y = ListArg(2);
        Z = ListArg(3);

        // check if the call is OK
        err = execstr("[XX,YY,ZZ,CC] = CreateFacetsFromXYZ(X,Y,Z,current_figure, cur_draw_mode)","errcatch","n");

        if (err <> 0) then
            // reset data
            processSurfError(current_figure, cur_draw_mode);
        end

    elseif given_data == 4 //surf(X,Y,Z,COLOR)
        // ---------------------------------------------------------- //
        if ((size(ListArg(3)) <> size(ListArg(4))) & (size(ListArg(3))-1 <> size(ListArg(4))))
            ResetFigureDDM(current_figure, cur_draw_mode);
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A %d-by-%d or %d-by-%d matrix expected.\n"),..
            "surf", 4, size(ListArg(3),1), size(ListArg(3),2), size(ListArg(3),1) - 1, size(ListArg(3),2) -1 ));
            return;
        end

        X = ListArg(1)
        Y = ListArg(2);
        Z = ListArg(3);
        C = ListArg(4);

        // check if the call is OK
        err = execstr("[XX,YY,ZZ,CC] = CreateFacetsFromXYZColor(X,Y,Z,C,current_figure, cur_draw_mode)","errcatch","n");
        if (err <> 0) then
            // reset data
            processSurfError(current_figure, cur_draw_mode);
        end
    end


    // P1 is the position of the first PropertyName field.
    Property = P1;

    while ((Property <> 0) & (Property <= nv-1))
        PropertyName  = ListArg(Property);
        PropertyValue = ListArg(Property+1);

        // Xdata
        PName = getSurfPropertyName(PropertyName);
        if (PName == "xdata")

            if (type(PropertyValue)<>1)
                ResetFigureDDM(current_figure, cur_draw_mode);
                error(msprintf(gettext("%s: Wrong type for input argument ''%s'': A Real matrix expected.\n"), "surf", "xdata"));
                return;
            end

            X = PropertyValue;
            [XX,tmp2,tmp3] = CreateFacetsFromXYZ(PropertyValue,Y,Z,current_figure, cur_draw_mode);

            // Ydata
        elseif (PName == "ydata")

            if (type(PropertyValue)<>1)
                ResetFigureDDM(current_figure, cur_draw_mode);
                error(msprintf(gettext("%s: Wrong type for input argument ''%s'': A Real matrix expected.\n"), "surf", "ydata"));
                return;
            end

            Y = PropertyValue;
            [tmp1,YY,tmp3] = CreateFacetsFromXYZ(X,PropertyValue,Z,current_figure, cur_draw_mode);

            // Zdata
        elseif (PName == "zdata")

            if (type(PropertyValue)<>1) then
                ResetFigureDDM(current_figure, cur_draw_mode);
                error(msprintf(gettext("%s: Wrong type for input argument ''%s'': A Real matrix expected.\n"), "surf", "zdata"));
                return;
            end
            if (or(size(PropertyValue)==1)) then
                ResetFigureDDM(current_figure, cur_draw_mode);
                error(msprintf(gettext("%s: Wrong size for input argument ''%s'': A matrix of size greater than %d-by-%d expected.\n"), "surf", "zdata",  2, 2));
                return;
            end

            Z = PropertyValue;
            [tmp1,tmp2,ZZ] = CreateFacetsFromXYZ(X,Y,PropertyValue,current_figure, cur_draw_mode);

        end

        Property = Property+2;
    end

    if isempty(XX) | isempty(YY) | isempty(ZZ) then
        // Nothing will be drawn so return (see http://bugzilla.scilab.org/13180 )
        return
    end

    // surf is made now !
    // with default option to simulate the Matlab mode

    err = execstr("plot3d(XX,YY,list(ZZ,CC))","errcatch","n");

    if err <> 0
        processSurfError(current_figure, cur_draw_mode);
    end

    a=gca();
    a.cube_scaling = "on";
    a.rotation_angles = [51 -125];
    e=gce();
    e.hiddencolor=0; // to avoid painting the hidden facets
    e.color_flag=4; // Matlab special flat mode by default (different from mode 2)
    e.cdata_mapping = "scaled"


    // F.Leray
    // Today: 17.03.05
    // XYZ-DataMode under Matlab seems really bugged (see following samples and read Matlab doc.):
    //
    // surf(X,Y,Z,'XDataMode','auto')
    // surf(X,Y,Z,'XDataMode','auto')
    // surf(X+20,Y,Z,'XDataMode','auto')
    // surf(X+20,Y,Z,'XDataMode','man')
    // surf(X+20,Y,Z,'XDataMode','auto')
    // surf(X+20,Y,Z,'Xdata',X-100,'XDataMode','auto')
    // surf(X+20,Y,Z,'Xdata',X-100,'XDataMode','man')
    // surf(X+20,Y,Z,'XData',X-100,'XDataMode','auto')
    // surf(X+20,Y,Z,'XData',X-100,'XDataMode','man')
    // surf(Z,'XData',X-100,'XDataMode','man')
    // surf(Z,'XData',X-100,'XDataMode','auto')
    // surf(X+20,Y,Z,'XDataMode','man')
    // surf(X+20,Y,Z,'XDataMode','auto')
    //
    // That is why I do not support those properties.
    // Below and in comment is the code we could add to treat those properties.
    // by giving  XYZ-DataModeVal to setSurfProperty (to better treat XYZ-Data input).
    //
    //
    //XdataModeVal=1;
    //YdataModeVal=1;
    //ZdataModeVal=1;
    //
    //if Property <> 0
    //  XdataMode = getIndexInStringTable('xdatam',ListArg([Property nv]))
    //  if XdataMode <> []
    //    XdataModeVal = getIndexInStringTable(ListArg(XdataMode+1),['auto','manual'])
    //    if size(XdataModeVal,'*') <> 1
    //      disp("Error: Bad XdataMode selected");
    //      return;
    //    end
    //  end
    //
    //  YdataMode = getIndexInStringTable('ydatam',ListArg([Property nv]))
    //  if YdataMode <> []
    //    YdataModeVal = getIndexInStringTable(ListArg(YdataMode+1),['auto','manual'])
    //    if size(YdataModeVal,'*') <> 1
    //      disp("Error: Bad YdataMode selected");
    //      return;
    //    end
    //  end
    //
    //  ZdataMode = getIndexInStringTable('zdatam',ListArg([Property nv]))
    //  if ZdataMode <> []
    //    ZdataModeVal = getIndexInStringTable(ListArg(ZdataMode+1),['auto','manual'])
    //    if size(ZdataModeVal,'*') <> 1
    //      disp("Error: Bad ZdataMode selected");
    //      return;
    //   end
    //  end
    //end
    //


    ///////////////////////////////////
    //Global Property treatment      //
    //PropertyName and PropertyValue //
    ///////////////////////////////////


    // P1 is the position of the first PropertyName field.
    Property = P1;

    current_surface = gce(); // get the newly created fac3d
    current_surface.mark_size_unit="point";



    while ((Property <> 0) & (Property <= nv-1))
        setSurfProperty(ListArg(Property),ListArg(Property+1),current_surface,X,Y,Z,C,current_figure,cur_draw_mode)

        Property = Property+2;
    end


    //postponed drawings are done now !
    // smart drawnow
    ResetFigureDDM(current_figure, cur_draw_mode);

endfunction

//
//function [C] = build_interp_color(C,colormap_size)
//// C is considered as a data value in Matlab
//MIN = min(C);
//MAX = max(C);
//NCOLMIN = 1;
//NCOLMAX = colormap_size;
//
//if MIN <> MAX
//  C = (NCOLMIN-NCOLMAX)/(MIN-MAX) * C + (MIN*NCOLMAX - NCOLMIN*MAX)/(MIN-MAX);
//  C = round(C);
//else
//  C = ones(C) * (NCOLMIN+NCOLMAX)/2;
//end
//endfunction
//
//


function k=getIndexInStringTable(pattern,table)

    str =  convstr(pattern);
    k=find(part(table,1:length(str))==str);

endfunction

function [XX,YY,ZZ,CC] = CreateFacetsFromXYZ(X,Y,Z,current_figure, cur_draw_mode)

    if or(size(X)==1) & or(size(Y)==1) // X and Y are vector

        tmp = X;
        X = Y;
        Y = tmp;

        if size(X,"*") ~= size(Z,1) then
            ResetFigureDDM(current_figure, cur_draw_mode);
            error(msprintf(gettext("%s: Wrong size for input argument ''%s'': A vector of size %d expected.\n"), "surf", "Y", size(Z,1)));
            return;
        end

        if size(Y,"*") ~= size(Z,2) then
            ResetFigureDDM(current_figure, cur_draw_mode);
            error(msprintf(gettext("%s: Wrong size for input argument ''%s'': A vector of size %d expected.\n"), "surf", "X", size(Z,2)));
            return;
        end

        [XX,YY,ZZ] = genfac3d(Y,X,Z');

        // COLOR treatment
        CC = ZZ;

    elseif and(size(X)>1) & and(size(Y)>1) // X and Y are matrix

        if or(size(X) ~= size(Y)) then
            ResetFigureDDM(current_figure, cur_draw_mode);
            error(msprintf(gettext("%s: Wrong size for input arguments ''%s'' and ''%s'': Matrices of same size expected.\n"), "surf", "X", "Y"));
            return;
        end

        if or(size(X) ~= size(Z)) then
            ResetFigureDDM(current_figure, cur_draw_mode);
            error(msprintf(gettext("%s: Wrong size for input arguments ''%s'' and ''%s'': Matrices of same size expected.\n"), "surf", "X", "Z"));
            return;
        end

        [XX,YY,ZZ] = nf3d(X,Y,Z);

        // COLOR treatment
        CC = ZZ;

    elseif or(size(X)==1) & and(size(Y)>1) // X is a vector and Y is a matrix

        if size(X,"*") ~= size(Z,2) then
            ResetFigureDDM(current_figure, cur_draw_mode);
            error(msprintf(gettext("%s: Wrong size for input argument ''%s'': A vector of size %d expected.\n"), "surf", "X", size(Z,2)));
            return;
        end

        if or(size(Y) ~= size(Z)) then
            ResetFigureDDM(current_figure, cur_draw_mode);
            error(msprintf(gettext("%s: Wrong size for input arguments ''%s'' and ''%s'': Matrices of same size expected.\n"), "surf", "Y", "Z"));
            return;
        end

        // X vector
        // Y matrix
        // Z matrix

        X=X(:)'; // X is forced to be a row vector
        XMAT=[];

        for i=1:size(Z,2)
            XMAT=[XMAT;X];
        end

        [XX,YY,ZZ] = nf3d(XMAT,Y,Z);

        // COLOR treatment
        CC = ZZ;

    elseif or(size(Y)==1) & and(size(X)>1) // Y is a vector and X is a matrix

        if or(size(X) ~= size(Z)) then
            ResetFigureDDM(current_figure, cur_draw_mode);
            error(msprintf(gettext("%s: Wrong size for input arguments ''%s'' and ''%s'': Matrices of same size expected.\n"), "surf", "X", "Z"));
            return;
        end

        if size(Y,"*") ~= size(Z,2) then
            ResetFigureDDM(current_figure, cur_draw_mode);
            error(msprintf(gettext("%s: Wrong size for input argument ''%s'': A vector of size %d expected.\n"), "surf", "Y", size(Z,2)));
            return;
        end

        // Y vector
        // X matrix
        // Z matrix

        Y=Y(:); // Y is forced to be a column vector
        YMAT=[];

        for i=1:size(Z,1)
            YMAT=[YMAT,Y];
        end

        [XX,YY,ZZ] = nf3d(X,YMAT,Z);

        // COLOR treatment
        CC = ZZ;

    else
        ResetFigureDDM(current_figure, cur_draw_mode);
        error(msprintf(gettext("%s: Wrong size for input arguments ''%s'' and ''%s'': Same size expected.\n"), "surf", "X", "Y"));
        return;
    end

endfunction

function [XX,YY,ZZ,CC] = CreateFacetsFromXYZColor(X,Y,Z,C,current_figure, cur_draw_mode)

    if or(size(X)==1) & or(size(Y)==1) // X and Y are vector

        Z = Z'; // here a transposition is needed
        C = C'; // here a transposition is needed

        if size(X,"*") ~= size(Z,1) then
            ResetFigureDDM(current_figure, cur_draw_mode);
            error(msprintf(gettext("%s: Wrong size for input argument ''%s'': A vector of size %d expected.\n"), "surf", "X", size(Z,1)));
            return;
        end

        if size(Y,"*") ~= size(Z,2) then
            ResetFigureDDM(current_figure, cur_draw_mode);
            error(msprintf(gettext("%s: Wrong size for input argument ''%s'': A vector of size %d expected.\n"), "surf", "Y", size(Z,2)));
            return;
        end

        [XX,YY,ZZ] = genfac3d(X,Y,Z);

        // COLOR treatment
        if (size(C) == size(Z)) // color number == zdata number
            [XX,YY,CC] = genfac3d(X,Y,C);     // CC must be a color matrix of size nf x n
        elseif (size(C) == size(Z)-1) // color number -1 == zdata number => ONLY flat mode can be enabled
            Ctmp=[];
            Ctmp = [C [C(:,$)]] ;
            Ctmp = [Ctmp; Ctmp($,:)];
            [XX,YY,CC] = genfac3d(X,Y,Ctmp);     // CC must be a color matrix of size nf x n
        end

    elseif and(size(X)>1) & and(size(Y)>1) // X and Y are matrices

        if or(size(X) ~= size(Y)) then
            ResetFigureDDM(current_figure, cur_draw_mode);
            error(msprintf(gettext("%s: Wrong size for input arguments ''%s'' and ''%s'': Matrices of same size expected.\n"), "surf", "X", "Y"));
            return;
        end

        if or(size(X) ~= size(Z)) then
            ResetFigureDDM(current_figure, cur_draw_mode);
            error(msprintf(gettext("%s: Wrong size for input arguments ''%s'' and ''%s'': Matrices of same size expected.\n"), "surf", "X", "Z"));
            return;
        end

        [XX,YY,ZZ] = nf3d(X,Y,Z);

        // COLOR treatment
        if (size(C) == size(Z)) // color number == zdata number
            [XX,YY,CC] = nf3d(X,Y,C);     // CC must be a color matrix of size nf x n
        elseif (size(C) == size(Z)-1) // color number -1 == zdata number => ONLY flat mode can be enabled
            Ctmp=[];
            Ctmp = [C [C(:,$)]] ;
            Ctmp = [Ctmp; Ctmp($,:)];
            [XX,YY,CC] = nf3d(X,Y,Ctmp);     // CC must be a color matrix of size nf x n
        end

    elseif or(size(X)==1) & and(size(Y)>1) // X is a vector and Y is a matrix

        if size(X,"*") ~= size(Z,2) then
            ResetFigureDDM(current_figure, cur_draw_mode);
            error(msprintf(gettext("%s: Wrong size for input argument ''%s'': A vector of size %d expected.\n"), "surf", "X", size(Z,2)));
            return;
        end

        if or(size(Y) ~= size(Z)) then
            ResetFigureDDM(current_figure, cur_draw_mode);
            error(msprintf(gettext("%s: Wrong size for input arguments ''%s'' and ''%s'': Matrices of same size expected.\n"), "surf", "Y", "Z"));
            return;
        end

        // X vector
        // Y matrix
        // Z matrix

        X=X(:)'; // X is forced to be a row vector
        XMAT=[];

        for i=1:size(Z,2)
            XMAT=[XMAT;X];
        end



        [XX,YY,ZZ] = nf3d(XMAT,Y,Z);

        // COLOR treatment
        if (size(C) == size(Z)) // color number == zdata number
            [XX,YY,CC] = nf3d(XMAT,Y,C);     // CC must be a color matrix of size nf x n
        elseif (size(C) == size(Z)-1) // color number -1 == zdata number => ONLY flat mode can be enabled
            Ctmp=[];
            Ctmp = [C [C(:,$)]] ;
            Ctmp = [Ctmp; Ctmp($,:)];
            [XX,YY,CC] = nf3d(XMAT,Y,Ctmp);     // CC must be a color matrix of size nf x n
        end

    elseif or(size(Y)==1) & and(size(X)>1) // Y is a vector and X is a matrix

        if or(size(X) ~= size(Z)) then
            ResetFigureDDM(current_figure, cur_draw_mode);
            error(msprintf(gettext("%s: Wrong size for input arguments ''%s'' and ''%s'': Matrices of same size expected.\n"), "surf", "X", "Z"));
            return;
        end

        if size(Y,"*") ~= size(Z,2) then
            ResetFigureDDM(current_figure, cur_draw_mode);
            error(msprintf(gettext("%s: Wrong size for input argument ''%s'': A vector of size %d expected.\n"), "surf", "Y", size(Z,2)));
            return;
        end


        // Y vector
        // X matrix
        // Z matrix

        Y=Y(:); // Y is forced to be a column vector
        YMAT=[];

        for i=1:size(Z,1)
            YMAT=[YMAT,Y];
        end

        [XX,YY,ZZ] = nf3d(X,YMAT,Z);

        // COLOR treatment
        if (size(C) == size(Z)) // color number == zdata number
            [XX,YY,CC] = nf3d(X,YMAT,C);     // CC must be a color matrix of size nf x n
        elseif (size(C) == size(Z)-1) // color number -1 == zdata number => ONLY flat mode can be enabled
            Ctmp=[];
            Ctmp = [C [C(:,$)]] ;
            Ctmp = [Ctmp; Ctmp($,:)];
            [XX,YY,CC] = nf3d(X,YMAT,Ctmp);     // CC must be a color matrix of size nf x n
        end

    else
        ResetFigureDDM(current_figure, cur_draw_mode);
        error(msprintf(gettext("%s: Wrong size for input arguments ''%s'' and ''%s'': Matrices of same size expected.\n"), "surf", "X", "Y"));
        return;
    end

endfunction

// If an error occurs in the surf code, we need to catch it
// order to reset some default values
function processSurfError(cur_figure, cur_draw_mode)
    // reset data
    ResetFigureDDM(current_figure, cur_draw_mode);

    // get the error
    [err_message, err_number, err_line, err_func] = lasterror(%t);

    // rethrow it

    // for now error can only have a single string as input.
    // If there are several lines we need to concatane them.
    err_message_nbLines = size(err_message, "*");
    if (err_message_nbLines > 1) then
        // put a \n betwee, each string
        err_message(1) = strcat(err_message, "\n");
    end
    error(err_message(1), err_number);
endfunction
