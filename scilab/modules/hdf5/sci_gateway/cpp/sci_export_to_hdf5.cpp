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
#include "polynom.hxx"
#include "sparse.hxx"
#include "graphichandle.hxx"
#include "handle_properties.hxx"
#include "context.hxx"

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
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "HandleManagement.h"
}

/*--------------------------------------------------------------------------*/
static bool isVarExist(int _iFile, const char* _pstVarName);
static int extractVarNameList(int* pvCtx, int _iStart, int _iEnd, char** _pstNameList);

static int export_data(int parent, const std::string& name, types::InternalType* data);
static int export_double(int parent, const std::string& name, types::Double* data);
static int export_string(int parent, const std::string& name, types::String* data);
static int export_boolean(int parent, const std::string& name, types::Bool* data);
static int export_list(int parent, const std::string& name, types::List* data);
static int export_struct(int parent, const std::string& name, types::Struct* data);
template <class T> static int export_int(int parent, const std::string& name, int type, const char* prec, T* data);
static int export_poly(int parent, const std::string& name, types::Polynom* data);
static int export_sparse(int parent, const std::string& name, types::Sparse* data);
static int export_cell(int parent, const std::string& name, types::Cell* data);

static int export_boolean_sparse(int parent, const std::string& name, types::SparseBool* data);
static int export_handles(int parent, const std::string& name, types::GraphicHandle* data);
static int export_void(int parent, const std::string& name);
static int export_undefined(int parent, const std::string& name);


/*--------------------------------------------------------------------------*/
static const std::string fname("export_to_hdf5");
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_export_to_hdf5(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iH5File = 0;
    bool bAppendMode = false;
    int rhs = static_cast<int>(in.size());
    std::string filename;
    std::map<std::string, types::InternalType*> vars;
    symbol::Context* ctx = symbol::Context::getInstance();

    /* Check the number of input argument */
    if (in.size() < 2)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): at least %d expected.\n"), fname.data(), 2);
        return types::Function::Error;
    }

    if (in[0]->getId() != types::InternalType::IdScalarString)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    wchar_t* wfilename = expandPathVariableW(in[0]->getAs<types::String>()->get()[0]);
    char* cfilename = wide_string_to_UTF8(wfilename);
    filename = cfilename;
    FREE(wfilename);
    FREE(cfilename);

    for (int i = 1; i < rhs; ++i)
    {
        if (in[i]->getId() != types::InternalType::IdScalarString)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), fname.data(), 1);
            return types::Function::Error;
        }

        wchar_t* wvar = in[i]->getAs<types::String>()->get()[0];
        if (wcscmp(wvar, L"-append") == 0)
        {
            bAppendMode = true;
            continue;
        }

        types::InternalType* pIT = ctx->get(symbol::Symbol(wvar));
        if (pIT == NULL)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Defined variable expected.\n"), fname, i + 1);
            return types::Function::Error;
        }

        char* cvar = wide_string_to_UTF8(wvar);
        std::string var(cvar);
        FREE(cvar);

        //check var exists
        vars[var] = pIT;
    }

    //check append option
    if (bAppendMode)
    {
        // open hdf5 file
        iH5File = openHDF5File(filename.data(), bAppendMode);
        if (iH5File < 0)
        {
            iH5File = createHDF5File(filename.data());
        }
    }
    else
    {
        iH5File = createHDF5File(filename.data());
    }


    if (iH5File < 0)
    {
        if (iH5File == -2)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: \"%s\" is a directory"), fname.data(), 1, filename.data());
        }
        else
        {
            Scierror(999, _("%s: Cannot open file %s.\n"), fname.data() , filename.data());
        }

        return types::Function::Error;
    }

    if (bAppendMode)
    {
        int iVersion = getSODFormatAttribute(iH5File);
        if (iVersion != SOD_FILE_VERSION)
        {
            //to update version must be the same
            closeHDF5File(iH5File);
            Scierror(999, _("%s: Wrong SOD file format version. Expected: %d Found: %d\n"), fname.data(), SOD_FILE_VERSION, iVersion);
            return types::Function::Error;
        }
    }

    // export data
    for (const auto var : vars)
    {
        if (isVarExist(iH5File, var.first.data()))
        {
            if (bAppendMode)
            {
                if (deleteHDF5Var(iH5File, var.first.data()))
                {
                    closeHDF5File(iH5File);
                    Scierror(999, _("%s: Unable to delete existing variable \"%s\"."), fname.data(), var.first.data());
                    return types::Function::Error;
                }
            }
            else
            {
                closeHDF5File(iH5File);
                Scierror(999, _("%s: Variable \'%s\' already exists in file \'%s\'\nUse -append option to replace existing variable\n."), fname.data(), var.first.data(), filename.data());
                return types::Function::Error;
            }
        }

        int iDataset = export_data(iH5File, var.first, var.second);
        if (iDataset == -1)
        {
            closeHDF5File(iH5File);
            deleteafile(filename.data());
            Scierror(999, _("%s: Variable \'%s\' already exists in file \'%s\'\nUse -append option to replace existing variable\n."), fname.data(), var.first.data(), filename.data());
            return types::Function::Error;
        }
    }

    //add or update scilab version and file version in hdf5 file
    if (updateScilabVersion(iH5File) < 0)
    {
        closeHDF5File(iH5File);
        Scierror(999, _("%s: Unable to update Scilab version in \"%s\"."), fname.data(), filename.data());
        return types::Function::Error;
    }

    if (updateFileVersion(iH5File) < 0)
    {
        closeHDF5File(iH5File);
        Scierror(999, _("%s: Unable to update HDF5 format version in \"%s\"."), fname.data(), filename.data());
        return types::Function::Error;
    }

    //close hdf5 file
    closeHDF5File(iH5File);

    out.push_back(new types::Bool(1));
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
static bool isVarExist(int _iFile, const char* _pstVarName)
{
    //check if variable already exists
    int iNbItem = getVariableNames(_iFile, NULL);
    if (iNbItem)
    {
        char **pstVarNameList = (char **)MALLOC(sizeof(char *) * iNbItem);

        iNbItem = getVariableNames(_iFile, pstVarNameList);

        //import all data
        for (int i = 0; i < iNbItem; i++)
        {
            if (strcmp(pstVarNameList[i], _pstVarName) == 0)
            {
                freeArrayOfString(pstVarNameList, iNbItem);
                return true;
            }
        }

        freeArrayOfString(pstVarNameList, iNbItem);
    }

    return false;
}
/*--------------------------------------------------------------------------*/
static int export_data(int parent, const std::string& name, types::InternalType* data)
{
    int dataset = -1;
    switch (data->getType())
    {
        case types::InternalType::ScilabDouble:
            dataset = export_double(parent, name, data->getAs<types::Double>());
            break;
        case types::InternalType::ScilabString:
            dataset = export_string(parent, name, data->getAs<types::String>());
            break;
        case types::InternalType::ScilabBool:
            dataset = export_boolean(parent, name, data->getAs<types::Bool>());
            break;
        case types::InternalType::ScilabTList:
        case types::InternalType::ScilabList:
        case types::InternalType::ScilabMList:
            dataset = export_list(parent, name, data->getAs<types::List>());
            break;
        case types::InternalType::ScilabInt8:
            dataset = export_int(parent, name, H5T_NATIVE_INT8, "8", data->getAs<types::Int8>());
            break;
        case types::InternalType::ScilabInt16:
            dataset = export_int(parent, name, H5T_NATIVE_INT16, "16", data->getAs<types::Int16>());
            break;
        case types::InternalType::ScilabInt32:
            dataset = export_int(parent, name, H5T_NATIVE_INT32, "32", data->getAs<types::Int32>());
            break;
        case types::InternalType::ScilabInt64:
            dataset = export_int(parent, name, H5T_NATIVE_INT64, "64", data->getAs<types::Int64>());
            break;
        case types::InternalType::ScilabUInt8:
            dataset = export_int(parent, name, H5T_NATIVE_UINT8, "u8", data->getAs<types::UInt8>());
            break;
        case types::InternalType::ScilabUInt16:
            dataset = export_int(parent, name, H5T_NATIVE_UINT16, "u16", data->getAs<types::UInt16>());
            break;
        case types::InternalType::ScilabUInt32:
            dataset = export_int(parent, name, H5T_NATIVE_UINT32, "u32", data->getAs<types::UInt32>());
            break;
        case types::InternalType::ScilabUInt64:
            dataset = export_int(parent, name, H5T_NATIVE_UINT64, "u64", data->getAs<types::UInt64>());
            break;
        case types::InternalType::ScilabStruct:
            dataset = export_struct(parent, name, data->getAs<types::Struct>());
            break;
        case types::InternalType::ScilabPolynom:
            dataset = export_poly(parent, name, data->getAs<types::Polynom>());
            break;
        case types::InternalType::ScilabSparse:
            dataset = export_sparse(parent, name, data->getAs<types::Sparse>());
            break;
        case types::InternalType::ScilabCell:
            dataset = export_cell(parent, name, data->getAs<types::Cell>());
            break;
        case types::InternalType::ScilabVoid:
            dataset = export_void(parent, name);
            break;
        case types::InternalType::ScilabListUndefinedOperation:
            dataset = export_undefined(parent, name);
            break;
        case types::InternalType::ScilabHandle:
            dataset = export_handles(parent, name, data->getAs<types::GraphicHandle>());
            break;
        default:
        {
            break;
        }
    }

    return dataset;
}

