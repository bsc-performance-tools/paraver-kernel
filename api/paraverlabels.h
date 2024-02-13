/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                      API Library for libparaver-kernel                    *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/


#pragma once

#include "utils/traceparser/tracelabels.h"

constexpr char CFG_CURRENT_VERSION[] = "3.4";

constexpr char OLDCFG_HEADER_VERSION[] = "version";
constexpr char OLDCFG_HEADER_NUM_WINDOWS[] = "number_of_windows";
constexpr char OLDCFG_HEADER_BEGIN_DESCRIPTION[] = "begin_description";
constexpr char OLDCFG_HEADER_END_DESCRIPTION[] = "end_description";

constexpr char OLDCFG_LVL_APPL[]     = "appl";
constexpr char OLDCFG_LVL_PTASK[]    = "ptask";
constexpr char OLDCFG_LVL_WORKLOAD[] = "workload";
constexpr char OLDCFG_LVL_TASK[]     = "task";
constexpr char OLDCFG_LVL_THREAD[]   = "thread";
constexpr char OLDCFG_LVL_CPU[]      = "cpu";
constexpr char OLDCFG_LVL_NODE[]     = "node";
constexpr char OLDCFG_LVL_SYSTEM[]   = "system";

constexpr char OLDCFG_LVL_HISTO_WORKLOAD[] = "Workload";
constexpr char OLDCFG_LVL_HISTO_APPL[] = "Application";
constexpr char OLDCFG_LVL_HISTO_TASK[] = "Task";
constexpr char OLDCFG_LVL_HISTO_THREAD[] = "Thread";
constexpr char OLDCFG_LVL_HISTO_SYSTEM[] = "System";
constexpr char OLDCFG_LVL_HISTO_NODE[] = "Node";
constexpr char OLDCFG_LVL_HISTO_CPU[] = "CPU";

constexpr char OLDCFG_LVL_TOPCOMPOSE[] =       "topcompose";
constexpr char OLDCFG_LVL_TOPCOMPOSE1[] =      "topcompose1";
constexpr char CFG_LVL_EXTRATOPCOMPOSE1[] =    "extratopcompose1";
constexpr char OLDCFG_LVL_TOPCOMPOSE2[] =      "topcompose2";
constexpr char OLDCFG_LVL_COMPOSE_WORKLOAD[] = "compose_workload";
constexpr char OLDCFG_LVL_COMPOSE_APPL[] =     "compose_appl";
constexpr char OLDCFG_LVL_COMPOSE_TASK[] =     "compose_task";
constexpr char OLDCFG_LVL_COMPOSE_THREAD[] =   "compose_thread";
constexpr char OLDCFG_LVL_COMPOSE_CPU[] =      "compose_cpu";
constexpr char OLDCFG_LVL_COMPOSE_NODE[] =     "compose_node";
constexpr char OLDCFG_LVL_COMPOSE_SYSTEM[] =   "compose_system";
constexpr char OLDCFG_LVL_COMPOSE1[] =         "compose1";
constexpr char OLDCFG_LVL_COMPOSE2[] =         "compose2";

