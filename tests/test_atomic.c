#include <stdio.h>
#include <arch_atomic.h>
#include <assert.h>

#if (defined(__i386__) || defined(__x86_64__))
#define HAS_ATOMIC_BYTE
#define HAS_ATOMIC_SHORT
#endif

struct testvals {
#ifdef HAS_ATOMIC_BYTE
	unsigned char c;
#endif
#ifdef HAS_ATOMIC_SHORT
	unsigned short s;
#endif
	unsigned int i;
	unsigned long l;
};

static struct testvals vals;

#define do_test(ptr)				\
do {						\
	__typeof__(*(ptr)) v;			\
						\
	atomic_add(ptr, 10);			\
	assert(atomic_read(ptr) == 10);		\
	atomic_add(ptr, -11UL);			\
	assert(atomic_read(ptr) == (__typeof__(*(ptr)))-1UL);	\
	v = cmpxchg(ptr, -1UL, 22);		\
	assert(atomic_read(ptr) == 22);		\
	assert(v == (__typeof__(*(ptr)))-1UL);	\
	v = cmpxchg(ptr, 33, 44);		\
	assert(atomic_read(ptr) == 22);		\
	assert(v == 22);			\
	v = xchg(ptr, 55);			\
	assert(atomic_read(ptr) == 55);		\
	assert(v == 22);			\
	atomic_set(ptr, 22);			\
	atomic_inc(ptr);			\
	assert(atomic_read(ptr) == 23);		\
	atomic_dec(ptr);			\
	assert(atomic_read(ptr) == 22);		\
	v = atomic_add_return(ptr, 100);	\
	assert(v == 122);			\
	assert(atomic_read(ptr) == 122);	\
	v = atomic_sub_return(ptr, 1);		\
	assert(v == 121);			\
	assert(atomic_read(ptr) == 121);	\
} while (0)

int main(int argc, char **argv)
{
#ifdef HAS_ATOMIC_BYTE
	do_test(&vals.c);
#endif
#ifdef HAS_ATOMIC_SHORT
	do_test(&vals.s);
#endif
	do_test(&vals.i);
	do_test(&vals.l);
	printf("Atomic ops test OK\n");

	return 0;
}
