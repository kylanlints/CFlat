	.file	"test.c"
# GNU C17 (Ubuntu 11.4.0-1ubuntu1~22.04) version 11.4.0 (x86_64-linux-gnu)
#	compiled by GNU C version 11.4.0, GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl version isl-0.24-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -mtune=generic -march=x86-64 -O3 -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection -fcf-protection
	.text
	.p2align 4
	.globl	hi2
	.type	hi2, @function
hi2:
.LFB23:
	.cfi_startproc
	endbr64	
# src/test.c:4:     float b = a2 + 2.0;
	addss	.LC0(%rip), %xmm0	#, b
# src/test.c:5:     return b;
	cvttss2sil	%xmm0, %eax	# b, tmp85
# src/test.c:6: }
	ret	
	.cfi_endproc
.LFE23:
	.size	hi2, .-hi2
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"%d"
	.text
	.p2align 4
	.globl	square
	.type	square, @function
square:
.LFB25:
	.cfi_startproc
	endbr64	
	pushq	%r15	#
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14	#
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13	#
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12	#
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp	#
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
# src/test.c:58:         scanf("%d", &a);
	leaq	.LC1(%rip), %rbp	#, tmp157
# src/test.c:22: int square(int num) {
	pushq	%rbx	#
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$152, %rsp	#,
	.cfi_def_cfa_offset 208
# src/test.c:22: int square(int num) {
	movl	%edi, 4(%rsp)	# tmp322, %sfp
# src/test.c:58:         scanf("%d", &a);
	leaq	24(%rsp), %rsi	#, tmp156
	movq	%rbp, %rdi	# tmp157,
# src/test.c:22: int square(int num) {
	movq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp323
	movq	%rax, 136(%rsp)	# tmp323, D.2628
	xorl	%eax, %eax	# tmp323
# src/test.c:25:     int a = 3;
	movl	$3, 24(%rsp)	#, a
# src/test.c:26:         int b = 4;
	movl	$4, 28(%rsp)	#, b
# src/test.c:27:         int c = 5;
	movl	$5, 32(%rsp)	#, c
# src/test.c:28:         int d = a / 5 + b * 8 - c * 5 * c - 5 * a * a;
	movl	$-138, 36(%rsp)	#, d
# src/test.c:29:         int e = 5;
	movl	$5, 40(%rsp)	#, e
# src/test.c:30:         int f = 5;
	movl	$5, 44(%rsp)	#, f
# src/test.c:31:         int g = 5;
	movl	$5, 48(%rsp)	#, g
# src/test.c:32:         int h = 5;
	movl	$5, 52(%rsp)	#, h
# src/test.c:33:         int j = 5;
	movl	$5, 56(%rsp)	#, j
# src/test.c:34:         int k = 5;
	movl	$5, 60(%rsp)	#, k
# src/test.c:35:         int l = 5;
	movl	$5, 64(%rsp)	#, l
# src/test.c:36:         int i = 5;
	movl	$5, 68(%rsp)	#, i
# src/test.c:37:         int u = 5;
	movl	$5, 72(%rsp)	#, u
# src/test.c:38:         int y = 5;
	movl	$5, 76(%rsp)	#, y
# src/test.c:39:         int t = 5;
	movl	$5, 80(%rsp)	#, t
# src/test.c:40:         int r = 5;
	movl	$5, 84(%rsp)	#, r
# src/test.c:41:         int w = 5;
	movl	$5, 88(%rsp)	#, w
# src/test.c:42:         int q = 5;
	movl	$5, 92(%rsp)	#, q
# src/test.c:43:         int s = 5;
	movl	$5, 96(%rsp)	#, s
# src/test.c:44:         int dd = 5;
	movl	$5, 100(%rsp)	#, dd
# src/test.c:45:         int aa = 6;
	movl	$6, 104(%rsp)	#, aa
# src/test.c:46:         int bb = 5;
	movl	$5, 108(%rsp)	#, bb
# src/test.c:47:         int ccc = 5;
	movl	$5, 112(%rsp)	#, ccc
# src/test.c:48:         int ddd = 5;
	movl	$5, 116(%rsp)	#, ddd
# src/test.c:49:         int ee = 5;
	movl	$5, 120(%rsp)	#, ee
# src/test.c:50:         int ff = 5;
	movl	$5, 124(%rsp)	#, ff
# src/test.c:51:         int gg = 5;
	movl	$5, 128(%rsp)	#, gg
# src/test.c:52:         int hh = 5;
	movl	$5, 132(%rsp)	#, hh
# src/test.c:58:         scanf("%d", &a);
	call	__isoc99_scanf@PLT	#
