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

#ifndef CONTROLLER_HXX_
#define CONTROLLER_HXX_

#include <vector>
#include <string>

#include "utilities.hxx"
#include "Model.hxx"
#include "View.hxx"

#include "dynlib_scicos.h"

namespace org_scilab_modules_scicos
{

/**
 * Controller from the MVC pattern
 *
 * All calls to objects should use this controller, all usable objects are referenced by this controller.
 */
SCICOS_IMPEXP class Controller
{
public:
    static void delete_all_instances();

    static void register_view(View* v);
    static void unregister_view(View* v);

    Controller();
    ~Controller();

    ScicosID createObject(kind_t k);
    void deleteObject(ScicosID uid);

    model::BaseObject* getObject(ScicosID uid);
    update_status_t setObject(model::BaseObject* o);

    template<typename T>
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, T& v) const
    {
        return _instance->model.getObjectProperty(uid, k, p, v);
    };

    template<typename T>
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, T v)
    {
        update_status_t status = _instance->model.setObjectProperty(uid, k, p, v);
        if (status == SUCCESS)
        {
            for (view_set_t::iterator iter = _instance->allViews.begin(); iter != _instance->allViews.end(); iter++)
            {
                (*iter)->propertyUpdated(uid, k, p);
            }
        }
        for (view_set_t::iterator iter = _instance->allViews.begin(); iter != _instance->allViews.end(); iter++)
        {
            (*iter)->propertyUpdated(uid, k, p, status);
        }
        return status;
    }

private:

    typedef std::vector<View*> view_set_t;

    /**
     * Shared data through all instance of the controllers
     */
    struct SharedData
    {
        Model model;
        view_set_t allViews;

        SharedData();
        ~SharedData();
    };

    /**
     * Shared instance of the data
     *
     * This will be allocated on-demand be Controller::get_instance()
     */
    static SharedData* _instance;
};

} /* namespace org_scilab_modules_scicos */

#endif /* CONTROLLER_HXX_ */
