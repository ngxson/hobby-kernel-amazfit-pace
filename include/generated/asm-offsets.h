#ifndef __ASM_OFFSETS_H__
#define __ASM_OFFSETS_H__
/*
 * DO NOT MODIFY.
 *
 * This file was generated by Kbuild
 *
 */

/* MIPS pt_regs offsets. */
#define PT_R0 24 /* offsetof(struct pt_regs, regs[0])	 # */
#define PT_R1 28 /* offsetof(struct pt_regs, regs[1])	 # */
#define PT_R2 32 /* offsetof(struct pt_regs, regs[2])	 # */
#define PT_R3 36 /* offsetof(struct pt_regs, regs[3])	 # */
#define PT_R4 40 /* offsetof(struct pt_regs, regs[4])	 # */
#define PT_R5 44 /* offsetof(struct pt_regs, regs[5])	 # */
#define PT_R6 48 /* offsetof(struct pt_regs, regs[6])	 # */
#define PT_R7 52 /* offsetof(struct pt_regs, regs[7])	 # */
#define PT_R8 56 /* offsetof(struct pt_regs, regs[8])	 # */
#define PT_R9 60 /* offsetof(struct pt_regs, regs[9])	 # */
#define PT_R10 64 /* offsetof(struct pt_regs, regs[10])	 # */
#define PT_R11 68 /* offsetof(struct pt_regs, regs[11])	 # */
#define PT_R12 72 /* offsetof(struct pt_regs, regs[12])	 # */
#define PT_R13 76 /* offsetof(struct pt_regs, regs[13])	 # */
#define PT_R14 80 /* offsetof(struct pt_regs, regs[14])	 # */
#define PT_R15 84 /* offsetof(struct pt_regs, regs[15])	 # */
#define PT_R16 88 /* offsetof(struct pt_regs, regs[16])	 # */
#define PT_R17 92 /* offsetof(struct pt_regs, regs[17])	 # */
#define PT_R18 96 /* offsetof(struct pt_regs, regs[18])	 # */
#define PT_R19 100 /* offsetof(struct pt_regs, regs[19])	 # */
#define PT_R20 104 /* offsetof(struct pt_regs, regs[20])	 # */
#define PT_R21 108 /* offsetof(struct pt_regs, regs[21])	 # */
#define PT_R22 112 /* offsetof(struct pt_regs, regs[22])	 # */
#define PT_R23 116 /* offsetof(struct pt_regs, regs[23])	 # */
#define PT_R24 120 /* offsetof(struct pt_regs, regs[24])	 # */
#define PT_R25 124 /* offsetof(struct pt_regs, regs[25])	 # */
#define PT_R26 128 /* offsetof(struct pt_regs, regs[26])	 # */
#define PT_R27 132 /* offsetof(struct pt_regs, regs[27])	 # */
#define PT_R28 136 /* offsetof(struct pt_regs, regs[28])	 # */
#define PT_R29 140 /* offsetof(struct pt_regs, regs[29])	 # */
#define PT_R30 144 /* offsetof(struct pt_regs, regs[30])	 # */
#define PT_R31 148 /* offsetof(struct pt_regs, regs[31])	 # */
#define PT_LO 160 /* offsetof(struct pt_regs, lo)	 # */
#define PT_HI 156 /* offsetof(struct pt_regs, hi)	 # */
#define PT_EPC 172 /* offsetof(struct pt_regs, cp0_epc)	 # */
#define PT_BVADDR 164 /* offsetof(struct pt_regs, cp0_badvaddr)	 # */
#define PT_STATUS 152 /* offsetof(struct pt_regs, cp0_status)	 # */
#define PT_CAUSE 168 /* offsetof(struct pt_regs, cp0_cause)	 # */
#define PT_SIZE 176 /* sizeof(struct pt_regs)	 # */

/* MIPS task_struct offsets. */
#define TASK_STATE 0 /* offsetof(struct task_struct, state)	 # */
#define TASK_THREAD_INFO 4 /* offsetof(struct task_struct, stack)	 # */
#define TASK_FLAGS 12 /* offsetof(struct task_struct, flags)	 # */
#define TASK_MM 472 /* offsetof(struct task_struct, mm)	 # */
#define TASK_PID 512 /* offsetof(struct task_struct, pid)	 # */
#define TASK_STRUCT_SIZE 1512 /* sizeof(struct task_struct)	 # */