// TIMELINE CFGs TAGS
constexpr char OLDCFG_TAG_WNDW_NAME[] =      "window_name";
constexpr char OLDCFG_TAG_WNDW_TYPE[] =      "window_type";
constexpr char OLDCFG_TAG_WNDW_ID[] =        "window_id";
constexpr char OLDCFG_TAG_WNDW_FACTORS[] =   "window_factors";
constexpr char CFG_TAG_WNDW_SHIFTS[] =       "window_shifts";
constexpr char OLDCFG_TAG_WNDW_POSX[] =      "window_position_x";
constexpr char OLDCFG_TAG_WNDW_POSY[] =      "window_position_y";
constexpr char OLDCFG_TAG_WNDW_WIDTH[] =      "window_width";
constexpr char OLDCFG_TAG_WNDW_HEIGHT[] =      "window_height";
constexpr char OLDCFG_TAG_WNDW_COMM_LINES[] =  "window_comm_lines_enabled";
constexpr char OLDCFG_TAG_WNDW_FLAGS_ENABLED[] =  "window_flags_enabled";
constexpr char OLDCFG_TAG_WNDW_NON_COLOR_MODE[] =  "window_noncolor_mode";
constexpr char OLDCFG_TAG_WNDW_COLOR_MODE[] =  "window_color_mode";
constexpr char CFG_TAG_WNDW_GRADIENT_FUNCTION[] =  "window_gradient_function";
constexpr char OLDCFG_TAG_WNDW_CUSTOM_COLOR_ENABLED[] =  "window_custom_color_enabled";
constexpr char OLDCFG_TAG_WNDW_CUSTOM_COLOR_PALETTE[] =  "window_custom_color_palette";
constexpr char OLDCFG_TAG_WNDW_SEMANTIC_SCALE_MIN_AT_ZERO[] =  "window_semantic_scale_min_at_zero";
constexpr char OLDCFG_TAG_WNDW_UNITS[] =     "window_units";
constexpr char OLDCFG_TAG_WNDW_OPERATION[] = "window_operation";
constexpr char OLDCFG_TAG_WNDW_MAXIMUM_Y[] = "window_maximum_y";
constexpr char OLDCFG_TAG_WNDW_MINIMUM_Y[] = "window_minimum_y";
constexpr char OLDCFG_TAG_WNDW_COMPUTE_Y_MAX[] = "window_compute_y_max";
constexpr char OLDCFG_TAG_WNDW_LEVEL[] =     "window_level";
constexpr char OLDCFG_TAG_WNDW_SCALE_RELATIVE[] = "window_scale_relative";
constexpr char OLDCFG_TAG_WNDW_OBJECT[] = "window_object";
constexpr char OLDCFG_TAG_WNDW_IDENTIFIERS[] = "window_identifiers";
constexpr char OLDCFG_TAG_WNDW_ZOOM_OBJECTS[] = "window_zoom_objects";
constexpr char OLDCFG_TAG_WNDW_BEGIN_TIME[] = "window_begin_time";
constexpr char OLDCFG_TAG_WNDW_STOP_TIME[] = "window_stop_time";
constexpr char OLDCFG_TAG_WNDW_END_TIME[] = "window_end_time";
constexpr char OLDCFG_TAG_WNDW_BEGIN_TIME_RELATIVE[] = "window_begin_time_relative";
constexpr char OLDCFG_TAG_WNDW_NUMBER_OF_ROWS[] = "window_number_of_row";
constexpr char OLDCFG_TAG_WNDW_SELECTED_FUNCTIONS[] = "window_selected_functions";
constexpr char OLDCFG_TAG_WNDW_SEMANTIC_MODULE[] = "window_semantic_module";
constexpr char OLDCFG_TAG_WNDW_COMPOSE_FUNCTIONS[] = "window_compose_functions";
constexpr char OLDCFG_TAG_WNDW_FILTER_MODULE[] = "window_filter_module";
constexpr char OLDCFG_TAG_WNDW_LOGICAL_FILTERED[] = "window_logical_filtered";
constexpr char OLDCFG_TAG_WNDW_PHYSICAL_FILTERED[] = "window_physical_filtered";
constexpr char OLDCFG_TAG_WNDW_FROMTO[] = "window_comm_fromto";
constexpr char OLDCFG_TAG_WNDW_COMM_TAGSIZE[] = "window_comm_tagsize";
constexpr char OLDCFG_TAG_WNDW_TYPEVAL[] = "window_comm_typeval";
constexpr char OLDCFG_TAG_WNDW_OPEN[] = "window_open";
constexpr char OLDCFG_TAG_WNDW_DRAW_MODE[] = "window_drawmode";
constexpr char OLDCFG_TAG_WNDW_DRAW_MODE_ROWS[] = "window_drawmode_rows";
constexpr char OLDCFG_TAG_WNDW_PIXEL_SIZE[] = "window_pixel_size";
constexpr char OLDCFG_TAG_WNDW_LABELS_TO_DRAW[] = "window_labels_to_draw";
constexpr char OLDCFG_TAG_WNDW_PUNCTUAL_COLOR_WIN[] = "window_punctual_color_window";
constexpr char OLDCFG_TAG_WNDW_SYNCHRONIZE[] = "window_synchronize";