/*--------------------------------------------------------------------------*/
static int export_list(int parent, const std::string& name, types::List* data)
{
    int size = data->getSize();

    const char* type = nullptr;
    switch (data->getType())
    {
        case types::InternalType::ScilabMList:
            type = g_SCILAB_CLASS_MLIST;
            break;
        case types::InternalType::ScilabTList:
            type = g_SCILAB_CLASS_TLIST;
            break;
        case types::InternalType::ScilabList:
            type = g_SCILAB_CLASS_LIST;
            break;
        default:
            return -1;
    }

    //create node with list name
    int dset = openList6(parent, name.data(), type);

    for (int i = 0; i < size; ++i)
    {
        if (export_data(dset, std::to_string(i).data(), data->get(i)) == -1)
        {
            closeList6(dset);
            return -1;
        }
    }

    if (closeList6(dset) == -1)
    {
        return -1;
    }
    return dset;
}
/*--------------------------------------------------------------------------*/
static int export_double(int parent, const std::string& name, types::Double* data)
{
    int dataset = -1;

    if (data->isComplex())
    {
        dataset = writeDoubleComplexMatrix6(parent, name.data(), data->getDims(), data->getDimsArray(), data->get(), data->getImg());
    }
    else
    {
        dataset = writeDoubleMatrix6(parent, name.data(), data->getDims(), data->getDimsArray(), data->get());
    }

    return dataset;
}
/*--------------------------------------------------------------------------*/
template <class T>
static int export_int(int parent, const std::string& name, int type, const char* prec, T* data)
{
    return writeIntegerMatrix6(parent, name.data(), type, prec, data->getDims(), data->getDimsArray(), data->get());
}
/*--------------------------------------------------------------------------*/
static int export_string(int parent, const std::string& name, types::String* data)
{
    int size = data->getSize();
    wchar_t** s = data->get();
    std::vector<char*> v(size, nullptr);

    //convert UTF16 strings to UTF8
    for (int i = 0; i < size; ++i)
    {
        v[i] = wide_string_to_UTF8(s[i]);
    }

    int dset = writeStringMatrix6(parent, name.data(), data->getDims(), data->getDimsArray(), v.data());

    //release memory
    for (int i = 0; i < size; ++i)
    {
        FREE(v[i]);
    }

    return dset;
}
/*--------------------------------------------------------------------------*/
static int export_boolean(int parent, const std::string& name, types::Bool* data)
{
    return writeBooleanMatrix6(parent, name.data(), data->getDims(), data->getDimsArray(), data->get());
}
/*--------------------------------------------------------------------------*/
static int export_struct(int parent, const std::string& name, types::Struct* data)
{
    //create a group with struct name
    int dset = openList6(parent, name.data(), g_SCILAB_CLASS_STRUCT);
    //store struct dimensions
    std::vector<int> dims = {1, data->getDims()};
    int ret = writeIntegerMatrix6(dset, "__dims__", H5T_NATIVE_INT32, "32", 2, dims.data(), data->getDimsArray());
    if (ret < 0)
    {
        return -1;
    }

    int size = data->getSize();

    if (size == 0)
    {
        if (closeList6(dset) == -1)
        {
            return -1;
        }

        return dset;
    }

    //create a node for fields references
    int refs = openList6(dset, "__refs__", g_SCILAB_CLASS_STRUCT);
    if (refs < 0)
    {
        return -1;
    }

    types::String* fields = data->getFieldNames();
    int fieldCount = fields->getSize();
    wchar_t** pfields = fields->get();


    std::vector<hobj_ref_t> vrefs(size, 0);
    //fill main group with struct field name
    for (int i = 0; i < fieldCount; ++i)
    {
        char* cfield = wide_string_to_UTF8(pfields[i]);
        for (int j = 0; j < size; ++j)
        {
            //get data
            types::InternalType* val = data->get(j)->get(pfields[i]);
            //create ref name
            std::string refname(cfield);
            refname += "_" + std::to_string(j);
            //export data in refs group
            int ref = export_data(refs, refname, val);
            //create reference of data
            ret = addItemStruct6(refs, vrefs.data(), j, refname.data());
            if (ret)
            {
                return -1;
            }
        }

        ret = writeStructField6(dset, cfield, data->getDims(), data->getDimsArray(), vrefs.data());
        FREE(cfield);
        if (ret < 0)
        {
            return -1;
        }
    }

    if (closeList6(refs) == -1)
    {
        return -1;
    }

    if (closeList6(dset) == -1)
    {
        return -1;
    }

    return dset;
}
/*--------------------------------------------------------------------------*/
static int export_void(int parent, const std::string& name)
{
    return writeVoid6(parent, name.data());
}
/*--------------------------------------------------------------------------*/
static int export_undefined(int parent, const std::string& name)
{
    return writeUndefined6(parent, name.data());
}
/*--------------------------------------------------------------------------*/
static int export_poly(int parent, const std::string& name, types::Polynom* data)
{
    //create a group with struct name
    int dset = openList6(parent, name.data(), g_SCILAB_CLASS_POLY);
    //store struct dimensions
    std::vector<int> dims = {1, data->getDims()};
    int ret = writeIntegerMatrix6(dset, "__dims__", H5T_NATIVE_INT32, "32", 2, dims.data(), data->getDimsArray());
    if (ret < 0)
    {
        return -1;
    }

    //store variable name
    std::vector<int> vardims = {1, 1};
    char* varname = wide_string_to_UTF8(data->getVariableName().data());
    ret = writeStringMatrix6(dset, "__varname__", 2, vardims.data(), &varname);
    FREE(varname);
    if (ret < 0)
    {
        return -1;
    }

    //create a node for fields references
    int refs = openList6(dset, "__refs__", g_SCILAB_CLASS_POLY);
    if (refs < 0)
    {
        return -1;
    }

    bool complex = data->isComplex();
    int size = data->getSize();
    std::vector<hobj_ref_t> vrefs(size, 0);
    types::SinglePoly** ss = data->get();
    //fill main group with struct field name
    for (int j = 0; j < size; ++j)
    {
        //get data
        types::SinglePoly* val = ss[j];
        //export data in refs group
        std::vector<int> ssdims = {1, val->getSize()};
        std::string polyname(std::to_string(j));
        if (complex)
        {
            writeDoubleComplexMatrix6(refs, polyname.data(), 2, ssdims.data(), val->get(), val->getImg());
        }
        else
        {
            writeDoubleMatrix6(refs, polyname.data(), 2, ssdims.data(), val->get());
        }

        //create reference of data
        ret = addItemStruct6(refs, vrefs.data(), j, polyname.data());
        if (ret)
        {
            return -1;
        }
    }


    if (closeList6(refs) == -1)
    {
        return -1;
    }

    if (closeList6(dset) == -1)
    {
        return -1;
    }

    return dset;
}
/*--------------------------------------------------------------------------*/
static int export_sparse(int parent, const std::string& name, types::Sparse* data)
{
    int nnz = static_cast<int>(data->nonZeros());
    int row = data->getRows();
    //create a group with sparse name
    int dset = openList6(parent, name.data(), g_SCILAB_CLASS_SPARSE);
    //store sparse dimensions
    std::vector<int> dims = {1, data->getDims()};
    int ret = writeIntegerMatrix6(dset, "__dims__", H5T_NATIVE_INT32, "32", 2, dims.data(), data->getDimsArray());
    if (ret < 0)
    {
        return -1;
    }

    //store numbers of non zero by rows.
    std::vector<int> dimsnnz = {1, 1};
    ret = writeIntegerMatrix6(dset, "__nnz__", H5T_NATIVE_INT32, "32", 2, dimsnnz.data(), &nnz);
    if (ret < 0)
    {
        return -1;
    }

    //store inner vector
    int innercount = 0;
    int* inner = data->getInnerPtr(&innercount);
    std::vector<int> dimsinner = {1, nnz};
    ret = writeIntegerMatrix6(dset, "__inner__", H5T_NATIVE_INT32, "32", 2, dimsinner.data(), inner);
    if (ret < 0)
    {
        return -1;
    }

    int outercount = 0;
    int* outer = data->getOuterPtr(&outercount);
    std::vector<int> dimsouter = {1, outercount + 1};
    ret = writeIntegerMatrix6(dset, "__outer__", H5T_NATIVE_INT32, "32", 2, dimsouter.data(), outer);
    if (ret < 0)
    {
        return -1;
    }

    if (data->isComplex())
    {
        double* real = new double[nnz];
        double* img = new double[nnz];
        std::complex<double>* d = data->getImg();
        for (int i = 0; i < nnz; ++i)
        {
            real[i] = d[i].real();
            img[i] = d[i].imag();
        }


        std::vector<int> dimsdata = {1, nnz};
        ret = writeDoubleComplexMatrix6(dset, "__data__", 2, dimsdata.data(), real, img);
        delete[] real;
        delete[] img;
        if (ret < 0)
        {
            return -1;
        }
    }
    else
    {
        std::vector<int> dimsdata = {1, nnz};
        ret = writeDoubleMatrix6(dset, "__data__", 2, dimsdata.data(), data->get());
        if (ret < 0)
        {
            return -1;
        }
    }

    if (closeList6(dset) == -1)
    {
        return -1;
    }

    return dset;

}
/*--------------------------------------------------------------------------*/
static int export_cell(int parent, const std::string& name, types::Cell* data)
{
    //create a group with cell name
    int dset = openList6(parent, name.data(), g_SCILAB_CLASS_CELL);
    //store cell dimensions
    std::vector<int> dims = {1, data->getDims()};
    int ret = writeIntegerMatrix6(dset, "__dims__", H5T_NATIVE_INT32, "32", 2, dims.data(), data->getDimsArray());
    if (ret < 0)
    {
        return -1;
    }

    //create a node for fields references
    int refs = openList6(dset, "__refs__", g_SCILAB_CLASS_CELL);
    if (refs < 0)
    {
        return -1;
    }

    int size = data->getSize();
    types::InternalType** it = data->get();
    std::vector<hobj_ref_t> vrefs(size, 0);
    for (int i = 0; i < size; ++i)
    {
        std::string refname(std::to_string(i));
        int ref = export_data(refs, refname, it[i]);
    }


    if (closeList6(refs) == -1)
    {
        return -1;
    }

    if (closeList6(dset) == -1)
    {
        return -1;
    }

    return dset;
}

