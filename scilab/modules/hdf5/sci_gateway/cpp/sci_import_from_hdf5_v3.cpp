/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
#include <list>
#include <map>
#include <hdf5.h>
#include "hdf5_gw.hxx"
#include "context.hxx"
#include "string.hxx"
#include "double.hxx"
#include "struct.hxx"
#include "cell.hxx"
#include "int.hxx"
#include "list.hxx"
#include "tlist.hxx"
#include "mlist.hxx"
#include "polynom.hxx"
#include "sparse.hxx"
#include "graphichandle.hxx"
#include "void.hxx"
#include "listundefined.hxx"
#include "context.hxx"
#include "handle_properties.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "os_string.h"
#include "deleteafile.h"
#include "expandPathVariable.h"
#include "h5_fileManagement.h"
#include "h5_writeDataToFile.h"
#include "h5_readDataFromFile.h"
#include "h5_attributeConstants.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "HandleManagement.h"
#include "createGraphicObject.h"
#include "FigureList.h"
#include "BuildObjects.h"
}
/*--------------------------------------------------------------------------*/
static bool import_variable(int file, std::string& name);
static types::InternalType* import_data(int dataset);
static types::InternalType* import_double(int dataset);
static types::InternalType* import_string(int dataset);
static types::InternalType* import_boolean(int dataset);
static types::InternalType* import_int(int dataset);
static types::InternalType* import_list(int dataset, types::List* lst);
static types::InternalType* import_struct(int dataset);
static types::InternalType* import_poly(int dataset);
static types::InternalType* import_cell(int dataset);
static types::InternalType* import_handles(int dataset);

static types::InternalType* import_sparse(int dataset);

static bool import_boolean_sparse(int dataset);

/*--------------------------------------------------------------------------*/
static const std::string fname("import_from_hdf5");
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_import_from_hdf5_v3(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    std::string filename;
    int rhs = static_cast<int>(in.size());

    if (rhs < 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): at least %d expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    if (in[0]->getId() != types::InternalType::IdScalarString)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    wchar_t* wfilename = expandPathVariableW(in[0]->getAs<types::String>()->get()[0]);
    char* cfilename = wide_string_to_UTF8(wfilename);
    filename = cfilename;
    FREE(wfilename);
    FREE(cfilename);

    int iFile = openHDF5File(filename.data(), 0);
    if (iFile < 0)
    {
        Scierror(999, _("%s: Unable to open file: %s\n"), fname.data(), filename.data());
        return types::Function::Error;
    }

    //manage version information
    int iVersion = getSODFormatAttribute(iFile);
    if (iVersion != SOD_FILE_VERSION)
    {
        if (iVersion > SOD_FILE_VERSION)
        {
            //can't read file with version newer that me !
            Scierror(999, _("%s: Wrong SOD file format version. Max Expected: %d Found: %d\n"), fname.data(), SOD_FILE_VERSION, iVersion);
            return types::Function::Error;
        }
        else
        {
            //call older import functions and exit or ... EXIT !
            if (iVersion == 1 || iVersion == -1)
            {
                //sciprint("old sci_import_from_hdf5_v1\n");
                //return sci_import_from_hdf5_v1(fname, pvApiCtx);
            }
        }
    }

    if (rhs > 1)
    {
        for (int i = 1; i < rhs; ++i)
        {
            std::string var;
            char* cvar = wide_string_to_UTF8(in[i]->getAs<types::String>()->get()[0]);
            var = cvar;
            FREE(cvar);

            if (import_variable(iFile, var) == false)
            {
                Scierror(999, _("%s: Unable to load \'%s\'.\n"), fname.data(), var);
                return types::Function::Error;
            }
        }
    }
    else
    {
        //restore all variables
        int iNbItem = 0;
        iNbItem = getVariableNames6(iFile, NULL);
        if (iNbItem != 0)
        {
            std::vector<char*> vars(iNbItem, nullptr);
            iNbItem = getVariableNames6(iFile, vars.data());
            for (auto &var : vars)
            {
                std::string s(var);
                if (import_variable(iFile, s) == false)
                {
                    Scierror(999, _("%s: Unable to load \'%s\'.\n"), fname.data(), var);
                    return types::Function::Error;
                }
            }
        }
    }

    //close the file
    closeHDF5File(iFile);

    out.push_back(new types::Bool(1));
    return types::Function::OK;
}

static bool import_variable(int file, std::string& name)
{
    int dataset = getDataSetIdFromName(file, name.data());
    if (dataset <= 0)
    {
        return false;
    }

    types::InternalType* data = import_data(dataset);
    if (data != nullptr)
    {
        wchar_t* var = to_wide_string(name.data());
        //add to context
        symbol::Context::getInstance()->put(symbol::Symbol(var), data);
        FREE(var);
        return true;
    }

    return false;
}

