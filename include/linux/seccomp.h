#ifndef _LINUX_SECCOMP_H
#define _LINUX_SECCOMP_H


#ifdef CONFIG_SECCOMP

#include <linux/thread_info.h>
#include <asm/seccomp.h>

typedef struct { int mode; } seccomp_t;

extern void __secure_computing(int);
static inline void secure_computing(int this_syscall)
{
	if (unlikely(test_thread_flag(TIF_SECCOMP)))
		__secure_computing(this_syscall);
}

/* A wrapper for architectures supporting only SECCOMP_MODE_STRICT. */
static inline void secure_computing_strict(int this_syscall)
{
	BUG_ON(secure_computing(this_syscall) != 0);
}

extern long prctl_get_seccomp(void);
extern long prctl_set_seccomp(unsigned long);

static inline int seccomp_mode(seccomp_t *s)
{
	return s->mode;
}

#else /* CONFIG_SECCOMP */

#include <linux/errno.h>

typedef struct { } seccomp_t;

static inline int secure_computing(int this_syscall) { return 0; }
static inline void secure_computing_strict(int this_syscall) { return; }

static inline long prctl_get_seccomp(void)
{
	return -EINVAL;
}

static inline long prctl_set_seccomp(unsigned long arg2)
{
	return -EINVAL;
}

static inline int seccomp_mode(seccomp_t *s)
{
	return 0;
}

#endif /* CONFIG_SECCOMP */

#endif /* _LINUX_SECCOMP_H */
