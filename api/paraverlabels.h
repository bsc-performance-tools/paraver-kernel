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


#ifndef PARAVERLABELS_H_INCLUDED
#define PARAVERLABELS_H_INCLUDED

#define CFG_CURRENT_VERSION "3.4"

#define OLDCFG_HEADER_VERSION "version"
#define OLDCFG_HEADER_NUM_WINDOWS "number_of_windows"
#define OLDCFG_HEADER_BEGIN_DESCRIPTION "begin_description"
#define OLDCFG_HEADER_END_DESCRIPTION "end_description"

#define OLDCFG_LVL_APPL     "appl"
#define OLDCFG_LVL_PTASK    "ptask"
#define OLDCFG_LVL_WORKLOAD "workload"
#define OLDCFG_LVL_TASK     "task"
#define OLDCFG_LVL_THREAD   "thread"
#define OLDCFG_LVL_CPU      "cpu"
#define OLDCFG_LVL_NODE     "node"
#define OLDCFG_LVL_SYSTEM    "system"

#define OLDCFG_LVL_HISTO_WORKLOAD "Workload"
#define OLDCFG_LVL_HISTO_APPL "Application"
#define OLDCFG_LVL_HISTO_TASK "Task"
#define OLDCFG_LVL_HISTO_THREAD "Thread"
#define OLDCFG_LVL_HISTO_SYSTEM "System"
#define OLDCFG_LVL_HISTO_NODE "Node"
#define OLDCFG_LVL_HISTO_CPU "CPU"

#define OLDCFG_LVL_TOPCOMPOSE       "topcompose"
#define OLDCFG_LVL_TOPCOMPOSE1      "topcompose1"
#define CFG_LVL_EXTRATOPCOMPOSE1    "extratopcompose1"
#define OLDCFG_LVL_TOPCOMPOSE2      "topcompose2"
#define OLDCFG_LVL_COMPOSE_WORKLOAD "compose_workload"
#define OLDCFG_LVL_COMPOSE_APPL     "compose_appl"
#define OLDCFG_LVL_COMPOSE_TASK     "compose_task"
#define OLDCFG_LVL_COMPOSE_THREAD   "compose_thread"
#define OLDCFG_LVL_COMPOSE_CPU      "compose_cpu"
#define OLDCFG_LVL_COMPOSE_NODE     "compose_node"
#define OLDCFG_LVL_COMPOSE_SYSTEM   "compose_system"
#define OLDCFG_LVL_COMPOSE1         "compose1"
#define OLDCFG_LVL_COMPOSE2         "compose2"

