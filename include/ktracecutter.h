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

#include <list>
#include <set>
#include <unordered_set>

#include "cubecontainer.h"
#include "ktraceoptions.h"
#include "tracecutter.h"
#include "tracestream.h"

class KTraceCutter : public TraceCutter
{
  public:
    KTraceCutter( TraceOptions *options,
                  const std::vector< TEventType > &whichHWCTypes,
                  const std::vector< TEventType > &whichNotHWCTypes );
    virtual ~KTraceCutter();

    virtual void set_by_time( bool byTime ) override;
    virtual void set_min_cutting_time( unsigned long long minCutTime ) override;
    virtual void set_max_cutting_time( unsigned long long maxCutTime ) override;
    virtual void set_minimum_time_percentage( unsigned long long minimumPercentage ) override;
    virtual void set_maximum_time_percentage( unsigned long long maximumPercentage ) override;
    virtual void set_tasks_list( char *tasksList ) override;
    virtual void set_original_time( bool originalTime ) override;
    virtual void set_max_trace_size( int traceSize ) override;
    virtual void set_break_states( bool breakStates ) override;
    virtual void set_remFirstStates( bool remStates ) override;
    virtual void set_remLastStates( bool remStates ) override;
    virtual void set_keep_boundary_events( bool keepEvents ) override;
    virtual void setCutterApplicationCaller( std::string caller ) override;

    virtual void execute( std::string trace_in,
                          std::string trace_out,
                          ProgressController *progress ) override;

  private:
    unsigned int min_perc;
    unsigned int max_perc;
    bool by_time;
    bool originalTime;
    unsigned int max_size;
    bool is_zip;
    unsigned int cut_tasks;
    bool break_states;
    bool strict_cut;
    unsigned long long time_min;
    unsigned long long time_max;
    unsigned long long total_time;
    unsigned long long first_record_time;
    unsigned long long last_record_time;
    unsigned long long current_size;
    unsigned long long total_size;
    unsigned long long trace_time;
    int useful_tasks;
    bool init_useful_tasks;
    bool remFirstStates;
    bool remLastStates;
    bool keep_boundary_events;
    bool keep_all_events;
    bool first_time_caught;

    /* Parameters for showing percentage */
    unsigned long long total_trace_size;
    unsigned long long current_read_size;
    unsigned long total_cutter_iters;
    bool writeToTmpFile;
    bool secondPhase;
    unsigned long long total_tmp_lines;
    unsigned long long current_tmp_lines;

    std::set< TEventType > HWCTypesInPCF;
    std::set< TEventType > notHWCTypesInPCF;

    class ThreadInfo
    {
      public:
        ThreadInfo() : last_time( 0 ), lastCPU( 0 ), finished( false ), without_states( false ), lastStateEndTime( 0 )
        {}

        ThreadInfo( std::set< TEventType > HWCTypesInPCF ) : last_time( 0 ), lastCPU( 0 ), finished( false ), without_states( false ), lastStateEndTime( 0 )
        {
          std::copy( HWCTypesInPCF.begin(), HWCTypesInPCF.end(), std::inserter( HWCTypesToReset, HWCTypesToReset.begin() ) );
        }

        unsigned long long last_time;
        unsigned long long lastStateEndTime;
        TCPUOrder lastCPU; // last CPU to be able to write trailing records.
        bool finished;
        bool without_states;
        std::list< TEventType > openedEventTypes;
        std::set< TEventType > HWCTypesInPRV;
        std::unordered_set< TEventType > HWCTypesToReset;
    };

    /* struct for cutting only selected tasks */
    typedef CubeContainer<TApplOrder, TTaskOrder, TThreadOrder, ThreadInfo> CutterThreadInfo;
    CutterThreadInfo threadsInfo;

    // PROFET
    class ApplicationInfo
    {
      public:
        ApplicationInfo( TThreadOrder whichNumberOfThreads ) : numberOfThreads( whichNumberOfThreads ),
                                                               finishedThreads( 0 ) {}
        bool addFinishedThread()
        {
          return ++finishedThreads == numberOfThreads;
        }
      
      private:
        const TThreadOrder numberOfThreads;
        TThreadOrder finishedThreads;
    };
    std::vector< ApplicationInfo > appsInfo;
    bool firstApplicationFinished;
    unsigned long long timeOfFirsApplicationFinished;


    struct selected_tasks
    {
      int min_task_id;
      int max_task_id;
      int range;
    };

    struct selected_tasks wanted_tasks[MAX_SELECTED_TASKS];
    KTraceOptions *exec_options;
    std::string cutterApplicationCaller;

    void read_cutter_params();
    void proces_cutter_header( const std::string &header, TraceStream *whichFile, FILE *outfile );
    void parseInHeaderAndDumpOut( TraceStream *whichFile, std::fstream& outfile );

    template< typename T >
    void writeOffsetLine( T& outfile,
                          const char *trace_in_name,
                          unsigned long long timeOffset,
                          unsigned long long timeCutBegin,
                          unsigned long long timeCutEnd );

    template< typename IteratorType >
    void dumpEventsSet( std::fstream &outFile,
                        const IteratorType& begin,
                        const IteratorType& end,
                        unsigned int cpu,
                        unsigned int appl,
                        unsigned int task,
                        unsigned int thread,
                        const unsigned long long final_time,
                        int &numWrittenChars,
                        bool &needEOL,
                        bool &writtenComment );

    void appendLastZerosToUnclosedEvents( const unsigned long long final_time, std::fstream &outFile );
    void ini_cutter_progress_bar( const std::string& file_name, ProgressController *progress );
    void show_cutter_progress_bar( ProgressController *progress, TraceStream *whichFile );
    void update_queue( unsigned int appl, unsigned int task, unsigned int thread,
                       unsigned long long type,
                       unsigned long long value );
    void load_counters_of_pcf( char *trace_name );
    void shiftLeft_TraceTimes_ToStartFromZero( const char *originalTraceName, const char *nameIn, const char *nameOut, ProgressController *progress );
    bool is_selected_task( int task_id );

    ThreadInfo& initThreadInfo( unsigned int appl, unsigned int task, unsigned int thread, unsigned int cpu );
};