static int getHandleBoolProperty(int uid, int prop)
{
    int val;
    int* pVal = &val;
    getGraphicObjectProperty(uid, prop, jni_bool, (void **)&pVal);
    return val;
}

static int getHandleIntProperty(int uid, int prop)
{
    int val;
    int* pVal = &val;
    getGraphicObjectProperty(uid, prop, jni_int, (void **)&pVal);
    return val;
}

static double getHandleDoubleProperty(int uid, int prop)
{
    double val;
    double* pVal = &val;
    getGraphicObjectProperty(uid, prop, jni_double, (void **)&pVal);
    return val;
}

static void getHandleStringProperty(int uid, int prop, char** str)
{
    getGraphicObjectProperty(uid, prop, jni_string, (void **)str);
}


static void getHandleBoolVectorProperty(int uid, int prop, int** vals)
{
    getGraphicObjectProperty(uid, prop, jni_bool_vector, (void **)vals);
}

static void getHandleIntVectorProperty(int uid, int prop, int** vals)
{
    getGraphicObjectProperty(uid, prop, jni_int_vector, (void **)vals);
}

static void getHandleDoubleVectorProperty(int uid, int prop, double** vals)
{
    getGraphicObjectProperty(uid, prop, jni_double_vector, (void **)vals);
}

