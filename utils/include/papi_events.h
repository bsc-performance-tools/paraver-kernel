#ifndef PAPI_EVENTS_H_INCLUDED
#define PAPI_EVENTS_H_INCLUDED

#include "events.h"

#define PAPI_L1_DCM_LABEL       "PAPI_L1_DCM"
#define PAPI_L1_DCM_EV          0x80000000
#define PAPI_L1_DCM_TYPE_LABEL  "Level 1 data cache misses"

#define PAPI_L1_ICM_LABEL       "PAPI_L1_ICM"
#define PAPI_L1_ICM_EV          0x80000001
#define PAPI_L1_ICM_TYPE_LABEL  "Level 1 instruction cache misses"

#define PAPI_L2_DCM_LABEL       "PAPI_L2_DCM"
#define PAPI_L2_DCM_EV          0x80000002
#define PAPI_L2_DCM_TYPE_LABEL  "Level 2 data cache misses"

#define PAPI_L2_ICM_LABEL       "PAPI_L2_ICM"
#define PAPI_L2_ICM_EV          0x80000003
#define PAPI_L2_ICM_TYPE_LABEL  "Level 2 instruction cache misses"

#define PAPI_L3_DCM_LABEL       "PAPI_L3_DCM"
#define PAPI_L3_DCM_EV          0x80000004
#define PAPI_L3_DCM_TYPE_LABEL  "Level 3 data cache misses"

#define PAPI_L3_ICM_LABEL       "PAPI_L3_ICM"
#define PAPI_L3_ICM_EV          0x80000005
#define PAPI_L3_ICM_TYPE_LABEL  "Level 3 instruction cache misses"

#define PAPI_L1_TCM_LABEL       "PAPI_L1_TCM"
#define PAPI_L1_TCM_EV          0x80000006
#define PAPI_L1_TCM_TYPE_LABEL  "Level 1 cache misses"

#define PAPI_L2_TCM_LABEL       "PAPI_L2_TCM"
#define PAPI_L2_TCM_EV          0x80000007
#define PAPI_L2_TCM_TYPE_LABEL  "Level 2 cache misses"

#define PAPI_L3_TCM_LABEL       "PAPI_L3_TCM"
#define PAPI_L3_TCM_EV          0x80000008
#define PAPI_L3_TCM_TYPE_LABEL  "Level 3 cache misses"

#define PAPI_CA_SNP_LABEL       "PAPI_CA_SNP"
#define PAPI_CA_SNP_EV          0x80000009
#define PAPI_CA_SNP_TYPE_LABEL  "Requests for a snoop"

#define PAPI_CA_SHR_LABEL       "PAPI_CA_SHR"
#define PAPI_CA_SHR_EV          0x8000000a
#define PAPI_CA_SHR_TYPE_LABEL  "Requests for exclusive access to shared cache line"

#define PAPI_CA_CLN_LABEL       "PAPI_CA_CLN"
#define PAPI_CA_CLN_EV          0x8000000b
#define PAPI_CA_CLN_TYPE_LABEL  "Requests for exclusive access to clean cache line"

#define PAPI_CA_INV_LABEL       "PAPI_CA_INV"
#define PAPI_CA_INV_EV          0x8000000c
#define PAPI_CA_INV_TYPE_LABEL  "Requests for cache line invalidation"

#define PAPI_CA_ITV_LABEL       "PAPI_CA_ITV"
#define PAPI_CA_ITV_EV          0x8000000d
#define PAPI_CA_ITV_TYPE_LABEL  "Requests for cache line intervention"

#define PAPI_L3_LDM_LABEL       "PAPI_L3_LDM"
#define PAPI_L3_LDM_EV          0x8000000e
#define PAPI_L3_LDM_TYPE_LABEL  "Level 3 load misses"

#define PAPI_L3_STM_LABEL       "PAPI_L3_STM"
#define PAPI_L3_STM_EV          0x8000000f
#define PAPI_L3_STM_TYPE_LABEL  "Level 3 store misses"

#define PAPI_BRU_IDL_LABEL      "PAPI_BRU_IDL"
#define PAPI_BRU_IDL_EV         0x80000010
#define PAPI_BRU_IDL_TYPE_LABEL "Cycles branch units are idle"

