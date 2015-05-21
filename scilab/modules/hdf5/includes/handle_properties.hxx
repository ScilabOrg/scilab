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

#include <unordered_map>
#include <list>
#include <vector>

#define SAVE_ONLY 0
#define SAVE_LOAD 1

extern "C"
{
#include "graphicObjectProperties.h"
#include "returnType.h"
}

typedef std::list<std::pair<std::string, std::vector<int> > > HandleProp;

struct FigureHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));
        m.emplace_back("toolbar", std::vector<int>({SAVE_ONLY, __GO_TOOLBAR__, jni_int}));
        m.emplace_back("toolbar_visible", std::vector<int>({SAVE_ONLY, __GO_TOOLBAR_VISIBLE__, jni_bool}));
        m.emplace_back("menubar", std::vector<int>({SAVE_ONLY, __GO_MENUBAR__, jni_int}));
        m.emplace_back("menubar_visible", std::vector<int>({SAVE_ONLY, __GO_MENUBAR_VISIBLE__, jni_bool}));
        m.emplace_back("infobar_visible", std::vector<int>({SAVE_ONLY, __GO_INFOBAR_VISIBLE__, jni_bool}));
        m.emplace_back("dockable", std::vector<int>({SAVE_ONLY, __GO_DOCKABLE__, jni_bool}));
        m.emplace_back("default_axes", std::vector<int>({SAVE_ONLY, __GO_DEFAULT_AXES__, jni_bool}));

        //@start
        m.emplace_back("figure_position", std::vector<int>({SAVE_LOAD, __GO_POSITION__, jni_int_vector, -1, -2}));
        m.emplace_back("auto_resize", std::vector<int>({SAVE_LOAD, __GO_AUTORESIZE__, jni_bool}));
        m.emplace_back("figure_size", std::vector<int>({SAVE_LOAD, __GO_SIZE__, jni_int_vector, -1, -2}));
        m.emplace_back("axes_size", std::vector<int>({SAVE_LOAD, __GO_AXES_SIZE__, jni_int_vector, -1, -2}));

        //@ don't care
        m.emplace_back("viewport", std::vector<int>({SAVE_LOAD, __GO_VIEWPORT__, jni_int_vector, -1, -2}));
        m.emplace_back("info_message", std::vector<int>({SAVE_LOAD, __GO_INFO_MESSAGE__, jni_string}));
        m.emplace_back("tag", std::vector<int>({SAVE_LOAD, __GO_TAG__, jni_string}));
        m.emplace_back("figure_name", std::vector<int>({SAVE_LOAD, __GO_NAME__, jni_string}));
        m.emplace_back("color_map", std::vector<int>({SAVE_LOAD, __GO_COLORMAP__, jni_double_vector, __GO_COLORMAP_SIZE__, -3}));
        m.emplace_back("pixel_drawing_mode", std::vector<int>({SAVE_LOAD, __GO_PIXEL_DRAWING_MODE__, jni_int}));
        m.emplace_back("anti_aliasing", std::vector<int>({SAVE_LOAD, __GO_ANTIALIASING__, jni_int}));
        m.emplace_back("immediate_drawing", std::vector<int>({SAVE_LOAD, __GO_IMMEDIATE_DRAWING__, jni_bool}));
        m.emplace_back("background", std::vector<int>({SAVE_LOAD, __GO_BACKGROUND__, jni_int}));
        m.emplace_back("rotation_style", std::vector<int>({SAVE_LOAD, __GO_ROTATION_TYPE__, jni_int}));
        m.emplace_back("event_handler_enable", std::vector<int>({SAVE_LOAD, __GO_EVENTHANDLER_ENABLE__, jni_bool}));
        m.emplace_back("closerequestfcn", std::vector<int>({SAVE_LOAD, __GO_CLOSEREQUESTFCN__, jni_string}));
        m.emplace_back("resize", std::vector<int>({SAVE_LOAD, __GO_RESIZE__, jni_bool}));
        m.emplace_back("layout", std::vector<int>({SAVE_LOAD, __GO_LAYOUT__, jni_int}));
        m.emplace_back("icon", std::vector<int>({SAVE_LOAD, __GO_UI_ICON__, jni_string}));

        //@end
        m.emplace_back("event_handler", std::vector<int>({SAVE_LOAD, __GO_EVENTHANDLER_NAME__, jni_string}));
        m.emplace_back("resizefcn", std::vector<int>({SAVE_LOAD, __GO_RESIZEFCN__, jni_string}));
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));
        return m;
    }
};