static void getHandleStringVectorProperty(int uid, int prop, char*** vals)
{
    getGraphicObjectProperty(uid, prop, jni_string_vector, (void **)vals);
}

static bool export_handle_generic(int parent, int uid, HandleProp& props);
static bool export_handle_layout_options(int parent, int uid);
static bool export_userdata(int parent, int uid);
static bool export_handle_figure(int parent, int uid);
static bool export_handle_axes(int parent, int uid);
static bool export_handle_label(int parent, int uid);
static bool export_handle_champ(int parent, int uid);
static bool export_handle(int parent, const std::string& name, int uid);
static bool export_handle_children(int parent, int uid);

static bool export_handle_generic(int parent, int uid, HandleProp& props)
{
    for (auto& prop : props)
    {
        const char* name = prop.first.data();
        std::vector<int> info(prop.second);

        //if (prop.first == "rotation_angles")
        //{
        //    std::cout << "prop : " << name << std::endl;
        //}
        //scalar variable
        if (info.size() == 3)
        {
            int go = info[1];
            int type = info[2];

            switch (type)
            {
                case jni_bool:
                {
                    std::vector<int> dims = {1, 1};
                    int val = getHandleBoolProperty(uid, go);
                    writeBooleanMatrix6(parent, name, static_cast<int>(dims.size()), dims.data(), &val);
                    break;
                }
                case jni_double:
                {
                    std::vector<int> dims = {1, 1};
                    double val = getHandleDoubleProperty(uid, go);
                    writeDoubleMatrix6(parent, name, static_cast<int>(dims.size()), dims.data(), &val);
                    break;
                }
                case jni_int:
                {
                    std::vector<int> dims = {1, 1};
                    int val = getHandleIntProperty(uid, go);
                    writeIntegerMatrix6(parent, name, H5T_NATIVE_INT32, "32", static_cast<int>(dims.size()), dims.data(), &val);
                    break;
                }
                case jni_string:
                {
                    std::vector<int> dims = {1, 1};
                    char* val;
                    getHandleStringProperty(uid, go, &val);
                    writeStringMatrix6(parent, name, static_cast<int>(dims.size()), dims.data(), &val);
                    releaseGraphicObjectProperty(go, val, jni_string, 1);
                    break;
                }
            }
        }
        else //vector variable
        {
            int go = info[1];
            int type = info[2];
            int row = info[3];
            int col = info.size() > 3 ? info[4] : -1;

            if (row < 0)
            {
                row = -row;
            }
            else
            {
                row = getHandleIntProperty(uid, row);
            }

            if (col < 0)
            {
                col = -col;
            }
            else
            {
                col = getHandleIntProperty(uid, col);
            }


            switch (type)
            {
                case jni_bool_vector:
                {
                    std::vector<int> dims = {row, col};
                    int* vals;
                    getHandleBoolVectorProperty(uid, go, &vals);
                    writeBooleanMatrix6(parent, name, static_cast<int>(dims.size()), dims.data(), vals);
                    releaseGraphicObjectProperty(go, vals, jni_bool_vector, row * col);
                    break;
                }
                case jni_double_vector:
                {
                    std::vector<int> dims = {row, col};
                    double* vals;
                    getHandleDoubleVectorProperty(uid, go, &vals);
                    writeDoubleMatrix6(parent, name, static_cast<int>(dims.size()), dims.data(), vals);
                    releaseGraphicObjectProperty(go, vals, jni_double_vector, row * col);
                    break;
                }
                case jni_int_vector:
                {
                    std::vector<int> dims = {row, col};
                    int* vals;
                    getHandleIntVectorProperty(uid, go, &vals);
                    writeIntegerMatrix6(parent, name, H5T_NATIVE_INT32, "32", static_cast<int>(dims.size()), dims.data(), vals);
                    releaseGraphicObjectProperty(go, vals, jni_int_vector, row * col);
                    break;
                }
                case jni_string_vector:
                {
                    std::vector<int> dims = {row, col};
                    char** vals;
                    getHandleStringVectorProperty(uid, go, &vals);
                    writeStringMatrix6(parent, name, static_cast<int>(dims.size()), dims.data(), vals);
                    releaseGraphicObjectProperty(go, vals, jni_string_vector, row * col);
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }

    //user_data
    export_userdata(parent, uid);
    //children
    export_handle_children(parent, uid);
    return true;
}

static bool export_handle_layout_options(int parent, int uid)
{
    int layout_type = getHandleIntProperty(uid, __GO_LAYOUT__);
    if (layout_type == 0 || layout_type == 1) //LAYOUT_NONE or LAYOUT_GRIDBAG
    {
        //nothing to save
        return true;
    }

    int layout = openList6(parent, "layout_options", g_SCILAB_CLASS_HANDLE);

    switch (layout_type)
    {
        case 2: //grid
        {
            std::vector<int> dims = {1, 2};
            int* grid = nullptr;
            getGraphicObjectProperty(uid, __GO_GRID_OPT_GRID__, jni_int_vector, (void **)&grid);
            writeIntegerMatrix6(layout, "grid", H5T_NATIVE_INT32, "32", 2, dims.data(), grid);

            int* pad = nullptr;
            getGraphicObjectProperty(uid, __GO_GRID_OPT_PADDING__, jni_int_vector, (void **)&pad);
            writeIntegerMatrix6(layout, "padding", H5T_NATIVE_INT32, "32", 2, dims.data(), pad);
            break;
        }
        case 3: //border
        {
            std::vector<int> dims = {1, 2};
            int* pad = nullptr;
            getGraphicObjectProperty(uid, __GO_BORDER_OPT_PADDING__, jni_int_vector, (void **)&pad);
            writeIntegerMatrix6(layout, "padding", H5T_NATIVE_INT32, "32", 2, dims.data(), pad);
            break;
        }
    }

    closeList6(layout);

    return true;
}

static bool export_userdata(int parent, int uid)
{
    int size = 0;
    int *psize = &size;
    int *data = NULL;

    getGraphicObjectProperty(uid, __GO_USER_DATA_SIZE__, jni_int, (void **)&psize);
    getGraphicObjectProperty(uid, __GO_USER_DATA__, jni_int_vector, (void **)&data);

    if (size == 0)
    {
        std::vector<int> dims = {0, 0};
        writeDoubleMatrix6(parent, "userdata", 2, dims.data(), NULL);
    }
    else
    {
        types::InternalType* pUD = nullptr;
        if (size == 1)
        {
            //32 bits
            int* p = (int*)data;
            pUD = ((types::InternalType*) * p);
        }
        else
        {
            //64 bits
            long long* p = (long long*)data;
            pUD = ((types::InternalType*) * p);
        }

        export_data(parent, "userdata", pUD);
    }

    return true;
}

static bool export_handle_datatips(int parent, int uid)
{
    int count = getHandleIntProperty(uid, __GO_DATATIPS_COUNT__);
    int node = openList6(parent, "datatips", g_SCILAB_CLASS_HANDLE);
    int* datatips = nullptr;

    if (count != 0)
    {
        getHandleIntVectorProperty(uid, __GO_DATATIPS__, &datatips);
    }

    for (int i = 0; i < count; ++i)
    {
        if (export_handle(node, std::to_string(i), datatips[i]) == false)
        {
            releaseGraphicObjectProperty(__GO_DATATIPS__, datatips, jni_int_vector, count);
            closeList6(node);
            return false;
        }
    }

    releaseGraphicObjectProperty(__GO_DATATIPS__, datatips, jni_int_vector, count);
    closeList6(node);
    return true;
}

static bool export_handle_segs(int parent, int uid)
{
    if (export_handle_generic(parent, uid, SegsHandle::getPropertyList()) == false)
    {
        return false;
    }

    closeList6(parent);
    return true;
}

static bool export_handle_arc(int parent, int uid)
{
    if (export_handle_generic(parent, uid, ArcHandle::getPropertyList()) == false)
    {
        return false;
    }

    closeList6(parent);
    return true;
}

static bool export_handle_rectangle(int parent, int uid)
{
    if (export_handle_generic(parent, uid, RectangleHandle::getPropertyList()) == false)
    {
        return false;
    }

    closeList6(parent);
    return true;
}

static bool export_handle_datatip(int parent, int uid)
{
    if (export_handle_generic(parent, uid, DatatipHandle::getPropertyList()) == false)
    {
        return false;
    }

    closeList6(parent);
    return true;
}

static bool export_handle_polyline_shift(int parent, int uid, const std::string& name, int go_set, int go_data)
{
    int set = getHandleBoolProperty(uid, go_set);
    if (set)
    {
        int count = getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_ELEMENTS__);
        double* data = nullptr;
        getHandleDoubleVectorProperty(uid, go_data, &data);

        int dims[2];
        dims[0] = 1;
        dims[1] = count;
        writeDoubleMatrix6(parent, name.data(), 2, dims, data);

        releaseGraphicObjectProperty(uid, data, jni_double_vector, count);
    }
    else
    {
        int dims[2];
        dims[0] = 0;
        dims[1] = 0;
        writeDoubleMatrix6(parent, name.data(), 2, dims, NULL);
    }

    return true;
}

static bool export_handle_polyline(int parent, int uid)
{
    if (export_handle_datatips(parent, uid) == false)
    {
        return false;
    }

    if (export_handle_generic(parent, uid, PolylineHandle::getPropertyList()) == false)
    {
        return false;
    }

    //x_shift
    export_handle_polyline_shift(parent, uid, "x_shift", __GO_DATA_MODEL_X_COORDINATES_SHIFT_SET__, __GO_DATA_MODEL_X_COORDINATES_SHIFT__);
    //y_shift
    export_handle_polyline_shift(parent, uid, "y_shift", __GO_DATA_MODEL_Y_COORDINATES_SHIFT_SET__, __GO_DATA_MODEL_Y_COORDINATES_SHIFT__);
    //z_shift
    export_handle_polyline_shift(parent, uid, "z_shift", __GO_DATA_MODEL_Z_COORDINATES_SHIFT_SET__, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__);

    //interp_color_vector
    int set = getHandleBoolProperty(uid, __GO_INTERP_COLOR_VECTOR_SET__);
    if (set)
    {
        int count = getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_ELEMENTS__);
        int* data = nullptr;
        getHandleIntVectorProperty(uid, __GO_INTERP_COLOR_VECTOR__, &data);

        int dims[2];
        dims[0] = 1;
        dims[1] = count;
        writeIntegerMatrix6(parent, "interp_color_vector", H5T_NATIVE_INT32, "32", 2, dims, data);
        releaseGraphicObjectProperty(uid, data, jni_int_vector, count);
    }
    else
    {
        int dims[2];
        dims[0] = 0;
        dims[1] = 0;
        writeIntegerMatrix6(parent, "interp_color_vector", H5T_NATIVE_INT32, "32", 2, dims, NULL);
    }

    //data
    int count = getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_ELEMENTS__);

    int dims[2];
    dims[0] = 1;
    dims[1] = count;

    double* dataX = nullptr;
    double* dataY = nullptr;
    getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_X__, &dataX);
    getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Y__, &dataY);

    writeDoubleMatrix6(parent, "data_x", 2, dims, dataX);
    writeDoubleMatrix6(parent, "data_y", 2, dims, dataY);

    releaseGraphicObjectProperty(__GO_DATA_MODEL_X__, dataX, jni_double_vector, count);
    releaseGraphicObjectProperty(__GO_DATA_MODEL_Y__, dataY, jni_double_vector, count);

    set = getHandleIntProperty(uid, __GO_DATA_MODEL_Z_COORDINATES_SET__);
    if (set)
    {
        double* dataZ = nullptr;
        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Z__, &dataZ);
        writeDoubleMatrix6(parent, "data_z", 2, dims, dataZ);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_Z__, dataZ, jni_double_vector, count);
    }
    else
    {
        //[]
        dims[0] = 0;
        dims[1] = 0;
        writeDoubleMatrix6(parent, "data_z", 2, dims, NULL);
    }

    closeList6(parent);
    return true;
}