# src/test.c:59:         scanf("%d", &b);
	movq	%rbp, %rdi	# tmp157,
	leaq	28(%rsp), %rsi	#, tmp158
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:60:         scanf("%d", &c);
	movq	%rbp, %rdi	# tmp157,
	leaq	32(%rsp), %rsi	#, tmp160
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:61:         scanf("%d", &d);
	movq	%rbp, %rdi	# tmp157,
	leaq	36(%rsp), %rsi	#, tmp162
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:62:         scanf("%d", &e);
	movq	%rbp, %rdi	# tmp157,
	leaq	40(%rsp), %rsi	#, tmp164
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:63:         scanf("%d", &f);
	movq	%rbp, %rdi	# tmp157,
	leaq	44(%rsp), %rsi	#, tmp166
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:64:         scanf("%d", &g);
	movq	%rbp, %rdi	# tmp157,
	leaq	48(%rsp), %rsi	#, tmp168
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:65:         scanf("%d", &h);
	movq	%rbp, %rdi	# tmp157,
	leaq	52(%rsp), %rsi	#, tmp170
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:66:         scanf("%d", &j);
	movq	%rbp, %rdi	# tmp157,
	leaq	56(%rsp), %rsi	#, tmp172
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:67:         scanf("%d", &k);
	movq	%rbp, %rdi	# tmp157,
	leaq	60(%rsp), %rsi	#, tmp174
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:68:         scanf("%d", &i);
	movq	%rbp, %rdi	# tmp157,
	leaq	68(%rsp), %rsi	#, tmp176
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:69:         scanf("%d", &l);
	movq	%rbp, %rdi	# tmp157,
	leaq	64(%rsp), %rsi	#, tmp178
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:70:         scanf("%d", &u);
	movq	%rbp, %rdi	# tmp157,
	leaq	72(%rsp), %rsi	#, tmp180
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:71:         scanf("%d", &y);
	movq	%rbp, %rdi	# tmp157,
	leaq	76(%rsp), %rsi	#, tmp182
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:72:         scanf("%d", &t);
	movq	%rbp, %rdi	# tmp157,
	leaq	80(%rsp), %rsi	#, tmp184
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:73:         scanf("%d", &r);
	movq	%rbp, %rdi	# tmp157,
	leaq	84(%rsp), %rsi	#, tmp186
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:74:         scanf("%d", &w);
	movq	%rbp, %rdi	# tmp157,
	leaq	88(%rsp), %rsi	#, tmp188
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:75:         scanf("%d", &q);
	movq	%rbp, %rdi	# tmp157,
	leaq	92(%rsp), %rsi	#, tmp190
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:76:         scanf("%d", &s);
	movq	%rbp, %rdi	# tmp157,
	leaq	96(%rsp), %rsi	#, tmp192
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:77:         scanf("%d", &dd);
	movq	%rbp, %rdi	# tmp157,
	leaq	100(%rsp), %rsi	#, tmp194
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:78:         scanf("%d", &aa);
	movq	%rbp, %rdi	# tmp157,
	leaq	104(%rsp), %rsi	#, tmp196
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:79:         scanf("%d", &bb);
	movq	%rbp, %rdi	# tmp157,
	leaq	108(%rsp), %rsi	#, tmp198
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:80:         scanf("%d", &ccc);
	movq	%rbp, %rdi	# tmp157,
	leaq	112(%rsp), %rsi	#, tmp200
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:81:         scanf("%d", &ddd);
	movq	%rbp, %rdi	# tmp157,
	leaq	116(%rsp), %rsi	#, tmp202
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:82:         scanf("%d", &ee);
	movq	%rbp, %rdi	# tmp157,
	leaq	120(%rsp), %rsi	#, tmp204
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:83:         scanf("%d", &ff);
	movq	%rbp, %rdi	# tmp157,
	leaq	124(%rsp), %rsi	#, tmp206
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:84:         scanf("%d", &gg);
	movq	%rbp, %rdi	# tmp157,
	leaq	128(%rsp), %rsi	#, tmp208
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:85:         scanf("%d", &hh);
	movq	%rbp, %rdi	# tmp157,
	leaq	132(%rsp), %rsi	#, tmp210
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	40(%rsp), %eax	# e, e.11_5
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	44(%rsp), %ecx	# f, f.13_7
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	52(%rsp), %esi	# h, h.17_11
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	56(%rsp), %edi	# j, j.19_13
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	imull	%eax, %eax	# e.11_5, e.11_5
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	%ecx, %edx	# f.13_7, tmp213
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	60(%rsp), %r8d	# k, k.21_15
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	64(%rsp), %r9d	# l, l.23_17
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	imull	%ecx, %edx	# f.13_7, tmp213
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	68(%rsp), %r10d	# i, i.25_19
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	92(%rsp), %r14d	# q, q.37_31
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	imull	%esi, %esi	# h.17_11, tmp215
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	88(%rsp), %r13d	# w, w.35_29
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	84(%rsp), %r12d	# r, r.33_27
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	imull	%edi, %edi	# j.19_13, tmp216
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	80(%rsp), %ebp	# t, t.31_25
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	72(%rsp), %r11d	# u, u.27_21
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	%eax, 8(%rsp)	# e.11_5, %sfp
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	128(%rsp), %eax	# gg, gg
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	imull	%r8d, %r8d	# k.21_15, tmp217
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	76(%rsp), %ebx	# y, y.29_23
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	%edx, 12(%rsp)	# tmp213, %sfp
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	imull	48(%rsp), %ecx	# g, tmp214
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	imull	%r9d, %r9d	# l.23_17, tmp218
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	imull	%r10d, %r10d	# i.25_19, tmp219
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	cltd
	idivl	132(%rsp)	# hh
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	imull	%r14d, %r14d	# q.37_31, tmp225
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	imull	%r13d, %r13d	# w.35_29, tmp224
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	imull	%r12d, %r12d	# r.33_27, tmp223
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	imull	%ebp, %ebp	# t.31_25, tmp222
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	imull	%r11d, %r11d	# u.27_21, tmp220
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	imull	%ebx, %ebx	# y.29_23, tmp221
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	%eax, %r15d	# gg, tmp227
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	124(%rsp), %eax	# ff, ff
	cltd
	idivl	%r15d	# tmp227
	movl	%eax, %r15d	# ff, tmp231
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	120(%rsp), %eax	# ee, ee
	cltd
	idivl	%r15d	# tmp231
	movl	%eax, %r15d	# ee, tmp235
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	116(%rsp), %eax	# ddd, ddd
	cltd
	idivl	%r15d	# tmp235
	movl	%eax, %r15d	# ddd, tmp239
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	112(%rsp), %eax	# ccc, ccc
	cltd
	idivl	%r15d	# tmp239
	movl	%eax, %r15d	# ccc, tmp243
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	108(%rsp), %eax	# bb, bb
	cltd
	idivl	%r15d	# tmp243
	movl	%eax, %r15d	# bb, tmp247
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	104(%rsp), %eax	# aa, aa
	cltd
	idivl	%r15d	# tmp247
	movl	%eax, %r15d	# aa, tmp251
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	100(%rsp), %eax	# dd, dd
	cltd
	idivl	%r15d	# tmp251
	movl	%eax, %r15d	# dd, tmp255
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	96(%rsp), %eax	# s, s
	cltd
	idivl	%r15d	# tmp255
	movl	%eax, %r15d	# s, tmp259
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	%r14d, %eax	# tmp225, tmp225
	cltd
	idivl	%r15d	# tmp259
	movl	%eax, %r14d	# tmp225, tmp263
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	%r13d, %eax	# tmp224, tmp224
	cltd
	idivl	%r14d	# tmp263
	movl	%eax, %r13d	# tmp224, tmp266
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	%r12d, %eax	# tmp223, tmp223
	cltd
	idivl	%r13d	# tmp266
	movl	%eax, %r12d	# tmp223, tmp269
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	%ebp, %eax	# tmp222, tmp222
	cltd
	idivl	%r12d	# tmp269
	movl	%eax, %ebp	# tmp222, tmp272
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	%ebx, %eax	# tmp221, tmp221
	cltd
	idivl	%ebp	# tmp272
	movl	%eax, %ebx	# tmp221, tmp275
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	%r11d, %eax	# tmp220, tmp220
	cltd
	idivl	%ebx	# tmp275
	movl	%eax, %r11d	# tmp220, tmp278
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	%r10d, %eax	# tmp219, tmp219
	cltd
	idivl	%r11d	# tmp278
	movl	%eax, %r10d	# tmp219, tmp281
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	%r9d, %eax	# tmp218, tmp218
	cltd
	idivl	%r10d	# tmp281
	movl	%eax, %r9d	# tmp218, tmp284
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	%r8d, %eax	# tmp217, tmp217
	cltd
	idivl	%r9d	# tmp284
	movl	%eax, %r8d	# tmp217, tmp287
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	%edi, %eax	# tmp216, tmp216
	cltd
	idivl	%r8d	# tmp287
	movl	%eax, %edi	# tmp216, tmp290
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	%esi, %eax	# tmp215, tmp215
	cltd
	idivl	%edi	# tmp290
	movl	%eax, %esi	# tmp215, tmp293
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	%ecx, %eax	# tmp214, tmp214
	cltd
	idivl	%esi	# tmp293
	movl	%eax, %ecx	# tmp214, tmp296
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	12(%rsp), %eax	# %sfp, tmp213
	cltd
	idivl	%ecx	# tmp296
	movl	%eax, %ecx	# tmp213, tmp299
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	8(%rsp), %eax	# %sfp, tmp212
	cltd
	idivl	%ecx	# tmp299
	movl	%eax, %ecx	# tmp212, tmp302
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	movl	36(%rsp), %eax	# d, d
	cltd
	idivl	%ecx	# tmp302
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	addl	28(%rsp), %eax	# b, tmp308
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	subl	32(%rsp), %eax	# c, tmp310
# src/test.c:86:         int cc = (a + (b - c + d / (e*e / (f*f / (g*f / (h*h / (j*j / (k*k / (l*l / (i*i / (u*u / (y*y / (t*t / (r*r / (w*w / (q*q / (s / (dd / (aa / (bb / (ccc / (ddd / (ee / (ff / (gg /  (hh / (ii / (jj / (kk / ((ll * kk) / (mm)))))))))))))))))))))))))))))));
	addl	24(%rsp), %eax	# a, cc
