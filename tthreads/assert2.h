/*
   assert2(condition, msg)
   does nothing if <condition> is true
   otherwise prints error <msg> (and more), and terminates 
   -- inspired by gnu assert() macro from /usr/include/assert.h
*/
#ifndef assert2_included
# define assert2_included
/** \file assert2.h
 *  \brief An assert debug function
 */

extern void __assert2(const char *filename, int linenum, const char *expr, char* msg);

/** \def assert2(e, msg)
 *  Extended assert function that prints debug information.
 *
 *  This assert tests expr, if its true it prints msg 
 *  along with the file and line number where the assert
 *  occured and terminates the program. 
 *
 *  \param e The expression being tested
 *  \param msg The message being printed 
 */
/** if NDEBUG is defined then assert2 checks assertions, otherwise they are ignored */
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