static bool export_handle_surface(int parent, int uid)
{
    return export_handle_generic(parent, uid, SurfaceHandle::getPropertyList());
}

static bool export_handle_plot3d(int parent, int uid)
{
    bool ret = export_handle_surface(parent, uid);
    if (ret)
    {
        double* colors = NULL;
        double* dataX = NULL;
        double* dataY = NULL;
        double* dataZ = NULL;

        //data
        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_X__, &dataX);
        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Y__, &dataY);
        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Z__, &dataZ);

        int row = getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_X__);
        int col = getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_Y__);

        int* xDims = nullptr;
        int* yDims = nullptr;
        getHandleIntVectorProperty(uid, __GO_DATA_MODEL_X_DIMENSIONS__, &xDims);
        getHandleIntVectorProperty(uid, __GO_DATA_MODEL_Y_DIMENSIONS__, &yDims);

        int dims[2];
        dims[0] = xDims[0];
        dims[1] = xDims[1];
        writeDoubleMatrix6(parent, "data_x", 2, dims, dataX);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_X__, dataX, jni_double_vector, dims[0] * dims[1]);

        dims[0] = yDims[0];
        dims[1] = yDims[1];
        writeDoubleMatrix6(parent, "data_y", 2, dims, dataY);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_Y__, dataY, jni_double_vector, dims[0] * dims[1]);

        dims[0] = row;
        dims[1] = col;
        writeDoubleMatrix6(parent, "data_z", 2, dims, dataZ);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_Z__, dataZ, jni_double_vector, dims[0] * dims[1]);

        releaseGraphicObjectProperty(__GO_DATA_MODEL_X_DIMENSIONS__, xDims, jni_int_vector, 2);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_Y_DIMENSIONS__, dataZ, jni_int_vector, 2);
    }

    closeList6(parent);
    return ret;
}