/* MIPS thread_info offsets. */
#define TI_TASK 0 /* offsetof(struct thread_info, task)	 # */
#define TI_EXEC_DOMAIN 4 /* offsetof(struct thread_info, exec_domain)	 # */
#define TI_FLAGS 8 /* offsetof(struct thread_info, flags)	 # */
#define TI_TP_VALUE 12 /* offsetof(struct thread_info, tp_value)	 # */
#define TI_CPU 16 /* offsetof(struct thread_info, cpu)	 # */
#define TI_PRE_COUNT 20 /* offsetof(struct thread_info, preempt_count)	 # */
#define TI_ADDR_LIMIT 24 /* offsetof(struct thread_info, addr_limit)	 # */
#define TI_RESTART_BLOCK 32 /* offsetof(struct thread_info, restart_block)	 # */
#define TI_REGS 72 /* offsetof(struct thread_info, regs)	 # */
#define _THREAD_SIZE 8192 /* THREAD_SIZE	 # */
#define _THREAD_MASK 8191 /* THREAD_MASK	 # */

/* MIPS specific thread_struct offsets. */
#define THREAD_REG16 760 /* offsetof(struct task_struct, thread.reg16)	 # */
#define THREAD_REG17 764 /* offsetof(struct task_struct, thread.reg17)	 # */
#define THREAD_REG18 768 /* offsetof(struct task_struct, thread.reg18)	 # */
#define THREAD_REG19 772 /* offsetof(struct task_struct, thread.reg19)	 # */
#define THREAD_REG20 776 /* offsetof(struct task_struct, thread.reg20)	 # */
#define THREAD_REG21 780 /* offsetof(struct task_struct, thread.reg21)	 # */
#define THREAD_REG22 784 /* offsetof(struct task_struct, thread.reg22)	 # */
#define THREAD_REG23 788 /* offsetof(struct task_struct, thread.reg23)	 # */
#define THREAD_REG29 792 /* offsetof(struct task_struct, thread.reg29)	 # */
#define THREAD_REG30 796 /* offsetof(struct task_struct, thread.reg30)	 # */
#define THREAD_REG31 800 /* offsetof(struct task_struct, thread.reg31)	 # */
#define THREAD_STATUS 804 /* offsetof(struct task_struct, thread.cp0_status)	 # */
#define THREAD_FPU 808 /* offsetof(struct task_struct, thread.fpu)	 # */
#define THREAD_BVADDR 1192 /* offsetof(struct task_struct, thread.cp0_badvaddr)	 # */
#define THREAD_BUADDR 1196 /* offsetof(struct task_struct, thread.cp0_baduaddr)	 # */
#define THREAD_ECODE 1200 /* offsetof(struct task_struct, thread.error_code)	 # */

