#ifndef __EXCEPTION_H
#define __EXCEPTION_H

#define E_DIVIDE_BY_ZERO            0
#define E_DEBUG                     1
#define E_NMI                       2
#define E_BREAKPOINT                3
#define E_OVERFLOW                  4
#define E_BOUND_RANGE_EXCEEDED      5
#define E_INVALID_OPCODE            6
#define E_DEVICE_NOT_AVAILABLE      7
#define E_DOUBLE_FAULT              8
#define E_INVALID_TSS               10
#define E_SEGMENT_NOT_PRESENT       11
#define E_STACK_SEGMENT_FAULT       12
#define E_GENERAL_PROTECTION_FAULT  13
#define E_PAGE_FAULT                14
#define E_x87_FP_EXCEPTION          16
#define E_ALIGNMENT_CHECK           17
#define E_MACHINE_CHEKC             18
#define E_SIMD_FP_EXCEPTION         19
#define E_VIRTUALIZATION_EXCEPTION  20
#define E_SECURITY_EXCEPTION        30

extern void installISRs();

#endif