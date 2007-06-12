#ifndef __ASM_SH64_PAGE_H
#define __ASM_SH64_PAGE_H

/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * include/asm-sh64/page.h
 *
 * Copyright (C) 2000, 2001  Paolo Alberelli
 * Copyright (C) 2003, 2004  Paul Mundt
 *
 * benedict.gaster@superh.com 19th, 24th July 2002.
 *
 * Modified to take account of enabling for D-CACHE support.
 *
 */


/* PAGE_SHIFT determines the page size */
#define PAGE_SHIFT	12
#ifdef __ASSEMBLY__
#define PAGE_SIZE	4096
#else
#define PAGE_SIZE	(1UL << PAGE_SHIFT)
#endif
#define PAGE_MASK	(~(PAGE_SIZE-1))
#define PTE_MASK	PAGE_MASK

#if defined(CONFIG_HUGETLB_PAGE_SIZE_64K)
#define HPAGE_SHIFT	16
#elif defined(CONFIG_HUGETLB_PAGE_SIZE_1MB)
#define HPAGE_SHIFT	20
#elif defined(CONFIG_HUGETLB_PAGE_SIZE_512MB)
#define HPAGE_SHIFT	29
#endif

#ifdef CONFIG_HUGETLB_PAGE
#define HPAGE_SIZE		(1UL << HPAGE_SHIFT)
#define HPAGE_MASK		(~(HPAGE_SIZE-1))
#define HUGETLB_PAGE_ORDER	(HPAGE_SHIFT-PAGE_SHIFT)
#define ARCH_HAS_SETCLEAR_HUGE_PTE
#endif

#endif /* __ASM_SH64_PAGE_H */