#define THREAD_FPR0 808 /* offsetof(struct task_struct, thread.fpu.fpr[0])	 # */
#define THREAD_FPR1 816 /* offsetof(struct task_struct, thread.fpu.fpr[1])	 # */
#define THREAD_FPR2 824 /* offsetof(struct task_struct, thread.fpu.fpr[2])	 # */
#define THREAD_FPR3 832 /* offsetof(struct task_struct, thread.fpu.fpr[3])	 # */
#define THREAD_FPR4 840 /* offsetof(struct task_struct, thread.fpu.fpr[4])	 # */
#define THREAD_FPR5 848 /* offsetof(struct task_struct, thread.fpu.fpr[5])	 # */
#define THREAD_FPR6 856 /* offsetof(struct task_struct, thread.fpu.fpr[6])	 # */
#define THREAD_FPR7 864 /* offsetof(struct task_struct, thread.fpu.fpr[7])	 # */
#define THREAD_FPR8 872 /* offsetof(struct task_struct, thread.fpu.fpr[8])	 # */
#define THREAD_FPR9 880 /* offsetof(struct task_struct, thread.fpu.fpr[9])	 # */
#define THREAD_FPR10 888 /* offsetof(struct task_struct, thread.fpu.fpr[10])	 # */
#define THREAD_FPR11 896 /* offsetof(struct task_struct, thread.fpu.fpr[11])	 # */
#define THREAD_FPR12 904 /* offsetof(struct task_struct, thread.fpu.fpr[12])	 # */
#define THREAD_FPR13 912 /* offsetof(struct task_struct, thread.fpu.fpr[13])	 # */
#define THREAD_FPR14 920 /* offsetof(struct task_struct, thread.fpu.fpr[14])	 # */
#define THREAD_FPR15 928 /* offsetof(struct task_struct, thread.fpu.fpr[15])	 # */
#define THREAD_FPR16 936 /* offsetof(struct task_struct, thread.fpu.fpr[16])	 # */
#define THREAD_FPR17 944 /* offsetof(struct task_struct, thread.fpu.fpr[17])	 # */
#define THREAD_FPR18 952 /* offsetof(struct task_struct, thread.fpu.fpr[18])	 # */
#define THREAD_FPR19 960 /* offsetof(struct task_struct, thread.fpu.fpr[19])	 # */
#define THREAD_FPR20 968 /* offsetof(struct task_struct, thread.fpu.fpr[20])	 # */
#define THREAD_FPR21 976 /* offsetof(struct task_struct, thread.fpu.fpr[21])	 # */
#define THREAD_FPR22 984 /* offsetof(struct task_struct, thread.fpu.fpr[22])	 # */
#define THREAD_FPR23 992 /* offsetof(struct task_struct, thread.fpu.fpr[23])	 # */
#define THREAD_FPR24 1000 /* offsetof(struct task_struct, thread.fpu.fpr[24])	 # */
#define THREAD_FPR25 1008 /* offsetof(struct task_struct, thread.fpu.fpr[25])	 # */
#define THREAD_FPR26 1016 /* offsetof(struct task_struct, thread.fpu.fpr[26])	 # */
#define THREAD_FPR27 1024 /* offsetof(struct task_struct, thread.fpu.fpr[27])	 # */
#define THREAD_FPR28 1032 /* offsetof(struct task_struct, thread.fpu.fpr[28])	 # */
#define THREAD_FPR29 1040 /* offsetof(struct task_struct, thread.fpu.fpr[29])	 # */
#define THREAD_FPR30 1048 /* offsetof(struct task_struct, thread.fpu.fpr[30])	 # */
#define THREAD_FPR31 1056 /* offsetof(struct task_struct, thread.fpu.fpr[31])	 # */
#define THREAD_FCR31 1064 /* offsetof(struct task_struct, thread.fpu.fcr31)	 # */

/* Size of struct page */
#define STRUCT_PAGE_SIZE 32 /* sizeof(struct page)	 # */

/* Linux mm_struct offsets. */
#define MM_USERS 48 /* offsetof(struct mm_struct, mm_users)	 # */
#define MM_PGD 44 /* offsetof(struct mm_struct, pgd)	 # */
#define MM_CONTEXT 344 /* offsetof(struct mm_struct, context)	 # */

#define _PGD_T_SIZE 4 /* sizeof(pgd_t)	 # */
#define _PMD_T_SIZE 4 /* sizeof(pmd_t)	 # */
#define _PTE_T_SIZE 4 /* sizeof(pte_t)	 # */

#define _PGD_T_LOG2 2 /* PGD_T_LOG2	 # */
#define _PTE_T_LOG2 2 /* PTE_T_LOG2	 # */

#define _PGD_ORDER 0 /* PGD_ORDER	 # */
#define _PTE_ORDER 0 /* PTE_ORDER	 # */

#define _PMD_SHIFT 22 /* PMD_SHIFT	 # */
#define _PGDIR_SHIFT 22 /* PGDIR_SHIFT	 # */

#define _PTRS_PER_PGD 1024 /* PTRS_PER_PGD	 # */
#define _PTRS_PER_PMD 1 /* PTRS_PER_PMD	 # */
#define _PTRS_PER_PTE 1024 /* PTRS_PER_PTE	 # */

#define _PAGE_SHIFT 12 /* PAGE_SHIFT	 # */
#define _PAGE_SIZE 4096 /* PAGE_SIZE	 # */

/* Linux sigcontext offsets. */
#define SC_REGS 16 /* offsetof(struct sigcontext, sc_regs)	 # */
#define SC_FPREGS 272 /* offsetof(struct sigcontext, sc_fpregs)	 # */
#define SC_ACX 528 /* offsetof(struct sigcontext, sc_acx)	 # */
#define SC_MDHI 552 /* offsetof(struct sigcontext, sc_mdhi)	 # */
#define SC_MDLO 560 /* offsetof(struct sigcontext, sc_mdlo)	 # */
#define SC_PC 8 /* offsetof(struct sigcontext, sc_pc)	 # */
#define SC_FPC_CSR 532 /* offsetof(struct sigcontext, sc_fpc_csr)	 # */
#define SC_FPC_EIR 536 /* offsetof(struct sigcontext, sc_fpc_eir)	 # */
#define SC_HI1 568 /* offsetof(struct sigcontext, sc_hi1)	 # */
#define SC_LO1 572 /* offsetof(struct sigcontext, sc_lo1)	 # */
#define SC_HI2 576 /* offsetof(struct sigcontext, sc_hi2)	 # */
#define SC_LO2 580 /* offsetof(struct sigcontext, sc_lo2)	 # */
#define SC_HI3 584 /* offsetof(struct sigcontext, sc_hi3)	 # */
#define SC_LO3 588 /* offsetof(struct sigcontext, sc_lo3)	 # */

