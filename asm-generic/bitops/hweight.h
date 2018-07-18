/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_GENERIC_BITOPS_HWEIGHT_H_
#define _ASM_GENERIC_BITOPS_HWEIGHT_H_

#ifdef __KERNEL__
#include <asm-generic/bitops/arch_hweight.h>
#include <asm-generic/bitops/const_hweight.h>
#else
#include "asm-generic/bitops/arch_hweight.h"
#include "asm-generic/bitops/const_hweight.h"
#endif

#endif /* _ASM_GENERIC_BITOPS_HWEIGHT_H_ */
