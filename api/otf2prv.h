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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#ifndef OTF2PRV_H_INCLUDED
#define OTF2PRV_H_INCLUDED

// From otf2_print
/*
#include <scorep_utility/SCOREP_Hashtab.h>
#include <scorep_utility/SCOREP_Vector.h>
*/

/** @internal
 *  @brief Keeps all data for the callbacks.
 *  Please see otf2_print_data_struct for a detailed description. */
//typedef struct otf2_print_data_struct otf2_print_data;

/** @internal
 *  @brief Region definition element.
 *  Please see otf2_print_region_struct for a detailed description. */
//typedef struct otf2_print_def_name_struct otf2_print_def_name;

//struct otf2_print_data_struct
//{
    /** @brief Reader handle. */
  //  OTF2_Reader*   reader;
    /** @brief List of locations to process. */
    //SCOREP_Vector* locations_to_read;

    /** @brief Set of string definitions. */
    //SCOREP_Hashtab* strings;
    /** @brief Set of system tree node definitions. */
    //SCOREP_Hashtab* system_tree_nodes;
    /** @brief Set of location group definitions. */
    //SCOREP_Hashtab* location_groups;
    /** @brief Set of region definitions. */
    //SCOREP_Hashtab* locations;
    /** @brief Set of region definitions. */
    //SCOREP_Hashtab* regions;
    /** @brief Set of group definitions. */
    //SCOREP_Hashtab* groups;
    /** @brief Set of metric definitions. */
    //SCOREP_Hashtab* metrics;
    /** @brief Set of MPI comms definitions. */
    //SCOREP_Hashtab* mpi_comms;
    /** @brief Set of attribute definitions. */
    //SCOREP_Hashtab* attributes;
    /** @brief Set of parameter definitions. */
    //SCOREP_Hashtab* parameters;

    /** @brief File handle for dot output. */
    //FILE* dot_file;
//};

/** @internal
 *  @brief Region definition element. */
//struct otf2_print_def_name_struct
//{
//    /** @brief The ID of the definition. */
 //   uint64_t def_id;
  //  /** @brief The name if the definition. */
   // char*    name;
//};
#endif // OTF2PRV_H_INCLUDED