// HISTOGRAM CFGs TAGS
constexpr char OLDCFG_TAG_AN2D_NEW[] = "< NEW ANALYZER2D >";
constexpr char OLDCFG_TAG_AN2D_NAME[] = "Analyzer2D.Name:";
constexpr char OLDCFG_TAG_AN2D_X[] = "Analyzer2D.X:";
constexpr char OLDCFG_TAG_AN2D_Y[] = "Analyzer2D.Y:";
constexpr char OLDCFG_TAG_AN2D_WIDTH[] = "Analyzer2D.Width:";
constexpr char OLDCFG_TAG_AN2D_HEIGHT[] = "Analyzer2D.Height:";
constexpr char OLDCFG_TAG_AN2D_CONTROL_WINDOW[] = "Analyzer2D.ControlWindow:";
constexpr char OLDCFG_TAG_AN2D_DATA_WINDOW[] = "Analyzer2D.DataWindow:";
constexpr char OLDCFG_TAG_AN2D_STATISTIC[] = "Analyzer2D.Statistic:";
constexpr char OLDCFG_TAG_AN2D_CALCULATE_ALL[] = "Analyzer2D.CalculateAll:";
constexpr char OLDCFG_TAG_AN2D_NUM_COLUMNS[] = "Analyzer2D.num_columns:";
constexpr char OLDCFG_TAG_AN2D_HIDE_COLS[] = "Analyzer2D.HideCols:";
constexpr char OLDCFG_TAG_AN2D_SCIENTIFIC_NOTATION[] = "Analyzer2D.scientific_notation:";
constexpr char OLDCFG_TAG_AN2D_NUM_DECIMALS[] = "Analyzer2D.num_decimals:";
constexpr char OLDCFG_TAG_AN2D_THOUSANDSEP[] = "Analyzer2D.thousandsep:";
constexpr char OLDCFG_TAG_AN2D_UNITS[] = "Analyzer2D.units:";
constexpr char OLDCFG_TAG_AN2D_HORIZONTAL[] = "Analyzer2D.HorizVert:";
constexpr char OLDCFG_TAG_AN2D_COLOR[] = "Analyzer2D.Color:";
constexpr char OLDCFG_TAG_AN2D_SEMANTIC_COLOR[] = "Analyzer2D.SemanticColor:";
constexpr char OLDCFG_TAG_AN2D_ZOOM[] = "Analyzer2D.Zoom:";
constexpr char OLDCFG_TAG_AN2D_ACCUMULATOR[] = "Analyzer2D.Accumulator:";
constexpr char OLDCFG_TAG_AN2D_ACCUM_BY_CTRL_WINDOW[] = "Analyzer2D.AccumulateByControlWindow:";
constexpr char OLDCFG_TAG_AN2D_SORTCOLS[] = "Analyzer2D.SortCols:";
constexpr char OLDCFG_TAG_AN2D_SORTCRITERIA[] = "Analyzer2D.SortCriteria:";
constexpr char OLDCFG_TAG_AN2D_SORTREVERSE[] = "Analyzer2D.SortReverse:";
constexpr char OLDCFG_TAG_AN2D_PARAMETERS[] = "Analyzer2D.Parameters:";
constexpr char OLDCFG_TAG_AN2D_ANALYSISLIMITS[] = "Analyzer2D.AnalysisLimits:";
constexpr char OLDCFG_TAG_AN2D_RELATIVETIME[] = "Analyzer2D.RelativeTime:";
constexpr char OLDCFG_TAG_AN2D_COMPUTEYSCALE[] = "Analyzer2D.ComputeYScale:";
constexpr char OLDCFG_TAG_AN2D_MINIMUM[] = "Analyzer2D.Minimum:";
constexpr char OLDCFG_TAG_AN2D_MAXIMUM[] = "Analyzer2D.Maximum:";
constexpr char OLDCFG_TAG_AN2D_DELTA[] = "Analyzer2D.Delta:";
constexpr char OLDCFG_TAG_AN2D_COMPUTEGRADIENT[] = "Analyzer2D.ComputeGradient:";
constexpr char OLDCFG_TAG_AN2D_MINIMUMGRADIENT[] = "Analyzer2D.MinimumGradient:";
constexpr char OLDCFG_TAG_AN2D_MAXIMUMGRADIENT[] = "Analyzer2D.MaximumGradient:";
constexpr char OLDCFG_TAG_AN2D_PIXEL_SIZE[] = "Analyzer2D.PixelSize:";
constexpr char OLDCFG_TAG_AN2D_CODE_COLOR[] = "Analyzer2D.CodeColor:";
constexpr char OLDCFG_TAG_AN2D_COLOR_MODE[] = "Analyzer2D.ColorMode:";
constexpr char OLDCFG_TAG_AN2D_ONLY_TOTALS[] = "Analyzer2D.ShowOnlyTotals:";
constexpr char OLDCFG_TAG_AN2D_SHORT_LABELS[] = "Analyzer2D.ShortHeaderLabels:";
constexpr char OLDCFG_TAG_AN2D_SYNCHRONIZE[] = "Analyzer2D.Synchronize:";
constexpr char OLDCFG_TAG_AN3D_CONTROLWINDOW[] = "Analyzer2D.3D_ControlWindow:";
constexpr char OLDCFG_TAG_AN3D_COMPUTEYSCALE[] = "Analyzer3D.ComputeYScale:";
constexpr char OLDCFG_TAG_AN3D_MINIMUM[] = "Analyzer2D.3D_Minimum:";
constexpr char OLDCFG_TAG_AN3D_MAXIMUM[] = "Analyzer2D.3D_Maximum:";
constexpr char OLDCFG_TAG_AN3D_DELTA[] = "Analyzer2D.3D_Delta:";
constexpr char OLDCFG_TAG_AN3D_FIXEDVALUE[] = "Analyzer2D.3D_FixedValue:";