static types::InternalType* import_data(int dataset)
{
    //get var type
    char* ctype = getScilabTypeFromDataSet6(dataset);
    std::string type(ctype);
    FREE(ctype);

    if (type == g_SCILAB_CLASS_DOUBLE)
    {
        return import_double(dataset);
    }

    if (type == g_SCILAB_CLASS_STRING)
    {
        return import_string(dataset);
    }

    if (type == g_SCILAB_CLASS_INT)
    {
        return import_int(dataset);
    }

    if (type == g_SCILAB_CLASS_BOOLEAN)
    {
        return import_boolean(dataset);
    }

    if (type == g_SCILAB_CLASS_LIST)
    {
        return import_list(dataset, new types::List());
    }

    if (type == g_SCILAB_CLASS_TLIST)
    {
        return import_list(dataset, new types::TList());
    }

    if (type == g_SCILAB_CLASS_MLIST)
    {
        return import_list(dataset, new types::MList());
    }

    if (type == g_SCILAB_CLASS_STRUCT)
    {
        return import_struct(dataset);
    }

    if (type == g_SCILAB_CLASS_POLY)
    {
        return import_poly(dataset);
    }

    if (type == g_SCILAB_CLASS_SPARSE)
    {
        return import_sparse(dataset);
    }

    if (type == g_SCILAB_CLASS_CELL)
    {
        return import_cell(dataset);
    }

    if (type == g_SCILAB_CLASS_HANDLE)
    {
        return import_handles(dataset);
    }

    if (type == g_SCILAB_CLASS_VOID)
    {
        closeDataSet(dataset);
        return new types::Void();
    }

    if (type == g_SCILAB_CLASS_UNDEFINED)
    {
        closeDataSet(dataset);
        return new types::ListUndefined();
    }

    return nullptr;
}

static types::InternalType* import_double(int dataset)
{
    int complex = 0;
    int dims = 0;
    int ret = getDatasetInfo(dataset, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(dataset);
        return nullptr;
    }

    std::vector<int> d(dims, 0);
    int size = getDatasetInfo(dataset, &complex, &dims, d.data());


    if (dims == 0 || size == 0)
    {
        closeDataSet(dataset);
        return types::Double::Empty();
    }

    types::Double* dbl = new types::Double(dims, d.data(), complex == 1);

    double* real = dbl->get();
    double* img = dbl->getImg();

    if (complex)
    {
        readDoubleComplexMatrix(dataset, real, img);
    }
    else
    {
        readDoubleMatrix(dataset, real);
    }
    return dbl;
}

static types::InternalType* import_string(int dataset)
{
    int complex = 0;
    int dims = 0;
    int ret = getDatasetInfo(dataset, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(dataset);
        return nullptr;
    }


    std::vector<int> d(dims, 0);
    int size = getDatasetInfo(dataset, &complex, &dims, d.data());


    if (dims == 0 || size == 0)
    {
        closeDataSet(dataset);
        return types::Double::Empty();
    }

    std::vector<char*> s(size, nullptr);
    ret = readStringMatrix(dataset, s.data());

    types::String* str = new types::String(dims, d.data());
    wchar_t** pstr = str->get();

    for (int i = 0; i < size; ++i)
    {
        pstr[i] = to_wide_string(s[i]);
    }

    freeStringMatrix(dataset, s.data());

    return str;
}

static types::InternalType* import_boolean(int dataset)
{
    int complex = 0;
    int dims = 0;
    int ret = getDatasetInfo(dataset, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(dataset);
        return nullptr;
    }

    std::vector<int> d(dims, 0);
    int size = getDatasetInfo(dataset, &complex, &dims, d.data());


    if (dims == 0 || size == 0)
    {
        closeDataSet(dataset);
        return types::Double::Empty();
    }

    types::Bool* bools = new types::Bool(dims, d.data());

    int* b = bools->get();
    readBooleanMatrix(dataset, b);
    return bools;
}

static types::InternalType* import_int(int dataset)
{
    types::InternalType* pOut = nullptr;
    int complex = 0;
    int dims = 0;
    int ret = getDatasetInfo(dataset, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(dataset);
        return nullptr;
    }

    std::vector<int> d(dims, 0);
    int size = getDatasetInfo(dataset, &complex, &dims, d.data());


    if (dims == 0 || size == 0)
    {
        closeDataSet(dataset);
        return types::Double::Empty();
    }

    int prec = 0;
    ret = getDatasetPrecision(dataset, &prec);
    if (ret != 0)
    {
        closeDataSet(dataset);
        return nullptr;
    }

    switch (prec)
    {
        case SCI_INT8:
        {
            types::Int8* pi = new types::Int8(dims, d.data());
            ret = readInteger8Matrix(dataset, pi->get());
            pOut = pi;
            break;
        }
        case SCI_INT16:
        {
            types::Int16* pi = new types::Int16(dims, d.data());
            ret = readInteger16Matrix(dataset, pi->get());
            pOut = pi;
            break;
        }
        case SCI_INT32:
        {
            types::Int32* pi = new types::Int32(dims, d.data());
            ret = readInteger32Matrix(dataset, pi->get());
            pOut = pi;
            break;
        }
        case SCI_INT64:
        {
            types::Int64* pi = new types::Int64(dims, d.data());
            ret = readInteger64Matrix(dataset, pi->get());
            pOut = pi;
            break;
        }
        case SCI_UINT8:
        {
            types::UInt8* pi = new types::UInt8(dims, d.data());
            ret = readUnsignedInteger8Matrix(dataset, pi->get());
            pOut = pi;
            break;
        }
        case SCI_UINT16:
        {
            types::UInt16* pi = new types::UInt16(dims, d.data());
            ret = readUnsignedInteger16Matrix(dataset, pi->get());
            pOut = pi;
            break;
        }
        case SCI_UINT32:
        {
            types::UInt32* pi = new types::UInt32(dims, d.data());
            ret = readUnsignedInteger32Matrix(dataset, pi->get());
            pOut = pi;
            break;
        }
        case SCI_UINT64:
        {
            types::UInt64* pi = new types::UInt64(dims, d.data());
            ret = readUnsignedInteger64Matrix(dataset, pi->get());
            pOut = pi;
            break;
        }
        default:
            return nullptr;
            break;
    }

    return pOut;
}