# src/test.c:87:     return 3/(num*cc*3);
	imull	4(%rsp), %eax	# %sfp, tmp313
# src/test.c:87:     return 3/(num*cc*3);
	leal	(%rax,%rax,2), %ecx	#, tmp316
# src/test.c:87:     return 3/(num*cc*3);
	movl	$3, %eax	#, tmp319
	cltd
	idivl	%ecx	# tmp316
# src/test.c:88: }
	movq	136(%rsp), %rdx	# D.2628, tmp324
	subq	%fs:40, %rdx	# MEM[(<address-space-1> long unsigned int *)40B], tmp324
	jne	.L6	#,
	addq	$152, %rsp	#,
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx	#
	.cfi_def_cfa_offset 48
	popq	%rbp	#
	.cfi_def_cfa_offset 40
	popq	%r12	#
	.cfi_def_cfa_offset 32
	popq	%r13	#
	.cfi_def_cfa_offset 24
	popq	%r14	#
	.cfi_def_cfa_offset 16
	popq	%r15	#
	.cfi_def_cfa_offset 8
	ret	
.L6:
	.cfi_restore_state
	call	__stack_chk_fail@PLT	#
	.cfi_endproc
.LFE25:
	.size	square, .-square
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB24:
	.cfi_startproc
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
# src/test.c:15:     scanf("%d", &x);
	leaq	.LC1(%rip), %rbp	#, tmp89
	movq	%rbp, %rdi	# tmp89,
