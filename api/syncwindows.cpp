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

#include "syncwindows.h"
#include "histogram.h"
#include "window.h"

using std::map;
using std::vector;

SyncWindows *SyncWindows::instance = nullptr;

SyncWindows *SyncWindows::getInstance ()
{
  if (SyncWindows::instance == nullptr)
    SyncWindows::instance = new SyncWindows ();
  return SyncWindows::instance;
}

SyncWindows::SyncWindows ()
{
  lastNewGroup = 0;
  syncGroups[lastNewGroup].syncGroupsWindows = vector<WindowGenericItem> ();
  addProperty (lastNewGroup, SyncPropertiesType::SYNC_TIME);
  removingAll = false;
}

SyncWindows::~SyncWindows ()
{
}

void SyncWindows::initConfigGroup (std::map<TGroupId, TGroupId> &syncRealGroup)
{
  bool tmpConfigGroupsCreated = false;

  int tmpIteratorGroupMap = 0;
  int tmpIteratorSyncGroups = 0;

  auto itMapSyncGroups = syncRealGroup.begin ();

  while (!tmpConfigGroupsCreated)
  {
    auto itSyncGroups = syncGroups.find (tmpIteratorSyncGroups);

    if (itSyncGroups == syncGroups.end ())
    {
      syncGroups[tmpIteratorSyncGroups].syncGroupsWindows = std::vector<WindowGenericItem> ();
      itMapSyncGroups->second = (TGroupId)tmpIteratorSyncGroups;
      itMapSyncGroups++;

      if (tmpIteratorSyncGroups > lastNewGroup)
        lastNewGroup = tmpIteratorSyncGroups;

      tmpIteratorGroupMap += 1;
      if (itMapSyncGroups == syncRealGroup.end ())
        tmpConfigGroupsCreated = true;
    }
    else
    {
      if (syncGroups[tmpIteratorSyncGroups].syncGroupsWindows.size () == 0)
      {
        itMapSyncGroups->second = (TGroupId)tmpIteratorSyncGroups;
        itMapSyncGroups++;

        tmpIteratorGroupMap += 1;
        if (itMapSyncGroups == syncRealGroup.end ())
          tmpConfigGroupsCreated = true;
      }
    }
    tmpIteratorSyncGroups += 1;
  }
}

void SyncWindows::changePropertiesGroup (WindowGenericItem genericWindow, TGroupId whichGroup, bool isAdded)
{
  auto &tmpSyncGroup = syncGroups[whichGroup].groupType;

  if (isAdded)
  {
    if (std::holds_alternative<Timeline *> (genericWindow))
    {
      if (tmpSyncGroup == SyncPropertiesGroup::SYNC_GROUP_HISTOGRAMS || tmpSyncGroup == SyncPropertiesGroup::SYNC_GROUP_MIXED)
        tmpSyncGroup = SyncPropertiesGroup::SYNC_GROUP_MIXED;
      else
        tmpSyncGroup = SyncPropertiesGroup::SYNC_GROUP_TIMELINES;
    }

    if (std::holds_alternative<Histogram *> (genericWindow))
    {
      auto &tmpSyncGroup = syncGroups[whichGroup].groupType;
      if (tmpSyncGroup == SyncPropertiesGroup::SYNC_GROUP_TIMELINES || tmpSyncGroup == SyncPropertiesGroup::SYNC_GROUP_MIXED)
        tmpSyncGroup = SyncPropertiesGroup::SYNC_GROUP_MIXED;
      else
        tmpSyncGroup = SyncPropertiesGroup::SYNC_GROUP_HISTOGRAMS;
    }
  }
  else
  {
    tmpSyncGroup = SyncPropertiesGroup::SYNC_GROUP_INI;

    for (auto &window : syncGroups[whichGroup].syncGroupsWindows)
    {
      if (std::holds_alternative<Timeline *> (window))
      {
        if (tmpSyncGroup == SyncPropertiesGroup::SYNC_GROUP_HISTOGRAMS || tmpSyncGroup == SyncPropertiesGroup::SYNC_GROUP_MIXED)
          tmpSyncGroup = SyncPropertiesGroup::SYNC_GROUP_MIXED;
        else
          tmpSyncGroup = SyncPropertiesGroup::SYNC_GROUP_TIMELINES;
      }
      if (std::holds_alternative<Histogram *> (window))
      {
        auto &tmpSyncGroup = syncGroups[whichGroup].groupType;
        if (tmpSyncGroup == SyncPropertiesGroup::SYNC_GROUP_TIMELINES || tmpSyncGroup == SyncPropertiesGroup::SYNC_GROUP_MIXED)
          tmpSyncGroup = SyncPropertiesGroup::SYNC_GROUP_MIXED;
        else
          tmpSyncGroup = SyncPropertiesGroup::SYNC_GROUP_HISTOGRAMS;
      }
    }
  }
}