constexpr char OLDCFG_VAL_DRAW_MODE_LAST[] =        "0";
constexpr char OLDCFG_VAL_DRAW_MODE_MAXIMUM[] =     "1";
constexpr char OLDCFG_VAL_DRAW_MODE_MINNOT0[] =     "2";
constexpr char OLDCFG_VAL_DRAW_MODE_RANDOM[] =      "3";
constexpr char OLDCFG_VAL_DRAW_MODE_RANDOMNOT0[] =  "4";
constexpr char OLDCFG_VAL_DRAW_MODE_AVERAGE[] =     "5";
constexpr char OLDCFG_VAL_DRAW_MODE_AVERAGENOT0[] = "6";
constexpr char OLDCFG_VAL_DRAW_MODE_MODE[] =        "7";

constexpr char OLDCFG_VAL_COLOR_MODE_CODE[] =              "window_in_code_mode";
constexpr char OLDCFG_VAL_COLOR_MODE_GRADIENT[] =          "window_in_gradient_mode";
constexpr char OLDCFG_VAL_COLOR_MODE_NULL_GRADIENT[] =     "window_in_null_gradient_mode";
constexpr char CFG_VAL_COLOR_MODE_ALTERNATIVE_GRADIENT[] = "window_in_alternative_gradient_mode";
constexpr char CFG_VAL_COLOR_MODE_PUNCTUAL[] =             "window_in_punctual_mode";
constexpr char CFG_VAL_COLOR_MODE_FUSED_LINES[] =          "window_in_fused_lines_mode";

constexpr char CFG_VAL_GRADIENT_FUNCTION_LINEAR[] =        "gradient_function_linear";
constexpr char CFG_VAL_GRADIENT_FUNCTION_STEPS[] =         "gradient_function_steps";
constexpr char CFG_VAL_GRADIENT_FUNCTION_LOG[] =           "gradient_function_log";
constexpr char CFG_VAL_GRADIENT_FUNCTION_EXP[] =           "gradient_function_exp";

constexpr char OLDCFG_VAL_TIMEUNIT_NS[] = "Nanoseconds";
constexpr char OLDCFG_VAL_TIMEUNIT_US[] = "Microseconds";
constexpr char OLDCFG_VAL_TIMEUNIT_MS[] = "Milliseconds";
constexpr char OLDCFG_VAL_TIMEUNIT_S[] =  "Seconds";
constexpr char OLDCFG_VAL_TIMEUNIT_M[] =  "Minutes";
constexpr char OLDCFG_VAL_TIMEUNIT_H[] =  "Hours";
constexpr char OLDCFG_VAL_TIMEUNIT_D[] =  "Days";

