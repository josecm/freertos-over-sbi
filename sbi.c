#include <sbi.h>

static inline struct sbiret sbi_ecall(long eid, long fid, long a0, long a1,
                                      long a2, long a3, long a4, long a5)
{
    long register _a0 asm("a0") = a0;
    long register _a1 asm("a1") = a1;
    long register _a2 asm("a2") = a2;
    long register _a3 asm("a3") = a3;
    long register _a4 asm("a4") = a4;
    long register _a5 asm("a5") = a5;
    long register _a6 asm("a6") = fid;
    long register _a7 asm("a7") = eid;

    asm volatile("ecall"
                 : "+r"(_a0), "+r"(_a1)
                 : "r"(_a2), "r"(_a3), "r"(_a4), "r"(_a5), "r"(_a6), "r"(_a7)
                 : "memory");

    struct sbiret ret = {.error = _a0, .value = _a1};

    return ret;
}

struct sbiret sbi_get_spec_version(void)
{
    return sbi_ecall(SBI_EXTID_BASE, SBI_GET_SBI_SPEC_VERSION_FID, 0, 0, 0, 0,
                     0, 0);
}
struct sbiret sbi_get_impl_id(void)
{
    return sbi_ecall(SBI_EXTID_BASE, SBI_GET_SBI_IMPL_ID_FID, 0, 0, 0, 0, 0, 0);
}
struct sbiret sbi_get_impl_version(void)
{
    return sbi_ecall(SBI_EXTID_BASE, SBI_GET_SBI_IMPL_VERSION_FID, 0, 0, 0, 0,
                     0, 0);
}
struct sbiret sbi_probe_extension(long extension_id)
{
    return sbi_ecall(SBI_EXTID_BASE, SBI_PROBE_EXTENSION_FID, extension_id, 0,
                     0, 0, 0, 0);
}
struct sbiret sbi_get_mvendorid(void)
{
    return sbi_ecall(SBI_EXTID_BASE, SBI_GET_MVENDORID_FID, 0, 0, 0, 0, 0, 0);
}
struct sbiret sbi_get_marchid(void)
{
    return sbi_ecall(SBI_EXTID_BASE, SBI_GET_MARCHID_FID, 0, 0, 0, 0, 0, 0);
}
struct sbiret sbi_get_mimpid(void)
{
    return sbi_ecall(SBI_EXTID_BASE, SBI_GET_MIMPID_FID, 0, 0, 0, 0, 0, 0);
}

struct sbiret sbi_send_ipi(const unsigned long hart_mask,
                           unsigned long hart_mask_base)
{
    return sbi_ecall(SBI_EXTID_IPI, SBI_SEND_IPI_FID, hart_mask, hart_mask_base,
                     0, 0, 0, 0);
}

struct sbiret sbi_set_timer(uint64_t stime_value)
{
    return sbi_ecall(SBI_EXTID_TIME, SBI_SET_TIMER_FID, stime_value, 0, 0, 0, 0, 0);
}

struct sbiret sbi_remote_fence_i(const unsigned long hart_mask, 
                                 unsigned long hart_mask_base)
{
    return sbi_ecall(SBI_EXTID_RFNC, SBI_REMOTE_FENCE_I_FID, hart_mask,
                     hart_mask_base, 0, 0, 0, 0);    
}

struct sbiret sbi_remote_sfence_vma(const unsigned long hart_mask,
                                    unsigned long hart_mask_base,
                                    unsigned long start_addr,
                                    unsigned long size)
{
    return sbi_ecall(SBI_EXTID_RFNC, SBI_REMOTE_SFENCE_VMA_FID, hart_mask,
                     hart_mask_base, start_addr, size, 0, 0);
}

struct sbiret sbi_remote_hfence_gvma(const unsigned long hart_mask,
                                     unsigned long hart_mask_base,
                                     unsigned long start_addr,
                                     unsigned long size)
{
    return sbi_ecall(SBI_EXTID_RFNC, SBI_REMOTE_HFENCE_GVMA_FID, hart_mask,
                     hart_mask_base, start_addr, size, 0, 0);
}

struct sbiret sbi_remote_hfence_gvma_vmid(const unsigned long hart_mask,
                                          unsigned long hart_mask_base,
                                          unsigned long start_addr,
                                          unsigned long size,
                                          unsigned long vmid)
{
    return sbi_ecall(SBI_EXTID_RFNC, SBI_REMOTE_HFENCE_GVMA_VMID_FID, hart_mask,
                     hart_mask_base, start_addr, size, vmid, 0);
}

struct sbiret sbi_remote_hfence_vvma_asid(const unsigned long hart_mask,
                                          unsigned long hart_mask_base,
                                          unsigned long start_addr,
                                          unsigned long size,
                                          unsigned long asid)
{
    return sbi_ecall(SBI_EXTID_RFNC, SBI_REMOTE_HFENCE_VVMA_ASID_FID, hart_mask,
                     hart_mask_base, start_addr, size, asid, 0);
}

struct sbiret sbi_remote_hfence_vvma(const unsigned long hart_mask,
                                     unsigned long hart_mask_base,
                                     unsigned long start_addr,
                                     unsigned long size)
{
    return sbi_ecall(SBI_EXTID_RFNC, SBI_REMOTE_HFENCE_VVMA_FID, hart_mask,
                     hart_mask_base, start_addr, size, 0, 0);
}