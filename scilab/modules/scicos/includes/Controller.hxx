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

#ifndef CONTROLLER_HXX_
#define CONTROLLER_HXX_

#include <atomic>
#include <string>
#include <vector>
#include <map>

#include "utilities.hxx"
#include "Model.hxx"
#include "View.hxx"
#include "model/BaseObject.hxx"

#include "dynlib_scicos.h"

namespace org_scilab_modules_scicos
{

/**
 * Controller from the MVC pattern
 *
 * All calls to objects should use this controller, all usable objects are referenced by this controller.
 */
class SCICOS_IMPEXP Controller
{
public:
    static View* register_view(const std::string& name, View* v);
    static void unregister_view(View* v);
    static View* unregister_view(const std::string& name);
    static View* look_for_view(const std::string& name);

    Controller();
    ~Controller();

    ScicosID createObject(kind_t k);
    unsigned referenceObject(const ScicosID uid) const;
    template<typename T>
    T* referenceObject(T* o) const
    {
        referenceObject(o->id());
        return o;
    }
    void deleteObject(ScicosID uid);
    ScicosID cloneObject(ScicosID uid, bool cloneChildren, bool clonePorts);

    kind_t getKind(ScicosID uid) const;
    std::vector<ScicosID> getAll(kind_t k) const;
    void sortAndFillKind(std::vector<ScicosID>& uids, std::vector<int>& kind);
    model::BaseObject* getObject(ScicosID uid) const;
    template<typename T>
    T* getObject(ScicosID uid) const
    {
        return static_cast<T*>(getObject(uid));
    }

    template<typename T> bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, T& v) const;
    template<typename T> update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, T v);

private:

    typedef std::vector<View*> view_set_t;
    typedef std::vector<std::string> view_name_set_t;

    /**
     * Shared data through all instance of the controllers
     */
    struct SharedData
    {
        std::atomic_flag onModelStructuralModification;
        Model model;

        std::atomic_flag onViewsStructuralModification;
        view_name_set_t allNamedViews;
        view_set_t allViews;

        SharedData();
        ~SharedData();
    };

    /**
     * Shared instance of the data
     *
     * This will be allocated on-demand by Controller::get_instance()
     */
    static SharedData m_instance;

    /*
     * Methods
     */

    ScicosID cloneObject(std::map<ScicosID, ScicosID>& mapped, ScicosID uid, bool cloneChildren, bool clonePorts);
    template<typename T>
    void cloneProperties(model::BaseObject* initial, ScicosID clone);
    void deepClone(std::map<ScicosID, ScicosID>& mapped, ScicosID uid, ScicosID clone, kind_t k, object_properties_t p, bool cloneIfNotFound);
    void deepCloneVector(std::map<ScicosID, ScicosID>& mapped, ScicosID uid, ScicosID clone, kind_t k, object_properties_t p, bool cloneIfNotFound);
    void unlinkVector(ScicosID uid, kind_t k, object_properties_t uid_prop, object_properties_t ref_prop);
    void unlink(ScicosID uid, kind_t k, object_properties_t uid_prop, object_properties_t ref_prop);
    void deleteVector(ScicosID uid, kind_t k, object_properties_t uid_prop);
};

#if ! defined SWIG /* SWIG does not support extern template parsing, templates are explicitly instanciated on the swig file */

extern template bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, double& v) const;
extern template bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, int& v) const;
extern template bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, bool& v) const;
extern template bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::string& v) const;
extern template bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, ScicosID& v) const;
extern template bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector<double>& v) const;
extern template bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector<int>& v) const;
extern template bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector<bool>& v) const;
extern template bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector< std::string >& v) const;
extern template bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector<ScicosID>& v) const;

extern template update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, double v);
extern template update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, int v);
extern template update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, bool v);
extern template update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, ScicosID v);
extern template update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::string& v);
extern template update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<double>& v);
extern template update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<int>& v);
extern template update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<bool>& v);
extern template update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector< std::string >& v);
extern template update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<ScicosID>& v);

#endif /* SWIG */

} /* namespace org_scilab_modules_scicos */

#endif /* CONTROLLER_HXX_ */
