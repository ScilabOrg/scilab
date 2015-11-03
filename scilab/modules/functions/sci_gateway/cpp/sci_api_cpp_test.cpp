#include "functions_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "cell.hxx"
#include "struct.hxx"
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_api_cpp_empty(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    out.push_back(new types::Double(1));
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
double computework(types::InternalType* item)
{
    double ret = 0;
    switch (item->getType())
    {
        case types::InternalType::ScilabDouble:
        {
            types::Double* d = item->getAs<types::Double>();
            ret += d->get()[0];
            break;
        }
        case types::InternalType::ScilabString:
        {
            types::String* s = item->getAs<types::String>();
            ret += wcslen(s->get()[0]);
            break;
        }
        case types::InternalType::ScilabCell:
        {
            types::Cell* c = item->getAs<types::Cell>();

            int size = c->getSize();
            types::InternalType** items = c->get();
            for (int i = 0; i < size; ++i)
            {
                ret += computework(items[i]);
            }
            break;
        }
        case types::InternalType::ScilabStruct:
        {
            types::Struct* c = item->getAs<types::Struct>();
            types::String* fields = c->getFieldNames();
            wchar_t** f = fields->get();
            int size = fields->getSize();
            types::SingleStruct* items = c->get()[0];
            for (int i = 0; i < size; ++i)
            {
                ret += computework(items->get(f[i]));
            }

            delete fields;
            break;
        }
    }

    return ret;
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_api_cpp_work(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    double ret = 0;
    for (auto& i : in)
    {
        ret += computework(i);
    }

    out.push_back(new types::Double(ret));
    return types::Function::OK;
}