/* Linux signal numbers. */
#define _SIGHUP 1 /* SIGHUP	 # */
#define _SIGINT 2 /* SIGINT	 # */
#define _SIGQUIT 3 /* SIGQUIT	 # */
#define _SIGILL 4 /* SIGILL	 # */
#define _SIGTRAP 5 /* SIGTRAP	 # */
#define _SIGIOT 6 /* SIGIOT	 # */
#define _SIGABRT 6 /* SIGABRT	 # */
#define _SIGEMT 7 /* SIGEMT	 # */
#define _SIGFPE 8 /* SIGFPE	 # */
#define _SIGKILL 9 /* SIGKILL	 # */
#define _SIGBUS 10 /* SIGBUS	 # */
#define _SIGSEGV 11 /* SIGSEGV	 # */
#define _SIGSYS 12 /* SIGSYS	 # */
#define _SIGPIPE 13 /* SIGPIPE	 # */
#define _SIGALRM 14 /* SIGALRM	 # */
#define _SIGTERM 15 /* SIGTERM	 # */
#define _SIGUSR1 16 /* SIGUSR1	 # */
#define _SIGUSR2 17 /* SIGUSR2	 # */
#define _SIGCHLD 18 /* SIGCHLD	 # */
#define _SIGPWR 19 /* SIGPWR	 # */
#define _SIGWINCH 20 /* SIGWINCH	 # */
#define _SIGURG 21 /* SIGURG	 # */
#define _SIGIO 22 /* SIGIO	 # */
#define _SIGSTOP 23 /* SIGSTOP	 # */
#define _SIGTSTP 24 /* SIGTSTP	 # */
#define _SIGCONT 25 /* SIGCONT	 # */
#define _SIGTTIN 26 /* SIGTTIN	 # */
#define _SIGTTOU 27 /* SIGTTOU	 # */
#define _SIGVTALRM 28 /* SIGVTALRM	 # */
#define _SIGPROF 29 /* SIGPROF	 # */
#define _SIGXCPU 30 /* SIGXCPU	 # */
#define _SIGXFSZ 31 /* SIGXFSZ	 # */

