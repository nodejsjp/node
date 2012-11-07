OPTION	DOTNAME
EXTERN	OPENSSL_cpuid_setup:NEAR
.CRT$XCU	SEGMENT READONLY DWORD
		DQ	OPENSSL_cpuid_setup

.CRT$XCU	ENDS
.text$	SEGMENT ALIGN(64) 'CODE'

PUBLIC	OPENSSL_atomic_add

ALIGN	16
OPENSSL_atomic_add	PROC PUBLIC
	mov	eax,DWORD PTR[rcx]
$L$spin::	lea	r8,QWORD PTR[rax*1+rdx]
DB	0f0h

	cmpxchg	DWORD PTR[rcx],r8d
	jne	$L$spin
	mov	eax,r8d
DB	048h,098h

	DB	0F3h,0C3h		;repret
OPENSSL_atomic_add	ENDP

PUBLIC	OPENSSL_rdtsc

ALIGN	16
OPENSSL_rdtsc	PROC PUBLIC
	rdtsc
	shl	rdx,32
	or	rax,rdx
	DB	0F3h,0C3h		;repret
OPENSSL_rdtsc	ENDP

PUBLIC	OPENSSL_ia32_cpuid

ALIGN	16
OPENSSL_ia32_cpuid	PROC PUBLIC
	mov	r8,rbx

	xor	eax,eax
	cpuid
	mov	r11d,eax

	xor	eax,eax
	cmp	ebx,0756e6547h
	setne	al
	mov	r9d,eax
	cmp	edx,049656e69h
	setne	al
	or	r9d,eax
	cmp	ecx,06c65746eh
	setne	al
	or	r9d,eax
	jz	$L$intel

	cmp	ebx,068747541h
	setne	al
	mov	r10d,eax
	cmp	edx,069746E65h
	setne	al
	or	r10d,eax
	cmp	ecx,0444D4163h
	setne	al
	or	r10d,eax
	jnz	$L$intel


	mov	eax,080000000h
	cpuid
	cmp	eax,080000008h
	jb	$L$intel

	mov	eax,080000008h
	cpuid
	movzx	r10,cl
	inc	r10

	mov	eax,1
	cpuid
	bt	edx,28
	jnc	$L$done
	shr	ebx,16
	cmp	bl,r10b
	ja	$L$done
	and	edx,0efffffffh
	jmp	$L$done

$L$intel::
	cmp	r11d,4
	mov	r10d,-1
	jb	$L$nocacheinfo

	mov	eax,4
	mov	ecx,0
	cpuid
	mov	r10d,eax
	shr	r10d,14
	and	r10d,0fffh

$L$nocacheinfo::
	mov	eax,1
	cpuid
	cmp	r9d,0
	jne	$L$notintel
	or	edx,000100000h
	and	ah,15
	cmp	ah,15
	je	$L$notintel
	or	edx,040000000h
$L$notintel::
	bt	edx,28
	jnc	$L$done
	and	edx,0efffffffh
	cmp	r10d,0
	je	$L$done

	or	edx,010000000h
	shr	ebx,16
	cmp	bl,1
	ja	$L$done
	and	edx,0efffffffh
$L$done::
	shl	rcx,32
	mov	eax,edx
	mov	rbx,r8
	or	rax,rcx
	DB	0F3h,0C3h		;repret
OPENSSL_ia32_cpuid	ENDP

PUBLIC	OPENSSL_cleanse

ALIGN	16
OPENSSL_cleanse	PROC PUBLIC
	xor	rax,rax
	cmp	rdx,15
	jae	$L$ot
	cmp	rdx,0
	je	$L$ret
$L$ittle::
	mov	BYTE PTR[rcx],al
	sub	rdx,1
	lea	rcx,QWORD PTR[1+rcx]
	jnz	$L$ittle
$L$ret::
	DB	0F3h,0C3h		;repret
ALIGN	16
$L$ot::
	test	rcx,7
	jz	$L$aligned
	mov	BYTE PTR[rcx],al
	lea	rdx,QWORD PTR[((-1))+rdx]
	lea	rcx,QWORD PTR[1+rcx]
	jmp	$L$ot
$L$aligned::
	mov	QWORD PTR[rcx],rax
	lea	rdx,QWORD PTR[((-8))+rdx]
	test	rdx,-8
	lea	rcx,QWORD PTR[8+rcx]
	jnz	$L$aligned
	cmp	rdx,0
	jne	$L$ittle
	DB	0F3h,0C3h		;repret
OPENSSL_cleanse	ENDP
PUBLIC	OPENSSL_wipe_cpu

ALIGN	16
OPENSSL_wipe_cpu	PROC PUBLIC
	pxor	xmm0,xmm0
	pxor	xmm1,xmm1
	pxor	xmm2,xmm2
	pxor	xmm3,xmm3
	pxor	xmm4,xmm4
	pxor	xmm5,xmm5
	xor	rcx,rcx
	xor	rdx,rdx
	xor	r8,r8
	xor	r9,r9
	xor	r10,r10
	xor	r11,r11
	lea	rax,QWORD PTR[8+rsp]
	DB	0F3h,0C3h		;repret
OPENSSL_wipe_cpu	ENDP

.text$	ENDS
END