// TIMELINE CFGs TAGS
#define OLDCFG_TAG_WNDW_NAME      "window_name"
#define OLDCFG_TAG_WNDW_TYPE      "window_type"
#define OLDCFG_TAG_WNDW_ID        "window_id"
#define OLDCFG_TAG_WNDW_FACTORS   "window_factors"
#define CFG_TAG_WNDW_SHIFTS       "window_shifts"
#define OLDCFG_TAG_WNDW_POSX      "window_position_x"
#define OLDCFG_TAG_WNDW_POSY      "window_position_y"
#define OLDCFG_TAG_WNDW_WIDTH      "window_width"
#define OLDCFG_TAG_WNDW_HEIGHT      "window_height"
#define OLDCFG_TAG_WNDW_COMM_LINES  "window_comm_lines_enabled"
#define OLDCFG_TAG_WNDW_FLAGS_ENABLED  "window_flags_enabled"
#define OLDCFG_TAG_WNDW_NON_COLOR_MODE  "window_noncolor_mode"
#define OLDCFG_TAG_WNDW_COLOR_MODE  "window_color_mode"
#define OLDCFG_TAG_WNDW_CUSTOM_COLOR_ENABLED  "window_custom_color_enabled"
#define OLDCFG_TAG_WNDW_CUSTOM_COLOR_PALETTE  "window_custom_color_palette"
#define OLDCFG_TAG_WNDW_SEMANTIC_SCALE_MIN_AT_ZERO  "window_semantic_scale_min_at_zero"
#define OLDCFG_TAG_WNDW_UNITS     "window_units"
#define OLDCFG_TAG_WNDW_OPERATION "window_operation"
#define OLDCFG_TAG_WNDW_MAXIMUM_Y "window_maximum_y"
#define OLDCFG_TAG_WNDW_MINIMUM_Y "window_minimum_y"
#define OLDCFG_TAG_WNDW_COMPUTE_Y_MAX "window_compute_y_max"
#define OLDCFG_TAG_WNDW_LEVEL     "window_level"
#define OLDCFG_TAG_WNDW_SCALE_RELATIVE "window_scale_relative"
#define OLDCFG_TAG_WNDW_OBJECT "window_object"
#define OLDCFG_TAG_WNDW_IDENTIFIERS "window_identifiers"
#define OLDCFG_TAG_WNDW_ZOOM_OBJECTS "window_zoom_objects"
#define OLDCFG_TAG_WNDW_BEGIN_TIME "window_begin_time"
#define OLDCFG_TAG_WNDW_STOP_TIME "window_stop_time"
#define OLDCFG_TAG_WNDW_END_TIME "window_end_time"
#define OLDCFG_TAG_WNDW_BEGIN_TIME_RELATIVE "window_begin_time_relative"
#define OLDCFG_TAG_WNDW_NUMBER_OF_ROWS "window_number_of_row"
#define OLDCFG_TAG_WNDW_SELECTED_FUNCTIONS "window_selected_functions"
#define OLDCFG_TAG_WNDW_SEMANTIC_MODULE "window_semantic_module"
#define OLDCFG_TAG_WNDW_COMPOSE_FUNCTIONS "window_compose_functions"
#define OLDCFG_TAG_WNDW_FILTER_MODULE "window_filter_module"
#define OLDCFG_TAG_WNDW_LOGICAL_FILTERED "window_logical_filtered"
#define OLDCFG_TAG_WNDW_PHYSICAL_FILTERED "window_physical_filtered"
#define OLDCFG_TAG_WNDW_FROMTO "window_comm_fromto"
#define OLDCFG_TAG_WNDW_COMM_TAGSIZE "window_comm_tagsize"
#define OLDCFG_TAG_WNDW_TYPEVAL "window_comm_typeval"
#define OLDCFG_TAG_WNDW_OPEN "window_open"
#define OLDCFG_TAG_WNDW_DRAW_MODE "window_drawmode"
#define OLDCFG_TAG_WNDW_DRAW_MODE_ROWS "window_drawmode_rows"
#define OLDCFG_TAG_WNDW_PIXEL_SIZE "window_pixel_size"
#define OLDCFG_TAG_WNDW_LABELS_TO_DRAW "window_labels_to_draw"
#define OLDCFG_TAG_WNDW_PUNCTUAL_COLOR_WIN "window_punctual_color_window"
#define OLDCFG_TAG_WNDW_SYNCHRONIZE "window_synchronize"