#define PAPI_FXU_IDL_LABEL      "PAPI_FXU_IDL"
#define PAPI_FXU_IDL_EV         0x80000011
#define PAPI_FXU_IDL_TYPE_LABEL "Cycles integer units are idle"

#define PAPI_FPU_IDL_LABEL      "PAPI_FPU_IDL"
#define PAPI_FPU_IDL_EV         0x80000012
#define PAPI_FPU_IDL_TYPE_LABEL "Cycles floating point units are idle"

#define PAPI_LSU_IDL_LABEL      "PAPI_LSU_IDL"
#define PAPI_LSU_IDL_EV         0x80000013
#define PAPI_LSU_IDL_TYPE_LABEL "Cycles load/store units are idle"

#define PAPI_TLB_DM_LABEL       "PAPI_TLB_DM"
#define PAPI_TLB_DM_EV          0x80000014
#define PAPI_TLB_DM_TYPE_LABEL  "Data translation lookaside buffer misses"

#define PAPI_TLB_IM_LABEL       "PAPI_TLB_IM"
#define PAPI_TLB_IM_EV          0x80000015
#define PAPI_TLB_IM_TYPE_LABEL  "Instruction translation lookaside buffer misses"

#define PAPI_TLB_TL_LABEL       "PAPI_TLB_TL"
#define PAPI_TLB_TL_EV          0x80000016
#define PAPI_TLB_TL_TYPE_LABEL  "Total translation lookaside buffer misses"

#define PAPI_L1_LDM_LABEL       "PAPI_L1_LDM"
#define PAPI_L1_LDM_EV          0x80000017
#define PAPI_L1_LDM_TYPE_LABEL  "Level 1 load misses"

#define PAPI_L1_STM_LABEL       "PAPI_L1_STM"
#define PAPI_L1_STM_EV          0x80000018
#define PAPI_L1_STM_TYPE_LABEL  "Level 1 store misses"

#define PAPI_L2_LDM_LABEL       "PAPI_L2_LDM"
#define PAPI_L2_LDM_EV          0x80000019
#define PAPI_L2_LDM_TYPE_LABEL  "Level 2 load misses"

#define PAPI_L2_STM_LABEL       "PAPI_L2_STM"
#define PAPI_L2_STM_EV          0x8000001a
#define PAPI_L2_STM_TYPE_LABEL  "Level 2 store misses"

#define PAPI_BTAC_M_LABEL       "PAPI_BTAC_M"
#define PAPI_BTAC_M_EV          0x8000001b
#define PAPI_BTAC_M_TYPE_LABEL  "Branch target address cache misses"

#define PAPI_PRF_DM_LABEL       "PAPI_PRF_DM"
#define PAPI_PRF_DM_EV          0x8000001c
#define PAPI_PRF_DM_TYPE_LABEL  "Data prefetch cache misses"

#define PAPI_L3_DCH_LABEL       "PAPI_L3_DCH"
#define PAPI_L3_DCH_EV          0x8000001d
#define PAPI_L3_DCH_TYPE_LABEL  "Level 3 data cache hits"

#define PAPI_TLB_SD_LABEL       "PAPI_TLB_SD"
#define PAPI_TLB_SD_EV          0x8000001e
#define PAPI_TLB_SD_TYPE_LABEL  "Translation lookaside buffer shootdowns"

#define PAPI_CSR_FAL_LABEL      "PAPI_CSR_FAL"
#define PAPI_CSR_FAL_EV         0x8000001f
#define PAPI_CSR_FAL_TYPE_LABEL "Failed store conditional instructions"

#define PAPI_CSR_SUC_LABEL      "PAPI_CSR_SUC"
#define PAPI_CSR_SUC_EV         0x80000020
#define PAPI_CSR_SUC_TYPE_LABEL "Successful store conditional instructions"

#define PAPI_CSR_TOT_LABEL      "PAPI_CSR_TOT"
#define PAPI_CSR_TOT_EV         0x80000021
#define PAPI_CSR_TOT_TYPE_LABEL "Total store conditional instructions"

#define PAPI_MEM_SCY_LABEL      "PAPI_MEM_SCY"
#define PAPI_MEM_SCY_EV         0x80000022
#define PAPI_MEM_SCY_TYPE_LABEL "Cycles Stalled Waiting for memory accesses"

