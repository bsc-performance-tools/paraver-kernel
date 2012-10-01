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

struct t_event_mpit2prv
{
  int tipus_mpit;
  int tipus_prv;
  int valor_prv;
  bool utilitzada;               /* Boolea que indica si apareix a la trac,a */
};


struct t_prv_type_info
{
  int type;
  //char *label;
  std::string label;
  int flag_color;
};

struct t_prv_val_label
{
  int value;
  //char *label;
  std::string label;
};

// EXTRAE TRUNK (2.2.1)

#define USER_FUNCTION 60000019

// MPI_EventEncoding.h
// mpi_prv_events.c, .h
#define NUM_MPI_BLOCK_GROUPS 2

#define 	MPITYPE_PTOP   50000001
#define 	MPITYPE_COLLECTIVE   50000002
//(...)

#define MPITYPE_PTOP_LABEL         "MPI Point-to-point"
#define MPITYPE_COLLECTIVE_LABEL   "MPI Collective Comm"
#define MPITYPE_OTHER_LABEL        "MPI Other"
//(...)

#define MPITYPE_FLAG_COLOR 9

/* Dels 12, de moment nomes 8 son diferents */
static struct t_prv_type_info prv_block_groups[ NUM_MPI_BLOCK_GROUPS ] =
{
  {MPITYPE_PTOP, MPITYPE_PTOP_LABEL, MPITYPE_FLAG_COLOR},
  {MPITYPE_COLLECTIVE, MPITYPE_COLLECTIVE_LABEL, MPITYPE_FLAG_COLOR}
};
// (...)

#define NUM_MPI_PRV_ELEMENTS 18

#define 	MPI_SEND_LABEL   "MPI_Send"
#define 	MPI_RECV_LABEL   "MPI_Recv"
#define 	MPI_ISEND_LABEL   "MPI_Isend"
#define 	MPI_IRECV_LABEL   "MPI_Irecv"
#define 	MPI_WAIT_LABEL   "MPI_Wait"
#define 	MPI_WAITALL_LABEL   "MPI_Waitall"
#define 	MPI_BCAST_LABEL   "MPI_Bcast"
#define 	MPI_BARRIER_LABEL   "MPI_Barrier"
#define 	MPI_REDUCE_LABEL   "MPI_Reduce"
#define 	MPI_ALLREDUCE_LABEL   "MPI_Allreduce"
#define   MPI_ALLTOALL_LABEL                "MPI_Alltoall"
#define   MPI_ALLTOALLV_LABEL                "MPI_Alltoallv"
#define 	MPI_GATHER_LABEL   "MPI_Gather"
#define 	MPI_GATHERV_LABEL   "MPI_Gatherv"
#define 	MPI_SCATTER_LABEL   "MPI_Scatter"
#define 	MPI_SCATTERV_LABEL   "MPI_Scatterv"
#define 	MPI_ALLGATHER_LABEL   "MPI_Allgather"
#define 	MPI_ALLGATHERV_LABEL   "MPI_Allgatherv"
// (...)

enum MPIVal
{
  MPI_END_VAL = 0,
  MPI_SEND_VAL, MPI_RECV_VAL, MPI_ISEND_VAL,
  MPI_IRECV_VAL, MPI_WAIT_VAL, MPI_WAITALL_VAL, MPI_BCAST_VAL,
  MPI_BARRIER_VAL, MPI_REDUCE_VAL, MPI_ALLREDUCE_VAL, MPI_ALLTOALL_VAL,
  MPI_ALLTOALLV_VAL, MPI_GATHER_VAL, MPI_GATHERV_VAL, MPI_SCATTER_VAL,
  MPI_SCATTERV_VAL, MPI_ALLGATHER_VAL, MPI_ALLGATHERV_VAL
};
// (...)

static struct t_prv_val_label mpi_prv_val_label[ NUM_MPI_PRV_ELEMENTS ] =
{
  {MPI_SEND_VAL, MPI_SEND_LABEL},
  {MPI_RECV_VAL, MPI_RECV_LABEL},
  {MPI_ISEND_VAL, MPI_ISEND_LABEL},
  {MPI_IRECV_VAL, MPI_IRECV_LABEL},
  {MPI_WAIT_VAL, MPI_WAIT_LABEL},
  {MPI_WAITALL_VAL, MPI_WAITALL_LABEL},
  {MPI_BCAST_VAL, MPI_BCAST_LABEL},
  {MPI_BARRIER_VAL, MPI_BARRIER_LABEL},
  {MPI_REDUCE_VAL, MPI_REDUCE_LABEL},
  {MPI_ALLREDUCE_VAL, MPI_ALLREDUCE_LABEL},
  {MPI_ALLTOALL_VAL, MPI_ALLTOALL_LABEL},
  {MPI_ALLTOALLV_VAL, MPI_ALLTOALLV_LABEL},
  {MPI_GATHER_VAL, MPI_GATHER_LABEL},
  {MPI_GATHERV_VAL, MPI_GATHERV_LABEL},
  {MPI_SCATTER_VAL, MPI_SCATTER_LABEL},
  {MPI_SCATTERV_VAL, MPI_SCATTERV_LABEL},
  {MPI_ALLGATHER_VAL, MPI_ALLGATHER_LABEL},
  {MPI_ALLGATHERV_VAL, MPI_ALLGATHERV_LABEL}
};
// (...)


static struct t_event_mpit2prv event_mpit2prv[ NUM_MPI_PRV_ELEMENTS ] =
{
  {-1, MPITYPE_COLLECTIVE, MPI_ALLGATHER_VAL, false}, /*   1 */
  {-1, MPITYPE_COLLECTIVE, MPI_ALLGATHERV_VAL, false},       /*   2 */
  {-1, MPITYPE_COLLECTIVE, MPI_ALLREDUCE_VAL, false}, /*   3 */
  {-1, MPITYPE_COLLECTIVE, MPI_ALLTOALL_VAL, false},   /*   4 */
  {-1, MPITYPE_COLLECTIVE, MPI_ALLTOALLV_VAL, false}, /*   5 */
  {-1, MPITYPE_COLLECTIVE, MPI_BARRIER_VAL, false},     /*   6 */
  {-1, MPITYPE_COLLECTIVE, MPI_BCAST_VAL, false}, /*   7 */
  {-1, MPITYPE_COLLECTIVE, MPI_GATHER_VAL, false},       /*   8 */
  {-1, MPITYPE_COLLECTIVE, MPI_GATHERV_VAL, false},     /*   9 */
  {-1, MPITYPE_COLLECTIVE, MPI_REDUCE_VAL, false},       /*  13 */
  {-1, MPITYPE_PTOP, MPI_IRECV_VAL, false},       /*  73 */
  {-1, MPITYPE_PTOP, MPI_ISEND_VAL, false},       /*  75 */
  {-1, MPITYPE_PTOP, MPI_RECV_VAL, false}, /*  80 */
  {-1, MPITYPE_PTOP, MPI_SEND_VAL, false}, /*  83 */
  {-1, MPITYPE_PTOP, MPI_WAIT_VAL, false}, /* 108 */
  {-1, MPITYPE_PTOP, MPI_WAITALL_VAL, false},   /* 109 */
//(...)
};

#endif // OTF2PRV_H_INCLUDED
