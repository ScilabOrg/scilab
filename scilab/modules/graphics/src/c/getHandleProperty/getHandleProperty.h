/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: getHandleProperty.h                                              */
/* desc : contains declarations of functions needed by sci_get            */
/*        Each function has the same signature int (void*, char*) and    */
/*        return a value in the scilab console                            */
/*------------------------------------------------------------------------*/

#ifndef _GET_HANDLE_PROPERTY_H_
#define _GET_HANDLE_PROPERTY_H_

#include "GetUimenuLabel.h"
#include "GetUicontrolString.h"
#include "GetUicontrolStyle.h"
#include "GetUicontrolPosition.h"
#include "GetUicontrolBackgroundColor.h"
#include "GetUiobjectForegroundColor.h"
#include "GetUicontrolFontWeight.h"
#include "GetUicontrolFontUnits.h"
#include "GetUicontrolFontSize.h"
#include "GetUicontrolFontAngle.h"
#include "GetUiobjectEnable.h"
#include "GetUicontrolMin.h"
#include "GetUicontrolMax.h"
#include "GetUicontrolUnits.h"
#include "GetUicontrolListboxTop.h"
#include "GetUicontrolValue.h"
#include "GetUicontrolRelief.h"
#include "GetUicontrolHorizontalAlignment.h"
#include "GetUicontrolVerticalAlignment.h"
#include "GetUicontrolFontName.h"
#include "GetUicontrolSliderStep.h"
#include "GetUimenuChecked.h"
#include "GetConsoleShowHiddenHandles.h"
#include "GetUicontrolTooltipString.h"

