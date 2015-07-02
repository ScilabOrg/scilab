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

std::unordered_map<int, Links::PathList> Links::paths;

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
#include "HandleManagement.h"

}
/*--------------------------------------------------------------------------*/
static bool import_variable(int file, std::string& name);
types::InternalType* import_data(int dataset);
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
            std::vector<char*> vars(iNbItem);
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

types::InternalType* import_data(int dataset)
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

    std::vector<int> d(dims);
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


    std::vector<int> d(dims);
    int size = getDatasetInfo(dataset, &complex, &dims, d.data());


    if (dims == 0 || size == 0)
    {
        closeDataSet(dataset);
        return types::Double::Empty();
    }

    std::vector<char*> s(size);
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

    std::vector<int> d(dims);
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

    std::vector<int> d(dims);
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
    std::vector<int> d(dim);
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
            std::vector<int> refdims(refdim);
            int refcount = getDatasetInfo(dataref, &complex, &refdim, refdims.data());
            std::vector<hobj_ref_t> vrefs(refcount);
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
    std::vector<int> dimsin(dimin);
    sizein = getDatasetInfo(datain, &complex, &dimin, dimsin.data());

    std::vector<int> in(sizein);
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
    std::vector<int> dimsout(dimout);
    sizeout = getDatasetInfo(dataout, &complex, &dimout, dimsout.data());

    std::vector<int> out(sizeout);
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
    std::vector<int> dimsdata(dimdata);
    sizedata = getDatasetInfo(ddata, &complex, &dimdata, dimsdata.data());

    std::vector<double> real(sizedata);

    if (complex)
    {
        std::vector<double> img(sizedata);
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
    std::vector<types::InternalType*> data(size);
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
        int val = import_handle(ref, -1);
        if (val < 0)
        {
            return nullptr;
        }

        h[i] = getHandle(val);
    }

    closeList6(refs);
    closeList6(dataset);


    //update links property of legend handle
    if (Links::count())
    {
        std::list<int> legends = Links::legends();
        for (auto& i : legends)
        {
            Links::PathList paths = Links::get(i);
            update_link_path(i, paths);
        }
    }
    return handles;

}