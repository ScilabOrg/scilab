/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef ADAPTERS_HXX_
#define ADAPTERS_HXX_

#include <vector>
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
        BLOCK_ADAPTER = 0,  //!< \see view_scilab::BlockAdapter
        CPR_ADAPTER,        //!< \see view_scilab::CprAdapter
        DIAGRAM_ADAPTER,    //!< \see view_scilab::DiagramAdapter
        GRAPHIC_ADAPTER,    //!< \see view_scilab::GraphicAdapter
        LINK_ADAPTER,       //!< \see view_scilab::LinkAdapter
        MODEL_ADAPTER,      //!< \see view_scilab::ModelAdapter
        PARAMS_ADAPTER,    //!< \see view_scilab::ParamsAdapter
        SCS_ADAPTER,        //!< \see view_scilab::ScsAdapter
        STATE_ADAPTER,      //!< \see view_scilab::StateAdapter
        TEXT_ADAPTER,       //!< \see view_scilab::TextAdapter

        INVALID_ADAPTER     //!< length of the enum is used to report error
    } adapters_index_t;

    static Adapters& instance();

    adapters_index_t lookup_by_typename(const std::wstring& name);

private:

    struct adapter_t
    {
        adapter_t(std::wstring n, adapters_index_t k) : name(n), kind(k) {};

        inline bool operator<(const adapter_t& v) const
        {
            return name < v.name;
        }

        inline bool operator<(const std::wstring& v) const
        {
            return name < v;
        }

        std::wstring name;
        adapters_index_t kind;
    };
    typedef std::vector<adapter_t> adapters_t;

    Adapters();
    ~Adapters();

    static Adapters m_instance;
    adapters_t adapters;
};

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* ADAPTERS_HXX_ */
