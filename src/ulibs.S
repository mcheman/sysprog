/*
** sccs id:	@(#)ulibs.S	1.1	4/22/16
**
** File:	ulibs.S
**
** Author:	CSCI-452 class of 20155
**
** Contributor: Matthew Cheman mnc3139
**
** Description:	assembly-language user-level library functions
*/

#define	__SP_ASM__

#include "syscall.h"

/*
** System call stubs
**
** All have the same structure:
**
**      move a code into EAX
**      generate the interrupt
**      return to the caller
**
** As these are simple "leaf" routines, we don't use
** the standard enter/leave method to set up a stack
** frame - that takes time, and we don't really need it.
*/

#define SYSCALL(name) \
	.globl	name              ; \
name:                             ; \
	movl	$SYS_##name, %eax ; \
	int	$INT_VEC_SYSCALL  ; \
	ret

SYSCALL(exit)
SYSCALL(readch)
SYSCALL(writes)
SYSCALL(fork)
SYSCALL(exec)
SYSCALL(wait)
SYSCALL(kill)
SYSCALL(sleep)
SYSCALL(getpid)
SYSCALL(getppid)
SYSCALL(gettime)
SYSCALL(readfile)
SYSCALL(writefile)
SYSCALL(clearfiles)
SYSCALL(getgfxcontext)
SYSCALL(drawscreen)
SYSCALL(reqport)
SYSCALL(relport)
SYSCALL(send)

// This is a bogus system call; it's here so that we can test
// our handling of out-of-range syscall codes in the syscall ISR.

SYSCALL(bogus)

/*
** do_exit()
**
** support function used as the "return to" address
** for user main() routines; it just calls exit(%eax)
*/

	.globl	do_exit
do_exit:
	pushl	%eax	// use whatever was in EAX as the status
	call	exit	// terminate this process