void SyncWindows::removeAllWindows (TGroupId whichGroup)
{
  if (syncGroups.find (whichGroup) == syncGroups.end ())
    return;

  removingAll = true;

  auto visitor = overloads{
      [] (Histogram *histogram)
      { histogram->removeFromSync (); },
      [] (Timeline *timeline)
      { timeline->removeFromSync (); }};

  auto windows = syncGroups[whichGroup].syncGroupsWindows;

  for (auto &it : windows)
    std::visit (visitor, it);

  windows.clear ();

  if (whichGroup != 0)
    syncGroups.erase (whichGroup);

  removingAll = false;
}

int SyncWindows::getNumWindows (TGroupId whichGroup)
{
  return syncGroups[whichGroup].syncGroupsWindows.size ();
}

std::vector<Timeline *> SyncWindows::getGroupTimelineWindows (TGroupId whichGroup)
{
  std::vector<Timeline *> tmpTimelineList;
  for (auto it = syncGroups[whichGroup].syncGroupsWindows.begin (); it != syncGroups[whichGroup].syncGroupsWindows.end (); it++)
  {
    if (std::holds_alternative<Timeline *> (*it))
    {
      tmpTimelineList.push_back (std::get<Timeline *> (*it));
    }
  }
  return tmpTimelineList;
}

void SyncWindows::removeAllGroups ()
{
  for (auto &it : syncGroups)
    removeAllWindows (it.first);
}

TGroupId SyncWindows::newGroup ()
{
  for (size_t i = 0; i <= lastNewGroup; ++i)
  {
    auto it = syncGroups.find (i);
    if (it == syncGroups.end ())
    {
      syncGroups[i].syncGroupsWindows = std::vector<WindowGenericItem> ();
      addProperty (i, SyncPropertiesType::SYNC_TIME);
      return i;
    }
    else if (it->second.syncGroupsWindows.size () == 0)
      return i;
  }

  ++lastNewGroup;
  syncGroups[lastNewGroup].syncGroupsWindows = std::vector<WindowGenericItem> ();
  addProperty (lastNewGroup, SyncPropertiesType::SYNC_TIME);

  return lastNewGroup;
}

int SyncWindows::getNumGroups () const
{
  return syncGroups.size ();
}

bool SyncWindows::isPropertySelected (TGroupId wichGroup, const SyncPropertiesType &newProperty)
{
  if (syncGroups[wichGroup].groupType == SyncPropertiesGroup::SYNC_GROUP_TIMELINES
      && newProperty == SyncPropertiesType::SYNC_HISTOGRAM_DELTA)
  {
    return false;
  }
  if ((!syncGroups[wichGroup].isSameTraceStruct) && newProperty == SyncPropertiesType::SYNC_INFO_PANEL)
  {
    return false;
  }

  auto groupProperties = syncGroups[wichGroup].syncGroupsSelectedProperties;

  return (std::find (groupProperties.begin (), groupProperties.end (), newProperty)
          != groupProperties.end ());
}

bool SyncWindows::isGroupCreated (TGroupId wichGroup) const
{
  return !((syncGroups.find (wichGroup) == syncGroups.end ()));
}

void SyncWindows::updateGroupTraceStruct (TGroupId whichGroup)
{
  auto visitor = overloads{
      [] (Histogram *histogram)
      { 
        if(histogram->getDataWindow () != nullptr)
        return histogram->getDataWindow () ->getTrace ();
        else return (Trace*)nullptr; },
      [] (Timeline *timeline)
      { return timeline->getTrace (); }};

  if (!syncGroups[whichGroup].syncGroupsWindows.empty () && syncGroups[whichGroup].syncGroupsWindows.size () > 0)
  {
    auto trace = std::visit (visitor, (*syncGroups[whichGroup].syncGroupsWindows.begin ()));

    for (auto &window : syncGroups[whichGroup].syncGroupsWindows)
    {
      auto tmpTrace = std::visit (visitor, window);

      if (tmpTrace == nullptr)
      {
        syncGroups[whichGroup].isSameTraceStruct = false;
        break;
      }
      if (tmpTrace == trace)
      {
        syncGroups[whichGroup].isSameTraceStruct = true;
      }
      else if (!trace->isSameObjectStruct (tmpTrace, true))
      {
        syncGroups[whichGroup].isSameTraceStruct = false;
        break;
      }
      else
      {
        syncGroups[whichGroup].isSameTraceStruct = true;
      }
    }
  }
}

void SyncWindows::getGroups (vector<TGroupId> &groups) const
{
  for (auto it = syncGroups.begin (); it != syncGroups.end (); ++it)
    groups.push_back (it->first);
}

void SyncWindows::getGroupsProperties (std::map<TGroupId, std::vector<SyncPropertiesType>> &groups) const
{
  for (auto it = syncGroups.begin (); it != syncGroups.end (); ++it)
    groups[it->first] = it->second.syncGroupsSelectedProperties;
}

