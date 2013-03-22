;
;
; PROJECT:         GiraffeOS
; FILE:            ap_boot.asm
; PURPOSE:         Boot code for application processors
; PROGRAMMER:      BorisJineman(xuezhe.liu@hotmail.com)
; UPDATE HISTORY:
;                  Created 2013-3-21
;
;	The Boot Code Start With 0x00080000
;	The Data On 0x00081000



;
; Segment selectors
;
%define KERNEL_CS     (0x8)
%define KERNEL_DS     (0x10)
%define MAIN_CS		  (0x18)
%define MAIN_DS       (0x20)


; 16 bit code
BITS 16					;start with bit16(real mode)


_APstart:
	  cli






test_real_start:
	  xor   ax,		ax					;clear ax,ds,ss
	  mov	ds,		ax
	  mov	ss,		ax

	  mov	ax,		08000H				;set seg 0x00080000
	  mov	ds,		ax
	  mov	bx,		01000H
	  mov	cx,		100					;loop to 100

	  mov	ax,		00H					;begin with 0
s:	  mov	[bx],	ax					;just write
	  inc	bx						;bx++
	  inc	bx						;bx++
	  inc	ax						;value++
	  loop	s						;go loop



	  in	ax,	92h					; enable A20
	  or    ax, 00000010b
	  out   92h, ax


	  xor   ax,		ax					;clear ax,ds,ss
	  mov	ds,		ax
	  mov	ss,		ax

      mov	ax,		08000H				;set the seg 0x00080000
	  mov	ds,		ax

      mov   eax, 00H + APgdt - _APstart
	  lgdt  [eax]






  mov   eax, cr0			; start cpu protect mode,32bit£¬
  or    eax, 1
  mov   cr0, eax



jmp dword KERNEL_CS:000080000H + protected_mode_begin - _APstart



BITS 32


protected_mode_begin:

  mov	ax, MAIN_DS
  mov	ds, ax
  mov	es, ax
  mov	fs, ax
  mov	gs, ax
  mov	ss, ax


  mov   esp, 01F000000h		; the stack 0xDF000000


start_main:
  jmp $
  jmp  MAIN_CS:0			; main fun ptr 0xC0000000


  hlt




; GDT reg
APgdt:
; Limit
  dw    5*8-1
; Base
  dd	080000H + gdt - _APstart

; GDT table
gdt:
  dw	0x0       ; Null descriptor
  dw	0x0
  dw	0x0
  dw	0x0

  dw	0xffff    ; Kernel code descriptor	0x00000000~0xFFFFFFFF
  dw	0x0000
  dw	0x9a00
  dw	0x00cf

  dw	0xffff    ; Kernel data descriptor	0x00000000~0xFFFFFFFF
  dw	0x0000
  dw	0x9200
  dw	0x00cf

  dw	0xffff    ; Main code descriptor	0xC0000000~0xFFFFFFFF
  dw	0x0000
  dw	0x9a00
  dw	0xc0c3

  dw	0xffff    ; Main data descriptor	0xC0000000~0xFFFFFFFF
  dw	0x0000
  dw	0x9200
  dw	0xc0c3