static types::InternalType* import_list(int dataset, types::List* lst)
{
    int count = 0;
    int ret  = getListDims6(dataset, &count);
    if (ret)
    {
        closeList6(dataset);
        delete lst;
        return nullptr;
    }

    if (count == 0)
    {
        closeList6(dataset);
        return lst;
    }

    //get children

    for (int i = 0; i < count; ++i)
    {
        int data = getDataSetIdFromName(dataset, std::to_string(i).data());
        if (data <= 0)
        {
            closeList6(dataset);
            delete lst;
            return nullptr;
        }

        types::InternalType* child = import_data(data);
        if (child == nullptr)
        {
            closeList6(dataset);
            delete lst;
            return nullptr;
        }

        lst->append(child);
    }

    closeList6(dataset);
    return lst;
}

static int getDimsNode(int dataset, int* complex, std::vector<int>& dims)
{
    dims.clear();
    int id = getDataSetIdFromName(dataset, "__dims__");
    if (id < 0)
    {
        return 0;
    }

    //get dims dimension count
    int dim = 0;
    getDatasetInfo(id, complex, &dim, NULL);
    //get dims dimension
    std::vector<int> d(dim, 0);
    int size = getDatasetInfo(id, complex, &dim, d.data());

    //get dims value
    dims.resize(size);
    readInteger32Matrix(id, dims.data());

    size = dims[0];
    for (int i = 1; i < dims.size(); ++i)
    {
        size *= dims[i];
    }
    return size;
}

static types::InternalType* import_struct(int dataset)
{
    //get struct dims node
    int complex = 0;
    std::vector<int> pdims;
    int size = getDimsNode(dataset, &complex, pdims);

    types::Struct* str = new types::Struct(static_cast<int>(pdims.size()), pdims.data());
    size = str->getSize();
    if (size == 0)
    {
        //empty struct
        closeList6(dataset);
        return new types::Struct();
    }

    types::SingleStruct** sstr = str->get();

    int fieldCount = 0;
    int ret = getListDims6(dataset, &fieldCount);
    if (ret < 0)
    {
        closeList6(dataset);
        return str;
    }

    //open __refs__ node
    int refs = getDataSetIdFromName(dataset, "__refs__");

    H5O_info_t oinfo;
    for (int i = 0; i < fieldCount; ++i)
    {
        H5Oget_info_by_idx(dataset, ".", H5_INDEX_NAME, H5_ITER_NATIVE, i, &oinfo, H5P_DEFAULT);
        ssize_t len = H5Lget_name_by_idx(dataset, ".", H5_INDEX_NAME, H5_ITER_INC, i, 0, 0, H5P_DEFAULT) + 1;
        char* name = (char*)MALLOC(sizeof(char) * len);
        H5Lget_name_by_idx(dataset, ".", H5_INDEX_NAME, H5_ITER_INC, i, name, len, H5P_DEFAULT);

        if (strcmp(name, "__dims__") != 0 && strcmp(name, "__refs__") != 0)
        {
            wchar_t* field = to_wide_string(name);
            str->addField(field);

            int dataref = getDataSetIdFromName(dataset, name);
            if (dataref < 0)
            {
                closeList6(dataset);
                FREE(name);
                return nullptr;
            }

            int refdim = 0;
            getDatasetInfo(dataref, &complex, &refdim, NULL);
            std::vector<int> refdims(refdim, 0);
            int refcount = getDatasetInfo(dataref, &complex, &refdim, refdims.data());
            std::vector<hobj_ref_t> vrefs(refcount, 0);
            ret = H5Dread(dataref, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, vrefs.data());
            if (ret < 0)
            {
                FREE(name);
                return nullptr;
            }


            //import field
            for (int j = 0; j < refcount; ++j)
            {
                int data = H5Rdereference(refs, H5R_OBJECT, &vrefs[j]);
                if (data < 0)
                {
                    FREE(name);
                    return nullptr;
                }

                types::InternalType* val = import_data(data);
                if (val == nullptr)
                {
                    FREE(name);
                    return nullptr;
                }

                sstr[j]->set(field, val);

            }
            FREE(field);

            closeDataSet(dataref);
        }

        FREE(name);
    }
    //str->addField();

    closeList6(refs);
    closeList6(dataset);
    return str;
}