void SyncWindows::getGroupAvailableProperties (TGroupId groupId, std::vector<SyncPropertiesType> &properties)
{
  // TODO: CHECK

  properties.push_back (SyncPropertiesType::SYNC_TIME);
  properties.push_back (SyncPropertiesType::SYNC_OBJECT_ZOOM);
  if (syncGroups[groupId].isSameTraceStruct)
  {
    properties.push_back (SyncPropertiesType::SYNC_OBJECT_SELECTION);
  }
  properties.push_back (SyncPropertiesType::SYNC_MAX);
  properties.push_back (SyncPropertiesType::SYNC_MIN);
  if (syncGroups[groupId].groupType != SyncPropertiesGroup::SYNC_GROUP_TIMELINES)
  {
    // properties.push_back(SyncPropertiesType::SYNC_HISTOGRAM_COLUMNS);
    properties.push_back (SyncPropertiesType::SYNC_HISTOGRAM_DELTA);
  }
  properties.push_back (SyncPropertiesType::SYNC_WINDOWS_POSITION);

  properties.push_back (SyncPropertiesType::SYNC_WINDOWS_SIZE);


  if (syncGroups[groupId].groupType != SyncPropertiesGroup::SYNC_GROUP_HISTOGRAMS)
  {
    properties.push_back (SyncPropertiesType::SYNC_COLOR_PALETTE);

    if (syncGroups[groupId].isSameTraceStruct)
    {
      properties.push_back (SyncPropertiesType::SYNC_INFO_PANEL);
    }
  }
}

void SyncWindows::broadcastProperty (TGroupId whichGroup)
{
  if (syncGroups.find (whichGroup) == syncGroups.end ())
    return;

  if (syncGroups[whichGroup].syncGroupsWindows.empty ())
    return;

  for (auto &property : syncGroups[whichGroup].syncGroupsSelectedProperties)
  {
    if (isPropertySelected (whichGroup, property))
    {
      switch (property)
      {
      case SyncPropertiesType::SYNC_TIME:
        /* code */
        broadcastTimeAll (whichGroup);
        break;
      case SyncPropertiesType::SYNC_HISTOGRAM_DELTA:
        /* code */
        broadcastDeltaAll (whichGroup);
        break;
      case SyncPropertiesType::SYNC_HISTOGRAM_COLUMNS:
        /* code */
        broadcastColumnsAll (whichGroup);
        break;
      case SyncPropertiesType::SYNC_MAX:
        /* code */
        broadcastMaxAll (whichGroup);
        break;
      case SyncPropertiesType::SYNC_MIN:
        /* code */
        broadcastMinAll (whichGroup);
        break;
      case SyncPropertiesType::SYNC_OBJECT_ZOOM:
        /* code */
        broadcastObjectZoomAll (whichGroup);
        break;
      case SyncPropertiesType::SYNC_WINDOWS_SIZE:
        /* code */
        broadcastSizeAll (whichGroup);
        break;
      case SyncPropertiesType::SYNC_WINDOWS_POSITION:
        /* code */
        // broadcastPositionAll (whichGroup);
        break;
      case SyncPropertiesType::SYNC_OBJECT_SELECTION:
        /* code */
        for (TTraceLevel level = TTraceLevel::NONE; level <= TTraceLevel::CPU; ++level)
        {
          broadcastObjectSelectionAll (whichGroup, level);
        }
        break;
      case SyncPropertiesType::SYNC_COLOR_PALETTE:
        broadcastColorPaletteAll (whichGroup);
        break;
      default:
        break;
      }
    }
  }
}

void SyncWindows::addProperty (const TGroupId &whichGroup, SyncPropertiesType newProperty)
{
  auto it = std::find (syncGroups[whichGroup].syncGroupsSelectedProperties.begin (), syncGroups[whichGroup].syncGroupsSelectedProperties.end (), newProperty);
  if (it == syncGroups[whichGroup].syncGroupsSelectedProperties.end ())
  {
    syncGroups[whichGroup].syncGroupsSelectedProperties.push_back (newProperty);
  }
}

void SyncWindows::removeProperty (const TGroupId &whichGroup, SyncPropertiesType newProperty)
{
  auto &vec = syncGroups[whichGroup].syncGroupsSelectedProperties;
  vec.erase (std::remove (vec.begin (), vec.end (), newProperty), vec.end ());
}

