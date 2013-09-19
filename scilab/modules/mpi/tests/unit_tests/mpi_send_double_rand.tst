// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2010 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- MPI TEST -->
// 
MPI_Init();
rnk =    MPI_Comm_rank();
sizeNodes =    MPI_Comm_size();
lines(0)
assert_checkequal(MPI_Comm_size(), 2);

SLV = rnk;                // handy shortcuts, master is rank 0
Master = ~ SLV;            // slaves are all other

if Master

    for slaveId = 1:sizeNodes-1
        MPI_Send(rand(10,10),slaveId)
    end

    for slaveId = 1:sizeNodes-1
        tag=0
        valueBack=MPI_Recv(slaveId, tag);
        assert_checkequal(size(valueBack),[10,10])
    end
else
    rankSource=0;
    tag=0;
    value=MPI_Recv(rankSource, tag)
    value=value+1;
    // Send back to the master
    MPI_Send(value,0)
end

MPI_Finalize()
exit();