/*  KVM/MIPS Specfic offsets.  */
#define VCPU_ARCH_SIZE 3344 /* sizeof(struct kvm_vcpu_arch)	 # */
#define VCPU_RUN 40 /* offsetof(struct kvm_vcpu, run)	 # */
#define VCPU_HOST_ARCH 208 /* offsetof(struct kvm_vcpu, arch)	 # */
#define VCPU_HOST_EBASE 0 /* offsetof(struct kvm_vcpu_arch, host_ebase)	 # */
#define VCPU_GUEST_EBASE 4 /* offsetof(struct kvm_vcpu_arch, guest_ebase)	 # */
#define VCPU_HOST_STACK 8 /* offsetof(struct kvm_vcpu_arch, host_stack)	 # */
#define VCPU_HOST_GP 12 /* offsetof(struct kvm_vcpu_arch, host_gp)	 # */
#define VCPU_HOST_CP0_BADVADDR 16 /* offsetof(struct kvm_vcpu_arch, host_cp0_badvaddr)	 # */
#define VCPU_HOST_CP0_CAUSE 20 /* offsetof(struct kvm_vcpu_arch, host_cp0_cause)	 # */
#define VCPU_HOST_EPC 24 /* offsetof(struct kvm_vcpu_arch, host_cp0_epc)	 # */
#define VCPU_HOST_ENTRYHI 28 /* offsetof(struct kvm_vcpu_arch, host_cp0_entryhi)	 # */
#define VCPU_GUEST_INST 32 /* offsetof(struct kvm_vcpu_arch, guest_inst)	 # */
#define VCPU_R0 36 /* offsetof(struct kvm_vcpu_arch, gprs[0])	 # */
#define VCPU_R1 40 /* offsetof(struct kvm_vcpu_arch, gprs[1])	 # */
#define VCPU_R2 44 /* offsetof(struct kvm_vcpu_arch, gprs[2])	 # */
#define VCPU_R3 48 /* offsetof(struct kvm_vcpu_arch, gprs[3])	 # */
#define VCPU_R4 52 /* offsetof(struct kvm_vcpu_arch, gprs[4])	 # */
#define VCPU_R5 56 /* offsetof(struct kvm_vcpu_arch, gprs[5])	 # */
#define VCPU_R6 60 /* offsetof(struct kvm_vcpu_arch, gprs[6])	 # */
#define VCPU_R7 64 /* offsetof(struct kvm_vcpu_arch, gprs[7])	 # */
#define VCPU_R8 68 /* offsetof(struct kvm_vcpu_arch, gprs[8])	 # */
#define VCPU_R9 72 /* offsetof(struct kvm_vcpu_arch, gprs[9])	 # */
#define VCPU_R10 76 /* offsetof(struct kvm_vcpu_arch, gprs[10])	 # */
#define VCPU_R11 80 /* offsetof(struct kvm_vcpu_arch, gprs[11])	 # */
#define VCPU_R12 84 /* offsetof(struct kvm_vcpu_arch, gprs[12])	 # */
#define VCPU_R13 88 /* offsetof(struct kvm_vcpu_arch, gprs[13])	 # */
#define VCPU_R14 92 /* offsetof(struct kvm_vcpu_arch, gprs[14])	 # */
#define VCPU_R15 96 /* offsetof(struct kvm_vcpu_arch, gprs[15])	 # */
#define VCPU_R16 100 /* offsetof(struct kvm_vcpu_arch, gprs[16])	 # */
#define VCPU_R17 104 /* offsetof(struct kvm_vcpu_arch, gprs[17])	 # */
#define VCPU_R18 108 /* offsetof(struct kvm_vcpu_arch, gprs[18])	 # */
#define VCPU_R19 112 /* offsetof(struct kvm_vcpu_arch, gprs[19])	 # */
#define VCPU_R20 116 /* offsetof(struct kvm_vcpu_arch, gprs[20])	 # */
#define VCPU_R21 120 /* offsetof(struct kvm_vcpu_arch, gprs[21])	 # */
#define VCPU_R22 124 /* offsetof(struct kvm_vcpu_arch, gprs[22])	 # */
#define VCPU_R23 128 /* offsetof(struct kvm_vcpu_arch, gprs[23])	 # */
#define VCPU_R24 132 /* offsetof(struct kvm_vcpu_arch, gprs[24])	 # */
#define VCPU_R25 136 /* offsetof(struct kvm_vcpu_arch, gprs[25])	 # */
#define VCPU_R26 140 /* offsetof(struct kvm_vcpu_arch, gprs[26])	 # */
#define VCPU_R27 144 /* offsetof(struct kvm_vcpu_arch, gprs[27])	 # */
#define VCPU_R28 148 /* offsetof(struct kvm_vcpu_arch, gprs[28])	 # */
#define VCPU_R29 152 /* offsetof(struct kvm_vcpu_arch, gprs[29])	 # */
#define VCPU_R30 156 /* offsetof(struct kvm_vcpu_arch, gprs[30])	 # */
#define VCPU_R31 160 /* offsetof(struct kvm_vcpu_arch, gprs[31])	 # */
#define VCPU_LO 168 /* offsetof(struct kvm_vcpu_arch, lo)	 # */
#define VCPU_HI 164 /* offsetof(struct kvm_vcpu_arch, hi)	 # */
#define VCPU_PC 172 /* offsetof(struct kvm_vcpu_arch, pc)	 # */
#define VCPU_COP0 440 /* offsetof(struct kvm_vcpu_arch, cop0)	 # */
#define VCPU_GUEST_KERNEL_ASID 1500 /* offsetof(struct kvm_vcpu_arch, guest_kernel_asid)	 # */
#define VCPU_GUEST_USER_ASID 1496 /* offsetof(struct kvm_vcpu_arch, guest_user_asid)	 # */
#define COP0_TLB_HI 320 /* offsetof(struct mips_coproc, reg[10][0])	 # */
#define COP0_STATUS 384 /* offsetof(struct mips_coproc, reg[12][0])	 # */


#endif