// HISTOGRAM CFGs TAGS
#define OLDCFG_TAG_AN2D_NEW "< NEW ANALYZER2D >"
#define OLDCFG_TAG_AN2D_NAME "Analyzer2D.Name:"
#define OLDCFG_TAG_AN2D_X "Analyzer2D.X:"
#define OLDCFG_TAG_AN2D_Y "Analyzer2D.Y:"
#define OLDCFG_TAG_AN2D_WIDTH "Analyzer2D.Width:"
#define OLDCFG_TAG_AN2D_HEIGHT "Analyzer2D.Height:"
#define OLDCFG_TAG_AN2D_CONTROL_WINDOW "Analyzer2D.ControlWindow:"
#define OLDCFG_TAG_AN2D_DATA_WINDOW "Analyzer2D.DataWindow:"
#define OLDCFG_TAG_AN2D_STATISTIC "Analyzer2D.Statistic:"
#define OLDCFG_TAG_AN2D_CALCULATE_ALL "Analyzer2D.CalculateAll:"
#define OLDCFG_TAG_AN2D_NUM_COLUMNS "Analyzer2D.num_columns:"
#define OLDCFG_TAG_AN2D_HIDE_COLS "Analyzer2D.HideCols:"
#define OLDCFG_TAG_AN2D_SCIENTIFIC_NOTATION "Analyzer2D.scientific_notation:"
#define OLDCFG_TAG_AN2D_NUM_DECIMALS "Analyzer2D.num_decimals:"
#define OLDCFG_TAG_AN2D_THOUSANDSEP "Analyzer2D.thousandsep:"
#define OLDCFG_TAG_AN2D_UNITS "Analyzer2D.units:"
#define OLDCFG_TAG_AN2D_HORIZONTAL "Analyzer2D.HorizVert:"
#define OLDCFG_TAG_AN2D_COLOR "Analyzer2D.Color:"
#define OLDCFG_TAG_AN2D_SEMANTIC_COLOR "Analyzer2D.SemanticColor:"
#define OLDCFG_TAG_AN2D_ZOOM "Analyzer2D.Zoom:"
#define OLDCFG_TAG_AN2D_ACCUMULATOR "Analyzer2D.Accumulator:"
#define OLDCFG_TAG_AN2D_ACCUM_BY_CTRL_WINDOW "Analyzer2D.AccumulateByControlWindow:"
#define OLDCFG_TAG_AN2D_SORTCOLS "Analyzer2D.SortCols:"
#define OLDCFG_TAG_AN2D_SORTCRITERIA "Analyzer2D.SortCriteria:"
#define OLDCFG_TAG_AN2D_SORTREVERSE "Analyzer2D.SortReverse:"
#define OLDCFG_TAG_AN2D_PARAMETERS "Analyzer2D.Parameters:"
#define OLDCFG_TAG_AN2D_ANALYSISLIMITS "Analyzer2D.AnalysisLimits:"
#define OLDCFG_TAG_AN2D_RELATIVETIME "Analyzer2D.RelativeTime:"
#define OLDCFG_TAG_AN2D_COMPUTEYSCALE "Analyzer2D.ComputeYScale:"
#define OLDCFG_TAG_AN2D_MINIMUM "Analyzer2D.Minimum:"
#define OLDCFG_TAG_AN2D_MAXIMUM "Analyzer2D.Maximum:"
#define OLDCFG_TAG_AN2D_DELTA "Analyzer2D.Delta:"
#define OLDCFG_TAG_AN2D_COMPUTEGRADIENT "Analyzer2D.ComputeGradient:"
#define OLDCFG_TAG_AN2D_MINIMUMGRADIENT "Analyzer2D.MinimumGradient:"
#define OLDCFG_TAG_AN2D_MAXIMUMGRADIENT "Analyzer2D.MaximumGradient:"
#define OLDCFG_TAG_AN2D_PIXEL_SIZE "Analyzer2D.PixelSize:"
#define OLDCFG_TAG_AN2D_CODE_COLOR "Analyzer2D.CodeColor:"
#define OLDCFG_TAG_AN2D_COLOR_MODE "Analyzer2D.ColorMode:"
#define OLDCFG_TAG_AN2D_ONLY_TOTALS "Analyzer2D.ShowOnlyTotals:"
#define OLDCFG_TAG_AN2D_SHORT_LABELS "Analyzer2D.ShortHeaderLabels:"
#define OLDCFG_TAG_AN2D_SYNCHRONIZE "Analyzer2D.Synchronize:"
#define OLDCFG_TAG_AN3D_CONTROLWINDOW "Analyzer2D.3D_ControlWindow:"
#define OLDCFG_TAG_AN3D_COMPUTEYSCALE "Analyzer3D.ComputeYScale:"
#define OLDCFG_TAG_AN3D_MINIMUM "Analyzer2D.3D_Minimum:"
#define OLDCFG_TAG_AN3D_MAXIMUM "Analyzer2D.3D_Maximum:"
#define OLDCFG_TAG_AN3D_DELTA "Analyzer2D.3D_Delta:"
#define OLDCFG_TAG_AN3D_FIXEDVALUE "Analyzer2D.3D_FixedValue:"

#define OLDCFG_VAL_DRAW_MODE_LAST        "0"
#define OLDCFG_VAL_DRAW_MODE_MAXIMUM     "1"
#define OLDCFG_VAL_DRAW_MODE_MINNOT0     "2"
#define OLDCFG_VAL_DRAW_MODE_RANDOM      "3"
#define OLDCFG_VAL_DRAW_MODE_RANDOMNOT0  "4"
#define OLDCFG_VAL_DRAW_MODE_AVERAGE     "5"
#define OLDCFG_VAL_DRAW_MODE_AVERAGENOT0 "6"
#define OLDCFG_VAL_DRAW_MODE_MODE        "7"

