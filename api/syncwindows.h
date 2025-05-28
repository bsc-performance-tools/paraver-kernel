/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                       Paraver Main Computing Library                      *
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

#include <map>
#include <optional>
#include <variant>

#include "paraverkerneltypes.h"

typedef unsigned int TGroupId;

enum class SyncPropertiesType
{
  SYNC_INIT = 0,
  SYNC_TIME,
  SYNC_HISTOGRAM_DELTA,
  SYNC_HISTOGRAM_COLUMNS,
  SYNC_MIN,
  SYNC_MAX,
  SYNC_OBJECT_ZOOM,
  SYNC_OBJECT_SELECTION,
  SYNC_WINDOWS_SIZE
};

enum class SyncPropertiesGroup
{
  SYNC_GROUP_INI = 0,
  SYNC_GROUP_HISTOGRAMS,
  SYNC_GROUP_TIMELINES,
  SYNC_GROUP_MIXED
};

class Timeline;
class Histogram;

using WindowGenericItem = std::variant<Histogram *, Timeline *>;

// Helper para sobrecargar lambdas
template <class... Ts>
struct overloads : Ts...
{
  using Ts::operator()...;
};

// Deduce constructor automatico para templates.CTAD
template <class... Ts>
overloads (Ts...) -> overloads<Ts...>;

struct SyncItem
{
  TGroupId groupId;
  SyncPropertiesGroup groupType = SyncPropertiesGroup::SYNC_GROUP_INI;
  std::vector<WindowGenericItem> syncGroupsWindows;
  std::vector<SyncPropertiesType> syncGroupsSelectedProperties;
  bool isSameTraceStruct = true;
  bool isChanging = false;
};

class SyncWindows
{
public:
  static SyncWindows *getInstance ();
  ~SyncWindows ();
  //////////////////////////////////////////////

  void initConfigGroup (std::map<TGroupId, TGroupId> &syncRealGroup);

  TGroupId newGroup ();

  void addProperty (const TGroupId &whichGroup, SyncPropertiesType newProperty);

  template <typename T>
  bool addWindow (T *whichWindow, TGroupId whichGroup)
  {
    WindowGenericItem genericWindow{whichWindow};

    if (syncGroups.find (whichGroup) == syncGroups.end ())
      return false;

    // TODO: CHECK OTHER PROPERTIES
    if (syncGroups[whichGroup].syncGroupsWindows.size () > 0)
    {
      applyGroupProperties (whichWindow, whichGroup);
    }
    changePropertiesGroup (genericWindow, whichGroup, true);
    updateGroupTraceStruct (whichGroup);

    syncGroups[whichGroup].syncGroupsWindows.push_back (genericWindow);
    return true;
  };

  template <typename T>
  void removeWindow (T *whichWindow, TGroupId whichGroup)
  {
    WindowGenericItem genericWindow{whichWindow};

    if (syncGroups.find (whichGroup) == syncGroups.end () || removingAll)
      return;

    auto &windows = syncGroups[whichGroup].syncGroupsWindows;
    for (auto it = windows.begin (); it != windows.end (); ++it)
    {
      if (*it == genericWindow)
      {
        windows.erase (it);
        break;
      }
    }
    changePropertiesGroup (genericWindow, whichGroup, false);
    updateGroupTraceStruct (whichGroup);
  }

  void removeAllWindows (TGroupId whichGroup);
  void removeAllGroups ();
  void removeProperty (const TGroupId &whichGroup, SyncPropertiesType newProperty);

  int getNumWindows (TGroupId whichGroup);
  int getNumGroups () const;

  void getGroups (std::vector<TGroupId> &groups) const;
  void getGroupsProperties (std::map<TGroupId, std::vector<SyncPropertiesType>> &groups) const;

  void getGroupAvailableProperties (TGroupId groupId, std::vector<SyncPropertiesType> &properties);

  bool isGroupCreated (TGroupId whichGroup) const;
  bool isPropertySelected (TGroupId whichGroup, const SyncPropertiesType &newProperty);

  void updateGroupTraceStruct (TGroupId whichGroup);

  void broadcastProperty (TGroupId whichGroup);

  void broadcastTimeAll (TGroupId whichGroup, std::optional<TTime> beginTime = std::nullopt, std::optional<TTime> endTime = std::nullopt);
  void broadcastDeltaAll (TGroupId whichGroup, std::optional<THistogramLimit> whichDelta = std::nullopt);
  void broadcastColumnsAll (TGroupId whichGroup, std::optional<THistogramColumn> whichColumns = std::nullopt);
  void broadcastMaxAll (TGroupId whichGroup, std::optional<double> whichMax = std::nullopt);
  void broadcastMinAll (TGroupId whichGroup, std::optional<double> whichMin = std::nullopt);
  void broadcastObjectZoomAll (TGroupId whichGroup, std::optional<TObjectOrder> beginObject = std::nullopt, std::optional<TObjectOrder> endObject = std::nullopt);
  void broadcastObjectSelectionAll (TGroupId whichGroup, TTraceLevel wichLevel, std::optional<std::vector<bool>> selectedObjects = std::nullopt);
  void broadcastSizeAll (TGroupId whichGroup, std::optional<PRV_UINT16> whichPosW = std::nullopt, std::optional<PRV_UINT16> whichPosH = std::nullopt);

  void getGroupTimes (TGroupId whichGroup, TTime &beginTime, TTime &endTime);
  void getGroupDelta (TGroupId whichGroup, THistogramLimit &whichDelta);
  void getGroupColumns (TGroupId whichGroup, THistogramColumn &beginTime);
  void getGroupMax (TGroupId whichGroup, double &whichMax);
  void getGroupMin (TGroupId whichGroup, double &whichMin);
  void getGroupObjectZoom (TGroupId whichGroup, TObjectOrder &beginObject, TObjectOrder &endObject);
  void getGroupSize (TGroupId whichGroup, PRV_UINT16 &whichPosW, PRV_UINT16 &whichPosH);