static types::InternalType* import_poly(int dataset)
{
    //get poly dims node
    int complex = 0;
    std::vector<int> pdims;
    int size = getDimsNode(dataset, &complex, pdims);

    //get variable name
    char* var = NULL;
    int varname = getDataSetIdFromName(dataset, "__varname__");
    readStringMatrix(varname, &var);
    wchar_t* wvar = to_wide_string(var);
    std::wstring wvarname(wvar);
    FREE(wvar);
    freeStringMatrix(varname, &var);

    types::Polynom* p = new types::Polynom(wvarname, static_cast<int>(pdims.size()), pdims.data());
    types::SinglePoly** pss = p->get();

    //open __refs__ node
    int refs = getDataSetIdFromName(dataset, "__refs__");
    size = p->getSize();

    //loop on children
    for (int i = 0; i < size; ++i)
    {
        //forge name
        std::string polyname(std::to_string(i));
        int poly = getDataSetIdFromName(refs, polyname.data());

        //get dims
        complex = 0;
        int dims = 0;
        int ret = getDatasetInfo(poly, &complex, &dims, NULL);
        if (ret < 0)
        {
            return nullptr;
        }

        std::vector<int> d(dims);
        int datasize = getDatasetInfo(poly, &complex, &dims, d.data());

        types::SinglePoly* ss = NULL;

        //get coef
        if (dims == 0 || datasize == 0)
        {
            ss = new types::SinglePoly();
        }
        else if (complex)
        {
            double* real = NULL;
            double* img = NULL;
            //create singlepoly
            ss = new types::SinglePoly(&real, &img, datasize - 1);
            readDoubleComplexMatrix(poly, real, img);
        }
        else
        {
            double* real = NULL;
            ss = new types::SinglePoly(&real, datasize - 1);
            readDoubleMatrix(poly, real);
        }

        pss[i] = ss;
    }

    closeList6(refs);
    closeList6(dataset);
    return p;
}

static types::InternalType* import_sparse(int dataset)
{
    types::Sparse* sp = nullptr;
    //get sparse dimensions
    int complex = 0;
    std::vector<int> pdims;
    int size = getDimsNode(dataset, &complex, pdims);

    //get non zeros count
    int nnz = 0;
    int datannz = getDataSetIdFromName(dataset, "__nnz__");
    readInteger32Matrix(datannz, &nnz);

    if (nnz == 0)
    {
        closeList6(dataset);
        return new types::Sparse(0, 0);
    }

    //get inner vector
    int datain = getDataSetIdFromName(dataset, "__inner__");
    int dimin = 0;
    int sizein = getDatasetInfo(datain, &complex, &dimin, NULL);
    std::vector<int> dimsin(dimin, 0);
    sizein = getDatasetInfo(datain, &complex, &dimin, dimsin.data());

    std::vector<int> in(sizein, 0);
    int ret = readInteger32Matrix(datain, in.data());
    if (ret < 0)
    {
        closeList6(dataset);
        return nullptr;
    }

    //get outer vector
    int dataout = getDataSetIdFromName(dataset, "__outer__");
    int dimout = 0;
    int sizeout = getDatasetInfo(dataout, &complex, &dimout, NULL);
    std::vector<int> dimsout(dimout, 0);
    sizeout = getDatasetInfo(dataout, &complex, &dimout, dimsout.data());

    std::vector<int> out(sizeout, 0);
    ret = readInteger32Matrix(dataout, out.data());
    if (ret < 0)
    {
        closeList6(dataset);
        return nullptr;
    }

    //get data
    int ddata = getDataSetIdFromName(dataset, "__data__");
    int dimdata = 0;
    int sizedata = getDatasetInfo(ddata, &complex, &dimdata, NULL);
    std::vector<int> dimsdata(dimdata, 0);
    sizedata = getDatasetInfo(ddata, &complex, &dimdata, dimsdata.data());

    std::vector<double> real(sizedata, 0);

    if (complex)
    {
        std::vector<double> img(sizedata, 0);
        ret = readDoubleComplexMatrix(ddata, real.data(), img.data());
        if (ret < 0)
        {
            closeList6(dataset);
            return nullptr;
        }

        sp = new types::Sparse(pdims[0], pdims[1], nnz, in.data(), out.data(), real.data(), img.data());
    }
    else
    {
        ret = readDoubleMatrix(ddata, real.data());
        if (ret < 0)
        {
            closeList6(dataset);
            return nullptr;
        }

        sp = new types::Sparse(pdims[0], pdims[1], nnz, in.data(), out.data(), real.data(), nullptr);
    }

    closeList6(dataset);
    return sp;
}

static types::InternalType* import_cell(int dataset)
{
    //get sparse dimensions
    int complex = 0;
    std::vector<int> pdims;
    int size = getDimsNode(dataset, &complex, pdims);

    if (size == 0)
    {
        closeList6(dataset);
        return new types::Cell();
    }

    //open __refs__ node
    int refs = getDataSetIdFromName(dataset, "__refs__");
    std::vector<types::InternalType*> data(size, nullptr);
    for (int i = 0; i < size; ++i)
    {
        int ref = getDataSetIdFromName(refs, std::to_string(i).data());
        types::InternalType* val = import_data(ref);
        if (val == nullptr)
        {
            return nullptr;
        }

        data[i] = val;
    }

    types::Cell* cell = new types::Cell(static_cast<int>(pdims.size()), pdims.data(), data.data());

    closeList6(refs);
    closeList6(dataset);
    return cell;
}