#define PAPI_MEM_RCY_LABEL      "PAPI_MEM_RCY"
#define PAPI_MEM_RCY_EV         0x80000023
#define PAPI_MEM_RCY_TYPE_LABEL "Cycles Stalled Waiting for memory Reads"

#define PAPI_MEM_WCY_LABEL      "PAPI_MEM_WCY"
#define PAPI_MEM_WCY_EV         0x80000024
#define PAPI_MEM_WCY_TYPE_LABEL "Cycles Stalled Waiting for memory writes"

#define PAPI_STL_ICY_LABEL      "PAPI_STL_ICY"
#define PAPI_STL_ICY_EV         0x80000025
#define PAPI_STL_ICY_TYPE_LABEL "Cycles with no instruction issue"

#define PAPI_FUL_ICY_LABEL      "PAPI_FUL_ICY"
#define PAPI_FUL_ICY_EV         0x80000026
#define PAPI_FUL_ICY_TYPE_LABEL "Cycles with maximum instruction issue"

#define PAPI_STL_CCY_LABEL      "PAPI_STL_CCY"
#define PAPI_STL_CCY_EV         0x80000027
#define PAPI_STL_CCY_TYPE_LABEL "Cycles with no instructions completed"

#define PAPI_FUL_CCY_LABEL      "PAPI_FUL_CCY"
#define PAPI_FUL_CCY_EV         0x80000028
#define PAPI_FUL_CCY_TYPE_LABEL "Cycles with maximum instructions completed"

#define PAPI_HW_INT_LABEL       "PAPI_HW_INT"
#define PAPI_HW_INT_EV          0x80000029
#define PAPI_HW_INT_TYPE_LABEL  "Hardware interrupts"

#define PAPI_BR_UCN_LABEL       "PAPI_BR_UCN"
#define PAPI_BR_UCN_EV          0x8000002a
#define PAPI_BR_UCN_TYPE_LABEL  "Unconditional branch instructions"

#define PAPI_BR_CN_LABEL        "PAPI_BR_CN"
#define PAPI_BR_CN_EV           0x8000002b
#define PAPI_BR_CN_TYPE_LABEL   "Conditional branch instructions"

#define PAPI_BR_TKN_LABEL       "PAPI_BR_TKN"
#define PAPI_BR_TKN_EV          0x8000002c
#define PAPI_BR_TKN_TYPE_LABEL  "Conditional branch instructions taken"

#define PAPI_BR_NTK_LABEL       "PAPI_BR_NTK"
#define PAPI_BR_NTK_EV          0x8000002d
#define PAPI_BR_NTK_TYPE_LABEL  "Conditional branch instructions not taken"

#define PAPI_BR_MSP_LABEL       "PAPI_BR_MSP"
#define PAPI_BR_MSP_EV          0x8000002e
#define PAPI_BR_MSP_TYPE_LABEL  "Conditional branch instructions mispredicted"

#define PAPI_BR_PRC_LABEL       "PAPI_BR_PRC"
#define PAPI_BR_PRC_EV          0x8000002f
#define PAPI_BR_PRC_TYPE_LABEL  "Conditional branch instructions correctly predicted"

#define PAPI_FMA_INS_LABEL      "PAPI_FMA_INS"
#define PAPI_FMA_INS_EV         0x80000030
#define PAPI_FMA_INS_TYPE_LABEL "FMA instructions completed"

#define PAPI_TOT_IIS_LABEL      "PAPI_TOT_IIS"
#define PAPI_TOT_IIS_EV         0x80000031
#define PAPI_TOT_IIS_TYPE_LABEL "Instructions issued"

#define PAPI_TOT_INS_LABEL      "PAPI_TOT_INS"
#define PAPI_TOT_INS_EV         0x80000032
#define PAPI_TOT_INS_TYPE_LABEL "Instructions completed"

#define PAPI_INT_INS_LABEL      "PAPI_INT_INS"
#define PAPI_INT_INS_EV         0x80000033
#define PAPI_INT_INS_TYPE_LABEL "Integer instructions"

#define PAPI_FP_INS_LABEL       "PAPI_FP_INS"
#define PAPI_FP_INS_EV          0x80000034
#define PAPI_FP_INS_TYPE_LABEL  "Floating point instructions"

