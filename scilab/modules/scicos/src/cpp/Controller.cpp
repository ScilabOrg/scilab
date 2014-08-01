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

#include <algorithm>

#include "Controller.hxx"

#include "LoggerView.hxx"

namespace org_scilab_modules_scicos
{

/*
 * Implement SharedData methods
 */
Controller::SharedData::SharedData() : model(), allViews()
{
    LoggerView* v = new LoggerView();
    allViews.push_back(v);
}

Controller::SharedData::~SharedData()
{
    for (view_set_t::iterator iter = _instance->allViews.begin(); iter != _instance->allViews.end(); iter++)
    {
        delete *iter;
    }
}

Controller::SharedData* Controller::_instance = 0;

void Controller::delete_all_instances()
{
    if (_instance == 0)
    {
        return;
    }

    delete _instance;
}

void Controller::register_view(View* v)
{
    if (_instance == 0)
    {
        _instance = new SharedData();
    }

    _instance->allViews.push_back(v);
}

void Controller::unregister_view(View* v)
{
    if (_instance == 0)
    {
        return;
    }

    view_set_t::iterator it = std::find(_instance->allViews.begin(), _instance->allViews.end(), v);
    if (it != _instance->allViews.end())
    {
        _instance->allViews.erase(it);
    }
}

Controller::Controller()
{
    if (_instance == 0)
    {
        _instance = new SharedData();
    }
}

Controller::~Controller()
{
}

ScicosID Controller::createObject(kind_t k)
{
    ScicosID id = _instance->model.createObject(k);

    for (view_set_t::iterator iter = _instance->allViews.begin(); iter != _instance->allViews.end(); iter++)
    {
        (*iter)->objectCreated(id, k);
    }

    return id;
}

void Controller::deleteObject(ScicosID uid)
{
    _instance->model.deleteObject(uid);

    for (view_set_t::iterator iter = _instance->allViews.begin(); iter != _instance->allViews.end(); iter++)
    {
        (*iter)->objectDeleted(uid);
    }
}

model::BaseObject* Controller::getObject(ScicosID uid)
{
    return _instance->model.getObject(uid);
}

update_status_t Controller::setObject(model::BaseObject* o)
{
    update_status_t status = _instance->model.setObject(o);

    if (status == SUCCESS)
    {
        for (view_set_t::iterator iter = _instance->allViews.begin(); iter != _instance->allViews.end(); iter++)
        {
            (*iter)->objectUpdated(o->id(), o->kind());
        }
    }
    return status;
}

} /* namespace org_scilab_modules_scicos */