static int getHandleInt(int dataset, const char* prop)
{
    int node = 0;
    node = getDataSetIdFromName(dataset, prop);
    if (node < 0)
    {
        return -1;
    }

    int val = 0;
    readInteger32Matrix(node, &val);
    return val;
}

static int getHandleIntVector(int dataset, const char* prop, int* row, int* col, int** vals)
{
    int node = 0;
    node = getDataSetIdFromName(dataset, prop);
    if (node < 0)
    {
        return -1;
    }

    int complex = 0;
    int dims = 0;;
    int ret = getDatasetInfo(node, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(dataset);
        return -1;
    }


    std::vector<int> d(dims);
    int size = getDatasetInfo(node, &complex, &dims, d.data());

    if (dims == 0 || size == 0)
    {
        closeDataSet(node);
        return -1;
    }

    if (*row && *row != d[0])
    {
        //error ?
    }

    if (*col && *col != d[1])
    {
        //error ?
    }

    *row = d[0];
    *col = d[1];
    *vals = new int[size];
    readInteger32Matrix(node, *vals);
    return 0;
}

static int getHandleBool(int dataset, const char* prop)
{
    int node = 0;
    node = getDataSetIdFromName(dataset, prop);
    if (node < 0)
    {
        return -1;
    }

    int val = 0;
    readBooleanMatrix(node, &val);
    return val;
}

static int getHandleBoolVector(int dataset, const char* prop, int* row, int* col, int** vals)
{
    int node = 0;
    node = getDataSetIdFromName(dataset, prop);
    if (node < 0)
    {
        return -1;
    }

    int complex = 0;
    int dims = 0;;
    int ret = getDatasetInfo(node, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(dataset);
        return -1;
    }


    std::vector<int> d(dims, 0);
    int size = getDatasetInfo(node, &complex, &dims, d.data());

    if (dims == 0 || size == 0)
    {
        closeDataSet(node);
        return -1;
    }

    if (*row && *row != d[0])
    {
        //error ?
    }

    if (*col && *col != d[1])
    {
        //error ?
    }

    *row = d[0];
    *col = d[1];

    *vals = new int[size];
    readBooleanMatrix(node, *vals);
    return 0;
}

static double getHandleDouble(int dataset, const char* prop)
{
    int node = 0;
    node = getDataSetIdFromName(dataset, prop);
    if (node < 0)
    {
        return -1;
    }

    double val = 0;
    readDoubleMatrix(node, &val);
    return val;
}

static int getHandleDoubleVector(int dataset, const char* prop, int* row, int* col, double** vals)
{
    int node = 0;
    node = getDataSetIdFromName(dataset, prop);
    if (node < 0)
    {
        return -1;
    }

    int complex = 0;
    int dims = 0;;
    int ret = getDatasetInfo(node, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(dataset);
        return -1;
    }


    std::vector<int> d(dims, 0);
    int size = getDatasetInfo(node, &complex, &dims, d.data());

    if (dims == 0 || size == 0)
    {
        closeDataSet(node);
        return -1;
    }

    if (*row && *row != d[0])
    {
        //error ?
    }

    if (*col && *col != d[1])
    {
        //error ?
    }

    *row = d[0];
    *col = d[1];

    *vals = new double[size];
    readDoubleMatrix(node, *vals);
    return 0;
}

static std::string getHandleString(int dataset, const char* prop)
{
    int node = 0;
    node = getDataSetIdFromName(dataset, prop);
    if (node < 0)
    {
        return "";
    }

    int complex = 0;
    int dims = 0;;
    int ret = getDatasetInfo(node, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(dataset);
        return "";
    }


    std::vector<int> d(dims, 0);
    int size = getDatasetInfo(node, &complex, &dims, d.data());

    if (dims == 0 || size == 0)
    {
        closeDataSet(node);
        return "";
    }

    std::vector<char*> s(size, nullptr);
    ret = readStringMatrix(node, s.data());

    return s[0];
}

static int getHandleStringVector(int dataset, const char* prop, int* row, int* col, char*** vals)
{
    int node = 0;
    node = getDataSetIdFromName(dataset, prop);
    if (node < 0)
    {
        return -1;
    }

    int complex = 0;
    int dims = 0;;
    int ret = getDatasetInfo(node, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(dataset);
        return -1;
    }


    std::vector<int> d(dims, 0);
    int size = getDatasetInfo(node, &complex, &dims, d.data());

    if (dims == 0 || size == 0)
    {
        closeDataSet(node);
        return -1;
    }

    if (*row && *row != d[0])
    {
        //error ?
    }

    if (*col && *col != d[1])
    {
        //error ?
    }

    *row = d[0];
    *col = d[1];

    *vals = new char*[size];
    readStringMatrix(node, *vals);
    return node;
}

static void import_userdata(int dataset, int uid);
static int import_handle_generic(int dataset, int uid, HandleProp& props, bool log = false);
static int import_champ_handle(int dataset);
static int import_axes_handle(int dataset);
static int import_figure_handle(int dataset);
static int import_handle(int dataset);

//not really a child
static int import_label_handle(int dataset, int uid);

static types::InternalType* import_handles(int dataset);