  void getSelectObjectZoom (TGroupId whichGroup, std::vector<bool> &beginObject, TTraceLevel &endObject);

private:
  SyncWindows ();

  static SyncWindows *instance;

  std::map<TGroupId, SyncItem> syncGroups;

  TGroupId lastNewGroup;
  bool removingAll;

  template <typename T, typename Visitor>
  bool applyToFirstWindow (TGroupId whichGroup, T &result, Visitor visitor);

  void changePropertiesGroup (WindowGenericItem genericWindow, TGroupId whichGroup, bool isAdded);

  void broadcastTime (Timeline *whichWindow, TGroupId whichGroup, TTime beginTime, TTime endTime);
  void broadcastTime (Histogram *whichWindow, TGroupId whichGroup, TTime beginTime, TTime endTime);

  void broadcastDelta (Histogram *whichWindow, TGroupId whichGroup, THistogramLimit whichDelta);

  void broadcastColumns (Histogram *histogram, TGroupId whichGroup, THistogramColumn newNumColumns);

  void broadcastSemanticMin (Histogram *histogram, TGroupId whichGroup, THistogramLimit newNumColumns);
  void broadcastSemanticMin (Timeline *histogram, TGroupId whichGroup, TSemanticValue newNumColumns);

  void broadcastSemanticMax (Histogram *histogram, TGroupId whichGroup, THistogramLimit newNumColumns);
  void broadcastSemanticMax (Timeline *histogram, TGroupId whichGroup, TSemanticValue newNumColumns);

  void broadcastObjectZoom (Timeline *whichWindow, TGroupId whichGroup, TObjectOrder beginTime, TObjectOrder endTime);

  void broadcastObjectSelection (Timeline *whichWindow, TGroupId whichGroup, TTraceLevel wichLevel, std::vector<bool> selectedObjects);
  void broadcastObjectSelection (Histogram *whichWindow, TGroupId whichGroup, TTraceLevel wichLevel, std::vector<bool> selectedObjects);

  void broadcastWindowsSize (Timeline *whichWindow, TGroupId whichGroup, PRV_UINT16 whichPosW, PRV_UINT16 whichPosH);
  void broadcastWindowsSize (Histogram *whichWindow, TGroupId whichGroup, PRV_UINT16 whichPosW, PRV_UINT16 whichPosH);

  template <typename T>
  void applyGroupProperties (T *whichWindow, TGroupId whichGroup)
  {
    if (syncGroups.find (whichGroup) == syncGroups.end ())
      return;

    for (auto &property : syncGroups[whichGroup].syncGroupsSelectedProperties)
    {
      if (isPropertySelected (whichGroup, property))
      {
        switch (property)
        {
        case SyncPropertiesType::SYNC_TIME:
          /* code */
          TTime tmpBeginTime, tmpEndTime;
          getGroupTimes (whichGroup, tmpBeginTime, tmpEndTime);
          broadcastTime (whichWindow, whichGroup, tmpBeginTime, tmpEndTime);

          break;
        case SyncPropertiesType::SYNC_HISTOGRAM_DELTA:
          /* code */
          if constexpr (std::is_same_v<T, Histogram>)
          {
            THistogramLimit tmpWhichDelta;
            getGroupDelta (whichGroup, tmpWhichDelta);
            broadcastDelta (whichWindow, whichGroup, tmpWhichDelta);
          }
          break;
        case SyncPropertiesType::SYNC_HISTOGRAM_COLUMNS:
          /* code */
          if constexpr (std::is_same_v<T, Histogram>)
          {
            THistogramColumn tmpWhichColumn;
            getGroupColumns (whichGroup, tmpWhichColumn);
            broadcastColumns (whichWindow, whichGroup, tmpWhichColumn);
          }
          break;
        case SyncPropertiesType::SYNC_MAX:
          /* code */
          THistogramLimit tmpWhichMax;
          getGroupMax (whichGroup, tmpWhichMax);
          broadcastSemanticMax (whichWindow, whichGroup, tmpWhichMax);
          break;
        case SyncPropertiesType::SYNC_MIN:
          /* code */
          THistogramLimit tmpWhichMin;
          getGroupMin (whichGroup, tmpWhichMin);
          broadcastSemanticMin (whichWindow, whichGroup, tmpWhichMin);
          break;
        case SyncPropertiesType::SYNC_OBJECT_ZOOM:
          /* code */
          if constexpr (std::is_same_v<T, Timeline>)
          {
            TObjectOrder tmpBeginObject;
            TObjectOrder tmpEndObject;
            getGroupObjectZoom (whichGroup, tmpBeginObject, tmpEndObject);
            broadcastObjectZoom (whichWindow, whichGroup, tmpBeginObject, tmpEndObject);
          }
          break;
        case SyncPropertiesType::SYNC_OBJECT_SELECTION:
          /* code */
          for (TTraceLevel level = TTraceLevel::NONE; level <= TTraceLevel::CPU; ++level)
          {
            std::vector<bool> tmpSelectedObjects;
            getSelectObjectZoom (whichGroup, tmpSelectedObjects, level);
            broadcastObjectSelection (whichWindow, whichGroup, level, tmpSelectedObjects);
          }

          break;
        case SyncPropertiesType::SYNC_WINDOWS_SIZE:
          /* code */
          PRV_UINT16 whichPosW, whichPosH;
          getGroupSize (whichGroup, whichPosW, whichPosH);
          broadcastWindowsSize (whichWindow, whichGroup, whichPosW, whichPosH);
          break;
        default:
          break;
        }
      }
    }
  }
};
