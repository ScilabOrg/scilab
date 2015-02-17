/*  Scicos
*
*  Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/
#include <cstring>
#include <cstdio>

#include "createblklist.hxx"
#include "double.hxx"
#include "function.hxx"
#include "execvisitor.hxx"
#include "vec2var.hxx"
#include "var2vec.hxx"

extern "C"
{
#include "sciblk4.h"
#include "machine.h" /*C2F*/
#include "sci_malloc.h"
#include "scicos.h"
#include "import.h"
#include "Scierror.h"
#include "localization.h"
    /*--------------------------------------------------------------------------*/
    /* Define external functions */
    extern int C2F(scierr)();
    extern void C2F(scifunc)(int&, int&);
    extern int* listentry(int* header, int i);
    extern void C2F(itosci)(int&, int, int);
}
/*--------------------------------------------------------------------------*/
template <class T>
bool sci2var(T* p, void* dest, int row, int col)
{
    int size = p->getSize();
    T::type* srcR = p->get();

    if (row != p->getRows())
    {
        return false;
    }

    if (col != p->getCols())
    {
        return false;
    }

    if (p->isComplex())
    {
        T::type* srcI = p->getImg();
        if (dest == nullptr)
        {
            return false;
        }

        T::type* destR = (T::type*)dest;
        T::type* destI = destR + size;
        for (int i = 0; i < size; ++i)
        {
            destR[i] = srcR[i];
            destI[i] = srcI[i];
        }
    }
    else
    {
        if (dest == nullptr)
        {
            return false;
        }

        T::type* destR = (T::type*)dest;
        for (int i = 0; i < size; ++i)
        {
            destR[i] = srcR[i];
        }
    }

    return true;
}
/*--------------------------------------------------------------------------*/
bool sci2var(types::InternalType* p, void* dest, int desttype, int row, int col)
{
    switch (p->getType())
    {
        case types::InternalType::ScilabDouble:
        {
            if (p->getAs<types::Double>()->isComplex() && desttype == SCSCOMPLEX_N)
            {
                return sci2var(p->getAs<types::Double>(), dest, row, col);
            }

            if (p->getAs<types::Double>()->isComplex() == false && desttype == SCSREAL_N)
            {
                return sci2var(p->getAs<types::Double>(), dest, row, col);
            }
        }
        case types::InternalType::ScilabInt8:
        {
            if (desttype == SCSINT8_N)
            {
                return sci2var(p->getAs<types::Int8>(), dest, row, col);
            }
        }
        case types::InternalType::ScilabInt16:
        {
            if (desttype == SCSINT16_N)
            {
                return sci2var(p->getAs<types::Int16>(), dest, row, col);
            }
        }
        case types::InternalType::ScilabInt32:
        {
            if (desttype == SCSINT32_N)
            {
                return sci2var(p->getAs<types::Int32>(), dest, row, col);
            }
        }
        case types::InternalType::ScilabUInt8:
        {
            if (desttype == SCSUINT8_N)
            {
                return sci2var(p->getAs<types::UInt8>(), dest, row, col);
            }
        }
        case types::InternalType::ScilabUInt16:
        {
            if (desttype == SCSUINT16_N)
            {
                return sci2var(p->getAs<types::UInt16>(), dest, row, col);
            }
        }
        case types::InternalType::ScilabUInt32:
        {
            if (desttype == SCSUINT32_N)
            {
                return sci2var(p->getAs<types::UInt32>(), dest, row, col);
            }
        }
    }

    return false;
}
/*--------------------------------------------------------------------------*/
bool getDoubleArray(types::InternalType* p, double* dest)
{
    if (p == nullptr)
    {
        return false;
    }

    if (p->isDouble())
    {
        types::Double* d = p->getAs<types::Double>();
        int size = d->getSize();
        if (size == 0)
        {
            return true;
        }

        if (dest == nullptr)
        {
            return false;
        }

        memcpy(dest, d->get(), sizeof(double) * size);
        return true;
    }

    return false;
}
/*--------------------------------------------------------------------------*/
bool getDoubleArrayAsInt(types::InternalType* p, int* dest)
{
    if (p == nullptr)
    {
        return false;
    }

    if (p->isDouble())
    {
        types::Double* d = p->getAs<types::Double>();
        int size = d->getSize();
        if (size == 0)
        {
            return true;
        }

        double* dbl = d->get();
        for (int i = 0; i < size; ++i)
        {
            dest[i] = static_cast<int>(dbl[i]);
        }
        return true;
    }

    return false;
}
/*--------------------------------------------------------------------------*/
void sciblk4(scicos_block* Blocks, int flag)
{
    int ierr = 0;
    bool bOK = false;
    /* Retrieve block number */
    int kfun = get_block_number();

    /* Retrieve 'funtyp' by import structure */
    int* ptr = nullptr;
    int nv = 0, mv = 0;
    char buf[] = "funtyp";
    ierr = getscicosvarsfromimport(buf, (void**)&ptr, &nv, &mv);
    if (ierr == 0)
    {
        return;
    }
    int* funtyp = (int *)ptr;

    /* Retrieve 'funptr' by import structure */
    void* ptr2 = nullptr;
    int nv2 = 0, mv2 = 0;
    char buf2[] = "funptr";
    ierr = getscicosvarsfromimport(buf2, (void**)&ptr2, &nv2, &mv2);
    if (ierr == 0)
    {
        return;
    }
    void* funptr = (void *)ptr2;

    types::typed_list in, out;
    types::optional_list opt;

    /*****************************
    * Create Scilab tlist Blocks *
    *****************************/
    types::InternalType* pIT = createblklist(Blocks, -1, funtyp[kfun - 1]);
    if (pIT == NULL)
    {
        set_block_error(-1);
        return;
    }

    in.push_back(pIT);
    /* * flag * */
    in.push_back(new types::Double(flag));

    /***********************
    * Call Scilab function *
    ***********************/
    ast::ExecVisitor exec;
    types::Callable* pCall = static_cast<types::Callable*>(Blocks->scsptr);
    if (pCall->call(in, opt, 1, out, &exec) != types::Function::OK)
    {
        return;
    }

    //clear inputs
    //delete in[0];
    //delete in[1];

    pIT = out[0];
    if (pIT->isTList() == false)
    {
        delete pIT;
        return;
    }

    types::TList* t = pIT->getAs<types::TList>();

    switch (flag)
    {
        /**************************
        * update continuous state
        **************************/
        case 0:
        {
            if (Blocks->nx != 0)
            {
                /* 14 - xd */
                if (getDoubleArray(t->getField(L"xd"), Blocks->xd) == false)
                {
                    delete t;
                    return;
                }

                if ((funtyp[kfun - 1] == 10004) || (funtyp[kfun - 1] == 10005))
                {
                    /* 15 - res */
                    if (getDoubleArray(t->getField(L"res"), Blocks->res) == false)
                    {
                        delete t;
                        return;
                    }
                }
            }
            break;
        }
        /**********************
        * update output state
        **********************/
        case 1:
        {
            /* 21 - outptr */
            if (Blocks->nout > 0)
            {
                InternalType* pIT = t->getField(L"outptr");
                if (pIT && pIT->isList())
                {
                    types::List* lout = pIT->getAs<List>();
                    if (Blocks->nout == lout->getSize())
                    {
                        for (int i = 0; i < Blocks->nout; ++i)
                        {
                            //update data
                            int row = Blocks->outsz[i];
                            int col = Blocks->outsz[i + Blocks->nout];
                            int type = Blocks->outsz[i + Blocks->nout * 2];
                            bOK = sci2var(lout->get(i), Blocks->outptr[i], type, row, col);
                        }
                    }
                }
            }
            break;
        }
        case 2:
        {
            /* 7 - z */
            if (Blocks[0].nz != 0)
            {
                if (getDoubleArray(t->getField(L"z"), Blocks->z) == false)
                {
                    delete t;
                    return;
                }
            }

            /* 11 - oz */
            //TODO : how to store object ?

            if (Blocks[0].nx != 0)
            {
                /* 13 - x */
                if (getDoubleArray(t->getField(L"x"), Blocks->x) == false)
                {
                    delete t;
                    return;
                }

                /* 14 - xd */
                if (getDoubleArray(t->getField(L"xd"), Blocks->xd) == false)
                {
                    delete t;
                    return;
                }
            }

            break;
        }

        /***************************
        * update event output state
        ***************************/
        case 3:
        {
            /* 23 - evout */
            if (getDoubleArray(t->getField(L"evout"), Blocks->evout) == false)
            {
                delete t;
                return;
            }
            break;
        }
        /**********************
        * state initialisation
        **********************/
        case 4:
        {
            /* 7 - z */
            if (Blocks[0].nz != 0)
            {
                if (getDoubleArray(t->getField(L"z"), Blocks->z) == false)
                {
                    delete t;
                    return;
                }
            }

            /* 11 - oz */
            //TODO : how to store object ?

            if (Blocks[0].nx != 0)
            {
                /* 13 - x */
                if (getDoubleArray(t->getField(L"x"), Blocks->x) == false)
                {
                    delete t;
                    return;
                }

                /* 14 - xd */
                if (getDoubleArray(t->getField(L"xd"), Blocks->xd) == false)
                {
                    delete t;
                    return;
                }
            }

            break;
        }

        case 5:
        {
            /* 7 - z */
            if (Blocks[0].nz != 0)
            {
                if (getDoubleArray(t->getField(L"z"), Blocks->z) == false)
                {
                    delete t;
                    return;
                }
            }

            /* 11 - oz */
            //TODO : how to store object ?

            break;
        }

        /*****************************
        * output state initialisation
        *****************************/
        case 6:
        {
            /* 7 - z */
            if (Blocks[0].nz != 0)
            {
                if (getDoubleArray(t->getField(L"z"), Blocks->z) == false)
                {
                    delete t;
                    return;
                }
            }

            /* 11 - oz */
            //TODO : how to store object ?

            if (Blocks[0].nx != 0)
            {
                /* 13 - x */
                if (getDoubleArray(t->getField(L"x"), Blocks->x) == false)
                {
                    delete t;
                    return;
                }

                /* 14 - xd */
                if (getDoubleArray(t->getField(L"xd"), Blocks->xd) == false)
                {
                    delete t;
                    return;
                }
            }

            /* 21 - outptr */
            if (Blocks->nout > 0)
            {
                InternalType* pIT = t->getField(L"outptr");
                if (pIT && pIT->isList())
                {
                    types::List* lout = pIT->getAs<List>();
                    if (Blocks->nout == lout->getSize())
                    {
                        for (int i = 0; i < Blocks->nout; ++i)
                        {
                            //update data
                            int row = Blocks->outsz[i];
                            int col = Blocks->outsz[i + Blocks->nout];
                            int type = Blocks->outsz[i + Blocks->nout * 2];
                            bOK = sci2var(lout->get(i), Blocks->outptr[i], type, row, col);
                        }
                    }
                }
            }
            break;
        }

        /*******************************************
        * define property of continuous time states
        * (algebraic or differential states)
        *******************************************/
        case 7:
        {
            if (Blocks[0].nx != 0)
            {
                /* 40 - xprop */
                if (getDoubleArrayAsInt(t->getField(L"xprop"), Blocks->xprop) == false)
                {
                    delete t;
                    return;
                }
            }
            break;
        }

        /****************************
        * zero crossing computation
        ****************************/
        case 9:
        {
            /* 33 - g */
            if (getDoubleArray(t->getField(L"g"), Blocks->g) == false)
            {
                delete t;
                return;
            }

            if (get_phase_simulation() == 1)
            {
                /* 39 - mode */
                if (getDoubleArrayAsInt(t->getField(L"mode"), Blocks->mode) == false)
                {
                    delete t;
                    return;
                }
            }
            break;
        }
        /**********************
        * Jacobian computation
        **********************/
        case 10:
        {
            if ((funtyp[kfun - 1] == 10004) || (funtyp[kfun - 1] == 10005))
            {
                /* 15 - res */
                if (getDoubleArray(t->getField(L"res"), Blocks->res) == false)
                {
                    delete t;
                    return;
                }
            }
            break;
        }
    }

    if (bOK == false)
    {
        Scierror(999, "KABOOM !");
    }
    return;
}
/*--------------------------------------------------------------------------*/