static void import_userdata(int dataset, int uid)
{
    types::InternalType* ud = nullptr;
    int node = 0;
    node = getDataSetIdFromName(dataset, "userdata");
    if (node < 0)
    {
        //no user data ?
        //assign an empty matrix
        types::InternalType* ud = types::Double::Empty();
    }
    else
    {
        ud = import_data(node);
    }

    //increase ref before tryong to delete old value to avoid double free
    ud->IncreaseRef();

    //get previous value
    int size = 0;
    int *psize = &size;
    int *data = NULL;

    getGraphicObjectProperty(uid, __GO_USER_DATA_SIZE__, jni_int, (void **)&psize);
    getGraphicObjectProperty(uid, __GO_USER_DATA__, jni_int_vector, (void **)&data);

    if (size != 0)
    {
        types::InternalType* previous = nullptr;
        if (size == 1)
        {
            //32 bits
            int* p = (int*)data;
            previous = ((types::InternalType*) * p);
        }
        else
        {
            //64 bits
            long long* p = (long long*)data;
            previous = ((types::InternalType*) * p);
        }

        previous->DecreaseRef();
        previous->killMe();
    }

    //set new value
    size = sizeof(void*) / sizeof(int);
    setGraphicObjectProperty(uid, __GO_USER_DATA__, &ud, jni_int_vector, size);
}


static int import_handle_generic(int dataset, int uid, HandleProp& props, bool log)
{
    //restore children before other property in case of properties has an
    //effect on children

    //reload children
    int children = getDataSetIdFromName(dataset, "children");
    int childcount = 0;
    getListDims6(children, &childcount);

    for (int i = 0; i < childcount; ++i)
    {
        int c = getDataSetIdFromName(children, std::to_string(i).data());
        int newChild = import_handle(c);
        setGraphicObjectRelationship(uid, newChild);
    }



    for (auto& prop : props)
    {
        const char* name = prop.first.data();
        std::vector<int> info(prop.second);

        if (log)
        {
            //std::cout << "prop : " << name << std::endl;
            //if (prop.first == "arrow_size")
            //{
            //    std::cout << "prop : " << name << std::endl;
            //}
        }

        if (info[0] == SAVE_ONLY)
        {
            continue;
        }

        int go = info[1];
        int type = info[2];
        int row = 0;
        int col = 0;

        switch (type)
        {
            case jni_bool:
            {
                int val = getHandleBool(dataset, name);
                setGraphicObjectProperty(uid, go, &val, jni_bool, 1);
                break;
            }
            case jni_int:
            {
                int val = getHandleInt(dataset, name);
                setGraphicObjectProperty(uid, go, &val, jni_int, 1);
                break;
            }
            case jni_double:
            {
                double val = getHandleDouble(dataset, name);
                setGraphicObjectProperty(uid, go, &val, jni_double, 1);
                break;
            }
            case jni_bool_vector:
            {
                int* vals = nullptr;
                getHandleBoolVector(dataset, name, &row, &col, &vals);
                setGraphicObjectProperty(uid, go, vals, jni_bool_vector, row * col);
                delete[] vals;
                break;
            }
            case jni_int_vector:
            {
                int* vals = nullptr;
                getHandleIntVector(dataset, name, &row, &col, &vals);
                setGraphicObjectProperty(uid, go, vals, jni_int_vector, row * col);
                delete[] vals;
                break;
            }
            case jni_double_vector:
            {
                double* vals = nullptr;
                getHandleDoubleVector(dataset, name, &row, &col, &vals);
                setGraphicObjectProperty(uid, go, vals, jni_double_vector, row * col);
                delete[] vals;
                break;
            }
            case jni_string_vector:
            {
                char** vals = nullptr;
                int node = getHandleStringVector(dataset, name, &row, &col, &vals);
                setGraphicObjectProperty(uid, go, vals, jni_string_vector, row * col);
                freeStringMatrix(node, vals);
                delete[] vals;
                break;
            }
        }
    }

    //userdata
    import_userdata(dataset, uid);

    closeList6(children);
    return uid;
}

static int import_surface_handle(int dataset, int uid)
{
    //import "standards" properties
    import_handle_generic(dataset, uid, SurfaceHandle::getPropertyList(), true);
    return uid;
}

static int import_plot3d_handle(int dataset)
{
    int plot = createGraphicObject(__GO_PLOT3D__);
    createDataObject(plot, __GO_PLOT3D__);
    import_surface_handle(dataset, plot);

    //data
    int xR = 0, xC = 0;
    double* dataX = nullptr;
    int yR = 0, yC = 0;
    double* dataY = nullptr;
    int zR = 0, zC = 0;
    double* dataZ = nullptr;

    getHandleDoubleVector(dataset, "data_x", &xR, &xC, &dataX);
    getHandleDoubleVector(dataset, "data_y", &yR, &yC, &dataY);
    getHandleDoubleVector(dataset, "data_z", &zR, &zC, &dataZ);

    int gridSize[4];
    int result;

    gridSize[0] = xR;
    gridSize[1] = xC;
    gridSize[2] = yR;
    gridSize[3] = yC;

    result = setGraphicObjectPropertyAndNoWarn(plot, __GO_DATA_MODEL_GRID_SIZE__, gridSize, jni_int_vector, 4);

    setGraphicObjectPropertyAndNoWarn(plot, __GO_DATA_MODEL_X__, dataX, jni_double_vector, xR * xC);
    setGraphicObjectPropertyAndNoWarn(plot, __GO_DATA_MODEL_Y__, dataY, jni_double_vector, yR * yC);
    setGraphicObjectProperty(plot, __GO_DATA_MODEL_Z__, dataZ, jni_double_vector, zR * zC);

    delete[] dataX;
    delete[] dataY;
    delete[] dataZ;


    closeList6(dataset);
    return plot;
}