void SyncWindows::broadcastTimeAll (TGroupId whichGroup, std::optional<TTime> beginTime, std::optional<TTime> endTime)
{
  if (syncGroups.find (whichGroup) == syncGroups.end ())
    return;
  if (syncGroups[whichGroup].isChanging)
    return;
  syncGroups[whichGroup].isChanging = true;

  if (beginTime == std::nullopt || endTime == std::nullopt)
  {
    TTime tmpBeginTime, tmpEndTime;
    getGroupTimes (whichGroup, tmpBeginTime, tmpEndTime);

    beginTime = tmpBeginTime;
    endTime = tmpEndTime;
  }

  for (auto &window : syncGroups[whichGroup].syncGroupsWindows)
  {
    std::visit ([this, whichGroup, beginTime, endTime] (auto &window)
                { broadcastTime (window, whichGroup, beginTime.value (), endTime.value ()); },
                window);
  }
  syncGroups[whichGroup].isChanging = false;
}

void SyncWindows::broadcastDeltaAll (TGroupId whichGroup, std::optional<THistogramLimit> whichDelta)
{
  if (syncGroups.find (whichGroup) == syncGroups.end ())
    return;

  if (syncGroups[whichGroup].isChanging)
    return;

  syncGroups[whichGroup].isChanging = true;

  if (whichDelta == std::nullopt)
  {
    THistogramLimit tmpWhichDelta;
    getGroupDelta (whichGroup, tmpWhichDelta);
    whichDelta = tmpWhichDelta;
  }

  for (auto &window : syncGroups[whichGroup].syncGroupsWindows)
  {
    std::visit (overloads{[this, whichGroup, whichDelta] (Histogram *window)
                          { broadcastDelta (window, whichGroup, whichDelta.value ()); },
                          [] (Timeline *window) {}},
                window);
  }
  syncGroups[whichGroup].isChanging = false;
}

void SyncWindows::broadcastColumnsAll (TGroupId whichGroup, std::optional<THistogramColumn> whichColumns)
{
  if (syncGroups.find (whichGroup) == syncGroups.end ())
    return;

  if (syncGroups[whichGroup].isChanging)
    return;

  syncGroups[whichGroup].isChanging = true;

  if (whichColumns == std::nullopt)
  {
    THistogramColumn tmpWhichColumn;
    getGroupColumns (whichGroup, tmpWhichColumn);
    whichColumns = tmpWhichColumn;
  }
  for (auto &window : syncGroups[whichGroup].syncGroupsWindows)
  {
    std::visit (overloads{[this, whichGroup, whichColumns] (Histogram *window)
                          { broadcastColumns (window, whichGroup, whichColumns.value ()); },
                          [] (Timeline *window) {}},
                window);
  }
  syncGroups[whichGroup].isChanging = false;
}

void SyncWindows::broadcastMaxAll (TGroupId whichGroup, std::optional<double> whichMax)
{
  if (syncGroups.find (whichGroup) == syncGroups.end ())
    return;

  if (syncGroups[whichGroup].isChanging)
    return;

  syncGroups[whichGroup].isChanging = true;

  if (whichMax == std::nullopt)
  {
    THistogramLimit tmpWhichMax;
    getGroupMax (whichGroup, tmpWhichMax);
    whichMax = tmpWhichMax;
  }
  for (auto &window : syncGroups[whichGroup].syncGroupsWindows)
  {
    std::visit (overloads{[this, whichGroup, whichMax] (Histogram *window)
                          { broadcastSemanticMax (window, whichGroup, whichMax.value ()); },
                          [this, whichGroup, whichMax] (Timeline *window)
                          { broadcastSemanticMax (window, whichGroup, whichMax.value ()); }},
                window);
  }
  syncGroups[whichGroup].isChanging = false;
}

void SyncWindows::broadcastMinAll (TGroupId whichGroup, std::optional<double> whichMin)
{
  if (syncGroups.find (whichGroup) == syncGroups.end ())
    return;

  if (syncGroups[whichGroup].isChanging)
    return;

  syncGroups[whichGroup].isChanging = true;

  if (whichMin == std::nullopt)
  {
    THistogramLimit tmpWhichMin;
    getGroupMin (whichGroup, tmpWhichMin);
    whichMin = tmpWhichMin;
  }
  for (auto &window : syncGroups[whichGroup].syncGroupsWindows)
  {
    std::visit (overloads{[this, whichGroup, whichMin] (Histogram *window)
                          { broadcastSemanticMin (window, whichGroup, whichMin.value ()); },
                          [this, whichGroup, whichMin] (Timeline *window)
                          { broadcastSemanticMin (window, whichGroup, whichMin.value ()); }},
                window);
  }
  syncGroups[whichGroup].isChanging = false;
}

