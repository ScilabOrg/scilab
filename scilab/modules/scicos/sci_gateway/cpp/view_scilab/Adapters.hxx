/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef ADAPTERS_H_
#define ADAPTERS_H_

#include <vector>
#include <utility>
#include <string>

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

/*
 * Shared data between adapters
 */
class Adapters
{
public:
    typedef enum
    {
        INVALID_ADAPTER = -1,
        BLOCK_ADAPTER = 0,
        GRAPHIC_ADAPTER,
        MODEL_ADAPTER
    } adapters_index_t;

    static Adapters& instance();

    adapters_index_t lookup_by_typename(const std::wstring& name);

private:

    struct adapter_t
    {
        adapter_t(std::wstring n, adapters_index_t k) : name(n), kind(k) {};
        std::wstring name;
        adapters_index_t kind;
        bool operator<(const std::wstring& v)
        {
            return name < v;
        }
    };
    typedef std::vector<adapter_t> adapters_t;

    Adapters();
    ~Adapters();

    void insert_adapter_klass(const std::wstring& n, adapters_index_t kind);

    static Adapters m_instance;
    adapters_t adapters;

};

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* ADAPTERS_H_ */