#define PAPI_LD_INS_LABEL       "PAPI_LD_INS"
#define PAPI_LD_INS_EV          0x80000035
#define PAPI_LD_INS_TYPE_LABEL  "Load instructions"

#define PAPI_SR_INS_LABEL       "PAPI_SR_INS"
#define PAPI_SR_INS_EV          0x80000036
#define PAPI_SR_INS_TYPE_LABEL  "Store instructions"

#define PAPI_BR_INS_LABEL       "PAPI_BR_INS"
#define PAPI_BR_INS_EV          0x80000037
#define PAPI_BR_INS_TYPE_LABEL  "Branch instructions"

#define PAPI_VEC_INS_LABEL      "PAPI_VEC_INS"
#define PAPI_VEC_INS_EV         0x80000038
#define PAPI_VEC_INS_TYPE_LABEL "Vector/SIMD instructions (could include integer)"

#define PAPI_RES_STL_LABEL      "PAPI_RES_STL"
#define PAPI_RES_STL_EV         0x80000039
#define PAPI_RES_STL_TYPE_LABEL "Cycles stalled on any resource"

#define PAPI_FP_STAL_LABEL      "PAPI_FP_STAL"
#define PAPI_FP_STAL_EV         0x8000003a
#define PAPI_FP_STAL_TYPE_LABEL "Cycles the FP unit(s) are stalled"

#define PAPI_TOT_CYC_LABEL      "PAPI_TOT_CYC"
#define PAPI_TOT_CYC_EV         0x8000003b
#define PAPI_TOT_CYC_TYPE_LABEL "Total cycles"

#define PAPI_LST_INS_LABEL      "PAPI_LST_INS"
#define PAPI_LST_INS_EV         0x8000003c
#define PAPI_LST_INS_TYPE_LABEL "Load/store instructions completed"

#define PAPI_SYC_INS_LABEL      "PAPI_SYC_INS"
#define PAPI_SYC_INS_EV         0x8000003d
#define PAPI_SYC_INS_TYPE_LABEL "Synchronization instructions completed"

#define PAPI_L1_DCH_LABEL       "PAPI_L1_DCH"
#define PAPI_L1_DCH_EV          0x8000003e
#define PAPI_L1_DCH_TYPE_LABEL  "Level 1 data cache hits"

#define PAPI_L2_DCH_LABEL       "PAPI_L2_DCH"
#define PAPI_L2_DCH_EV          0x8000003f
#define PAPI_L2_DCH_TYPE_LABEL  "Level 2 data cache hits"

#define PAPI_L1_DCA_LABEL       "PAPI_L1_DCA"
#define PAPI_L1_DCA_EV          0x80000040
#define PAPI_L1_DCA_TYPE_LABEL  "Level 1 data cache accesses"

#define PAPI_L2_DCA_LABEL       "PAPI_L2_DCA"
#define PAPI_L2_DCA_EV          0x80000041
#define PAPI_L2_DCA_TYPE_LABEL  "Level 2 data cache accesses"

#define PAPI_L3_DCA_LABEL       "PAPI_L3_DCA"
#define PAPI_L3_DCA_EV          0x80000042
#define PAPI_L3_DCA_TYPE_LABEL  "Level 3 data cache accesses"

#define PAPI_L1_DCR_LABEL       "PAPI_L1_DCR"
#define PAPI_L1_DCR_EV          0x80000043
#define PAPI_L1_DCR_TYPE_LABEL  "Level 1 data cache reads"

#define PAPI_L2_DCR_LABEL       "PAPI_L2_DCR"
#define PAPI_L2_DCR_EV          0x80000044
#define PAPI_L2_DCR_TYPE_LABEL  "Level 2 data cache reads"

#define PAPI_L3_DCR_LABEL       "PAPI_L3_DCR"
#define PAPI_L3_DCR_EV          0x80000045
#define PAPI_L3_DCR_TYPE_LABEL  "Level 3 data cache reads"

#define PAPI_L1_DCW_LABEL       "PAPI_L1_DCW"
#define PAPI_L1_DCW_EV          0x80000046
#define PAPI_L1_DCW_TYPE_LABEL  "Level 1 data cache writes"