constexpr char OLDCFG_VAL_FILTER_OBJ_FROM[] =     "from_obj";
constexpr char OLDCFG_VAL_FILTER_OBJ_TO[] =       "to_obj";
constexpr char OLDCFG_VAL_FILTER_COM_TAG[] =      "tag_msg";
constexpr char OLDCFG_VAL_FILTER_COM_SIZE[] =     "size_msg";
constexpr char OLDCFG_VAL_FILTER_COM_BW[] =       "bw_msg";
constexpr char OLDCFG_VAL_FILTER_EVT_TYPE[] =     "evt_type";
constexpr char OLDCFG_VAL_FILTER_EVT_VALUE[] =    "evt_value";
constexpr char CFG_VAL_FILTER_EVT_TYPE_LABEL[] =  "evt_type_label";
constexpr char CFG_VAL_FILTER_EVT_VALUE_LABEL[] = "evt_value_label";

constexpr char CFG_VAL_FILTER_FUNCTION_RANGE[]  = "[x,y]";

constexpr char OLDCFG_VAL_TRUE[] =   "true";
constexpr char OLDCFG_VAL_TRUE2[] =  "True";
constexpr char OLDCFG_VAL_FALSE[] =  "false";
constexpr char OLDCFG_VAL_FALSE2[] = "False";

constexpr char OLDCFG_VAL_ENABLED[] = "Enabled";
constexpr char OLDCFG_VAL_DISABLED[] = "Disabled";

constexpr char OLDCFG_VAL_HORIZONTAL[] = "Horizontal";
constexpr char OLDCFG_VAL_VERTICAL[] =   "Vertical";

constexpr char OLDCFG_VAL_SORT_AVERAGE[] = "Average";
constexpr char OLDCFG_VAL_SORT_TOTAL[] =   "Total";
constexpr char OLDCFG_VAL_SORT_MAXIMUM[] = "Maximum";
constexpr char OLDCFG_VAL_SORT_MINIMUM[] = "Minimum";
constexpr char OLDCFG_VAL_SORT_STDEV[] =   "Stdev";
constexpr char OLDCFG_VAL_SORT_AVGDIVMAX[] = "Avg/Max";
constexpr char OLDCFG_VAL_SORT_CUSTOM[] = "Custom";

constexpr char OLDCFG_VAL_WNDW_TYPE_SINGLE[] =   "single";
constexpr char OLDCFG_VAL_WNDW_TYPE_COMPOSED[] = "composed";

constexpr char OLDCFG_VAL_LIMIT_ALLTRACE[] =  "Alltrace";
constexpr char OLDCFG_VAL_LIMIT_ALLWINDOW[] = "Allwindow";
constexpr char OLDCFG_VAL_LIMIT_REGION[] =    "Region";

constexpr char OLDCFG_VAL_AN2D_ACCUM_SEMANTIC[] = "Semantic";

constexpr char CFG_TAG_ALIAS_CFG4D[] = "cfg4d";
constexpr char CFG_TAG_LINK_CFG4D[] = "cfg4dlink";
constexpr char CFG_TAG_STATISTIC_ALIAS_CFG4D[] = "cfg4dstatistic";
constexpr char CFG_TAG_PARAM_ALIAS_CFG4D[] = "cfg4dparam";
constexpr char CFG_TAG_CFG4D_ENABLED[] = "CFG4D_ENABLED";

constexpr char CFG_TAG_OBJECTS[] = "Analyzer2D.Objects:";
constexpr char CFG_TAG_DRAWMODE_OBJECTS[] = "Analyzer2D.DrawModeObjects:";
constexpr char CFG_TAG_DRAWMODE_COLUMNS[] = "Analyzer2D.DrawModeColumns:";
constexpr char CFG_TAG_AN2D_COMPUTEYSCALE_ZERO[] = "Analyzer2D.ComputeYScaleZero:";
constexpr char CFG_TAG_AN2D_NUMCOLUMNS[] = "Analyzer2D.NumColumns:";
constexpr char CFG_TAG_AN2D_USE_CUSTOM_DELTA[] = "Analyzer2D.UseCustomDelta:";
constexpr char CFG_TAG_AN2D_USE_FIXED_DELTA[] = "Analyzer2D.UseFixedDelta:";

static const std::string LABEL_TIMEUNIT[ DAY + 1 ] =
{
  "ns",
  "us",
  "ms",
  "s",
  "m",
  "h",
  "d"
};

static const std::string FULL_LABEL_TIMEUNIT[ DAY + 1 ] =
{
  "Nanoseconds",
  "Microseconds",
  "Milliseconds",
  "Seconds",
  "Minutes",
  "Hours",
  "Days"
};