static int import_fac3d_handle(int dataset)
{
    int fac = createGraphicObject(__GO_FAC3D__);
    createDataObject(fac, __GO_FAC3D__);

    import_surface_handle(dataset, fac);

    //data
    int xR = 0, xC = 0;
    double* dataX = nullptr;
    int yR = 0, yC = 0;
    double* dataY = nullptr;
    int zR = 0, zC = 0;
    double* dataZ = nullptr;

    getHandleDoubleVector(dataset, "data_x", &xR, &xC, &dataX);
    getHandleDoubleVector(dataset, "data_y", &yR, &yC, &dataY);
    getHandleDoubleVector(dataset, "data_z", &zR, &zC, &dataZ);

    int cR = 0;
    int cC = 0;
    double* colors = nullptr;
    getHandleDoubleVector(dataset, "colors", &cR, &cC, &colors);
    int cSize = cR * cC;

    int numElementsArray[3];

    numElementsArray[0] = xC;
    numElementsArray[1] = xR;
    numElementsArray[2] = cSize;

    setGraphicObjectPropertyAndNoWarn(fac, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, numElementsArray, jni_int_vector, 3);

    setGraphicObjectPropertyAndNoWarn(fac, __GO_DATA_MODEL_X__, dataX, jni_double_vector, xR * xC);
    setGraphicObjectPropertyAndNoWarn(fac, __GO_DATA_MODEL_Y__, dataY, jni_double_vector, yR * yC);
    setGraphicObjectPropertyAndNoWarn(fac, __GO_DATA_MODEL_Z__, dataZ, jni_double_vector, zR * zC);
    setGraphicObjectProperty(fac, __GO_DATA_MODEL_COLORS__, colors, jni_double_vector, cSize);

    delete[] dataX;
    delete[] dataY;
    delete[] dataZ;
    delete[] colors;


    //cdata_mapping
    int cdata = getHandleInt(dataset, "cdata_mapping");
    setGraphicObjectProperty(fac, __GO_DATA_MAPPING__, &cdata, jni_int, 1);


    closeList6(dataset);
    return fac;
}


static int import_champ_handle(int dataset)
{
    //need to get properties and call a "creator" :x

    int champ = createGraphicObject(__GO_CHAMP__);

    //data
    int row = 0;
    int col = 0;
    int dims[2];
    int num = 0;
    double* baseX = nullptr;
    getHandleDoubleVector(dataset, "base_x", &row, &col, &baseX);
    dims[0] = col;

    double* baseY = nullptr;
    getHandleDoubleVector(dataset, "base_y", &row, &col, &baseY);
    dims[1] = col;
    num = dims[0] * dims[1];

    double* directionX = nullptr;
    getHandleDoubleVector(dataset, "direction_x", &row, &col, &directionX);

    double* directionY = nullptr;
    getHandleDoubleVector(dataset, "direction_y", &row, &col, &directionY);

    setGraphicObjectProperty(champ, __GO_NUMBER_ARROWS__, &num, jni_int, 1);
    setGraphicObjectProperty(champ, __GO_CHAMP_DIMENSIONS__, dims, jni_int_vector, 2);
    setGraphicObjectProperty(champ, __GO_BASE_X__, baseX, jni_double_vector, dims[0]);
    setGraphicObjectProperty(champ, __GO_BASE_Y__, baseY, jni_double_vector, dims[1]);
    setGraphicObjectProperty(champ, __GO_DIRECTION_X__, directionX, jni_double_vector, dims[0] * dims[1]);
    setGraphicObjectProperty(champ, __GO_DIRECTION_Y__, directionY, jni_double_vector, dims[0] * dims[1]);

    delete[] baseX;
    delete[] baseY;
    delete[] directionX;
    delete[] directionY;

    //import "standards" properties
    import_handle_generic(dataset, champ, ChampHandle::getPropertyList(), true);

    closeList6(dataset);
    return champ;
}
static int import_label_handle(int dataset, int uid)
{
    //import "standards" properties
    import_handle_generic(dataset, uid, LabelHandle::getPropertyList());

    //text
    std::vector<int> dims(2);
    char** data = nullptr;
    int node = getHandleStringVector(dataset, "text", &dims[0], &dims[1], &data);

    setGraphicObjectProperty(uid, __GO_TEXT_ARRAY_DIMENSIONS__, dims.data(), jni_int_vector, 2);
    setGraphicObjectProperty(uid, __GO_TEXT_STRINGS__, data, jni_string_vector, dims[0] * dims[1]);
    freeStringMatrix(node, data);

    closeList6(dataset);
    return uid;
}