static bool export_handle_fac3d(int parent, int uid)
{
    bool ret = export_handle_surface(parent, uid);
    if (ret)
    {
        double* colors = NULL;
        double* dataX = NULL;
        double* dataY = NULL;
        double* dataZ = NULL;

        //data
        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_X__, &dataX);
        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Y__, &dataY);
        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Z__, &dataZ);

        int row = getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__);
        int col = getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_GONS__);

        int dims[2];
        dims[0] = row;
        dims[1] = col;

        writeDoubleMatrix6(parent, "data_x", 2, dims, dataX);
        writeDoubleMatrix6(parent, "data_y", 2, dims, dataY);
        writeDoubleMatrix6(parent, "data_z", 2, dims, dataZ);

        releaseGraphicObjectProperty(__GO_DATA_MODEL_X__, dataX, jni_double_vector, dims[0] * dims[1]);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_Y__, dataY, jni_double_vector, dims[0] * dims[1]);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_Z__, dataZ, jni_double_vector, dims[0] * dims[1]);

        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_COLORS__, &colors);
        if (colors)
        {
            int numColors = getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_COLORS__);
            if (numColors == col)
            {
                dims[0] = 1;
            }
            else
            {
                dims[0] = row;
            }

            dims[1] = col;
        }
        else
        {
            //export []
            dims[0] = 0;
            dims[1] = 0;
        }

        writeDoubleMatrix6(parent, "colors", 2, dims, colors);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_COLORS__, colors, jni_double_vector, dims[0] * dims[1]);

        //cdata_mapping
        int cdata = getHandleIntProperty(uid, __GO_DATA_MAPPING__);
        dims[0] = 1;
        dims[1] = 1;
        writeIntegerMatrix6(parent, "cdata_mapping", H5T_NATIVE_INT32, "32", 2, dims, &cdata);

    }

    closeList6(parent);
    return ret;
}