// New cfg label definitions
constexpr char CFG_SHEBANG[] =                    "#ParaverCFG";
constexpr char CFG_HEADER_VERSION[] =             "ConfigFile.Version:";
constexpr char CFG_HEADER_NUM_WINDOWS[] =         "ConfigFile.NumWindows:";
constexpr char CFG_HEADER_BEGIN_DESCRIPTION[] =   "ConfigFile.BeginDescription";
constexpr char CFG_HEADER_END_DESCRIPTION[] =     "ConfigFile.EndDescription";

constexpr char CFG_TAG_WNDW_END_TIME_RELATIVE[] = "window_end_time_relative";

// GUI Misc Labels
constexpr char GUI_COLORS_CODE_COLOR[] =                  "Code Color";
constexpr char GUI_COLORS_GRADIENT_COLOR[] =              "Gradient Color";
constexpr char GUI_COLORS_NOT_NULL_GRADIENT[] =           "Not Null Gradient";
constexpr char GUI_COLORS_FUNCTION_LINE[] =               "Function Line";
constexpr char GUI_COLORS_PUNCTUAL[] =                    "Punctual";
constexpr char GUI_GRADIENT_FUNCTION_LINEAR[] =           "Linear";
constexpr char GUI_GRADIENT_FUNCTION_STEPS[] =            "Steps";
constexpr char GUI_GRADIENT_FUNCTION_LOGARITHMIC[] =      "Logarithmic";
constexpr char GUI_GRADIENT_FUNCTION_EXPONENTIAL[] =      "Exponential";
constexpr char GUI_DRAWMODE_LAST[] =                      "Last";
constexpr char GUI_DRAWMODE_MAXIMUM[] =                   "Maximum";
constexpr char GUI_DRAWMODE_MINIMUM_NOT_ZERO[] =          "Minimum not zero";
constexpr char GUI_DRAWMODE_ABSOLUTE_MAXIMUM[] =          "Maximum abs.";
constexpr char GUI_DRAWMODE_ABSOLUTE_MINIMUM_NOT_ZERO[] = "Minimum abs. not zero";
constexpr char GUI_DRAWMODE_RANDOM[] =                    "Random";
constexpr char GUI_DRAWMODE_RANDOM_NOT_ZERO[] =           "Random not zero";
constexpr char GUI_DRAWMODE_AVERAGE[] =                   "Average";
constexpr char GUI_DRAWMODE_AVERAGE_NOT_ZERO[] =          "Average not zero";
constexpr char GUI_DRAWMODE_MODE[] =                      "Mode";
constexpr char GUI_PIXEL_SIZE_X1[] =                      "x1";
constexpr char GUI_PIXEL_SIZE_X2[] =                      "x2";
constexpr char GUI_PIXEL_SIZE_X4[] =                      "x4";
constexpr char GUI_PIXEL_SIZE_X8[] =                      "x8";
constexpr char GUI_IMAGE_FORMAT_BMP[] =                   "BMP";
constexpr char GUI_IMAGE_FORMAT_JPEG[] =                  "JPEG";
constexpr char GUI_IMAGE_FORMAT_PNG[] =                   "PNG";
constexpr char GUI_IMAGE_FORMAT_XPM[] =                   "XPM";
constexpr char GUI_TEXT_FORMAT_CSV[] =                    "CSV";
constexpr char GUI_TEXT_FORMAT_GNUPLOT[] =                "GNUPlot";
constexpr char GUI_TEXT_FORMAT_PLAIN[] =                  "Plain";
constexpr char GUI_OBJECT_LABELS_ALL[] =                  "All";
constexpr char GUI_OBJECT_LABELS_SPACED[] =               "Spaced";
constexpr char GUI_OBJECT_LABELS_POWER2[] =               "2^n";
constexpr char GUI_OBJECT_AXIS_CURRENT[] =                "Fit Current Level";
constexpr char GUI_OBJECT_AXIS_ALL[] =                    "Fit All Levels";
constexpr char GUI_OBJECT_AXIS_ZERO[] =                   "0%";
constexpr char GUI_OBJECT_AXIS_FIVE[] =                   "5%";
constexpr char GUI_OBJECT_AXIS_TEN[] =                    "10%";
constexpr char GUI_OBJECT_AXIS_TWENTYFIVE[] =             "25%";


