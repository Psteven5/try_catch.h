#ifndef TRY_CATCH__H
#define TRY_CATCH__H

#include <setjmp.h
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

#ifndef thread_local
#       if !defined(__STDC_NO_THREADS__)
#               include <thread.h>
#       elif 201100 <= __STDC_VERSION__
#               define thread_local _Thread_local
#       elif defined(_MSC_VER)
#               define thread_local __declspec(thread)
#       elif defined(__GNUC__)
#               define thread_local __thread
#       else
#               define thread_local
#       endif
#endif

#ifndef TRY_CATCH_BUFSIZ
#       define TRY_CATCH_BUFSIZ BUFSIZ
#endif

#define TRY do { \
        struct try_catch__env try_catch__env = { \
                .prev = try_catch__envs, \
        }; \
        try_catch__envs = &try_catch__env; \
        if (0 == setjmp(try_catch__env.buf)) {

#define THROW(E, …) do { \
        *(E *)try_catch__buf = (E) __VA_ARGS__; \
        try_catch__type = #E; \
        try_catch__file = __FILE__; \
        try_catch__line = __LINE__; \
        try_catch__throw(); \
} while (0)

#define TRY_CATCH__CATCH_1(e) } else if (NULL != try_catch__type) { \
        const char *const e = try_catch__type;

#define TRY_CATCH__CATCH_2(E, e) } else if (#E == try_catch__type) { \
        E e = *(E *)try_catch__buf;

#define TRY_CATCH__CATCH_N(E, e, macro, …) macro

#define CATCH(...) TRY_CATCH__CATCH_N(__VA_ARGS__, TRY_CATCH__CATCH_2, TRY_CATCH__CATCH_1)(__VA_ARGS__)

#define END_TRY } else try_catch__throw(); \
        if (NULL == try_catch__type) \
                try_catch__envs = try_catch__envs->prev; \
        try_catch__type = NULL; \
} while (0)

extern thread_local struct try_catch__env {
        jmp_buf buf;
        struct try_catch__env *prev;
} *try_catch__envs;
extern thread_local char try_catch__buf[TRY_CATCH_BUFSIZ];
extern thread_local const char *try_catch__type;
extern thread_local const char *try_catch__file;
extern thread_local int try_catch__line;

noreturn inline static void try_catch__throw();

#ifdef TRY_CATCH_IMPLEMENTATION
        thread_local struct try_catch__env *try_catch__envs = NULL;
        thread_local char try_catch__buf[TRY_CATCH_BUFSIZ];
        thread_local const char *try_catch__type = NULL;
        thread_local const char *try_catch__file;
        thread_local int try_catch__line;

        noreturn inline static void try_catch__throw()
        {
                if (NULL == try_catch__envs) {
                        fprintf(stderr, “Uncaught exception of type %s, file %s, line %d\n”,
                                try_catch__type, try_catch__file, try_catch__line);
                        abort();
                }
                struct try_catch__env *const try_catch__env = try_catch__envs;
                try_catch__envs = try_catch__envs->prev;
                longjmp(try_catch__env->buf, 0);
        }
#endif

#endif