static bool export_handle_champ(int parent, int uid)
{
    if (export_handle_generic(parent, uid, ChampHandle::getPropertyList()) == false)
    {
        return false;
    }

    //data
    int* dimensions = NULL;
    double* arrowBasesX = NULL;
    double* arrowBasesY = NULL;
    double* arrowDirectionsX = NULL;
    double* arrowDirectionsY = NULL;
    int dims[2];
    getHandleIntVectorProperty(uid, __GO_CHAMP_DIMENSIONS__, &dimensions);

    //base X
    getHandleDoubleVectorProperty(uid, __GO_BASE_X__, &arrowBasesX);
    dims[0] = 1;
    dims[1] = dimensions[0];
    writeDoubleMatrix(parent, "base_x", 2, dims, arrowBasesX);
    releaseGraphicObjectProperty(__GO_BASE_X__, arrowBasesX, jni_double_vector, dims[1]);

    //base y
    getHandleDoubleVectorProperty(uid, __GO_BASE_Y__, &arrowBasesY);
    dims[0] = 1;
    dims[1] = dimensions[1];
    writeDoubleMatrix(parent, "base_y", 2, dims, arrowBasesY);
    releaseGraphicObjectProperty(__GO_BASE_Y__, arrowBasesY, jni_double_vector, dims[1]);

    //direction x
    getHandleDoubleVectorProperty(uid, __GO_DIRECTION_X__, &arrowDirectionsX);
    dims[0] = dimensions[0];
    dims[1] = dimensions[1];
    writeDoubleMatrix(parent, "direction_x", 2, dims, arrowDirectionsX);
    releaseGraphicObjectProperty(__GO_DIRECTION_X__, arrowDirectionsX, jni_double_vector, dims[0] * dims[1]);

    //direction y
    getHandleDoubleVectorProperty(uid, __GO_DIRECTION_Y__, &arrowDirectionsY);
    dims[0] = dimensions[0];
    dims[1] = dimensions[1];
    writeDoubleMatrix(parent, "direction_y", 2, dims, arrowDirectionsY);
    releaseGraphicObjectProperty(__GO_DIRECTION_Y__, arrowDirectionsY, jni_double_vector, dims[0] * dims[1]);

    releaseGraphicObjectProperty(__GO_CHAMP_DIMENSIONS__, dimensions, jni_int_vector, 2);
    closeList6(parent);
    return true;
}
static bool export_handle_label(int parent, int uid)
{
    if (export_handle_generic(parent, uid, LabelHandle::getPropertyList()) == false)
    {
        return false;
    }

    //text
    int* dimensions = nullptr;
    char** text = nullptr;

    getGraphicObjectProperty(uid, __GO_TEXT_ARRAY_DIMENSIONS__, jni_int_vector, (void **)&dimensions);
    getGraphicObjectProperty(uid, __GO_TEXT_STRINGS__, jni_string_vector, (void **)&text);

    std::vector<int> dims = {dimensions[0], dimensions[1]};
    releaseGraphicObjectProperty(__GO_TEXT_ARRAY_DIMENSIONS__, dimensions, jni_int_vector, 2);

    writeStringMatrix6(parent, "text", 2, dims.data(), text);
    releaseGraphicObjectProperty(__GO_TEXT_STRINGS__, text, jni_string_vector, dims[0] * dims[1]);

    closeList6(parent);
    return true;
}
static bool export_handle_axes(int parent, int uid)
{
    if (export_handle_generic(parent, uid, AxesHandle::getPropertyList()) == false)
    {
        return false;
    }

    //title
    int title = getHandleIntProperty(uid, __GO_TITLE__);
    export_handle(parent, "title", title);

    //x_label
    int x_label = getHandleIntProperty(uid, __GO_X_AXIS_LABEL__);
    export_handle(parent, "x_label", x_label);

    //y_label
    int y_label = getHandleIntProperty(uid, __GO_Y_AXIS_LABEL__);
    export_handle(parent, "y_label", y_label);

    //z_label
    int z_label = getHandleIntProperty(uid, __GO_Z_AXIS_LABEL__);
    export_handle(parent, "z_label", z_label);


    closeList6(parent);
    return true;
}