// GUI Semantic Levels - INDEXED WITH TWindowLevel
static const std::string TimelineLevelLabels[ DERIVED + 1 ] =
{
  "None",
  "Workload",
  "Application",
  "Task",
  "Thread",
  "System",
  "Node",
  "CPU",
  "Top Compose 1",
  "Top Compose 2",
  "Compose Workload",
  "Compose Appl",
  "Compose Task",
  "Compose Thread",
  "Compose System",
  "Compose Node",
  "Compose CPU",
  "Derived"
};


// GUI Single Timeline property labels
enum TSingleTimelineProperties
{
  SINGLE_NAME = 0,
  SINGLE_BEGINTIME,
  SINGLE_ENDTIME,
  SINGLE_SEMANTICMINIMUM,
  SINGLE_SEMANTICMAXIMUM,
  SINGLE_LEVEL,
  SINGLE_TIMEUNIT,
  SINGLE_COMMLOGICAL,
  SINGLE_COMMPHYSICAL,
  SINGLE_COMMINTRA,
  SINGLE_COMMINTER,
  SINGLE_COMMFROMFUNCTION,
  SINGLE_COMMFROMVALUES,
  SINGLE_COMMFROMTOOP,
  SINGLE_COMMTOFUNCTION,
  SINGLE_COMMTOVALUES,
  SINGLE_COMMTAGFUNCTION,
  SINGLE_COMMTAGVALUES,
  SINGLE_COMMTAGSIZEOP,
  SINGLE_COMMSIZEFUNCTION,
  SINGLE_COMMSIZEVALUES,
  SINGLE_COMMBANDWIDTHFUNCTION,
  SINGLE_COMMBANDWIDTHVALUES,
  SINGLE_EVENTTYPEFUNCTION,
  SINGLE_EVENTTYPEVALUES,
  SINGLE_EVENTTYPEVALUESOP,
  SINGLE_EVENTVALUEFUNCTION,
  SINGLE_EVENTVALUEVALUES,
  SINGLE_TOPCOMPOSE1,
  SINGLE_TOPCOMPOSE2,
  SINGLE_COMPOSEWORKLOAD,
  SINGLE_WORKLOAD,
  SINGLE_COMPOSEAPPL,
  SINGLE_APPLICATION,
  SINGLE_COMPOSETASK,
  SINGLE_TASK,
  SINGLE_COMPOSETHREAD,
  SINGLE_THREAD,
  SINGLE_COMPOSESYSTEM,
  SINGLE_SYSTEM,
  SINGLE_COMPOSENODE,
  SINGLE_NODE,
  SINGLE_COMPOSECPU,
  SINGLE_CPU,

  TOTAL_SINGLE_PROPERTIES,

// Dynamic properties
  SINGLE_EXTRATOPCOMPOSE1,
  SINGLE_FUNCTIONPARAMETERS,
  SINGLE_EXTRAFUNCTIONPARAMETERS,

  SINGLE_NULL
};

static const std::string SingleTimelinePropertyLabels[ SINGLE_NULL ] =
{
  "Name",
  "Begin time",
  "End time",
  "Semantic Minimum",
  "Semantic Maximum",
  "Level",
  "Time unit",
  "Logical",
  "Physical",
  "Intra Node Comms",
  "Inter Node Comms",
  "Comm.From.Function",
  "Comm.From.From",
  "From/To Op",
  "Comm.To.Function",
  "Comm.To.To",
  "Comm.Tag.Function",
  "Comm.Tag.Tag",
  "Tag/Size Op",
  "Comm.Size.Function",
  "Comm.Size.Size",
  "Comm.Bandwidth.Function",
  "Comm.Bandwidth.Bandwidth",
  "Event.Type.Function",
  "Event.Type.Types",
  "Type/Value Op",
  "Event.Value.Function",
  "Event.Value.Values",
  "Top Compose 1",
  "Top Compose 2",
  "Compose Workload",
  "Workload",
  "Compose Appl",
  "Application",
  "Compose Task",
  "Task",
  "Compose Thread",
  "Thread",
  "Compose System",
  "System",
  "Compose Node",
  "Node",
  "Compose CPU",
  "CPU",
  "-----------TOTAL_SINGLE_PROPERTIES-----------",
  "Extra Top Compose 1",
  "Parameters",
  "Extra Parameters"
};

