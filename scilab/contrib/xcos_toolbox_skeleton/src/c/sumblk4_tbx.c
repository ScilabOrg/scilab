#include "scicos_block4.h"

static void perform_sum(int n, double* y, int nin, double** u);

void sumblk4_tbx(scicos_block *block, int flag)
{
    switch(flag) {
        case DerivativeState:
        {
            // no specific thing for derivative calculus (continous one)
        }
        break;
        case OutputUpdate:
        {
            // update the out signals
            perform_sum(block->outsz[0], (double*) block->outptr, block->nin, (double**) block->inptr);
        }
        break;
        case StateUpdate:
        {
            // this block has no state
        }
        break;
        case OutputEventTiming:
        {
            // no event is generated
        }
        break;
        case Initialization:
        {
            // no initialization (allocation)
        }
        break;
        case Ending:
        {
            // no de-initialization (des-allocation)
        }
        break;
        case ReInitialization:
        {
            // no re-initialization (allocation then des-allocation)
        }
        break;
        default:
        break;
    }    
}

static void perform_sum(int n, double* y, int nin, double** u) {
    int i;
    int k;

    for (i = 0; i < n; i++)
    {
        y[i] = 0.0;     // there is no shift

        for (k = 0; k < nin; k++)
        {
            y[i] = y[i] + u[k][i];
        }
    }
}