void SyncWindows::broadcastSizeAll (TGroupId whichGroup, std::optional<PRV_UINT16> whichPosW, std::optional<PRV_UINT16> whichPosH)
{
  if (syncGroups.find (whichGroup) == syncGroups.end ())
    return;

  if (syncGroups[whichGroup].isChanging)
    return;

  syncGroups[whichGroup].isChanging = true;

  if (whichPosW == std::nullopt)
  {
    PRV_UINT16 tmpShichPosW, tmpShichPosH;
    getGroupSize (whichGroup, tmpShichPosW, tmpShichPosH);
    whichPosW = tmpShichPosW;
    whichPosH = tmpShichPosH;
  }
  for (auto &window : syncGroups[whichGroup].syncGroupsWindows)
  {
    std::visit (overloads{[this, whichGroup, whichPosW, whichPosH] (Histogram *window)
                          { broadcastWindowsSize (window, whichGroup, whichPosW.value (), whichPosH.value ()); },
                          [this, whichGroup, whichPosW, whichPosH] (Timeline *window)
                          { broadcastWindowsSize (window, whichGroup, whichPosW.value (), whichPosH.value ()); }},
                window);
  }
  syncGroups[whichGroup].isChanging = false;
}

void SyncWindows::broadcastPositionAll (TGroupId whichGroup, int whichPosYDiff, int whichPosXDiff)
{
  if (syncGroups.find (whichGroup) == syncGroups.end ())
    return;

  if (syncGroups[whichGroup].isChanging)
    return;

  syncGroups[whichGroup].isChanging = true;

  for (auto &window : syncGroups[whichGroup].syncGroupsWindows)
  {
    std::visit (overloads{[this, whichGroup, whichPosYDiff, whichPosXDiff] (Histogram *window)
                          { broadcastWindowsPosition (window, whichGroup, whichPosXDiff, whichPosYDiff); },
                          [this, whichGroup, whichPosYDiff, whichPosXDiff] (Timeline *window)
                          { broadcastWindowsPosition (window, whichGroup, whichPosXDiff, whichPosYDiff); }},
                window);
  }
  syncGroups[whichGroup].isChanging = false;
}

void SyncWindows::broadcastColorPaletteAll (TGroupId whichGroup, std::optional<std::map<TSemanticValue, rgb>> paletteColors, std::optional<rgb> backgroundColor,
                                            std::optional<rgb> axisColor, std::optional<bool> backgroundAsZero)
{
  if (syncGroups.find (whichGroup) == syncGroups.end ())
    return;

  if (syncGroups[whichGroup].isChanging)
    return;

  syncGroups[whichGroup].isChanging = true;

  if (paletteColors == std::nullopt && backgroundColor == std::nullopt && axisColor == std::nullopt && backgroundAsZero == std::nullopt)
  {
    std::map<TSemanticValue, rgb> tmpPaletteColors;
    rgb tmpBackgroundColor;
    rgb tmpAxisColor;
    bool tmpBackgroundAsZero;

    getGroupColorPalette (whichGroup, tmpPaletteColors, tmpBackgroundColor, tmpAxisColor, tmpBackgroundAsZero);

    paletteColors = tmpPaletteColors;
    backgroundColor = tmpBackgroundColor;
    axisColor = tmpAxisColor;
    backgroundAsZero = tmpBackgroundAsZero;
  }

  for (auto &window : syncGroups[whichGroup].syncGroupsWindows)
  {
    std::visit (overloads{[] (Histogram *) {},
                          [this, whichGroup, paletteColors, backgroundColor, axisColor, backgroundAsZero] (Timeline *window)
                          { broadcastColorPalette (window, whichGroup, paletteColors, backgroundColor, axisColor, backgroundAsZero); }},
                window);
  }
  syncGroups[whichGroup].isChanging = false;
}
void SyncWindows::broadcastObjectZoomAll (TGroupId whichGroup, std::optional<TObjectOrder> beginObject, std::optional<TObjectOrder> endObject)
{
  if (syncGroups.find (whichGroup) == syncGroups.end ())
    return;

  if (syncGroups[whichGroup].isChanging)
    return;

  syncGroups[whichGroup].isChanging = true;

  if (beginObject == std::nullopt || endObject == std::nullopt)
  {
    TObjectOrder tmpBeginObject;
    TObjectOrder tmpEndObject;

    getGroupObjectZoom (whichGroup, tmpBeginObject, tmpEndObject);

    beginObject = tmpBeginObject;
    endObject = tmpEndObject;
  }

  for (auto &window : syncGroups[whichGroup].syncGroupsWindows)
  {
    std::visit (overloads{[this, whichGroup, beginObject, endObject] (auto *window)
                          { broadcastObjectZoom (window, whichGroup, beginObject.value (), endObject.value ()); }},
                window);
  }
  syncGroups[whichGroup].isChanging = false;
}