#define OLDCFG_VAL_COLOR_MODE_CODE           "window_in_code_mode"
#define OLDCFG_VAL_COLOR_MODE_GRADIENT       "window_in_gradient_mode"
#define OLDCFG_VAL_COLOR_MODE_NULL_GRADIENT  "window_in_null_gradient_mode"
#define CFG_VAL_COLOR_MODE_PUNCTUAL          "window_in_punctual_mode"
#define CFG_VAL_COLOR_MODE_FUSED_LINES       "window_in_fused_lines_mode"

#define OLDCFG_VAL_TIMEUNIT_NS "Nanoseconds"
#define OLDCFG_VAL_TIMEUNIT_US "Microseconds"
#define OLDCFG_VAL_TIMEUNIT_MS "Milliseconds"
#define OLDCFG_VAL_TIMEUNIT_S  "Seconds"
#define OLDCFG_VAL_TIMEUNIT_M  "Minutes"
#define OLDCFG_VAL_TIMEUNIT_H  "Hours"
#define OLDCFG_VAL_TIMEUNIT_D  "Days"

#define OLDCFG_VAL_FILTER_OBJ_FROM     "from_obj"
#define OLDCFG_VAL_FILTER_OBJ_TO       "to_obj"
#define OLDCFG_VAL_FILTER_COM_TAG      "tag_msg"
#define OLDCFG_VAL_FILTER_COM_SIZE     "size_msg"
#define OLDCFG_VAL_FILTER_COM_BW       "bw_msg"
#define OLDCFG_VAL_FILTER_EVT_TYPE     "evt_type"
#define OLDCFG_VAL_FILTER_EVT_VALUE    "evt_value"
#define CFG_VAL_FILTER_EVT_TYPE_LABEL  "evt_type_label"
#define CFG_VAL_FILTER_EVT_VALUE_LABEL "evt_value_label"

#define OLDCFG_VAL_TRUE   "true"
#define OLDCFG_VAL_TRUE2  "True"
#define OLDCFG_VAL_FALSE  "false"
#define OLDCFG_VAL_FALSE2 "False"

#define OLDCFG_VAL_ENABLED "Enabled"
#define OLDCFG_VAL_DISABLED "Disabled"

#define OLDCFG_VAL_HORIZONTAL "Horizontal"
#define OLDCFG_VAL_VERTICAL   "Vertical"

#define OLDCFG_VAL_SORT_AVERAGE "Average"
#define OLDCFG_VAL_SORT_TOTAL   "Total"
#define OLDCFG_VAL_SORT_MAXIMUM "Maximum"
#define OLDCFG_VAL_SORT_MINIMUM "Minimum"
#define OLDCFG_VAL_SORT_STDEV   "Stdev"
#define OLDCFG_VAL_SORT_AVGDIVMAX "Avg/Max"

#define OLDCFG_VAL_WNDW_TYPE_SINGLE   "single"
#define OLDCFG_VAL_WNDW_TYPE_COMPOSED "composed"

#define OLDCFG_VAL_LIMIT_ALLTRACE  "Alltrace"
#define OLDCFG_VAL_LIMIT_ALLWINDOW "Allwindow"
#define OLDCFG_VAL_LIMIT_REGION    "Region"

#define OLDCFG_VAL_AN2D_ACCUM_SEMANTIC "Semantic"

#define CFG_TAG_ALIAS_CFG4D "cfg4d"
#define CFG_TAG_LINK_CFG4D "cfg4dlink"
#define CFG_TAG_STATISTIC_ALIAS_CFG4D "cfg4dstatistic"
#define CFG_TAG_PARAM_ALIAS_CFG4D "cfg4dparam"
#define CFG_TAG_CFG4D_ENABLED "CFG4D_ENABLED"

#define CFG_TAG_OBJECTS          "Analyzer2D.Objects:"
#define CFG_TAG_DRAWMODE_OBJECTS "Analyzer2D.DrawModeObjects:"
#define CFG_TAG_DRAWMODE_COLUMNS "Analyzer2D.DrawModeColumns:"
#define CFG_TAG_AN2D_COMPUTEYSCALE_ZERO "Analyzer2D.ComputeYScaleZero:"