#define PAPI_L2_DCW_LABEL       "PAPI_L2_DCW"
#define PAPI_L2_DCW_EV          0x80000047
#define PAPI_L2_DCW_TYPE_LABEL  "Level 2 data cache writes"

#define PAPI_L3_DCW_LABEL       "PAPI_L3_DCW"
#define PAPI_L3_DCW_EV          0x80000048
#define PAPI_L3_DCW_TYPE_LABEL  "Level 3 data cache writes"

#define PAPI_L1_ICH_LABEL       "PAPI_L1_ICH"
#define PAPI_L1_ICH_EV          0x80000049
#define PAPI_L1_ICH_TYPE_LABEL  "Level 1 instruction cache hits"

#define PAPI_L2_ICH_LABEL       "PAPI_L2_ICH"
#define PAPI_L2_ICH_EV          0x8000004a
#define PAPI_L2_ICH_TYPE_LABEL  "Level 2 instruction cache hits"

#define PAPI_L3_ICH_LABEL       "PAPI_L3_ICH"
#define PAPI_L3_ICH_EV          0x8000004b
#define PAPI_L3_ICH_TYPE_LABEL  "Level 3 instruction cache hits"

#define PAPI_L1_ICA_LABEL       "PAPI_L1_ICA"
#define PAPI_L1_ICA_EV          0x8000004c
#define PAPI_L1_ICA_TYPE_LABEL  "Level 1 instruction cache accesses"

#define PAPI_L2_ICA_LABEL       "PAPI_L2_ICA"
#define PAPI_L2_ICA_EV          0x8000004d
#define PAPI_L2_ICA_TYPE_LABEL  "Level 2 instruction cache accesses"

#define PAPI_L3_ICA_LABEL       "PAPI_L3_ICA"
#define PAPI_L3_ICA_EV          0x8000004e
#define PAPI_L3_ICA_TYPE_LABEL  "Level 3 instruction cache accesses"

#define PAPI_L1_ICR_LABEL       "PAPI_L1_ICR"
#define PAPI_L1_ICR_EV          0x8000004f
#define PAPI_L1_ICR_TYPE_LABEL  "Level 1 instruction cache reads"

#define PAPI_L2_ICR_LABEL       "PAPI_L2_ICR"
#define PAPI_L2_ICR_EV          0x80000050
#define PAPI_L2_ICR_TYPE_LABEL  "Level 2 instruction cache reads"

#define PAPI_L3_ICR_LABEL       "PAPI_L3_ICR"
#define PAPI_L3_ICR_EV          0x80000051
#define PAPI_L3_ICR_TYPE_LABEL  "Level 3 instruction cache reads"

#define PAPI_L1_ICW_LABEL       "PAPI_L1_ICW"
#define PAPI_L1_ICW_EV          0x80000052
#define PAPI_L1_ICW_TYPE_LABEL  "Level 1 instruction cache writes"

#define PAPI_L2_ICW_LABEL       "PAPI_L2_ICW"
#define PAPI_L2_ICW_EV          0x80000053
#define PAPI_L2_ICW_TYPE_LABEL  "Level 2 instruction cache writes"

#define PAPI_L3_ICW_LABEL       "PAPI_L3_ICW"
#define PAPI_L3_ICW_EV          0x80000054
#define PAPI_L3_ICW_TYPE_LABEL  "Level 3 instruction cache writes"

#define PAPI_L1_TCH_LABEL       "PAPI_L1_TCH"
#define PAPI_L1_TCH_EV          0x80000055
#define PAPI_L1_TCH_TYPE_LABEL  "Level 1 total cache hits"

#define PAPI_L2_TCH_LABEL       "PAPI_L2_TCH"
#define PAPI_L2_TCH_EV          0x80000056
#define PAPI_L2_TCH_TYPE_LABEL  "Level 2 total cache hits"

#define PAPI_L3_TCH_LABEL       "PAPI_L3_TCH"
#define PAPI_L3_TCH_EV          0x80000057
#define PAPI_L3_TCH_TYPE_LABEL  "Level 3 total cache hits"

#define PAPI_L1_TCA_LABEL       "PAPI_L1_TCA"
#define PAPI_L1_TCA_EV          0x80000058
#define PAPI_L1_TCA_TYPE_LABEL  "Level 1 total cache accesses"

