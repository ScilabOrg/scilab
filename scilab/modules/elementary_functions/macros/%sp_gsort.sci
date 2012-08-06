// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2009 - Allan CORNET
// Copyrifht (C) 2012 - Scilab Enterprises - Adeline CARNIS
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [A, k] = %sp_gsort(A, optsort, directionsort)
    rhs = argn(2);
    lhs = argn(1);
    // arguments by default in gsort
    select rhs
    case 1
        optsort = 'g';
        directionsort = 'd';
    case 2
        directionsort = 'd';
    end

    [ij, v, mn] = spget(A);
    if mn(1) <> 1 & mn(2) <> 1 then
        error(999,msprintf(_("%s: Wrong size for input argument #%d: sparse vectors expected.\n"),'gsort',1));
    end

    // if optsort='c', the result is the same because the first argument is a 
    // column vector
    if (strcmp(optsort, 'c')) == 0 then
        A = A;
        //k is a vector containing the origin indices
        // As the first argument is a column vector, k contains only 1
        if lhs == 2 then
            if mn(2) == 1 then
                k = ij(:,2);
            else
                k = ij(:,1);
            end
        end
    else

        if mn(2) == 1 then
            dif = mn(1) - length(v);
            v = gsort(v, optsort, directionsort);

            //k is a vector containing the origin indices
            if lhs == 2 then
                k = [];
                for i = 1:length(v)
                    k = [k; find(v(i) == A)];
                end
            end

            //Obtain the indices corresponding to positive values of v
            // and negative value of v
            // If A is complex, the elements are sorted by magnitude
            if isreal(A) then
                last = find(v<0);
                first = find(v>0);
            else
                s = abs(v);
                last = find(s<0);
                first = find(s>0);
            end

            // sort the indices in terms of directionsort = 'i' or 'd'
            // if directionsort='i' and v>0, the elements are sorted in the 
            // increasing order, ie [0,..,v] and, conversely, in the decreasing
            // order the elements are sorted : [v,..,0]
            // if v<0, the elements are sorted in the increasing order, 
            // ie [v,..,0] and, conversely, in the decreasing order the 
            // elements are sorted : [0,..,v]
            // And else, if v contains positive and neqative values, the 
            // elements are sorted in the increasing order,ie [v_neg,0,v_pos],
            // and conversely for the decreasing order.
            if last == [] & first <> [] then
                if strcmp(directionsort, 'i')== 0 then
                    ij(:,1) = first(:) + dif;
                else
                    ij(:,1) = first(:);
                end
            elseif first == [] & last <> [] then
                if strcmp(directionsort, 'i')== 0 then
                    ij(:,1) = last(:);
                else
                    ij(:,1) = last(:) + dif;
                end
            else
                if strcmp(directionsort, 'i')== 0 then
                    ij(:,1) = [last(:); first(:) + dif];
                else
                    ij(:,1) = [first(:); last(:) + dif];
                end
            end

        elseif mn(1) == 1 then
            dif = mn(2) - length(v);
            v = gsort(v, optsort, directionsort);

            //k is a vector containing the origin indices
            if lhs == 2 then
                k = [];
                for i = 1:length(v)
                    k = [k; find(v(i) == A)];
                end
            end

            //Obtain the indices corresponding to positive values of v
            // and negative value of v
            // If A is complex, the elements are sorted by magnitude
            if isreal(A) then
                last = find(v<0);
                first = find(v>0);
            else
                s = abs(v);
                last = find(s<0);
                first = find(s>0);
            end

            // Sort the indices
            if last == [] & first <> [] then
                if strcmp(directionsort, 'i')== 0 then
                    ij(:,2) = first(:) + dif;
                else
                    ij(:,2) = first(:);
                end
            elseif first == [] & last <> [] then
                if strcmp(directionsort, 'i')== 0 then
                    ij(:,1) = last(:);
                else
                    ij(:,1) = last(:) + dif;
                end
            else
                if strcmp(directionsort, 'i')== 0 then
                    ij(:,2) = [last(:); first(:) + dif];
                else
                    ij(:,2) = [first(:); last(:) + dif];
                end
            end
        end
        A = sparse(ij, v, mn)
    end

endfunction