// STRING CONSTRUCTION LABELS FOR THE GUI
#define LEVEL_WORKLOAD     "WORKLOAD"
#define LEVEL_APPLICATION  "APPL"
#define LEVEL_TASK         "TASK"
#define LEVEL_THREAD       "THREAD"
#define LEVEL_SYSTEM       "SYSTEM"
#define LEVEL_NODE         "NODE"
#define LEVEL_CPU          "CPU"

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
#define CFG_SHEBANG                    "#ParaverCFG"
#define CFG_HEADER_VERSION             "ConfigFile.Version:"
#define CFG_HEADER_NUM_WINDOWS         "ConfigFile.NumWindows:"
#define CFG_HEADER_BEGIN_DESCRIPTION   "ConfigFile.BeginDescription"
#define CFG_HEADER_END_DESCRIPTION     "ConfigFile.EndDescription"

#define CFG_TAG_WNDW_END_TIME_RELATIVE "window_end_time_relative"

// GUI Misc Labels
#define GUI_COLORS_CODE_COLOR                  "Code Color"
#define GUI_COLORS_GRADIENT_COLOR              "Gradient Color"
#define GUI_COLORS_NOT_NULL_GRADIENT           "Not Null Gradient"
#define GUI_COLORS_FUNCTION_LINE               "Function Line"
#define GUI_COLORS_PUNCTUAL                    "Punctual"
#define GUI_GRADIENT_FUNCTION_LINEAR           "Linear"
#define GUI_GRADIENT_FUNCTION_STEPS            "Steps"
#define GUI_GRADIENT_FUNCTION_LOGARITHMIC      "Logarithmic"
#define GUI_GRADIENT_FUNCTION_EXPONENTIAL      "Exponential"
#define GUI_DRAWMODE_LAST                      "Last"
#define GUI_DRAWMODE_MAXIMUM                   "Maximum"
#define GUI_DRAWMODE_MINIMUM_NOT_ZERO          "Minimum not zero"
#define GUI_DRAWMODE_ABSOLUTE_MAXIMUM          "Maximum abs."
#define GUI_DRAWMODE_ABSOLUTE_MINIMUM_NOT_ZERO "Minimum abs. not zero"
#define GUI_DRAWMODE_RANDOM                    "Random"
#define GUI_DRAWMODE_RANDOM_NOT_ZERO           "Random not zero"
#define GUI_DRAWMODE_AVERAGE                   "Average"
#define GUI_DRAWMODE_AVERAGE_NOT_ZERO          "Average not zero"
#define GUI_DRAWMODE_MODE                      "Mode"
#define GUI_PIXEL_SIZE_X1                      "x1"
#define GUI_PIXEL_SIZE_X2                      "x2"
#define GUI_PIXEL_SIZE_X4                      "x4"
#define GUI_PIXEL_SIZE_X8                      "x8"
#define GUI_IMAGE_FORMAT_BMP                   "BMP"
#define GUI_IMAGE_FORMAT_JPEG                  "JPEG"
#define GUI_IMAGE_FORMAT_PNG                   "PNG"
#define GUI_IMAGE_FORMAT_XPM                   "XPM"
#define GUI_TEXT_FORMAT_CSV                    "CSV"
#define GUI_TEXT_FORMAT_GNUPLOT                "GNUPlot"
#define GUI_TEXT_FORMAT_PLAIN                  "Plain"
#define GUI_OBJECT_LABELS_ALL                  "All"
#define GUI_OBJECT_LABELS_SPACED               "Spaced"
#define GUI_OBJECT_LABELS_POWER2               "2^n"
#define GUI_OBJECT_AXIS_CURRENT                "Fit Current Level"
#define GUI_OBJECT_AXIS_ALL                    "Fit All Levels"
#define GUI_OBJECT_AXIS_ZERO                   "0%"
#define GUI_OBJECT_AXIS_FIVE                   "5%"
#define GUI_OBJECT_AXIS_TEN                    "10%"
#define GUI_OBJECT_AXIS_TWENTYFIVE             "25%"


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
  "Parameters"
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
  "Parameters"
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
  "Type",
  "Statistic",
  "Minimum Gradient",
  "Maximum Gradient",
  "Data.Window",
  "3D.3rd Window",
  "3D.Minimum",
  "3D.Maximum",
  "3D.Delta",
  "3D.Plane"
};

// DIMEMAS
#define OLD_DIMEMAS_SHEBANG "SDDFA"
#define DIMEMAS_SHEBANG "#DIMEMAS_CONFIGURATION"


#endif // PARAVERLABELS_H_INCLUDED