#define PAPI_L2_TCA_LABEL       "PAPI_L2_TCA"
#define PAPI_L2_TCA_EV          0x80000059
#define PAPI_L2_TCA_TYPE_LABEL  "Level 2 total cache accesses"

#define PAPI_L3_TCA_LABEL       "PAPI_L3_TCA"
#define PAPI_L3_TCA_EV          0x8000005a
#define PAPI_L3_TCA_TYPE_LABEL  "Level 3 total cache accesses"

#define PAPI_L1_TCR_LABEL       "PAPI_L1_TCR"
#define PAPI_L1_TCR_EV          0x8000005b
#define PAPI_L1_TCR_TYPE_LABEL  "Level 1 total cache reads"

#define PAPI_L2_TCR_LABEL       "PAPI_L2_TCR"
#define PAPI_L2_TCR_EV          0x8000005c
#define PAPI_L2_TCR_TYPE_LABEL  "Level 2 total cache reads"

#define PAPI_L3_TCR_LABEL       "PAPI_L3_TCR"
#define PAPI_L3_TCR_EV          0x8000005d
#define PAPI_L3_TCR_TYPE_LABEL  "Level 3 total cache reads"

#define PAPI_L1_TCW_LABEL       "PAPI_L1_TCW"
#define PAPI_L1_TCW_EV          0x8000005e
#define PAPI_L1_TCW_TYPE_LABEL  "Level 1 total cache writes"

#define PAPI_L2_TCW_LABEL       "PAPI_L2_TCW"
#define PAPI_L2_TCW_EV          0x8000005f
#define PAPI_L2_TCW_TYPE_LABEL  "Level 2 total cache writes"

#define PAPI_L3_TCW_LABEL       "PAPI_L3_TCW"
#define PAPI_L3_TCW_EV          0x80000060
#define PAPI_L3_TCW_TYPE_LABEL  "Level 3 total cache writes"

#define PAPI_FML_INS_LABEL      "PAPI_FML_INS"
#define PAPI_FML_INS_EV         0x80000061
#define PAPI_FML_INS_TYPE_LABEL "Floating point multiply instructions"

#define PAPI_FAD_INS_LABEL      "PAPI_FAD_INS"
#define PAPI_FAD_INS_EV         0x80000062
#define PAPI_FAD_INS_TYPE_LABEL "Floating point add instructions"

#define PAPI_FDV_INS_LABEL      "PAPI_FDV_INS"
#define PAPI_FDV_INS_EV         0x80000063
#define PAPI_FDV_INS_TYPE_LABEL "Floating point divide instructions"

#define PAPI_FSQ_INS_LABEL      "PAPI_FSQ_INS"
#define PAPI_FSQ_INS_EV         0x80000064
#define PAPI_FSQ_INS_TYPE_LABEL "Floating point square root instructions"

#define PAPI_FNV_INS_LABEL      "PAPI_FNV_INS"
#define PAPI_FNV_INS_EV         0x80000065
#define PAPI_FNV_INS_TYPE_LABEL "Floating point inverse instructions"

#define PAPI_FP_OPS_LABEL       "PAPI_FP_OPS"
#define PAPI_FP_OPS_EV          0x80000066
#define PAPI_FP_OPS_TYPE_LABEL  "Floating point operations"

#define PAPI_SP_OPS_LABEL       "PAPI_SP_OPS"
#define PAPI_SP_OPS_EV          0x80000067
#define PAPI_SP_OPS_TYPE_LABEL  "Floating point operations; optimized to count scaled single precision vector operations"

#define PAPI_DP_OPS_LABEL       "PAPI_DP_OPS"
#define PAPI_DP_OPS_EV          0x80000068
#define PAPI_DP_OPS_TYPE_LABEL  "Floating point operations; optimized to count scaled double precision vector operations"

#define PAPI_VEC_SP_LABEL       "PAPI_VEC_SP"
#define PAPI_VEC_SP_EV          0x80000069
#define PAPI_VEC_SP_TYPE_LABEL  "Single precision vector/SIMD instructions"

#define PAPI_VEC_DP_LABEL       "PAPI_VEC_DP"
#define PAPI_VEC_DP_EV          0x8000006a
#define PAPI_VEC_DP_TYPE_LABEL  "Double precision vector/SIMD instructions"


#endif // PAPI_EVENTS_H_INCLUDED
