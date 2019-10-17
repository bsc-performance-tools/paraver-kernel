/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                                  otf2prv                                  *
 *             Trace translator from OTF2 format to PRV format.              *
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


#ifndef OTF2PRV_H_INCLUDED
#define OTF2PRV_H_INCLUDED

/*
 * Default Paraver Options
 */
#define DEFAULT_LEVEL               "THREAD"
#define DEFAULT_SPEED               1
#define DEFAULT_UNITS               "NANOSEC"
#define DEFAULT_LOOK_BACK           100
#define DEFAULT_FLAG_ICONS          "ENABLED"
#define DEFAULT_NUM_OF_STATE_COLORS 1000
#define DEFAULT_YMAX_SCALE          37

#define DEFAULT_THREAD_FUNC    "State As Is"

#define TYPE_LABEL           "EVENT_TYPE"
#define VALUES_LABEL         "VALUES"

#define LET_SPACES(file) file<<endl<<endl

#define TYPE_LBL   150
typedef struct color_t
{
  int value;
  char label[TYPE_LBL];
  int rgb[3];
}
color_t;

/******************************************************************************
 *   Code Colors.
 ******************************************************************************/

#define STATE_0              0
#define STATE0_LBL           "Idle"
#define STATE0_COLOR         {117,195,255}

#define STATE_1              1
#define STATE1_LBL           "Running"
#define STATE1_COLOR         {0,0,255}

#define STATE_2              2
#define STATE2_LBL           "Not created"
#define STATE2_COLOR         {255,255,255}

#define STATE_3              3
#define STATE3_LBL           "Waiting a message"
#define STATE3_COLOR         {255,0,0}

#define STATE_4              4
#define STATE4_LBL           "Blocking Send"
#define STATE4_COLOR         {255,0,174}

#define STATE_5              5
#define STATE5_LBL           "Synchronization"
#define STATE5_COLOR         {179,0,0}

#define STATE_6              6
#define STATE6_LBL           "Test/Probe"
#define STATE6_COLOR         {0,255,0}

#define STATE_7              7
#define STATE7_LBL           "Scheduling and Fork/Join"
#define STATE7_COLOR         {255,255,0}

#define STATE_8              8
#define STATE8_LBL           "Wait/WaitAll"
#define STATE8_COLOR         {235,0,0}

#define STATE_9              9
#define STATE9_LBL           "Blocked"
#define STATE9_COLOR         {0,162,0}

#define STATE_10             10
#define STATE10_LBL          "Immediate Send"
#define STATE10_COLOR        {255,0,255}

#define STATE_11             11
#define STATE11_LBL          "Immediate Receive"
#define STATE11_COLOR        {100,100,177}

#define STATE_12             12
#define STATE12_LBL          "I/O"
#define STATE12_COLOR        {172,174,41}

#define STATE_13             13
#define STATE13_LBL          "Group Communication"
#define STATE13_COLOR        {255,144,26}

#define STATE_14             14
#define STATE14_LBL          "Tracing Disabled"
#define STATE14_COLOR        {2,255,177}

#define STATE_15             15
#define STATE15_LBL          "Others"
#define STATE15_COLOR        {192,224,0}

#define STATE_16             16
#define STATE16_LBL          "Send Receive"
#define STATE16_COLOR        {66,66,66}

#define STATE_17             17
#define STATE17_LBL          "Memory transfer"
#define STATE17_COLOR        {0xff, 0x0, 0x60}

#define STATES_LBL           "STATES"
#define STATES_COLOR_LBL     "STATES_COLOR"

#define STATES_NUMBER        18
extern struct color_t states_inf[STATES_NUMBER];

/******************************************************************************
 *   Gradient Colors.
 ******************************************************************************/

#define GRADIENT_0            0
#define GRADIENT0_LBL         "Gradient 0"
#define GRADIENT0_COLOR       {0,255,2}

#define GRADIENT_1            1
#define GRADIENT1_LBL         "Grad. 1/MPI Events"
#define GRADIENT1_COLOR       {0,244,13}

#define GRADIENT_2            2
#define GRADIENT2_LBL         "Grad. 2/OMP Events"
#define GRADIENT2_COLOR       {0,232,25}

#define GRADIENT_3            3
#define GRADIENT3_LBL         "Grad. 3/OMP locks"
#define GRADIENT3_COLOR       {0,220,37}

#define GRADIENT_4            4
#define GRADIENT4_LBL         "Grad. 4/User func"
#define GRADIENT4_COLOR       {0,209,48}

#define GRADIENT_5            5
#define GRADIENT5_LBL         "Grad. 5/User Events"
#define GRADIENT5_COLOR       {0,197,60}

#define GRADIENT_6            6
#define GRADIENT6_LBL         "Grad. 6/General Events"
#define GRADIENT6_COLOR       {0,185,72}

#define GRADIENT_7            7
#define GRADIENT7_LBL         "Grad. 7/Hardware Counters"
#define GRADIENT7_COLOR       {0,173,84}

#define GRADIENT_8            8
#define GRADIENT8_LBL         "Gradient 8"
#define GRADIENT8_COLOR       {0,162,95}

#define GRADIENT_9             9
#define GRADIENT9_LBL         "Gradient 9"
#define GRADIENT9_COLOR       {0,150,107}

#define GRADIENT_10           10
#define GRADIENT10_LBL        "Gradient 10"
#define GRADIENT10_COLOR      {0,138,119}

#define GRADIENT_11           11
#define GRADIENT11_LBL        "Gradient 11"
#define GRADIENT11_COLOR      {0,127,130}

#define GRADIENT_12           12
#define GRADIENT12_LBL        "Gradient 12"
#define GRADIENT12_COLOR      {0,115,142}

#define GRADIENT_13           13
#define GRADIENT13_LBL        "Gradient 13"
#define GRADIENT13_COLOR      {0,103,154}

#define GRADIENT_14           14
#define GRADIENT14_LBL        "Gradient 14"
#define GRADIENT14_COLOR      {0,91,166}

#define GRADIENT_LBL          "GRADIENT_NAMES"
#define GRADIENT_COLOR_LBL    "GRADIENT_COLOR"

#define GRADIENT_NUMBER       15
extern struct color_t gradient_inf[GRADIENT_NUMBER];

#endif // OTF2PRV_H_INCLUDED
