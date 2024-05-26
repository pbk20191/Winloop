#include <errno.h>
#include <stddef.h>
#include "Python.h"
#include "uv.h"


#ifndef EWOULDBLOCK
#define EWOULDBLOCK EAGAIN
#endif

#ifdef __APPLE__
#define PLATFORM_IS_APPLE 1
#else
#define PLATFORM_IS_APPLE 0
#endif


#ifdef __linux__
#  define PLATFORM_IS_LINUX 1
#  include <sys/epoll.h>
#else
#  define PLATFORM_IS_LINUX 0
#endif

#ifdef __APPLE__
#  define EPOLL_CTL_DEL 2
struct epoll_event {};
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event) {
    return 0;
};
#endif

#ifdef _WIN32
#  define EPOLL_CTL_DEL 2
/* error C2016: C requires that a struct or union have at least one member on Windows
with default compilation flags. Therefore put dummy field for now. */
struct epoll_event {int dummyfield;};
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event) {
    return 0;
};
#endif

/* TODO: add missing parts of uvloop/includes/compat.h */

#ifdef _WIN32
#define PLATFORM_IS_WINDOWS 1
int getuid() {
	return 0;
}
#else
#define PLATFORM_IS_WINDOWS 0
#endif


#if PY_VERSION_HEX < 0x03070100

PyObject * Context_CopyCurrent(void) {
    return (PyObject *)PyContext_CopyCurrent();
};

int Context_Enter(PyObject *ctx) {
    return PyContext_Enter((PyContext *)ctx);
}

int Context_Exit(PyObject *ctx) {
    return PyContext_Exit((PyContext *)ctx);
}

#else

PyObject * Context_CopyCurrent(void) {
    return PyContext_CopyCurrent();
};

int Context_Enter(PyObject *ctx) {
    return PyContext_Enter(ctx);
}

int Context_Exit(PyObject *ctx) {
    return PyContext_Exit(ctx);
}

#endif