static int import_axes_handle(int dataset)
{
    //how to manage call by %h_copy ?

    int axes = getOrCreateDefaultSubwin();

    //hide current axes
    int visible = 0;
    setGraphicObjectProperty(axes, __GO_VISIBLE__, &visible, jni_bool, 1);

    //import "standards" properties
    import_handle_generic(dataset, axes, AxesHandle::getPropertyList());

    //title
    int title = 0;
    int *ptitle = &title;
    int nodeTitle = getDataSetIdFromName(dataset, "title");
    getGraphicObjectProperty(axes, __GO_TITLE__, jni_int, (void **)&ptitle);
    import_label_handle(nodeTitle, title);

    //x_label
    int x_label = 0;
    int *px_label = &x_label;
    int nodeX = getDataSetIdFromName(dataset, "x_label");
    getGraphicObjectProperty(axes, __GO_X_AXIS_LABEL__, jni_int, (void **)&px_label);
    import_label_handle(nodeX, x_label);

    //y_label
    int y_label = 0;
    int *py_label = &y_label;
    int nodeY = getDataSetIdFromName(dataset, "y_label");
    getGraphicObjectProperty(axes, __GO_Y_AXIS_LABEL__, jni_int, (void **)&py_label);
    import_label_handle(nodeY, y_label);

    //z_label
    int z_label = 0;
    int *pz_label = &z_label;
    int nodeZ = getDataSetIdFromName(dataset, "z_label");
    getGraphicObjectProperty(axes, __GO_Z_AXIS_LABEL__, jni_int, (void **)&pz_label);
    import_label_handle(nodeZ, z_label);

    //set real visible state
    visible = getHandleInt(dataset, "visible");
    setGraphicObjectProperty(axes, __GO_VISIBLE__, &visible, jni_bool, 1);

    closeList6(dataset);
    return axes;
}

static int import_figure_handle(int dataset)
{
    //some properties must be set @ creation time
    int menubar = getHandleInt(dataset, "menubar");
    int toolbar = getHandleInt(dataset, "toolbar");
    int dockable = getHandleBool(dataset, "dockable");
    int default_axes = getHandleBool(dataset, "default_axes");

    //force visible true FOR DEBUG ONLY
    int visible = 0;

    //create a new hidden figure
    int fig = createFigure(dockable, menubar, toolbar, default_axes, visible);
    int id = getValidDefaultFigureId();
    setGraphicObjectProperty(fig, __GO_ID__, &id, jni_int, 1);

    int menu = getHandleBool(dataset, "menubar_visible");
    int notmenu = !menu;
    int info = getHandleBool(dataset, "infobar_visible");
    int notinfo = !info;
    int tool = getHandleBool(dataset, "toolbar_visible");
    int nottool = !tool;

    //force inverse flag
    setGraphicObjectProperty(fig, __GO_MENUBAR_VISIBLE__, &notmenu, jni_bool, 1);
    setGraphicObjectProperty(fig, __GO_INFOBAR_VISIBLE__, &notinfo, jni_bool, 1);
    setGraphicObjectProperty(fig, __GO_TOOLBAR_VISIBLE__, &nottool, jni_bool, 1);

    //set real value
    setGraphicObjectProperty(fig, __GO_MENUBAR_VISIBLE__, &menu, jni_bool, 1);
    setGraphicObjectProperty(fig, __GO_INFOBAR_VISIBLE__, &info, jni_bool, 1);
    setGraphicObjectProperty(fig, __GO_TOOLBAR_VISIBLE__, &tool, jni_bool, 1);

    //import "standards" properties
    import_handle_generic(dataset, fig, FigureHandle::getPropertyList());

    closeList6(dataset);
    return fig;
}

static int import_handle(int dataset)
{
    //get type
    int node = getDataSetIdFromName(dataset, "type");
    if (node < 0)
    {
        return -1;
    }

    int type = 0;
    readInteger32Matrix(node, &type);

    switch (type)
    {
        case __GO_FIGURE__:
        {
            return import_figure_handle(dataset);
        }
        case __GO_AXES__:
        {
            return import_axes_handle(dataset);
        }
        case __GO_CHAMP__:
        {
            return import_champ_handle(dataset);
        }
        case __GO_FAC3D__:
        {
            return import_fac3d_handle(dataset);
        }
        case __GO_PLOT3D__:
        {
            return import_plot3d_handle(dataset);
        }
    }
    return -1;
}

static types::InternalType* import_handles(int dataset)
{
    //get sparse dimensions
    int complex = 0;
    std::vector<int> pdims;
    int size = getDimsNode(dataset, &complex, pdims);

    if (size == 0)
    {
        closeList6(dataset);
        return nullptr;
    }

    //open __refs__ node
    int refs = getDataSetIdFromName(dataset, "__refs__");
    types::GraphicHandle* handles = new types::GraphicHandle(static_cast<int>(pdims.size()), pdims.data());
    long long* h = handles->get();
    for (int i = 0; i < size; ++i)
    {
        int ref = getDataSetIdFromName(refs, std::to_string(i).data());
        int val = import_handle(ref);
        if (val < 0)
        {
            return nullptr;
        }

        h[i] = getHandle(val);
    }

    closeList6(refs);
    closeList6(dataset);

    return handles;

}