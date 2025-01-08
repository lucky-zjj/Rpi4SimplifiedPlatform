#include "config.h"
#include "stdint.h"

#ifdef RUN_CACHE

#include "lib/printf.h"
#include "mmu.h"
#include "cache.h"

#include "experiment/exp_cache_runner.h"


#define __UNUSED __attribute__((unused))
#define __ALIGN(x) __attribute__ ((aligned (x)))
#define PAGE_SIZE (4096)
/* page table memory */
uint64_t page_table_l1[4] __ALIGN(PAGE_SIZE);

void reset_cache_experiment() {
  disable_mmu();
}

void print_page_table_l1(uint64_t *page_table_l1) {
    printf("\nPage Table L1:\n");
    printf("Idx    VA Start       PA Start       Cacheable\n");
    printf("------------------------------------------------\n");

    for (int i = 0; i < 4; i++) {
        uint64_t entry = page_table_l1[i];
        if (entry == 0) {
            continue; // 如果条目为空，跳过
        }

        uint64_t va_start = i * L1_PAGE_SIZE;
        uint64_t pa_start = (entry & 0xFFFFFFFFF000) & ~(L1_PAGE_SIZE - 1); // 提取物理地址
        uint64_t attr_idx = (entry >> 2) & 0x7; // 提取 ATTR
        uint64_t is_cacheable = (attr_idx == 0b101 || attr_idx == 0b011); // 判断是否可缓存

        printf("%d    0x%x    0x%x    %s\n", 
               i, 
               va_start, 
               pa_start, 
               is_cacheable ? "Cacheable" : "Non-Cacheable");
    }
}

static void basic_mmu() {
  init_mmu();
  set_l1(page_table_l1);
  // Set up translation table entries in memory with looped store
  // instructions.
  // Set the level 1 translation table.
  l1_set_translation(page_table_l1, 0, 0, 0);
  l1_set_translation(page_table_l1, 0x40000000, 0, 0);
  // Executable Inner and Outer Shareable.
  // R/W at all ELs secure memory
  // AttrIdx=000 Device-nGnRnE.
  // The third entry is 1GB block from 0x80000000 to 0xBFFFFFFF.
  l1_set_translation(page_table_l1, 0x80000000, 0, 1);
  //l1_set_translation(page_table_l1, 0xC0000000, 0, 1);

  // TODO: dirty quick fix for rpi4, overwrites the last mapping, second cacheable alias
  l1_set_translation(page_table_l1, 0xC0000000, 0xC0000000, 0);

  enable_mmu();
  print_page_table_l1(page_table_l1);
}

#define CACHEABLE(x) ((void *)(((uint64_t)(&x)) + 0x80000000))
#define ALIAS(x)     ((void *)(((uint64_t)(&x)) + 0x40000000))


unsigned int check_cache_status_el3() {
    unsigned long sctlr_el3;
    
    // Read SCTLR_EL3
    asm volatile("MRS %0, SCTLR_EL3" : "=r" (sctlr_el3));

    // Extract the status of C and I bits
    unsigned int data_cache_enabled = (sctlr_el3 >> 2) & 1;  // Bit 2
    unsigned int instruction_cache_enabled = (sctlr_el3 >> 12) & 1;  // Bit 12

    // Combine results into a single value
    return (data_cache_enabled << 1) | instruction_cache_enabled;
}

#ifndef SINGLE_EXPERIMENTS
void run_cache_experiment() {
  // setup and enable mmu
  basic_mmu();


#ifdef __MEASUREMENT__CACHE
  flush_d_cache(0); // flush L1
  flush_d_cache(1); // flush L2
  static cache_state cache1;
  // static cache_l2_state cache2;

  unsigned int cache_status = check_cache_status_el3();

  printf("Cache status:\n");
  if (cache_status & 2) {
      printf("Data cache: Enabled\n");
  } else {
      printf("Data cache: Disabled\n");
  }

  if (cache_status & 1) {
      printf("Instruction cache: Enabled\n");
  } else {
      printf("Instruction cache: Disabled\n");
  }

  cache_run_mult_compare(1, cache1, NUM_MUL_RUNS);
  // save_cache_l2_state(cache2);
  print_cache_valid(cache1);
  // print_cache_l2_valid(cache2);
#endif


#ifdef __MEASUREMENT__BRANCH_PREDICTOR
  branch_rev();
#endif
 
}
#endif // !SINGLE_EXPERIMENTS

#endif // RUN_CACHE
