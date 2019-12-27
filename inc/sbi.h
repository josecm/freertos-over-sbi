#ifndef __SBI_H__
#define __SBI_H__

/**
 * From https://github.com/riscv/riscv-sbi-doc
 */

#define SBI_SUCCESS (0)
#define SBI_ERR_FAILURE (-1)
#define SBI_ERR_NOT_SUPPORTED (-2)
#define SBI_ERR_INVALID_PARAM (-3)
#define SBI_ERR_DENIED (-4)
#define SBI_ERR_INVALID_ADDRESS (-5)
#define SBI_ERR_ALREADY_AVAILABLE (-6)

#define SBI_EXTID_SETTIMER (0x0)
#define SBI_EXTID_PUTCHAR (0x1)
#define SBI_EXTID_GETCHAR (0x2)
#define SBI_EXTID_CLEARIPI (0x3)
#define SBI_EXTID_SENDIPI (0x4)
#define SBI_EXTID_REMFENCEI (0x5)
#define SBI_EXTID_REMSFENCEVMA (0x6)
#define SBI_EXTID_REMSFENCEASID (0x7)
#define SBI_EXTID_SHUTDOWN (0x8)

#define SBI_EXTID_BASE (0x10)
#define SBI_GET_SBI_SPEC_VERSION_FID (0)
#define SBI_GET_SBI_IMPL_ID_FID (1)
#define SBI_GET_SBI_IMPL_VERSION_FID (2)
#define SBI_PROBE_EXTENSION_FID (3)
#define SBI_GET_MVENDORID_FID (4)
#define SBI_GET_MARCHID_FID (5)
#define SBI_GET_MIMPID_FID (6)

#define SBI_EXTID_TIME (0x54494D45)
#define SBI_SET_TIMER_FID (0x0)

#define SBI_EXTID_IPI (0x735049)
#define SBI_SEND_IPI_FID (0x0)

#define SBI_EXTID_RFNC (0x52464E43)
#define SBI_REMOTE_FENCE_I_FID (0)
#define SBI_REMOTE_SFENCE_VMA_FID (1)
#define SBI_REMOTE_SFENCE_VMA_ASID_FID (2)
#define SBI_REMOTE_HFENCE_GVMA_FID (3)
#define SBI_REMOTE_HFENCE_GVMA_VMID_FID (4)
#define SBI_REMOTE_HFENCE_VVMA_FID (5)
#define SBI_REMOTE_HFENCE_VVMA_ASID_FID (6)

#ifndef __ASSEMBLER__

#include <stdint.h>

struct sbiret {
    long error;
    long value;
};

struct sbiret sbi_get_spec_version(void);
struct sbiret sbi_get_impl_id(void);
struct sbiret sbi_get_impl_version(void);
struct sbiret sbi_probe_extension(long extension_id);
struct sbiret sbi_get_mvendorid(void);
struct sbiret sbi_get_marchid(void);
struct sbiret sbi_get_mimpid(void);

struct sbiret sbi_send_ipi(const unsigned long hart_mask,
                           unsigned long hart_mask_base);

struct sbiret sbi_set_timer(uint64_t stime_value);

struct sbiret sbi_remote_fence_i(const unsigned long hart_mask,
                                 unsigned long hart_mask_base);

struct sbiret sbi_remote_sfence_vma(const unsigned long hart_mask,
                                    unsigned long hart_mask_base,
                                    unsigned long start_addr,
                                    unsigned long size);

struct sbiret sbi_remote_sfence_vma_asid(const unsigned long hart_mask,
                                         unsigned long hart_mask_base,
                                         unsigned long start_addr,
                                         unsigned long size,
                                         unsigned long asid);

struct sbiret sbi_remote_hfence_gvma_vmid(const unsigned long hart_mask,
                                          unsigned long hart_mask_base,
                                          unsigned long start_addr,
                                          unsigned long size,
                                          unsigned long vmid);

struct sbiret sbi_remote_hfence_gvma(const unsigned long hart_mask,
                                     unsigned long hart_mask_base,
                                     unsigned long start_addr,
                                     unsigned long size);

struct sbiret sbi_remote_hfence_vvma_asid(const unsigned long hart_mask,
                                          unsigned long hart_mask_base,
                                          unsigned long start_addr,
                                          unsigned long size,
                                          unsigned long asid);

struct sbiret sbi_remote_hfence_vvma(const unsigned long hart_mask,
                                     unsigned long hart_mask_base,
                                     unsigned long start_addr,
                                     unsigned long size);

#endif

#endif /* __SBI_H__ */