void SyncWindows::broadcastObjectSelectionAll (TGroupId whichGroup, TTraceLevel wichLevel, std::optional<std::vector<bool>> selectedObjects)
{
  if (syncGroups.find (whichGroup) == syncGroups.end ())
    return;

  if (syncGroups[whichGroup].isChanging)
    return;

  syncGroups[whichGroup].isChanging = true;

  if (selectedObjects == std::nullopt)
  {
    std::vector<bool> tmpSelectedObjects;

    getSelectObjectZoom (whichGroup, tmpSelectedObjects, wichLevel);

    selectedObjects = tmpSelectedObjects;
  }

  for (auto &window : syncGroups[whichGroup].syncGroupsWindows)
  {
    std::visit (overloads{[this, &whichGroup, &wichLevel, &selectedObjects] (auto *w)
                          { broadcastObjectSelection (w, whichGroup, wichLevel, selectedObjects.value ()); }},
                window);
    // [this,whichGroup,beginObject, endObject](Timeline* window){broadcastObjectSelection( window, whichGroup , beginObject.value(), endObject.value());}},
    // window  );
  }
  syncGroups[whichGroup].isChanging = false;
}

void SyncWindows::broadcastTime (Timeline *timeline, TGroupId whichGroup, TTime beginTime, TTime endTime)
{
  if ((timeline->getWindowBeginTime () != beginTime || timeline->getWindowEndTime () != endTime))
  {
    timeline->addZoom (beginTime, endTime);
    timeline->setWindowBeginTime (beginTime);
    timeline->setWindowEndTime (endTime);
    timeline->setChanged (true);
    timeline->setRedraw (true);
  }
}

void SyncWindows::broadcastTime (Histogram *histogram, TGroupId whichGroup, TTime beginTime, TTime endTime)
{
  if ((histogram->getBeginTime () != beginTime || histogram->getEndTime () != endTime))
  {
    histogram->setWindowBeginTime (beginTime);
    histogram->setWindowEndTime (endTime);
    histogram->setChanged (true);
    histogram->setRecalc (true);
  }
}

void SyncWindows::broadcastDelta (Histogram *histogram, TGroupId whichGroup, THistogramLimit whichDelta)
{
  if (histogram->getControlDelta () != whichDelta)
  {
    histogram->setControlDelta (whichDelta);

    // modify current zoom directly
    std::pair<HistogramProxy::TZoomInfo, HistogramProxy::TZoomInfo> zoomInfo = histogram->getZoomFirstDimension ();
    zoomInfo.second.begin = whichDelta; // delta
    histogram->setZoomFirstDimension (zoomInfo);
    histogram->setCompute2DScale (false);
    histogram->setUseFixedDelta (true);
    histogram->setRecalc (true);
  }
}

void SyncWindows::broadcastColumns (Histogram *histogram, TGroupId whichGroup, THistogramColumn newNumColumns)
{
  if (histogram->getNumColumns () != newNumColumns)
  {
    histogram->setUseFixedDelta (false);
    histogram->setNumColumns (newNumColumns);
    histogram->setCompute2DScale (false);
    histogram->setRecalc (true);
  }
}

void SyncWindows::broadcastSemanticMin (Histogram *whichHistogram, TGroupId whichGroup, THistogramLimit newHistoMin)
{
  if (whichHistogram->getControlMin () != newHistoMin)
  {
    whichHistogram->setControlMin (newHistoMin);

    // modify current zoom directly
    std::pair<HistogramProxy::TZoomInfo, HistogramProxy::TZoomInfo> zoomInfo = whichHistogram->getZoomFirstDimension ();
    zoomInfo.first.begin = newHistoMin; // minimum
    whichHistogram->setZoomFirstDimension (zoomInfo);

    whichHistogram->setCompute2DScale (false);
    whichHistogram->setRecalc (true);
  }
}

void SyncWindows::broadcastSemanticMin (Timeline *whichTimeline, TGroupId whichGroup, THistogramLimit newHistoMin)
{
  if (whichTimeline->getMinimumY () != newHistoMin)
  {
    whichTimeline->setMinimumY (newHistoMin);

    // modify current zoom directly
    whichTimeline->setRedraw (true);
  }
}

void SyncWindows::broadcastSemanticMax (Histogram *whichHistogram, TGroupId whichGroup, THistogramLimit newHistoMax)
{
  if (whichHistogram->getControlMax () != newHistoMax)
  {
    whichHistogram->setControlMax (newHistoMax);

    // modify current zoom directly
    std::pair<HistogramProxy::TZoomInfo, HistogramProxy::TZoomInfo> zoomInfo = whichHistogram->getZoomFirstDimension ();
    zoomInfo.first.begin = newHistoMax; // minimum
    whichHistogram->setZoomFirstDimension (zoomInfo);

    whichHistogram->setCompute2DScale (false);
    whichHistogram->setRecalc (true);
  }
}

void SyncWindows::broadcastSemanticMax (Timeline *whichTimeline, TGroupId whichGroup, THistogramLimit newHistoMax)
{
  if (whichTimeline->getMaximumY () != newHistoMax)
  {
    whichTimeline->setMaximumY (newHistoMax);

    // modify current zoom directly
    whichTimeline->setRedraw (true);
  }
}