# src/test.c:8: int main(int argc, char* argv[]) {
	subq	$16, %rsp	#,
	.cfi_def_cfa_offset 32
# src/test.c:8: int main(int argc, char* argv[]) {
	movq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp97
	movq	%rax, 8(%rsp)	# tmp97, D.2635
	xorl	%eax, %eax	# tmp97
# src/test.c:15:     scanf("%d", &x);
	movq	%rsp, %rsi	#, tmp88
# src/test.c:12:     int x = 5;
	movl	$5, (%rsp)	#, x
# src/test.c:13:     int y = 10;
	movl	$10, 4(%rsp)	#, y
# src/test.c:15:     scanf("%d", &x);
	call	__isoc99_scanf@PLT	#
# src/test.c:16:     scanf("%d", &y);
	leaq	4(%rsp), %rsi	#, tmp90
	movq	%rbp, %rdi	# tmp89,
	xorl	%eax, %eax	#
	call	__isoc99_scanf@PLT	#
# src/test.c:17:     square(3);
	movl	$3, %edi	#,
	xorl	%eax, %eax	#
	call	square	#
# /usr/include/x86_64-linux-gnu/bits/stdio2.h:112:   return __printf_chk (__USE_FORTIFY_LEVEL - 1, __fmt, __va_arg_pack ());
	xorl	%eax, %eax	#
	movq	%rbp, %rsi	# tmp89,
	movl	$1, %edi	#,
# src/test.c:18:     printf("%d", z*x);
	imull	$23901243, (%rsp), %edx	#, x, tmp92
# /usr/include/x86_64-linux-gnu/bits/stdio2.h:112:   return __printf_chk (__USE_FORTIFY_LEVEL - 1, __fmt, __va_arg_pack ());
	call	__printf_chk@PLT	#
# src/test.c:19: }
	movq	8(%rsp), %rax	# D.2635, tmp98
	subq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp98
	jne	.L10	#,
	addq	$16, %rsp	#,
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax	#
	popq	%rbp	#
	.cfi_def_cfa_offset 8
	ret	
.L10:
	.cfi_restore_state
	call	__stack_chk_fail@PLT	#
	.cfi_endproc
.LFE24:
	.size	main, .-main
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC0:
	.long	1073741824
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