// GUI Derived Timeline property labels
enum TDerivedTimelineProperties
{
  DERIVED_NAME = 0,
  DERIVED_BEGINTIME,
  DERIVED_ENDTIME,
  DERIVED_SEMANTICMINIMUM,
  DERIVED_SEMANTICMAXIMUM,
  DERIVED_LEVEL,
  DERIVED_TIMEUNIT,
  DERIVED_TOPCOMPOSE1,
  DERIVED_TOPCOMPOSE2,
  DERIVED_COMPOSEWORKLOAD,
  DERIVED_WORKLOAD,
  DERIVED_COMPOSEAPPL,
  DERIVED_APPLICATION,
  DERIVED_COMPOSETASK,
  DERIVED_TASK,
  DERIVED_COMPOSETHREAD,
  DERIVED_COMPOSESYSTEM,
  DERIVED_SYSTEM,
  DERIVED_COMPOSENODE,
  DERIVED_NODE,
  DERIVED_COMPOSECPU,
  DERIVED_SHIFT1,
  DERIVED_FACTOR1,
  DERIVED_DERIVED,
  DERIVED_FACTOR2,
  DERIVED_SHIFT2,

  TOTAL_DERIVED_PROPERTIES,

// Dynamic properties
  DERIVED_EXTRATOPCOMPOSE1,
  DERIVED_FUNCTIONPARAMETERS,
  DERIVED_EXTRAFUNCTIONPARAMETERS,

  DERIVED_NULL
};

static const std::string DerivedTimelinePropertyLabels[ DERIVED_NULL ] =
{
  "Name",
  "Begin time",
  "End time",
  "Semantic Minimum",
  "Semantic Maximum",
  "Level",
  "Time unit",
  "Top Compose 1",
  "Top Compose 2",
  "Compose Workload",
  "Workload",
  "Compose Appl",
  "Application",
  "Compose Task",
  "Task",
  "Compose Thread",
  "Compose System",
  "System",
  "Compose Node",
  "Node",
  "Compose CPU",
  "Shift #1",
  "Factor #1",
  "Derived",
  "Factor #2",
  "Shift #2",
  "-----------TOTAL_DERIVED_PROPERTIES-----------",
  "Extra Top Compose 1",
  "Parameters",
  "Extra Parameters"
};

// GUI Histogram property labels
enum THistogramProperties
{
  HISTOGRAM_NAME = 0,
  HISTOGRAM_BEGINTIME,
  HISTOGRAM_ENDTIME,
  HISTOGRAM_CONTROLWINDOW,
  HISTOGRAM_CONTROLMINIMUM,
  HISTOGRAM_CONTROLMAXIMUM,
  HISTOGRAM_CONTROLDELTA,
  HISTOGRAM_NUMCOLUMNS,
  HISTOGRAM_TYPE,
  HISTOGRAM_STATISTIC,
  HISTOGRAM_MINIMUMGRADIENT,
  HISTOGRAM_MAXIMUMGRADIENT,
  HISTOGRAM_DATAWINDOW,
  HISTOGRAM_3D3RDWINDOW,
  HISTOGRAM_3DMINIMUM,
  HISTOGRAM_3DMAXIMUM,
  HISTOGRAM_3DDELTA,
  HISTOGRAM_3DPLANE,

  TOTAL_HISTOGRAM_PROPERTIES,
  HISTOGRAM_NULL
};

static const std::string HistogramPropertyLabels[ TOTAL_HISTOGRAM_PROPERTIES ] =
{
  "Name",
  "Begin time",
  "End time",
  "Control.Window",
  "Control.Minimum",
  "Control.Maximum",
  "Control.Delta",
  "Num Columns",
  "Type",
  "Statistic",
  "Minimum Gradient",
  "Maximum Gradient",
  "Data.Window",
  "3D.3rd Window",
  "3D.Minimum",
  "3D.Maximum",
  "3D.Delta",
  "3D.Plane",
};

// DIMEMAS
constexpr char OLD_DIMEMAS_SHEBANG[] = "SDDFA";
constexpr char DIMEMAS_SHEBANG[] = "#DIMEMAS_CONFIGURATION";