void SyncWindows::broadcastObjectZoom (Timeline *whichWindow, TGroupId whichGroup, TObjectOrder beginObject, TObjectOrder endObject)
{
  if (whichWindow->getZoomSecondDimension ().first != beginObject && whichWindow->getZoomSecondDimension ().first != endObject)
  {
    whichWindow->addZoom (beginObject, endObject);

    whichWindow->setRedraw (true);
  }
}

void SyncWindows::broadcastObjectZoom (Histogram *whichWindow, TGroupId whichGroup, TObjectOrder beginObject, TObjectOrder endObject)
{
  if (!whichWindow->isZoomEmpty ())
  {
    if (whichWindow->getZoomSecondDimension ().first != beginObject && whichWindow->getZoomSecondDimension ().first != endObject)
    {
      whichWindow->addZoom (beginObject, endObject);

      whichWindow->setRedraw (true);
    }
  }
}

void SyncWindows::broadcastObjectSelection (Timeline *whichWindow, TGroupId whichGroup, TTraceLevel wichLevel, std::vector<bool> selectedObjects)
{
  std::vector<bool> tmpSelectedObjects;
  whichWindow->getSelectedRows (wichLevel, tmpSelectedObjects);

  if (tmpSelectedObjects != selectedObjects)
  {
    whichWindow->setSelectedRows (wichLevel, selectedObjects);

    whichWindow->setRedraw (true);
    whichWindow->setChanged (true);
  }
}

void SyncWindows::broadcastObjectSelection (Histogram *whichWindow, TGroupId whichGroup, TTraceLevel wichLevel, std::vector<bool> selectedObjects)
{
  std::vector<bool> tmpSelectedObjects;
  whichWindow->getRowSelectionManagement ()->getSelected (tmpSelectedObjects, wichLevel);

  if (tmpSelectedObjects != selectedObjects)
  {
    whichWindow->setSelectedRows (wichLevel, selectedObjects);
    whichWindow->setRecalc (true);
  }
}

void SyncWindows::broadcastWindowsSize (Histogram *whichWindow, TGroupId whichGroup, PRV_UINT16 whichPosW, PRV_UINT16 whichPosH)
{
  if (whichWindow->getWidth () != whichPosW || whichWindow->getHeight () != whichPosH)
  {
    whichWindow->setWidth (whichPosW, false);
    whichWindow->setHeight (whichPosH, false);
    whichWindow->onResizeFunctionCallback (whichPosW, whichPosH);
  }
}

void SyncWindows::broadcastWindowsSize (Timeline *whichWindow, TGroupId whichGroup, PRV_UINT16 whichPosW, PRV_UINT16 whichPosH)
{
  if (whichWindow->getWidth () != whichPosW || whichWindow->getHeight () != whichPosH)
  {
    whichWindow->setWidth (whichPosW, false);
    whichWindow->setHeight (whichPosH, false);
    whichWindow->onResizeFunctionCallback (whichPosW, whichPosH);
    whichWindow->setRedraw (true);
  }
}

void SyncWindows::broadcastWindowsPosition (Histogram *whichWindow, TGroupId whichGroup, int whichPosX, int whichPosY)
{

  whichWindow->onPositionFunctionCallback (whichPosX, whichPosY);
}

void SyncWindows::broadcastWindowsPosition (Timeline *whichWindow, TGroupId whichGroup, int whichPosX, int whichPosY)
{
  whichWindow->onPositionFunctionCallback (whichPosX, whichPosY);
}

void SyncWindows::broadcastColorPalette (Timeline *whichWindow, TGroupId whichGroup, std::optional<std::map<TSemanticValue, rgb>> paletteColors, std::optional<rgb> backgroundColor,
                                         std::optional<rgb> axisColor, std::optional<bool> backgroundAsZero)
{
  if (paletteColors != std::nullopt)
    whichWindow->setCustomPalette (paletteColors.value ());
  if (backgroundColor != std::nullopt)
    whichWindow->setCustomBackgroundColor (backgroundColor.value ());
  if (axisColor != std::nullopt)
    whichWindow->setCustomAxisColor (axisColor.value ());
  if (backgroundAsZero != std::nullopt)
    whichWindow->setBackgroundAsZero (backgroundAsZero.value ());

  whichWindow->setUseCustomPalette (true);
  whichWindow->setRedraw (true);
}

void SyncWindows::getGroupSize (TGroupId whichGroup, PRV_UINT16 &whichPosW, PRV_UINT16 &whichPosH)
{
  auto visitor_begin = overloads{
      [] (Histogram *h)
      { return h->getHeight (); },
      [] (Timeline *t)
      { return t->getHeight (); }};
  auto visitor_end = overloads{
      [] (Histogram *h)
      { return h->getWidth (); },
      [] (Timeline *t)
      { return t->getWidth (); }};

  applyToFirstWindow (whichGroup, whichPosW, visitor_end);
  applyToFirstWindow (whichGroup, whichPosH, visitor_begin);
}

