/*
   assert2(condition, msg)
   does nothing if <condition> is true
   otherwise prints error <msg> (and more), and terminates 
   -- inspired by gnu assert() macro from /usr/include/assert.h
*/
#ifndef assert2_included
# define assert2_included

extern void __assert2(const char *filename, int linenum, const char *expr, char* msg);

# ifndef NDEBUG
#  ifdef __STDC__
#  define assert2(e, msg)       ((e) ? (void)0 : __assert2(__FILE__, __LINE__, #e, msg))
#  else   /* PCC */
#  define assert2(e, msg)       ((e) ? (void)0 : __assert2(__FILE__, __LINE__, "e", msg))
#  endif
# else  /* NDEBUG */
#  define assert2(e, msg) 1
# endif 

#endif /* assert2_included */