struct AxesHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));
        m.emplace_back("visible", std::vector<int>({SAVE_ONLY, __GO_VISIBLE__, jni_bool}));

        //@start

        //@ don't care
        m.emplace_back("axes_visible_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_VISIBLE__, jni_bool}));
        m.emplace_back("axes_visible_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_VISIBLE__, jni_bool}));
        m.emplace_back("axes_visible_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_VISIBLE__, jni_bool}));
        m.emplace_back("axes_reverse_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_REVERSE__, jni_bool}));
        m.emplace_back("axes_reverse_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_REVERSE__, jni_bool}));
        m.emplace_back("axes_reverse_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_REVERSE__, jni_bool}));
        m.emplace_back("grid_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_GRID_COLOR__, jni_int}));
        m.emplace_back("grid_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_GRID_COLOR__, jni_int}));
        m.emplace_back("grid_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_GRID_COLOR__, jni_int}));
        m.emplace_back("grid_position", std::vector<int>({SAVE_LOAD, __GO_GRID_POSITION__, jni_int}));
        m.emplace_back("grid_thickness_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_GRID_THICKNESS__, jni_double}));
        m.emplace_back("grid_thickness_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_GRID_THICKNESS__, jni_double}));
        m.emplace_back("grid_thickness_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_GRID_THICKNESS__, jni_double}));
        m.emplace_back("grid_style_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_GRID_STYLE__, jni_int}));
        m.emplace_back("grid_style_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_GRID_STYLE__, jni_int}));
        m.emplace_back("grid_style_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_GRID_STYLE__, jni_int}));
        m.emplace_back("x_location", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_LOCATION__, jni_int}));
        m.emplace_back("y_location", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_LOCATION__, jni_int}));
        m.emplace_back("view", std::vector<int>({SAVE_LOAD, __GO_VIEW__, jni_int}));
        m.emplace_back("ticks_format_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_FORMAT__, jni_string}));
        m.emplace_back("ticks_format_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_FORMAT__, jni_string}));
        m.emplace_back("ticks_format_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_FORMAT__, jni_string}));
        m.emplace_back("ticks_st_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_ST_FACTORS__, jni_double_vector, -1, -2}));
        m.emplace_back("ticks_st_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_ST_FACTORS__, jni_double_vector, -1, -2}));
        m.emplace_back("ticks_st_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_ST_FACTORS__, jni_double_vector, -1, -2}));
        m.emplace_back("box", std::vector<int>({SAVE_LOAD, __GO_BOX_TYPE__, jni_int}));
        m.emplace_back("filled", std::vector<int>({SAVE_LOAD, __GO_FILLED__, jni_bool}));
        m.emplace_back("sub_tics_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_SUBTICKS__, jni_int}));
        m.emplace_back("sub_tics_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_SUBTICKS__, jni_int}));
        m.emplace_back("sub_tics_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_SUBTICKS__, jni_int}));
        m.emplace_back("font_style", std::vector<int>({SAVE_LOAD, __GO_FONT_STYLE__, jni_int}));
        m.emplace_back("font_size", std::vector<int>({SAVE_LOAD, __GO_FONT_SIZE__, jni_double}));
        m.emplace_back("font_color", std::vector<int>({SAVE_LOAD, __GO_FONT_COLOR__, jni_int}));
        m.emplace_back("fractional_font", std::vector<int>({SAVE_LOAD, __GO_FONT_FRACTIONAL__, jni_bool}));
        m.emplace_back("isoview", std::vector<int>({SAVE_LOAD, __GO_ISOVIEW__, jni_bool}));
        m.emplace_back("cube_scaling", std::vector<int>({SAVE_LOAD, __GO_CUBE_SCALING__, jni_bool}));
        m.emplace_back("rotation_angles", std::vector<int>({SAVE_LOAD, __GO_ROTATION_ANGLES__, jni_double_vector, -1, -2}));
        m.emplace_back("tight_limits_x", std::vector<int>({SAVE_LOAD, __GO_X_TIGHT_LIMITS__, jni_bool}));
        m.emplace_back("tight_limits_y", std::vector<int>({SAVE_LOAD, __GO_Y_TIGHT_LIMITS__, jni_bool}));
        m.emplace_back("tight_limits_z", std::vector<int>({SAVE_LOAD, __GO_Z_TIGHT_LIMITS__, jni_bool}));
        m.emplace_back("data_bounds", std::vector<int>({SAVE_LOAD, __GO_DATA_BOUNDS__, jni_double_vector, -2, -3}));
        m.emplace_back("zoom_box", std::vector<int>({SAVE_LOAD, __GO_ZOOM_BOX__, jni_double_vector, -1, -6}));
        m.emplace_back("margins", std::vector<int>({SAVE_LOAD, __GO_MARGINS__, jni_double_vector, -1, -4}));
        m.emplace_back("auto_margins", std::vector<int>({SAVE_LOAD, __GO_AUTO_MARGINS__, jni_bool}));
        m.emplace_back("axes_bounds", std::vector<int>({SAVE_LOAD, __GO_AXES_BOUNDS__, jni_double_vector, -1, -4}));
        m.emplace_back("auto_clear", std::vector<int>({SAVE_LOAD, __GO_AUTO_CLEAR__, jni_bool}));
        m.emplace_back("auto_scale", std::vector<int>({SAVE_LOAD, __GO_AUTO_SCALE__, jni_bool}));
        m.emplace_back("hidden_axis_color", std::vector<int>({SAVE_LOAD, __GO_HIDDEN_AXIS_COLOR__, jni_int}));
        m.emplace_back("arc_drawing_method", std::vector<int>({SAVE_LOAD, __GO_ARC_DRAWING_METHOD__, jni_int}));
        m.emplace_back("hiddencolor", std::vector<int>({SAVE_LOAD, __GO_HIDDEN_COLOR__, jni_int}));
        m.emplace_back("line_mode", std::vector<int>({SAVE_LOAD, __GO_LINE_MODE__, jni_bool}));
        m.emplace_back("line_style", std::vector<int>({SAVE_LOAD, __GO_LINE_STYLE__, jni_int}));
        m.emplace_back("thickness", std::vector<int>({SAVE_LOAD, __GO_LINE_THICKNESS__, jni_double}));
        m.emplace_back("mark_style", std::vector<int>({SAVE_LOAD, __GO_MARK_STYLE__, jni_int}));
        m.emplace_back("mark_size", std::vector<int>({SAVE_LOAD, __GO_MARK_SIZE__, jni_int}));
        m.emplace_back("mark_size_unit", std::vector<int>({SAVE_LOAD, __GO_MARK_SIZE_UNIT__, jni_int}));
        m.emplace_back("mark_foreground", std::vector<int>({SAVE_LOAD, __GO_MARK_FOREGROUND__, jni_int}));
        m.emplace_back("mark_background", std::vector<int>({SAVE_LOAD, __GO_MARK_BACKGROUND__, jni_int}));
        m.emplace_back("foreground", std::vector<int>({SAVE_LOAD, __GO_LINE_COLOR__, jni_int}));
        m.emplace_back("background", std::vector<int>({SAVE_LOAD, __GO_BACKGROUND__, jni_int}));
        m.emplace_back("clip_state", std::vector<int>({SAVE_LOAD, __GO_CLIP_STATE__, jni_int}));
        m.emplace_back("clip_box", std::vector<int>({SAVE_LOAD, __GO_CLIP_BOX__, jni_double_vector, -1, -4}));

        m.emplace_back("ticks_locations_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_TICKS_LOCATIONS__, jni_double_vector, -1, __GO_X_AXIS_NUMBER_TICKS__}));
        m.emplace_back("ticks_labels_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_TICKS_LABELS__, jni_string_vector, -1, __GO_X_AXIS_NUMBER_TICKS__}));
        m.emplace_back("ticks_locations_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_TICKS_LOCATIONS__, jni_double_vector, -1, __GO_Y_AXIS_NUMBER_TICKS__}));
        m.emplace_back("ticks_labels_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_TICKS_LABELS__, jni_string_vector, -1, __GO_Y_AXIS_NUMBER_TICKS__}));
        m.emplace_back("ticks_locations_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_TICKS_LOCATIONS__, jni_double_vector, -1, __GO_Z_AXIS_NUMBER_TICKS__}));
        m.emplace_back("ticks_labels_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_TICKS_LABELS__, jni_string_vector, -1, __GO_Z_AXIS_NUMBER_TICKS__}));

        //@end

        //must be set after data_bounds
        m.emplace_back("log_flags_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_LOG_FLAG__, jni_bool}));
        m.emplace_back("log_flags_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_LOG_FLAG__, jni_bool}));
        m.emplace_back("log_flags_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_LOG_FLAG__, jni_bool}));

        //must be set mark_style
        m.emplace_back("mark_mode", std::vector<int>({SAVE_LOAD, __GO_MARK_MODE__, jni_bool}));

        //must be set after ticks labels
        m.emplace_back("auto_ticks_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_AUTO_TICKS__, jni_bool}));
        m.emplace_back("auto_ticks_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_AUTO_TICKS__, jni_bool}));
        m.emplace_back("auto_ticks_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_AUTO_TICKS__, jni_bool}));

        return m;
    }
};

struct LabelHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));

        //@start

        //@ don't care
        m.emplace_back("position", std::vector<int>({SAVE_LOAD, __GO_POSITION__, jni_double_vector, -1, -3}));
        m.emplace_back("font_foreground", std::vector<int>({SAVE_LOAD, __GO_FONT_COLOR__, jni_int}));
        m.emplace_back("foreground", std::vector<int>({SAVE_LOAD, __GO_LINE_COLOR__, jni_int}));
        m.emplace_back("background", std::vector<int>({SAVE_LOAD, __GO_BACKGROUND__, jni_int}));
        m.emplace_back("fill_mode", std::vector<int>({SAVE_LOAD, __GO_FILL_MODE__, jni_bool}));
        m.emplace_back("font_style", std::vector<int>({SAVE_LOAD, __GO_FONT_STYLE__, jni_int}));
        m.emplace_back("font_size", std::vector<int>({SAVE_LOAD, __GO_FONT_SIZE__, jni_double}));
        m.emplace_back("fractional_font", std::vector<int>({SAVE_LOAD, __GO_FONT_FRACTIONAL__, jni_bool}));
        m.emplace_back("font_angle", std::vector<int>({SAVE_LOAD, __GO_FONT_ANGLE__, jni_double}));
        m.emplace_back("auto_rotation", std::vector<int>({SAVE_LOAD, __GO_AUTO_ROTATION__, jni_bool}));
        m.emplace_back("auto_position", std::vector<int>({SAVE_LOAD, __GO_AUTO_POSITION__, jni_bool}));

        //@end
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));

        return m;
    }
};

struct ChampHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));

        //@start
        m.emplace_back("clip_box", std::vector<int>({SAVE_LOAD, __GO_CLIP_BOX__, jni_double_vector, -1, -4}));
        m.emplace_back("clip_state", std::vector<int>({SAVE_LOAD, __GO_CLIP_STATE__, jni_int}));

        //@ don't care
        m.emplace_back("arrow_size", std::vector<int>({SAVE_LOAD, __GO_ARROW_SIZE__, jni_double}));
        m.emplace_back("line_style", std::vector<int>({SAVE_LOAD, __GO_LINE_STYLE__, jni_int}));
        m.emplace_back("thickness", std::vector<int>({SAVE_LOAD, __GO_LINE_THICKNESS__, jni_double}));
        m.emplace_back("colored", std::vector<int>({SAVE_LOAD, __GO_COLORED__, jni_bool}));

        //@end
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));
        return m;
    }
};

struct SurfaceHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));

        //@start

        //@ don't care
        m.emplace_back("surface_mode", std::vector<int>({SAVE_LOAD, __GO_SURFACE_MODE__, jni_bool}));
        m.emplace_back("foreground", std::vector<int>({SAVE_LOAD, __GO_LINE_COLOR__, jni_int}));
        m.emplace_back("thickness", std::vector<int>({SAVE_LOAD, __GO_LINE_THICKNESS__, jni_double}));
        m.emplace_back("mark_mode", std::vector<int>({SAVE_LOAD, __GO_MARK_MODE__, jni_bool}));
        m.emplace_back("mark_style", std::vector<int>({SAVE_LOAD, __GO_MARK_STYLE__, jni_int}));
        m.emplace_back("mark_size", std::vector<int>({SAVE_LOAD, __GO_MARK_SIZE__, jni_int}));
        m.emplace_back("mark_size_unit", std::vector<int>({SAVE_LOAD, __GO_MARK_SIZE_UNIT__, jni_int}));
        m.emplace_back("mark_foreground", std::vector<int>({SAVE_LOAD, __GO_MARK_FOREGROUND__, jni_int}));
        m.emplace_back("mark_background", std::vector<int>({SAVE_LOAD, __GO_MARK_BACKGROUND__, jni_int}));
        m.emplace_back("color_mode", std::vector<int>({SAVE_LOAD, __GO_COLOR_MODE__, jni_int}));
        m.emplace_back("color_flag", std::vector<int>({SAVE_LOAD, __GO_COLOR_FLAG__, jni_int}));
        m.emplace_back("ambient_color", std::vector<int>({SAVE_LOAD, __GO_AMBIENTCOLOR__, jni_double_vector, -1, -3}));
        m.emplace_back("diffuse_color", std::vector<int>({SAVE_LOAD, __GO_DIFFUSECOLOR__, jni_double_vector, -1, -3}));
        m.emplace_back("specular_color", std::vector<int>({SAVE_LOAD, __GO_SPECULARCOLOR__, jni_double_vector, -1, -3}));
        m.emplace_back("use_color_material", std::vector<int>({SAVE_LOAD, __GO_COLOR_MATERIAL__, jni_bool}));
        m.emplace_back("material_shininess", std::vector<int>({SAVE_LOAD, __GO_MATERIAL_SHININESS__, jni_double}));
        m.emplace_back("hiddencolor", std::vector<int>({SAVE_LOAD, __GO_HIDDEN_COLOR__, jni_int}));
        m.emplace_back("clip_box", std::vector<int>({SAVE_LOAD, __GO_CLIP_BOX__, jni_double_vector, -1, -4}));
        m.emplace_back("clip_state", std::vector<int>({SAVE_LOAD, __GO_CLIP_STATE__, jni_int}));

        //only for Fac3d
        //m.emplace_back("cdata_mapping", std::vector<int>({SAVE_LOAD, __GO_COLORED__, jni_bool}));

        //@end
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));
        return m;
    }
};