static bool export_handle_figure(int parent, int uid)
{
    if (export_handle_generic(parent, uid, FigureHandle::getPropertyList()) == false)
    {
        return false;
    }

    //layout_options
    export_handle_layout_options(parent, uid);

    closeList6(parent);
    return true;
}

static bool export_handle_compound(int parent, int uid)
{
    if (export_handle_generic(parent, uid, CompoundHandle::getPropertyList()) == false)
    {
        return false;
    }

    closeList6(parent);
    return true;
}

static bool export_handle(int parent, const std::string& name, int uid)
{
    //get handle type
    int iType = -1;
    int *piType = &iType;
    getGraphicObjectProperty(uid, __GO_TYPE__, jni_int, (void **)&piType);

    //create handle node in __refs__
    int h = openList6(parent, name.data(), g_SCILAB_CLASS_HANDLE);

    bool ret = false;
    switch (iType)
    {
        case __GO_FIGURE__:
        {
            ret = export_handle_figure(h, uid);
            break;
        }
        case __GO_AXES__:
        {
            ret = export_handle_axes(h, uid);
            break;
        }
        case __GO_LABEL__:
        {
            ret = export_handle_label(h, uid);
            break;
        }
        case __GO_CHAMP__:
        {
            ret = export_handle_champ(h, uid);
            break;
        }
        case __GO_FAC3D__:
        {
            ret = export_handle_fac3d(h, uid);
            break;
        }
        case __GO_PLOT3D__:
        {
            ret = export_handle_plot3d(h, uid);
            break;
        }
        case __GO_POLYLINE__:
        {
            ret = export_handle_polyline(h, uid);
            break;
        }
        case __GO_DATATIP__:
        {
            ret = export_handle_datatip(h, uid);
            break;
        }
        case __GO_COMPOUND__:
        {
            ret = export_handle_compound(h, uid);
            break;
        }
        case __GO_RECTANGLE__:
        {
            ret = export_handle_rectangle(h, uid);
            break;
        }
        case __GO_ARC__:
        {
            ret = export_handle_arc(h, uid);
            break;
        }
        case __GO_SEGS__:
        {
            ret = export_handle_segs(h, uid);
            break;
        }
        default:
        {
        }

    }

    return ret;
}

static bool export_handle_children(int parent, int uid)
{
    int count = getHandleIntProperty(uid, __GO_CHILDREN_COUNT__);
    int node = openList6(parent, "children", g_SCILAB_CLASS_HANDLE);
    int* children = nullptr;

    if (count != 0)
    {
        getHandleIntVectorProperty(uid, __GO_CHILDREN__, &children);
    }

    int index = 0;
    for (int i = 0; i < count; ++i)
    {
        int hidden = 0;
        int *phidden = &hidden;

        int child = children[count - 1 - i]; //reverse order
        getGraphicObjectProperty(child, __GO_HIDDEN__, jni_bool, (void **)&phidden);

        if (hidden == 0)
        {
            if (export_handle(node, std::to_string(index), child) == false)
            {
                releaseGraphicObjectProperty(__GO_CHILDREN__, children, jni_int_vector, count);
                closeList6(node);
                return false;
            }

            ++index;
        }
    }

    releaseGraphicObjectProperty(__GO_CHILDREN__, children, jni_int_vector, count);
    closeList6(node);
    return true;
}

static int export_handles(int parent, const std::string& name, types::GraphicHandle* data)
{
    //create a group with cell name
    int dset = openList6(parent, name.data(), g_SCILAB_CLASS_HANDLE);
    //store cell dimensions
    std::vector<int> dims = {1, data->getDims()};
    int ret = writeIntegerMatrix6(dset, "__dims__", H5T_NATIVE_INT32, "32", 2, dims.data(), data->getDimsArray());
    if (ret < 0)
    {
        return -1;
    }

    //create a node for fields references
    int refs = openList6(dset, "__refs__", g_SCILAB_CLASS_HANDLE);
    if (refs < 0)
    {
        closeList6(dset);
        return -1;
    }

    int size = data->getSize();
    long long* ll = data->get();
    std::vector<hobj_ref_t> vrefs(size);
    for (int i = 0; i < size; ++i)
    {
        //get handle uid
        int hl = getObjectFromHandle(static_cast<long>(ll[i]));
        std::string refname(std::to_string(i));
        if (export_handle(refs, refname, hl) == false)
        {
            closeList6(refs);
            closeList6(dset);
            return -1;
        }
    }

    closeList6(refs);
    closeList6(dset);
    return dset;
}