void* get_figures_id_property(void* _pvCtx, char* pobjUID);
void* get_visible_property(void* _pvCtx, char* pobjUID);
void* get_pixel_drawing_mode_property(void* _pvCtx, char* pobjUID);
void* get_old_style_property(void* _pvCtx, char* pobjUID);
void* get_figure_style_property(void* _pvCtx, char* pobjUID);
void* get_auto_resize_property(void* _pvCtx, char* pobjUID);
void* get_figure_position_property(void* _pvCtx, char* pobjUID);
void* get_axes_size_property(void* _pvCtx, char* pobjUID);
void* get_figure_size_property(void* _pvCtx, char* pobjUID);
void* get_figure_name_property(void* _pvCtx, char* pobjUID);
void* get_figure_id_property(void* _pvCtx, char* pobjUID);
void* get_figure_resizefcn_property(void* _pvCtx, char* pobjUID);
void* get_figure_closerequestfcn_property(void* _pvCtx, char* pobjUID);
void* get_rotation_style_property(void* _pvCtx, char* pobjUID);
void* get_immediate_drawing_property(void* _pvCtx, char* pobjUID);
void* get_pixmap_property(void* _pvCtx, char* pobjUID);
void* get_type_property(void* _pvCtx, char* pobjUID);
void* get_parent_property(void* _pvCtx, char* pobjUID);
void* get_current_axes_property(void* _pvCtx, char* pobjUID);
void* get_current_figure_property(void* _pvCtx, char* pobjUID);
void* get_current_entity_property(void* _pvCtx, char* pobjUID);
void* get_children_property(void* _pvCtx, char* pobjUID);
void* get_default_figure_property(void* _pvCtx, char* pobjUID);
void* get_default_axes_property(void* _pvCtx, char* pobjUID);
void* get_color_map_property(void* _pvCtx, char* pobjUID);
void* get_interp_color_vector_property(void* _pvCtx, char* pobjUID);
void* get_interp_color_mode_property(void* _pvCtx, char* pobjUID);
void* get_background_property(void* _pvCtx, char* pobjUID);
void* get_foreground_property(void* _pvCtx, char* pobjUID);
void* get_fill_mode_property(void* _pvCtx, char* pobjUID);
void* get_thickness_property(void* _pvCtx, char* pobjUID);
void* get_arrow_size_factor_property(void* _pvCtx, char* pobjUID);
void* get_line_style_property(void* _pvCtx, char* pobjUID);
void* get_line_mode_property(void* _pvCtx, char* pobjUID);
void* get_surface_mode_property(void* _pvCtx, char* pobjUID);
void* get_mark_style_property(void* _pvCtx, char* pobjUID);
void* get_mark_mode_property(void* _pvCtx, char* pobjUID);
void* get_mark_size_unit_property(void* _pvCtx, char* pobjUID);
void* get_mark_size_property(void* _pvCtx, char* pobjUID);
void* get_mark_foreground_property(void* _pvCtx, char* pobjUID);
void* get_mark_background_property(void* _pvCtx, char* pobjUID);
void* get_bar_layout_property(void* _pvCtx, char* pobjUID);
void* get_bar_width_property(void* _pvCtx, char* pobjUID);
void* get_x_shift_property(void* _pvCtx, char* pobjUID);
void* get_y_shift_property(void* _pvCtx, char* pobjUID);
void* get_z_shift_property(void* _pvCtx, char* pobjUID);
void* get_polyline_style_property(void* _pvCtx, char* pobjUID);
void* get_font_size_property(void* _pvCtx, char* pobjUID);
void* get_font_angle_property(void* _pvCtx, char* pobjUID);
void* get_font_foreground_property(void* _pvCtx, char* pobjUID);
void* get_font_color_property(void* _pvCtx, char* pobjUID);
void* get_font_style_property(void* _pvCtx, char* pobjUID);
void* get_text_box_mode_property(void* _pvCtx, char* pobjUID);
void* get_auto_dimensionning_property(void* _pvCtx, char* pobjUID);
void* get_alignment_property(void* _pvCtx, char* pobjUID);
void* get_text_box_property(void* _pvCtx, char* pobjUID);
void* get_text_property(void* _pvCtx, char* pobjUID);
void* get_auto_clear_property(void* _pvCtx, char* pobjUID);
void* get_auto_scale_property(void* _pvCtx, char* pobjUID);
void* get_zoom_box_property(void* _pvCtx, char* pobjUID);
void* get_zoom_state_property(void* _pvCtx, char* pobjUID);
void* get_clip_box_property(void* _pvCtx, char* pobjUID);
void* get_clip_state_property(void* _pvCtx, char* pobjUID);
void* get_data_property(void* _pvCtx, char* pobjUID);
void* get_callback_property(void* _pvCtx, char* pobjUID);
void* get_x_label_property(void* _pvCtx, char* pobjUID);
void* get_y_label_property(void* _pvCtx, char* pobjUID);
void* get_z_label_property(void* _pvCtx, char* pobjUID);
void* get_title_property(void* _pvCtx, char* pobjUID);
void* get_log_flags_property(void* _pvCtx, char* pobjUID);
void* get_tics_direction_property(void* _pvCtx, char* pobjUID);
void* get_x_location_property(void* _pvCtx, char* pobjUID);
void* get_y_location_property(void* _pvCtx, char* pobjUID);
void* get_tight_limits_property(void* _pvCtx, char* pobjUID);
void* get_closed_property(void* _pvCtx, char* pobjUID);
void* get_auto_position_property(void* _pvCtx, char* pobjUID);
void* get_auto_rotation_property(void* _pvCtx, char* pobjUID);
void* get_position_property(void* _pvCtx, char* pobjUID);
void* get_auto_ticks_property(void* _pvCtx, char* pobjUID);
void* get_axes_reverse_property(void* _pvCtx, char* pobjUID);
void* get_view_property(void* _pvCtx, char* pobjUID);
void* get_axes_bounds_property(void* _pvCtx, char* pobjUID);
void* get_data_bounds_property(void* _pvCtx, char* pobjUID);
void* get_margins_property(void* _pvCtx, char* pobjUID);
void* get_tics_color_property(void* _pvCtx, char* pobjUID);
void* get_tics_style_property(void* _pvCtx, char* pobjUID);
void* get_sub_tics_property(void* _pvCtx, char* pobjUID);
void* get_tics_segment_property(void* _pvCtx, char* pobjUID);
void* get_labels_font_size_property(void* _pvCtx, char* pobjUID);
void* get_labels_font_color_property(void* _pvCtx, char* pobjUID);
void* get_labels_font_style_property(void* _pvCtx, char* pobjUID);
void* get_format_n_property(void* _pvCtx, char* pobjUID);
void* get_xtics_coord_property(void* _pvCtx, char* pobjUID);
void* get_ytics_coord_property(void* _pvCtx, char* pobjUID);
void* get_tics_labels_property(void* _pvCtx, char* pobjUID);
void* get_box_property(void* _pvCtx, char* pobjUID);
void* get_grid_property(void* _pvCtx, char* pobjUID);
void* get_axes_visible_property(void* _pvCtx, char* pobjUID);
void* get_hidden_color_property(void* _pvCtx, char* pobjUID);
void* get_isoview_property(void* _pvCtx, char* pobjUID);
void* get_cube_scaling_property(void* _pvCtx, char* pobjUID);
void* get_arrow_size_property(void* _pvCtx, char* pobjUID);
void* get_segs_color_property(void* _pvCtx, char* pobjUID);
void* get_colored_property(void* _pvCtx, char* pobjUID);
void* get_data_mapping_property(void* _pvCtx, char* pobjUID);
void* get_rotation_angles_property(void* _pvCtx, char* pobjUID);
void* get_color_mode_property(void* _pvCtx, char* pobjUID);
void* get_color_flag_property(void* _pvCtx, char* pobjUID);
void* get_cdata_mapping_property(void* _pvCtx, char* pobjUID);
void* get_surface_color_property(void* _pvCtx, char* pobjUID);
void* get_triangles_property(void* _pvCtx, char* pobjUID);
void* get_z_bounds_property(void* _pvCtx, char* pobjUID);
void* get_user_data_property(void* _pvCtx, char* pobjUID);
void* get_handle_visible_property(void* _pvCtx, char* pobjUID);
void* get_callback_type_property(void* _pvCtx, char* pobjUID);
void* get_hidden_axis_color_property(void* _pvCtx, char* pobjUID);
void* get_x_ticks_property(void* _pvCtx, char* pobjUID);
void* get_y_ticks_property(void* _pvCtx, char* pobjUID);
void* get_z_ticks_property(void* _pvCtx, char* pobjUID);
void* get_viewport_property(void* _pvCtx, char* pobjUID);
void* get_info_message_property(void* _pvCtx, char* pobjUID);
void* get_screen_position_property(void* _pvCtx, char* pobjUID);
void* get_event_handler_enable_property(void* _pvCtx, char* pobjUID);
void* get_event_handler_property(void* _pvCtx, char* pobjUID);
void* get_arc_drawing_method_property(void* _pvCtx, char* pobjUID);
void* get_fractional_font_property(void* _pvCtx, char* pobjUID);
void* get_links_property(void* _pvCtx, char* pobjUID);
void* get_legend_location_property(void* _pvCtx, char* pobjUID);
void* get_filled_property(void* _pvCtx, char* pobjUID);
void* get_color_range_property(void* _pvCtx, char* pobjUID);
void* get_outside_colors_property(void* _pvCtx, char* pobjUID);
void* get_grid_position_property(void* _pvCtx, char* pobjUID);
void* get_anti_aliasing_property(void* _pvCtx, char* pobjUID);
void* get_tag_property(void* _pvCtx, char* pobjUID);
void* get_UID(void* _pvCtx, char *pobjUID);

#endif /* _GET_HANDLE_PROPERTY_H_ */