void SyncWindows::getGroupTimes (TGroupId whichGroup, TTime &beginTime, TTime &endTime)
{
  auto visitor_begin = overloads{
      [] (Histogram *h)
      { return h->getControlWindow ()->traceUnitsToCustomUnits (h->getBeginTime (), NS); },
      [] (Timeline *t)
      { return t->traceUnitsToCustomUnits (t->getWindowBeginTime (), NS); }};
  auto visitor_end = overloads{
      [] (Histogram *h)
      { return h->getControlWindow ()->traceUnitsToCustomUnits (h->getEndTime (), NS); },
      [] (Timeline *t)
      { return t->traceUnitsToCustomUnits (t->getWindowEndTime (), NS); }};

  applyToFirstWindow (whichGroup, beginTime, visitor_begin);
  applyToFirstWindow (whichGroup, endTime, visitor_end);
}

void SyncWindows::getGroupDelta (TGroupId whichGroup, THistogramLimit &whichDelta)
{
  auto visitor = overloads{
      [] (Histogram *h)
      { return h->getControlDelta (); },
      [] (Timeline *)
      { return 1.0; }};

  applyToFirstWindow (whichGroup, whichDelta, visitor);
}

void SyncWindows::getGroupObjectZoom (TGroupId whichGroup, TObjectOrder &beginObject, TObjectOrder &endObject)
{
  auto visitor_begin = overloads{
      [] (Timeline *t)
      { return t->getZoomSecondDimension ().first; },
      [] (Histogram *h)
      { return h->getZoomSecondDimension ().first; }};

  auto visitor_end = overloads{
      [] (Timeline *t)
      { return t->getZoomSecondDimension ().second; },
      [] (Histogram *h)
      { return h->getZoomSecondDimension ().second; }};

  applyToFirstWindow (whichGroup, beginObject, visitor_begin);
  applyToFirstWindow (whichGroup, endObject, visitor_end);
}

void SyncWindows::getGroupColorPalette (TGroupId whichGroup, std::map<TSemanticValue, rgb> &paletteColors, rgb &backgroundColor,
                                        rgb &axisColor, bool &backgroundAsZero)
{
  auto values_getter = overloads{
      [&paletteColors, &backgroundColor, &axisColor, &backgroundAsZero] (Timeline *t)
      { paletteColors = t->getSemanticColor ().getCustomPalette ();
        backgroundColor = t->getBackgroundColor ();
        axisColor = t->getAxisColor ();
        backgroundAsZero = t->getBackgroundAsZero ();
         return 0; },
      [] (Histogram *)
      { return 0; }};

  auto it = syncGroups.find (whichGroup);
  if (it == syncGroups.end ())
    return;

  const WindowGenericItem &firstElement = *(it->second.syncGroupsWindows.begin ());
  std::visit (values_getter, firstElement);
}

void SyncWindows::getSelectObjectZoom (TGroupId whichGroup, std::vector<bool> &selectedObjects, TTraceLevel &objectsLevel)
{

  auto visitor = overloads{
      [&selectedObjects, &objectsLevel] (Timeline *t)
      {  t->getSelectedRows(objectsLevel,selectedObjects); return selectedObjects; },
      [&selectedObjects, &objectsLevel] (Histogram *h)
      {  h->getRowSelectionManagement()->getSelected(selectedObjects, objectsLevel); return selectedObjects; },
  };

  applyToFirstWindow (whichGroup, selectedObjects, visitor);
}

void SyncWindows::getGroupColumns (TGroupId whichGroup, THistogramColumn &whichColumns)
{
  auto visitor = overloads{
      [] (Histogram *h)
      { return h->getNumColumns (); },
      [] (Timeline *)
      { return (PRV_UINT32)0; }};

  applyToFirstWindow (whichGroup, whichColumns, visitor);
}

void SyncWindows::getGroupMax (TGroupId whichGroup, double &whichMax)
{
  auto visitor = overloads{
      [] (Histogram *h)
      { return h->getControlMax (); },
      [] (Timeline *t)
      { return t->getMaximumY (); }};

  applyToFirstWindow (whichGroup, whichMax, visitor);
}

void SyncWindows::getGroupMin (TGroupId whichGroup, double &whichMin)
{
  auto visitor = overloads{
      [] (Histogram *h)
      { return h->getControlMin (); },
      [] (Timeline *t)
      { return t->getMinimumY (); }};

  applyToFirstWindow (whichGroup, whichMin, visitor);
}

template <typename T, typename Visitor>
bool SyncWindows::applyToFirstWindow (TGroupId whichGroup, T &result, Visitor visitor)
{
  auto it = syncGroups.find (whichGroup);
  if (it == syncGroups.end ())
    return false;

  const WindowGenericItem &firstElement = *(it->second.syncGroupsWindows.begin ());
  result = std::visit (visitor, firstElement);
  return true;
}
