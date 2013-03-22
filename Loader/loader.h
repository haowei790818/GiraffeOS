/*
 * loader.h
 *
 *  Created on: 2013-3-21
 *      Author: BorisJineman
 */

#ifndef LOADER_H_
#define LOADER_H_

#define CMOS_WRITE(address, value) { WRITE_PORT_UCHAR((PUCHAR)0x70, address); WRITE_PORT_UCHAR((PUCHAR)0x71, value); }

typedef struct _APIC {
/*000*/ struct { unsigned long reserved[4]; } __reserved_01;
/*010*/ struct { unsigned long reserved[4]; } __reserved_02;
/*020*/ struct { /* APIC ID Register */
                unsigned long   __reserved_1      : 24,
                        phys_apic_id    :  4,
                        __reserved_2    :  4;
                unsigned long reserved[3];
        } id;
/*030*/ const struct { /* APIC Version Register */
                unsigned long   version           :  8,
                        __reserved_1    :  8,
                        max_lvt         :  8,
                        __reserved_2    :  8;
                unsigned long reserved[3];
        } version;
/*040*/ struct { unsigned long reserved[4]; } __reserved_03;
/*050*/ struct { unsigned long reserved[4]; } __reserved_04;
/*060*/ struct { unsigned long reserved[4]; } __reserved_05;
/*070*/ struct { unsigned long reserved[4]; } __reserved_06;
/*080*/ struct { /* Task Priority Register */
                unsigned long   priority  :  8,
                        __reserved_1    : 24;
                unsigned long __reserved_2[3];
        } tpr;
/*090*/ const struct { /* Arbitration Priority Register */
                unsigned long   priority  :  8,
                        __reserved_1    : 24;
                unsigned long __reserved_2[3];
        } apr;
/*0A0*/ const struct { /* Processor Priority Register */
                unsigned long   priority  :  8,
                        __reserved_1    : 24;
                unsigned long __reserved_2[3];
        } ppr;
/*0B0*/ struct { /* End Of Interrupt Register */
                unsigned long   eoi;
                unsigned long reserved[3];
        } eoi;
/*0C0*/ struct { unsigned long reserved[4]; } __reserved_07;
/*0D0*/ struct { /* Logical Destination Register */
                unsigned long   __reserved_1      : 24,
                        logical_dest    :  8;
                unsigned long __reserved_2[3];
        } ldr;
/*0E0*/ struct { /* Destination Format Register */
                unsigned long   __reserved_1      : 28,
                        model           :  4;
                unsigned long __reserved_2[3];
        } dfr;
/*0F0*/ struct { /* Spurious Interrupt Vector Register */
                unsigned long     spurious_vector :  8,
                        apic_enabled    :  1,
                        focus_cpu       :  1,
                        __reserved_2    : 22;
                unsigned long __reserved_3[3];
        } svr;
/*100*/ struct { /* In Service Register */
/*170*/         unsigned long bitfield;
                unsigned long reserved[3];
        } isr [8];
/*180*/ struct { /* Trigger Mode Register */
/*1F0*/         unsigned long bitfield;
                unsigned long reserved[3];
        } tmr [8];
/*200*/ struct { /* Interrupt Request Register */
/*270*/         unsigned long bitfield;
                unsigned long reserved[3];
        } irr [8];
/*280*/ union { /* Error Status Register */
                struct {
                        unsigned long   send_EIP_error                     :  1,
                                receive_EIP_error                :  1,
                                send_accept_error               :  1,
                                receive_accept_error            :  1,
                                __reserved_1                    :  1,
                                send_illegal_vector             :  1,
                                receive_illegal_vector          :  1,
                                illegal_register_address        :  1,
                                __reserved_2                    : 24;
                        unsigned long __reserved_3[3];
                } error_bits;
                struct {
                        unsigned long errors;
                        unsigned long __reserved_3[3];
                } all_errors;
        } esr;
/*290*/ struct { unsigned long reserved[4]; } __reserved_08;
/*2A0*/ struct { unsigned long reserved[4]; } __reserved_09;
/*2B0*/ struct { unsigned long reserved[4]; } __reserved_10;
/*2C0*/ struct { unsigned long reserved[4]; } __reserved_11;
/*2D0*/ struct { unsigned long reserved[4]; } __reserved_12;
/*2E0*/ struct { unsigned long reserved[4]; } __reserved_13;
/*2F0*/ struct { unsigned long reserved[4]; } __reserved_14;
/*300*/ struct { /* Interrupt Command Register 1 */
                unsigned long   vector                    :  8,
                        delivery_mode           :  3,
                        destination_mode        :  1,
                        delivery_status         :  1,
                        __reserved_1            :  1,
                        level                   :  1,
                        trigger                 :  1,
                        __reserved_2            :  2,
                        shorthand               :  2,
                        __reserved_3            :  12;
                unsigned long __reserved_4[3];
        } icr1;
/*310*/ struct { /* Interrupt Command Register 2 */
                union {
                        unsigned long   __reserved_1      : 24,
                                phys_dest       :  4,
                                __reserved_2    :  4;
                        unsigned long   __reserved_3      : 24,
                                logical_dest    :  8;
                } dest;
                unsigned long __reserved_4[3];
        } icr2;
/*320*/ struct { /* LVT - Timer */
                unsigned long   vector            :  8,
                        __reserved_1    :  4,
                        delivery_status :  1,
                        __reserved_2    :  3,
                        mask            :  1,
                        timer_mode      :  1,
                        __reserved_3    : 14;
                unsigned long __reserved_4[3];
        } lvt_timer;
/*330*/ struct { unsigned long reserved[4]; } __reserved_15;
/*340*/ struct { /* LVT - Performance Counter */
                unsigned long   vector            :  8,
                        delivery_mode   :  3,
                        __reserved_1    :  1,
                        delivery_status :  1,
                        __reserved_2    :  3,
                        mask            :  1,
                        __reserved_3    : 15;
                unsigned long __reserved_4[3];
        } lvt_pc;
/*350*/ struct { /* LVT - LINT0 */
                unsigned long   vector            :  8,
                        delivery_mode   :  3,
                        __reserved_1    :  1,
                        delivery_status :  1,
                        polarity        :  1,
                        remote_irr      :  1,
                        trigger         :  1,
                        mask            :  1,
                        __reserved_2    : 15;
                unsigned long __reserved_3[3];
        } lvt_lint0;
/*360*/ struct { /* LVT - LINT1 */
                unsigned long   vector            :  8,
                        delivery_mode   :  3,
                        __reserved_1    :  1,
                        delivery_status :  1,
                        polarity        :  1,
                        remote_irr      :  1,
                        trigger         :  1,
                        mask            :  1,
                        __reserved_2    : 15;
                unsigned long __reserved_3[3];
        } lvt_lint1;
/*370*/ struct { /* LVT - Error */
                unsigned long   vector            :  8,
                        __reserved_1    :  4,
                        delivery_status :  1,
                        __reserved_2    :  3,
                        mask            :  1,
                        __reserved_3    : 15;
                unsigned long __reserved_4[3];
        } lvt_error;
/*380*/ struct { /* Timer Initial Count Register */
                unsigned long   initial_count;
                unsigned long __reserved_2[3];
        } timer_icr;
/*390*/ const struct { /* Timer Current Count Register */
                unsigned long   curr_count;
                unsigned long __reserved_2[3];
        } timer_ccr;
/*3A0*/ struct { unsigned long reserved[4]; } __reserved_16;
/*3B0*/ struct { unsigned long reserved[4]; } __reserved_17;
/*3C0*/ struct { unsigned long reserved[4]; } __reserved_18;
/*3D0*/ struct { unsigned long reserved[4]; } __reserved_19;
/*3E0*/ struct { /* Timer Divide Configuration Register */
                unsigned long   divisor           :  4,
                        __reserved_1    : 28;
                unsigned long __reserved_2[3];
        } timer_dcr;
/*3F0*/ struct { unsigned long reserved[4]; } __reserved_20;
} APIC, *PAPIC;
#pragma pack()



#endif /* LOADER_H_ */
