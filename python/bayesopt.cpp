
#define PY_SSIZE_T_CLEAN
#ifndef CYTHON_USE_PYLONG_INTERNALS
#ifdef PYLONG_BITS_IN_DIGIT
#define CYTHON_USE_PYLONG_INTERNALS 0
#else
#include "pyconfig.h"
#ifdef PYLONG_BITS_IN_DIGIT
#define CYTHON_USE_PYLONG_INTERNALS 1
#else
#define CYTHON_USE_PYLONG_INTERNALS 0
#endif
#endif
#endif
#include "Python.h"
#ifndef Py_PYTHON_H
    #error Python headers needed to compile C extensions, please install development version of Python.
#elif PY_VERSION_HEX < 0x02040000
    #error Cython requires Python 2.4+.
#else
#include <stddef.h> /* For offsetof */
#ifndef offsetof
#define offsetof(type, member) ( (size_t) & ((type*)0) -> member )
#endif
#if !defined(WIN32) && !defined(MS_WINDOWS)
  #ifndef __stdcall
    #define __stdcall
  #endif
  #ifndef __cdecl
    #define __cdecl
  #endif
  #ifndef __fastcall
    #define __fastcall
  #endif
#endif
#ifndef DL_IMPORT
  #define DL_IMPORT(t) t
#endif
#ifndef DL_EXPORT
  #define DL_EXPORT(t) t
#endif
#ifndef PY_LONG_LONG
  #define PY_LONG_LONG LONG_LONG
#endif
#ifndef Py_HUGE_VAL
  #define Py_HUGE_VAL HUGE_VAL
#endif
#ifdef PYPY_VERSION
#define CYTHON_COMPILING_IN_PYPY 1
#define CYTHON_COMPILING_IN_CPYTHON 0
#else
#define CYTHON_COMPILING_IN_PYPY 0
#define CYTHON_COMPILING_IN_CPYTHON 1
#endif
#if PY_VERSION_HEX < 0x02050000
  typedef int Py_ssize_t;
  #define PY_SSIZE_T_MAX INT_MAX
  #define PY_SSIZE_T_MIN INT_MIN
  #define PY_FORMAT_SIZE_T ""
  #define CYTHON_FORMAT_SSIZE_T ""
  #define PyInt_FromSsize_t(z) PyInt_FromLong(z)
  #define PyInt_AsSsize_t(o)   __Pyx_PyInt_AsInt(o)
  #define PyNumber_Index(o)    ((PyNumber_Check(o) && !PyFloat_Check(o)) ? PyNumber_Int(o) : \
                                (PyErr_Format(PyExc_TypeError, \
                                              "expected index value, got %.200s", Py_TYPE(o)->tp_name), \
                                 (PyObject*)0))
  #define __Pyx_PyIndex_Check(o) (PyNumber_Check(o) && !PyFloat_Check(o) && \
                                  !PyComplex_Check(o))
  #define PyIndex_Check __Pyx_PyIndex_Check
  #define PyErr_WarnEx(category, message, stacklevel) PyErr_Warn(category, message)
  #define __PYX_BUILD_PY_SSIZE_T "i"
#else
  #define __PYX_BUILD_PY_SSIZE_T "n"
  #define CYTHON_FORMAT_SSIZE_T "z"
  #define __Pyx_PyIndex_Check PyIndex_Check
#endif
#if PY_VERSION_HEX < 0x02060000
  #define Py_REFCNT(ob) (((PyObject*)(ob))->ob_refcnt)
  #define Py_TYPE(ob)   (((PyObject*)(ob))->ob_type)
  #define Py_SIZE(ob)   (((PyVarObject*)(ob))->ob_size)
  #define PyVarObject_HEAD_INIT(type, size) \
          PyObject_HEAD_INIT(type) size,
  #define PyType_Modified(t)
  typedef struct {
     void *buf;
     PyObject *obj;
     Py_ssize_t len;
     Py_ssize_t itemsize;
     int readonly;
     int ndim;
     char *format;
     Py_ssize_t *shape;
     Py_ssize_t *strides;
     Py_ssize_t *suboffsets;
     void *internal;
  } Py_buffer;
  #define PyBUF_SIMPLE 0
  #define PyBUF_WRITABLE 0x0001
  #define PyBUF_FORMAT 0x0004
  #define PyBUF_ND 0x0008
  #define PyBUF_STRIDES (0x0010 | PyBUF_ND)
  #define PyBUF_C_CONTIGUOUS (0x0020 | PyBUF_STRIDES)
  #define PyBUF_F_CONTIGUOUS (0x0040 | PyBUF_STRIDES)
  #define PyBUF_ANY_CONTIGUOUS (0x0080 | PyBUF_STRIDES)
  #define PyBUF_INDIRECT (0x0100 | PyBUF_STRIDES)
  #define PyBUF_RECORDS (PyBUF_STRIDES | PyBUF_FORMAT | PyBUF_WRITABLE)
  #define PyBUF_FULL (PyBUF_INDIRECT | PyBUF_FORMAT | PyBUF_WRITABLE)
  typedef int (*getbufferproc)(PyObject *, Py_buffer *, int);
  typedef void (*releasebufferproc)(PyObject *, Py_buffer *);
#endif
#if PY_MAJOR_VERSION < 3
  #define __Pyx_BUILTIN_MODULE_NAME "__builtin__"
  #define __Pyx_PyCode_New(a, k, l, s, f, code, c, n, v, fv, cell, fn, name, fline, lnos) \
          PyCode_New(a, l, s, f, code, c, n, v, fv, cell, fn, name, fline, lnos)
#else
  #define __Pyx_BUILTIN_MODULE_NAME "builtins"
  #define __Pyx_PyCode_New(a, k, l, s, f, code, c, n, v, fv, cell, fn, name, fline, lnos) \
          PyCode_New(a, k, l, s, f, code, c, n, v, fv, cell, fn, name, fline, lnos)
#endif
#if PY_MAJOR_VERSION < 3 && PY_MINOR_VERSION < 6
  #define PyUnicode_FromString(s) PyUnicode_Decode(s, strlen(s), "UTF-8", "strict")
#endif
#if PY_MAJOR_VERSION >= 3
  #define Py_TPFLAGS_CHECKTYPES 0
  #define Py_TPFLAGS_HAVE_INDEX 0
#endif
#if (PY_VERSION_HEX < 0x02060000) || (PY_MAJOR_VERSION >= 3)
  #define Py_TPFLAGS_HAVE_NEWBUFFER 0
#endif
#if PY_VERSION_HEX < 0x02060000
  #define Py_TPFLAGS_HAVE_VERSION_TAG 0
#endif
#if PY_VERSION_HEX > 0x03030000 && defined(PyUnicode_KIND)
  #define CYTHON_PEP393_ENABLED 1
  #define __Pyx_PyUnicode_READY(op)       (likely(PyUnicode_IS_READY(op)) ? \
                                              0 : _PyUnicode_Ready((PyObject *)(op)))
  #define __Pyx_PyUnicode_GET_LENGTH(u)   PyUnicode_GET_LENGTH(u)
  #define __Pyx_PyUnicode_READ_CHAR(u, i) PyUnicode_READ_CHAR(u, i)
  #define __Pyx_PyUnicode_READ(k, d, i)   PyUnicode_READ(k, d, i)
#else
  #define CYTHON_PEP393_ENABLED 0
  #define __Pyx_PyUnicode_READY(op)       (0)
  #define __Pyx_PyUnicode_GET_LENGTH(u)   PyUnicode_GET_SIZE(u)
  #define __Pyx_PyUnicode_READ_CHAR(u, i) ((Py_UCS4)(PyUnicode_AS_UNICODE(u)[i]))
  #define __Pyx_PyUnicode_READ(k, d, i)   ((k=k), (Py_UCS4)(((Py_UNICODE*)d)[i]))
#endif
#if PY_MAJOR_VERSION >= 3
  #define PyBaseString_Type            PyUnicode_Type
  #define PyStringObject               PyUnicodeObject
  #define PyString_Type                PyUnicode_Type
  #define PyString_Check               PyUnicode_Check
  #define PyString_CheckExact          PyUnicode_CheckExact
#endif
#if PY_VERSION_HEX < 0x02060000
  #define PyBytesObject                PyStringObject
  #define PyBytes_Type                 PyString_Type
  #define PyBytes_Check                PyString_Check
  #define PyBytes_CheckExact           PyString_CheckExact
  #define PyBytes_FromString           PyString_FromString
  #define PyBytes_FromStringAndSize    PyString_FromStringAndSize
  #define PyBytes_FromFormat           PyString_FromFormat
  #define PyBytes_DecodeEscape         PyString_DecodeEscape
  #define PyBytes_AsString             PyString_AsString
  #define PyBytes_AsStringAndSize      PyString_AsStringAndSize
  #define PyBytes_Size                 PyString_Size
  #define PyBytes_AS_STRING            PyString_AS_STRING
  #define PyBytes_GET_SIZE             PyString_GET_SIZE
  #define PyBytes_Repr                 PyString_Repr
  #define PyBytes_Concat               PyString_Concat
  #define PyBytes_ConcatAndDel         PyString_ConcatAndDel
#endif
#if PY_MAJOR_VERSION >= 3
  #define __Pyx_PyBaseString_Check(obj) PyUnicode_Check(obj)
  #define __Pyx_PyBaseString_CheckExact(obj) PyUnicode_CheckExact(obj)
#else
  #define __Pyx_PyBaseString_Check(obj) (PyString_CheckExact(obj) || PyUnicode_CheckExact(obj) || \
                                         PyString_Check(obj) || PyUnicode_Check(obj))
  #define __Pyx_PyBaseString_CheckExact(obj) (Py_TYPE(obj) == &PyBaseString_Type)
#endif
#if PY_VERSION_HEX < 0x02060000
  #define PySet_Check(obj)             PyObject_TypeCheck(obj, &PySet_Type)
  #define PyFrozenSet_Check(obj)       PyObject_TypeCheck(obj, &PyFrozenSet_Type)
#endif
#ifndef PySet_CheckExact
  #define PySet_CheckExact(obj)        (Py_TYPE(obj) == &PySet_Type)
#endif
#define __Pyx_TypeCheck(obj, type) PyObject_TypeCheck(obj, (PyTypeObject *)type)
#if PY_MAJOR_VERSION >= 3
  #define PyIntObject                  PyLongObject
  #define PyInt_Type                   PyLong_Type
  #define PyInt_Check(op)              PyLong_Check(op)
  #define PyInt_CheckExact(op)         PyLong_CheckExact(op)
  #define PyInt_FromString             PyLong_FromString
  #define PyInt_FromUnicode            PyLong_FromUnicode
  #define PyInt_FromLong               PyLong_FromLong
  #define PyInt_FromSize_t             PyLong_FromSize_t
  #define PyInt_FromSsize_t            PyLong_FromSsize_t
  #define PyInt_AsLong                 PyLong_AsLong
  #define PyInt_AS_LONG                PyLong_AS_LONG
  #define PyInt_AsSsize_t              PyLong_AsSsize_t
  #define PyInt_AsUnsignedLongMask     PyLong_AsUnsignedLongMask
  #define PyInt_AsUnsignedLongLongMask PyLong_AsUnsignedLongLongMask
#endif
#if PY_MAJOR_VERSION >= 3
  #define PyBoolObject                 PyLongObject
#endif
#if PY_VERSION_HEX < 0x03020000
  typedef long Py_hash_t;
  #define __Pyx_PyInt_FromHash_t PyInt_FromLong
  #define __Pyx_PyInt_AsHash_t   PyInt_AsLong
#else
  #define __Pyx_PyInt_FromHash_t PyInt_FromSsize_t
  #define __Pyx_PyInt_AsHash_t   PyInt_AsSsize_t
#endif
#if (PY_MAJOR_VERSION < 3) || (PY_VERSION_HEX >= 0x03010300)
  #define __Pyx_PySequence_GetSlice(obj, a, b) PySequence_GetSlice(obj, a, b)
  #define __Pyx_PySequence_SetSlice(obj, a, b, value) PySequence_SetSlice(obj, a, b, value)
  #define __Pyx_PySequence_DelSlice(obj, a, b) PySequence_DelSlice(obj, a, b)
#else
  #define __Pyx_PySequence_GetSlice(obj, a, b) (unlikely(!(obj)) ? \
        (PyErr_SetString(PyExc_SystemError, "null argument to internal routine"), (PyObject*)0) : \
        (likely((obj)->ob_type->tp_as_mapping) ? (PySequence_GetSlice(obj, a, b)) : \
            (PyErr_Format(PyExc_TypeError, "'%.200s' object is unsliceable", (obj)->ob_type->tp_name), (PyObject*)0)))
  #define __Pyx_PySequence_SetSlice(obj, a, b, value) (unlikely(!(obj)) ? \
        (PyErr_SetString(PyExc_SystemError, "null argument to internal routine"), -1) : \
        (likely((obj)->ob_type->tp_as_mapping) ? (PySequence_SetSlice(obj, a, b, value)) : \
            (PyErr_Format(PyExc_TypeError, "'%.200s' object doesn't support slice assignment", (obj)->ob_type->tp_name), -1)))
  #define __Pyx_PySequence_DelSlice(obj, a, b) (unlikely(!(obj)) ? \
        (PyErr_SetString(PyExc_SystemError, "null argument to internal routine"), -1) : \
        (likely((obj)->ob_type->tp_as_mapping) ? (PySequence_DelSlice(obj, a, b)) : \
            (PyErr_Format(PyExc_TypeError, "'%.200s' object doesn't support slice deletion", (obj)->ob_type->tp_name), -1)))
#endif
#if PY_MAJOR_VERSION >= 3
  #define PyMethod_New(func, self, klass) ((self) ? PyMethod_New(func, self) : PyInstanceMethod_New(func))
#endif
#if PY_VERSION_HEX < 0x02050000
  #define __Pyx_GetAttrString(o,n)   PyObject_GetAttrString((o),((char *)(n)))
  #define __Pyx_SetAttrString(o,n,a) PyObject_SetAttrString((o),((char *)(n)),(a))
  #define __Pyx_DelAttrString(o,n)   PyObject_DelAttrString((o),((char *)(n)))
#else
  #define __Pyx_GetAttrString(o,n)   PyObject_GetAttrString((o),(n))
  #define __Pyx_SetAttrString(o,n,a) PyObject_SetAttrString((o),(n),(a))
  #define __Pyx_DelAttrString(o,n)   PyObject_DelAttrString((o),(n))
#endif
#if PY_VERSION_HEX < 0x02050000
  #define __Pyx_NAMESTR(n) ((char *)(n))
  #define __Pyx_DOCSTR(n)  ((char *)(n))
#else
  #define __Pyx_NAMESTR(n) (n)
  #define __Pyx_DOCSTR(n)  (n)
#endif
#ifndef CYTHON_INLINE
  #if defined(__GNUC__)
    #define CYTHON_INLINE __inline__
  #elif defined(_MSC_VER)
    #define CYTHON_INLINE __inline
  #elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    #define CYTHON_INLINE inline
  #else
    #define CYTHON_INLINE
  #endif
#endif
#ifndef CYTHON_RESTRICT
  #if defined(__GNUC__)
    #define CYTHON_RESTRICT __restrict__
  #elif defined(_MSC_VER)
    #define CYTHON_RESTRICT __restrict
  #elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    #define CYTHON_RESTRICT restrict
  #else
    #define CYTHON_RESTRICT
  #endif
#endif
#ifdef NAN
#define __PYX_NAN() ((float) NAN)
#else
static CYTHON_INLINE float __PYX_NAN() {
  /* Initialize NaN. The sign is irrelevant, an exponent with all bits 1 and
   a nonzero mantissa means NaN. If the first bit in the mantissa is 1, it is
   a quiet NaN. */
  float value;
  memset(&value, 0xFF, sizeof(value));
  return value;
}
#endif


#if PY_MAJOR_VERSION >= 3
  #define __Pyx_PyNumber_Divide(x,y)         PyNumber_TrueDivide(x,y)
  #define __Pyx_PyNumber_InPlaceDivide(x,y)  PyNumber_InPlaceTrueDivide(x,y)
#else
  #define __Pyx_PyNumber_Divide(x,y)         PyNumber_Divide(x,y)
  #define __Pyx_PyNumber_InPlaceDivide(x,y)  PyNumber_InPlaceDivide(x,y)
#endif

#ifndef __PYX_EXTERN_C
  #ifdef __cplusplus
    #define __PYX_EXTERN_C extern "C"
  #else
    #define __PYX_EXTERN_C extern
  #endif
#endif

#if defined(WIN32) || defined(MS_WINDOWS)
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#define __PYX_HAVE__bayesopt
#define __PYX_HAVE_API__bayesopt
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "numpy/arrayobject.h"
#include "numpy/ufuncobject.h"
#include "pythread.h"
#include "datetime.h"
#include "math.h"
#include "parameters.h"
#include "bayesopt.h"
#ifdef _OPENMP
#include <omp.h>
#endif /* _OPENMP */

#ifdef PYREX_WITHOUT_ASSERTIONS
#define CYTHON_WITHOUT_ASSERTIONS
#endif

#ifndef CYTHON_UNUSED
# if defined(__GNUC__)
#   if !(defined(__cplusplus)) || (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
#     define CYTHON_UNUSED __attribute__ ((__unused__))
#   else
#     define CYTHON_UNUSED
#   endif
# elif defined(__ICC) || (defined(__INTEL_COMPILER) && !defined(_MSC_VER))
#   define CYTHON_UNUSED __attribute__ ((__unused__))
# else
#   define CYTHON_UNUSED
# endif
#endif
typedef struct {PyObject **p; char *s; const Py_ssize_t n; const char* encoding;
                const char is_unicode; const char is_str; const char intern; } __Pyx_StringTabEntry; /*proto*/

#define __PYX_DEFAULT_STRING_ENCODING_IS_ASCII 0
#define __PYX_DEFAULT_STRING_ENCODING_IS_DEFAULT 0
#define __PYX_DEFAULT_STRING_ENCODING ""
#define __Pyx_PyObject_FromString __Pyx_PyBytes_FromString
#define __Pyx_PyObject_FromStringAndSize __Pyx_PyBytes_FromStringAndSize
static CYTHON_INLINE char* __Pyx_PyObject_AsString(PyObject*);
static CYTHON_INLINE char* __Pyx_PyObject_AsStringAndSize(PyObject*, Py_ssize_t* length);
#define __Pyx_PyBytes_FromString        PyBytes_FromString
#define __Pyx_PyBytes_FromStringAndSize PyBytes_FromStringAndSize
static CYTHON_INLINE PyObject* __Pyx_PyUnicode_FromString(char*);
#if PY_MAJOR_VERSION < 3
    #define __Pyx_PyStr_FromString        __Pyx_PyBytes_FromString
    #define __Pyx_PyStr_FromStringAndSize __Pyx_PyBytes_FromStringAndSize
#else
    #define __Pyx_PyStr_FromString        __Pyx_PyUnicode_FromString
    #define __Pyx_PyStr_FromStringAndSize __Pyx_PyUnicode_FromStringAndSize
#endif
#define __Pyx_PyObject_AsUString(s)    ((unsigned char*) __Pyx_PyObject_AsString(s))
#define __Pyx_PyObject_FromUString(s)  __Pyx_PyObject_FromString((char*)s)
#define __Pyx_PyBytes_FromUString(s)   __Pyx_PyBytes_FromString((char*)s)
#define __Pyx_PyStr_FromUString(s)     __Pyx_PyStr_FromString((char*)s)
#define __Pyx_PyUnicode_FromUString(s) __Pyx_PyUnicode_FromString((char*)s)
#if PY_MAJOR_VERSION < 3
static CYTHON_INLINE size_t __Pyx_Py_UNICODE_strlen(const Py_UNICODE *u)
{
    const Py_UNICODE *u_end = u;
    while (*u_end++) ;
    return u_end - u - 1;
}
#else
#define __Pyx_Py_UNICODE_strlen Py_UNICODE_strlen
#endif
#define __Pyx_PyUnicode_FromUnicode(u)       PyUnicode_FromUnicode(u, __Pyx_Py_UNICODE_strlen(u))
#define __Pyx_PyUnicode_FromUnicodeAndLength PyUnicode_FromUnicode
#define __Pyx_PyUnicode_AsUnicode            PyUnicode_AsUnicode
#define __Pyx_Owned_Py_None(b) (Py_INCREF(Py_None), Py_None)
#define __Pyx_PyBool_FromLong(b) ((b) ? (Py_INCREF(Py_True), Py_True) : (Py_INCREF(Py_False), Py_False))
static CYTHON_INLINE int __Pyx_PyObject_IsTrue(PyObject*);
static CYTHON_INLINE PyObject* __Pyx_PyNumber_Int(PyObject* x);
static CYTHON_INLINE Py_ssize_t __Pyx_PyIndex_AsSsize_t(PyObject*);
static CYTHON_INLINE PyObject * __Pyx_PyInt_FromSize_t(size_t);
static CYTHON_INLINE size_t __Pyx_PyInt_AsSize_t(PyObject*);
#if CYTHON_COMPILING_IN_CPYTHON
#define __pyx_PyFloat_AsDouble(x) (PyFloat_CheckExact(x) ? PyFloat_AS_DOUBLE(x) : PyFloat_AsDouble(x))
#else
#define __pyx_PyFloat_AsDouble(x) PyFloat_AsDouble(x)
#endif
#define __pyx_PyFloat_AsFloat(x) ((float) __pyx_PyFloat_AsDouble(x))
#if PY_MAJOR_VERSION < 3 && __PYX_DEFAULT_STRING_ENCODING_IS_ASCII
static int __Pyx_sys_getdefaultencoding_not_ascii;
static int __Pyx_init_sys_getdefaultencoding_params() {
    PyObject* sys = NULL;
    PyObject* default_encoding = NULL;
    PyObject* ascii_chars_u = NULL;
    PyObject* ascii_chars_b = NULL;
    sys = PyImport_ImportModule("sys");
    if (sys == NULL) goto bad;
    default_encoding = PyObject_CallMethod(sys, (char*) (const char*) "getdefaultencoding", NULL);
    if (default_encoding == NULL) goto bad;
    if (strcmp(PyBytes_AsString(default_encoding), "ascii") == 0) {
        __Pyx_sys_getdefaultencoding_not_ascii = 0;
    } else {
        const char* default_encoding_c = PyBytes_AS_STRING(default_encoding);
        char ascii_chars[128];
        int c;
        for (c = 0; c < 128; c++) {
            ascii_chars[c] = c;
        }
        __Pyx_sys_getdefaultencoding_not_ascii = 1;
        ascii_chars_u = PyUnicode_DecodeASCII(ascii_chars, 128, NULL);
        if (ascii_chars_u == NULL) goto bad;
        ascii_chars_b = PyUnicode_AsEncodedString(ascii_chars_u, default_encoding_c, NULL);
        if (ascii_chars_b == NULL || strncmp(ascii_chars, PyBytes_AS_STRING(ascii_chars_b), 128) != 0) {
            PyErr_Format(
                PyExc_ValueError,
                "This module compiled with c_string_encoding=ascii, but default encoding '%s' is not a superset of ascii.",
                default_encoding_c);
            goto bad;
        }
    }
    Py_XDECREF(sys);
    Py_XDECREF(default_encoding);
    Py_XDECREF(ascii_chars_u);
    Py_XDECREF(ascii_chars_b);
    return 0;
bad:
    Py_XDECREF(sys);
    Py_XDECREF(default_encoding);
    Py_XDECREF(ascii_chars_u);
    Py_XDECREF(ascii_chars_b);
    return -1;
}
#endif
#if __PYX_DEFAULT_STRING_ENCODING_IS_DEFAULT && PY_MAJOR_VERSION >= 3
#define __Pyx_PyUnicode_FromStringAndSize(c_str, size) PyUnicode_DecodeUTF8(c_str, size, NULL)
#else
#define __Pyx_PyUnicode_FromStringAndSize(c_str, size) PyUnicode_Decode(c_str, size, __PYX_DEFAULT_STRING_ENCODING, NULL)
#if __PYX_DEFAULT_STRING_ENCODING_IS_DEFAULT
static char* __PYX_DEFAULT_STRING_ENCODING;
static int __Pyx_init_sys_getdefaultencoding_params() {
    PyObject* sys = NULL;
    PyObject* default_encoding = NULL;
    char* default_encoding_c;
    sys = PyImport_ImportModule("sys");
    if (sys == NULL) goto bad;
    default_encoding = PyObject_CallMethod(sys, (char*) (const char*) "getdefaultencoding", NULL);
    if (default_encoding == NULL) goto bad;
    default_encoding_c = PyBytes_AS_STRING(default_encoding);
    __PYX_DEFAULT_STRING_ENCODING = (char*) malloc(strlen(default_encoding_c));
    strcpy(__PYX_DEFAULT_STRING_ENCODING, default_encoding_c);
    Py_DECREF(sys);
    Py_DECREF(default_encoding);
    return 0;
bad:
    Py_XDECREF(sys);
    Py_XDECREF(default_encoding);
    return -1;
}
#endif
#endif


#ifdef __GNUC__
  /* Test for GCC > 2.95 */
  #if __GNUC__ > 2 || (__GNUC__ == 2 && (__GNUC_MINOR__ > 95))
    #define likely(x)   __builtin_expect(!!(x), 1)
    #define unlikely(x) __builtin_expect(!!(x), 0)
  #else /* __GNUC__ > 2 ... */
    #define likely(x)   (x)
    #define unlikely(x) (x)
  #endif /* __GNUC__ > 2 ... */
#else /* __GNUC__ */
  #define likely(x)   (x)
  #define unlikely(x) (x)
#endif /* __GNUC__ */

static PyObject *__pyx_m;
static PyObject *__pyx_d;
static PyObject *__pyx_b;
static PyObject *__pyx_empty_tuple;
static PyObject *__pyx_empty_bytes;
static int __pyx_lineno;
static int __pyx_clineno = 0;
static const char * __pyx_cfilenm= __FILE__;
static const char *__pyx_filename;

#if !defined(CYTHON_CCOMPLEX)
  #if defined(__cplusplus)
    #define CYTHON_CCOMPLEX 1
  #elif defined(_Complex_I)
    #define CYTHON_CCOMPLEX 1
  #else
    #define CYTHON_CCOMPLEX 0
  #endif
#endif
#if CYTHON_CCOMPLEX
  #ifdef __cplusplus
    #include <complex>
  #else
    #include <complex.h>
  #endif
#endif
#if CYTHON_CCOMPLEX && !defined(__cplusplus) && defined(__sun__) && defined(__GNUC__)
  #undef _Complex_I
  #define _Complex_I 1.0fj
#endif


static const char *__pyx_f[] = {
  "bayesopt.pyx",
  "numpy.pxd",
  "datetime.pxd",
  "type.pxd",
  "bool.pxd",
  "complex.pxd",
};
#define IS_UNSIGNED(type) (((type) -1) > 0)
struct __Pyx_StructField_;
#define __PYX_BUF_FLAGS_PACKED_STRUCT (1 << 0)
typedef struct {
  const char* name; /* for error messages only */
  struct __Pyx_StructField_* fields;
  size_t size;     /* sizeof(type) */
  size_t arraysize[8]; /* length of array in each dimension */
  int ndim;
  char typegroup; /* _R_eal, _C_omplex, Signed _I_nt, _U_nsigned int, _S_truct, _P_ointer, _O_bject, c_H_ar */
  char is_unsigned;
  int flags;
} __Pyx_TypeInfo;
typedef struct __Pyx_StructField_ {
  __Pyx_TypeInfo* type;
  const char* name;
  size_t offset;
} __Pyx_StructField;
typedef struct {
  __Pyx_StructField* field;
  size_t parent_offset;
} __Pyx_BufFmt_StackElem;
typedef struct {
  __Pyx_StructField root;
  __Pyx_BufFmt_StackElem* head;
  size_t fmt_offset;
  size_t new_count, enc_count;
  size_t struct_alignment;
  int is_complex;
  char enc_type;
  char new_packmode;
  char enc_packmode;
  char is_valid_array;
} __Pyx_BufFmt_Context;


/* "numpy.pxd":723
 * # in Cython to enable them only on the right systems.
 * 
 * ctypedef npy_int8       int8_t             # <<<<<<<<<<<<<<
 * ctypedef npy_int16      int16_t
 * ctypedef npy_int32      int32_t
 */
typedef npy_int8 __pyx_t_5numpy_int8_t;

/* "numpy.pxd":724
 * 
 * ctypedef npy_int8       int8_t
 * ctypedef npy_int16      int16_t             # <<<<<<<<<<<<<<
 * ctypedef npy_int32      int32_t
 * ctypedef npy_int64      int64_t
 */
typedef npy_int16 __pyx_t_5numpy_int16_t;

/* "numpy.pxd":725
 * ctypedef npy_int8       int8_t
 * ctypedef npy_int16      int16_t
 * ctypedef npy_int32      int32_t             # <<<<<<<<<<<<<<
 * ctypedef npy_int64      int64_t
 * #ctypedef npy_int96      int96_t
 */
typedef npy_int32 __pyx_t_5numpy_int32_t;

/* "numpy.pxd":726
 * ctypedef npy_int16      int16_t
 * ctypedef npy_int32      int32_t
 * ctypedef npy_int64      int64_t             # <<<<<<<<<<<<<<
 * #ctypedef npy_int96      int96_t
 * #ctypedef npy_int128     int128_t
 */
typedef npy_int64 __pyx_t_5numpy_int64_t;

/* "numpy.pxd":730
 * #ctypedef npy_int128     int128_t
 * 
 * ctypedef npy_uint8      uint8_t             # <<<<<<<<<<<<<<
 * ctypedef npy_uint16     uint16_t
 * ctypedef npy_uint32     uint32_t
 */
typedef npy_uint8 __pyx_t_5numpy_uint8_t;

/* "numpy.pxd":731
 * 
 * ctypedef npy_uint8      uint8_t
 * ctypedef npy_uint16     uint16_t             # <<<<<<<<<<<<<<
 * ctypedef npy_uint32     uint32_t
 * ctypedef npy_uint64     uint64_t
 */
typedef npy_uint16 __pyx_t_5numpy_uint16_t;

/* "numpy.pxd":732
 * ctypedef npy_uint8      uint8_t
 * ctypedef npy_uint16     uint16_t
 * ctypedef npy_uint32     uint32_t             # <<<<<<<<<<<<<<
 * ctypedef npy_uint64     uint64_t
 * #ctypedef npy_uint96     uint96_t
 */
typedef npy_uint32 __pyx_t_5numpy_uint32_t;

/* "numpy.pxd":733
 * ctypedef npy_uint16     uint16_t
 * ctypedef npy_uint32     uint32_t
 * ctypedef npy_uint64     uint64_t             # <<<<<<<<<<<<<<
 * #ctypedef npy_uint96     uint96_t
 * #ctypedef npy_uint128    uint128_t
 */
typedef npy_uint64 __pyx_t_5numpy_uint64_t;

/* "numpy.pxd":737
 * #ctypedef npy_uint128    uint128_t
 * 
 * ctypedef npy_float32    float32_t             # <<<<<<<<<<<<<<
 * ctypedef npy_float64    float64_t
 * #ctypedef npy_float80    float80_t
 */
typedef npy_float32 __pyx_t_5numpy_float32_t;

/* "numpy.pxd":738
 * 
 * ctypedef npy_float32    float32_t
 * ctypedef npy_float64    float64_t             # <<<<<<<<<<<<<<
 * #ctypedef npy_float80    float80_t
 * #ctypedef npy_float128   float128_t
 */
typedef npy_float64 __pyx_t_5numpy_float64_t;

/* "numpy.pxd":747
 * # The int types are mapped a bit surprising --
 * # numpy.int corresponds to 'l' and numpy.long to 'q'
 * ctypedef npy_long       int_t             # <<<<<<<<<<<<<<
 * ctypedef npy_longlong   long_t
 * ctypedef npy_longlong   longlong_t
 */
typedef npy_long __pyx_t_5numpy_int_t;

/* "numpy.pxd":748
 * # numpy.int corresponds to 'l' and numpy.long to 'q'
 * ctypedef npy_long       int_t
 * ctypedef npy_longlong   long_t             # <<<<<<<<<<<<<<
 * ctypedef npy_longlong   longlong_t
 * 
 */
typedef npy_longlong __pyx_t_5numpy_long_t;

/* "numpy.pxd":749
 * ctypedef npy_long       int_t
 * ctypedef npy_longlong   long_t
 * ctypedef npy_longlong   longlong_t             # <<<<<<<<<<<<<<
 * 
 * ctypedef npy_ulong      uint_t
 */
typedef npy_longlong __pyx_t_5numpy_longlong_t;

/* "numpy.pxd":751
 * ctypedef npy_longlong   longlong_t
 * 
 * ctypedef npy_ulong      uint_t             # <<<<<<<<<<<<<<
 * ctypedef npy_ulonglong  ulong_t
 * ctypedef npy_ulonglong  ulonglong_t
 */
typedef npy_ulong __pyx_t_5numpy_uint_t;

/* "numpy.pxd":752
 * 
 * ctypedef npy_ulong      uint_t
 * ctypedef npy_ulonglong  ulong_t             # <<<<<<<<<<<<<<
 * ctypedef npy_ulonglong  ulonglong_t
 * 
 */
typedef npy_ulonglong __pyx_t_5numpy_ulong_t;

/* "numpy.pxd":753
 * ctypedef npy_ulong      uint_t
 * ctypedef npy_ulonglong  ulong_t
 * ctypedef npy_ulonglong  ulonglong_t             # <<<<<<<<<<<<<<
 * 
 * ctypedef npy_intp       intp_t
 */
typedef npy_ulonglong __pyx_t_5numpy_ulonglong_t;

/* "numpy.pxd":755
 * ctypedef npy_ulonglong  ulonglong_t
 * 
 * ctypedef npy_intp       intp_t             # <<<<<<<<<<<<<<
 * ctypedef npy_uintp      uintp_t
 * 
 */
typedef npy_intp __pyx_t_5numpy_intp_t;

/* "numpy.pxd":756
 * 
 * ctypedef npy_intp       intp_t
 * ctypedef npy_uintp      uintp_t             # <<<<<<<<<<<<<<
 * 
 * ctypedef npy_double     float_t
 */
typedef npy_uintp __pyx_t_5numpy_uintp_t;

/* "numpy.pxd":758
 * ctypedef npy_uintp      uintp_t
 * 
 * ctypedef npy_double     float_t             # <<<<<<<<<<<<<<
 * ctypedef npy_double     double_t
 * ctypedef npy_longdouble longdouble_t
 */
typedef npy_double __pyx_t_5numpy_float_t;

/* "numpy.pxd":759
 * 
 * ctypedef npy_double     float_t
 * ctypedef npy_double     double_t             # <<<<<<<<<<<<<<
 * ctypedef npy_longdouble longdouble_t
 * 
 */
typedef npy_double __pyx_t_5numpy_double_t;

/* "numpy.pxd":760
 * ctypedef npy_double     float_t
 * ctypedef npy_double     double_t
 * ctypedef npy_longdouble longdouble_t             # <<<<<<<<<<<<<<
 * 
 * ctypedef npy_cfloat      cfloat_t
 */
typedef npy_longdouble __pyx_t_5numpy_longdouble_t;
#if CYTHON_CCOMPLEX
  #ifdef __cplusplus
    typedef ::std::complex< float > __pyx_t_float_complex;
  #else
    typedef float _Complex __pyx_t_float_complex;
  #endif
#else
    typedef struct { float real, imag; } __pyx_t_float_complex;
#endif

#if CYTHON_CCOMPLEX
  #ifdef __cplusplus
    typedef ::std::complex< double > __pyx_t_double_complex;
  #else
    typedef double _Complex __pyx_t_double_complex;
  #endif
#else
    typedef struct { double real, imag; } __pyx_t_double_complex;
#endif


/*--- Type declarations ---*/

/* "numpy.pxd":762
 * ctypedef npy_longdouble longdouble_t
 * 
 * ctypedef npy_cfloat      cfloat_t             # <<<<<<<<<<<<<<
 * ctypedef npy_cdouble     cdouble_t
 * ctypedef npy_clongdouble clongdouble_t
 */
typedef npy_cfloat __pyx_t_5numpy_cfloat_t;

/* "numpy.pxd":763
 * 
 * ctypedef npy_cfloat      cfloat_t
 * ctypedef npy_cdouble     cdouble_t             # <<<<<<<<<<<<<<
 * ctypedef npy_clongdouble clongdouble_t
 * 
 */
typedef npy_cdouble __pyx_t_5numpy_cdouble_t;

/* "numpy.pxd":764
 * ctypedef npy_cfloat      cfloat_t
 * ctypedef npy_cdouble     cdouble_t
 * ctypedef npy_clongdouble clongdouble_t             # <<<<<<<<<<<<<<
 * 
 * ctypedef npy_cdouble     complex_t
 */
typedef npy_clongdouble __pyx_t_5numpy_clongdouble_t;

/* "numpy.pxd":766
 * ctypedef npy_clongdouble clongdouble_t
 * 
 * ctypedef npy_cdouble     complex_t             # <<<<<<<<<<<<<<
 * 
 * cdef inline object PyArray_MultiIterNew1(a):
 */
typedef npy_cdouble __pyx_t_5numpy_complex_t;
#ifndef CYTHON_REFNANNY
  #define CYTHON_REFNANNY 0
#endif
#if CYTHON_REFNANNY
  typedef struct {
    void (*INCREF)(void*, PyObject*, int);
    void (*DECREF)(void*, PyObject*, int);
    void (*GOTREF)(void*, PyObject*, int);
    void (*GIVEREF)(void*, PyObject*, int);
    void* (*SetupContext)(const char*, int, const char*);
    void (*FinishContext)(void**);
  } __Pyx_RefNannyAPIStruct;
  static __Pyx_RefNannyAPIStruct *__Pyx_RefNanny = NULL;
  static __Pyx_RefNannyAPIStruct *__Pyx_RefNannyImportAPI(const char *modname); /*proto*/
  #define __Pyx_RefNannyDeclarations void *__pyx_refnanny = NULL;
#ifdef WITH_THREAD
  #define __Pyx_RefNannySetupContext(name, acquire_gil) \
          if (acquire_gil) { \
              PyGILState_STATE __pyx_gilstate_save = PyGILState_Ensure(); \
              __pyx_refnanny = __Pyx_RefNanny->SetupContext((name), __LINE__, __FILE__); \
              PyGILState_Release(__pyx_gilstate_save); \
          } else { \
              __pyx_refnanny = __Pyx_RefNanny->SetupContext((name), __LINE__, __FILE__); \
          }
#else
  #define __Pyx_RefNannySetupContext(name, acquire_gil) \
          __pyx_refnanny = __Pyx_RefNanny->SetupContext((name), __LINE__, __FILE__)
#endif
  #define __Pyx_RefNannyFinishContext() \
          __Pyx_RefNanny->FinishContext(&__pyx_refnanny)
  #define __Pyx_INCREF(r)  __Pyx_RefNanny->INCREF(__pyx_refnanny, (PyObject *)(r), __LINE__)
  #define __Pyx_DECREF(r)  __Pyx_RefNanny->DECREF(__pyx_refnanny, (PyObject *)(r), __LINE__)
  #define __Pyx_GOTREF(r)  __Pyx_RefNanny->GOTREF(__pyx_refnanny, (PyObject *)(r), __LINE__)
  #define __Pyx_GIVEREF(r) __Pyx_RefNanny->GIVEREF(__pyx_refnanny, (PyObject *)(r), __LINE__)
  #define __Pyx_XINCREF(r)  do { if((r) != NULL) {__Pyx_INCREF(r); }} while(0)
  #define __Pyx_XDECREF(r)  do { if((r) != NULL) {__Pyx_DECREF(r); }} while(0)
  #define __Pyx_XGOTREF(r)  do { if((r) != NULL) {__Pyx_GOTREF(r); }} while(0)
  #define __Pyx_XGIVEREF(r) do { if((r) != NULL) {__Pyx_GIVEREF(r);}} while(0)
#else
  #define __Pyx_RefNannyDeclarations
  #define __Pyx_RefNannySetupContext(name, acquire_gil)
  #define __Pyx_RefNannyFinishContext()
  #define __Pyx_INCREF(r) Py_INCREF(r)
  #define __Pyx_DECREF(r) Py_DECREF(r)
  #define __Pyx_GOTREF(r)
  #define __Pyx_GIVEREF(r)
  #define __Pyx_XINCREF(r) Py_XINCREF(r)
  #define __Pyx_XDECREF(r) Py_XDECREF(r)
  #define __Pyx_XGOTREF(r)
  #define __Pyx_XGIVEREF(r)
#endif /* CYTHON_REFNANNY */
#define __Pyx_CLEAR(r)    do { PyObject* tmp = ((PyObject*)(r)); r = NULL; __Pyx_DECREF(tmp);} while(0)
#define __Pyx_XCLEAR(r)   do { if((r) != NULL) {PyObject* tmp = ((PyObject*)(r)); r = NULL; __Pyx_DECREF(tmp);}} while(0)

#if CYTHON_COMPILING_IN_CPYTHON
static CYTHON_INLINE PyObject* __Pyx_PyObject_GetAttrStr(PyObject* obj, PyObject* attr_name) {
    PyTypeObject* tp = Py_TYPE(obj);
    if (likely(tp->tp_getattro))
        return tp->tp_getattro(obj, attr_name);
#if PY_MAJOR_VERSION < 3
    if (likely(tp->tp_getattr))
        return tp->tp_getattr(obj, PyString_AS_STRING(attr_name));
#endif
    return PyObject_GetAttr(obj, attr_name);
}
#else
#define __Pyx_PyObject_GetAttrStr(o,n) PyObject_GetAttr(o,n)
#endif

static PyObject *__Pyx_GetBuiltinName(PyObject *name); /*proto*/

static PyObject* __Pyx_PyDict_GetItemDefault(PyObject* d, PyObject* key, PyObject* default_value); /*proto*/

#define __Pyx_GetItemInt(o, i, size, to_py_func, is_list, wraparound, boundscheck) \
    (((size) <= sizeof(Py_ssize_t)) ? \
    __Pyx_GetItemInt_Fast(o, i, is_list, wraparound, boundscheck) : \
    __Pyx_GetItemInt_Generic(o, to_py_func(i)))
#define __Pyx_GetItemInt_List(o, i, size, to_py_func, is_list, wraparound, boundscheck) \
    (((size) <= sizeof(Py_ssize_t)) ? \
    __Pyx_GetItemInt_List_Fast(o, i, wraparound, boundscheck) : \
    __Pyx_GetItemInt_Generic(o, to_py_func(i)))
static CYTHON_INLINE PyObject *__Pyx_GetItemInt_List_Fast(PyObject *o, Py_ssize_t i,
                                                              int wraparound, int boundscheck);
#define __Pyx_GetItemInt_Tuple(o, i, size, to_py_func, is_list, wraparound, boundscheck) \
    (((size) <= sizeof(Py_ssize_t)) ? \
    __Pyx_GetItemInt_Tuple_Fast(o, i, wraparound, boundscheck) : \
    __Pyx_GetItemInt_Generic(o, to_py_func(i)))
static CYTHON_INLINE PyObject *__Pyx_GetItemInt_Tuple_Fast(PyObject *o, Py_ssize_t i,
                                                              int wraparound, int boundscheck);
static CYTHON_INLINE PyObject *__Pyx_GetItemInt_Generic(PyObject *o, PyObject* j);
static CYTHON_INLINE PyObject *__Pyx_GetItemInt_Fast(PyObject *o, Py_ssize_t i,
                                                     int is_list, int wraparound, int boundscheck);

static CYTHON_INLINE void __Pyx_ErrRestore(PyObject *type, PyObject *value, PyObject *tb); /*proto*/
static CYTHON_INLINE void __Pyx_ErrFetch(PyObject **type, PyObject **value, PyObject **tb); /*proto*/

static void __Pyx_WriteUnraisable(const char *name, int clineno,
                                  int lineno, const char *filename); /*proto*/

static CYTHON_INLINE PyObject *__Pyx_GetModuleGlobalName(PyObject *name); /*proto*/

#define __Pyx_SetItemInt(o, i, v, size, to_py_func, is_list, wraparound, boundscheck) \
    (((size) <= sizeof(Py_ssize_t)) ? \
    __Pyx_SetItemInt_Fast(o, i, v, is_list, wraparound, boundscheck) : \
    __Pyx_SetItemInt_Generic(o, to_py_func(i), v))
static CYTHON_INLINE int __Pyx_SetItemInt_Generic(PyObject *o, PyObject *j, PyObject *v);
static CYTHON_INLINE int __Pyx_SetItemInt_Fast(PyObject *o, Py_ssize_t i, PyObject *v,
                                               int is_list, int wraparound, int boundscheck);

static int __Pyx_GetException(PyObject **type, PyObject **value, PyObject **tb); /*proto*/

static void __Pyx_Raise(PyObject *type, PyObject *value, PyObject *tb, PyObject *cause); /*proto*/

static void __Pyx_RaiseArgtupleInvalid(const char* func_name, int exact,
    Py_ssize_t num_min, Py_ssize_t num_max, Py_ssize_t num_found); /*proto*/

static void __Pyx_RaiseDoubleKeywordsError(const char* func_name, PyObject* kw_name); /*proto*/

static int __Pyx_ParseOptionalKeywords(PyObject *kwds, PyObject **argnames[], \
    PyObject *kwds2, PyObject *values[], Py_ssize_t num_pos_args, \
    const char* function_name); /*proto*/

static int __Pyx_ArgTypeTest(PyObject *obj, PyTypeObject *type, int none_allowed,
    const char *name, int exact); /*proto*/

static CYTHON_INLINE int  __Pyx_GetBufferAndValidate(Py_buffer* buf, PyObject* obj,
    __Pyx_TypeInfo* dtype, int flags, int nd, int cast, __Pyx_BufFmt_StackElem* stack);
static CYTHON_INLINE void __Pyx_SafeReleaseBuffer(Py_buffer* info);

static CYTHON_INLINE int __Pyx_TypeTest(PyObject *obj, PyTypeObject *type); /*proto*/

static void __Pyx_RaiseBufferFallbackError(void); /*proto*/

static void __Pyx_RaiseBufferIndexError(int axis); /*proto*/

#define __Pyx_BufPtrCContig1d(type, buf, i0, s0) ((type)buf + i0)
#define __Pyx_BufPtrCContig2d(type, buf, i0, s0, i1, s1) ((type)((char*)buf + i0 * s0) + i1)
static CYTHON_INLINE void __Pyx_RaiseTooManyValuesError(Py_ssize_t expected);

static CYTHON_INLINE void __Pyx_RaiseNeedMoreValuesError(Py_ssize_t index);

static CYTHON_INLINE void __Pyx_RaiseNoneNotIterableError(void);

static CYTHON_INLINE int __Pyx_IterFinish(void); /*proto*/

static int __Pyx_IternextUnpackEndCheck(PyObject *retval, Py_ssize_t expected); /*proto*/

static CYTHON_INLINE void __Pyx_ExceptionSave(PyObject **type, PyObject **value, PyObject **tb); /*proto*/
static void __Pyx_ExceptionReset(PyObject *type, PyObject *value, PyObject *tb); /*proto*/

typedef struct {
  Py_ssize_t shape, strides, suboffsets;
} __Pyx_Buf_DimInfo;
typedef struct {
  size_t refcount;
  Py_buffer pybuffer;
} __Pyx_Buffer;
typedef struct {
  __Pyx_Buffer *rcbuffer;
  char *data;
  __Pyx_Buf_DimInfo diminfo[8];
} __Pyx_LocalBuf_ND;

#if PY_MAJOR_VERSION < 3
    static int __Pyx_GetBuffer(PyObject *obj, Py_buffer *view, int flags);
    static void __Pyx_ReleaseBuffer(Py_buffer *view);
#else
    #define __Pyx_GetBuffer PyObject_GetBuffer
    #define __Pyx_ReleaseBuffer PyBuffer_Release
#endif


static Py_ssize_t __Pyx_zeros[] = {0, 0, 0, 0, 0, 0, 0, 0};
static Py_ssize_t __Pyx_minusones[] = {-1, -1, -1, -1, -1, -1, -1, -1};

static PyObject *__Pyx_Import(PyObject *name, PyObject *from_list, int level); /*proto*/

static CYTHON_INLINE PyObject *__Pyx_PyInt_to_py_Py_intptr_t(Py_intptr_t);

#if CYTHON_CCOMPLEX
  #ifdef __cplusplus
    #define __Pyx_CREAL(z) ((z).real())
    #define __Pyx_CIMAG(z) ((z).imag())
  #else
    #define __Pyx_CREAL(z) (__real__(z))
    #define __Pyx_CIMAG(z) (__imag__(z))
  #endif
#else
    #define __Pyx_CREAL(z) ((z).real)
    #define __Pyx_CIMAG(z) ((z).imag)
#endif
#if defined(_WIN32) && defined(__cplusplus) && CYTHON_CCOMPLEX
    #define __Pyx_SET_CREAL(z,x) ((z).real(x))
    #define __Pyx_SET_CIMAG(z,y) ((z).imag(y))
#else
    #define __Pyx_SET_CREAL(z,x) __Pyx_CREAL(z) = (x)
    #define __Pyx_SET_CIMAG(z,y) __Pyx_CIMAG(z) = (y)
#endif

static CYTHON_INLINE __pyx_t_float_complex __pyx_t_float_complex_from_parts(float, float);

#if CYTHON_CCOMPLEX
    #define __Pyx_c_eqf(a, b)   ((a)==(b))
    #define __Pyx_c_sumf(a, b)  ((a)+(b))
    #define __Pyx_c_difff(a, b) ((a)-(b))
    #define __Pyx_c_prodf(a, b) ((a)*(b))
    #define __Pyx_c_quotf(a, b) ((a)/(b))
    #define __Pyx_c_negf(a)     (-(a))
  #ifdef __cplusplus
    #define __Pyx_c_is_zerof(z) ((z)==(float)0)
    #define __Pyx_c_conjf(z)    (::std::conj(z))
    #if 1
        #define __Pyx_c_absf(z)     (::std::abs(z))
        #define __Pyx_c_powf(a, b)  (::std::pow(a, b))
    #endif
  #else
    #define __Pyx_c_is_zerof(z) ((z)==0)
    #define __Pyx_c_conjf(z)    (conjf(z))
    #if 1
        #define __Pyx_c_absf(z)     (cabsf(z))
        #define __Pyx_c_powf(a, b)  (cpowf(a, b))
    #endif
 #endif
#else
    static CYTHON_INLINE int __Pyx_c_eqf(__pyx_t_float_complex, __pyx_t_float_complex);
    static CYTHON_INLINE __pyx_t_float_complex __Pyx_c_sumf(__pyx_t_float_complex, __pyx_t_float_complex);
    static CYTHON_INLINE __pyx_t_float_complex __Pyx_c_difff(__pyx_t_float_complex, __pyx_t_float_complex);
    static CYTHON_INLINE __pyx_t_float_complex __Pyx_c_prodf(__pyx_t_float_complex, __pyx_t_float_complex);
    static CYTHON_INLINE __pyx_t_float_complex __Pyx_c_quotf(__pyx_t_float_complex, __pyx_t_float_complex);
    static CYTHON_INLINE __pyx_t_float_complex __Pyx_c_negf(__pyx_t_float_complex);
    static CYTHON_INLINE int __Pyx_c_is_zerof(__pyx_t_float_complex);
    static CYTHON_INLINE __pyx_t_float_complex __Pyx_c_conjf(__pyx_t_float_complex);
    #if 1
        static CYTHON_INLINE float __Pyx_c_absf(__pyx_t_float_complex);
        static CYTHON_INLINE __pyx_t_float_complex __Pyx_c_powf(__pyx_t_float_complex, __pyx_t_float_complex);
    #endif
#endif

static CYTHON_INLINE __pyx_t_double_complex __pyx_t_double_complex_from_parts(double, double);

#if CYTHON_CCOMPLEX
    #define __Pyx_c_eq(a, b)   ((a)==(b))
    #define __Pyx_c_sum(a, b)  ((a)+(b))
    #define __Pyx_c_diff(a, b) ((a)-(b))
    #define __Pyx_c_prod(a, b) ((a)*(b))
    #define __Pyx_c_quot(a, b) ((a)/(b))
    #define __Pyx_c_neg(a)     (-(a))
  #ifdef __cplusplus
    #define __Pyx_c_is_zero(z) ((z)==(double)0)
    #define __Pyx_c_conj(z)    (::std::conj(z))
    #if 1
        #define __Pyx_c_abs(z)     (::std::abs(z))
        #define __Pyx_c_pow(a, b)  (::std::pow(a, b))
    #endif
  #else
    #define __Pyx_c_is_zero(z) ((z)==0)
    #define __Pyx_c_conj(z)    (conj(z))
    #if 1
        #define __Pyx_c_abs(z)     (cabs(z))
        #define __Pyx_c_pow(a, b)  (cpow(a, b))
    #endif
 #endif
#else
    static CYTHON_INLINE int __Pyx_c_eq(__pyx_t_double_complex, __pyx_t_double_complex);
    static CYTHON_INLINE __pyx_t_double_complex __Pyx_c_sum(__pyx_t_double_complex, __pyx_t_double_complex);
    static CYTHON_INLINE __pyx_t_double_complex __Pyx_c_diff(__pyx_t_double_complex, __pyx_t_double_complex);
    static CYTHON_INLINE __pyx_t_double_complex __Pyx_c_prod(__pyx_t_double_complex, __pyx_t_double_complex);
    static CYTHON_INLINE __pyx_t_double_complex __Pyx_c_quot(__pyx_t_double_complex, __pyx_t_double_complex);
    static CYTHON_INLINE __pyx_t_double_complex __Pyx_c_neg(__pyx_t_double_complex);
    static CYTHON_INLINE int __Pyx_c_is_zero(__pyx_t_double_complex);
    static CYTHON_INLINE __pyx_t_double_complex __Pyx_c_conj(__pyx_t_double_complex);
    #if 1
        static CYTHON_INLINE double __Pyx_c_abs(__pyx_t_double_complex);
        static CYTHON_INLINE __pyx_t_double_complex __Pyx_c_pow(__pyx_t_double_complex, __pyx_t_double_complex);
    #endif
#endif

static CYTHON_INLINE unsigned char __Pyx_PyInt_AsUnsignedChar(PyObject *);

static CYTHON_INLINE unsigned short __Pyx_PyInt_AsUnsignedShort(PyObject *);

static CYTHON_INLINE unsigned int __Pyx_PyInt_AsUnsignedInt(PyObject *);

static CYTHON_INLINE char __Pyx_PyInt_AsChar(PyObject *);

static CYTHON_INLINE short __Pyx_PyInt_AsShort(PyObject *);

static CYTHON_INLINE int __Pyx_PyInt_AsInt(PyObject *);

static CYTHON_INLINE signed char __Pyx_PyInt_AsSignedChar(PyObject *);

static CYTHON_INLINE signed short __Pyx_PyInt_AsSignedShort(PyObject *);

static CYTHON_INLINE signed int __Pyx_PyInt_AsSignedInt(PyObject *);

static CYTHON_INLINE int __Pyx_PyInt_AsLongDouble(PyObject *);

static CYTHON_INLINE unsigned long __Pyx_PyInt_AsUnsignedLong(PyObject *);

static CYTHON_INLINE unsigned PY_LONG_LONG __Pyx_PyInt_AsUnsignedLongLong(PyObject *);

static CYTHON_INLINE long __Pyx_PyInt_AsLong(PyObject *);

static CYTHON_INLINE PY_LONG_LONG __Pyx_PyInt_AsLongLong(PyObject *);

static CYTHON_INLINE signed long __Pyx_PyInt_AsSignedLong(PyObject *);

static CYTHON_INLINE signed PY_LONG_LONG __Pyx_PyInt_AsSignedLongLong(PyObject *);

static int __Pyx_check_binary_version(void);

#if !defined(__Pyx_PyIdentifier_FromString)
#if PY_MAJOR_VERSION < 3
  #define __Pyx_PyIdentifier_FromString(s) PyString_FromString(s)
#else
  #define __Pyx_PyIdentifier_FromString(s) PyUnicode_FromString(s)
#endif
#endif

static PyObject *__Pyx_ImportModule(const char *name); /*proto*/

static PyTypeObject *__Pyx_ImportType(const char *module_name, const char *class_name, size_t size, int strict);  /*proto*/

typedef struct {
    int code_line;
    PyCodeObject* code_object;
} __Pyx_CodeObjectCacheEntry;
struct __Pyx_CodeObjectCache {
    int count;
    int max_count;
    __Pyx_CodeObjectCacheEntry* entries;
};
static struct __Pyx_CodeObjectCache __pyx_code_cache = {0,0,NULL};
static int __pyx_bisect_code_objects(__Pyx_CodeObjectCacheEntry* entries, int count, int code_line);
static PyCodeObject *__pyx_find_code_object(int code_line);
static void __pyx_insert_code_object(int code_line, PyCodeObject* code_object);

static void __Pyx_AddTraceback(const char *funcname, int c_line,
                               int py_line, const char *filename); /*proto*/

static int __Pyx_InitStrings(__Pyx_StringTabEntry *t); /*proto*/


/* Module declarations from 'cpython.buffer' */

/* Module declarations from 'cpython.ref' */

/* Module declarations from 'libc.string' */

/* Module declarations from 'libc.stdio' */

/* Module declarations from 'cpython.object' */

/* Module declarations from '__builtin__' */

/* Module declarations from 'cpython.type' */
static PyTypeObject *__pyx_ptype_7cpython_4type_type = 0;

/* Module declarations from 'libc.stdlib' */

/* Module declarations from 'numpy' */

/* Module declarations from 'numpy' */
static PyTypeObject *__pyx_ptype_5numpy_dtype = 0;
static PyTypeObject *__pyx_ptype_5numpy_flatiter = 0;
static PyTypeObject *__pyx_ptype_5numpy_broadcast = 0;
static PyTypeObject *__pyx_ptype_5numpy_ndarray = 0;
static PyTypeObject *__pyx_ptype_5numpy_ufunc = 0;
static CYTHON_INLINE char *__pyx_f_5numpy__util_dtypestring(PyArray_Descr *, char *, char *, int *); /*proto*/

/* Module declarations from 'cpython.version' */

/* Module declarations from 'cpython.exc' */

/* Module declarations from 'cpython.module' */

/* Module declarations from 'cpython.mem' */

/* Module declarations from 'cpython.tuple' */

/* Module declarations from 'cpython.list' */

/* Module declarations from 'cpython.sequence' */

/* Module declarations from 'cpython.mapping' */

/* Module declarations from 'cpython.iterator' */

/* Module declarations from 'cpython.number' */

/* Module declarations from 'cpython.int' */

/* Module declarations from '__builtin__' */

/* Module declarations from 'cpython.bool' */
static PyTypeObject *__pyx_ptype_7cpython_4bool_bool = 0;

/* Module declarations from 'cpython.long' */

/* Module declarations from 'cpython.float' */

/* Module declarations from '__builtin__' */

/* Module declarations from 'cpython.complex' */
static PyTypeObject *__pyx_ptype_7cpython_7complex_complex = 0;

/* Module declarations from 'cpython.string' */

/* Module declarations from 'cpython.unicode' */

/* Module declarations from 'cpython.dict' */

/* Module declarations from 'cpython.instance' */

/* Module declarations from 'cpython.function' */

/* Module declarations from 'cpython.method' */

/* Module declarations from 'cpython.weakref' */

/* Module declarations from 'cpython.getargs' */

/* Module declarations from 'cpython.pythread' */

/* Module declarations from 'cpython.pystate' */

/* Module declarations from 'cpython.cobject' */

/* Module declarations from 'cpython.oldbuffer' */

/* Module declarations from 'cpython.set' */

/* Module declarations from 'cpython.bytes' */

/* Module declarations from 'cpython.pycapsule' */

/* Module declarations from 'datetime' */

/* Module declarations from 'cpython.datetime' */
static PyTypeObject *__pyx_ptype_7cpython_8datetime_date = 0;
static PyTypeObject *__pyx_ptype_7cpython_8datetime_time = 0;
static PyTypeObject *__pyx_ptype_7cpython_8datetime_datetime = 0;
static PyTypeObject *__pyx_ptype_7cpython_8datetime_timedelta = 0;
static PyTypeObject *__pyx_ptype_7cpython_8datetime_tzinfo = 0;

/* Module declarations from 'cpython' */

/* Module declarations from 'bayesopt' */
static bopt_params __pyx_f_8bayesopt_dict2structparams(PyObject *); /*proto*/
static double __pyx_f_8bayesopt_callback(unsigned int, const double*, double *, void *); /*proto*/
static __Pyx_TypeInfo __Pyx_TypeInfo_nn___pyx_t_5numpy_double_t = { "double_t", NULL, sizeof(__pyx_t_5numpy_double_t), { 0 }, 0, 'R', 0, 0 };
static __Pyx_TypeInfo __Pyx_TypeInfo_nn___pyx_t_5numpy_int_t = { "int_t", NULL, sizeof(__pyx_t_5numpy_int_t), { 0 }, 0, IS_UNSIGNED(__pyx_t_5numpy_int_t) ? 'U' : 'I', IS_UNSIGNED(__pyx_t_5numpy_int_t), 0 };
#define __Pyx_MODULE_NAME "bayesopt"
int __pyx_module_is_main_bayesopt = 0;

/* Implementation of 'bayesopt' */
static PyObject *__pyx_builtin_range;
static PyObject *__pyx_builtin_Exception;
static PyObject *__pyx_builtin_ValueError;
static PyObject *__pyx_builtin_MemoryError;
static PyObject *__pyx_builtin_RuntimeError;
static PyObject *__pyx_pf_8bayesopt_raise_problem(CYTHON_UNUSED PyObject *__pyx_self, PyObject *__pyx_v_error_code); /* proto */
static PyObject *__pyx_pf_8bayesopt_2optimize(CYTHON_UNUSED PyObject *__pyx_self, PyObject *__pyx_v_f, int __pyx_v_nDim, PyArrayObject *__pyx_v_np_lb, PyArrayObject *__pyx_v_np_ub, PyObject *__pyx_v_dparams); /* proto */
static PyObject *__pyx_pf_8bayesopt_4optimize_discrete(CYTHON_UNUSED PyObject *__pyx_self, PyObject *__pyx_v_f, PyArrayObject *__pyx_v_np_valid_x, PyObject *__pyx_v_dparams); /* proto */
static PyObject *__pyx_pf_8bayesopt_6optimize_categorical(CYTHON_UNUSED PyObject *__pyx_self, PyObject *__pyx_v_f, PyArrayObject *__pyx_v_np_categories, PyObject *__pyx_v_dparams); /* proto */
static int __pyx_pf_5numpy_7ndarray___getbuffer__(PyArrayObject *__pyx_v_self, Py_buffer *__pyx_v_info, int __pyx_v_flags); /* proto */
static void __pyx_pf_5numpy_7ndarray_2__releasebuffer__(PyArrayObject *__pyx_v_self, Py_buffer *__pyx_v_info); /* proto */
static char __pyx_k_1[] = "Unknown error";
static char __pyx_k_3[] = "Invalid argument";
static char __pyx_k_5[] = "ndarray is not C contiguous";
static char __pyx_k_7[] = "ndarray is not Fortran contiguous";
static char __pyx_k_9[] = "Non-native byte order not supported";
static char __pyx_k_11[] = "unknown dtype code in numpy.pxd (%d)";
static char __pyx_k_12[] = "Format string allocated too short, see comment in numpy.pxd";
static char __pyx_k_15[] = "Format string allocated too short.";
static char __pyx_k_19[] = "/home/rmcantin/code/bayesopt/python/bayesopt.pyx";
static char __pyx_k_26[] = "optimize_categorical";
static char __pyx_k__B[] = "B";
static char __pyx_k__H[] = "H";
static char __pyx_k__I[] = "I";
static char __pyx_k__L[] = "L";
static char __pyx_k__O[] = "O";
static char __pyx_k__Q[] = "Q";
static char __pyx_k__b[] = "b";
static char __pyx_k__d[] = "d";
static char __pyx_k__f[] = "f";
static char __pyx_k__g[] = "g";
static char __pyx_k__h[] = "h";
static char __pyx_k__i[] = "i";
static char __pyx_k__l[] = "l";
static char __pyx_k__q[] = "q";
static char __pyx_k__x[] = "x";
static char __pyx_k__Zd[] = "Zd";
static char __pyx_k__Zf[] = "Zf";
static char __pyx_k__Zg[] = "Zg";
static char __pyx_k__lb[] = "lb";
static char __pyx_k__np[] = "np";
static char __pyx_k__ub[] = "ub";
static char __pyx_k__get[] = "get";
static char __pyx_k__int[] = "int";
static char __pyx_k__beta[] = "beta";
static char __pyx_k__minf[] = "minf";
static char __pyx_k__nDim[] = "nDim";
static char __pyx_k__np_x[] = "np_x";
static char __pyx_k__ones[] = "ones";
static char __pyx_k__alpha[] = "alpha";
static char __pyx_k__dtype[] = "dtype";
static char __pyx_k__noise[] = "noise";
static char __pyx_k__np_lb[] = "np_lb";
static char __pyx_k__np_ub[] = "np_ub";
static char __pyx_k__numpy[] = "numpy";
static char __pyx_k__range[] = "range";
static char __pyx_k__zeros[] = "zeros";
static char __pyx_k__double[] = "double";
static char __pyx_k__l_type[] = "l_type";
static char __pyx_k__params[] = "params";
static char __pyx_k__dparams[] = "dparams";
static char __pyx_k__epsilon[] = "epsilon";
static char __pyx_k__sc_type[] = "sc_type";
static char __pyx_k__sigma_s[] = "sigma_s";
static char __pyx_k__valid_x[] = "valid_x";
static char __pyx_k____main__[] = "__main__";
static char __pyx_k____test__[] = "__test__";
static char __pyx_k__bayesopt[] = "bayesopt";
static char __pyx_k__optimize[] = "optimize";
static char __pyx_k__Exception[] = "Exception";
static char __pyx_k__crit_name[] = "crit_name";
static char __pyx_k__mean_name[] = "mean_name";
static char __pyx_k__min_value[] = "min_value";
static char __pyx_k__surr_name[] = "surr_name";
static char __pyx_k__ValueError[] = "ValueError";
static char __pyx_k____import__[] = "__import__";
static char __pyx_k__categories[] = "categories";
static char __pyx_k__error_code[] = "error_code";
static char __pyx_k__force_jump[] = "force_jump";
static char __pyx_k__np_valid_x[] = "np_valid_x";
static char __pyx_k__MemoryError[] = "MemoryError";
static char __pyx_k__crit_params[] = "crit_params";
static char __pyx_k__init_method[] = "init_method";
static char __pyx_k__kernel_name[] = "kernel_name";
static char __pyx_k__random_seed[] = "random_seed";
static char __pyx_k__RuntimeError[] = "RuntimeError";
static char __pyx_k__log_filename[] = "log_filename";
static char __pyx_k__n_iterations[] = "n_iterations";
static char __pyx_k__kernel_hp_std[] = "kernel_hp_std";
static char __pyx_k__mean_coef_std[] = "mean_coef_std";
static char __pyx_k__np_categories[] = "np_categories";
static char __pyx_k__raise_problem[] = "raise_problem";
static char __pyx_k__verbose_level[] = "verbose_level";
static char __pyx_k__kernel_hp_mean[] = "kernel_hp_mean";
static char __pyx_k__mean_coef_mean[] = "mean_coef_mean";
static char __pyx_k__n_init_samples[] = "n_init_samples";
static char __pyx_k__n_iter_relearn[] = "n_iter_relearn";
static char __pyx_k____pyx_getbuffer[] = "__pyx_getbuffer";
static char __pyx_k__ascontiguousarray[] = "ascontiguousarray";
static char __pyx_k__optimize_discrete[] = "optimize_discrete";
static char __pyx_k__n_inner_iterations[] = "n_inner_iterations";
static char __pyx_k____pyx_releasebuffer[] = "__pyx_releasebuffer";
static PyObject *__pyx_kp_s_1;
static PyObject *__pyx_kp_u_11;
static PyObject *__pyx_kp_u_12;
static PyObject *__pyx_kp_u_15;
static PyObject *__pyx_kp_s_19;
static PyObject *__pyx_n_s_26;
static PyObject *__pyx_kp_s_3;
static PyObject *__pyx_kp_u_5;
static PyObject *__pyx_kp_u_7;
static PyObject *__pyx_kp_u_9;
static PyObject *__pyx_n_s__Exception;
static PyObject *__pyx_n_s__MemoryError;
static PyObject *__pyx_n_s__RuntimeError;
static PyObject *__pyx_n_s__ValueError;
static PyObject *__pyx_n_s____import__;
static PyObject *__pyx_n_s____main__;
static PyObject *__pyx_n_s____pyx_getbuffer;
static PyObject *__pyx_n_s____pyx_releasebuffer;
static PyObject *__pyx_n_s____test__;
static PyObject *__pyx_n_s__alpha;
static PyObject *__pyx_n_s__ascontiguousarray;
static PyObject *__pyx_n_s__bayesopt;
static PyObject *__pyx_n_s__beta;
static PyObject *__pyx_n_s__categories;
static PyObject *__pyx_n_s__crit_name;
static PyObject *__pyx_n_s__crit_params;
static PyObject *__pyx_n_s__double;
static PyObject *__pyx_n_s__dparams;
static PyObject *__pyx_n_s__dtype;
static PyObject *__pyx_n_s__epsilon;
static PyObject *__pyx_n_s__error_code;
static PyObject *__pyx_n_s__f;
static PyObject *__pyx_n_s__force_jump;
static PyObject *__pyx_n_s__get;
static PyObject *__pyx_n_s__init_method;
static PyObject *__pyx_n_s__int;
static PyObject *__pyx_n_s__kernel_hp_mean;
static PyObject *__pyx_n_s__kernel_hp_std;
static PyObject *__pyx_n_s__kernel_name;
static PyObject *__pyx_n_s__l_type;
static PyObject *__pyx_n_s__lb;
static PyObject *__pyx_n_s__log_filename;
static PyObject *__pyx_n_s__mean_coef_mean;
static PyObject *__pyx_n_s__mean_coef_std;
static PyObject *__pyx_n_s__mean_name;
static PyObject *__pyx_n_s__min_value;
static PyObject *__pyx_n_s__minf;
static PyObject *__pyx_n_s__nDim;
static PyObject *__pyx_n_s__n_init_samples;
static PyObject *__pyx_n_s__n_inner_iterations;
static PyObject *__pyx_n_s__n_iter_relearn;
static PyObject *__pyx_n_s__n_iterations;
static PyObject *__pyx_n_s__noise;
static PyObject *__pyx_n_s__np;
static PyObject *__pyx_n_s__np_categories;
static PyObject *__pyx_n_s__np_lb;
static PyObject *__pyx_n_s__np_ub;
static PyObject *__pyx_n_s__np_valid_x;
static PyObject *__pyx_n_s__np_x;
static PyObject *__pyx_n_s__numpy;
static PyObject *__pyx_n_s__ones;
static PyObject *__pyx_n_s__optimize;
static PyObject *__pyx_n_s__optimize_discrete;
static PyObject *__pyx_n_s__params;
static PyObject *__pyx_n_s__raise_problem;
static PyObject *__pyx_n_s__random_seed;
static PyObject *__pyx_n_s__range;
static PyObject *__pyx_n_s__sc_type;
static PyObject *__pyx_n_s__sigma_s;
static PyObject *__pyx_n_s__surr_name;
static PyObject *__pyx_n_s__ub;
static PyObject *__pyx_n_s__valid_x;
static PyObject *__pyx_n_s__verbose_level;
static PyObject *__pyx_n_s__x;
static PyObject *__pyx_n_s__zeros;
static PyObject *__pyx_int_neg_1;
static PyObject *__pyx_int_neg_2;
static PyObject *__pyx_int_neg_3;
static PyObject *__pyx_int_neg_4;
static PyObject *__pyx_int_15;
static PyObject *__pyx_k_tuple_2;
static PyObject *__pyx_k_tuple_4;
static PyObject *__pyx_k_tuple_6;
static PyObject *__pyx_k_tuple_8;
static PyObject *__pyx_k_tuple_10;
static PyObject *__pyx_k_tuple_13;
static PyObject *__pyx_k_tuple_14;
static PyObject *__pyx_k_tuple_16;
static PyObject *__pyx_k_tuple_17;
static PyObject *__pyx_k_tuple_20;
static PyObject *__pyx_k_tuple_22;
static PyObject *__pyx_k_tuple_24;
static PyObject *__pyx_k_codeobj_18;
static PyObject *__pyx_k_codeobj_21;
static PyObject *__pyx_k_codeobj_23;
static PyObject *__pyx_k_codeobj_25;

/* "bayesopt.pyx":122
 * 
 * ###########################################################################
 * cdef bopt_params dict2structparams(dict dparams):             # <<<<<<<<<<<<<<
 * 
 *     params = initialize_parameters_to_default()
 */

static bopt_params __pyx_f_8bayesopt_dict2structparams(PyObject *__pyx_v_dparams) {
  bopt_params __pyx_v_params;
  PyObject *__pyx_v_name = NULL;
  PyObject *__pyx_v_learning = NULL;
  PyObject *__pyx_v_score = NULL;
  PyObject *__pyx_v_theta = NULL;
  PyObject *__pyx_v_stheta = NULL;
  long __pyx_v_i;
  PyObject *__pyx_v_mu = NULL;
  PyObject *__pyx_v_smu = NULL;
  PyObject *__pyx_v_cp = NULL;
  bopt_params __pyx_r;
  __Pyx_RefNannyDeclarations
  PyObject *__pyx_t_1 = NULL;
  PyObject *__pyx_t_2 = NULL;
  unsigned int __pyx_t_3;
  int __pyx_t_4;
  char *__pyx_t_5;
  double __pyx_t_6;
  int __pyx_t_7;
  int __pyx_t_8;
  char const *__pyx_t_9;
  char const *__pyx_t_10;
  int __pyx_t_11;
  Py_ssize_t __pyx_t_12;
  long __pyx_t_13;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("dict2structparams", 0);

  /* "bayesopt.pyx":124
 * cdef bopt_params dict2structparams(dict dparams):
 * 
 *     params = initialize_parameters_to_default()             # <<<<<<<<<<<<<<
 * 
 *     params.n_iterations = dparams.get('n_iterations',params.n_iterations)
 */
  __pyx_v_params = initialize_parameters_to_default();

  /* "bayesopt.pyx":126
 *     params = initialize_parameters_to_default()
 * 
 *     params.n_iterations = dparams.get('n_iterations',params.n_iterations)             # <<<<<<<<<<<<<<
 *     params.n_inner_iterations = dparams.get('n_inner_iterations',
 *                                             params.n_inner_iterations)
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 126; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_1 = PyLong_FromUnsignedLong(__pyx_v_params.n_iterations); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 126; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_t_2 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__n_iterations), __pyx_t_1); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 126; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  __pyx_t_3 = __Pyx_PyInt_AsUnsignedInt(__pyx_t_2); if (unlikely((__pyx_t_3 == (unsigned int)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 126; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  __pyx_v_params.n_iterations = __pyx_t_3;

  /* "bayesopt.pyx":127
 * 
 *     params.n_iterations = dparams.get('n_iterations',params.n_iterations)
 *     params.n_inner_iterations = dparams.get('n_inner_iterations',             # <<<<<<<<<<<<<<
 *                                             params.n_inner_iterations)
 *     params.n_init_samples = dparams.get('n_init_samples',params.n_init_samples)
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 127; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }

  /* "bayesopt.pyx":128
 *     params.n_iterations = dparams.get('n_iterations',params.n_iterations)
 *     params.n_inner_iterations = dparams.get('n_inner_iterations',
 *                                             params.n_inner_iterations)             # <<<<<<<<<<<<<<
 *     params.n_init_samples = dparams.get('n_init_samples',params.n_init_samples)
 *     params.n_iter_relearn = dparams.get('n_iter_relearn',params.n_iter_relearn)
 */
  __pyx_t_2 = PyLong_FromUnsignedLong(__pyx_v_params.n_inner_iterations); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 128; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __pyx_t_1 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__n_inner_iterations), __pyx_t_2); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 127; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  __pyx_t_3 = __Pyx_PyInt_AsUnsignedInt(__pyx_t_1); if (unlikely((__pyx_t_3 == (unsigned int)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 127; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;

  /* "bayesopt.pyx":127
 * 
 *     params.n_iterations = dparams.get('n_iterations',params.n_iterations)
 *     params.n_inner_iterations = dparams.get('n_inner_iterations',             # <<<<<<<<<<<<<<
 *                                             params.n_inner_iterations)
 *     params.n_init_samples = dparams.get('n_init_samples',params.n_init_samples)
 */
  __pyx_v_params.n_inner_iterations = __pyx_t_3;

  /* "bayesopt.pyx":129
 *     params.n_inner_iterations = dparams.get('n_inner_iterations',
 *                                             params.n_inner_iterations)
 *     params.n_init_samples = dparams.get('n_init_samples',params.n_init_samples)             # <<<<<<<<<<<<<<
 *     params.n_iter_relearn = dparams.get('n_iter_relearn',params.n_iter_relearn)
 * 
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 129; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_1 = PyLong_FromUnsignedLong(__pyx_v_params.n_init_samples); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 129; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_t_2 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__n_init_samples), __pyx_t_1); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 129; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  __pyx_t_3 = __Pyx_PyInt_AsUnsignedInt(__pyx_t_2); if (unlikely((__pyx_t_3 == (unsigned int)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 129; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  __pyx_v_params.n_init_samples = __pyx_t_3;

  /* "bayesopt.pyx":130
 *                                             params.n_inner_iterations)
 *     params.n_init_samples = dparams.get('n_init_samples',params.n_init_samples)
 *     params.n_iter_relearn = dparams.get('n_iter_relearn',params.n_iter_relearn)             # <<<<<<<<<<<<<<
 * 
 *     params.init_method = dparams.get('init_method',params.init_method)
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 130; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_2 = PyLong_FromUnsignedLong(__pyx_v_params.n_iter_relearn); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 130; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __pyx_t_1 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__n_iter_relearn), __pyx_t_2); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 130; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  __pyx_t_3 = __Pyx_PyInt_AsUnsignedInt(__pyx_t_1); if (unlikely((__pyx_t_3 == (unsigned int)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 130; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  __pyx_v_params.n_iter_relearn = __pyx_t_3;

  /* "bayesopt.pyx":132
 *     params.n_iter_relearn = dparams.get('n_iter_relearn',params.n_iter_relearn)
 * 
 *     params.init_method = dparams.get('init_method',params.init_method)             # <<<<<<<<<<<<<<
 *     params.random_seed = dparams.get('random_seed',params.random_seed)
 * 
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 132; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_1 = PyLong_FromUnsignedLong(__pyx_v_params.init_method); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 132; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_t_2 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__init_method), __pyx_t_1); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 132; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  __pyx_t_3 = __Pyx_PyInt_AsUnsignedInt(__pyx_t_2); if (unlikely((__pyx_t_3 == (unsigned int)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 132; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  __pyx_v_params.init_method = __pyx_t_3;

  /* "bayesopt.pyx":133
 * 
 *     params.init_method = dparams.get('init_method',params.init_method)
 *     params.random_seed = dparams.get('random_seed',params.random_seed)             # <<<<<<<<<<<<<<
 * 
 *     params.verbose_level = dparams.get('verbose_level',params.verbose_level)
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 133; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_2 = PyInt_FromLong(__pyx_v_params.random_seed); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 133; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __pyx_t_1 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__random_seed), __pyx_t_2); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 133; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  __pyx_t_4 = __Pyx_PyInt_AsInt(__pyx_t_1); if (unlikely((__pyx_t_4 == (int)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 133; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  __pyx_v_params.random_seed = __pyx_t_4;

  /* "bayesopt.pyx":135
 *     params.random_seed = dparams.get('random_seed',params.random_seed)
 * 
 *     params.verbose_level = dparams.get('verbose_level',params.verbose_level)             # <<<<<<<<<<<<<<
 *     name = dparams.get('log_filename',params.log_filename)
 *     set_log_file(&params,name)
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 135; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_1 = PyInt_FromLong(__pyx_v_params.verbose_level); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 135; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_t_2 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__verbose_level), __pyx_t_1); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 135; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  __pyx_t_4 = __Pyx_PyInt_AsInt(__pyx_t_2); if (unlikely((__pyx_t_4 == (int)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 135; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  __pyx_v_params.verbose_level = __pyx_t_4;

  /* "bayesopt.pyx":136
 * 
 *     params.verbose_level = dparams.get('verbose_level',params.verbose_level)
 *     name = dparams.get('log_filename',params.log_filename)             # <<<<<<<<<<<<<<
 *     set_log_file(&params,name)
 * 
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 136; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_2 = __Pyx_PyBytes_FromString(__pyx_v_params.log_filename); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 136; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(((PyObject *)__pyx_t_2));
  __pyx_t_1 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__log_filename), ((PyObject *)__pyx_t_2)); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 136; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __Pyx_DECREF(((PyObject *)__pyx_t_2)); __pyx_t_2 = 0;
  __pyx_v_name = __pyx_t_1;
  __pyx_t_1 = 0;

  /* "bayesopt.pyx":137
 *     params.verbose_level = dparams.get('verbose_level',params.verbose_level)
 *     name = dparams.get('log_filename',params.log_filename)
 *     set_log_file(&params,name)             # <<<<<<<<<<<<<<
 * 
 *     name = dparams.get('surr_name',params.surr_name)
 */
  __pyx_t_5 = __Pyx_PyObject_AsString(__pyx_v_name); if (unlikely((!__pyx_t_5) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 137; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  set_log_file((&__pyx_v_params), __pyx_t_5);

  /* "bayesopt.pyx":139
 *     set_log_file(&params,name)
 * 
 *     name = dparams.get('surr_name',params.surr_name)             # <<<<<<<<<<<<<<
 *     set_surrogate(&params,name)
 * 
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 139; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_1 = __Pyx_PyBytes_FromString(__pyx_v_params.surr_name); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 139; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(((PyObject *)__pyx_t_1));
  __pyx_t_2 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__surr_name), ((PyObject *)__pyx_t_1)); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 139; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_DECREF(((PyObject *)__pyx_t_1)); __pyx_t_1 = 0;
  __Pyx_DECREF(__pyx_v_name);
  __pyx_v_name = __pyx_t_2;
  __pyx_t_2 = 0;

  /* "bayesopt.pyx":140
 * 
 *     name = dparams.get('surr_name',params.surr_name)
 *     set_surrogate(&params,name)             # <<<<<<<<<<<<<<
 * 
 *     params.sigma_s = dparams.get('sigma_s',params.sigma_s)
 */
  __pyx_t_5 = __Pyx_PyObject_AsString(__pyx_v_name); if (unlikely((!__pyx_t_5) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 140; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  set_surrogate((&__pyx_v_params), __pyx_t_5);

  /* "bayesopt.pyx":142
 *     set_surrogate(&params,name)
 * 
 *     params.sigma_s = dparams.get('sigma_s',params.sigma_s)             # <<<<<<<<<<<<<<
 *     params.noise = dparams.get('noise',params.noise)
 *     params.alpha = dparams.get('alpha',params.alpha)
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 142; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_2 = PyFloat_FromDouble(__pyx_v_params.sigma_s); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 142; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __pyx_t_1 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__sigma_s), __pyx_t_2); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 142; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  __pyx_t_6 = __pyx_PyFloat_AsDouble(__pyx_t_1); if (unlikely((__pyx_t_6 == (double)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 142; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  __pyx_v_params.sigma_s = __pyx_t_6;

  /* "bayesopt.pyx":143
 * 
 *     params.sigma_s = dparams.get('sigma_s',params.sigma_s)
 *     params.noise = dparams.get('noise',params.noise)             # <<<<<<<<<<<<<<
 *     params.alpha = dparams.get('alpha',params.alpha)
 *     params.beta = dparams.get('beta',params.beta)
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 143; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_1 = PyFloat_FromDouble(__pyx_v_params.noise); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 143; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_t_2 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__noise), __pyx_t_1); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 143; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  __pyx_t_6 = __pyx_PyFloat_AsDouble(__pyx_t_2); if (unlikely((__pyx_t_6 == (double)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 143; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  __pyx_v_params.noise = __pyx_t_6;

  /* "bayesopt.pyx":144
 *     params.sigma_s = dparams.get('sigma_s',params.sigma_s)
 *     params.noise = dparams.get('noise',params.noise)
 *     params.alpha = dparams.get('alpha',params.alpha)             # <<<<<<<<<<<<<<
 *     params.beta = dparams.get('beta',params.beta)
 * 
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 144; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_2 = PyFloat_FromDouble(__pyx_v_params.alpha); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 144; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __pyx_t_1 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__alpha), __pyx_t_2); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 144; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  __pyx_t_6 = __pyx_PyFloat_AsDouble(__pyx_t_1); if (unlikely((__pyx_t_6 == (double)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 144; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  __pyx_v_params.alpha = __pyx_t_6;

  /* "bayesopt.pyx":145
 *     params.noise = dparams.get('noise',params.noise)
 *     params.alpha = dparams.get('alpha',params.alpha)
 *     params.beta = dparams.get('beta',params.beta)             # <<<<<<<<<<<<<<
 * 
 *     learning = dparams.get('l_type', None)
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 145; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_1 = PyFloat_FromDouble(__pyx_v_params.beta); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 145; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_t_2 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__beta), __pyx_t_1); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 145; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  __pyx_t_6 = __pyx_PyFloat_AsDouble(__pyx_t_2); if (unlikely((__pyx_t_6 == (double)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 145; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  __pyx_v_params.beta = __pyx_t_6;

  /* "bayesopt.pyx":147
 *     params.beta = dparams.get('beta',params.beta)
 * 
 *     learning = dparams.get('l_type', None)             # <<<<<<<<<<<<<<
 *     if learning is not None:
 *         set_learning(&params,learning)
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 147; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_2 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__l_type), Py_None); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 147; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __pyx_v_learning = __pyx_t_2;
  __pyx_t_2 = 0;

  /* "bayesopt.pyx":148
 * 
 *     learning = dparams.get('l_type', None)
 *     if learning is not None:             # <<<<<<<<<<<<<<
 *         set_learning(&params,learning)
 * 
 */
  __pyx_t_7 = (__pyx_v_learning != Py_None);
  __pyx_t_8 = (__pyx_t_7 != 0);
  if (__pyx_t_8) {

    /* "bayesopt.pyx":149
 *     learning = dparams.get('l_type', None)
 *     if learning is not None:
 *         set_learning(&params,learning)             # <<<<<<<<<<<<<<
 * 
 *     score = dparams.get('sc_type', None)
 */
    __pyx_t_9 = __Pyx_PyObject_AsString(__pyx_v_learning); if (unlikely((!__pyx_t_9) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 149; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    set_learning((&__pyx_v_params), __pyx_t_9);
    goto __pyx_L3;
  }
  __pyx_L3:;

  /* "bayesopt.pyx":151
 *         set_learning(&params,learning)
 * 
 *     score = dparams.get('sc_type', None)             # <<<<<<<<<<<<<<
 *     if score is not None:
 *         set_score(&params,score)
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 151; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_2 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__sc_type), Py_None); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 151; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __pyx_v_score = __pyx_t_2;
  __pyx_t_2 = 0;

  /* "bayesopt.pyx":152
 * 
 *     score = dparams.get('sc_type', None)
 *     if score is not None:             # <<<<<<<<<<<<<<
 *         set_score(&params,score)
 * 
 */
  __pyx_t_8 = (__pyx_v_score != Py_None);
  __pyx_t_7 = (__pyx_t_8 != 0);
  if (__pyx_t_7) {

    /* "bayesopt.pyx":153
 *     score = dparams.get('sc_type', None)
 *     if score is not None:
 *         set_score(&params,score)             # <<<<<<<<<<<<<<
 * 
 *     params.epsilon = dparams.get('epsilon',params.epsilon)
 */
    __pyx_t_10 = __Pyx_PyObject_AsString(__pyx_v_score); if (unlikely((!__pyx_t_10) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 153; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    set_score((&__pyx_v_params), __pyx_t_10);
    goto __pyx_L4;
  }
  __pyx_L4:;

  /* "bayesopt.pyx":155
 *         set_score(&params,score)
 * 
 *     params.epsilon = dparams.get('epsilon',params.epsilon)             # <<<<<<<<<<<<<<
 *     params.force_jump= dparams.get('force_jump',params.force_jump)
 * 
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 155; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_2 = PyFloat_FromDouble(__pyx_v_params.epsilon); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 155; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __pyx_t_1 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__epsilon), __pyx_t_2); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 155; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  __pyx_t_6 = __pyx_PyFloat_AsDouble(__pyx_t_1); if (unlikely((__pyx_t_6 == (double)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 155; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  __pyx_v_params.epsilon = __pyx_t_6;

  /* "bayesopt.pyx":156
 * 
 *     params.epsilon = dparams.get('epsilon',params.epsilon)
 *     params.force_jump= dparams.get('force_jump',params.force_jump)             # <<<<<<<<<<<<<<
 * 
 *     name = dparams.get('kernel_name',params.kernel.name)
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 156; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_1 = PyLong_FromUnsignedLong(__pyx_v_params.force_jump); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 156; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_t_2 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__force_jump), __pyx_t_1); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 156; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  __pyx_t_3 = __Pyx_PyInt_AsUnsignedInt(__pyx_t_2); if (unlikely((__pyx_t_3 == (unsigned int)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 156; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  __pyx_v_params.force_jump = __pyx_t_3;

  /* "bayesopt.pyx":158
 *     params.force_jump= dparams.get('force_jump',params.force_jump)
 * 
 *     name = dparams.get('kernel_name',params.kernel.name)             # <<<<<<<<<<<<<<
 *     set_kernel(&params,name)
 * 
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 158; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_2 = __Pyx_PyBytes_FromString(__pyx_v_params.kernel.name); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 158; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(((PyObject *)__pyx_t_2));
  __pyx_t_1 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__kernel_name), ((PyObject *)__pyx_t_2)); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 158; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __Pyx_DECREF(((PyObject *)__pyx_t_2)); __pyx_t_2 = 0;
  __Pyx_DECREF(__pyx_v_name);
  __pyx_v_name = __pyx_t_1;
  __pyx_t_1 = 0;

  /* "bayesopt.pyx":159
 * 
 *     name = dparams.get('kernel_name',params.kernel.name)
 *     set_kernel(&params,name)             # <<<<<<<<<<<<<<
 * 
 *     theta = dparams.get('kernel_hp_mean',None)
 */
  __pyx_t_5 = __Pyx_PyObject_AsString(__pyx_v_name); if (unlikely((!__pyx_t_5) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 159; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  set_kernel((&__pyx_v_params), __pyx_t_5);

  /* "bayesopt.pyx":161
 *     set_kernel(&params,name)
 * 
 *     theta = dparams.get('kernel_hp_mean',None)             # <<<<<<<<<<<<<<
 *     stheta = dparams.get('kernel_hp_std',None)
 *     if theta is not None and stheta is not None:
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 161; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_1 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__kernel_hp_mean), Py_None); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 161; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_v_theta = __pyx_t_1;
  __pyx_t_1 = 0;

  /* "bayesopt.pyx":162
 * 
 *     theta = dparams.get('kernel_hp_mean',None)
 *     stheta = dparams.get('kernel_hp_std',None)             # <<<<<<<<<<<<<<
 *     if theta is not None and stheta is not None:
 *         params.kernel.n_hp = len(theta)
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 162; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_1 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__kernel_hp_std), Py_None); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 162; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_v_stheta = __pyx_t_1;
  __pyx_t_1 = 0;

  /* "bayesopt.pyx":163
 *     theta = dparams.get('kernel_hp_mean',None)
 *     stheta = dparams.get('kernel_hp_std',None)
 *     if theta is not None and stheta is not None:             # <<<<<<<<<<<<<<
 *         params.kernel.n_hp = len(theta)
 *         for i in range(0,params.kernel.n_hp):
 */
  __pyx_t_7 = (__pyx_v_theta != Py_None);
  if ((__pyx_t_7 != 0)) {
    __pyx_t_8 = (__pyx_v_stheta != Py_None);
    __pyx_t_11 = (__pyx_t_8 != 0);
  } else {
    __pyx_t_11 = (__pyx_t_7 != 0);
  }
  if (__pyx_t_11) {

    /* "bayesopt.pyx":164
 *     stheta = dparams.get('kernel_hp_std',None)
 *     if theta is not None and stheta is not None:
 *         params.kernel.n_hp = len(theta)             # <<<<<<<<<<<<<<
 *         for i in range(0,params.kernel.n_hp):
 *             params.kernel.hp_mean[i] = theta[i]
 */
    __pyx_t_12 = PyObject_Length(__pyx_v_theta); if (unlikely(__pyx_t_12 == -1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 164; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __pyx_v_params.kernel.n_hp = __pyx_t_12;

    /* "bayesopt.pyx":165
 *     if theta is not None and stheta is not None:
 *         params.kernel.n_hp = len(theta)
 *         for i in range(0,params.kernel.n_hp):             # <<<<<<<<<<<<<<
 *             params.kernel.hp_mean[i] = theta[i]
 *             params.kernel.hp_std[i] = stheta[i]
 */
    __pyx_t_3 = __pyx_v_params.kernel.n_hp;
    for (__pyx_t_13 = 0; __pyx_t_13 < __pyx_t_3; __pyx_t_13+=1) {
      __pyx_v_i = __pyx_t_13;

      /* "bayesopt.pyx":166
 *         params.kernel.n_hp = len(theta)
 *         for i in range(0,params.kernel.n_hp):
 *             params.kernel.hp_mean[i] = theta[i]             # <<<<<<<<<<<<<<
 *             params.kernel.hp_std[i] = stheta[i]
 * 
 */
      __pyx_t_1 = __Pyx_GetItemInt(__pyx_v_theta, __pyx_v_i, sizeof(long), PyInt_FromLong, 0, 1, 1); if (!__pyx_t_1) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 166; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_1);
      __pyx_t_6 = __pyx_PyFloat_AsDouble(__pyx_t_1); if (unlikely((__pyx_t_6 == (double)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 166; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
      (__pyx_v_params.kernel.hp_mean[__pyx_v_i]) = __pyx_t_6;

      /* "bayesopt.pyx":167
 *         for i in range(0,params.kernel.n_hp):
 *             params.kernel.hp_mean[i] = theta[i]
 *             params.kernel.hp_std[i] = stheta[i]             # <<<<<<<<<<<<<<
 * 
 *     name = dparams.get('mean_name',params.mean.name)
 */
      __pyx_t_1 = __Pyx_GetItemInt(__pyx_v_stheta, __pyx_v_i, sizeof(long), PyInt_FromLong, 0, 1, 1); if (!__pyx_t_1) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 167; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_1);
      __pyx_t_6 = __pyx_PyFloat_AsDouble(__pyx_t_1); if (unlikely((__pyx_t_6 == (double)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 167; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
      (__pyx_v_params.kernel.hp_std[__pyx_v_i]) = __pyx_t_6;
    }
    goto __pyx_L5;
  }
  __pyx_L5:;

  /* "bayesopt.pyx":169
 *             params.kernel.hp_std[i] = stheta[i]
 * 
 *     name = dparams.get('mean_name',params.mean.name)             # <<<<<<<<<<<<<<
 *     set_mean(&params,name)
 * 
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 169; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_1 = __Pyx_PyBytes_FromString(__pyx_v_params.mean.name); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 169; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(((PyObject *)__pyx_t_1));
  __pyx_t_2 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__mean_name), ((PyObject *)__pyx_t_1)); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 169; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_DECREF(((PyObject *)__pyx_t_1)); __pyx_t_1 = 0;
  __Pyx_DECREF(__pyx_v_name);
  __pyx_v_name = __pyx_t_2;
  __pyx_t_2 = 0;

  /* "bayesopt.pyx":170
 * 
 *     name = dparams.get('mean_name',params.mean.name)
 *     set_mean(&params,name)             # <<<<<<<<<<<<<<
 * 
 *     mu = dparams.get('mean_coef_mean',None)
 */
  __pyx_t_5 = __Pyx_PyObject_AsString(__pyx_v_name); if (unlikely((!__pyx_t_5) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 170; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  set_mean((&__pyx_v_params), __pyx_t_5);

  /* "bayesopt.pyx":172
 *     set_mean(&params,name)
 * 
 *     mu = dparams.get('mean_coef_mean',None)             # <<<<<<<<<<<<<<
 *     smu = dparams.get('mean_coef_std',None)
 *     if mu is not None and smu is not None:
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 172; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_2 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__mean_coef_mean), Py_None); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 172; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __pyx_v_mu = __pyx_t_2;
  __pyx_t_2 = 0;

  /* "bayesopt.pyx":173
 * 
 *     mu = dparams.get('mean_coef_mean',None)
 *     smu = dparams.get('mean_coef_std',None)             # <<<<<<<<<<<<<<
 *     if mu is not None and smu is not None:
 *         params.mean.n_coef = len(mu)
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 173; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_2 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__mean_coef_std), Py_None); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 173; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __pyx_v_smu = __pyx_t_2;
  __pyx_t_2 = 0;

  /* "bayesopt.pyx":174
 *     mu = dparams.get('mean_coef_mean',None)
 *     smu = dparams.get('mean_coef_std',None)
 *     if mu is not None and smu is not None:             # <<<<<<<<<<<<<<
 *         params.mean.n_coef = len(mu)
 *         for i in range(0,params.mean.n_coef):
 */
  __pyx_t_11 = (__pyx_v_mu != Py_None);
  if ((__pyx_t_11 != 0)) {
    __pyx_t_7 = (__pyx_v_smu != Py_None);
    __pyx_t_8 = (__pyx_t_7 != 0);
  } else {
    __pyx_t_8 = (__pyx_t_11 != 0);
  }
  if (__pyx_t_8) {

    /* "bayesopt.pyx":175
 *     smu = dparams.get('mean_coef_std',None)
 *     if mu is not None and smu is not None:
 *         params.mean.n_coef = len(mu)             # <<<<<<<<<<<<<<
 *         for i in range(0,params.mean.n_coef):
 *             params.mean.coef_mean[i] = mu[i]
 */
    __pyx_t_12 = PyObject_Length(__pyx_v_mu); if (unlikely(__pyx_t_12 == -1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 175; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __pyx_v_params.mean.n_coef = __pyx_t_12;

    /* "bayesopt.pyx":176
 *     if mu is not None and smu is not None:
 *         params.mean.n_coef = len(mu)
 *         for i in range(0,params.mean.n_coef):             # <<<<<<<<<<<<<<
 *             params.mean.coef_mean[i] = mu[i]
 *             params.mean.coef_std[i] = smu[i]
 */
    __pyx_t_3 = __pyx_v_params.mean.n_coef;
    for (__pyx_t_13 = 0; __pyx_t_13 < __pyx_t_3; __pyx_t_13+=1) {
      __pyx_v_i = __pyx_t_13;

      /* "bayesopt.pyx":177
 *         params.mean.n_coef = len(mu)
 *         for i in range(0,params.mean.n_coef):
 *             params.mean.coef_mean[i] = mu[i]             # <<<<<<<<<<<<<<
 *             params.mean.coef_std[i] = smu[i]
 * 
 */
      __pyx_t_2 = __Pyx_GetItemInt(__pyx_v_mu, __pyx_v_i, sizeof(long), PyInt_FromLong, 0, 1, 1); if (!__pyx_t_2) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 177; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_2);
      __pyx_t_6 = __pyx_PyFloat_AsDouble(__pyx_t_2); if (unlikely((__pyx_t_6 == (double)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 177; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
      (__pyx_v_params.mean.coef_mean[__pyx_v_i]) = __pyx_t_6;

      /* "bayesopt.pyx":178
 *         for i in range(0,params.mean.n_coef):
 *             params.mean.coef_mean[i] = mu[i]
 *             params.mean.coef_std[i] = smu[i]             # <<<<<<<<<<<<<<
 * 
 *     name = dparams.get('crit_name',params.crit_name)
 */
      __pyx_t_2 = __Pyx_GetItemInt(__pyx_v_smu, __pyx_v_i, sizeof(long), PyInt_FromLong, 0, 1, 1); if (!__pyx_t_2) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 178; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_2);
      __pyx_t_6 = __pyx_PyFloat_AsDouble(__pyx_t_2); if (unlikely((__pyx_t_6 == (double)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 178; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
      (__pyx_v_params.mean.coef_std[__pyx_v_i]) = __pyx_t_6;
    }
    goto __pyx_L8;
  }
  __pyx_L8:;

  /* "bayesopt.pyx":180
 *             params.mean.coef_std[i] = smu[i]
 * 
 *     name = dparams.get('crit_name',params.crit_name)             # <<<<<<<<<<<<<<
 *     set_criteria(&params,name)
 * 
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 180; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_2 = __Pyx_PyBytes_FromString(__pyx_v_params.crit_name); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 180; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(((PyObject *)__pyx_t_2));
  __pyx_t_1 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__crit_name), ((PyObject *)__pyx_t_2)); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 180; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __Pyx_DECREF(((PyObject *)__pyx_t_2)); __pyx_t_2 = 0;
  __Pyx_DECREF(__pyx_v_name);
  __pyx_v_name = __pyx_t_1;
  __pyx_t_1 = 0;

  /* "bayesopt.pyx":181
 * 
 *     name = dparams.get('crit_name',params.crit_name)
 *     set_criteria(&params,name)             # <<<<<<<<<<<<<<
 * 
 *     cp = dparams.get('crit_params',None)
 */
  __pyx_t_5 = __Pyx_PyObject_AsString(__pyx_v_name); if (unlikely((!__pyx_t_5) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 181; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  set_criteria((&__pyx_v_params), __pyx_t_5);

  /* "bayesopt.pyx":183
 *     set_criteria(&params,name)
 * 
 *     cp = dparams.get('crit_params',None)             # <<<<<<<<<<<<<<
 *     if cp is not None:
 *         params.n_crit_params = len(cp)
 */
  if (unlikely(((PyObject *)__pyx_v_dparams) == Py_None)) {
    PyErr_Format(PyExc_AttributeError, "'NoneType' object has no attribute '%s'", "get");
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 183; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_1 = __Pyx_PyDict_GetItemDefault(((PyObject *)__pyx_v_dparams), ((PyObject *)__pyx_n_s__crit_params), Py_None); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 183; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_v_cp = __pyx_t_1;
  __pyx_t_1 = 0;

  /* "bayesopt.pyx":184
 * 
 *     cp = dparams.get('crit_params',None)
 *     if cp is not None:             # <<<<<<<<<<<<<<
 *         params.n_crit_params = len(cp)
 *         for i in range(0,params.n_crit_params):
 */
  __pyx_t_8 = (__pyx_v_cp != Py_None);
  __pyx_t_11 = (__pyx_t_8 != 0);
  if (__pyx_t_11) {

    /* "bayesopt.pyx":185
 *     cp = dparams.get('crit_params',None)
 *     if cp is not None:
 *         params.n_crit_params = len(cp)             # <<<<<<<<<<<<<<
 *         for i in range(0,params.n_crit_params):
 *             params.crit_params[i] = cp[i]
 */
    __pyx_t_12 = PyObject_Length(__pyx_v_cp); if (unlikely(__pyx_t_12 == -1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 185; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __pyx_v_params.n_crit_params = __pyx_t_12;

    /* "bayesopt.pyx":186
 *     if cp is not None:
 *         params.n_crit_params = len(cp)
 *         for i in range(0,params.n_crit_params):             # <<<<<<<<<<<<<<
 *             params.crit_params[i] = cp[i]
 * 
 */
    __pyx_t_3 = __pyx_v_params.n_crit_params;
    for (__pyx_t_13 = 0; __pyx_t_13 < __pyx_t_3; __pyx_t_13+=1) {
      __pyx_v_i = __pyx_t_13;

      /* "bayesopt.pyx":187
 *         params.n_crit_params = len(cp)
 *         for i in range(0,params.n_crit_params):
 *             params.crit_params[i] = cp[i]             # <<<<<<<<<<<<<<
 * 
 *     return params
 */
      __pyx_t_1 = __Pyx_GetItemInt(__pyx_v_cp, __pyx_v_i, sizeof(long), PyInt_FromLong, 0, 1, 1); if (!__pyx_t_1) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 187; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_1);
      __pyx_t_6 = __pyx_PyFloat_AsDouble(__pyx_t_1); if (unlikely((__pyx_t_6 == (double)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 187; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
      (__pyx_v_params.crit_params[__pyx_v_i]) = __pyx_t_6;
    }
    goto __pyx_L11;
  }
  __pyx_L11:;

  /* "bayesopt.pyx":189
 *             params.crit_params[i] = cp[i]
 * 
 *     return params             # <<<<<<<<<<<<<<
 * 
 * cdef double callback(unsigned int n, const_double_ptr x,
 */
  __pyx_r = __pyx_v_params;
  goto __pyx_L0;

  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  __Pyx_XDECREF(__pyx_t_2);
  __Pyx_WriteUnraisable("bayesopt.dict2structparams", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_L0:;
  __Pyx_XDECREF(__pyx_v_name);
  __Pyx_XDECREF(__pyx_v_learning);
  __Pyx_XDECREF(__pyx_v_score);
  __Pyx_XDECREF(__pyx_v_theta);
  __Pyx_XDECREF(__pyx_v_stheta);
  __Pyx_XDECREF(__pyx_v_mu);
  __Pyx_XDECREF(__pyx_v_smu);
  __Pyx_XDECREF(__pyx_v_cp);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "bayesopt.pyx":191
 *     return params
 * 
 * cdef double callback(unsigned int n, const_double_ptr x,             # <<<<<<<<<<<<<<
 *                      double *gradient, void *func_data):
 *     try:
 */

static double __pyx_f_8bayesopt_callback(unsigned int __pyx_v_n, const double* __pyx_v_x, CYTHON_UNUSED double *__pyx_v_gradient, void *__pyx_v_func_data) {
  PyObject *__pyx_v_x_np = NULL;
  long __pyx_v_i;
  PyObject *__pyx_v_result = NULL;
  double __pyx_r;
  __Pyx_RefNannyDeclarations
  PyObject *__pyx_t_1 = NULL;
  PyObject *__pyx_t_2 = NULL;
  PyObject *__pyx_t_3 = NULL;
  PyObject *__pyx_t_4 = NULL;
  PyObject *__pyx_t_5 = NULL;
  PyObject *__pyx_t_6 = NULL;
  unsigned int __pyx_t_7;
  long __pyx_t_8;
  double __pyx_t_9;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("callback", 0);

  /* "bayesopt.pyx":193
 * cdef double callback(unsigned int n, const_double_ptr x,
 *                      double *gradient, void *func_data):
 *     try:             # <<<<<<<<<<<<<<
 *         x_np = np.zeros(n)
 * 
 */
  {
    __Pyx_ExceptionSave(&__pyx_t_1, &__pyx_t_2, &__pyx_t_3);
    __Pyx_XGOTREF(__pyx_t_1);
    __Pyx_XGOTREF(__pyx_t_2);
    __Pyx_XGOTREF(__pyx_t_3);
    /*try:*/ {

      /* "bayesopt.pyx":194
 *                      double *gradient, void *func_data):
 *     try:
 *         x_np = np.zeros(n)             # <<<<<<<<<<<<<<
 * 
 *         for i in range(0,n):
 */
      __pyx_t_4 = __Pyx_GetModuleGlobalName(__pyx_n_s__np); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 194; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
      __Pyx_GOTREF(__pyx_t_4);
      __pyx_t_5 = __Pyx_PyObject_GetAttrStr(__pyx_t_4, __pyx_n_s__zeros); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 194; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
      __Pyx_GOTREF(__pyx_t_5);
      __Pyx_DECREF(__pyx_t_4); __pyx_t_4 = 0;
      __pyx_t_4 = PyLong_FromUnsignedLong(__pyx_v_n); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 194; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
      __Pyx_GOTREF(__pyx_t_4);
      __pyx_t_6 = PyTuple_New(1); if (unlikely(!__pyx_t_6)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 194; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
      __Pyx_GOTREF(__pyx_t_6);
      PyTuple_SET_ITEM(__pyx_t_6, 0, __pyx_t_4);
      __Pyx_GIVEREF(__pyx_t_4);
      __pyx_t_4 = 0;
      __pyx_t_4 = PyObject_Call(__pyx_t_5, ((PyObject *)__pyx_t_6), NULL); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 194; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
      __Pyx_GOTREF(__pyx_t_4);
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      __Pyx_DECREF(((PyObject *)__pyx_t_6)); __pyx_t_6 = 0;
      __pyx_v_x_np = __pyx_t_4;
      __pyx_t_4 = 0;

      /* "bayesopt.pyx":196
 *         x_np = np.zeros(n)
 * 
 *         for i in range(0,n):             # <<<<<<<<<<<<<<
 *             x_np[i] = <double>x[i]
 * 
 */
      __pyx_t_7 = __pyx_v_n;
      for (__pyx_t_8 = 0; __pyx_t_8 < __pyx_t_7; __pyx_t_8+=1) {
        __pyx_v_i = __pyx_t_8;

        /* "bayesopt.pyx":197
 * 
 *         for i in range(0,n):
 *             x_np[i] = <double>x[i]             # <<<<<<<<<<<<<<
 * 
 *         result = (<object>func_data)(x_np)
 */
        __pyx_t_4 = PyFloat_FromDouble(((double)(__pyx_v_x[__pyx_v_i]))); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 197; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
        __Pyx_GOTREF(__pyx_t_4);
        if (__Pyx_SetItemInt(__pyx_v_x_np, __pyx_v_i, __pyx_t_4, sizeof(long), PyInt_FromLong, 0, 1, 1) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 197; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
        __Pyx_DECREF(__pyx_t_4); __pyx_t_4 = 0;
      }

      /* "bayesopt.pyx":199
 *             x_np[i] = <double>x[i]
 * 
 *         result = (<object>func_data)(x_np)             # <<<<<<<<<<<<<<
 *         return result
 *     except:
 */
      __pyx_t_4 = PyTuple_New(1); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 199; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
      __Pyx_GOTREF(__pyx_t_4);
      __Pyx_INCREF(__pyx_v_x_np);
      PyTuple_SET_ITEM(__pyx_t_4, 0, __pyx_v_x_np);
      __Pyx_GIVEREF(__pyx_v_x_np);
      __pyx_t_6 = PyObject_Call(((PyObject *)__pyx_v_func_data), ((PyObject *)__pyx_t_4), NULL); if (unlikely(!__pyx_t_6)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 199; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
      __Pyx_GOTREF(__pyx_t_6);
      __Pyx_DECREF(((PyObject *)__pyx_t_4)); __pyx_t_4 = 0;
      __pyx_v_result = __pyx_t_6;
      __pyx_t_6 = 0;

      /* "bayesopt.pyx":200
 * 
 *         result = (<object>func_data)(x_np)
 *         return result             # <<<<<<<<<<<<<<
 *     except:
 *         return HUGE_VAL
 */
      __pyx_t_9 = __pyx_PyFloat_AsDouble(__pyx_v_result); if (unlikely((__pyx_t_9 == (double)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 200; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
      __pyx_r = __pyx_t_9;
      goto __pyx_L7_try_return;
    }
    __Pyx_XDECREF(__pyx_t_1); __pyx_t_1 = 0;
    __Pyx_XDECREF(__pyx_t_2); __pyx_t_2 = 0;
    __Pyx_XDECREF(__pyx_t_3); __pyx_t_3 = 0;
    goto __pyx_L10_try_end;
    __pyx_L7_try_return:;
    __Pyx_XGIVEREF(__pyx_t_1);
    __Pyx_XGIVEREF(__pyx_t_2);
    __Pyx_XGIVEREF(__pyx_t_3);
    __Pyx_ExceptionReset(__pyx_t_1, __pyx_t_2, __pyx_t_3);
    goto __pyx_L0;
    __pyx_L3_error:;
    __Pyx_XDECREF(__pyx_t_5); __pyx_t_5 = 0;
    __Pyx_XDECREF(__pyx_t_4); __pyx_t_4 = 0;
    __Pyx_XDECREF(__pyx_t_6); __pyx_t_6 = 0;

    /* "bayesopt.pyx":201
 *         result = (<object>func_data)(x_np)
 *         return result
 *     except:             # <<<<<<<<<<<<<<
 *         return HUGE_VAL
 * 
 */
    /*except:*/ {
      __Pyx_AddTraceback("bayesopt.callback", __pyx_clineno, __pyx_lineno, __pyx_filename);
      if (__Pyx_GetException(&__pyx_t_6, &__pyx_t_4, &__pyx_t_5) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 201; __pyx_clineno = __LINE__; goto __pyx_L5_except_error;}
      __Pyx_GOTREF(__pyx_t_6);
      __Pyx_GOTREF(__pyx_t_4);
      __Pyx_GOTREF(__pyx_t_5);

      /* "bayesopt.pyx":202
 *         return result
 *     except:
 *         return HUGE_VAL             # <<<<<<<<<<<<<<
 * 
 * def raise_problem(error_code):
 */
      __pyx_r = HUGE_VAL;
      __Pyx_DECREF(__pyx_t_4); __pyx_t_4 = 0;
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      __Pyx_DECREF(__pyx_t_6); __pyx_t_6 = 0;
      goto __pyx_L6_except_return;
      __Pyx_DECREF(__pyx_t_6); __pyx_t_6 = 0;
      __Pyx_DECREF(__pyx_t_4); __pyx_t_4 = 0;
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      goto __pyx_L4_exception_handled;
    }
    __pyx_L5_except_error:;
    __Pyx_XGIVEREF(__pyx_t_1);
    __Pyx_XGIVEREF(__pyx_t_2);
    __Pyx_XGIVEREF(__pyx_t_3);
    __Pyx_ExceptionReset(__pyx_t_1, __pyx_t_2, __pyx_t_3);
    goto __pyx_L1_error;
    __pyx_L6_except_return:;
    __Pyx_XGIVEREF(__pyx_t_1);
    __Pyx_XGIVEREF(__pyx_t_2);
    __Pyx_XGIVEREF(__pyx_t_3);
    __Pyx_ExceptionReset(__pyx_t_1, __pyx_t_2, __pyx_t_3);
    goto __pyx_L0;
    __pyx_L4_exception_handled:;
    __Pyx_XGIVEREF(__pyx_t_1);
    __Pyx_XGIVEREF(__pyx_t_2);
    __Pyx_XGIVEREF(__pyx_t_3);
    __Pyx_ExceptionReset(__pyx_t_1, __pyx_t_2, __pyx_t_3);
    __pyx_L10_try_end:;
  }

  __pyx_r = 0;
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_4);
  __Pyx_XDECREF(__pyx_t_5);
  __Pyx_XDECREF(__pyx_t_6);
  __Pyx_WriteUnraisable("bayesopt.callback", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_XDECREF(__pyx_v_x_np);
  __Pyx_XDECREF(__pyx_v_result);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* Python wrapper */
static PyObject *__pyx_pw_8bayesopt_1raise_problem(PyObject *__pyx_self, PyObject *__pyx_v_error_code); /*proto*/
static PyMethodDef __pyx_mdef_8bayesopt_1raise_problem = {__Pyx_NAMESTR("raise_problem"), (PyCFunction)__pyx_pw_8bayesopt_1raise_problem, METH_O, __Pyx_DOCSTR(0)};
static PyObject *__pyx_pw_8bayesopt_1raise_problem(PyObject *__pyx_self, PyObject *__pyx_v_error_code) {
  PyObject *__pyx_r = 0;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("raise_problem (wrapper)", 0);
  __pyx_r = __pyx_pf_8bayesopt_raise_problem(__pyx_self, ((PyObject *)__pyx_v_error_code));
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "bayesopt.pyx":204
 *         return HUGE_VAL
 * 
 * def raise_problem(error_code):             # <<<<<<<<<<<<<<
 *     # This is a little bit hacky, but we lose track of the C++
 *     # exception since we use the C wrapper for interface:
 */

static PyObject *__pyx_pf_8bayesopt_raise_problem(CYTHON_UNUSED PyObject *__pyx_self, PyObject *__pyx_v_error_code) {
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  PyObject *__pyx_t_1 = NULL;
  int __pyx_t_2;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("raise_problem", 0);

  /* "bayesopt.pyx":215
 *     #static const int BAYESOPT_RUNTIME_ERROR = -4;
 * 
 *     if error_code == -1: raise Exception('Unknown error');             # <<<<<<<<<<<<<<
 *     elif error_code == -2: raise ValueError('Invalid argument');
 *     elif error_code == -3: raise MemoryError;
 */
  __pyx_t_1 = PyObject_RichCompare(__pyx_v_error_code, __pyx_int_neg_1, Py_EQ); __Pyx_XGOTREF(__pyx_t_1); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 215; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_t_2 = __Pyx_PyObject_IsTrue(__pyx_t_1); if (unlikely(__pyx_t_2 < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 215; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  if (__pyx_t_2) {
    __pyx_t_1 = PyObject_Call(__pyx_builtin_Exception, ((PyObject *)__pyx_k_tuple_2), NULL); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 215; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_1);
    __Pyx_Raise(__pyx_t_1, 0, 0, 0);
    __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 215; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    goto __pyx_L3;
  }

  /* "bayesopt.pyx":216
 * 
 *     if error_code == -1: raise Exception('Unknown error');
 *     elif error_code == -2: raise ValueError('Invalid argument');             # <<<<<<<<<<<<<<
 *     elif error_code == -3: raise MemoryError;
 *     elif error_code == -4: raise RuntimeError;
 */
  __pyx_t_1 = PyObject_RichCompare(__pyx_v_error_code, __pyx_int_neg_2, Py_EQ); __Pyx_XGOTREF(__pyx_t_1); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 216; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_t_2 = __Pyx_PyObject_IsTrue(__pyx_t_1); if (unlikely(__pyx_t_2 < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 216; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  if (__pyx_t_2) {
    __pyx_t_1 = PyObject_Call(__pyx_builtin_ValueError, ((PyObject *)__pyx_k_tuple_4), NULL); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 216; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_1);
    __Pyx_Raise(__pyx_t_1, 0, 0, 0);
    __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 216; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    goto __pyx_L3;
  }

  /* "bayesopt.pyx":217
 *     if error_code == -1: raise Exception('Unknown error');
 *     elif error_code == -2: raise ValueError('Invalid argument');
 *     elif error_code == -3: raise MemoryError;             # <<<<<<<<<<<<<<
 *     elif error_code == -4: raise RuntimeError;
 * 
 */
  __pyx_t_1 = PyObject_RichCompare(__pyx_v_error_code, __pyx_int_neg_3, Py_EQ); __Pyx_XGOTREF(__pyx_t_1); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 217; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_t_2 = __Pyx_PyObject_IsTrue(__pyx_t_1); if (unlikely(__pyx_t_2 < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 217; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  if (__pyx_t_2) {
    PyErr_NoMemory(); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 217; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    goto __pyx_L3;
  }

  /* "bayesopt.pyx":218
 *     elif error_code == -2: raise ValueError('Invalid argument');
 *     elif error_code == -3: raise MemoryError;
 *     elif error_code == -4: raise RuntimeError;             # <<<<<<<<<<<<<<
 * 
 * def optimize(f, int nDim, np.ndarray[np.double_t] np_lb,
 */
  __pyx_t_1 = PyObject_RichCompare(__pyx_v_error_code, __pyx_int_neg_4, Py_EQ); __Pyx_XGOTREF(__pyx_t_1); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 218; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_t_2 = __Pyx_PyObject_IsTrue(__pyx_t_1); if (unlikely(__pyx_t_2 < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 218; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  if (__pyx_t_2) {
    __Pyx_Raise(__pyx_builtin_RuntimeError, 0, 0, 0);
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 218; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    goto __pyx_L3;
  }
  __pyx_L3:;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  __Pyx_AddTraceback("bayesopt.raise_problem", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = NULL;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* Python wrapper */
static PyObject *__pyx_pw_8bayesopt_3optimize(PyObject *__pyx_self, PyObject *__pyx_args, PyObject *__pyx_kwds); /*proto*/
static PyMethodDef __pyx_mdef_8bayesopt_3optimize = {__Pyx_NAMESTR("optimize"), (PyCFunction)__pyx_pw_8bayesopt_3optimize, METH_VARARGS|METH_KEYWORDS, __Pyx_DOCSTR(0)};
static PyObject *__pyx_pw_8bayesopt_3optimize(PyObject *__pyx_self, PyObject *__pyx_args, PyObject *__pyx_kwds) {
  PyObject *__pyx_v_f = 0;
  int __pyx_v_nDim;
  PyArrayObject *__pyx_v_np_lb = 0;
  PyArrayObject *__pyx_v_np_ub = 0;
  PyObject *__pyx_v_dparams = 0;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  PyObject *__pyx_r = 0;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("optimize (wrapper)", 0);
  {
    static PyObject **__pyx_pyargnames[] = {&__pyx_n_s__f,&__pyx_n_s__nDim,&__pyx_n_s__np_lb,&__pyx_n_s__np_ub,&__pyx_n_s__dparams,0};
    PyObject* values[5] = {0,0,0,0,0};
    if (unlikely(__pyx_kwds)) {
      Py_ssize_t kw_args;
      const Py_ssize_t pos_args = PyTuple_GET_SIZE(__pyx_args);
      switch (pos_args) {
        case  5: values[4] = PyTuple_GET_ITEM(__pyx_args, 4);
        case  4: values[3] = PyTuple_GET_ITEM(__pyx_args, 3);
        case  3: values[2] = PyTuple_GET_ITEM(__pyx_args, 2);
        case  2: values[1] = PyTuple_GET_ITEM(__pyx_args, 1);
        case  1: values[0] = PyTuple_GET_ITEM(__pyx_args, 0);
        case  0: break;
        default: goto __pyx_L5_argtuple_error;
      }
      kw_args = PyDict_Size(__pyx_kwds);
      switch (pos_args) {
        case  0:
        if (likely((values[0] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__f)) != 0)) kw_args--;
        else goto __pyx_L5_argtuple_error;
        case  1:
        if (likely((values[1] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__nDim)) != 0)) kw_args--;
        else {
          __Pyx_RaiseArgtupleInvalid("optimize", 1, 5, 5, 1); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 220; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
        }
        case  2:
        if (likely((values[2] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__np_lb)) != 0)) kw_args--;
        else {
          __Pyx_RaiseArgtupleInvalid("optimize", 1, 5, 5, 2); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 220; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
        }
        case  3:
        if (likely((values[3] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__np_ub)) != 0)) kw_args--;
        else {
          __Pyx_RaiseArgtupleInvalid("optimize", 1, 5, 5, 3); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 220; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
        }
        case  4:
        if (likely((values[4] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__dparams)) != 0)) kw_args--;
        else {
          __Pyx_RaiseArgtupleInvalid("optimize", 1, 5, 5, 4); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 220; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
        }
      }
      if (unlikely(kw_args > 0)) {
        if (unlikely(__Pyx_ParseOptionalKeywords(__pyx_kwds, __pyx_pyargnames, 0, values, pos_args, "optimize") < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 220; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
      }
    } else if (PyTuple_GET_SIZE(__pyx_args) != 5) {
      goto __pyx_L5_argtuple_error;
    } else {
      values[0] = PyTuple_GET_ITEM(__pyx_args, 0);
      values[1] = PyTuple_GET_ITEM(__pyx_args, 1);
      values[2] = PyTuple_GET_ITEM(__pyx_args, 2);
      values[3] = PyTuple_GET_ITEM(__pyx_args, 3);
      values[4] = PyTuple_GET_ITEM(__pyx_args, 4);
    }
    __pyx_v_f = values[0];
    __pyx_v_nDim = __Pyx_PyInt_AsInt(values[1]); if (unlikely((__pyx_v_nDim == (int)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 220; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
    __pyx_v_np_lb = ((PyArrayObject *)values[2]);
    __pyx_v_np_ub = ((PyArrayObject *)values[3]);
    __pyx_v_dparams = ((PyObject*)values[4]);
  }
  goto __pyx_L4_argument_unpacking_done;
  __pyx_L5_argtuple_error:;
  __Pyx_RaiseArgtupleInvalid("optimize", 1, 5, 5, PyTuple_GET_SIZE(__pyx_args)); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 220; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
  __pyx_L3_error:;
  __Pyx_AddTraceback("bayesopt.optimize", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __Pyx_RefNannyFinishContext();
  return NULL;
  __pyx_L4_argument_unpacking_done:;
  if (unlikely(!__Pyx_ArgTypeTest(((PyObject *)__pyx_v_np_lb), __pyx_ptype_5numpy_ndarray, 1, "np_lb", 0))) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 220; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  if (unlikely(!__Pyx_ArgTypeTest(((PyObject *)__pyx_v_np_ub), __pyx_ptype_5numpy_ndarray, 1, "np_ub", 0))) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 221; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  if (unlikely(!__Pyx_ArgTypeTest(((PyObject *)__pyx_v_dparams), (&PyDict_Type), 1, "dparams", 1))) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 221; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_r = __pyx_pf_8bayesopt_2optimize(__pyx_self, __pyx_v_f, __pyx_v_nDim, __pyx_v_np_lb, __pyx_v_np_ub, __pyx_v_dparams);
  goto __pyx_L0;
  __pyx_L1_error:;
  __pyx_r = NULL;
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "bayesopt.pyx":220
 *     elif error_code == -4: raise RuntimeError;
 * 
 * def optimize(f, int nDim, np.ndarray[np.double_t] np_lb,             # <<<<<<<<<<<<<<
 *              np.ndarray[np.double_t] np_ub, dict dparams):
 * 
 */

static PyObject *__pyx_pf_8bayesopt_2optimize(CYTHON_UNUSED PyObject *__pyx_self, PyObject *__pyx_v_f, int __pyx_v_nDim, PyArrayObject *__pyx_v_np_lb, PyArrayObject *__pyx_v_np_ub, PyObject *__pyx_v_dparams) {
  bopt_params __pyx_v_params;
  double __pyx_v_minf[1];
  PyArrayObject *__pyx_v_np_x = 0;
  PyArrayObject *__pyx_v_lb = 0;
  PyArrayObject *__pyx_v_ub = 0;
  PyArrayObject *__pyx_v_x = 0;
  int __pyx_v_error_code;
  double __pyx_v_min_value;
  __Pyx_LocalBuf_ND __pyx_pybuffernd_lb;
  __Pyx_Buffer __pyx_pybuffer_lb;
  __Pyx_LocalBuf_ND __pyx_pybuffernd_np_lb;
  __Pyx_Buffer __pyx_pybuffer_np_lb;
  __Pyx_LocalBuf_ND __pyx_pybuffernd_np_ub;
  __Pyx_Buffer __pyx_pybuffer_np_ub;
  __Pyx_LocalBuf_ND __pyx_pybuffernd_ub;
  __Pyx_Buffer __pyx_pybuffer_ub;
  __Pyx_LocalBuf_ND __pyx_pybuffernd_x;
  __Pyx_Buffer __pyx_pybuffer_x;
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  PyObject *__pyx_t_1 = NULL;
  PyObject *__pyx_t_2 = NULL;
  PyObject *__pyx_t_3 = NULL;
  PyObject *__pyx_t_4 = NULL;
  PyObject *__pyx_t_5 = NULL;
  PyArrayObject *__pyx_t_6 = NULL;
  int __pyx_t_7;
  PyObject *__pyx_t_8 = NULL;
  PyObject *__pyx_t_9 = NULL;
  PyObject *__pyx_t_10 = NULL;
  PyArrayObject *__pyx_t_11 = NULL;
  PyArrayObject *__pyx_t_12 = NULL;
  long __pyx_t_13;
  long __pyx_t_14;
  long __pyx_t_15;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("optimize", 0);
  __pyx_pybuffer_lb.pybuffer.buf = NULL;
  __pyx_pybuffer_lb.refcount = 0;
  __pyx_pybuffernd_lb.data = NULL;
  __pyx_pybuffernd_lb.rcbuffer = &__pyx_pybuffer_lb;
  __pyx_pybuffer_ub.pybuffer.buf = NULL;
  __pyx_pybuffer_ub.refcount = 0;
  __pyx_pybuffernd_ub.data = NULL;
  __pyx_pybuffernd_ub.rcbuffer = &__pyx_pybuffer_ub;
  __pyx_pybuffer_x.pybuffer.buf = NULL;
  __pyx_pybuffer_x.refcount = 0;
  __pyx_pybuffernd_x.data = NULL;
  __pyx_pybuffernd_x.rcbuffer = &__pyx_pybuffer_x;
  __pyx_pybuffer_np_lb.pybuffer.buf = NULL;
  __pyx_pybuffer_np_lb.refcount = 0;
  __pyx_pybuffernd_np_lb.data = NULL;
  __pyx_pybuffernd_np_lb.rcbuffer = &__pyx_pybuffer_np_lb;
  __pyx_pybuffer_np_ub.pybuffer.buf = NULL;
  __pyx_pybuffer_np_ub.refcount = 0;
  __pyx_pybuffernd_np_ub.data = NULL;
  __pyx_pybuffernd_np_ub.rcbuffer = &__pyx_pybuffer_np_ub;
  {
    __Pyx_BufFmt_StackElem __pyx_stack[1];
    if (unlikely(__Pyx_GetBufferAndValidate(&__pyx_pybuffernd_np_lb.rcbuffer->pybuffer, (PyObject*)__pyx_v_np_lb, &__Pyx_TypeInfo_nn___pyx_t_5numpy_double_t, PyBUF_FORMAT| PyBUF_STRIDES, 1, 0, __pyx_stack) == -1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 220; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_pybuffernd_np_lb.diminfo[0].strides = __pyx_pybuffernd_np_lb.rcbuffer->pybuffer.strides[0]; __pyx_pybuffernd_np_lb.diminfo[0].shape = __pyx_pybuffernd_np_lb.rcbuffer->pybuffer.shape[0];
  {
    __Pyx_BufFmt_StackElem __pyx_stack[1];
    if (unlikely(__Pyx_GetBufferAndValidate(&__pyx_pybuffernd_np_ub.rcbuffer->pybuffer, (PyObject*)__pyx_v_np_ub, &__Pyx_TypeInfo_nn___pyx_t_5numpy_double_t, PyBUF_FORMAT| PyBUF_STRIDES, 1, 0, __pyx_stack) == -1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 220; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_pybuffernd_np_ub.diminfo[0].strides = __pyx_pybuffernd_np_ub.rcbuffer->pybuffer.strides[0]; __pyx_pybuffernd_np_ub.diminfo[0].shape = __pyx_pybuffernd_np_ub.rcbuffer->pybuffer.shape[0];

  /* "bayesopt.pyx":223
 *              np.ndarray[np.double_t] np_ub, dict dparams):
 * 
 *     cdef bopt_params params = dict2structparams(dparams)             # <<<<<<<<<<<<<<
 *     cdef double minf[1]
 *     cdef np.ndarray np_x = np.ones([nDim], dtype=np.double)*0.5
 */
  __pyx_v_params = __pyx_f_8bayesopt_dict2structparams(__pyx_v_dparams);

  /* "bayesopt.pyx":225
 *     cdef bopt_params params = dict2structparams(dparams)
 *     cdef double minf[1]
 *     cdef np.ndarray np_x = np.ones([nDim], dtype=np.double)*0.5             # <<<<<<<<<<<<<<
 * 
 *     cdef np.ndarray[np.double_t, ndim=1, mode="c"] lb
 */
  __pyx_t_1 = __Pyx_GetModuleGlobalName(__pyx_n_s__np); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 225; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_t_2 = __Pyx_PyObject_GetAttrStr(__pyx_t_1, __pyx_n_s__ones); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 225; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  __pyx_t_1 = PyInt_FromLong(__pyx_v_nDim); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 225; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_t_3 = PyList_New(1); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 225; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_3);
  PyList_SET_ITEM(__pyx_t_3, 0, __pyx_t_1);
  __Pyx_GIVEREF(__pyx_t_1);
  __pyx_t_1 = 0;
  __pyx_t_1 = PyTuple_New(1); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 225; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  PyTuple_SET_ITEM(__pyx_t_1, 0, ((PyObject *)__pyx_t_3));
  __Pyx_GIVEREF(((PyObject *)__pyx_t_3));
  __pyx_t_3 = 0;
  __pyx_t_3 = PyDict_New(); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 225; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(((PyObject *)__pyx_t_3));
  __pyx_t_4 = __Pyx_GetModuleGlobalName(__pyx_n_s__np); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 225; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_4);
  __pyx_t_5 = __Pyx_PyObject_GetAttrStr(__pyx_t_4, __pyx_n_s__double); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 225; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_5);
  __Pyx_DECREF(__pyx_t_4); __pyx_t_4 = 0;
  if (PyDict_SetItem(__pyx_t_3, ((PyObject *)__pyx_n_s__dtype), __pyx_t_5) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 225; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
  __pyx_t_5 = PyObject_Call(__pyx_t_2, ((PyObject *)__pyx_t_1), ((PyObject *)__pyx_t_3)); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 225; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_5);
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_1)); __pyx_t_1 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_3)); __pyx_t_3 = 0;
  __pyx_t_3 = PyFloat_FromDouble(0.5); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 225; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_3);
  __pyx_t_1 = PyNumber_Multiply(__pyx_t_5, __pyx_t_3); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 225; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
  __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
  if (!(likely(((__pyx_t_1) == Py_None) || likely(__Pyx_TypeTest(__pyx_t_1, __pyx_ptype_5numpy_ndarray))))) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 225; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_v_np_x = ((PyArrayObject *)__pyx_t_1);
  __pyx_t_1 = 0;

  /* "bayesopt.pyx":231
 *     cdef np.ndarray[np.double_t, ndim=1, mode="c"] x
 * 
 *     lb = np.ascontiguousarray(np_lb,dtype=np.double)             # <<<<<<<<<<<<<<
 *     ub = np.ascontiguousarray(np_ub,dtype=np.double)
 *     x  = np.ascontiguousarray(np_x,dtype=np.double)
 */
  __pyx_t_1 = __Pyx_GetModuleGlobalName(__pyx_n_s__np); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 231; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_t_3 = __Pyx_PyObject_GetAttrStr(__pyx_t_1, __pyx_n_s__ascontiguousarray); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 231; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_3);
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  __pyx_t_1 = PyTuple_New(1); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 231; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __Pyx_INCREF(((PyObject *)__pyx_v_np_lb));
  PyTuple_SET_ITEM(__pyx_t_1, 0, ((PyObject *)__pyx_v_np_lb));
  __Pyx_GIVEREF(((PyObject *)__pyx_v_np_lb));
  __pyx_t_5 = PyDict_New(); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 231; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(((PyObject *)__pyx_t_5));
  __pyx_t_2 = __Pyx_GetModuleGlobalName(__pyx_n_s__np); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 231; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __pyx_t_4 = __Pyx_PyObject_GetAttrStr(__pyx_t_2, __pyx_n_s__double); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 231; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_4);
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  if (PyDict_SetItem(__pyx_t_5, ((PyObject *)__pyx_n_s__dtype), __pyx_t_4) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 231; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_4); __pyx_t_4 = 0;
  __pyx_t_4 = PyObject_Call(__pyx_t_3, ((PyObject *)__pyx_t_1), ((PyObject *)__pyx_t_5)); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 231; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_4);
  __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_1)); __pyx_t_1 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_5)); __pyx_t_5 = 0;
  if (!(likely(((__pyx_t_4) == Py_None) || likely(__Pyx_TypeTest(__pyx_t_4, __pyx_ptype_5numpy_ndarray))))) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 231; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_t_6 = ((PyArrayObject *)__pyx_t_4);
  {
    __Pyx_BufFmt_StackElem __pyx_stack[1];
    __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_lb.rcbuffer->pybuffer);
    __pyx_t_7 = __Pyx_GetBufferAndValidate(&__pyx_pybuffernd_lb.rcbuffer->pybuffer, (PyObject*)__pyx_t_6, &__Pyx_TypeInfo_nn___pyx_t_5numpy_double_t, PyBUF_FORMAT| PyBUF_C_CONTIGUOUS, 1, 0, __pyx_stack);
    if (unlikely(__pyx_t_7 < 0)) {
      PyErr_Fetch(&__pyx_t_8, &__pyx_t_9, &__pyx_t_10);
      if (unlikely(__Pyx_GetBufferAndValidate(&__pyx_pybuffernd_lb.rcbuffer->pybuffer, (PyObject*)__pyx_v_lb, &__Pyx_TypeInfo_nn___pyx_t_5numpy_double_t, PyBUF_FORMAT| PyBUF_C_CONTIGUOUS, 1, 0, __pyx_stack) == -1)) {
        Py_XDECREF(__pyx_t_8); Py_XDECREF(__pyx_t_9); Py_XDECREF(__pyx_t_10);
        __Pyx_RaiseBufferFallbackError();
      } else {
        PyErr_Restore(__pyx_t_8, __pyx_t_9, __pyx_t_10);
      }
    }
    __pyx_pybuffernd_lb.diminfo[0].strides = __pyx_pybuffernd_lb.rcbuffer->pybuffer.strides[0]; __pyx_pybuffernd_lb.diminfo[0].shape = __pyx_pybuffernd_lb.rcbuffer->pybuffer.shape[0];
    if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 231; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_6 = 0;
  __pyx_v_lb = ((PyArrayObject *)__pyx_t_4);
  __pyx_t_4 = 0;

  /* "bayesopt.pyx":232
 * 
 *     lb = np.ascontiguousarray(np_lb,dtype=np.double)
 *     ub = np.ascontiguousarray(np_ub,dtype=np.double)             # <<<<<<<<<<<<<<
 *     x  = np.ascontiguousarray(np_x,dtype=np.double)
 * 
 */
  __pyx_t_4 = __Pyx_GetModuleGlobalName(__pyx_n_s__np); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 232; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_4);
  __pyx_t_5 = __Pyx_PyObject_GetAttrStr(__pyx_t_4, __pyx_n_s__ascontiguousarray); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 232; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_5);
  __Pyx_DECREF(__pyx_t_4); __pyx_t_4 = 0;
  __pyx_t_4 = PyTuple_New(1); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 232; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_4);
  __Pyx_INCREF(((PyObject *)__pyx_v_np_ub));
  PyTuple_SET_ITEM(__pyx_t_4, 0, ((PyObject *)__pyx_v_np_ub));
  __Pyx_GIVEREF(((PyObject *)__pyx_v_np_ub));
  __pyx_t_1 = PyDict_New(); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 232; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(((PyObject *)__pyx_t_1));
  __pyx_t_3 = __Pyx_GetModuleGlobalName(__pyx_n_s__np); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 232; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_3);
  __pyx_t_2 = __Pyx_PyObject_GetAttrStr(__pyx_t_3, __pyx_n_s__double); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 232; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
  if (PyDict_SetItem(__pyx_t_1, ((PyObject *)__pyx_n_s__dtype), __pyx_t_2) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 232; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  __pyx_t_2 = PyObject_Call(__pyx_t_5, ((PyObject *)__pyx_t_4), ((PyObject *)__pyx_t_1)); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 232; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_4)); __pyx_t_4 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_1)); __pyx_t_1 = 0;
  if (!(likely(((__pyx_t_2) == Py_None) || likely(__Pyx_TypeTest(__pyx_t_2, __pyx_ptype_5numpy_ndarray))))) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 232; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_t_11 = ((PyArrayObject *)__pyx_t_2);
  {
    __Pyx_BufFmt_StackElem __pyx_stack[1];
    __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_ub.rcbuffer->pybuffer);
    __pyx_t_7 = __Pyx_GetBufferAndValidate(&__pyx_pybuffernd_ub.rcbuffer->pybuffer, (PyObject*)__pyx_t_11, &__Pyx_TypeInfo_nn___pyx_t_5numpy_double_t, PyBUF_FORMAT| PyBUF_C_CONTIGUOUS, 1, 0, __pyx_stack);
    if (unlikely(__pyx_t_7 < 0)) {
      PyErr_Fetch(&__pyx_t_10, &__pyx_t_9, &__pyx_t_8);
      if (unlikely(__Pyx_GetBufferAndValidate(&__pyx_pybuffernd_ub.rcbuffer->pybuffer, (PyObject*)__pyx_v_ub, &__Pyx_TypeInfo_nn___pyx_t_5numpy_double_t, PyBUF_FORMAT| PyBUF_C_CONTIGUOUS, 1, 0, __pyx_stack) == -1)) {
        Py_XDECREF(__pyx_t_10); Py_XDECREF(__pyx_t_9); Py_XDECREF(__pyx_t_8);
        __Pyx_RaiseBufferFallbackError();
      } else {
        PyErr_Restore(__pyx_t_10, __pyx_t_9, __pyx_t_8);
      }
    }
    __pyx_pybuffernd_ub.diminfo[0].strides = __pyx_pybuffernd_ub.rcbuffer->pybuffer.strides[0]; __pyx_pybuffernd_ub.diminfo[0].shape = __pyx_pybuffernd_ub.rcbuffer->pybuffer.shape[0];
    if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 232; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_11 = 0;
  __pyx_v_ub = ((PyArrayObject *)__pyx_t_2);
  __pyx_t_2 = 0;

  /* "bayesopt.pyx":233
 *     lb = np.ascontiguousarray(np_lb,dtype=np.double)
 *     ub = np.ascontiguousarray(np_ub,dtype=np.double)
 *     x  = np.ascontiguousarray(np_x,dtype=np.double)             # <<<<<<<<<<<<<<
 * 
 *     Py_INCREF(f)
 */
  __pyx_t_2 = __Pyx_GetModuleGlobalName(__pyx_n_s__np); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 233; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __pyx_t_1 = __Pyx_PyObject_GetAttrStr(__pyx_t_2, __pyx_n_s__ascontiguousarray); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 233; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  __pyx_t_2 = PyTuple_New(1); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 233; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_INCREF(((PyObject *)__pyx_v_np_x));
  PyTuple_SET_ITEM(__pyx_t_2, 0, ((PyObject *)__pyx_v_np_x));
  __Pyx_GIVEREF(((PyObject *)__pyx_v_np_x));
  __pyx_t_4 = PyDict_New(); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 233; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(((PyObject *)__pyx_t_4));
  __pyx_t_5 = __Pyx_GetModuleGlobalName(__pyx_n_s__np); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 233; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_5);
  __pyx_t_3 = __Pyx_PyObject_GetAttrStr(__pyx_t_5, __pyx_n_s__double); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 233; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_3);
  __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
  if (PyDict_SetItem(__pyx_t_4, ((PyObject *)__pyx_n_s__dtype), __pyx_t_3) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 233; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
  __pyx_t_3 = PyObject_Call(__pyx_t_1, ((PyObject *)__pyx_t_2), ((PyObject *)__pyx_t_4)); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 233; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_3);
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_2)); __pyx_t_2 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_4)); __pyx_t_4 = 0;
  if (!(likely(((__pyx_t_3) == Py_None) || likely(__Pyx_TypeTest(__pyx_t_3, __pyx_ptype_5numpy_ndarray))))) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 233; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_t_12 = ((PyArrayObject *)__pyx_t_3);
  {
    __Pyx_BufFmt_StackElem __pyx_stack[1];
    __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_x.rcbuffer->pybuffer);
    __pyx_t_7 = __Pyx_GetBufferAndValidate(&__pyx_pybuffernd_x.rcbuffer->pybuffer, (PyObject*)__pyx_t_12, &__Pyx_TypeInfo_nn___pyx_t_5numpy_double_t, PyBUF_FORMAT| PyBUF_C_CONTIGUOUS, 1, 0, __pyx_stack);
    if (unlikely(__pyx_t_7 < 0)) {
      PyErr_Fetch(&__pyx_t_8, &__pyx_t_9, &__pyx_t_10);
      if (unlikely(__Pyx_GetBufferAndValidate(&__pyx_pybuffernd_x.rcbuffer->pybuffer, (PyObject*)__pyx_v_x, &__Pyx_TypeInfo_nn___pyx_t_5numpy_double_t, PyBUF_FORMAT| PyBUF_C_CONTIGUOUS, 1, 0, __pyx_stack) == -1)) {
        Py_XDECREF(__pyx_t_8); Py_XDECREF(__pyx_t_9); Py_XDECREF(__pyx_t_10);
        __Pyx_RaiseBufferFallbackError();
      } else {
        PyErr_Restore(__pyx_t_8, __pyx_t_9, __pyx_t_10);
      }
    }
    __pyx_pybuffernd_x.diminfo[0].strides = __pyx_pybuffernd_x.rcbuffer->pybuffer.strides[0]; __pyx_pybuffernd_x.diminfo[0].shape = __pyx_pybuffernd_x.rcbuffer->pybuffer.shape[0];
    if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 233; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_12 = 0;
  __pyx_v_x = ((PyArrayObject *)__pyx_t_3);
  __pyx_t_3 = 0;

  /* "bayesopt.pyx":235
 *     x  = np.ascontiguousarray(np_x,dtype=np.double)
 * 
 *     Py_INCREF(f)             # <<<<<<<<<<<<<<
 * 
 *     error_code = bayes_optimization(nDim, callback, <void *> f,
 */
  Py_INCREF(__pyx_v_f);

  /* "bayesopt.pyx":238
 * 
 *     error_code = bayes_optimization(nDim, callback, <void *> f,
 *                                     &lb[0], &ub[0], &x[0], minf, params)             # <<<<<<<<<<<<<<
 * 
 * 
 */
  __pyx_t_13 = 0;
  __pyx_t_7 = -1;
  if (__pyx_t_13 < 0) {
    __pyx_t_13 += __pyx_pybuffernd_lb.diminfo[0].shape;
    if (unlikely(__pyx_t_13 < 0)) __pyx_t_7 = 0;
  } else if (unlikely(__pyx_t_13 >= __pyx_pybuffernd_lb.diminfo[0].shape)) __pyx_t_7 = 0;
  if (unlikely(__pyx_t_7 != -1)) {
    __Pyx_RaiseBufferIndexError(__pyx_t_7);
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 238; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_14 = 0;
  __pyx_t_7 = -1;
  if (__pyx_t_14 < 0) {
    __pyx_t_14 += __pyx_pybuffernd_ub.diminfo[0].shape;
    if (unlikely(__pyx_t_14 < 0)) __pyx_t_7 = 0;
  } else if (unlikely(__pyx_t_14 >= __pyx_pybuffernd_ub.diminfo[0].shape)) __pyx_t_7 = 0;
  if (unlikely(__pyx_t_7 != -1)) {
    __Pyx_RaiseBufferIndexError(__pyx_t_7);
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 238; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_15 = 0;
  __pyx_t_7 = -1;
  if (__pyx_t_15 < 0) {
    __pyx_t_15 += __pyx_pybuffernd_x.diminfo[0].shape;
    if (unlikely(__pyx_t_15 < 0)) __pyx_t_7 = 0;
  } else if (unlikely(__pyx_t_15 >= __pyx_pybuffernd_x.diminfo[0].shape)) __pyx_t_7 = 0;
  if (unlikely(__pyx_t_7 != -1)) {
    __Pyx_RaiseBufferIndexError(__pyx_t_7);
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 238; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_v_error_code = bayes_optimization(__pyx_v_nDim, __pyx_f_8bayesopt_callback, ((void *)__pyx_v_f), (&(*__Pyx_BufPtrCContig1d(__pyx_t_5numpy_double_t *, __pyx_pybuffernd_lb.rcbuffer->pybuffer.buf, __pyx_t_13, __pyx_pybuffernd_lb.diminfo[0].strides))), (&(*__Pyx_BufPtrCContig1d(__pyx_t_5numpy_double_t *, __pyx_pybuffernd_ub.rcbuffer->pybuffer.buf, __pyx_t_14, __pyx_pybuffernd_ub.diminfo[0].strides))), (&(*__Pyx_BufPtrCContig1d(__pyx_t_5numpy_double_t *, __pyx_pybuffernd_x.rcbuffer->pybuffer.buf, __pyx_t_15, __pyx_pybuffernd_x.diminfo[0].strides))), __pyx_v_minf, __pyx_v_params);

  /* "bayesopt.pyx":241
 * 
 * 
 *     Py_DECREF(f)             # <<<<<<<<<<<<<<
 * 
 *     raise_problem(error_code)
 */
  Py_DECREF(__pyx_v_f);

  /* "bayesopt.pyx":243
 *     Py_DECREF(f)
 * 
 *     raise_problem(error_code)             # <<<<<<<<<<<<<<
 * 
 *     min_value = minf[0]
 */
  __pyx_t_3 = __Pyx_GetModuleGlobalName(__pyx_n_s__raise_problem); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 243; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_3);
  __pyx_t_4 = PyInt_FromLong(__pyx_v_error_code); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 243; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_4);
  __pyx_t_2 = PyTuple_New(1); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 243; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  PyTuple_SET_ITEM(__pyx_t_2, 0, __pyx_t_4);
  __Pyx_GIVEREF(__pyx_t_4);
  __pyx_t_4 = 0;
  __pyx_t_4 = PyObject_Call(__pyx_t_3, ((PyObject *)__pyx_t_2), NULL); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 243; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_4);
  __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_2)); __pyx_t_2 = 0;
  __Pyx_DECREF(__pyx_t_4); __pyx_t_4 = 0;

  /* "bayesopt.pyx":245
 *     raise_problem(error_code)
 * 
 *     min_value = minf[0]             # <<<<<<<<<<<<<<
 *     return min_value,np_x,error_code
 * 
 */
  __pyx_v_min_value = (__pyx_v_minf[0]);

  /* "bayesopt.pyx":246
 * 
 *     min_value = minf[0]
 *     return min_value,np_x,error_code             # <<<<<<<<<<<<<<
 * 
 * 
 */
  __Pyx_XDECREF(__pyx_r);
  __pyx_t_4 = PyFloat_FromDouble(__pyx_v_min_value); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 246; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_4);
  __pyx_t_2 = PyInt_FromLong(__pyx_v_error_code); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 246; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __pyx_t_3 = PyTuple_New(3); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 246; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_3);
  PyTuple_SET_ITEM(__pyx_t_3, 0, __pyx_t_4);
  __Pyx_GIVEREF(__pyx_t_4);
  __Pyx_INCREF(((PyObject *)__pyx_v_np_x));
  PyTuple_SET_ITEM(__pyx_t_3, 1, ((PyObject *)__pyx_v_np_x));
  __Pyx_GIVEREF(((PyObject *)__pyx_v_np_x));
  PyTuple_SET_ITEM(__pyx_t_3, 2, __pyx_t_2);
  __Pyx_GIVEREF(__pyx_t_2);
  __pyx_t_4 = 0;
  __pyx_t_2 = 0;
  __pyx_r = ((PyObject *)__pyx_t_3);
  __pyx_t_3 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  __Pyx_XDECREF(__pyx_t_2);
  __Pyx_XDECREF(__pyx_t_3);
  __Pyx_XDECREF(__pyx_t_4);
  __Pyx_XDECREF(__pyx_t_5);
  { PyObject *__pyx_type, *__pyx_value, *__pyx_tb;
    __Pyx_ErrFetch(&__pyx_type, &__pyx_value, &__pyx_tb);
    __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_lb.rcbuffer->pybuffer);
    __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_np_lb.rcbuffer->pybuffer);
    __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_np_ub.rcbuffer->pybuffer);
    __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_ub.rcbuffer->pybuffer);
    __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_x.rcbuffer->pybuffer);
  __Pyx_ErrRestore(__pyx_type, __pyx_value, __pyx_tb);}
  __Pyx_AddTraceback("bayesopt.optimize", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = NULL;
  goto __pyx_L2;
  __pyx_L0:;
  __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_lb.rcbuffer->pybuffer);
  __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_np_lb.rcbuffer->pybuffer);
  __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_np_ub.rcbuffer->pybuffer);
  __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_ub.rcbuffer->pybuffer);
  __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_x.rcbuffer->pybuffer);
  __pyx_L2:;
  __Pyx_XDECREF((PyObject *)__pyx_v_np_x);
  __Pyx_XDECREF((PyObject *)__pyx_v_lb);
  __Pyx_XDECREF((PyObject *)__pyx_v_ub);
  __Pyx_XDECREF((PyObject *)__pyx_v_x);
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* Python wrapper */
static PyObject *__pyx_pw_8bayesopt_5optimize_discrete(PyObject *__pyx_self, PyObject *__pyx_args, PyObject *__pyx_kwds); /*proto*/
static PyMethodDef __pyx_mdef_8bayesopt_5optimize_discrete = {__Pyx_NAMESTR("optimize_discrete"), (PyCFunction)__pyx_pw_8bayesopt_5optimize_discrete, METH_VARARGS|METH_KEYWORDS, __Pyx_DOCSTR(0)};
static PyObject *__pyx_pw_8bayesopt_5optimize_discrete(PyObject *__pyx_self, PyObject *__pyx_args, PyObject *__pyx_kwds) {
  PyObject *__pyx_v_f = 0;
  PyArrayObject *__pyx_v_np_valid_x = 0;
  PyObject *__pyx_v_dparams = 0;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  PyObject *__pyx_r = 0;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("optimize_discrete (wrapper)", 0);
  {
    static PyObject **__pyx_pyargnames[] = {&__pyx_n_s__f,&__pyx_n_s__np_valid_x,&__pyx_n_s__dparams,0};
    PyObject* values[3] = {0,0,0};
    if (unlikely(__pyx_kwds)) {
      Py_ssize_t kw_args;
      const Py_ssize_t pos_args = PyTuple_GET_SIZE(__pyx_args);
      switch (pos_args) {
        case  3: values[2] = PyTuple_GET_ITEM(__pyx_args, 2);
        case  2: values[1] = PyTuple_GET_ITEM(__pyx_args, 1);
        case  1: values[0] = PyTuple_GET_ITEM(__pyx_args, 0);
        case  0: break;
        default: goto __pyx_L5_argtuple_error;
      }
      kw_args = PyDict_Size(__pyx_kwds);
      switch (pos_args) {
        case  0:
        if (likely((values[0] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__f)) != 0)) kw_args--;
        else goto __pyx_L5_argtuple_error;
        case  1:
        if (likely((values[1] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__np_valid_x)) != 0)) kw_args--;
        else {
          __Pyx_RaiseArgtupleInvalid("optimize_discrete", 1, 3, 3, 1); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 249; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
        }
        case  2:
        if (likely((values[2] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__dparams)) != 0)) kw_args--;
        else {
          __Pyx_RaiseArgtupleInvalid("optimize_discrete", 1, 3, 3, 2); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 249; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
        }
      }
      if (unlikely(kw_args > 0)) {
        if (unlikely(__Pyx_ParseOptionalKeywords(__pyx_kwds, __pyx_pyargnames, 0, values, pos_args, "optimize_discrete") < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 249; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
      }
    } else if (PyTuple_GET_SIZE(__pyx_args) != 3) {
      goto __pyx_L5_argtuple_error;
    } else {
      values[0] = PyTuple_GET_ITEM(__pyx_args, 0);
      values[1] = PyTuple_GET_ITEM(__pyx_args, 1);
      values[2] = PyTuple_GET_ITEM(__pyx_args, 2);
    }
    __pyx_v_f = values[0];
    __pyx_v_np_valid_x = ((PyArrayObject *)values[1]);
    __pyx_v_dparams = ((PyObject*)values[2]);
  }
  goto __pyx_L4_argument_unpacking_done;
  __pyx_L5_argtuple_error:;
  __Pyx_RaiseArgtupleInvalid("optimize_discrete", 1, 3, 3, PyTuple_GET_SIZE(__pyx_args)); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 249; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
  __pyx_L3_error:;
  __Pyx_AddTraceback("bayesopt.optimize_discrete", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __Pyx_RefNannyFinishContext();
  return NULL;
  __pyx_L4_argument_unpacking_done:;
  if (unlikely(!__Pyx_ArgTypeTest(((PyObject *)__pyx_v_np_valid_x), __pyx_ptype_5numpy_ndarray, 1, "np_valid_x", 0))) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 249; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  if (unlikely(!__Pyx_ArgTypeTest(((PyObject *)__pyx_v_dparams), (&PyDict_Type), 1, "dparams", 1))) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 250; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_r = __pyx_pf_8bayesopt_4optimize_discrete(__pyx_self, __pyx_v_f, __pyx_v_np_valid_x, __pyx_v_dparams);
  goto __pyx_L0;
  __pyx_L1_error:;
  __pyx_r = NULL;
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "bayesopt.pyx":249
 * 
 * 
 * def optimize_discrete(f, np.ndarray[np.double_t,ndim=2] np_valid_x,             # <<<<<<<<<<<<<<
 *                       dict dparams):
 * 
 */

static PyObject *__pyx_pf_8bayesopt_4optimize_discrete(CYTHON_UNUSED PyObject *__pyx_self, PyObject *__pyx_v_f, PyArrayObject *__pyx_v_np_valid_x, PyObject *__pyx_v_dparams) {
  bopt_params __pyx_v_params;
  npy_intp __pyx_v_nDim;
  double __pyx_v_minf[1];
  PyArrayObject *__pyx_v_np_x = 0;
  PyArrayObject *__pyx_v_x = 0;
  PyArrayObject *__pyx_v_valid_x = 0;
  int __pyx_v_error_code;
  double __pyx_v_min_value;
  __Pyx_LocalBuf_ND __pyx_pybuffernd_np_valid_x;
  __Pyx_Buffer __pyx_pybuffer_np_valid_x;
  __Pyx_LocalBuf_ND __pyx_pybuffernd_valid_x;
  __Pyx_Buffer __pyx_pybuffer_valid_x;
  __Pyx_LocalBuf_ND __pyx_pybuffernd_x;
  __Pyx_Buffer __pyx_pybuffer_x;
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  PyObject *__pyx_t_1 = NULL;
  PyObject *__pyx_t_2 = NULL;
  PyObject *__pyx_t_3 = NULL;
  PyObject *__pyx_t_4 = NULL;
  PyObject *__pyx_t_5 = NULL;
  PyArrayObject *__pyx_t_6 = NULL;
  int __pyx_t_7;
  PyObject *__pyx_t_8 = NULL;
  PyObject *__pyx_t_9 = NULL;
  PyObject *__pyx_t_10 = NULL;
  PyArrayObject *__pyx_t_11 = NULL;
  long __pyx_t_12;
  long __pyx_t_13;
  long __pyx_t_14;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("optimize_discrete", 0);
  __pyx_pybuffer_x.pybuffer.buf = NULL;
  __pyx_pybuffer_x.refcount = 0;
  __pyx_pybuffernd_x.data = NULL;
  __pyx_pybuffernd_x.rcbuffer = &__pyx_pybuffer_x;
  __pyx_pybuffer_valid_x.pybuffer.buf = NULL;
  __pyx_pybuffer_valid_x.refcount = 0;
  __pyx_pybuffernd_valid_x.data = NULL;
  __pyx_pybuffernd_valid_x.rcbuffer = &__pyx_pybuffer_valid_x;
  __pyx_pybuffer_np_valid_x.pybuffer.buf = NULL;
  __pyx_pybuffer_np_valid_x.refcount = 0;
  __pyx_pybuffernd_np_valid_x.data = NULL;
  __pyx_pybuffernd_np_valid_x.rcbuffer = &__pyx_pybuffer_np_valid_x;
  {
    __Pyx_BufFmt_StackElem __pyx_stack[1];
    if (unlikely(__Pyx_GetBufferAndValidate(&__pyx_pybuffernd_np_valid_x.rcbuffer->pybuffer, (PyObject*)__pyx_v_np_valid_x, &__Pyx_TypeInfo_nn___pyx_t_5numpy_double_t, PyBUF_FORMAT| PyBUF_STRIDES, 2, 0, __pyx_stack) == -1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 249; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_pybuffernd_np_valid_x.diminfo[0].strides = __pyx_pybuffernd_np_valid_x.rcbuffer->pybuffer.strides[0]; __pyx_pybuffernd_np_valid_x.diminfo[0].shape = __pyx_pybuffernd_np_valid_x.rcbuffer->pybuffer.shape[0]; __pyx_pybuffernd_np_valid_x.diminfo[1].strides = __pyx_pybuffernd_np_valid_x.rcbuffer->pybuffer.strides[1]; __pyx_pybuffernd_np_valid_x.diminfo[1].shape = __pyx_pybuffernd_np_valid_x.rcbuffer->pybuffer.shape[1];

  /* "bayesopt.pyx":252
 *                       dict dparams):
 * 
 *     cdef bopt_params params = dict2structparams(dparams)             # <<<<<<<<<<<<<<
 * 
 *     nDim = np_valid_x.shape[1]
 */
  __pyx_v_params = __pyx_f_8bayesopt_dict2structparams(__pyx_v_dparams);

  /* "bayesopt.pyx":254
 *     cdef bopt_params params = dict2structparams(dparams)
 * 
 *     nDim = np_valid_x.shape[1]             # <<<<<<<<<<<<<<
 * 
 *     cdef double minf[1]
 */
  __pyx_v_nDim = (__pyx_v_np_valid_x->dimensions[1]);

  /* "bayesopt.pyx":257
 * 
 *     cdef double minf[1]
 *     cdef np.ndarray np_x = np.zeros([nDim], dtype=np.double)             # <<<<<<<<<<<<<<
 * 
 *     cdef np.ndarray[np.double_t, ndim=1, mode="c"] x
 */
  __pyx_t_1 = __Pyx_GetModuleGlobalName(__pyx_n_s__np); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 257; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_t_2 = __Pyx_PyObject_GetAttrStr(__pyx_t_1, __pyx_n_s__zeros); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 257; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  __pyx_t_1 = __Pyx_PyInt_to_py_Py_intptr_t(__pyx_v_nDim); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 257; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_t_3 = PyList_New(1); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 257; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_3);
  PyList_SET_ITEM(__pyx_t_3, 0, __pyx_t_1);
  __Pyx_GIVEREF(__pyx_t_1);
  __pyx_t_1 = 0;
  __pyx_t_1 = PyTuple_New(1); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 257; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  PyTuple_SET_ITEM(__pyx_t_1, 0, ((PyObject *)__pyx_t_3));
  __Pyx_GIVEREF(((PyObject *)__pyx_t_3));
  __pyx_t_3 = 0;
  __pyx_t_3 = PyDict_New(); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 257; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(((PyObject *)__pyx_t_3));
  __pyx_t_4 = __Pyx_GetModuleGlobalName(__pyx_n_s__np); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 257; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_4);
  __pyx_t_5 = __Pyx_PyObject_GetAttrStr(__pyx_t_4, __pyx_n_s__double); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 257; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_5);
  __Pyx_DECREF(__pyx_t_4); __pyx_t_4 = 0;
  if (PyDict_SetItem(__pyx_t_3, ((PyObject *)__pyx_n_s__dtype), __pyx_t_5) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 257; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
  __pyx_t_5 = PyObject_Call(__pyx_t_2, ((PyObject *)__pyx_t_1), ((PyObject *)__pyx_t_3)); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 257; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_5);
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_1)); __pyx_t_1 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_3)); __pyx_t_3 = 0;
  if (!(likely(((__pyx_t_5) == Py_None) || likely(__Pyx_TypeTest(__pyx_t_5, __pyx_ptype_5numpy_ndarray))))) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 257; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_v_np_x = ((PyArrayObject *)__pyx_t_5);
  __pyx_t_5 = 0;

  /* "bayesopt.pyx":262
 *     cdef np.ndarray[np.double_t, ndim=2, mode="c"] valid_x
 * 
 *     x  = np.ascontiguousarray(np_x,dtype=np.double)             # <<<<<<<<<<<<<<
 *     valid_x = np.ascontiguousarray(np_valid_x,dtype=np.double)
 * 
 */
  __pyx_t_5 = __Pyx_GetModuleGlobalName(__pyx_n_s__np); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 262; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_5);
  __pyx_t_3 = __Pyx_PyObject_GetAttrStr(__pyx_t_5, __pyx_n_s__ascontiguousarray); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 262; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_3);
  __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
  __pyx_t_5 = PyTuple_New(1); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 262; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_5);
  __Pyx_INCREF(((PyObject *)__pyx_v_np_x));
  PyTuple_SET_ITEM(__pyx_t_5, 0, ((PyObject *)__pyx_v_np_x));
  __Pyx_GIVEREF(((PyObject *)__pyx_v_np_x));
  __pyx_t_1 = PyDict_New(); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 262; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(((PyObject *)__pyx_t_1));
  __pyx_t_2 = __Pyx_GetModuleGlobalName(__pyx_n_s__np); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 262; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __pyx_t_4 = __Pyx_PyObject_GetAttrStr(__pyx_t_2, __pyx_n_s__double); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 262; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_4);
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  if (PyDict_SetItem(__pyx_t_1, ((PyObject *)__pyx_n_s__dtype), __pyx_t_4) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 262; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_4); __pyx_t_4 = 0;
  __pyx_t_4 = PyObject_Call(__pyx_t_3, ((PyObject *)__pyx_t_5), ((PyObject *)__pyx_t_1)); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 262; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_4);
  __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_5)); __pyx_t_5 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_1)); __pyx_t_1 = 0;
  if (!(likely(((__pyx_t_4) == Py_None) || likely(__Pyx_TypeTest(__pyx_t_4, __pyx_ptype_5numpy_ndarray))))) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 262; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_t_6 = ((PyArrayObject *)__pyx_t_4);
  {
    __Pyx_BufFmt_StackElem __pyx_stack[1];
    __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_x.rcbuffer->pybuffer);
    __pyx_t_7 = __Pyx_GetBufferAndValidate(&__pyx_pybuffernd_x.rcbuffer->pybuffer, (PyObject*)__pyx_t_6, &__Pyx_TypeInfo_nn___pyx_t_5numpy_double_t, PyBUF_FORMAT| PyBUF_C_CONTIGUOUS, 1, 0, __pyx_stack);
    if (unlikely(__pyx_t_7 < 0)) {
      PyErr_Fetch(&__pyx_t_8, &__pyx_t_9, &__pyx_t_10);
      if (unlikely(__Pyx_GetBufferAndValidate(&__pyx_pybuffernd_x.rcbuffer->pybuffer, (PyObject*)__pyx_v_x, &__Pyx_TypeInfo_nn___pyx_t_5numpy_double_t, PyBUF_FORMAT| PyBUF_C_CONTIGUOUS, 1, 0, __pyx_stack) == -1)) {
        Py_XDECREF(__pyx_t_8); Py_XDECREF(__pyx_t_9); Py_XDECREF(__pyx_t_10);
        __Pyx_RaiseBufferFallbackError();
      } else {
        PyErr_Restore(__pyx_t_8, __pyx_t_9, __pyx_t_10);
      }
    }
    __pyx_pybuffernd_x.diminfo[0].strides = __pyx_pybuffernd_x.rcbuffer->pybuffer.strides[0]; __pyx_pybuffernd_x.diminfo[0].shape = __pyx_pybuffernd_x.rcbuffer->pybuffer.shape[0];
    if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 262; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_6 = 0;
  __pyx_v_x = ((PyArrayObject *)__pyx_t_4);
  __pyx_t_4 = 0;

  /* "bayesopt.pyx":263
 * 
 *     x  = np.ascontiguousarray(np_x,dtype=np.double)
 *     valid_x = np.ascontiguousarray(np_valid_x,dtype=np.double)             # <<<<<<<<<<<<<<
 * 
 *     Py_INCREF(f)
 */
  __pyx_t_4 = __Pyx_GetModuleGlobalName(__pyx_n_s__np); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 263; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_4);
  __pyx_t_1 = __Pyx_PyObject_GetAttrStr(__pyx_t_4, __pyx_n_s__ascontiguousarray); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 263; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __Pyx_DECREF(__pyx_t_4); __pyx_t_4 = 0;
  __pyx_t_4 = PyTuple_New(1); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 263; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_4);
  __Pyx_INCREF(((PyObject *)__pyx_v_np_valid_x));
  PyTuple_SET_ITEM(__pyx_t_4, 0, ((PyObject *)__pyx_v_np_valid_x));
  __Pyx_GIVEREF(((PyObject *)__pyx_v_np_valid_x));
  __pyx_t_5 = PyDict_New(); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 263; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(((PyObject *)__pyx_t_5));
  __pyx_t_3 = __Pyx_GetModuleGlobalName(__pyx_n_s__np); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 263; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_3);
  __pyx_t_2 = __Pyx_PyObject_GetAttrStr(__pyx_t_3, __pyx_n_s__double); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 263; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
  if (PyDict_SetItem(__pyx_t_5, ((PyObject *)__pyx_n_s__dtype), __pyx_t_2) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 263; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  __pyx_t_2 = PyObject_Call(__pyx_t_1, ((PyObject *)__pyx_t_4), ((PyObject *)__pyx_t_5)); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 263; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_4)); __pyx_t_4 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_5)); __pyx_t_5 = 0;
  if (!(likely(((__pyx_t_2) == Py_None) || likely(__Pyx_TypeTest(__pyx_t_2, __pyx_ptype_5numpy_ndarray))))) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 263; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_t_11 = ((PyArrayObject *)__pyx_t_2);
  {
    __Pyx_BufFmt_StackElem __pyx_stack[1];
    __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_valid_x.rcbuffer->pybuffer);
    __pyx_t_7 = __Pyx_GetBufferAndValidate(&__pyx_pybuffernd_valid_x.rcbuffer->pybuffer, (PyObject*)__pyx_t_11, &__Pyx_TypeInfo_nn___pyx_t_5numpy_double_t, PyBUF_FORMAT| PyBUF_C_CONTIGUOUS, 2, 0, __pyx_stack);
    if (unlikely(__pyx_t_7 < 0)) {
      PyErr_Fetch(&__pyx_t_10, &__pyx_t_9, &__pyx_t_8);
      if (unlikely(__Pyx_GetBufferAndValidate(&__pyx_pybuffernd_valid_x.rcbuffer->pybuffer, (PyObject*)__pyx_v_valid_x, &__Pyx_TypeInfo_nn___pyx_t_5numpy_double_t, PyBUF_FORMAT| PyBUF_C_CONTIGUOUS, 2, 0, __pyx_stack) == -1)) {
        Py_XDECREF(__pyx_t_10); Py_XDECREF(__pyx_t_9); Py_XDECREF(__pyx_t_8);
        __Pyx_RaiseBufferFallbackError();
      } else {
        PyErr_Restore(__pyx_t_10, __pyx_t_9, __pyx_t_8);
      }
    }
    __pyx_pybuffernd_valid_x.diminfo[0].strides = __pyx_pybuffernd_valid_x.rcbuffer->pybuffer.strides[0]; __pyx_pybuffernd_valid_x.diminfo[0].shape = __pyx_pybuffernd_valid_x.rcbuffer->pybuffer.shape[0]; __pyx_pybuffernd_valid_x.diminfo[1].strides = __pyx_pybuffernd_valid_x.rcbuffer->pybuffer.strides[1]; __pyx_pybuffernd_valid_x.diminfo[1].shape = __pyx_pybuffernd_valid_x.rcbuffer->pybuffer.shape[1];
    if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 263; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_11 = 0;
  __pyx_v_valid_x = ((PyArrayObject *)__pyx_t_2);
  __pyx_t_2 = 0;

  /* "bayesopt.pyx":265
 *     valid_x = np.ascontiguousarray(np_valid_x,dtype=np.double)
 * 
 *     Py_INCREF(f)             # <<<<<<<<<<<<<<
 * 
 *     error_code = bayes_optimization_disc(nDim, callback, <void *> f,
 */
  Py_INCREF(__pyx_v_f);

  /* "bayesopt.pyx":268
 * 
 *     error_code = bayes_optimization_disc(nDim, callback, <void *> f,
 *                                          &valid_x[0,0], np_valid_x.shape[0],             # <<<<<<<<<<<<<<
 *                                          &x[0], minf, params)
 * 
 */
  __pyx_t_12 = 0;
  __pyx_t_13 = 0;
  __pyx_t_7 = -1;
  if (__pyx_t_12 < 0) {
    __pyx_t_12 += __pyx_pybuffernd_valid_x.diminfo[0].shape;
    if (unlikely(__pyx_t_12 < 0)) __pyx_t_7 = 0;
  } else if (unlikely(__pyx_t_12 >= __pyx_pybuffernd_valid_x.diminfo[0].shape)) __pyx_t_7 = 0;
  if (__pyx_t_13 < 0) {
    __pyx_t_13 += __pyx_pybuffernd_valid_x.diminfo[1].shape;
    if (unlikely(__pyx_t_13 < 0)) __pyx_t_7 = 1;
  } else if (unlikely(__pyx_t_13 >= __pyx_pybuffernd_valid_x.diminfo[1].shape)) __pyx_t_7 = 1;
  if (unlikely(__pyx_t_7 != -1)) {
    __Pyx_RaiseBufferIndexError(__pyx_t_7);
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 268; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }

  /* "bayesopt.pyx":269
 *     error_code = bayes_optimization_disc(nDim, callback, <void *> f,
 *                                          &valid_x[0,0], np_valid_x.shape[0],
 *                                          &x[0], minf, params)             # <<<<<<<<<<<<<<
 * 
 *     Py_DECREF(f)
 */
  __pyx_t_14 = 0;
  __pyx_t_7 = -1;
  if (__pyx_t_14 < 0) {
    __pyx_t_14 += __pyx_pybuffernd_x.diminfo[0].shape;
    if (unlikely(__pyx_t_14 < 0)) __pyx_t_7 = 0;
  } else if (unlikely(__pyx_t_14 >= __pyx_pybuffernd_x.diminfo[0].shape)) __pyx_t_7 = 0;
  if (unlikely(__pyx_t_7 != -1)) {
    __Pyx_RaiseBufferIndexError(__pyx_t_7);
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 269; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_v_error_code = bayes_optimization_disc(__pyx_v_nDim, __pyx_f_8bayesopt_callback, ((void *)__pyx_v_f), (&(*__Pyx_BufPtrCContig2d(__pyx_t_5numpy_double_t *, __pyx_pybuffernd_valid_x.rcbuffer->pybuffer.buf, __pyx_t_12, __pyx_pybuffernd_valid_x.diminfo[0].strides, __pyx_t_13, __pyx_pybuffernd_valid_x.diminfo[1].strides))), (__pyx_v_np_valid_x->dimensions[0]), (&(*__Pyx_BufPtrCContig1d(__pyx_t_5numpy_double_t *, __pyx_pybuffernd_x.rcbuffer->pybuffer.buf, __pyx_t_14, __pyx_pybuffernd_x.diminfo[0].strides))), __pyx_v_minf, __pyx_v_params);

  /* "bayesopt.pyx":271
 *                                          &x[0], minf, params)
 * 
 *     Py_DECREF(f)             # <<<<<<<<<<<<<<
 * 
 *     raise_problem(error_code)
 */
  Py_DECREF(__pyx_v_f);

  /* "bayesopt.pyx":273
 *     Py_DECREF(f)
 * 
 *     raise_problem(error_code)             # <<<<<<<<<<<<<<
 * 
 *     min_value = minf[0]
 */
  __pyx_t_2 = __Pyx_GetModuleGlobalName(__pyx_n_s__raise_problem); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 273; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __pyx_t_5 = PyInt_FromLong(__pyx_v_error_code); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 273; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_5);
  __pyx_t_4 = PyTuple_New(1); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 273; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_4);
  PyTuple_SET_ITEM(__pyx_t_4, 0, __pyx_t_5);
  __Pyx_GIVEREF(__pyx_t_5);
  __pyx_t_5 = 0;
  __pyx_t_5 = PyObject_Call(__pyx_t_2, ((PyObject *)__pyx_t_4), NULL); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 273; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_5);
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_4)); __pyx_t_4 = 0;
  __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;

  /* "bayesopt.pyx":275
 *     raise_problem(error_code)
 * 
 *     min_value = minf[0]             # <<<<<<<<<<<<<<
 *     return min_value,np_x,error_code
 * 
 */
  __pyx_v_min_value = (__pyx_v_minf[0]);

  /* "bayesopt.pyx":276
 * 
 *     min_value = minf[0]
 *     return min_value,np_x,error_code             # <<<<<<<<<<<<<<
 * 
 * def optimize_categorical(f, np.ndarray[np.int_t,ndim=1] np_categories,
 */
  __Pyx_XDECREF(__pyx_r);
  __pyx_t_5 = PyFloat_FromDouble(__pyx_v_min_value); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 276; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_5);
  __pyx_t_4 = PyInt_FromLong(__pyx_v_error_code); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 276; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_4);
  __pyx_t_2 = PyTuple_New(3); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 276; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  PyTuple_SET_ITEM(__pyx_t_2, 0, __pyx_t_5);
  __Pyx_GIVEREF(__pyx_t_5);
  __Pyx_INCREF(((PyObject *)__pyx_v_np_x));
  PyTuple_SET_ITEM(__pyx_t_2, 1, ((PyObject *)__pyx_v_np_x));
  __Pyx_GIVEREF(((PyObject *)__pyx_v_np_x));
  PyTuple_SET_ITEM(__pyx_t_2, 2, __pyx_t_4);
  __Pyx_GIVEREF(__pyx_t_4);
  __pyx_t_5 = 0;
  __pyx_t_4 = 0;
  __pyx_r = ((PyObject *)__pyx_t_2);
  __pyx_t_2 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  __Pyx_XDECREF(__pyx_t_2);
  __Pyx_XDECREF(__pyx_t_3);
  __Pyx_XDECREF(__pyx_t_4);
  __Pyx_XDECREF(__pyx_t_5);
  { PyObject *__pyx_type, *__pyx_value, *__pyx_tb;
    __Pyx_ErrFetch(&__pyx_type, &__pyx_value, &__pyx_tb);
    __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_np_valid_x.rcbuffer->pybuffer);
    __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_valid_x.rcbuffer->pybuffer);
    __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_x.rcbuffer->pybuffer);
  __Pyx_ErrRestore(__pyx_type, __pyx_value, __pyx_tb);}
  __Pyx_AddTraceback("bayesopt.optimize_discrete", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = NULL;
  goto __pyx_L2;
  __pyx_L0:;
  __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_np_valid_x.rcbuffer->pybuffer);
  __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_valid_x.rcbuffer->pybuffer);
  __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_x.rcbuffer->pybuffer);
  __pyx_L2:;
  __Pyx_XDECREF((PyObject *)__pyx_v_np_x);
  __Pyx_XDECREF((PyObject *)__pyx_v_x);
  __Pyx_XDECREF((PyObject *)__pyx_v_valid_x);
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* Python wrapper */
static PyObject *__pyx_pw_8bayesopt_7optimize_categorical(PyObject *__pyx_self, PyObject *__pyx_args, PyObject *__pyx_kwds); /*proto*/
static PyMethodDef __pyx_mdef_8bayesopt_7optimize_categorical = {__Pyx_NAMESTR("optimize_categorical"), (PyCFunction)__pyx_pw_8bayesopt_7optimize_categorical, METH_VARARGS|METH_KEYWORDS, __Pyx_DOCSTR(0)};
static PyObject *__pyx_pw_8bayesopt_7optimize_categorical(PyObject *__pyx_self, PyObject *__pyx_args, PyObject *__pyx_kwds) {
  PyObject *__pyx_v_f = 0;
  PyArrayObject *__pyx_v_np_categories = 0;
  PyObject *__pyx_v_dparams = 0;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  PyObject *__pyx_r = 0;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("optimize_categorical (wrapper)", 0);
  {
    static PyObject **__pyx_pyargnames[] = {&__pyx_n_s__f,&__pyx_n_s__np_categories,&__pyx_n_s__dparams,0};
    PyObject* values[3] = {0,0,0};
    if (unlikely(__pyx_kwds)) {
      Py_ssize_t kw_args;
      const Py_ssize_t pos_args = PyTuple_GET_SIZE(__pyx_args);
      switch (pos_args) {
        case  3: values[2] = PyTuple_GET_ITEM(__pyx_args, 2);
        case  2: values[1] = PyTuple_GET_ITEM(__pyx_args, 1);
        case  1: values[0] = PyTuple_GET_ITEM(__pyx_args, 0);
        case  0: break;
        default: goto __pyx_L5_argtuple_error;
      }
      kw_args = PyDict_Size(__pyx_kwds);
      switch (pos_args) {
        case  0:
        if (likely((values[0] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__f)) != 0)) kw_args--;
        else goto __pyx_L5_argtuple_error;
        case  1:
        if (likely((values[1] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__np_categories)) != 0)) kw_args--;
        else {
          __Pyx_RaiseArgtupleInvalid("optimize_categorical", 1, 3, 3, 1); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 278; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
        }
        case  2:
        if (likely((values[2] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__dparams)) != 0)) kw_args--;
        else {
          __Pyx_RaiseArgtupleInvalid("optimize_categorical", 1, 3, 3, 2); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 278; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
        }
      }
      if (unlikely(kw_args > 0)) {
        if (unlikely(__Pyx_ParseOptionalKeywords(__pyx_kwds, __pyx_pyargnames, 0, values, pos_args, "optimize_categorical") < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 278; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
      }
    } else if (PyTuple_GET_SIZE(__pyx_args) != 3) {
      goto __pyx_L5_argtuple_error;
    } else {
      values[0] = PyTuple_GET_ITEM(__pyx_args, 0);
      values[1] = PyTuple_GET_ITEM(__pyx_args, 1);
      values[2] = PyTuple_GET_ITEM(__pyx_args, 2);
    }
    __pyx_v_f = values[0];
    __pyx_v_np_categories = ((PyArrayObject *)values[1]);
    __pyx_v_dparams = ((PyObject*)values[2]);
  }
  goto __pyx_L4_argument_unpacking_done;
  __pyx_L5_argtuple_error:;
  __Pyx_RaiseArgtupleInvalid("optimize_categorical", 1, 3, 3, PyTuple_GET_SIZE(__pyx_args)); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 278; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
  __pyx_L3_error:;
  __Pyx_AddTraceback("bayesopt.optimize_categorical", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __Pyx_RefNannyFinishContext();
  return NULL;
  __pyx_L4_argument_unpacking_done:;
  if (unlikely(!__Pyx_ArgTypeTest(((PyObject *)__pyx_v_np_categories), __pyx_ptype_5numpy_ndarray, 1, "np_categories", 0))) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 278; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  if (unlikely(!__Pyx_ArgTypeTest(((PyObject *)__pyx_v_dparams), (&PyDict_Type), 1, "dparams", 1))) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 279; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_r = __pyx_pf_8bayesopt_6optimize_categorical(__pyx_self, __pyx_v_f, __pyx_v_np_categories, __pyx_v_dparams);
  goto __pyx_L0;
  __pyx_L1_error:;
  __pyx_r = NULL;
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "bayesopt.pyx":278
 *     return min_value,np_x,error_code
 * 
 * def optimize_categorical(f, np.ndarray[np.int_t,ndim=1] np_categories,             # <<<<<<<<<<<<<<
 *                          dict dparams):
 * 
 */

static PyObject *__pyx_pf_8bayesopt_6optimize_categorical(CYTHON_UNUSED PyObject *__pyx_self, PyObject *__pyx_v_f, PyArrayObject *__pyx_v_np_categories, PyObject *__pyx_v_dparams) {
  bopt_params __pyx_v_params;
  npy_intp __pyx_v_nDim;
  double __pyx_v_minf[1];
  PyArrayObject *__pyx_v_np_x = 0;
  PyArrayObject *__pyx_v_x = 0;
  PyArrayObject *__pyx_v_categories = 0;
  int __pyx_v_error_code;
  double __pyx_v_min_value;
  __Pyx_LocalBuf_ND __pyx_pybuffernd_categories;
  __Pyx_Buffer __pyx_pybuffer_categories;
  __Pyx_LocalBuf_ND __pyx_pybuffernd_np_categories;
  __Pyx_Buffer __pyx_pybuffer_np_categories;
  __Pyx_LocalBuf_ND __pyx_pybuffernd_x;
  __Pyx_Buffer __pyx_pybuffer_x;
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  PyObject *__pyx_t_1 = NULL;
  PyObject *__pyx_t_2 = NULL;
  PyObject *__pyx_t_3 = NULL;
  PyObject *__pyx_t_4 = NULL;
  PyObject *__pyx_t_5 = NULL;
  PyArrayObject *__pyx_t_6 = NULL;
  int __pyx_t_7;
  PyObject *__pyx_t_8 = NULL;
  PyObject *__pyx_t_9 = NULL;
  PyObject *__pyx_t_10 = NULL;
  PyArrayObject *__pyx_t_11 = NULL;
  long __pyx_t_12;
  long __pyx_t_13;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("optimize_categorical", 0);
  __pyx_pybuffer_x.pybuffer.buf = NULL;
  __pyx_pybuffer_x.refcount = 0;
  __pyx_pybuffernd_x.data = NULL;
  __pyx_pybuffernd_x.rcbuffer = &__pyx_pybuffer_x;
  __pyx_pybuffer_categories.pybuffer.buf = NULL;
  __pyx_pybuffer_categories.refcount = 0;
  __pyx_pybuffernd_categories.data = NULL;
  __pyx_pybuffernd_categories.rcbuffer = &__pyx_pybuffer_categories;
  __pyx_pybuffer_np_categories.pybuffer.buf = NULL;
  __pyx_pybuffer_np_categories.refcount = 0;
  __pyx_pybuffernd_np_categories.data = NULL;
  __pyx_pybuffernd_np_categories.rcbuffer = &__pyx_pybuffer_np_categories;
  {
    __Pyx_BufFmt_StackElem __pyx_stack[1];
    if (unlikely(__Pyx_GetBufferAndValidate(&__pyx_pybuffernd_np_categories.rcbuffer->pybuffer, (PyObject*)__pyx_v_np_categories, &__Pyx_TypeInfo_nn___pyx_t_5numpy_int_t, PyBUF_FORMAT| PyBUF_STRIDES, 1, 0, __pyx_stack) == -1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 278; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_pybuffernd_np_categories.diminfo[0].strides = __pyx_pybuffernd_np_categories.rcbuffer->pybuffer.strides[0]; __pyx_pybuffernd_np_categories.diminfo[0].shape = __pyx_pybuffernd_np_categories.rcbuffer->pybuffer.shape[0];

  /* "bayesopt.pyx":281
 *                          dict dparams):
 * 
 *     cdef bopt_params params = dict2structparams(dparams)             # <<<<<<<<<<<<<<
 * 
 *     nDim = np_categories.shape[0]
 */
  __pyx_v_params = __pyx_f_8bayesopt_dict2structparams(__pyx_v_dparams);

  /* "bayesopt.pyx":283
 *     cdef bopt_params params = dict2structparams(dparams)
 * 
 *     nDim = np_categories.shape[0]             # <<<<<<<<<<<<<<
 * 
 *     cdef double minf[1]
 */
  __pyx_v_nDim = (__pyx_v_np_categories->dimensions[0]);

  /* "bayesopt.pyx":286
 * 
 *     cdef double minf[1]
 *     cdef np.ndarray np_x = np.zeros([nDim], dtype=np.double)             # <<<<<<<<<<<<<<
 * 
 *     cdef np.ndarray[np.double_t, ndim=1, mode="c"] x
 */
  __pyx_t_1 = __Pyx_GetModuleGlobalName(__pyx_n_s__np); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 286; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_t_2 = __Pyx_PyObject_GetAttrStr(__pyx_t_1, __pyx_n_s__zeros); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 286; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  __pyx_t_1 = __Pyx_PyInt_to_py_Py_intptr_t(__pyx_v_nDim); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 286; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_t_3 = PyList_New(1); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 286; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_3);
  PyList_SET_ITEM(__pyx_t_3, 0, __pyx_t_1);
  __Pyx_GIVEREF(__pyx_t_1);
  __pyx_t_1 = 0;
  __pyx_t_1 = PyTuple_New(1); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 286; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  PyTuple_SET_ITEM(__pyx_t_1, 0, ((PyObject *)__pyx_t_3));
  __Pyx_GIVEREF(((PyObject *)__pyx_t_3));
  __pyx_t_3 = 0;
  __pyx_t_3 = PyDict_New(); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 286; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(((PyObject *)__pyx_t_3));
  __pyx_t_4 = __Pyx_GetModuleGlobalName(__pyx_n_s__np); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 286; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_4);
  __pyx_t_5 = __Pyx_PyObject_GetAttrStr(__pyx_t_4, __pyx_n_s__double); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 286; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_5);
  __Pyx_DECREF(__pyx_t_4); __pyx_t_4 = 0;
  if (PyDict_SetItem(__pyx_t_3, ((PyObject *)__pyx_n_s__dtype), __pyx_t_5) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 286; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
  __pyx_t_5 = PyObject_Call(__pyx_t_2, ((PyObject *)__pyx_t_1), ((PyObject *)__pyx_t_3)); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 286; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_5);
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_1)); __pyx_t_1 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_3)); __pyx_t_3 = 0;
  if (!(likely(((__pyx_t_5) == Py_None) || likely(__Pyx_TypeTest(__pyx_t_5, __pyx_ptype_5numpy_ndarray))))) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 286; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_v_np_x = ((PyArrayObject *)__pyx_t_5);
  __pyx_t_5 = 0;

  /* "bayesopt.pyx":291
 *     cdef np.ndarray[np.int_t, ndim=1, mode="c"] categories
 * 
 *     x  = np.ascontiguousarray(np_x,dtype=np.double)             # <<<<<<<<<<<<<<
 *     categories = np.ascontiguousarray(np_categories,dtype=np.int)
 * 
 */
  __pyx_t_5 = __Pyx_GetModuleGlobalName(__pyx_n_s__np); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 291; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_5);
  __pyx_t_3 = __Pyx_PyObject_GetAttrStr(__pyx_t_5, __pyx_n_s__ascontiguousarray); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 291; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_3);
  __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
  __pyx_t_5 = PyTuple_New(1); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 291; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_5);
  __Pyx_INCREF(((PyObject *)__pyx_v_np_x));
  PyTuple_SET_ITEM(__pyx_t_5, 0, ((PyObject *)__pyx_v_np_x));
  __Pyx_GIVEREF(((PyObject *)__pyx_v_np_x));
  __pyx_t_1 = PyDict_New(); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 291; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(((PyObject *)__pyx_t_1));
  __pyx_t_2 = __Pyx_GetModuleGlobalName(__pyx_n_s__np); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 291; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __pyx_t_4 = __Pyx_PyObject_GetAttrStr(__pyx_t_2, __pyx_n_s__double); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 291; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_4);
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  if (PyDict_SetItem(__pyx_t_1, ((PyObject *)__pyx_n_s__dtype), __pyx_t_4) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 291; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_4); __pyx_t_4 = 0;
  __pyx_t_4 = PyObject_Call(__pyx_t_3, ((PyObject *)__pyx_t_5), ((PyObject *)__pyx_t_1)); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 291; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_4);
  __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_5)); __pyx_t_5 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_1)); __pyx_t_1 = 0;
  if (!(likely(((__pyx_t_4) == Py_None) || likely(__Pyx_TypeTest(__pyx_t_4, __pyx_ptype_5numpy_ndarray))))) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 291; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_t_6 = ((PyArrayObject *)__pyx_t_4);
  {
    __Pyx_BufFmt_StackElem __pyx_stack[1];
    __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_x.rcbuffer->pybuffer);
    __pyx_t_7 = __Pyx_GetBufferAndValidate(&__pyx_pybuffernd_x.rcbuffer->pybuffer, (PyObject*)__pyx_t_6, &__Pyx_TypeInfo_nn___pyx_t_5numpy_double_t, PyBUF_FORMAT| PyBUF_C_CONTIGUOUS, 1, 0, __pyx_stack);
    if (unlikely(__pyx_t_7 < 0)) {
      PyErr_Fetch(&__pyx_t_8, &__pyx_t_9, &__pyx_t_10);
      if (unlikely(__Pyx_GetBufferAndValidate(&__pyx_pybuffernd_x.rcbuffer->pybuffer, (PyObject*)__pyx_v_x, &__Pyx_TypeInfo_nn___pyx_t_5numpy_double_t, PyBUF_FORMAT| PyBUF_C_CONTIGUOUS, 1, 0, __pyx_stack) == -1)) {
        Py_XDECREF(__pyx_t_8); Py_XDECREF(__pyx_t_9); Py_XDECREF(__pyx_t_10);
        __Pyx_RaiseBufferFallbackError();
      } else {
        PyErr_Restore(__pyx_t_8, __pyx_t_9, __pyx_t_10);
      }
    }
    __pyx_pybuffernd_x.diminfo[0].strides = __pyx_pybuffernd_x.rcbuffer->pybuffer.strides[0]; __pyx_pybuffernd_x.diminfo[0].shape = __pyx_pybuffernd_x.rcbuffer->pybuffer.shape[0];
    if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 291; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_6 = 0;
  __pyx_v_x = ((PyArrayObject *)__pyx_t_4);
  __pyx_t_4 = 0;

  /* "bayesopt.pyx":292
 * 
 *     x  = np.ascontiguousarray(np_x,dtype=np.double)
 *     categories = np.ascontiguousarray(np_categories,dtype=np.int)             # <<<<<<<<<<<<<<
 * 
 *     Py_INCREF(f)
 */
  __pyx_t_4 = __Pyx_GetModuleGlobalName(__pyx_n_s__np); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 292; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_4);
  __pyx_t_1 = __Pyx_PyObject_GetAttrStr(__pyx_t_4, __pyx_n_s__ascontiguousarray); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 292; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __Pyx_DECREF(__pyx_t_4); __pyx_t_4 = 0;
  __pyx_t_4 = PyTuple_New(1); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 292; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_4);
  __Pyx_INCREF(((PyObject *)__pyx_v_np_categories));
  PyTuple_SET_ITEM(__pyx_t_4, 0, ((PyObject *)__pyx_v_np_categories));
  __Pyx_GIVEREF(((PyObject *)__pyx_v_np_categories));
  __pyx_t_5 = PyDict_New(); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 292; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(((PyObject *)__pyx_t_5));
  __pyx_t_3 = __Pyx_GetModuleGlobalName(__pyx_n_s__np); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 292; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_3);
  __pyx_t_2 = __Pyx_PyObject_GetAttrStr(__pyx_t_3, __pyx_n_s__int); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 292; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
  if (PyDict_SetItem(__pyx_t_5, ((PyObject *)__pyx_n_s__dtype), __pyx_t_2) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 292; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  __pyx_t_2 = PyObject_Call(__pyx_t_1, ((PyObject *)__pyx_t_4), ((PyObject *)__pyx_t_5)); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 292; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_4)); __pyx_t_4 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_5)); __pyx_t_5 = 0;
  if (!(likely(((__pyx_t_2) == Py_None) || likely(__Pyx_TypeTest(__pyx_t_2, __pyx_ptype_5numpy_ndarray))))) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 292; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_t_11 = ((PyArrayObject *)__pyx_t_2);
  {
    __Pyx_BufFmt_StackElem __pyx_stack[1];
    __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_categories.rcbuffer->pybuffer);
    __pyx_t_7 = __Pyx_GetBufferAndValidate(&__pyx_pybuffernd_categories.rcbuffer->pybuffer, (PyObject*)__pyx_t_11, &__Pyx_TypeInfo_nn___pyx_t_5numpy_int_t, PyBUF_FORMAT| PyBUF_C_CONTIGUOUS, 1, 0, __pyx_stack);
    if (unlikely(__pyx_t_7 < 0)) {
      PyErr_Fetch(&__pyx_t_10, &__pyx_t_9, &__pyx_t_8);
      if (unlikely(__Pyx_GetBufferAndValidate(&__pyx_pybuffernd_categories.rcbuffer->pybuffer, (PyObject*)__pyx_v_categories, &__Pyx_TypeInfo_nn___pyx_t_5numpy_int_t, PyBUF_FORMAT| PyBUF_C_CONTIGUOUS, 1, 0, __pyx_stack) == -1)) {
        Py_XDECREF(__pyx_t_10); Py_XDECREF(__pyx_t_9); Py_XDECREF(__pyx_t_8);
        __Pyx_RaiseBufferFallbackError();
      } else {
        PyErr_Restore(__pyx_t_10, __pyx_t_9, __pyx_t_8);
      }
    }
    __pyx_pybuffernd_categories.diminfo[0].strides = __pyx_pybuffernd_categories.rcbuffer->pybuffer.strides[0]; __pyx_pybuffernd_categories.diminfo[0].shape = __pyx_pybuffernd_categories.rcbuffer->pybuffer.shape[0];
    if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 292; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_11 = 0;
  __pyx_v_categories = ((PyArrayObject *)__pyx_t_2);
  __pyx_t_2 = 0;

  /* "bayesopt.pyx":294
 *     categories = np.ascontiguousarray(np_categories,dtype=np.int)
 * 
 *     Py_INCREF(f)             # <<<<<<<<<<<<<<
 * 
 *     error_code = bayes_optimization_categorical(nDim, callback, <void *> f,
 */
  Py_INCREF(__pyx_v_f);

  /* "bayesopt.pyx":297
 * 
 *     error_code = bayes_optimization_categorical(nDim, callback, <void *> f,
 *                                                 <int *>&categories[0], &x[0],             # <<<<<<<<<<<<<<
 *                                                 minf, params)
 * 
 */
  __pyx_t_12 = 0;
  __pyx_t_7 = -1;
  if (__pyx_t_12 < 0) {
    __pyx_t_12 += __pyx_pybuffernd_categories.diminfo[0].shape;
    if (unlikely(__pyx_t_12 < 0)) __pyx_t_7 = 0;
  } else if (unlikely(__pyx_t_12 >= __pyx_pybuffernd_categories.diminfo[0].shape)) __pyx_t_7 = 0;
  if (unlikely(__pyx_t_7 != -1)) {
    __Pyx_RaiseBufferIndexError(__pyx_t_7);
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 297; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_13 = 0;
  __pyx_t_7 = -1;
  if (__pyx_t_13 < 0) {
    __pyx_t_13 += __pyx_pybuffernd_x.diminfo[0].shape;
    if (unlikely(__pyx_t_13 < 0)) __pyx_t_7 = 0;
  } else if (unlikely(__pyx_t_13 >= __pyx_pybuffernd_x.diminfo[0].shape)) __pyx_t_7 = 0;
  if (unlikely(__pyx_t_7 != -1)) {
    __Pyx_RaiseBufferIndexError(__pyx_t_7);
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 297; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }

  /* "bayesopt.pyx":298
 *     error_code = bayes_optimization_categorical(nDim, callback, <void *> f,
 *                                                 <int *>&categories[0], &x[0],
 *                                                 minf, params)             # <<<<<<<<<<<<<<
 * 
 *     Py_DECREF(f)
 */
  __pyx_v_error_code = bayes_optimization_categorical(__pyx_v_nDim, __pyx_f_8bayesopt_callback, ((void *)__pyx_v_f), ((int *)(&(*__Pyx_BufPtrCContig1d(__pyx_t_5numpy_int_t *, __pyx_pybuffernd_categories.rcbuffer->pybuffer.buf, __pyx_t_12, __pyx_pybuffernd_categories.diminfo[0].strides)))), (&(*__Pyx_BufPtrCContig1d(__pyx_t_5numpy_double_t *, __pyx_pybuffernd_x.rcbuffer->pybuffer.buf, __pyx_t_13, __pyx_pybuffernd_x.diminfo[0].strides))), __pyx_v_minf, __pyx_v_params);

  /* "bayesopt.pyx":300
 *                                                 minf, params)
 * 
 *     Py_DECREF(f)             # <<<<<<<<<<<<<<
 * 
 *     raise_problem(error_code)
 */
  Py_DECREF(__pyx_v_f);

  /* "bayesopt.pyx":302
 *     Py_DECREF(f)
 * 
 *     raise_problem(error_code)             # <<<<<<<<<<<<<<
 * 
 *     min_value = minf[0]
 */
  __pyx_t_2 = __Pyx_GetModuleGlobalName(__pyx_n_s__raise_problem); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 302; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __pyx_t_5 = PyInt_FromLong(__pyx_v_error_code); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 302; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_5);
  __pyx_t_4 = PyTuple_New(1); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 302; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_4);
  PyTuple_SET_ITEM(__pyx_t_4, 0, __pyx_t_5);
  __Pyx_GIVEREF(__pyx_t_5);
  __pyx_t_5 = 0;
  __pyx_t_5 = PyObject_Call(__pyx_t_2, ((PyObject *)__pyx_t_4), NULL); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 302; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_5);
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_4)); __pyx_t_4 = 0;
  __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;

  /* "bayesopt.pyx":304
 *     raise_problem(error_code)
 * 
 *     min_value = minf[0]             # <<<<<<<<<<<<<<
 *     return min_value,np_x,error_code
 */
  __pyx_v_min_value = (__pyx_v_minf[0]);

  /* "bayesopt.pyx":305
 * 
 *     min_value = minf[0]
 *     return min_value,np_x,error_code             # <<<<<<<<<<<<<<
 */
  __Pyx_XDECREF(__pyx_r);
  __pyx_t_5 = PyFloat_FromDouble(__pyx_v_min_value); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 305; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_5);
  __pyx_t_4 = PyInt_FromLong(__pyx_v_error_code); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 305; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_4);
  __pyx_t_2 = PyTuple_New(3); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 305; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  PyTuple_SET_ITEM(__pyx_t_2, 0, __pyx_t_5);
  __Pyx_GIVEREF(__pyx_t_5);
  __Pyx_INCREF(((PyObject *)__pyx_v_np_x));
  PyTuple_SET_ITEM(__pyx_t_2, 1, ((PyObject *)__pyx_v_np_x));
  __Pyx_GIVEREF(((PyObject *)__pyx_v_np_x));
  PyTuple_SET_ITEM(__pyx_t_2, 2, __pyx_t_4);
  __Pyx_GIVEREF(__pyx_t_4);
  __pyx_t_5 = 0;
  __pyx_t_4 = 0;
  __pyx_r = ((PyObject *)__pyx_t_2);
  __pyx_t_2 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  __Pyx_XDECREF(__pyx_t_2);
  __Pyx_XDECREF(__pyx_t_3);
  __Pyx_XDECREF(__pyx_t_4);
  __Pyx_XDECREF(__pyx_t_5);
  { PyObject *__pyx_type, *__pyx_value, *__pyx_tb;
    __Pyx_ErrFetch(&__pyx_type, &__pyx_value, &__pyx_tb);
    __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_categories.rcbuffer->pybuffer);
    __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_np_categories.rcbuffer->pybuffer);
    __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_x.rcbuffer->pybuffer);
  __Pyx_ErrRestore(__pyx_type, __pyx_value, __pyx_tb);}
  __Pyx_AddTraceback("bayesopt.optimize_categorical", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = NULL;
  goto __pyx_L2;
  __pyx_L0:;
  __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_categories.rcbuffer->pybuffer);
  __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_np_categories.rcbuffer->pybuffer);
  __Pyx_SafeReleaseBuffer(&__pyx_pybuffernd_x.rcbuffer->pybuffer);
  __pyx_L2:;
  __Pyx_XDECREF((PyObject *)__pyx_v_np_x);
  __Pyx_XDECREF((PyObject *)__pyx_v_x);
  __Pyx_XDECREF((PyObject *)__pyx_v_categories);
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* Python wrapper */
static CYTHON_UNUSED int __pyx_pw_5numpy_7ndarray_1__getbuffer__(PyObject *__pyx_v_self, Py_buffer *__pyx_v_info, int __pyx_v_flags); /*proto*/
static CYTHON_UNUSED int __pyx_pw_5numpy_7ndarray_1__getbuffer__(PyObject *__pyx_v_self, Py_buffer *__pyx_v_info, int __pyx_v_flags) {
  int __pyx_r;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("__getbuffer__ (wrapper)", 0);
  __pyx_r = __pyx_pf_5numpy_7ndarray___getbuffer__(((PyArrayObject *)__pyx_v_self), ((Py_buffer *)__pyx_v_info), ((int)__pyx_v_flags));
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "numpy.pxd":194
 *         # experimental exception made for __getbuffer__ and __releasebuffer__
 *         # -- the details of this may change.
 *         def __getbuffer__(ndarray self, Py_buffer* info, int flags):             # <<<<<<<<<<<<<<
 *             # This implementation of getbuffer is geared towards Cython
 *             # requirements, and does not yet fullfill the PEP.
 */

static int __pyx_pf_5numpy_7ndarray___getbuffer__(PyArrayObject *__pyx_v_self, Py_buffer *__pyx_v_info, int __pyx_v_flags) {
  int __pyx_v_copy_shape;
  int __pyx_v_i;
  int __pyx_v_ndim;
  int __pyx_v_endian_detector;
  int __pyx_v_little_endian;
  int __pyx_v_t;
  char *__pyx_v_f;
  PyArray_Descr *__pyx_v_descr = 0;
  int __pyx_v_offset;
  int __pyx_v_hasfields;
  int __pyx_r;
  __Pyx_RefNannyDeclarations
  int __pyx_t_1;
  int __pyx_t_2;
  int __pyx_t_3;
  PyObject *__pyx_t_4 = NULL;
  int __pyx_t_5;
  int __pyx_t_6;
  int __pyx_t_7;
  PyObject *__pyx_t_8 = NULL;
  char *__pyx_t_9;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("__getbuffer__", 0);
  if (__pyx_v_info != NULL) {
    __pyx_v_info->obj = Py_None; __Pyx_INCREF(Py_None);
    __Pyx_GIVEREF(__pyx_v_info->obj);
  }

  /* "numpy.pxd":200
 *             # of flags
 * 
 *             if info == NULL: return             # <<<<<<<<<<<<<<
 * 
 *             cdef int copy_shape, i, ndim
 */
  __pyx_t_1 = ((__pyx_v_info == NULL) != 0);
  if (__pyx_t_1) {
    __pyx_r = 0;
    goto __pyx_L0;
    goto __pyx_L3;
  }
  __pyx_L3:;

  /* "numpy.pxd":203
 * 
 *             cdef int copy_shape, i, ndim
 *             cdef int endian_detector = 1             # <<<<<<<<<<<<<<
 *             cdef bint little_endian = ((<char*>&endian_detector)[0] != 0)
 * 
 */
  __pyx_v_endian_detector = 1;

  /* "numpy.pxd":204
 *             cdef int copy_shape, i, ndim
 *             cdef int endian_detector = 1
 *             cdef bint little_endian = ((<char*>&endian_detector)[0] != 0)             # <<<<<<<<<<<<<<
 * 
 *             ndim = PyArray_NDIM(self)
 */
  __pyx_v_little_endian = ((((char *)(&__pyx_v_endian_detector))[0]) != 0);

  /* "numpy.pxd":206
 *             cdef bint little_endian = ((<char*>&endian_detector)[0] != 0)
 * 
 *             ndim = PyArray_NDIM(self)             # <<<<<<<<<<<<<<
 * 
 *             if sizeof(npy_intp) != sizeof(Py_ssize_t):
 */
  __pyx_v_ndim = PyArray_NDIM(__pyx_v_self);

  /* "numpy.pxd":208
 *             ndim = PyArray_NDIM(self)
 * 
 *             if sizeof(npy_intp) != sizeof(Py_ssize_t):             # <<<<<<<<<<<<<<
 *                 copy_shape = 1
 *             else:
 */
  __pyx_t_1 = (((sizeof(npy_intp)) != (sizeof(Py_ssize_t))) != 0);
  if (__pyx_t_1) {

    /* "numpy.pxd":209
 * 
 *             if sizeof(npy_intp) != sizeof(Py_ssize_t):
 *                 copy_shape = 1             # <<<<<<<<<<<<<<
 *             else:
 *                 copy_shape = 0
 */
    __pyx_v_copy_shape = 1;
    goto __pyx_L4;
  }
  /*else*/ {

    /* "numpy.pxd":211
 *                 copy_shape = 1
 *             else:
 *                 copy_shape = 0             # <<<<<<<<<<<<<<
 * 
 *             if ((flags & pybuf.PyBUF_C_CONTIGUOUS == pybuf.PyBUF_C_CONTIGUOUS)
 */
    __pyx_v_copy_shape = 0;
  }
  __pyx_L4:;

  /* "numpy.pxd":213
 *                 copy_shape = 0
 * 
 *             if ((flags & pybuf.PyBUF_C_CONTIGUOUS == pybuf.PyBUF_C_CONTIGUOUS)             # <<<<<<<<<<<<<<
 *                 and not PyArray_CHKFLAGS(self, NPY_C_CONTIGUOUS)):
 *                 raise ValueError(u"ndarray is not C contiguous")
 */
  __pyx_t_1 = (((__pyx_v_flags & PyBUF_C_CONTIGUOUS) == PyBUF_C_CONTIGUOUS) != 0);
  if (__pyx_t_1) {

    /* "numpy.pxd":214
 * 
 *             if ((flags & pybuf.PyBUF_C_CONTIGUOUS == pybuf.PyBUF_C_CONTIGUOUS)
 *                 and not PyArray_CHKFLAGS(self, NPY_C_CONTIGUOUS)):             # <<<<<<<<<<<<<<
 *                 raise ValueError(u"ndarray is not C contiguous")
 * 
 */
    __pyx_t_2 = ((!(PyArray_CHKFLAGS(__pyx_v_self, NPY_C_CONTIGUOUS) != 0)) != 0);
    __pyx_t_3 = __pyx_t_2;
  } else {
    __pyx_t_3 = __pyx_t_1;
  }
  if (__pyx_t_3) {

    /* "numpy.pxd":215
 *             if ((flags & pybuf.PyBUF_C_CONTIGUOUS == pybuf.PyBUF_C_CONTIGUOUS)
 *                 and not PyArray_CHKFLAGS(self, NPY_C_CONTIGUOUS)):
 *                 raise ValueError(u"ndarray is not C contiguous")             # <<<<<<<<<<<<<<
 * 
 *             if ((flags & pybuf.PyBUF_F_CONTIGUOUS == pybuf.PyBUF_F_CONTIGUOUS)
 */
    __pyx_t_4 = PyObject_Call(__pyx_builtin_ValueError, ((PyObject *)__pyx_k_tuple_6), NULL); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 215; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_4);
    __Pyx_Raise(__pyx_t_4, 0, 0, 0);
    __Pyx_DECREF(__pyx_t_4); __pyx_t_4 = 0;
    {__pyx_filename = __pyx_f[1]; __pyx_lineno = 215; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    goto __pyx_L5;
  }
  __pyx_L5:;

  /* "numpy.pxd":217
 *                 raise ValueError(u"ndarray is not C contiguous")
 * 
 *             if ((flags & pybuf.PyBUF_F_CONTIGUOUS == pybuf.PyBUF_F_CONTIGUOUS)             # <<<<<<<<<<<<<<
 *                 and not PyArray_CHKFLAGS(self, NPY_F_CONTIGUOUS)):
 *                 raise ValueError(u"ndarray is not Fortran contiguous")
 */
  __pyx_t_3 = (((__pyx_v_flags & PyBUF_F_CONTIGUOUS) == PyBUF_F_CONTIGUOUS) != 0);
  if (__pyx_t_3) {

    /* "numpy.pxd":218
 * 
 *             if ((flags & pybuf.PyBUF_F_CONTIGUOUS == pybuf.PyBUF_F_CONTIGUOUS)
 *                 and not PyArray_CHKFLAGS(self, NPY_F_CONTIGUOUS)):             # <<<<<<<<<<<<<<
 *                 raise ValueError(u"ndarray is not Fortran contiguous")
 * 
 */
    __pyx_t_1 = ((!(PyArray_CHKFLAGS(__pyx_v_self, NPY_F_CONTIGUOUS) != 0)) != 0);
    __pyx_t_2 = __pyx_t_1;
  } else {
    __pyx_t_2 = __pyx_t_3;
  }
  if (__pyx_t_2) {

    /* "numpy.pxd":219
 *             if ((flags & pybuf.PyBUF_F_CONTIGUOUS == pybuf.PyBUF_F_CONTIGUOUS)
 *                 and not PyArray_CHKFLAGS(self, NPY_F_CONTIGUOUS)):
 *                 raise ValueError(u"ndarray is not Fortran contiguous")             # <<<<<<<<<<<<<<
 * 
 *             info.buf = PyArray_DATA(self)
 */
    __pyx_t_4 = PyObject_Call(__pyx_builtin_ValueError, ((PyObject *)__pyx_k_tuple_8), NULL); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 219; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_4);
    __Pyx_Raise(__pyx_t_4, 0, 0, 0);
    __Pyx_DECREF(__pyx_t_4); __pyx_t_4 = 0;
    {__pyx_filename = __pyx_f[1]; __pyx_lineno = 219; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    goto __pyx_L6;
  }
  __pyx_L6:;

  /* "numpy.pxd":221
 *                 raise ValueError(u"ndarray is not Fortran contiguous")
 * 
 *             info.buf = PyArray_DATA(self)             # <<<<<<<<<<<<<<
 *             info.ndim = ndim
 *             if copy_shape:
 */
  __pyx_v_info->buf = PyArray_DATA(__pyx_v_self);

  /* "numpy.pxd":222
 * 
 *             info.buf = PyArray_DATA(self)
 *             info.ndim = ndim             # <<<<<<<<<<<<<<
 *             if copy_shape:
 *                 # Allocate new buffer for strides and shape info.
 */
  __pyx_v_info->ndim = __pyx_v_ndim;

  /* "numpy.pxd":223
 *             info.buf = PyArray_DATA(self)
 *             info.ndim = ndim
 *             if copy_shape:             # <<<<<<<<<<<<<<
 *                 # Allocate new buffer for strides and shape info.
 *                 # This is allocated as one block, strides first.
 */
  __pyx_t_2 = (__pyx_v_copy_shape != 0);
  if (__pyx_t_2) {

    /* "numpy.pxd":226
 *                 # Allocate new buffer for strides and shape info.
 *                 # This is allocated as one block, strides first.
 *                 info.strides = <Py_ssize_t*>stdlib.malloc(sizeof(Py_ssize_t) * <size_t>ndim * 2)             # <<<<<<<<<<<<<<
 *                 info.shape = info.strides + ndim
 *                 for i in range(ndim):
 */
    __pyx_v_info->strides = ((Py_ssize_t *)malloc((((sizeof(Py_ssize_t)) * ((size_t)__pyx_v_ndim)) * 2)));

    /* "numpy.pxd":227
 *                 # This is allocated as one block, strides first.
 *                 info.strides = <Py_ssize_t*>stdlib.malloc(sizeof(Py_ssize_t) * <size_t>ndim * 2)
 *                 info.shape = info.strides + ndim             # <<<<<<<<<<<<<<
 *                 for i in range(ndim):
 *                     info.strides[i] = PyArray_STRIDES(self)[i]
 */
    __pyx_v_info->shape = (__pyx_v_info->strides + __pyx_v_ndim);

    /* "numpy.pxd":228
 *                 info.strides = <Py_ssize_t*>stdlib.malloc(sizeof(Py_ssize_t) * <size_t>ndim * 2)
 *                 info.shape = info.strides + ndim
 *                 for i in range(ndim):             # <<<<<<<<<<<<<<
 *                     info.strides[i] = PyArray_STRIDES(self)[i]
 *                     info.shape[i] = PyArray_DIMS(self)[i]
 */
    __pyx_t_5 = __pyx_v_ndim;
    for (__pyx_t_6 = 0; __pyx_t_6 < __pyx_t_5; __pyx_t_6+=1) {
      __pyx_v_i = __pyx_t_6;

      /* "numpy.pxd":229
 *                 info.shape = info.strides + ndim
 *                 for i in range(ndim):
 *                     info.strides[i] = PyArray_STRIDES(self)[i]             # <<<<<<<<<<<<<<
 *                     info.shape[i] = PyArray_DIMS(self)[i]
 *             else:
 */
      (__pyx_v_info->strides[__pyx_v_i]) = (PyArray_STRIDES(__pyx_v_self)[__pyx_v_i]);

      /* "numpy.pxd":230
 *                 for i in range(ndim):
 *                     info.strides[i] = PyArray_STRIDES(self)[i]
 *                     info.shape[i] = PyArray_DIMS(self)[i]             # <<<<<<<<<<<<<<
 *             else:
 *                 info.strides = <Py_ssize_t*>PyArray_STRIDES(self)
 */
      (__pyx_v_info->shape[__pyx_v_i]) = (PyArray_DIMS(__pyx_v_self)[__pyx_v_i]);
    }
    goto __pyx_L7;
  }
  /*else*/ {

    /* "numpy.pxd":232
 *                     info.shape[i] = PyArray_DIMS(self)[i]
 *             else:
 *                 info.strides = <Py_ssize_t*>PyArray_STRIDES(self)             # <<<<<<<<<<<<<<
 *                 info.shape = <Py_ssize_t*>PyArray_DIMS(self)
 *             info.suboffsets = NULL
 */
    __pyx_v_info->strides = ((Py_ssize_t *)PyArray_STRIDES(__pyx_v_self));

    /* "numpy.pxd":233
 *             else:
 *                 info.strides = <Py_ssize_t*>PyArray_STRIDES(self)
 *                 info.shape = <Py_ssize_t*>PyArray_DIMS(self)             # <<<<<<<<<<<<<<
 *             info.suboffsets = NULL
 *             info.itemsize = PyArray_ITEMSIZE(self)
 */
    __pyx_v_info->shape = ((Py_ssize_t *)PyArray_DIMS(__pyx_v_self));
  }
  __pyx_L7:;

  /* "numpy.pxd":234
 *                 info.strides = <Py_ssize_t*>PyArray_STRIDES(self)
 *                 info.shape = <Py_ssize_t*>PyArray_DIMS(self)
 *             info.suboffsets = NULL             # <<<<<<<<<<<<<<
 *             info.itemsize = PyArray_ITEMSIZE(self)
 *             info.readonly = not PyArray_ISWRITEABLE(self)
 */
  __pyx_v_info->suboffsets = NULL;

  /* "numpy.pxd":235
 *                 info.shape = <Py_ssize_t*>PyArray_DIMS(self)
 *             info.suboffsets = NULL
 *             info.itemsize = PyArray_ITEMSIZE(self)             # <<<<<<<<<<<<<<
 *             info.readonly = not PyArray_ISWRITEABLE(self)
 * 
 */
  __pyx_v_info->itemsize = PyArray_ITEMSIZE(__pyx_v_self);

  /* "numpy.pxd":236
 *             info.suboffsets = NULL
 *             info.itemsize = PyArray_ITEMSIZE(self)
 *             info.readonly = not PyArray_ISWRITEABLE(self)             # <<<<<<<<<<<<<<
 * 
 *             cdef int t
 */
  __pyx_v_info->readonly = (!(PyArray_ISWRITEABLE(__pyx_v_self) != 0));

  /* "numpy.pxd":239
 * 
 *             cdef int t
 *             cdef char* f = NULL             # <<<<<<<<<<<<<<
 *             cdef dtype descr = self.descr
 *             cdef list stack
 */
  __pyx_v_f = NULL;

  /* "numpy.pxd":240
 *             cdef int t
 *             cdef char* f = NULL
 *             cdef dtype descr = self.descr             # <<<<<<<<<<<<<<
 *             cdef list stack
 *             cdef int offset
 */
  __pyx_t_4 = ((PyObject *)__pyx_v_self->descr);
  __Pyx_INCREF(__pyx_t_4);
  __pyx_v_descr = ((PyArray_Descr *)__pyx_t_4);
  __pyx_t_4 = 0;

  /* "numpy.pxd":244
 *             cdef int offset
 * 
 *             cdef bint hasfields = PyDataType_HASFIELDS(descr)             # <<<<<<<<<<<<<<
 * 
 *             if not hasfields and not copy_shape:
 */
  __pyx_v_hasfields = PyDataType_HASFIELDS(__pyx_v_descr);

  /* "numpy.pxd":246
 *             cdef bint hasfields = PyDataType_HASFIELDS(descr)
 * 
 *             if not hasfields and not copy_shape:             # <<<<<<<<<<<<<<
 *                 # do not call releasebuffer
 *                 info.obj = None
 */
  __pyx_t_2 = ((!(__pyx_v_hasfields != 0)) != 0);
  if (__pyx_t_2) {
    __pyx_t_3 = ((!(__pyx_v_copy_shape != 0)) != 0);
    __pyx_t_1 = __pyx_t_3;
  } else {
    __pyx_t_1 = __pyx_t_2;
  }
  if (__pyx_t_1) {

    /* "numpy.pxd":248
 *             if not hasfields and not copy_shape:
 *                 # do not call releasebuffer
 *                 info.obj = None             # <<<<<<<<<<<<<<
 *             else:
 *                 # need to call releasebuffer
 */
    __Pyx_INCREF(Py_None);
    __Pyx_GIVEREF(Py_None);
    __Pyx_GOTREF(__pyx_v_info->obj);
    __Pyx_DECREF(__pyx_v_info->obj);
    __pyx_v_info->obj = Py_None;
    goto __pyx_L10;
  }
  /*else*/ {

    /* "numpy.pxd":251
 *             else:
 *                 # need to call releasebuffer
 *                 info.obj = self             # <<<<<<<<<<<<<<
 * 
 *             if not hasfields:
 */
    __Pyx_INCREF(((PyObject *)__pyx_v_self));
    __Pyx_GIVEREF(((PyObject *)__pyx_v_self));
    __Pyx_GOTREF(__pyx_v_info->obj);
    __Pyx_DECREF(__pyx_v_info->obj);
    __pyx_v_info->obj = ((PyObject *)__pyx_v_self);
  }
  __pyx_L10:;

  /* "numpy.pxd":253
 *                 info.obj = self
 * 
 *             if not hasfields:             # <<<<<<<<<<<<<<
 *                 t = descr.type_num
 *                 if ((descr.byteorder == c'>' and little_endian) or
 */
  __pyx_t_1 = ((!(__pyx_v_hasfields != 0)) != 0);
  if (__pyx_t_1) {

    /* "numpy.pxd":254
 * 
 *             if not hasfields:
 *                 t = descr.type_num             # <<<<<<<<<<<<<<
 *                 if ((descr.byteorder == c'>' and little_endian) or
 *                     (descr.byteorder == c'<' and not little_endian)):
 */
    __pyx_t_5 = __pyx_v_descr->type_num;
    __pyx_v_t = __pyx_t_5;

    /* "numpy.pxd":255
 *             if not hasfields:
 *                 t = descr.type_num
 *                 if ((descr.byteorder == c'>' and little_endian) or             # <<<<<<<<<<<<<<
 *                     (descr.byteorder == c'<' and not little_endian)):
 *                     raise ValueError(u"Non-native byte order not supported")
 */
    __pyx_t_1 = ((__pyx_v_descr->byteorder == '>') != 0);
    if (__pyx_t_1) {
      __pyx_t_2 = (__pyx_v_little_endian != 0);
    } else {
      __pyx_t_2 = __pyx_t_1;
    }
    if (!__pyx_t_2) {

      /* "numpy.pxd":256
 *                 t = descr.type_num
 *                 if ((descr.byteorder == c'>' and little_endian) or
 *                     (descr.byteorder == c'<' and not little_endian)):             # <<<<<<<<<<<<<<
 *                     raise ValueError(u"Non-native byte order not supported")
 *                 if   t == NPY_BYTE:        f = "b"
 */
      __pyx_t_1 = ((__pyx_v_descr->byteorder == '<') != 0);
      if (__pyx_t_1) {
        __pyx_t_3 = ((!(__pyx_v_little_endian != 0)) != 0);
        __pyx_t_7 = __pyx_t_3;
      } else {
        __pyx_t_7 = __pyx_t_1;
      }
      __pyx_t_1 = __pyx_t_7;
    } else {
      __pyx_t_1 = __pyx_t_2;
    }
    if (__pyx_t_1) {

      /* "numpy.pxd":257
 *                 if ((descr.byteorder == c'>' and little_endian) or
 *                     (descr.byteorder == c'<' and not little_endian)):
 *                     raise ValueError(u"Non-native byte order not supported")             # <<<<<<<<<<<<<<
 *                 if   t == NPY_BYTE:        f = "b"
 *                 elif t == NPY_UBYTE:       f = "B"
 */
      __pyx_t_4 = PyObject_Call(__pyx_builtin_ValueError, ((PyObject *)__pyx_k_tuple_10), NULL); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 257; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_4);
      __Pyx_Raise(__pyx_t_4, 0, 0, 0);
      __Pyx_DECREF(__pyx_t_4); __pyx_t_4 = 0;
      {__pyx_filename = __pyx_f[1]; __pyx_lineno = 257; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      goto __pyx_L12;
    }
    __pyx_L12:;

    /* "numpy.pxd":274
 *                 elif t == NPY_CDOUBLE:     f = "Zd"
 *                 elif t == NPY_CLONGDOUBLE: f = "Zg"
 *                 elif t == NPY_OBJECT:      f = "O"             # <<<<<<<<<<<<<<
 *                 else:
 *                     raise ValueError(u"unknown dtype code in numpy.pxd (%d)" % t)
 */
    switch (__pyx_v_t) {

      /* "numpy.pxd":258
 *                     (descr.byteorder == c'<' and not little_endian)):
 *                     raise ValueError(u"Non-native byte order not supported")
 *                 if   t == NPY_BYTE:        f = "b"             # <<<<<<<<<<<<<<
 *                 elif t == NPY_UBYTE:       f = "B"
 *                 elif t == NPY_SHORT:       f = "h"
 */
      case NPY_BYTE:
      __pyx_v_f = __pyx_k__b;
      break;

      /* "numpy.pxd":259
 *                     raise ValueError(u"Non-native byte order not supported")
 *                 if   t == NPY_BYTE:        f = "b"
 *                 elif t == NPY_UBYTE:       f = "B"             # <<<<<<<<<<<<<<
 *                 elif t == NPY_SHORT:       f = "h"
 *                 elif t == NPY_USHORT:      f = "H"
 */
      case NPY_UBYTE:
      __pyx_v_f = __pyx_k__B;
      break;

      /* "numpy.pxd":260
 *                 if   t == NPY_BYTE:        f = "b"
 *                 elif t == NPY_UBYTE:       f = "B"
 *                 elif t == NPY_SHORT:       f = "h"             # <<<<<<<<<<<<<<
 *                 elif t == NPY_USHORT:      f = "H"
 *                 elif t == NPY_INT:         f = "i"
 */
      case NPY_SHORT:
      __pyx_v_f = __pyx_k__h;
      break;

      /* "numpy.pxd":261
 *                 elif t == NPY_UBYTE:       f = "B"
 *                 elif t == NPY_SHORT:       f = "h"
 *                 elif t == NPY_USHORT:      f = "H"             # <<<<<<<<<<<<<<
 *                 elif t == NPY_INT:         f = "i"
 *                 elif t == NPY_UINT:        f = "I"
 */
      case NPY_USHORT:
      __pyx_v_f = __pyx_k__H;
      break;

      /* "numpy.pxd":262
 *                 elif t == NPY_SHORT:       f = "h"
 *                 elif t == NPY_USHORT:      f = "H"
 *                 elif t == NPY_INT:         f = "i"             # <<<<<<<<<<<<<<
 *                 elif t == NPY_UINT:        f = "I"
 *                 elif t == NPY_LONG:        f = "l"
 */
      case NPY_INT:
      __pyx_v_f = __pyx_k__i;
      break;

      /* "numpy.pxd":263
 *                 elif t == NPY_USHORT:      f = "H"
 *                 elif t == NPY_INT:         f = "i"
 *                 elif t == NPY_UINT:        f = "I"             # <<<<<<<<<<<<<<
 *                 elif t == NPY_LONG:        f = "l"
 *                 elif t == NPY_ULONG:       f = "L"
 */
      case NPY_UINT:
      __pyx_v_f = __pyx_k__I;
      break;

      /* "numpy.pxd":264
 *                 elif t == NPY_INT:         f = "i"
 *                 elif t == NPY_UINT:        f = "I"
 *                 elif t == NPY_LONG:        f = "l"             # <<<<<<<<<<<<<<
 *                 elif t == NPY_ULONG:       f = "L"
 *                 elif t == NPY_LONGLONG:    f = "q"
 */
      case NPY_LONG:
      __pyx_v_f = __pyx_k__l;
      break;

      /* "numpy.pxd":265
 *                 elif t == NPY_UINT:        f = "I"
 *                 elif t == NPY_LONG:        f = "l"
 *                 elif t == NPY_ULONG:       f = "L"             # <<<<<<<<<<<<<<
 *                 elif t == NPY_LONGLONG:    f = "q"
 *                 elif t == NPY_ULONGLONG:   f = "Q"
 */
      case NPY_ULONG:
      __pyx_v_f = __pyx_k__L;
      break;

      /* "numpy.pxd":266
 *                 elif t == NPY_LONG:        f = "l"
 *                 elif t == NPY_ULONG:       f = "L"
 *                 elif t == NPY_LONGLONG:    f = "q"             # <<<<<<<<<<<<<<
 *                 elif t == NPY_ULONGLONG:   f = "Q"
 *                 elif t == NPY_FLOAT:       f = "f"
 */
      case NPY_LONGLONG:
      __pyx_v_f = __pyx_k__q;
      break;

      /* "numpy.pxd":267
 *                 elif t == NPY_ULONG:       f = "L"
 *                 elif t == NPY_LONGLONG:    f = "q"
 *                 elif t == NPY_ULONGLONG:   f = "Q"             # <<<<<<<<<<<<<<
 *                 elif t == NPY_FLOAT:       f = "f"
 *                 elif t == NPY_DOUBLE:      f = "d"
 */
      case NPY_ULONGLONG:
      __pyx_v_f = __pyx_k__Q;
      break;

      /* "numpy.pxd":268
 *                 elif t == NPY_LONGLONG:    f = "q"
 *                 elif t == NPY_ULONGLONG:   f = "Q"
 *                 elif t == NPY_FLOAT:       f = "f"             # <<<<<<<<<<<<<<
 *                 elif t == NPY_DOUBLE:      f = "d"
 *                 elif t == NPY_LONGDOUBLE:  f = "g"
 */
      case NPY_FLOAT:
      __pyx_v_f = __pyx_k__f;
      break;

      /* "numpy.pxd":269
 *                 elif t == NPY_ULONGLONG:   f = "Q"
 *                 elif t == NPY_FLOAT:       f = "f"
 *                 elif t == NPY_DOUBLE:      f = "d"             # <<<<<<<<<<<<<<
 *                 elif t == NPY_LONGDOUBLE:  f = "g"
 *                 elif t == NPY_CFLOAT:      f = "Zf"
 */
      case NPY_DOUBLE:
      __pyx_v_f = __pyx_k__d;
      break;

      /* "numpy.pxd":270
 *                 elif t == NPY_FLOAT:       f = "f"
 *                 elif t == NPY_DOUBLE:      f = "d"
 *                 elif t == NPY_LONGDOUBLE:  f = "g"             # <<<<<<<<<<<<<<
 *                 elif t == NPY_CFLOAT:      f = "Zf"
 *                 elif t == NPY_CDOUBLE:     f = "Zd"
 */
      case NPY_LONGDOUBLE:
      __pyx_v_f = __pyx_k__g;
      break;

      /* "numpy.pxd":271
 *                 elif t == NPY_DOUBLE:      f = "d"
 *                 elif t == NPY_LONGDOUBLE:  f = "g"
 *                 elif t == NPY_CFLOAT:      f = "Zf"             # <<<<<<<<<<<<<<
 *                 elif t == NPY_CDOUBLE:     f = "Zd"
 *                 elif t == NPY_CLONGDOUBLE: f = "Zg"
 */
      case NPY_CFLOAT:
      __pyx_v_f = __pyx_k__Zf;
      break;

      /* "numpy.pxd":272
 *                 elif t == NPY_LONGDOUBLE:  f = "g"
 *                 elif t == NPY_CFLOAT:      f = "Zf"
 *                 elif t == NPY_CDOUBLE:     f = "Zd"             # <<<<<<<<<<<<<<
 *                 elif t == NPY_CLONGDOUBLE: f = "Zg"
 *                 elif t == NPY_OBJECT:      f = "O"
 */
      case NPY_CDOUBLE:
      __pyx_v_f = __pyx_k__Zd;
      break;

      /* "numpy.pxd":273
 *                 elif t == NPY_CFLOAT:      f = "Zf"
 *                 elif t == NPY_CDOUBLE:     f = "Zd"
 *                 elif t == NPY_CLONGDOUBLE: f = "Zg"             # <<<<<<<<<<<<<<
 *                 elif t == NPY_OBJECT:      f = "O"
 *                 else:
 */
      case NPY_CLONGDOUBLE:
      __pyx_v_f = __pyx_k__Zg;
      break;

      /* "numpy.pxd":274
 *                 elif t == NPY_CDOUBLE:     f = "Zd"
 *                 elif t == NPY_CLONGDOUBLE: f = "Zg"
 *                 elif t == NPY_OBJECT:      f = "O"             # <<<<<<<<<<<<<<
 *                 else:
 *                     raise ValueError(u"unknown dtype code in numpy.pxd (%d)" % t)
 */
      case NPY_OBJECT:
      __pyx_v_f = __pyx_k__O;
      break;
      default:

      /* "numpy.pxd":276
 *                 elif t == NPY_OBJECT:      f = "O"
 *                 else:
 *                     raise ValueError(u"unknown dtype code in numpy.pxd (%d)" % t)             # <<<<<<<<<<<<<<
 *                 info.format = f
 *                 return
 */
      __pyx_t_4 = PyInt_FromLong(__pyx_v_t); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 276; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_4);
      __pyx_t_8 = PyNumber_Remainder(((PyObject *)__pyx_kp_u_11), __pyx_t_4); if (unlikely(!__pyx_t_8)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 276; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(((PyObject *)__pyx_t_8));
      __Pyx_DECREF(__pyx_t_4); __pyx_t_4 = 0;
      __pyx_t_4 = PyTuple_New(1); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 276; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_4);
      PyTuple_SET_ITEM(__pyx_t_4, 0, ((PyObject *)__pyx_t_8));
      __Pyx_GIVEREF(((PyObject *)__pyx_t_8));
      __pyx_t_8 = 0;
      __pyx_t_8 = PyObject_Call(__pyx_builtin_ValueError, ((PyObject *)__pyx_t_4), NULL); if (unlikely(!__pyx_t_8)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 276; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_8);
      __Pyx_DECREF(((PyObject *)__pyx_t_4)); __pyx_t_4 = 0;
      __Pyx_Raise(__pyx_t_8, 0, 0, 0);
      __Pyx_DECREF(__pyx_t_8); __pyx_t_8 = 0;
      {__pyx_filename = __pyx_f[1]; __pyx_lineno = 276; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      break;
    }

    /* "numpy.pxd":277
 *                 else:
 *                     raise ValueError(u"unknown dtype code in numpy.pxd (%d)" % t)
 *                 info.format = f             # <<<<<<<<<<<<<<
 *                 return
 *             else:
 */
    __pyx_v_info->format = __pyx_v_f;

    /* "numpy.pxd":278
 *                     raise ValueError(u"unknown dtype code in numpy.pxd (%d)" % t)
 *                 info.format = f
 *                 return             # <<<<<<<<<<<<<<
 *             else:
 *                 info.format = <char*>stdlib.malloc(_buffer_format_string_len)
 */
    __pyx_r = 0;
    goto __pyx_L0;
    goto __pyx_L11;
  }
  /*else*/ {

    /* "numpy.pxd":280
 *                 return
 *             else:
 *                 info.format = <char*>stdlib.malloc(_buffer_format_string_len)             # <<<<<<<<<<<<<<
 *                 info.format[0] = c'^' # Native data types, manual alignment
 *                 offset = 0
 */
    __pyx_v_info->format = ((char *)malloc(255));

    /* "numpy.pxd":281
 *             else:
 *                 info.format = <char*>stdlib.malloc(_buffer_format_string_len)
 *                 info.format[0] = c'^' # Native data types, manual alignment             # <<<<<<<<<<<<<<
 *                 offset = 0
 *                 f = _util_dtypestring(descr, info.format + 1,
 */
    (__pyx_v_info->format[0]) = '^';

    /* "numpy.pxd":282
 *                 info.format = <char*>stdlib.malloc(_buffer_format_string_len)
 *                 info.format[0] = c'^' # Native data types, manual alignment
 *                 offset = 0             # <<<<<<<<<<<<<<
 *                 f = _util_dtypestring(descr, info.format + 1,
 *                                       info.format + _buffer_format_string_len,
 */
    __pyx_v_offset = 0;

    /* "numpy.pxd":285
 *                 f = _util_dtypestring(descr, info.format + 1,
 *                                       info.format + _buffer_format_string_len,
 *                                       &offset)             # <<<<<<<<<<<<<<
 *                 f[0] = c'\0' # Terminate format string
 * 
 */
    __pyx_t_9 = __pyx_f_5numpy__util_dtypestring(__pyx_v_descr, (__pyx_v_info->format + 1), (__pyx_v_info->format + 255), (&__pyx_v_offset)); if (unlikely(__pyx_t_9 == NULL)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 283; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __pyx_v_f = __pyx_t_9;

    /* "numpy.pxd":286
 *                                       info.format + _buffer_format_string_len,
 *                                       &offset)
 *                 f[0] = c'\0' # Terminate format string             # <<<<<<<<<<<<<<
 * 
 *         def __releasebuffer__(ndarray self, Py_buffer* info):
 */
    (__pyx_v_f[0]) = '\x00';
  }
  __pyx_L11:;

  __pyx_r = 0;
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_4);
  __Pyx_XDECREF(__pyx_t_8);
  __Pyx_AddTraceback("numpy.ndarray.__getbuffer__", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = -1;
  if (__pyx_v_info != NULL && __pyx_v_info->obj != NULL) {
    __Pyx_GOTREF(__pyx_v_info->obj);
    __Pyx_DECREF(__pyx_v_info->obj); __pyx_v_info->obj = NULL;
  }
  goto __pyx_L2;
  __pyx_L0:;
  if (__pyx_v_info != NULL && __pyx_v_info->obj == Py_None) {
    __Pyx_GOTREF(Py_None);
    __Pyx_DECREF(Py_None); __pyx_v_info->obj = NULL;
  }
  __pyx_L2:;
  __Pyx_XDECREF((PyObject *)__pyx_v_descr);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* Python wrapper */
static CYTHON_UNUSED void __pyx_pw_5numpy_7ndarray_3__releasebuffer__(PyObject *__pyx_v_self, Py_buffer *__pyx_v_info); /*proto*/
static CYTHON_UNUSED void __pyx_pw_5numpy_7ndarray_3__releasebuffer__(PyObject *__pyx_v_self, Py_buffer *__pyx_v_info) {
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("__releasebuffer__ (wrapper)", 0);
  __pyx_pf_5numpy_7ndarray_2__releasebuffer__(((PyArrayObject *)__pyx_v_self), ((Py_buffer *)__pyx_v_info));
  __Pyx_RefNannyFinishContext();
}

/* "numpy.pxd":288
 *                 f[0] = c'\0' # Terminate format string
 * 
 *         def __releasebuffer__(ndarray self, Py_buffer* info):             # <<<<<<<<<<<<<<
 *             if PyArray_HASFIELDS(self):
 *                 stdlib.free(info.format)
 */

static void __pyx_pf_5numpy_7ndarray_2__releasebuffer__(PyArrayObject *__pyx_v_self, Py_buffer *__pyx_v_info) {
  __Pyx_RefNannyDeclarations
  int __pyx_t_1;
  __Pyx_RefNannySetupContext("__releasebuffer__", 0);

  /* "numpy.pxd":289
 * 
 *         def __releasebuffer__(ndarray self, Py_buffer* info):
 *             if PyArray_HASFIELDS(self):             # <<<<<<<<<<<<<<
 *                 stdlib.free(info.format)
 *             if sizeof(npy_intp) != sizeof(Py_ssize_t):
 */
  __pyx_t_1 = (PyArray_HASFIELDS(__pyx_v_self) != 0);
  if (__pyx_t_1) {

    /* "numpy.pxd":290
 *         def __releasebuffer__(ndarray self, Py_buffer* info):
 *             if PyArray_HASFIELDS(self):
 *                 stdlib.free(info.format)             # <<<<<<<<<<<<<<
 *             if sizeof(npy_intp) != sizeof(Py_ssize_t):
 *                 stdlib.free(info.strides)
 */
    free(__pyx_v_info->format);
    goto __pyx_L3;
  }
  __pyx_L3:;

  /* "numpy.pxd":291
 *             if PyArray_HASFIELDS(self):
 *                 stdlib.free(info.format)
 *             if sizeof(npy_intp) != sizeof(Py_ssize_t):             # <<<<<<<<<<<<<<
 *                 stdlib.free(info.strides)
 *                 # info.shape was stored after info.strides in the same block
 */
  __pyx_t_1 = (((sizeof(npy_intp)) != (sizeof(Py_ssize_t))) != 0);
  if (__pyx_t_1) {

    /* "numpy.pxd":292
 *                 stdlib.free(info.format)
 *             if sizeof(npy_intp) != sizeof(Py_ssize_t):
 *                 stdlib.free(info.strides)             # <<<<<<<<<<<<<<
 *                 # info.shape was stored after info.strides in the same block
 * 
 */
    free(__pyx_v_info->strides);
    goto __pyx_L4;
  }
  __pyx_L4:;

  __Pyx_RefNannyFinishContext();
}

/* "numpy.pxd":768
 * ctypedef npy_cdouble     complex_t
 * 
 * cdef inline object PyArray_MultiIterNew1(a):             # <<<<<<<<<<<<<<
 *     return PyArray_MultiIterNew(1, <void*>a)
 * 
 */

static CYTHON_INLINE PyObject *__pyx_f_5numpy_PyArray_MultiIterNew1(PyObject *__pyx_v_a) {
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  PyObject *__pyx_t_1 = NULL;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("PyArray_MultiIterNew1", 0);

  /* "numpy.pxd":769
 * 
 * cdef inline object PyArray_MultiIterNew1(a):
 *     return PyArray_MultiIterNew(1, <void*>a)             # <<<<<<<<<<<<<<
 * 
 * cdef inline object PyArray_MultiIterNew2(a, b):
 */
  __Pyx_XDECREF(__pyx_r);
  __pyx_t_1 = PyArray_MultiIterNew(1, ((void *)__pyx_v_a)); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 769; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_r = __pyx_t_1;
  __pyx_t_1 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  __Pyx_AddTraceback("numpy.PyArray_MultiIterNew1", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "numpy.pxd":771
 *     return PyArray_MultiIterNew(1, <void*>a)
 * 
 * cdef inline object PyArray_MultiIterNew2(a, b):             # <<<<<<<<<<<<<<
 *     return PyArray_MultiIterNew(2, <void*>a, <void*>b)
 * 
 */

static CYTHON_INLINE PyObject *__pyx_f_5numpy_PyArray_MultiIterNew2(PyObject *__pyx_v_a, PyObject *__pyx_v_b) {
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  PyObject *__pyx_t_1 = NULL;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("PyArray_MultiIterNew2", 0);

  /* "numpy.pxd":772
 * 
 * cdef inline object PyArray_MultiIterNew2(a, b):
 *     return PyArray_MultiIterNew(2, <void*>a, <void*>b)             # <<<<<<<<<<<<<<
 * 
 * cdef inline object PyArray_MultiIterNew3(a, b, c):
 */
  __Pyx_XDECREF(__pyx_r);
  __pyx_t_1 = PyArray_MultiIterNew(2, ((void *)__pyx_v_a), ((void *)__pyx_v_b)); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 772; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_r = __pyx_t_1;
  __pyx_t_1 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  __Pyx_AddTraceback("numpy.PyArray_MultiIterNew2", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "numpy.pxd":774
 *     return PyArray_MultiIterNew(2, <void*>a, <void*>b)
 * 
 * cdef inline object PyArray_MultiIterNew3(a, b, c):             # <<<<<<<<<<<<<<
 *     return PyArray_MultiIterNew(3, <void*>a, <void*>b, <void*> c)
 * 
 */

static CYTHON_INLINE PyObject *__pyx_f_5numpy_PyArray_MultiIterNew3(PyObject *__pyx_v_a, PyObject *__pyx_v_b, PyObject *__pyx_v_c) {
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  PyObject *__pyx_t_1 = NULL;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("PyArray_MultiIterNew3", 0);

  /* "numpy.pxd":775
 * 
 * cdef inline object PyArray_MultiIterNew3(a, b, c):
 *     return PyArray_MultiIterNew(3, <void*>a, <void*>b, <void*> c)             # <<<<<<<<<<<<<<
 * 
 * cdef inline object PyArray_MultiIterNew4(a, b, c, d):
 */
  __Pyx_XDECREF(__pyx_r);
  __pyx_t_1 = PyArray_MultiIterNew(3, ((void *)__pyx_v_a), ((void *)__pyx_v_b), ((void *)__pyx_v_c)); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 775; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_r = __pyx_t_1;
  __pyx_t_1 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  __Pyx_AddTraceback("numpy.PyArray_MultiIterNew3", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "numpy.pxd":777
 *     return PyArray_MultiIterNew(3, <void*>a, <void*>b, <void*> c)
 * 
 * cdef inline object PyArray_MultiIterNew4(a, b, c, d):             # <<<<<<<<<<<<<<
 *     return PyArray_MultiIterNew(4, <void*>a, <void*>b, <void*>c, <void*> d)
 * 
 */

static CYTHON_INLINE PyObject *__pyx_f_5numpy_PyArray_MultiIterNew4(PyObject *__pyx_v_a, PyObject *__pyx_v_b, PyObject *__pyx_v_c, PyObject *__pyx_v_d) {
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  PyObject *__pyx_t_1 = NULL;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("PyArray_MultiIterNew4", 0);

  /* "numpy.pxd":778
 * 
 * cdef inline object PyArray_MultiIterNew4(a, b, c, d):
 *     return PyArray_MultiIterNew(4, <void*>a, <void*>b, <void*>c, <void*> d)             # <<<<<<<<<<<<<<
 * 
 * cdef inline object PyArray_MultiIterNew5(a, b, c, d, e):
 */
  __Pyx_XDECREF(__pyx_r);
  __pyx_t_1 = PyArray_MultiIterNew(4, ((void *)__pyx_v_a), ((void *)__pyx_v_b), ((void *)__pyx_v_c), ((void *)__pyx_v_d)); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 778; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_r = __pyx_t_1;
  __pyx_t_1 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  __Pyx_AddTraceback("numpy.PyArray_MultiIterNew4", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "numpy.pxd":780
 *     return PyArray_MultiIterNew(4, <void*>a, <void*>b, <void*>c, <void*> d)
 * 
 * cdef inline object PyArray_MultiIterNew5(a, b, c, d, e):             # <<<<<<<<<<<<<<
 *     return PyArray_MultiIterNew(5, <void*>a, <void*>b, <void*>c, <void*> d, <void*> e)
 * 
 */

static CYTHON_INLINE PyObject *__pyx_f_5numpy_PyArray_MultiIterNew5(PyObject *__pyx_v_a, PyObject *__pyx_v_b, PyObject *__pyx_v_c, PyObject *__pyx_v_d, PyObject *__pyx_v_e) {
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  PyObject *__pyx_t_1 = NULL;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("PyArray_MultiIterNew5", 0);

  /* "numpy.pxd":781
 * 
 * cdef inline object PyArray_MultiIterNew5(a, b, c, d, e):
 *     return PyArray_MultiIterNew(5, <void*>a, <void*>b, <void*>c, <void*> d, <void*> e)             # <<<<<<<<<<<<<<
 * 
 * cdef inline char* _util_dtypestring(dtype descr, char* f, char* end, int* offset) except NULL:
 */
  __Pyx_XDECREF(__pyx_r);
  __pyx_t_1 = PyArray_MultiIterNew(5, ((void *)__pyx_v_a), ((void *)__pyx_v_b), ((void *)__pyx_v_c), ((void *)__pyx_v_d), ((void *)__pyx_v_e)); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 781; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_r = __pyx_t_1;
  __pyx_t_1 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  __Pyx_AddTraceback("numpy.PyArray_MultiIterNew5", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "numpy.pxd":783
 *     return PyArray_MultiIterNew(5, <void*>a, <void*>b, <void*>c, <void*> d, <void*> e)
 * 
 * cdef inline char* _util_dtypestring(dtype descr, char* f, char* end, int* offset) except NULL:             # <<<<<<<<<<<<<<
 *     # Recursive utility function used in __getbuffer__ to get format
 *     # string. The new location in the format string is returned.
 */

static CYTHON_INLINE char *__pyx_f_5numpy__util_dtypestring(PyArray_Descr *__pyx_v_descr, char *__pyx_v_f, char *__pyx_v_end, int *__pyx_v_offset) {
  PyArray_Descr *__pyx_v_child = 0;
  int __pyx_v_endian_detector;
  int __pyx_v_little_endian;
  PyObject *__pyx_v_fields = 0;
  PyObject *__pyx_v_childname = NULL;
  PyObject *__pyx_v_new_offset = NULL;
  PyObject *__pyx_v_t = NULL;
  char *__pyx_r;
  __Pyx_RefNannyDeclarations
  PyObject *__pyx_t_1 = NULL;
  Py_ssize_t __pyx_t_2;
  PyObject *__pyx_t_3 = NULL;
  PyObject *__pyx_t_4 = NULL;
  PyObject *__pyx_t_5 = NULL;
  PyObject *(*__pyx_t_6)(PyObject *);
  int __pyx_t_7;
  int __pyx_t_8;
  int __pyx_t_9;
  int __pyx_t_10;
  long __pyx_t_11;
  char *__pyx_t_12;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("_util_dtypestring", 0);

  /* "numpy.pxd":790
 *     cdef int delta_offset
 *     cdef tuple i
 *     cdef int endian_detector = 1             # <<<<<<<<<<<<<<
 *     cdef bint little_endian = ((<char*>&endian_detector)[0] != 0)
 *     cdef tuple fields
 */
  __pyx_v_endian_detector = 1;

  /* "numpy.pxd":791
 *     cdef tuple i
 *     cdef int endian_detector = 1
 *     cdef bint little_endian = ((<char*>&endian_detector)[0] != 0)             # <<<<<<<<<<<<<<
 *     cdef tuple fields
 * 
 */
  __pyx_v_little_endian = ((((char *)(&__pyx_v_endian_detector))[0]) != 0);

  /* "numpy.pxd":794
 *     cdef tuple fields
 * 
 *     for childname in descr.names:             # <<<<<<<<<<<<<<
 *         fields = descr.fields[childname]
 *         child, new_offset = fields
 */
  if (unlikely(((PyObject *)__pyx_v_descr->names) == Py_None)) {
    PyErr_SetString(PyExc_TypeError, "'NoneType' object is not iterable");
    {__pyx_filename = __pyx_f[1]; __pyx_lineno = 794; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  }
  __pyx_t_1 = ((PyObject *)__pyx_v_descr->names); __Pyx_INCREF(__pyx_t_1); __pyx_t_2 = 0;
  for (;;) {
    if (__pyx_t_2 >= PyTuple_GET_SIZE(__pyx_t_1)) break;
    #if CYTHON_COMPILING_IN_CPYTHON
    __pyx_t_3 = PyTuple_GET_ITEM(__pyx_t_1, __pyx_t_2); __Pyx_INCREF(__pyx_t_3); __pyx_t_2++; if (unlikely(0 < 0)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 794; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    #else
    __pyx_t_3 = PySequence_ITEM(__pyx_t_1, __pyx_t_2); __pyx_t_2++; if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 794; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    #endif
    __Pyx_XDECREF(__pyx_v_childname);
    __pyx_v_childname = __pyx_t_3;
    __pyx_t_3 = 0;

    /* "numpy.pxd":795
 * 
 *     for childname in descr.names:
 *         fields = descr.fields[childname]             # <<<<<<<<<<<<<<
 *         child, new_offset = fields
 * 
 */
    __pyx_t_3 = PyObject_GetItem(__pyx_v_descr->fields, __pyx_v_childname); if (!__pyx_t_3) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 795; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_3);
    if (!(likely(PyTuple_CheckExact(__pyx_t_3))||((__pyx_t_3) == Py_None)||(PyErr_Format(PyExc_TypeError, "Expected tuple, got %.200s", Py_TYPE(__pyx_t_3)->tp_name), 0))) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 795; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_XDECREF(((PyObject *)__pyx_v_fields));
    __pyx_v_fields = ((PyObject*)__pyx_t_3);
    __pyx_t_3 = 0;

    /* "numpy.pxd":796
 *     for childname in descr.names:
 *         fields = descr.fields[childname]
 *         child, new_offset = fields             # <<<<<<<<<<<<<<
 * 
 *         if (end - f) - (new_offset - offset[0]) < 15:
 */
    if (likely(PyTuple_CheckExact(((PyObject *)__pyx_v_fields)))) {
      PyObject* sequence = ((PyObject *)__pyx_v_fields);
      #if CYTHON_COMPILING_IN_CPYTHON
      Py_ssize_t size = Py_SIZE(sequence);
      #else
      Py_ssize_t size = PySequence_Size(sequence);
      #endif
      if (unlikely(size != 2)) {
        if (size > 2) __Pyx_RaiseTooManyValuesError(2);
        else if (size >= 0) __Pyx_RaiseNeedMoreValuesError(size);
        {__pyx_filename = __pyx_f[1]; __pyx_lineno = 796; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      }
      #if CYTHON_COMPILING_IN_CPYTHON
      __pyx_t_3 = PyTuple_GET_ITEM(sequence, 0); 
      __pyx_t_4 = PyTuple_GET_ITEM(sequence, 1); 
      __Pyx_INCREF(__pyx_t_3);
      __Pyx_INCREF(__pyx_t_4);
      #else
      __pyx_t_3 = PySequence_ITEM(sequence, 0); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 796; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_3);
      __pyx_t_4 = PySequence_ITEM(sequence, 1); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 796; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_4);
      #endif
    } else if (1) {
      __Pyx_RaiseNoneNotIterableError(); {__pyx_filename = __pyx_f[1]; __pyx_lineno = 796; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    } else
    {
      Py_ssize_t index = -1;
      __pyx_t_5 = PyObject_GetIter(((PyObject *)__pyx_v_fields)); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 796; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_5);
      __pyx_t_6 = Py_TYPE(__pyx_t_5)->tp_iternext;
      index = 0; __pyx_t_3 = __pyx_t_6(__pyx_t_5); if (unlikely(!__pyx_t_3)) goto __pyx_L5_unpacking_failed;
      __Pyx_GOTREF(__pyx_t_3);
      index = 1; __pyx_t_4 = __pyx_t_6(__pyx_t_5); if (unlikely(!__pyx_t_4)) goto __pyx_L5_unpacking_failed;
      __Pyx_GOTREF(__pyx_t_4);
      if (__Pyx_IternextUnpackEndCheck(__pyx_t_6(__pyx_t_5), 2) < 0) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 796; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __pyx_t_6 = NULL;
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      goto __pyx_L6_unpacking_done;
      __pyx_L5_unpacking_failed:;
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      __pyx_t_6 = NULL;
      if (__Pyx_IterFinish() == 0) __Pyx_RaiseNeedMoreValuesError(index);
      {__pyx_filename = __pyx_f[1]; __pyx_lineno = 796; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __pyx_L6_unpacking_done:;
    }
    if (!(likely(((__pyx_t_3) == Py_None) || likely(__Pyx_TypeTest(__pyx_t_3, __pyx_ptype_5numpy_dtype))))) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 796; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_XDECREF(((PyObject *)__pyx_v_child));
    __pyx_v_child = ((PyArray_Descr *)__pyx_t_3);
    __pyx_t_3 = 0;
    __Pyx_XDECREF(__pyx_v_new_offset);
    __pyx_v_new_offset = __pyx_t_4;
    __pyx_t_4 = 0;

    /* "numpy.pxd":798
 *         child, new_offset = fields
 * 
 *         if (end - f) - (new_offset - offset[0]) < 15:             # <<<<<<<<<<<<<<
 *             raise RuntimeError(u"Format string allocated too short, see comment in numpy.pxd")
 * 
 */
    __pyx_t_4 = PyInt_FromLong((__pyx_v_end - __pyx_v_f)); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 798; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_4);
    __pyx_t_3 = PyInt_FromLong((__pyx_v_offset[0])); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 798; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_3);
    __pyx_t_5 = PyNumber_Subtract(__pyx_v_new_offset, __pyx_t_3); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 798; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_5);
    __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
    __pyx_t_3 = PyNumber_Subtract(__pyx_t_4, __pyx_t_5); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 798; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_3);
    __Pyx_DECREF(__pyx_t_4); __pyx_t_4 = 0;
    __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
    __pyx_t_5 = PyObject_RichCompare(__pyx_t_3, __pyx_int_15, Py_LT); __Pyx_XGOTREF(__pyx_t_5); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 798; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
    __pyx_t_7 = __Pyx_PyObject_IsTrue(__pyx_t_5); if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 798; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
    if (__pyx_t_7) {

      /* "numpy.pxd":799
 * 
 *         if (end - f) - (new_offset - offset[0]) < 15:
 *             raise RuntimeError(u"Format string allocated too short, see comment in numpy.pxd")             # <<<<<<<<<<<<<<
 * 
 *         if ((child.byteorder == c'>' and little_endian) or
 */
      __pyx_t_5 = PyObject_Call(__pyx_builtin_RuntimeError, ((PyObject *)__pyx_k_tuple_13), NULL); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 799; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_5);
      __Pyx_Raise(__pyx_t_5, 0, 0, 0);
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      {__pyx_filename = __pyx_f[1]; __pyx_lineno = 799; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      goto __pyx_L7;
    }
    __pyx_L7:;

    /* "numpy.pxd":801
 *             raise RuntimeError(u"Format string allocated too short, see comment in numpy.pxd")
 * 
 *         if ((child.byteorder == c'>' and little_endian) or             # <<<<<<<<<<<<<<
 *             (child.byteorder == c'<' and not little_endian)):
 *             raise ValueError(u"Non-native byte order not supported")
 */
    __pyx_t_7 = ((__pyx_v_child->byteorder == '>') != 0);
    if (__pyx_t_7) {
      __pyx_t_8 = (__pyx_v_little_endian != 0);
    } else {
      __pyx_t_8 = __pyx_t_7;
    }
    if (!__pyx_t_8) {

      /* "numpy.pxd":802
 * 
 *         if ((child.byteorder == c'>' and little_endian) or
 *             (child.byteorder == c'<' and not little_endian)):             # <<<<<<<<<<<<<<
 *             raise ValueError(u"Non-native byte order not supported")
 *             # One could encode it in the format string and have Cython
 */
      __pyx_t_7 = ((__pyx_v_child->byteorder == '<') != 0);
      if (__pyx_t_7) {
        __pyx_t_9 = ((!(__pyx_v_little_endian != 0)) != 0);
        __pyx_t_10 = __pyx_t_9;
      } else {
        __pyx_t_10 = __pyx_t_7;
      }
      __pyx_t_7 = __pyx_t_10;
    } else {
      __pyx_t_7 = __pyx_t_8;
    }
    if (__pyx_t_7) {

      /* "numpy.pxd":803
 *         if ((child.byteorder == c'>' and little_endian) or
 *             (child.byteorder == c'<' and not little_endian)):
 *             raise ValueError(u"Non-native byte order not supported")             # <<<<<<<<<<<<<<
 *             # One could encode it in the format string and have Cython
 *             # complain instead, BUT: < and > in format strings also imply
 */
      __pyx_t_5 = PyObject_Call(__pyx_builtin_ValueError, ((PyObject *)__pyx_k_tuple_14), NULL); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 803; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_5);
      __Pyx_Raise(__pyx_t_5, 0, 0, 0);
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      {__pyx_filename = __pyx_f[1]; __pyx_lineno = 803; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      goto __pyx_L8;
    }
    __pyx_L8:;

    /* "numpy.pxd":813
 * 
 *         # Output padding bytes
 *         while offset[0] < new_offset:             # <<<<<<<<<<<<<<
 *             f[0] = 120 # "x"; pad byte
 *             f += 1
 */
    while (1) {
      __pyx_t_5 = PyInt_FromLong((__pyx_v_offset[0])); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 813; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_5);
      __pyx_t_3 = PyObject_RichCompare(__pyx_t_5, __pyx_v_new_offset, Py_LT); __Pyx_XGOTREF(__pyx_t_3); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 813; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      __pyx_t_7 = __Pyx_PyObject_IsTrue(__pyx_t_3); if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 813; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
      if (!__pyx_t_7) break;

      /* "numpy.pxd":814
 *         # Output padding bytes
 *         while offset[0] < new_offset:
 *             f[0] = 120 # "x"; pad byte             # <<<<<<<<<<<<<<
 *             f += 1
 *             offset[0] += 1
 */
      (__pyx_v_f[0]) = 120;

      /* "numpy.pxd":815
 *         while offset[0] < new_offset:
 *             f[0] = 120 # "x"; pad byte
 *             f += 1             # <<<<<<<<<<<<<<
 *             offset[0] += 1
 * 
 */
      __pyx_v_f = (__pyx_v_f + 1);

      /* "numpy.pxd":816
 *             f[0] = 120 # "x"; pad byte
 *             f += 1
 *             offset[0] += 1             # <<<<<<<<<<<<<<
 * 
 *         offset[0] += child.itemsize
 */
      __pyx_t_11 = 0;
      (__pyx_v_offset[__pyx_t_11]) = ((__pyx_v_offset[__pyx_t_11]) + 1);
    }

    /* "numpy.pxd":818
 *             offset[0] += 1
 * 
 *         offset[0] += child.itemsize             # <<<<<<<<<<<<<<
 * 
 *         if not PyDataType_HASFIELDS(child):
 */
    __pyx_t_11 = 0;
    (__pyx_v_offset[__pyx_t_11]) = ((__pyx_v_offset[__pyx_t_11]) + __pyx_v_child->elsize);

    /* "numpy.pxd":820
 *         offset[0] += child.itemsize
 * 
 *         if not PyDataType_HASFIELDS(child):             # <<<<<<<<<<<<<<
 *             t = child.type_num
 *             if end - f < 5:
 */
    __pyx_t_7 = ((!(PyDataType_HASFIELDS(__pyx_v_child) != 0)) != 0);
    if (__pyx_t_7) {

      /* "numpy.pxd":821
 * 
 *         if not PyDataType_HASFIELDS(child):
 *             t = child.type_num             # <<<<<<<<<<<<<<
 *             if end - f < 5:
 *                 raise RuntimeError(u"Format string allocated too short.")
 */
      __pyx_t_3 = PyInt_FromLong(__pyx_v_child->type_num); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 821; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_3);
      __Pyx_XDECREF(__pyx_v_t);
      __pyx_v_t = __pyx_t_3;
      __pyx_t_3 = 0;

      /* "numpy.pxd":822
 *         if not PyDataType_HASFIELDS(child):
 *             t = child.type_num
 *             if end - f < 5:             # <<<<<<<<<<<<<<
 *                 raise RuntimeError(u"Format string allocated too short.")
 * 
 */
      __pyx_t_7 = (((__pyx_v_end - __pyx_v_f) < 5) != 0);
      if (__pyx_t_7) {

        /* "numpy.pxd":823
 *             t = child.type_num
 *             if end - f < 5:
 *                 raise RuntimeError(u"Format string allocated too short.")             # <<<<<<<<<<<<<<
 * 
 *             # Until ticket #99 is fixed, use integers to avoid warnings
 */
        __pyx_t_3 = PyObject_Call(__pyx_builtin_RuntimeError, ((PyObject *)__pyx_k_tuple_16), NULL); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 823; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
        __Pyx_GOTREF(__pyx_t_3);
        __Pyx_Raise(__pyx_t_3, 0, 0, 0);
        __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
        {__pyx_filename = __pyx_f[1]; __pyx_lineno = 823; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
        goto __pyx_L12;
      }
      __pyx_L12:;

      /* "numpy.pxd":826
 * 
 *             # Until ticket #99 is fixed, use integers to avoid warnings
 *             if   t == NPY_BYTE:        f[0] =  98 #"b"             # <<<<<<<<<<<<<<
 *             elif t == NPY_UBYTE:       f[0] =  66 #"B"
 *             elif t == NPY_SHORT:       f[0] = 104 #"h"
 */
      __pyx_t_3 = PyInt_FromLong(NPY_BYTE); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 826; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_3);
      __pyx_t_5 = PyObject_RichCompare(__pyx_v_t, __pyx_t_3, Py_EQ); __Pyx_XGOTREF(__pyx_t_5); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 826; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
      __pyx_t_7 = __Pyx_PyObject_IsTrue(__pyx_t_5); if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 826; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      if (__pyx_t_7) {
        (__pyx_v_f[0]) = 98;
        goto __pyx_L13;
      }

      /* "numpy.pxd":827
 *             # Until ticket #99 is fixed, use integers to avoid warnings
 *             if   t == NPY_BYTE:        f[0] =  98 #"b"
 *             elif t == NPY_UBYTE:       f[0] =  66 #"B"             # <<<<<<<<<<<<<<
 *             elif t == NPY_SHORT:       f[0] = 104 #"h"
 *             elif t == NPY_USHORT:      f[0] =  72 #"H"
 */
      __pyx_t_5 = PyInt_FromLong(NPY_UBYTE); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 827; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_5);
      __pyx_t_3 = PyObject_RichCompare(__pyx_v_t, __pyx_t_5, Py_EQ); __Pyx_XGOTREF(__pyx_t_3); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 827; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      __pyx_t_7 = __Pyx_PyObject_IsTrue(__pyx_t_3); if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 827; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
      if (__pyx_t_7) {
        (__pyx_v_f[0]) = 66;
        goto __pyx_L13;
      }

      /* "numpy.pxd":828
 *             if   t == NPY_BYTE:        f[0] =  98 #"b"
 *             elif t == NPY_UBYTE:       f[0] =  66 #"B"
 *             elif t == NPY_SHORT:       f[0] = 104 #"h"             # <<<<<<<<<<<<<<
 *             elif t == NPY_USHORT:      f[0] =  72 #"H"
 *             elif t == NPY_INT:         f[0] = 105 #"i"
 */
      __pyx_t_3 = PyInt_FromLong(NPY_SHORT); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 828; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_3);
      __pyx_t_5 = PyObject_RichCompare(__pyx_v_t, __pyx_t_3, Py_EQ); __Pyx_XGOTREF(__pyx_t_5); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 828; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
      __pyx_t_7 = __Pyx_PyObject_IsTrue(__pyx_t_5); if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 828; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      if (__pyx_t_7) {
        (__pyx_v_f[0]) = 104;
        goto __pyx_L13;
      }

      /* "numpy.pxd":829
 *             elif t == NPY_UBYTE:       f[0] =  66 #"B"
 *             elif t == NPY_SHORT:       f[0] = 104 #"h"
 *             elif t == NPY_USHORT:      f[0] =  72 #"H"             # <<<<<<<<<<<<<<
 *             elif t == NPY_INT:         f[0] = 105 #"i"
 *             elif t == NPY_UINT:        f[0] =  73 #"I"
 */
      __pyx_t_5 = PyInt_FromLong(NPY_USHORT); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 829; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_5);
      __pyx_t_3 = PyObject_RichCompare(__pyx_v_t, __pyx_t_5, Py_EQ); __Pyx_XGOTREF(__pyx_t_3); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 829; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      __pyx_t_7 = __Pyx_PyObject_IsTrue(__pyx_t_3); if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 829; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
      if (__pyx_t_7) {
        (__pyx_v_f[0]) = 72;
        goto __pyx_L13;
      }

      /* "numpy.pxd":830
 *             elif t == NPY_SHORT:       f[0] = 104 #"h"
 *             elif t == NPY_USHORT:      f[0] =  72 #"H"
 *             elif t == NPY_INT:         f[0] = 105 #"i"             # <<<<<<<<<<<<<<
 *             elif t == NPY_UINT:        f[0] =  73 #"I"
 *             elif t == NPY_LONG:        f[0] = 108 #"l"
 */
      __pyx_t_3 = PyInt_FromLong(NPY_INT); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 830; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_3);
      __pyx_t_5 = PyObject_RichCompare(__pyx_v_t, __pyx_t_3, Py_EQ); __Pyx_XGOTREF(__pyx_t_5); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 830; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
      __pyx_t_7 = __Pyx_PyObject_IsTrue(__pyx_t_5); if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 830; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      if (__pyx_t_7) {
        (__pyx_v_f[0]) = 105;
        goto __pyx_L13;
      }

      /* "numpy.pxd":831
 *             elif t == NPY_USHORT:      f[0] =  72 #"H"
 *             elif t == NPY_INT:         f[0] = 105 #"i"
 *             elif t == NPY_UINT:        f[0] =  73 #"I"             # <<<<<<<<<<<<<<
 *             elif t == NPY_LONG:        f[0] = 108 #"l"
 *             elif t == NPY_ULONG:       f[0] = 76  #"L"
 */
      __pyx_t_5 = PyInt_FromLong(NPY_UINT); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 831; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_5);
      __pyx_t_3 = PyObject_RichCompare(__pyx_v_t, __pyx_t_5, Py_EQ); __Pyx_XGOTREF(__pyx_t_3); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 831; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      __pyx_t_7 = __Pyx_PyObject_IsTrue(__pyx_t_3); if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 831; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
      if (__pyx_t_7) {
        (__pyx_v_f[0]) = 73;
        goto __pyx_L13;
      }

      /* "numpy.pxd":832
 *             elif t == NPY_INT:         f[0] = 105 #"i"
 *             elif t == NPY_UINT:        f[0] =  73 #"I"
 *             elif t == NPY_LONG:        f[0] = 108 #"l"             # <<<<<<<<<<<<<<
 *             elif t == NPY_ULONG:       f[0] = 76  #"L"
 *             elif t == NPY_LONGLONG:    f[0] = 113 #"q"
 */
      __pyx_t_3 = PyInt_FromLong(NPY_LONG); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 832; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_3);
      __pyx_t_5 = PyObject_RichCompare(__pyx_v_t, __pyx_t_3, Py_EQ); __Pyx_XGOTREF(__pyx_t_5); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 832; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
      __pyx_t_7 = __Pyx_PyObject_IsTrue(__pyx_t_5); if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 832; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      if (__pyx_t_7) {
        (__pyx_v_f[0]) = 108;
        goto __pyx_L13;
      }

      /* "numpy.pxd":833
 *             elif t == NPY_UINT:        f[0] =  73 #"I"
 *             elif t == NPY_LONG:        f[0] = 108 #"l"
 *             elif t == NPY_ULONG:       f[0] = 76  #"L"             # <<<<<<<<<<<<<<
 *             elif t == NPY_LONGLONG:    f[0] = 113 #"q"
 *             elif t == NPY_ULONGLONG:   f[0] = 81  #"Q"
 */
      __pyx_t_5 = PyInt_FromLong(NPY_ULONG); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 833; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_5);
      __pyx_t_3 = PyObject_RichCompare(__pyx_v_t, __pyx_t_5, Py_EQ); __Pyx_XGOTREF(__pyx_t_3); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 833; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      __pyx_t_7 = __Pyx_PyObject_IsTrue(__pyx_t_3); if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 833; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
      if (__pyx_t_7) {
        (__pyx_v_f[0]) = 76;
        goto __pyx_L13;
      }

      /* "numpy.pxd":834
 *             elif t == NPY_LONG:        f[0] = 108 #"l"
 *             elif t == NPY_ULONG:       f[0] = 76  #"L"
 *             elif t == NPY_LONGLONG:    f[0] = 113 #"q"             # <<<<<<<<<<<<<<
 *             elif t == NPY_ULONGLONG:   f[0] = 81  #"Q"
 *             elif t == NPY_FLOAT:       f[0] = 102 #"f"
 */
      __pyx_t_3 = PyInt_FromLong(NPY_LONGLONG); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 834; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_3);
      __pyx_t_5 = PyObject_RichCompare(__pyx_v_t, __pyx_t_3, Py_EQ); __Pyx_XGOTREF(__pyx_t_5); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 834; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
      __pyx_t_7 = __Pyx_PyObject_IsTrue(__pyx_t_5); if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 834; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      if (__pyx_t_7) {
        (__pyx_v_f[0]) = 113;
        goto __pyx_L13;
      }

      /* "numpy.pxd":835
 *             elif t == NPY_ULONG:       f[0] = 76  #"L"
 *             elif t == NPY_LONGLONG:    f[0] = 113 #"q"
 *             elif t == NPY_ULONGLONG:   f[0] = 81  #"Q"             # <<<<<<<<<<<<<<
 *             elif t == NPY_FLOAT:       f[0] = 102 #"f"
 *             elif t == NPY_DOUBLE:      f[0] = 100 #"d"
 */
      __pyx_t_5 = PyInt_FromLong(NPY_ULONGLONG); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 835; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_5);
      __pyx_t_3 = PyObject_RichCompare(__pyx_v_t, __pyx_t_5, Py_EQ); __Pyx_XGOTREF(__pyx_t_3); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 835; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      __pyx_t_7 = __Pyx_PyObject_IsTrue(__pyx_t_3); if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 835; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
      if (__pyx_t_7) {
        (__pyx_v_f[0]) = 81;
        goto __pyx_L13;
      }

      /* "numpy.pxd":836
 *             elif t == NPY_LONGLONG:    f[0] = 113 #"q"
 *             elif t == NPY_ULONGLONG:   f[0] = 81  #"Q"
 *             elif t == NPY_FLOAT:       f[0] = 102 #"f"             # <<<<<<<<<<<<<<
 *             elif t == NPY_DOUBLE:      f[0] = 100 #"d"
 *             elif t == NPY_LONGDOUBLE:  f[0] = 103 #"g"
 */
      __pyx_t_3 = PyInt_FromLong(NPY_FLOAT); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 836; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_3);
      __pyx_t_5 = PyObject_RichCompare(__pyx_v_t, __pyx_t_3, Py_EQ); __Pyx_XGOTREF(__pyx_t_5); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 836; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
      __pyx_t_7 = __Pyx_PyObject_IsTrue(__pyx_t_5); if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 836; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      if (__pyx_t_7) {
        (__pyx_v_f[0]) = 102;
        goto __pyx_L13;
      }

      /* "numpy.pxd":837
 *             elif t == NPY_ULONGLONG:   f[0] = 81  #"Q"
 *             elif t == NPY_FLOAT:       f[0] = 102 #"f"
 *             elif t == NPY_DOUBLE:      f[0] = 100 #"d"             # <<<<<<<<<<<<<<
 *             elif t == NPY_LONGDOUBLE:  f[0] = 103 #"g"
 *             elif t == NPY_CFLOAT:      f[0] = 90; f[1] = 102; f += 1 # Zf
 */
      __pyx_t_5 = PyInt_FromLong(NPY_DOUBLE); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 837; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_5);
      __pyx_t_3 = PyObject_RichCompare(__pyx_v_t, __pyx_t_5, Py_EQ); __Pyx_XGOTREF(__pyx_t_3); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 837; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      __pyx_t_7 = __Pyx_PyObject_IsTrue(__pyx_t_3); if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 837; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
      if (__pyx_t_7) {
        (__pyx_v_f[0]) = 100;
        goto __pyx_L13;
      }

      /* "numpy.pxd":838
 *             elif t == NPY_FLOAT:       f[0] = 102 #"f"
 *             elif t == NPY_DOUBLE:      f[0] = 100 #"d"
 *             elif t == NPY_LONGDOUBLE:  f[0] = 103 #"g"             # <<<<<<<<<<<<<<
 *             elif t == NPY_CFLOAT:      f[0] = 90; f[1] = 102; f += 1 # Zf
 *             elif t == NPY_CDOUBLE:     f[0] = 90; f[1] = 100; f += 1 # Zd
 */
      __pyx_t_3 = PyInt_FromLong(NPY_LONGDOUBLE); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 838; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_3);
      __pyx_t_5 = PyObject_RichCompare(__pyx_v_t, __pyx_t_3, Py_EQ); __Pyx_XGOTREF(__pyx_t_5); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 838; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
      __pyx_t_7 = __Pyx_PyObject_IsTrue(__pyx_t_5); if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 838; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      if (__pyx_t_7) {
        (__pyx_v_f[0]) = 103;
        goto __pyx_L13;
      }

      /* "numpy.pxd":839
 *             elif t == NPY_DOUBLE:      f[0] = 100 #"d"
 *             elif t == NPY_LONGDOUBLE:  f[0] = 103 #"g"
 *             elif t == NPY_CFLOAT:      f[0] = 90; f[1] = 102; f += 1 # Zf             # <<<<<<<<<<<<<<
 *             elif t == NPY_CDOUBLE:     f[0] = 90; f[1] = 100; f += 1 # Zd
 *             elif t == NPY_CLONGDOUBLE: f[0] = 90; f[1] = 103; f += 1 # Zg
 */
      __pyx_t_5 = PyInt_FromLong(NPY_CFLOAT); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 839; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_5);
      __pyx_t_3 = PyObject_RichCompare(__pyx_v_t, __pyx_t_5, Py_EQ); __Pyx_XGOTREF(__pyx_t_3); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 839; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      __pyx_t_7 = __Pyx_PyObject_IsTrue(__pyx_t_3); if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 839; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
      if (__pyx_t_7) {
        (__pyx_v_f[0]) = 90;
        (__pyx_v_f[1]) = 102;
        __pyx_v_f = (__pyx_v_f + 1);
        goto __pyx_L13;
      }

      /* "numpy.pxd":840
 *             elif t == NPY_LONGDOUBLE:  f[0] = 103 #"g"
 *             elif t == NPY_CFLOAT:      f[0] = 90; f[1] = 102; f += 1 # Zf
 *             elif t == NPY_CDOUBLE:     f[0] = 90; f[1] = 100; f += 1 # Zd             # <<<<<<<<<<<<<<
 *             elif t == NPY_CLONGDOUBLE: f[0] = 90; f[1] = 103; f += 1 # Zg
 *             elif t == NPY_OBJECT:      f[0] = 79 #"O"
 */
      __pyx_t_3 = PyInt_FromLong(NPY_CDOUBLE); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 840; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_3);
      __pyx_t_5 = PyObject_RichCompare(__pyx_v_t, __pyx_t_3, Py_EQ); __Pyx_XGOTREF(__pyx_t_5); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 840; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
      __pyx_t_7 = __Pyx_PyObject_IsTrue(__pyx_t_5); if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 840; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      if (__pyx_t_7) {
        (__pyx_v_f[0]) = 90;
        (__pyx_v_f[1]) = 100;
        __pyx_v_f = (__pyx_v_f + 1);
        goto __pyx_L13;
      }

      /* "numpy.pxd":841
 *             elif t == NPY_CFLOAT:      f[0] = 90; f[1] = 102; f += 1 # Zf
 *             elif t == NPY_CDOUBLE:     f[0] = 90; f[1] = 100; f += 1 # Zd
 *             elif t == NPY_CLONGDOUBLE: f[0] = 90; f[1] = 103; f += 1 # Zg             # <<<<<<<<<<<<<<
 *             elif t == NPY_OBJECT:      f[0] = 79 #"O"
 *             else:
 */
      __pyx_t_5 = PyInt_FromLong(NPY_CLONGDOUBLE); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 841; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_5);
      __pyx_t_3 = PyObject_RichCompare(__pyx_v_t, __pyx_t_5, Py_EQ); __Pyx_XGOTREF(__pyx_t_3); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 841; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      __pyx_t_7 = __Pyx_PyObject_IsTrue(__pyx_t_3); if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 841; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
      if (__pyx_t_7) {
        (__pyx_v_f[0]) = 90;
        (__pyx_v_f[1]) = 103;
        __pyx_v_f = (__pyx_v_f + 1);
        goto __pyx_L13;
      }

      /* "numpy.pxd":842
 *             elif t == NPY_CDOUBLE:     f[0] = 90; f[1] = 100; f += 1 # Zd
 *             elif t == NPY_CLONGDOUBLE: f[0] = 90; f[1] = 103; f += 1 # Zg
 *             elif t == NPY_OBJECT:      f[0] = 79 #"O"             # <<<<<<<<<<<<<<
 *             else:
 *                 raise ValueError(u"unknown dtype code in numpy.pxd (%d)" % t)
 */
      __pyx_t_3 = PyInt_FromLong(NPY_OBJECT); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 842; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_GOTREF(__pyx_t_3);
      __pyx_t_5 = PyObject_RichCompare(__pyx_v_t, __pyx_t_3, Py_EQ); __Pyx_XGOTREF(__pyx_t_5); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 842; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
      __pyx_t_7 = __Pyx_PyObject_IsTrue(__pyx_t_5); if (unlikely(__pyx_t_7 < 0)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 842; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
      if (__pyx_t_7) {
        (__pyx_v_f[0]) = 79;
        goto __pyx_L13;
      }
      /*else*/ {

        /* "numpy.pxd":844
 *             elif t == NPY_OBJECT:      f[0] = 79 #"O"
 *             else:
 *                 raise ValueError(u"unknown dtype code in numpy.pxd (%d)" % t)             # <<<<<<<<<<<<<<
 *             f += 1
 *         else:
 */
        __pyx_t_5 = PyNumber_Remainder(((PyObject *)__pyx_kp_u_11), __pyx_v_t); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 844; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
        __Pyx_GOTREF(((PyObject *)__pyx_t_5));
        __pyx_t_3 = PyTuple_New(1); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 844; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
        __Pyx_GOTREF(__pyx_t_3);
        PyTuple_SET_ITEM(__pyx_t_3, 0, ((PyObject *)__pyx_t_5));
        __Pyx_GIVEREF(((PyObject *)__pyx_t_5));
        __pyx_t_5 = 0;
        __pyx_t_5 = PyObject_Call(__pyx_builtin_ValueError, ((PyObject *)__pyx_t_3), NULL); if (unlikely(!__pyx_t_5)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 844; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
        __Pyx_GOTREF(__pyx_t_5);
        __Pyx_DECREF(((PyObject *)__pyx_t_3)); __pyx_t_3 = 0;
        __Pyx_Raise(__pyx_t_5, 0, 0, 0);
        __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
        {__pyx_filename = __pyx_f[1]; __pyx_lineno = 844; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      }
      __pyx_L13:;

      /* "numpy.pxd":845
 *             else:
 *                 raise ValueError(u"unknown dtype code in numpy.pxd (%d)" % t)
 *             f += 1             # <<<<<<<<<<<<<<
 *         else:
 *             # Cython ignores struct boundary information ("T{...}"),
 */
      __pyx_v_f = (__pyx_v_f + 1);
      goto __pyx_L11;
    }
    /*else*/ {

      /* "numpy.pxd":849
 *             # Cython ignores struct boundary information ("T{...}"),
 *             # so don't output it
 *             f = _util_dtypestring(child, f, end, offset)             # <<<<<<<<<<<<<<
 *     return f
 * 
 */
      __pyx_t_12 = __pyx_f_5numpy__util_dtypestring(__pyx_v_child, __pyx_v_f, __pyx_v_end, __pyx_v_offset); if (unlikely(__pyx_t_12 == NULL)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 849; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
      __pyx_v_f = __pyx_t_12;
    }
    __pyx_L11:;
  }
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;

  /* "numpy.pxd":850
 *             # so don't output it
 *             f = _util_dtypestring(child, f, end, offset)
 *     return f             # <<<<<<<<<<<<<<
 * 
 * 
 */
  __pyx_r = __pyx_v_f;
  goto __pyx_L0;

  __pyx_r = 0;
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  __Pyx_XDECREF(__pyx_t_3);
  __Pyx_XDECREF(__pyx_t_4);
  __Pyx_XDECREF(__pyx_t_5);
  __Pyx_AddTraceback("numpy._util_dtypestring", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = NULL;
  __pyx_L0:;
  __Pyx_XDECREF((PyObject *)__pyx_v_child);
  __Pyx_XDECREF(__pyx_v_fields);
  __Pyx_XDECREF(__pyx_v_childname);
  __Pyx_XDECREF(__pyx_v_new_offset);
  __Pyx_XDECREF(__pyx_v_t);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "numpy.pxd":965
 * 
 * 
 * cdef inline void set_array_base(ndarray arr, object base):             # <<<<<<<<<<<<<<
 *      cdef PyObject* baseptr
 *      if base is None:
 */

static CYTHON_INLINE void __pyx_f_5numpy_set_array_base(PyArrayObject *__pyx_v_arr, PyObject *__pyx_v_base) {
  PyObject *__pyx_v_baseptr;
  __Pyx_RefNannyDeclarations
  int __pyx_t_1;
  int __pyx_t_2;
  __Pyx_RefNannySetupContext("set_array_base", 0);

  /* "numpy.pxd":967
 * cdef inline void set_array_base(ndarray arr, object base):
 *      cdef PyObject* baseptr
 *      if base is None:             # <<<<<<<<<<<<<<
 *          baseptr = NULL
 *      else:
 */
  __pyx_t_1 = (__pyx_v_base == Py_None);
  __pyx_t_2 = (__pyx_t_1 != 0);
  if (__pyx_t_2) {

    /* "numpy.pxd":968
 *      cdef PyObject* baseptr
 *      if base is None:
 *          baseptr = NULL             # <<<<<<<<<<<<<<
 *      else:
 *          Py_INCREF(base) # important to do this before decref below!
 */
    __pyx_v_baseptr = NULL;
    goto __pyx_L3;
  }
  /*else*/ {

    /* "numpy.pxd":970
 *          baseptr = NULL
 *      else:
 *          Py_INCREF(base) # important to do this before decref below!             # <<<<<<<<<<<<<<
 *          baseptr = <PyObject*>base
 *      Py_XDECREF(arr.base)
 */
    Py_INCREF(__pyx_v_base);

    /* "numpy.pxd":971
 *      else:
 *          Py_INCREF(base) # important to do this before decref below!
 *          baseptr = <PyObject*>base             # <<<<<<<<<<<<<<
 *      Py_XDECREF(arr.base)
 *      arr.base = baseptr
 */
    __pyx_v_baseptr = ((PyObject *)__pyx_v_base);
  }
  __pyx_L3:;

  /* "numpy.pxd":972
 *          Py_INCREF(base) # important to do this before decref below!
 *          baseptr = <PyObject*>base
 *      Py_XDECREF(arr.base)             # <<<<<<<<<<<<<<
 *      arr.base = baseptr
 * 
 */
  Py_XDECREF(__pyx_v_arr->base);

  /* "numpy.pxd":973
 *          baseptr = <PyObject*>base
 *      Py_XDECREF(arr.base)
 *      arr.base = baseptr             # <<<<<<<<<<<<<<
 * 
 * cdef inline object get_array_base(ndarray arr):
 */
  __pyx_v_arr->base = __pyx_v_baseptr;

  __Pyx_RefNannyFinishContext();
}

/* "numpy.pxd":975
 *      arr.base = baseptr
 * 
 * cdef inline object get_array_base(ndarray arr):             # <<<<<<<<<<<<<<
 *     if arr.base is NULL:
 *         return None
 */

static CYTHON_INLINE PyObject *__pyx_f_5numpy_get_array_base(PyArrayObject *__pyx_v_arr) {
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  int __pyx_t_1;
  __Pyx_RefNannySetupContext("get_array_base", 0);

  /* "numpy.pxd":976
 * 
 * cdef inline object get_array_base(ndarray arr):
 *     if arr.base is NULL:             # <<<<<<<<<<<<<<
 *         return None
 *     else:
 */
  __pyx_t_1 = ((__pyx_v_arr->base == NULL) != 0);
  if (__pyx_t_1) {

    /* "numpy.pxd":977
 * cdef inline object get_array_base(ndarray arr):
 *     if arr.base is NULL:
 *         return None             # <<<<<<<<<<<<<<
 *     else:
 *         return <object>arr.base
 */
    __Pyx_XDECREF(__pyx_r);
    __Pyx_INCREF(Py_None);
    __pyx_r = Py_None;
    goto __pyx_L0;
    goto __pyx_L3;
  }
  /*else*/ {

    /* "numpy.pxd":979
 *         return None
 *     else:
 *         return <object>arr.base             # <<<<<<<<<<<<<<
 */
    __Pyx_XDECREF(__pyx_r);
    __Pyx_INCREF(((PyObject *)__pyx_v_arr->base));
    __pyx_r = ((PyObject *)__pyx_v_arr->base);
    goto __pyx_L0;
  }
  __pyx_L3:;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":104
 * # Datetime C API initialization function.
 * # You have to call it before any usage of DateTime CAPI functions.
 * cdef inline void import_datetime():             # <<<<<<<<<<<<<<
 *     PyDateTime_IMPORT
 * 
 */

static CYTHON_INLINE void __pyx_f_7cpython_8datetime_import_datetime(void) {
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("import_datetime", 0);

  /* "cpython/datetime.pxd":105
 * # You have to call it before any usage of DateTime CAPI functions.
 * cdef inline void import_datetime():
 *     PyDateTime_IMPORT             # <<<<<<<<<<<<<<
 * 
 * # Create date object using DateTime CAPI factory function.
 */
  PyDateTime_IMPORT;

  __Pyx_RefNannyFinishContext();
}

/* "cpython/datetime.pxd":109
 * # Create date object using DateTime CAPI factory function.
 * # Note, there are no range checks for any of the arguments.
 * cdef inline object date_new(int year, int month, int day):             # <<<<<<<<<<<<<<
 *     return PyDateTimeAPI.Date_FromDate(year, month, day, PyDateTimeAPI.DateType)
 * 
 */

static CYTHON_INLINE PyObject *__pyx_f_7cpython_8datetime_date_new(int __pyx_v_year, int __pyx_v_month, int __pyx_v_day) {
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  PyObject *__pyx_t_1 = NULL;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("date_new", 0);

  /* "cpython/datetime.pxd":110
 * # Note, there are no range checks for any of the arguments.
 * cdef inline object date_new(int year, int month, int day):
 *     return PyDateTimeAPI.Date_FromDate(year, month, day, PyDateTimeAPI.DateType)             # <<<<<<<<<<<<<<
 * 
 * # Create time object using DateTime CAPI factory function
 */
  __Pyx_XDECREF(__pyx_r);
  __pyx_t_1 = PyDateTimeAPI->Date_FromDate(__pyx_v_year, __pyx_v_month, __pyx_v_day, PyDateTimeAPI->DateType); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[2]; __pyx_lineno = 110; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_r = __pyx_t_1;
  __pyx_t_1 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  __Pyx_AddTraceback("cpython.datetime.date_new", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":114
 * # Create time object using DateTime CAPI factory function
 * # Note, there are no range checks for any of the arguments.
 * cdef inline object time_new(int hour, int minute, int second, int microsecond, object tz):             # <<<<<<<<<<<<<<
 *     return PyDateTimeAPI.Time_FromTime(hour, minute, second, microsecond, tz, PyDateTimeAPI.TimeType)
 * 
 */

static CYTHON_INLINE PyObject *__pyx_f_7cpython_8datetime_time_new(int __pyx_v_hour, int __pyx_v_minute, int __pyx_v_second, int __pyx_v_microsecond, PyObject *__pyx_v_tz) {
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  PyObject *__pyx_t_1 = NULL;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("time_new", 0);

  /* "cpython/datetime.pxd":115
 * # Note, there are no range checks for any of the arguments.
 * cdef inline object time_new(int hour, int minute, int second, int microsecond, object tz):
 *     return PyDateTimeAPI.Time_FromTime(hour, minute, second, microsecond, tz, PyDateTimeAPI.TimeType)             # <<<<<<<<<<<<<<
 * 
 * # Create datetime object using DateTime CAPI factory function.
 */
  __Pyx_XDECREF(__pyx_r);
  __pyx_t_1 = PyDateTimeAPI->Time_FromTime(__pyx_v_hour, __pyx_v_minute, __pyx_v_second, __pyx_v_microsecond, __pyx_v_tz, PyDateTimeAPI->TimeType); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[2]; __pyx_lineno = 115; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_r = __pyx_t_1;
  __pyx_t_1 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  __Pyx_AddTraceback("cpython.datetime.time_new", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":119
 * # Create datetime object using DateTime CAPI factory function.
 * # Note, there are no range checks for any of the arguments.
 * cdef inline object datetime_new(int year, int month, int day, int hour, int minute, int second, int microsecond, object tz):             # <<<<<<<<<<<<<<
 *     return PyDateTimeAPI.DateTime_FromDateAndTime(year, month, day, hour, minute, second, microsecond, tz, PyDateTimeAPI.DateTimeType)
 * 
 */

static CYTHON_INLINE PyObject *__pyx_f_7cpython_8datetime_datetime_new(int __pyx_v_year, int __pyx_v_month, int __pyx_v_day, int __pyx_v_hour, int __pyx_v_minute, int __pyx_v_second, int __pyx_v_microsecond, PyObject *__pyx_v_tz) {
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  PyObject *__pyx_t_1 = NULL;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("datetime_new", 0);

  /* "cpython/datetime.pxd":120
 * # Note, there are no range checks for any of the arguments.
 * cdef inline object datetime_new(int year, int month, int day, int hour, int minute, int second, int microsecond, object tz):
 *     return PyDateTimeAPI.DateTime_FromDateAndTime(year, month, day, hour, minute, second, microsecond, tz, PyDateTimeAPI.DateTimeType)             # <<<<<<<<<<<<<<
 * 
 * # Create timedelta object using DateTime CAPI factory function.
 */
  __Pyx_XDECREF(__pyx_r);
  __pyx_t_1 = PyDateTimeAPI->DateTime_FromDateAndTime(__pyx_v_year, __pyx_v_month, __pyx_v_day, __pyx_v_hour, __pyx_v_minute, __pyx_v_second, __pyx_v_microsecond, __pyx_v_tz, PyDateTimeAPI->DateTimeType); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[2]; __pyx_lineno = 120; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_r = __pyx_t_1;
  __pyx_t_1 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  __Pyx_AddTraceback("cpython.datetime.datetime_new", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":124
 * # Create timedelta object using DateTime CAPI factory function.
 * # Note, there are no range checks for any of the arguments.
 * cdef inline object timedelta_new(int days, int seconds, int useconds):             # <<<<<<<<<<<<<<
 *     return PyDateTimeAPI.Delta_FromDelta(days, seconds, useconds, 1, PyDateTimeAPI.DeltaType)
 * 
 */

static CYTHON_INLINE PyObject *__pyx_f_7cpython_8datetime_timedelta_new(int __pyx_v_days, int __pyx_v_seconds, int __pyx_v_useconds) {
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  PyObject *__pyx_t_1 = NULL;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("timedelta_new", 0);

  /* "cpython/datetime.pxd":125
 * # Note, there are no range checks for any of the arguments.
 * cdef inline object timedelta_new(int days, int seconds, int useconds):
 *     return PyDateTimeAPI.Delta_FromDelta(days, seconds, useconds, 1, PyDateTimeAPI.DeltaType)             # <<<<<<<<<<<<<<
 * 
 * # More recognizable getters for date/time/datetime/timedelta.
 */
  __Pyx_XDECREF(__pyx_r);
  __pyx_t_1 = PyDateTimeAPI->Delta_FromDelta(__pyx_v_days, __pyx_v_seconds, __pyx_v_useconds, 1, PyDateTimeAPI->DeltaType); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[2]; __pyx_lineno = 125; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_r = __pyx_t_1;
  __pyx_t_1 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  __Pyx_AddTraceback("cpython.datetime.timedelta_new", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":133
 * 
 * # Get tzinfo of time
 * cdef inline object time_tzinfo(object o):             # <<<<<<<<<<<<<<
 *     if (<PyDateTime_Time*>o).hastzinfo:
 *         return <object>(<PyDateTime_Time*>o).tzinfo
 */

static CYTHON_INLINE PyObject *__pyx_f_7cpython_8datetime_time_tzinfo(PyObject *__pyx_v_o) {
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  int __pyx_t_1;
  __Pyx_RefNannySetupContext("time_tzinfo", 0);

  /* "cpython/datetime.pxd":134
 * # Get tzinfo of time
 * cdef inline object time_tzinfo(object o):
 *     if (<PyDateTime_Time*>o).hastzinfo:             # <<<<<<<<<<<<<<
 *         return <object>(<PyDateTime_Time*>o).tzinfo
 *     else:
 */
  __pyx_t_1 = (((PyDateTime_Time *)__pyx_v_o)->hastzinfo != 0);
  if (__pyx_t_1) {

    /* "cpython/datetime.pxd":135
 * cdef inline object time_tzinfo(object o):
 *     if (<PyDateTime_Time*>o).hastzinfo:
 *         return <object>(<PyDateTime_Time*>o).tzinfo             # <<<<<<<<<<<<<<
 *     else:
 *         return None
 */
    __Pyx_XDECREF(__pyx_r);
    __Pyx_INCREF(((PyObject *)((PyDateTime_Time *)__pyx_v_o)->tzinfo));
    __pyx_r = ((PyObject *)((PyDateTime_Time *)__pyx_v_o)->tzinfo);
    goto __pyx_L0;
    goto __pyx_L3;
  }
  /*else*/ {

    /* "cpython/datetime.pxd":137
 *         return <object>(<PyDateTime_Time*>o).tzinfo
 *     else:
 *         return None             # <<<<<<<<<<<<<<
 * 
 * # Get tzinfo of datetime
 */
    __Pyx_XDECREF(__pyx_r);
    __Pyx_INCREF(Py_None);
    __pyx_r = Py_None;
    goto __pyx_L0;
  }
  __pyx_L3:;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":140
 * 
 * # Get tzinfo of datetime
 * cdef inline object datetime_tzinfo(object o):             # <<<<<<<<<<<<<<
 *     if (<PyDateTime_DateTime*>o).hastzinfo:
 *         return <object>(<PyDateTime_DateTime*>o).tzinfo
 */

static CYTHON_INLINE PyObject *__pyx_f_7cpython_8datetime_datetime_tzinfo(PyObject *__pyx_v_o) {
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  int __pyx_t_1;
  __Pyx_RefNannySetupContext("datetime_tzinfo", 0);

  /* "cpython/datetime.pxd":141
 * # Get tzinfo of datetime
 * cdef inline object datetime_tzinfo(object o):
 *     if (<PyDateTime_DateTime*>o).hastzinfo:             # <<<<<<<<<<<<<<
 *         return <object>(<PyDateTime_DateTime*>o).tzinfo
 *     else:
 */
  __pyx_t_1 = (((PyDateTime_DateTime *)__pyx_v_o)->hastzinfo != 0);
  if (__pyx_t_1) {

    /* "cpython/datetime.pxd":142
 * cdef inline object datetime_tzinfo(object o):
 *     if (<PyDateTime_DateTime*>o).hastzinfo:
 *         return <object>(<PyDateTime_DateTime*>o).tzinfo             # <<<<<<<<<<<<<<
 *     else:
 *         return None
 */
    __Pyx_XDECREF(__pyx_r);
    __Pyx_INCREF(((PyObject *)((PyDateTime_DateTime *)__pyx_v_o)->tzinfo));
    __pyx_r = ((PyObject *)((PyDateTime_DateTime *)__pyx_v_o)->tzinfo);
    goto __pyx_L0;
    goto __pyx_L3;
  }
  /*else*/ {

    /* "cpython/datetime.pxd":144
 *         return <object>(<PyDateTime_DateTime*>o).tzinfo
 *     else:
 *         return None             # <<<<<<<<<<<<<<
 * 
 * # Get year of date
 */
    __Pyx_XDECREF(__pyx_r);
    __Pyx_INCREF(Py_None);
    __pyx_r = Py_None;
    goto __pyx_L0;
  }
  __pyx_L3:;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":147
 * 
 * # Get year of date
 * cdef inline int date_year(object o):             # <<<<<<<<<<<<<<
 *     return PyDateTime_GET_YEAR(o)
 * 
 */

static CYTHON_INLINE int __pyx_f_7cpython_8datetime_date_year(PyObject *__pyx_v_o) {
  int __pyx_r;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("date_year", 0);

  /* "cpython/datetime.pxd":148
 * # Get year of date
 * cdef inline int date_year(object o):
 *     return PyDateTime_GET_YEAR(o)             # <<<<<<<<<<<<<<
 * 
 * # Get month of date
 */
  __pyx_r = PyDateTime_GET_YEAR(__pyx_v_o);
  goto __pyx_L0;

  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":151
 * 
 * # Get month of date
 * cdef inline int date_month(object o):             # <<<<<<<<<<<<<<
 *     return PyDateTime_GET_MONTH(o)
 * 
 */

static CYTHON_INLINE int __pyx_f_7cpython_8datetime_date_month(PyObject *__pyx_v_o) {
  int __pyx_r;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("date_month", 0);

  /* "cpython/datetime.pxd":152
 * # Get month of date
 * cdef inline int date_month(object o):
 *     return PyDateTime_GET_MONTH(o)             # <<<<<<<<<<<<<<
 * 
 * # Get day of date
 */
  __pyx_r = PyDateTime_GET_MONTH(__pyx_v_o);
  goto __pyx_L0;

  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":155
 * 
 * # Get day of date
 * cdef inline int date_day(object o):             # <<<<<<<<<<<<<<
 *     return PyDateTime_GET_DAY(o)
 * 
 */

static CYTHON_INLINE int __pyx_f_7cpython_8datetime_date_day(PyObject *__pyx_v_o) {
  int __pyx_r;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("date_day", 0);

  /* "cpython/datetime.pxd":156
 * # Get day of date
 * cdef inline int date_day(object o):
 *     return PyDateTime_GET_DAY(o)             # <<<<<<<<<<<<<<
 * 
 * # Get year of datetime
 */
  __pyx_r = PyDateTime_GET_DAY(__pyx_v_o);
  goto __pyx_L0;

  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":159
 * 
 * # Get year of datetime
 * cdef inline int datetime_year(object o):             # <<<<<<<<<<<<<<
 *     return PyDateTime_GET_YEAR(o)
 * 
 */

static CYTHON_INLINE int __pyx_f_7cpython_8datetime_datetime_year(PyObject *__pyx_v_o) {
  int __pyx_r;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("datetime_year", 0);

  /* "cpython/datetime.pxd":160
 * # Get year of datetime
 * cdef inline int datetime_year(object o):
 *     return PyDateTime_GET_YEAR(o)             # <<<<<<<<<<<<<<
 * 
 * # Get month of datetime
 */
  __pyx_r = PyDateTime_GET_YEAR(__pyx_v_o);
  goto __pyx_L0;

  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":163
 * 
 * # Get month of datetime
 * cdef inline int datetime_month(object o):             # <<<<<<<<<<<<<<
 *     return PyDateTime_GET_MONTH(o)
 * 
 */

static CYTHON_INLINE int __pyx_f_7cpython_8datetime_datetime_month(PyObject *__pyx_v_o) {
  int __pyx_r;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("datetime_month", 0);

  /* "cpython/datetime.pxd":164
 * # Get month of datetime
 * cdef inline int datetime_month(object o):
 *     return PyDateTime_GET_MONTH(o)             # <<<<<<<<<<<<<<
 * 
 * # Get day of datetime
 */
  __pyx_r = PyDateTime_GET_MONTH(__pyx_v_o);
  goto __pyx_L0;

  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":167
 * 
 * # Get day of datetime
 * cdef inline int datetime_day(object o):             # <<<<<<<<<<<<<<
 *     return PyDateTime_GET_DAY(o)
 * 
 */

static CYTHON_INLINE int __pyx_f_7cpython_8datetime_datetime_day(PyObject *__pyx_v_o) {
  int __pyx_r;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("datetime_day", 0);

  /* "cpython/datetime.pxd":168
 * # Get day of datetime
 * cdef inline int datetime_day(object o):
 *     return PyDateTime_GET_DAY(o)             # <<<<<<<<<<<<<<
 * 
 * # Get hour of time
 */
  __pyx_r = PyDateTime_GET_DAY(__pyx_v_o);
  goto __pyx_L0;

  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":171
 * 
 * # Get hour of time
 * cdef inline int time_hour(object o):             # <<<<<<<<<<<<<<
 *     return PyDateTime_TIME_GET_HOUR(o)
 * 
 */

static CYTHON_INLINE int __pyx_f_7cpython_8datetime_time_hour(PyObject *__pyx_v_o) {
  int __pyx_r;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("time_hour", 0);

  /* "cpython/datetime.pxd":172
 * # Get hour of time
 * cdef inline int time_hour(object o):
 *     return PyDateTime_TIME_GET_HOUR(o)             # <<<<<<<<<<<<<<
 * 
 * # Get minute of time
 */
  __pyx_r = PyDateTime_TIME_GET_HOUR(__pyx_v_o);
  goto __pyx_L0;

  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":175
 * 
 * # Get minute of time
 * cdef inline int time_minute(object o):             # <<<<<<<<<<<<<<
 *     return PyDateTime_TIME_GET_MINUTE(o)
 * 
 */

static CYTHON_INLINE int __pyx_f_7cpython_8datetime_time_minute(PyObject *__pyx_v_o) {
  int __pyx_r;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("time_minute", 0);

  /* "cpython/datetime.pxd":176
 * # Get minute of time
 * cdef inline int time_minute(object o):
 *     return PyDateTime_TIME_GET_MINUTE(o)             # <<<<<<<<<<<<<<
 * 
 * # Get second of time
 */
  __pyx_r = PyDateTime_TIME_GET_MINUTE(__pyx_v_o);
  goto __pyx_L0;

  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":179
 * 
 * # Get second of time
 * cdef inline int time_second(object o):             # <<<<<<<<<<<<<<
 *     return PyDateTime_TIME_GET_SECOND(o)
 * 
 */

static CYTHON_INLINE int __pyx_f_7cpython_8datetime_time_second(PyObject *__pyx_v_o) {
  int __pyx_r;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("time_second", 0);

  /* "cpython/datetime.pxd":180
 * # Get second of time
 * cdef inline int time_second(object o):
 *     return PyDateTime_TIME_GET_SECOND(o)             # <<<<<<<<<<<<<<
 * 
 * # Get microsecond of time
 */
  __pyx_r = PyDateTime_TIME_GET_SECOND(__pyx_v_o);
  goto __pyx_L0;

  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":183
 * 
 * # Get microsecond of time
 * cdef inline int time_microsecond(object o):             # <<<<<<<<<<<<<<
 *     return PyDateTime_TIME_GET_MICROSECOND(o)
 * 
 */

static CYTHON_INLINE int __pyx_f_7cpython_8datetime_time_microsecond(PyObject *__pyx_v_o) {
  int __pyx_r;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("time_microsecond", 0);

  /* "cpython/datetime.pxd":184
 * # Get microsecond of time
 * cdef inline int time_microsecond(object o):
 *     return PyDateTime_TIME_GET_MICROSECOND(o)             # <<<<<<<<<<<<<<
 * 
 * # Get hour of datetime
 */
  __pyx_r = PyDateTime_TIME_GET_MICROSECOND(__pyx_v_o);
  goto __pyx_L0;

  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":187
 * 
 * # Get hour of datetime
 * cdef inline int datetime_hour(object o):             # <<<<<<<<<<<<<<
 *     return PyDateTime_DATE_GET_HOUR(o)
 * 
 */

static CYTHON_INLINE int __pyx_f_7cpython_8datetime_datetime_hour(PyObject *__pyx_v_o) {
  int __pyx_r;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("datetime_hour", 0);

  /* "cpython/datetime.pxd":188
 * # Get hour of datetime
 * cdef inline int datetime_hour(object o):
 *     return PyDateTime_DATE_GET_HOUR(o)             # <<<<<<<<<<<<<<
 * 
 * # Get minute of datetime
 */
  __pyx_r = PyDateTime_DATE_GET_HOUR(__pyx_v_o);
  goto __pyx_L0;

  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":191
 * 
 * # Get minute of datetime
 * cdef inline int datetime_minute(object o):             # <<<<<<<<<<<<<<
 *     return PyDateTime_DATE_GET_MINUTE(o)
 * 
 */

static CYTHON_INLINE int __pyx_f_7cpython_8datetime_datetime_minute(PyObject *__pyx_v_o) {
  int __pyx_r;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("datetime_minute", 0);

  /* "cpython/datetime.pxd":192
 * # Get minute of datetime
 * cdef inline int datetime_minute(object o):
 *     return PyDateTime_DATE_GET_MINUTE(o)             # <<<<<<<<<<<<<<
 * 
 * # Get second of datetime
 */
  __pyx_r = PyDateTime_DATE_GET_MINUTE(__pyx_v_o);
  goto __pyx_L0;

  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":195
 * 
 * # Get second of datetime
 * cdef inline int datetime_second(object o):             # <<<<<<<<<<<<<<
 *     return PyDateTime_DATE_GET_SECOND(o)
 * 
 */

static CYTHON_INLINE int __pyx_f_7cpython_8datetime_datetime_second(PyObject *__pyx_v_o) {
  int __pyx_r;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("datetime_second", 0);

  /* "cpython/datetime.pxd":196
 * # Get second of datetime
 * cdef inline int datetime_second(object o):
 *     return PyDateTime_DATE_GET_SECOND(o)             # <<<<<<<<<<<<<<
 * 
 * # Get microsecond of datetime
 */
  __pyx_r = PyDateTime_DATE_GET_SECOND(__pyx_v_o);
  goto __pyx_L0;

  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":199
 * 
 * # Get microsecond of datetime
 * cdef inline int datetime_microsecond(object o):             # <<<<<<<<<<<<<<
 *     return PyDateTime_DATE_GET_MICROSECOND(o)
 * 
 */

static CYTHON_INLINE int __pyx_f_7cpython_8datetime_datetime_microsecond(PyObject *__pyx_v_o) {
  int __pyx_r;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("datetime_microsecond", 0);

  /* "cpython/datetime.pxd":200
 * # Get microsecond of datetime
 * cdef inline int datetime_microsecond(object o):
 *     return PyDateTime_DATE_GET_MICROSECOND(o)             # <<<<<<<<<<<<<<
 * 
 * # Get days of timedelta
 */
  __pyx_r = PyDateTime_DATE_GET_MICROSECOND(__pyx_v_o);
  goto __pyx_L0;

  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":203
 * 
 * # Get days of timedelta
 * cdef inline int timedelta_days(object o):             # <<<<<<<<<<<<<<
 *     return (<PyDateTime_Delta*>o).days
 * 
 */

static CYTHON_INLINE int __pyx_f_7cpython_8datetime_timedelta_days(PyObject *__pyx_v_o) {
  int __pyx_r;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("timedelta_days", 0);

  /* "cpython/datetime.pxd":204
 * # Get days of timedelta
 * cdef inline int timedelta_days(object o):
 *     return (<PyDateTime_Delta*>o).days             # <<<<<<<<<<<<<<
 * 
 * # Get seconds of timedelta
 */
  __pyx_r = ((PyDateTime_Delta *)__pyx_v_o)->days;
  goto __pyx_L0;

  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":207
 * 
 * # Get seconds of timedelta
 * cdef inline int timedelta_seconds(object o):             # <<<<<<<<<<<<<<
 *     return (<PyDateTime_Delta*>o).seconds
 * 
 */

static CYTHON_INLINE int __pyx_f_7cpython_8datetime_timedelta_seconds(PyObject *__pyx_v_o) {
  int __pyx_r;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("timedelta_seconds", 0);

  /* "cpython/datetime.pxd":208
 * # Get seconds of timedelta
 * cdef inline int timedelta_seconds(object o):
 *     return (<PyDateTime_Delta*>o).seconds             # <<<<<<<<<<<<<<
 * 
 * # Get microseconds of timedelta
 */
  __pyx_r = ((PyDateTime_Delta *)__pyx_v_o)->seconds;
  goto __pyx_L0;

  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

/* "cpython/datetime.pxd":211
 * 
 * # Get microseconds of timedelta
 * cdef inline int timedelta_microseconds(object o):             # <<<<<<<<<<<<<<
 *     return (<PyDateTime_Delta*>o).microseconds
 */

static CYTHON_INLINE int __pyx_f_7cpython_8datetime_timedelta_microseconds(PyObject *__pyx_v_o) {
  int __pyx_r;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("timedelta_microseconds", 0);

  /* "cpython/datetime.pxd":212
 * # Get microseconds of timedelta
 * cdef inline int timedelta_microseconds(object o):
 *     return (<PyDateTime_Delta*>o).microseconds             # <<<<<<<<<<<<<<
 */
  __pyx_r = ((PyDateTime_Delta *)__pyx_v_o)->microseconds;
  goto __pyx_L0;

  __pyx_r = 0;
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

static PyMethodDef __pyx_methods[] = {
  {0, 0, 0, 0}
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef __pyx_moduledef = {
  #if PY_VERSION_HEX < 0x03020000
    { PyObject_HEAD_INIT(NULL) NULL, 0, NULL },
  #else
    PyModuleDef_HEAD_INIT,
  #endif
    __Pyx_NAMESTR("bayesopt"),
    0, /* m_doc */
    -1, /* m_size */
    __pyx_methods /* m_methods */,
    NULL, /* m_reload */
    NULL, /* m_traverse */
    NULL, /* m_clear */
    NULL /* m_free */
};
#endif

static __Pyx_StringTabEntry __pyx_string_tab[] = {
  {&__pyx_kp_s_1, __pyx_k_1, sizeof(__pyx_k_1), 0, 0, 1, 0},
  {&__pyx_kp_u_11, __pyx_k_11, sizeof(__pyx_k_11), 0, 1, 0, 0},
  {&__pyx_kp_u_12, __pyx_k_12, sizeof(__pyx_k_12), 0, 1, 0, 0},
  {&__pyx_kp_u_15, __pyx_k_15, sizeof(__pyx_k_15), 0, 1, 0, 0},
  {&__pyx_kp_s_19, __pyx_k_19, sizeof(__pyx_k_19), 0, 0, 1, 0},
  {&__pyx_n_s_26, __pyx_k_26, sizeof(__pyx_k_26), 0, 0, 1, 1},
  {&__pyx_kp_s_3, __pyx_k_3, sizeof(__pyx_k_3), 0, 0, 1, 0},
  {&__pyx_kp_u_5, __pyx_k_5, sizeof(__pyx_k_5), 0, 1, 0, 0},
  {&__pyx_kp_u_7, __pyx_k_7, sizeof(__pyx_k_7), 0, 1, 0, 0},
  {&__pyx_kp_u_9, __pyx_k_9, sizeof(__pyx_k_9), 0, 1, 0, 0},
  {&__pyx_n_s__Exception, __pyx_k__Exception, sizeof(__pyx_k__Exception), 0, 0, 1, 1},
  {&__pyx_n_s__MemoryError, __pyx_k__MemoryError, sizeof(__pyx_k__MemoryError), 0, 0, 1, 1},
  {&__pyx_n_s__RuntimeError, __pyx_k__RuntimeError, sizeof(__pyx_k__RuntimeError), 0, 0, 1, 1},
  {&__pyx_n_s__ValueError, __pyx_k__ValueError, sizeof(__pyx_k__ValueError), 0, 0, 1, 1},
  {&__pyx_n_s____import__, __pyx_k____import__, sizeof(__pyx_k____import__), 0, 0, 1, 1},
  {&__pyx_n_s____main__, __pyx_k____main__, sizeof(__pyx_k____main__), 0, 0, 1, 1},
  {&__pyx_n_s____pyx_getbuffer, __pyx_k____pyx_getbuffer, sizeof(__pyx_k____pyx_getbuffer), 0, 0, 1, 1},
  {&__pyx_n_s____pyx_releasebuffer, __pyx_k____pyx_releasebuffer, sizeof(__pyx_k____pyx_releasebuffer), 0, 0, 1, 1},
  {&__pyx_n_s____test__, __pyx_k____test__, sizeof(__pyx_k____test__), 0, 0, 1, 1},
  {&__pyx_n_s__alpha, __pyx_k__alpha, sizeof(__pyx_k__alpha), 0, 0, 1, 1},
  {&__pyx_n_s__ascontiguousarray, __pyx_k__ascontiguousarray, sizeof(__pyx_k__ascontiguousarray), 0, 0, 1, 1},
  {&__pyx_n_s__bayesopt, __pyx_k__bayesopt, sizeof(__pyx_k__bayesopt), 0, 0, 1, 1},
  {&__pyx_n_s__beta, __pyx_k__beta, sizeof(__pyx_k__beta), 0, 0, 1, 1},
  {&__pyx_n_s__categories, __pyx_k__categories, sizeof(__pyx_k__categories), 0, 0, 1, 1},
  {&__pyx_n_s__crit_name, __pyx_k__crit_name, sizeof(__pyx_k__crit_name), 0, 0, 1, 1},
  {&__pyx_n_s__crit_params, __pyx_k__crit_params, sizeof(__pyx_k__crit_params), 0, 0, 1, 1},
  {&__pyx_n_s__double, __pyx_k__double, sizeof(__pyx_k__double), 0, 0, 1, 1},
  {&__pyx_n_s__dparams, __pyx_k__dparams, sizeof(__pyx_k__dparams), 0, 0, 1, 1},
  {&__pyx_n_s__dtype, __pyx_k__dtype, sizeof(__pyx_k__dtype), 0, 0, 1, 1},
  {&__pyx_n_s__epsilon, __pyx_k__epsilon, sizeof(__pyx_k__epsilon), 0, 0, 1, 1},
  {&__pyx_n_s__error_code, __pyx_k__error_code, sizeof(__pyx_k__error_code), 0, 0, 1, 1},
  {&__pyx_n_s__f, __pyx_k__f, sizeof(__pyx_k__f), 0, 0, 1, 1},
  {&__pyx_n_s__force_jump, __pyx_k__force_jump, sizeof(__pyx_k__force_jump), 0, 0, 1, 1},
  {&__pyx_n_s__get, __pyx_k__get, sizeof(__pyx_k__get), 0, 0, 1, 1},
  {&__pyx_n_s__init_method, __pyx_k__init_method, sizeof(__pyx_k__init_method), 0, 0, 1, 1},
  {&__pyx_n_s__int, __pyx_k__int, sizeof(__pyx_k__int), 0, 0, 1, 1},
  {&__pyx_n_s__kernel_hp_mean, __pyx_k__kernel_hp_mean, sizeof(__pyx_k__kernel_hp_mean), 0, 0, 1, 1},
  {&__pyx_n_s__kernel_hp_std, __pyx_k__kernel_hp_std, sizeof(__pyx_k__kernel_hp_std), 0, 0, 1, 1},
  {&__pyx_n_s__kernel_name, __pyx_k__kernel_name, sizeof(__pyx_k__kernel_name), 0, 0, 1, 1},
  {&__pyx_n_s__l_type, __pyx_k__l_type, sizeof(__pyx_k__l_type), 0, 0, 1, 1},
  {&__pyx_n_s__lb, __pyx_k__lb, sizeof(__pyx_k__lb), 0, 0, 1, 1},
  {&__pyx_n_s__log_filename, __pyx_k__log_filename, sizeof(__pyx_k__log_filename), 0, 0, 1, 1},
  {&__pyx_n_s__mean_coef_mean, __pyx_k__mean_coef_mean, sizeof(__pyx_k__mean_coef_mean), 0, 0, 1, 1},
  {&__pyx_n_s__mean_coef_std, __pyx_k__mean_coef_std, sizeof(__pyx_k__mean_coef_std), 0, 0, 1, 1},
  {&__pyx_n_s__mean_name, __pyx_k__mean_name, sizeof(__pyx_k__mean_name), 0, 0, 1, 1},
  {&__pyx_n_s__min_value, __pyx_k__min_value, sizeof(__pyx_k__min_value), 0, 0, 1, 1},
  {&__pyx_n_s__minf, __pyx_k__minf, sizeof(__pyx_k__minf), 0, 0, 1, 1},
  {&__pyx_n_s__nDim, __pyx_k__nDim, sizeof(__pyx_k__nDim), 0, 0, 1, 1},
  {&__pyx_n_s__n_init_samples, __pyx_k__n_init_samples, sizeof(__pyx_k__n_init_samples), 0, 0, 1, 1},
  {&__pyx_n_s__n_inner_iterations, __pyx_k__n_inner_iterations, sizeof(__pyx_k__n_inner_iterations), 0, 0, 1, 1},
  {&__pyx_n_s__n_iter_relearn, __pyx_k__n_iter_relearn, sizeof(__pyx_k__n_iter_relearn), 0, 0, 1, 1},
  {&__pyx_n_s__n_iterations, __pyx_k__n_iterations, sizeof(__pyx_k__n_iterations), 0, 0, 1, 1},
  {&__pyx_n_s__noise, __pyx_k__noise, sizeof(__pyx_k__noise), 0, 0, 1, 1},
  {&__pyx_n_s__np, __pyx_k__np, sizeof(__pyx_k__np), 0, 0, 1, 1},
  {&__pyx_n_s__np_categories, __pyx_k__np_categories, sizeof(__pyx_k__np_categories), 0, 0, 1, 1},
  {&__pyx_n_s__np_lb, __pyx_k__np_lb, sizeof(__pyx_k__np_lb), 0, 0, 1, 1},
  {&__pyx_n_s__np_ub, __pyx_k__np_ub, sizeof(__pyx_k__np_ub), 0, 0, 1, 1},
  {&__pyx_n_s__np_valid_x, __pyx_k__np_valid_x, sizeof(__pyx_k__np_valid_x), 0, 0, 1, 1},
  {&__pyx_n_s__np_x, __pyx_k__np_x, sizeof(__pyx_k__np_x), 0, 0, 1, 1},
  {&__pyx_n_s__numpy, __pyx_k__numpy, sizeof(__pyx_k__numpy), 0, 0, 1, 1},
  {&__pyx_n_s__ones, __pyx_k__ones, sizeof(__pyx_k__ones), 0, 0, 1, 1},
  {&__pyx_n_s__optimize, __pyx_k__optimize, sizeof(__pyx_k__optimize), 0, 0, 1, 1},
  {&__pyx_n_s__optimize_discrete, __pyx_k__optimize_discrete, sizeof(__pyx_k__optimize_discrete), 0, 0, 1, 1},
  {&__pyx_n_s__params, __pyx_k__params, sizeof(__pyx_k__params), 0, 0, 1, 1},
  {&__pyx_n_s__raise_problem, __pyx_k__raise_problem, sizeof(__pyx_k__raise_problem), 0, 0, 1, 1},
  {&__pyx_n_s__random_seed, __pyx_k__random_seed, sizeof(__pyx_k__random_seed), 0, 0, 1, 1},
  {&__pyx_n_s__range, __pyx_k__range, sizeof(__pyx_k__range), 0, 0, 1, 1},
  {&__pyx_n_s__sc_type, __pyx_k__sc_type, sizeof(__pyx_k__sc_type), 0, 0, 1, 1},
  {&__pyx_n_s__sigma_s, __pyx_k__sigma_s, sizeof(__pyx_k__sigma_s), 0, 0, 1, 1},
  {&__pyx_n_s__surr_name, __pyx_k__surr_name, sizeof(__pyx_k__surr_name), 0, 0, 1, 1},
  {&__pyx_n_s__ub, __pyx_k__ub, sizeof(__pyx_k__ub), 0, 0, 1, 1},
  {&__pyx_n_s__valid_x, __pyx_k__valid_x, sizeof(__pyx_k__valid_x), 0, 0, 1, 1},
  {&__pyx_n_s__verbose_level, __pyx_k__verbose_level, sizeof(__pyx_k__verbose_level), 0, 0, 1, 1},
  {&__pyx_n_s__x, __pyx_k__x, sizeof(__pyx_k__x), 0, 0, 1, 1},
  {&__pyx_n_s__zeros, __pyx_k__zeros, sizeof(__pyx_k__zeros), 0, 0, 1, 1},
  {0, 0, 0, 0, 0, 0, 0}
};
static int __Pyx_InitCachedBuiltins(void) {
  __pyx_builtin_range = __Pyx_GetBuiltinName(__pyx_n_s__range); if (!__pyx_builtin_range) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 165; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_builtin_Exception = __Pyx_GetBuiltinName(__pyx_n_s__Exception); if (!__pyx_builtin_Exception) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 215; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_builtin_ValueError = __Pyx_GetBuiltinName(__pyx_n_s__ValueError); if (!__pyx_builtin_ValueError) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 216; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_builtin_MemoryError = __Pyx_GetBuiltinName(__pyx_n_s__MemoryError); if (!__pyx_builtin_MemoryError) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 217; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_builtin_RuntimeError = __Pyx_GetBuiltinName(__pyx_n_s__RuntimeError); if (!__pyx_builtin_RuntimeError) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 218; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  return 0;
  __pyx_L1_error:;
  return -1;
}

static int __Pyx_InitCachedConstants(void) {
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("__Pyx_InitCachedConstants", 0);

  /* "bayesopt.pyx":215
 *     #static const int BAYESOPT_RUNTIME_ERROR = -4;
 * 
 *     if error_code == -1: raise Exception('Unknown error');             # <<<<<<<<<<<<<<
 *     elif error_code == -2: raise ValueError('Invalid argument');
 *     elif error_code == -3: raise MemoryError;
 */
  __pyx_k_tuple_2 = PyTuple_Pack(1, ((PyObject *)__pyx_kp_s_1)); if (unlikely(!__pyx_k_tuple_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 215; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_k_tuple_2);
  __Pyx_GIVEREF(((PyObject *)__pyx_k_tuple_2));

  /* "bayesopt.pyx":216
 * 
 *     if error_code == -1: raise Exception('Unknown error');
 *     elif error_code == -2: raise ValueError('Invalid argument');             # <<<<<<<<<<<<<<
 *     elif error_code == -3: raise MemoryError;
 *     elif error_code == -4: raise RuntimeError;
 */
  __pyx_k_tuple_4 = PyTuple_Pack(1, ((PyObject *)__pyx_kp_s_3)); if (unlikely(!__pyx_k_tuple_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 216; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_k_tuple_4);
  __Pyx_GIVEREF(((PyObject *)__pyx_k_tuple_4));

  /* "numpy.pxd":215
 *             if ((flags & pybuf.PyBUF_C_CONTIGUOUS == pybuf.PyBUF_C_CONTIGUOUS)
 *                 and not PyArray_CHKFLAGS(self, NPY_C_CONTIGUOUS)):
 *                 raise ValueError(u"ndarray is not C contiguous")             # <<<<<<<<<<<<<<
 * 
 *             if ((flags & pybuf.PyBUF_F_CONTIGUOUS == pybuf.PyBUF_F_CONTIGUOUS)
 */
  __pyx_k_tuple_6 = PyTuple_Pack(1, ((PyObject *)__pyx_kp_u_5)); if (unlikely(!__pyx_k_tuple_6)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 215; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_k_tuple_6);
  __Pyx_GIVEREF(((PyObject *)__pyx_k_tuple_6));

  /* "numpy.pxd":219
 *             if ((flags & pybuf.PyBUF_F_CONTIGUOUS == pybuf.PyBUF_F_CONTIGUOUS)
 *                 and not PyArray_CHKFLAGS(self, NPY_F_CONTIGUOUS)):
 *                 raise ValueError(u"ndarray is not Fortran contiguous")             # <<<<<<<<<<<<<<
 * 
 *             info.buf = PyArray_DATA(self)
 */
  __pyx_k_tuple_8 = PyTuple_Pack(1, ((PyObject *)__pyx_kp_u_7)); if (unlikely(!__pyx_k_tuple_8)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 219; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_k_tuple_8);
  __Pyx_GIVEREF(((PyObject *)__pyx_k_tuple_8));

  /* "numpy.pxd":257
 *                 if ((descr.byteorder == c'>' and little_endian) or
 *                     (descr.byteorder == c'<' and not little_endian)):
 *                     raise ValueError(u"Non-native byte order not supported")             # <<<<<<<<<<<<<<
 *                 if   t == NPY_BYTE:        f = "b"
 *                 elif t == NPY_UBYTE:       f = "B"
 */
  __pyx_k_tuple_10 = PyTuple_Pack(1, ((PyObject *)__pyx_kp_u_9)); if (unlikely(!__pyx_k_tuple_10)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 257; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_k_tuple_10);
  __Pyx_GIVEREF(((PyObject *)__pyx_k_tuple_10));

  /* "numpy.pxd":799
 * 
 *         if (end - f) - (new_offset - offset[0]) < 15:
 *             raise RuntimeError(u"Format string allocated too short, see comment in numpy.pxd")             # <<<<<<<<<<<<<<
 * 
 *         if ((child.byteorder == c'>' and little_endian) or
 */
  __pyx_k_tuple_13 = PyTuple_Pack(1, ((PyObject *)__pyx_kp_u_12)); if (unlikely(!__pyx_k_tuple_13)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 799; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_k_tuple_13);
  __Pyx_GIVEREF(((PyObject *)__pyx_k_tuple_13));

  /* "numpy.pxd":803
 *         if ((child.byteorder == c'>' and little_endian) or
 *             (child.byteorder == c'<' and not little_endian)):
 *             raise ValueError(u"Non-native byte order not supported")             # <<<<<<<<<<<<<<
 *             # One could encode it in the format string and have Cython
 *             # complain instead, BUT: < and > in format strings also imply
 */
  __pyx_k_tuple_14 = PyTuple_Pack(1, ((PyObject *)__pyx_kp_u_9)); if (unlikely(!__pyx_k_tuple_14)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 803; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_k_tuple_14);
  __Pyx_GIVEREF(((PyObject *)__pyx_k_tuple_14));

  /* "numpy.pxd":823
 *             t = child.type_num
 *             if end - f < 5:
 *                 raise RuntimeError(u"Format string allocated too short.")             # <<<<<<<<<<<<<<
 * 
 *             # Until ticket #99 is fixed, use integers to avoid warnings
 */
  __pyx_k_tuple_16 = PyTuple_Pack(1, ((PyObject *)__pyx_kp_u_15)); if (unlikely(!__pyx_k_tuple_16)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 823; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_k_tuple_16);
  __Pyx_GIVEREF(((PyObject *)__pyx_k_tuple_16));

  /* "bayesopt.pyx":204
 *         return HUGE_VAL
 * 
 * def raise_problem(error_code):             # <<<<<<<<<<<<<<
 *     # This is a little bit hacky, but we lose track of the C++
 *     # exception since we use the C wrapper for interface:
 */
  __pyx_k_tuple_17 = PyTuple_Pack(1, ((PyObject *)__pyx_n_s__error_code)); if (unlikely(!__pyx_k_tuple_17)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 204; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_k_tuple_17);
  __Pyx_GIVEREF(((PyObject *)__pyx_k_tuple_17));
  __pyx_k_codeobj_18 = (PyObject*)__Pyx_PyCode_New(1, 0, 1, 0, 0, __pyx_empty_bytes, __pyx_empty_tuple, __pyx_empty_tuple, __pyx_k_tuple_17, __pyx_empty_tuple, __pyx_empty_tuple, __pyx_kp_s_19, __pyx_n_s__raise_problem, 204, __pyx_empty_bytes); if (unlikely(!__pyx_k_codeobj_18)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 204; __pyx_clineno = __LINE__; goto __pyx_L1_error;}

  /* "bayesopt.pyx":220
 *     elif error_code == -4: raise RuntimeError;
 * 
 * def optimize(f, int nDim, np.ndarray[np.double_t] np_lb,             # <<<<<<<<<<<<<<
 *              np.ndarray[np.double_t] np_ub, dict dparams):
 * 
 */
  __pyx_k_tuple_20 = PyTuple_Pack(13, ((PyObject *)__pyx_n_s__f), ((PyObject *)__pyx_n_s__nDim), ((PyObject *)__pyx_n_s__np_lb), ((PyObject *)__pyx_n_s__np_ub), ((PyObject *)__pyx_n_s__dparams), ((PyObject *)__pyx_n_s__params), ((PyObject *)__pyx_n_s__minf), ((PyObject *)__pyx_n_s__np_x), ((PyObject *)__pyx_n_s__lb), ((PyObject *)__pyx_n_s__ub), ((PyObject *)__pyx_n_s__x), ((PyObject *)__pyx_n_s__error_code), ((PyObject *)__pyx_n_s__min_value)); if (unlikely(!__pyx_k_tuple_20)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 220; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_k_tuple_20);
  __Pyx_GIVEREF(((PyObject *)__pyx_k_tuple_20));
  __pyx_k_codeobj_21 = (PyObject*)__Pyx_PyCode_New(5, 0, 13, 0, 0, __pyx_empty_bytes, __pyx_empty_tuple, __pyx_empty_tuple, __pyx_k_tuple_20, __pyx_empty_tuple, __pyx_empty_tuple, __pyx_kp_s_19, __pyx_n_s__optimize, 220, __pyx_empty_bytes); if (unlikely(!__pyx_k_codeobj_21)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 220; __pyx_clineno = __LINE__; goto __pyx_L1_error;}

  /* "bayesopt.pyx":249
 * 
 * 
 * def optimize_discrete(f, np.ndarray[np.double_t,ndim=2] np_valid_x,             # <<<<<<<<<<<<<<
 *                       dict dparams):
 * 
 */
  __pyx_k_tuple_22 = PyTuple_Pack(11, ((PyObject *)__pyx_n_s__f), ((PyObject *)__pyx_n_s__np_valid_x), ((PyObject *)__pyx_n_s__dparams), ((PyObject *)__pyx_n_s__params), ((PyObject *)__pyx_n_s__nDim), ((PyObject *)__pyx_n_s__minf), ((PyObject *)__pyx_n_s__np_x), ((PyObject *)__pyx_n_s__x), ((PyObject *)__pyx_n_s__valid_x), ((PyObject *)__pyx_n_s__error_code), ((PyObject *)__pyx_n_s__min_value)); if (unlikely(!__pyx_k_tuple_22)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 249; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_k_tuple_22);
  __Pyx_GIVEREF(((PyObject *)__pyx_k_tuple_22));
  __pyx_k_codeobj_23 = (PyObject*)__Pyx_PyCode_New(3, 0, 11, 0, 0, __pyx_empty_bytes, __pyx_empty_tuple, __pyx_empty_tuple, __pyx_k_tuple_22, __pyx_empty_tuple, __pyx_empty_tuple, __pyx_kp_s_19, __pyx_n_s__optimize_discrete, 249, __pyx_empty_bytes); if (unlikely(!__pyx_k_codeobj_23)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 249; __pyx_clineno = __LINE__; goto __pyx_L1_error;}

  /* "bayesopt.pyx":278
 *     return min_value,np_x,error_code
 * 
 * def optimize_categorical(f, np.ndarray[np.int_t,ndim=1] np_categories,             # <<<<<<<<<<<<<<
 *                          dict dparams):
 * 
 */
  __pyx_k_tuple_24 = PyTuple_Pack(11, ((PyObject *)__pyx_n_s__f), ((PyObject *)__pyx_n_s__np_categories), ((PyObject *)__pyx_n_s__dparams), ((PyObject *)__pyx_n_s__params), ((PyObject *)__pyx_n_s__nDim), ((PyObject *)__pyx_n_s__minf), ((PyObject *)__pyx_n_s__np_x), ((PyObject *)__pyx_n_s__x), ((PyObject *)__pyx_n_s__categories), ((PyObject *)__pyx_n_s__error_code), ((PyObject *)__pyx_n_s__min_value)); if (unlikely(!__pyx_k_tuple_24)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 278; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_k_tuple_24);
  __Pyx_GIVEREF(((PyObject *)__pyx_k_tuple_24));
  __pyx_k_codeobj_25 = (PyObject*)__Pyx_PyCode_New(3, 0, 11, 0, 0, __pyx_empty_bytes, __pyx_empty_tuple, __pyx_empty_tuple, __pyx_k_tuple_24, __pyx_empty_tuple, __pyx_empty_tuple, __pyx_kp_s_19, __pyx_n_s_26, 278, __pyx_empty_bytes); if (unlikely(!__pyx_k_codeobj_25)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 278; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_RefNannyFinishContext();
  return 0;
  __pyx_L1_error:;
  __Pyx_RefNannyFinishContext();
  return -1;
}

static int __Pyx_InitGlobals(void) {
  if (__Pyx_InitStrings(__pyx_string_tab) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;};
  __pyx_int_neg_1 = PyInt_FromLong(-1); if (unlikely(!__pyx_int_neg_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;};
  __pyx_int_neg_2 = PyInt_FromLong(-2); if (unlikely(!__pyx_int_neg_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;};
  __pyx_int_neg_3 = PyInt_FromLong(-3); if (unlikely(!__pyx_int_neg_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;};
  __pyx_int_neg_4 = PyInt_FromLong(-4); if (unlikely(!__pyx_int_neg_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;};
  __pyx_int_15 = PyInt_FromLong(15); if (unlikely(!__pyx_int_15)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;};
  return 0;
  __pyx_L1_error:;
  return -1;
}

#if PY_MAJOR_VERSION < 3
PyMODINIT_FUNC initbayesopt(void); /*proto*/
PyMODINIT_FUNC initbayesopt(void)
#else
PyMODINIT_FUNC PyInit_bayesopt(void); /*proto*/
PyMODINIT_FUNC PyInit_bayesopt(void)
#endif
{
  PyObject *__pyx_t_1 = NULL;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannyDeclarations
  #if CYTHON_REFNANNY
  __Pyx_RefNanny = __Pyx_RefNannyImportAPI("refnanny");
  if (!__Pyx_RefNanny) {
      PyErr_Clear();
      __Pyx_RefNanny = __Pyx_RefNannyImportAPI("Cython.Runtime.refnanny");
      if (!__Pyx_RefNanny)
          Py_FatalError("failed to import 'refnanny' module");
  }
  #endif
  __Pyx_RefNannySetupContext("PyMODINIT_FUNC PyInit_bayesopt(void)", 0);
  if ( __Pyx_check_binary_version() < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_empty_tuple = PyTuple_New(0); if (unlikely(!__pyx_empty_tuple)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_empty_bytes = PyBytes_FromStringAndSize("", 0); if (unlikely(!__pyx_empty_bytes)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  #ifdef __Pyx_CyFunction_USED
  if (__Pyx_CyFunction_init() < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  #endif
  #ifdef __Pyx_FusedFunction_USED
  if (__pyx_FusedFunction_init() < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  #endif
  #ifdef __Pyx_Generator_USED
  if (__pyx_Generator_init() < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  #endif
  /*--- Library function declarations ---*/
  /*--- Threads initialization code ---*/
  #if defined(__PYX_FORCE_INIT_THREADS) && __PYX_FORCE_INIT_THREADS
  #ifdef WITH_THREAD /* Python build with threading support? */
  PyEval_InitThreads();
  #endif
  #endif
  /*--- Module creation code ---*/
  #if PY_MAJOR_VERSION < 3
  __pyx_m = Py_InitModule4(__Pyx_NAMESTR("bayesopt"), __pyx_methods, 0, 0, PYTHON_API_VERSION); Py_XINCREF(__pyx_m);
  #else
  __pyx_m = PyModule_Create(&__pyx_moduledef);
  #endif
  if (unlikely(!__pyx_m)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_d = PyModule_GetDict(__pyx_m); if (unlikely(!__pyx_d)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  Py_INCREF(__pyx_d);
  #if PY_MAJOR_VERSION >= 3
  {
    PyObject *modules = PyImport_GetModuleDict(); if (unlikely(!modules)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    if (!PyDict_GetItemString(modules, "bayesopt")) {
      if (unlikely(PyDict_SetItemString(modules, "bayesopt", __pyx_m) < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    }
  }
  #endif
  __pyx_b = PyImport_AddModule(__Pyx_NAMESTR(__Pyx_BUILTIN_MODULE_NAME)); if (unlikely(!__pyx_b)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  #if CYTHON_COMPILING_IN_PYPY
  Py_INCREF(__pyx_b);
  #endif
  if (__Pyx_SetAttrString(__pyx_m, "__builtins__", __pyx_b) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;};
  /*--- Initialize various global constants etc. ---*/
  if (unlikely(__Pyx_InitGlobals() < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  #if PY_MAJOR_VERSION < 3 && (__PYX_DEFAULT_STRING_ENCODING_IS_ASCII || __PYX_DEFAULT_STRING_ENCODING_IS_DEFAULT)
  if (__Pyx_init_sys_getdefaultencoding_params() < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  #endif
  if (__pyx_module_is_main_bayesopt) {
    if (__Pyx_SetAttrString(__pyx_m, "__name__", __pyx_n_s____main__) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;};
  }
  /*--- Builtin init code ---*/
  if (unlikely(__Pyx_InitCachedBuiltins() < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  /*--- Constants init code ---*/
  if (unlikely(__Pyx_InitCachedConstants() < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  /*--- Global init code ---*/
  /*--- Variable export code ---*/
  /*--- Function export code ---*/
  /*--- Type init code ---*/
  /*--- Type import code ---*/
  __pyx_ptype_7cpython_4type_type = __Pyx_ImportType(__Pyx_BUILTIN_MODULE_NAME, "type", 
  #if CYTHON_COMPILING_IN_PYPY
  sizeof(PyTypeObject),
  #else
  sizeof(PyHeapTypeObject),
  #endif
  0); if (unlikely(!__pyx_ptype_7cpython_4type_type)) {__pyx_filename = __pyx_f[3]; __pyx_lineno = 9; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_ptype_5numpy_dtype = __Pyx_ImportType("numpy", "dtype", sizeof(PyArray_Descr), 0); if (unlikely(!__pyx_ptype_5numpy_dtype)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 155; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_ptype_5numpy_flatiter = __Pyx_ImportType("numpy", "flatiter", sizeof(PyArrayIterObject), 0); if (unlikely(!__pyx_ptype_5numpy_flatiter)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 165; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_ptype_5numpy_broadcast = __Pyx_ImportType("numpy", "broadcast", sizeof(PyArrayMultiIterObject), 0); if (unlikely(!__pyx_ptype_5numpy_broadcast)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 169; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_ptype_5numpy_ndarray = __Pyx_ImportType("numpy", "ndarray", sizeof(PyArrayObject), 0); if (unlikely(!__pyx_ptype_5numpy_ndarray)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 178; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_ptype_5numpy_ufunc = __Pyx_ImportType("numpy", "ufunc", sizeof(PyUFuncObject), 0); if (unlikely(!__pyx_ptype_5numpy_ufunc)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 861; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_ptype_7cpython_4bool_bool = __Pyx_ImportType(__Pyx_BUILTIN_MODULE_NAME, "bool", sizeof(PyBoolObject), 0); if (unlikely(!__pyx_ptype_7cpython_4bool_bool)) {__pyx_filename = __pyx_f[4]; __pyx_lineno = 8; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_ptype_7cpython_7complex_complex = __Pyx_ImportType(__Pyx_BUILTIN_MODULE_NAME, "complex", sizeof(PyComplexObject), 0); if (unlikely(!__pyx_ptype_7cpython_7complex_complex)) {__pyx_filename = __pyx_f[5]; __pyx_lineno = 15; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_ptype_7cpython_8datetime_date = __Pyx_ImportType("datetime", "date", sizeof(PyDateTime_Date), 0); if (unlikely(!__pyx_ptype_7cpython_8datetime_date)) {__pyx_filename = __pyx_f[2]; __pyx_lineno = 9; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_ptype_7cpython_8datetime_time = __Pyx_ImportType("datetime", "time", sizeof(PyDateTime_Time), 0); if (unlikely(!__pyx_ptype_7cpython_8datetime_time)) {__pyx_filename = __pyx_f[2]; __pyx_lineno = 12; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_ptype_7cpython_8datetime_datetime = __Pyx_ImportType("datetime", "datetime", sizeof(PyDateTime_DateTime), 0); if (unlikely(!__pyx_ptype_7cpython_8datetime_datetime)) {__pyx_filename = __pyx_f[2]; __pyx_lineno = 15; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_ptype_7cpython_8datetime_timedelta = __Pyx_ImportType("datetime", "timedelta", sizeof(PyDateTime_Delta), 0); if (unlikely(!__pyx_ptype_7cpython_8datetime_timedelta)) {__pyx_filename = __pyx_f[2]; __pyx_lineno = 18; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_ptype_7cpython_8datetime_tzinfo = __Pyx_ImportType("datetime", "tzinfo", sizeof(PyDateTime_TZInfo), 0); if (unlikely(!__pyx_ptype_7cpython_8datetime_tzinfo)) {__pyx_filename = __pyx_f[2]; __pyx_lineno = 21; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  /*--- Variable import code ---*/
  /*--- Function import code ---*/
  /*--- Execution code ---*/

  /* "bayesopt.pyx":23
 * 
 * 
 * import numpy as np             # <<<<<<<<<<<<<<
 * cimport numpy as np
 * #from python_ref cimport Py_INCREF, Py_DECREF
 */
  __pyx_t_1 = __Pyx_Import(((PyObject *)__pyx_n_s__numpy), 0, -1); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 23; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  if (PyDict_SetItem(__pyx_d, __pyx_n_s__np, __pyx_t_1) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 23; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;

  /* "bayesopt.pyx":204
 *         return HUGE_VAL
 * 
 * def raise_problem(error_code):             # <<<<<<<<<<<<<<
 *     # This is a little bit hacky, but we lose track of the C++
 *     # exception since we use the C wrapper for interface:
 */
  __pyx_t_1 = PyCFunction_NewEx(&__pyx_mdef_8bayesopt_1raise_problem, NULL, __pyx_n_s__bayesopt); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 204; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  if (PyDict_SetItem(__pyx_d, __pyx_n_s__raise_problem, __pyx_t_1) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 204; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;

  /* "bayesopt.pyx":220
 *     elif error_code == -4: raise RuntimeError;
 * 
 * def optimize(f, int nDim, np.ndarray[np.double_t] np_lb,             # <<<<<<<<<<<<<<
 *              np.ndarray[np.double_t] np_ub, dict dparams):
 * 
 */
  __pyx_t_1 = PyCFunction_NewEx(&__pyx_mdef_8bayesopt_3optimize, NULL, __pyx_n_s__bayesopt); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 220; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  if (PyDict_SetItem(__pyx_d, __pyx_n_s__optimize, __pyx_t_1) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 220; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;

  /* "bayesopt.pyx":249
 * 
 * 
 * def optimize_discrete(f, np.ndarray[np.double_t,ndim=2] np_valid_x,             # <<<<<<<<<<<<<<
 *                       dict dparams):
 * 
 */
  __pyx_t_1 = PyCFunction_NewEx(&__pyx_mdef_8bayesopt_5optimize_discrete, NULL, __pyx_n_s__bayesopt); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 249; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  if (PyDict_SetItem(__pyx_d, __pyx_n_s__optimize_discrete, __pyx_t_1) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 249; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;

  /* "bayesopt.pyx":278
 *     return min_value,np_x,error_code
 * 
 * def optimize_categorical(f, np.ndarray[np.int_t,ndim=1] np_categories,             # <<<<<<<<<<<<<<
 *                          dict dparams):
 * 
 */
  __pyx_t_1 = PyCFunction_NewEx(&__pyx_mdef_8bayesopt_7optimize_categorical, NULL, __pyx_n_s__bayesopt); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 278; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  if (PyDict_SetItem(__pyx_d, __pyx_n_s_26, __pyx_t_1) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 278; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;

  /* "bayesopt.pyx":1
 * ## \file bayesopt.pyx \brief Cython wrapper for the BayesOpt Python API             # <<<<<<<<<<<<<<
 * # -------------------------------------------------------------------------
 * #    This file is part of BayesOpt, an efficient C++ library for
 */
  __pyx_t_1 = PyDict_New(); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(((PyObject *)__pyx_t_1));
  if (PyDict_SetItem(__pyx_d, __pyx_n_s____test__, ((PyObject *)__pyx_t_1)) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(((PyObject *)__pyx_t_1)); __pyx_t_1 = 0;

  /* "cpython/datetime.pxd":211
 * 
 * # Get microseconds of timedelta
 * cdef inline int timedelta_microseconds(object o):             # <<<<<<<<<<<<<<
 *     return (<PyDateTime_Delta*>o).microseconds
 */
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  if (__pyx_m) {
    __Pyx_AddTraceback("init bayesopt", __pyx_clineno, __pyx_lineno, __pyx_filename);
    Py_DECREF(__pyx_m); __pyx_m = 0;
  } else if (!PyErr_Occurred()) {
    PyErr_SetString(PyExc_ImportError, "init bayesopt");
  }
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  #if PY_MAJOR_VERSION < 3
  return;
  #else
  return __pyx_m;
  #endif
}

/* Runtime support code */
#if CYTHON_REFNANNY
static __Pyx_RefNannyAPIStruct *__Pyx_RefNannyImportAPI(const char *modname) {
    PyObject *m = NULL, *p = NULL;
    void *r = NULL;
    m = PyImport_ImportModule((char *)modname);
    if (!m) goto end;
    p = PyObject_GetAttrString(m, (char *)"RefNannyAPI");
    if (!p) goto end;
    r = PyLong_AsVoidPtr(p);
end:
    Py_XDECREF(p);
    Py_XDECREF(m);
    return (__Pyx_RefNannyAPIStruct *)r;
}
#endif /* CYTHON_REFNANNY */

static PyObject *__Pyx_GetBuiltinName(PyObject *name) {
    PyObject* result = __Pyx_PyObject_GetAttrStr(__pyx_b, name);
    if (unlikely(!result)) {
        PyErr_Format(PyExc_NameError,
#if PY_MAJOR_VERSION >= 3
            "name '%U' is not defined", name);
#else
            "name '%s' is not defined", PyString_AS_STRING(name));
#endif
    }
    return result;
}

static PyObject* __Pyx_PyDict_GetItemDefault(PyObject* d, PyObject* key, PyObject* default_value) {
    PyObject* value;
#if PY_MAJOR_VERSION >= 3
    value = PyDict_GetItemWithError(d, key);
    if (unlikely(!value)) {
        if (unlikely(PyErr_Occurred()))
            return NULL;
        value = default_value;
    }
    Py_INCREF(value);
#else
    if (PyString_CheckExact(key) || PyUnicode_CheckExact(key) || PyInt_CheckExact(key)) {
        value = PyDict_GetItem(d, key);
        if (unlikely(!value)) {
            value = default_value;
        }
        Py_INCREF(value);
    } else {
        if (default_value == Py_None)
            default_value = NULL;
        value = PyObject_CallMethodObjArgs(
            d, __pyx_n_s__get, key, default_value, NULL);
    }
#endif
    return value;
}

static CYTHON_INLINE PyObject *__Pyx_GetItemInt_Generic(PyObject *o, PyObject* j) {
    PyObject *r;
    if (!j) return NULL;
    r = PyObject_GetItem(o, j);
    Py_DECREF(j);
    return r;
}
static CYTHON_INLINE PyObject *__Pyx_GetItemInt_List_Fast(PyObject *o, Py_ssize_t i,
                                                              int wraparound, int boundscheck) {
#if CYTHON_COMPILING_IN_CPYTHON
    if (wraparound & unlikely(i < 0)) i += PyList_GET_SIZE(o);
    if ((!boundscheck) || likely((0 <= i) & (i < PyList_GET_SIZE(o)))) {
        PyObject *r = PyList_GET_ITEM(o, i);
        Py_INCREF(r);
        return r;
    }
    return __Pyx_GetItemInt_Generic(o, PyInt_FromSsize_t(i));
#else
    return PySequence_GetItem(o, i);
#endif
}
static CYTHON_INLINE PyObject *__Pyx_GetItemInt_Tuple_Fast(PyObject *o, Py_ssize_t i,
                                                              int wraparound, int boundscheck) {
#if CYTHON_COMPILING_IN_CPYTHON
    if (wraparound & unlikely(i < 0)) i += PyTuple_GET_SIZE(o);
    if ((!boundscheck) || likely((0 <= i) & (i < PyTuple_GET_SIZE(o)))) {
        PyObject *r = PyTuple_GET_ITEM(o, i);
        Py_INCREF(r);
        return r;
    }
    return __Pyx_GetItemInt_Generic(o, PyInt_FromSsize_t(i));
#else
    return PySequence_GetItem(o, i);
#endif
}
static CYTHON_INLINE PyObject *__Pyx_GetItemInt_Fast(PyObject *o, Py_ssize_t i,
                                                     int is_list, int wraparound, int boundscheck) {
#if CYTHON_COMPILING_IN_CPYTHON
    if (is_list || PyList_CheckExact(o)) {
        Py_ssize_t n = ((!wraparound) | likely(i >= 0)) ? i : i + PyList_GET_SIZE(o);
        if ((!boundscheck) || (likely((n >= 0) & (n < PyList_GET_SIZE(o))))) {
            PyObject *r = PyList_GET_ITEM(o, n);
            Py_INCREF(r);
            return r;
        }
    }
    else if (PyTuple_CheckExact(o)) {
        Py_ssize_t n = ((!wraparound) | likely(i >= 0)) ? i : i + PyTuple_GET_SIZE(o);
        if ((!boundscheck) || likely((n >= 0) & (n < PyTuple_GET_SIZE(o)))) {
            PyObject *r = PyTuple_GET_ITEM(o, n);
            Py_INCREF(r);
            return r;
        }
    } else {
        PySequenceMethods *m = Py_TYPE(o)->tp_as_sequence;
        if (likely(m && m->sq_item)) {
            if (wraparound && unlikely(i < 0) && likely(m->sq_length)) {
                Py_ssize_t l = m->sq_length(o);
                if (likely(l >= 0)) {
                    i += l;
                } else {
                    if (PyErr_ExceptionMatches(PyExc_OverflowError))
                        PyErr_Clear();
                    else
                        return NULL;
                }
            }
            return m->sq_item(o, i);
        }
    }
#else
    if (is_list || PySequence_Check(o)) {
        return PySequence_GetItem(o, i);
    }
#endif
    return __Pyx_GetItemInt_Generic(o, PyInt_FromSsize_t(i));
}

static CYTHON_INLINE void __Pyx_ErrRestore(PyObject *type, PyObject *value, PyObject *tb) {
#if CYTHON_COMPILING_IN_CPYTHON
    PyObject *tmp_type, *tmp_value, *tmp_tb;
    PyThreadState *tstate = PyThreadState_GET();
    tmp_type = tstate->curexc_type;
    tmp_value = tstate->curexc_value;
    tmp_tb = tstate->curexc_traceback;
    tstate->curexc_type = type;
    tstate->curexc_value = value;
    tstate->curexc_traceback = tb;
    Py_XDECREF(tmp_type);
    Py_XDECREF(tmp_value);
    Py_XDECREF(tmp_tb);
#else
    PyErr_Restore(type, value, tb);
#endif
}
static CYTHON_INLINE void __Pyx_ErrFetch(PyObject **type, PyObject **value, PyObject **tb) {
#if CYTHON_COMPILING_IN_CPYTHON
    PyThreadState *tstate = PyThreadState_GET();
    *type = tstate->curexc_type;
    *value = tstate->curexc_value;
    *tb = tstate->curexc_traceback;
    tstate->curexc_type = 0;
    tstate->curexc_value = 0;
    tstate->curexc_traceback = 0;
#else
    PyErr_Fetch(type, value, tb);
#endif
}

static void __Pyx_WriteUnraisable(const char *name, CYTHON_UNUSED int clineno,
                                  CYTHON_UNUSED int lineno, CYTHON_UNUSED const char *filename) {
    PyObject *old_exc, *old_val, *old_tb;
    PyObject *ctx;
    __Pyx_ErrFetch(&old_exc, &old_val, &old_tb);
    #if PY_MAJOR_VERSION < 3
    ctx = PyString_FromString(name);
    #else
    ctx = PyUnicode_FromString(name);
    #endif
    __Pyx_ErrRestore(old_exc, old_val, old_tb);
    if (!ctx) {
        PyErr_WriteUnraisable(Py_None);
    } else {
        PyErr_WriteUnraisable(ctx);
        Py_DECREF(ctx);
    }
}

static CYTHON_INLINE PyObject *__Pyx_GetModuleGlobalName(PyObject *name) {
    PyObject *result;
#if CYTHON_COMPILING_IN_CPYTHON
    result = PyDict_GetItem(__pyx_d, name);
    if (result) {
        Py_INCREF(result);
    } else {
#else
    result = PyObject_GetItem(__pyx_d, name);
    if (!result) {
        PyErr_Clear();
#endif
        result = __Pyx_GetBuiltinName(name);
    }
    return result;
}

static CYTHON_INLINE int __Pyx_SetItemInt_Generic(PyObject *o, PyObject *j, PyObject *v) {
    int r;
    if (!j) return -1;
    r = PyObject_SetItem(o, j, v);
    Py_DECREF(j);
    return r;
}
static CYTHON_INLINE int __Pyx_SetItemInt_Fast(PyObject *o, Py_ssize_t i, PyObject *v,
                                               int is_list, int wraparound, int boundscheck) {
#if CYTHON_COMPILING_IN_CPYTHON
    if (is_list || PyList_CheckExact(o)) {
        Py_ssize_t n = (!wraparound) ? i : ((likely(i >= 0)) ? i : i + PyList_GET_SIZE(o));
        if ((!boundscheck) || likely((n >= 0) & (n < PyList_GET_SIZE(o)))) {
            PyObject* old = PyList_GET_ITEM(o, n);
            Py_INCREF(v);
            PyList_SET_ITEM(o, n, v);
            Py_DECREF(old);
            return 1;
        }
    } else {
        PySequenceMethods *m = Py_TYPE(o)->tp_as_sequence;
        if (likely(m && m->sq_ass_item)) {
            if (wraparound && unlikely(i < 0) && likely(m->sq_length)) {
                Py_ssize_t l = m->sq_length(o);
                if (likely(l >= 0)) {
                    i += l;
                } else {
                    if (PyErr_ExceptionMatches(PyExc_OverflowError))
                        PyErr_Clear();
                    else
                        return -1;
                }
            }
            return m->sq_ass_item(o, i, v);
        }
    }
#else
#if CYTHON_COMPILING_IN_PYPY
    if (is_list || (PySequence_Check(o) && !PyDict_Check(o))) {
#else
    if (is_list || PySequence_Check(o)) {
#endif
        return PySequence_SetItem(o, i, v);
    }
#endif
    return __Pyx_SetItemInt_Generic(o, PyInt_FromSsize_t(i), v);
}

static int __Pyx_GetException(PyObject **type, PyObject **value, PyObject **tb) {
    PyObject *local_type, *local_value, *local_tb;
#if CYTHON_COMPILING_IN_CPYTHON
    PyObject *tmp_type, *tmp_value, *tmp_tb;
    PyThreadState *tstate = PyThreadState_GET();
    local_type = tstate->curexc_type;
    local_value = tstate->curexc_value;
    local_tb = tstate->curexc_traceback;
    tstate->curexc_type = 0;
    tstate->curexc_value = 0;
    tstate->curexc_traceback = 0;
#else
    PyErr_Fetch(&local_type, &local_value, &local_tb);
#endif
    PyErr_NormalizeException(&local_type, &local_value, &local_tb);
#if CYTHON_COMPILING_IN_CPYTHON
    if (unlikely(tstate->curexc_type))
#else
    if (unlikely(PyErr_Occurred()))
#endif
        goto bad;
    #if PY_MAJOR_VERSION >= 3
    if (unlikely(PyException_SetTraceback(local_value, local_tb) < 0))
        goto bad;
    #endif
    Py_INCREF(local_type);
    Py_INCREF(local_value);
    Py_INCREF(local_tb);
    *type = local_type;
    *value = local_value;
    *tb = local_tb;
#if CYTHON_COMPILING_IN_CPYTHON
    tmp_type = tstate->exc_type;
    tmp_value = tstate->exc_value;
    tmp_tb = tstate->exc_traceback;
    tstate->exc_type = local_type;
    tstate->exc_value = local_value;
    tstate->exc_traceback = local_tb;
    /* Make sure tstate is in a consistent state when we XDECREF
       these objects (DECREF may run arbitrary code). */
    Py_XDECREF(tmp_type);
    Py_XDECREF(tmp_value);
    Py_XDECREF(tmp_tb);
#else
    PyErr_SetExcInfo(local_type, local_value, local_tb);
#endif
    return 0;
bad:
    *type = 0;
    *value = 0;
    *tb = 0;
    Py_XDECREF(local_type);
    Py_XDECREF(local_value);
    Py_XDECREF(local_tb);
    return -1;
}

#if PY_MAJOR_VERSION < 3
static void __Pyx_Raise(PyObject *type, PyObject *value, PyObject *tb,
                        CYTHON_UNUSED PyObject *cause) {
    Py_XINCREF(type);
    if (!value || value == Py_None)
        value = NULL;
    else
        Py_INCREF(value);
    if (!tb || tb == Py_None)
        tb = NULL;
    else {
        Py_INCREF(tb);
        if (!PyTraceBack_Check(tb)) {
            PyErr_SetString(PyExc_TypeError,
                "raise: arg 3 must be a traceback or None");
            goto raise_error;
        }
    }
    #if PY_VERSION_HEX < 0x02050000
    if (PyClass_Check(type)) {
    #else
    if (PyType_Check(type)) {
    #endif
#if CYTHON_COMPILING_IN_PYPY
        if (!value) {
            Py_INCREF(Py_None);
            value = Py_None;
        }
#endif
        PyErr_NormalizeException(&type, &value, &tb);
    } else {
        if (value) {
            PyErr_SetString(PyExc_TypeError,
                "instance exception may not have a separate value");
            goto raise_error;
        }
        value = type;
        #if PY_VERSION_HEX < 0x02050000
        if (PyInstance_Check(type)) {
            type = (PyObject*) ((PyInstanceObject*)type)->in_class;
            Py_INCREF(type);
        } else {
            type = 0;
            PyErr_SetString(PyExc_TypeError,
                "raise: exception must be an old-style class or instance");
            goto raise_error;
        }
        #else
        type = (PyObject*) Py_TYPE(type);
        Py_INCREF(type);
        if (!PyType_IsSubtype((PyTypeObject *)type, (PyTypeObject *)PyExc_BaseException)) {
            PyErr_SetString(PyExc_TypeError,
                "raise: exception class must be a subclass of BaseException");
            goto raise_error;
        }
        #endif
    }
    __Pyx_ErrRestore(type, value, tb);
    return;
raise_error:
    Py_XDECREF(value);
    Py_XDECREF(type);
    Py_XDECREF(tb);
    return;
}
#else /* Python 3+ */
static void __Pyx_Raise(PyObject *type, PyObject *value, PyObject *tb, PyObject *cause) {
    PyObject* owned_instance = NULL;
    if (tb == Py_None) {
        tb = 0;
    } else if (tb && !PyTraceBack_Check(tb)) {
        PyErr_SetString(PyExc_TypeError,
            "raise: arg 3 must be a traceback or None");
        goto bad;
    }
    if (value == Py_None)
        value = 0;
    if (PyExceptionInstance_Check(type)) {
        if (value) {
            PyErr_SetString(PyExc_TypeError,
                "instance exception may not have a separate value");
            goto bad;
        }
        value = type;
        type = (PyObject*) Py_TYPE(value);
    } else if (PyExceptionClass_Check(type)) {
        PyObject *args;
        if (!value)
            args = PyTuple_New(0);
        else if (PyTuple_Check(value)) {
            Py_INCREF(value);
            args = value;
        } else
            args = PyTuple_Pack(1, value);
        if (!args)
            goto bad;
        owned_instance = PyEval_CallObject(type, args);
        Py_DECREF(args);
        if (!owned_instance)
            goto bad;
        value = owned_instance;
        if (!PyExceptionInstance_Check(value)) {
            PyErr_Format(PyExc_TypeError,
                         "calling %R should have returned an instance of "
                         "BaseException, not %R",
                         type, Py_TYPE(value));
            goto bad;
        }
    } else {
        PyErr_SetString(PyExc_TypeError,
            "raise: exception class must be a subclass of BaseException");
        goto bad;
    }
#if PY_VERSION_HEX >= 0x03030000
    if (cause) {
#else
    if (cause && cause != Py_None) {
#endif
        PyObject *fixed_cause;
        if (cause == Py_None) {
            fixed_cause = NULL;
        } else if (PyExceptionClass_Check(cause)) {
            fixed_cause = PyObject_CallObject(cause, NULL);
            if (fixed_cause == NULL)
                goto bad;
        } else if (PyExceptionInstance_Check(cause)) {
            fixed_cause = cause;
            Py_INCREF(fixed_cause);
        } else {
            PyErr_SetString(PyExc_TypeError,
                            "exception causes must derive from "
                            "BaseException");
            goto bad;
        }
        PyException_SetCause(value, fixed_cause);
    }
    PyErr_SetObject(type, value);
    if (tb) {
        PyThreadState *tstate = PyThreadState_GET();
        PyObject* tmp_tb = tstate->curexc_traceback;
        if (tb != tmp_tb) {
            Py_INCREF(tb);
            tstate->curexc_traceback = tb;
            Py_XDECREF(tmp_tb);
        }
    }
bad:
    Py_XDECREF(owned_instance);
    return;
}
#endif

static void __Pyx_RaiseArgtupleInvalid(
    const char* func_name,
    int exact,
    Py_ssize_t num_min,
    Py_ssize_t num_max,
    Py_ssize_t num_found)
{
    Py_ssize_t num_expected;
    const char *more_or_less;
    if (num_found < num_min) {
        num_expected = num_min;
        more_or_less = "at least";
    } else {
        num_expected = num_max;
        more_or_less = "at most";
    }
    if (exact) {
        more_or_less = "exactly";
    }
    PyErr_Format(PyExc_TypeError,
                 "%s() takes %s %" CYTHON_FORMAT_SSIZE_T "d positional argument%s (%" CYTHON_FORMAT_SSIZE_T "d given)",
                 func_name, more_or_less, num_expected,
                 (num_expected == 1) ? "" : "s", num_found);
}

static void __Pyx_RaiseDoubleKeywordsError(
    const char* func_name,
    PyObject* kw_name)
{
    PyErr_Format(PyExc_TypeError,
        #if PY_MAJOR_VERSION >= 3
        "%s() got multiple values for keyword argument '%U'", func_name, kw_name);
        #else
        "%s() got multiple values for keyword argument '%s'", func_name,
        PyString_AsString(kw_name));
        #endif
}

static int __Pyx_ParseOptionalKeywords(
    PyObject *kwds,
    PyObject **argnames[],
    PyObject *kwds2,
    PyObject *values[],
    Py_ssize_t num_pos_args,
    const char* function_name)
{
    PyObject *key = 0, *value = 0;
    Py_ssize_t pos = 0;
    PyObject*** name;
    PyObject*** first_kw_arg = argnames + num_pos_args;
    while (PyDict_Next(kwds, &pos, &key, &value)) {
        name = first_kw_arg;
        while (*name && (**name != key)) name++;
        if (*name) {
            values[name-argnames] = value;
            continue;
        }
        name = first_kw_arg;
        #if PY_MAJOR_VERSION < 3
        if (likely(PyString_CheckExact(key)) || likely(PyString_Check(key))) {
            while (*name) {
                if ((CYTHON_COMPILING_IN_PYPY || PyString_GET_SIZE(**name) == PyString_GET_SIZE(key))
                        && _PyString_Eq(**name, key)) {
                    values[name-argnames] = value;
                    break;
                }
                name++;
            }
            if (*name) continue;
            else {
                PyObject*** argname = argnames;
                while (argname != first_kw_arg) {
                    if ((**argname == key) || (
                            (CYTHON_COMPILING_IN_PYPY || PyString_GET_SIZE(**argname) == PyString_GET_SIZE(key))
                             && _PyString_Eq(**argname, key))) {
                        goto arg_passed_twice;
                    }
                    argname++;
                }
            }
        } else
        #endif
        if (likely(PyUnicode_Check(key))) {
            while (*name) {
                int cmp = (**name == key) ? 0 :
                #if !CYTHON_COMPILING_IN_PYPY && PY_MAJOR_VERSION >= 3
                    (PyUnicode_GET_SIZE(**name) != PyUnicode_GET_SIZE(key)) ? 1 :
                #endif
                    PyUnicode_Compare(**name, key);
                if (cmp < 0 && unlikely(PyErr_Occurred())) goto bad;
                if (cmp == 0) {
                    values[name-argnames] = value;
                    break;
                }
                name++;
            }
            if (*name) continue;
            else {
                PyObject*** argname = argnames;
                while (argname != first_kw_arg) {
                    int cmp = (**argname == key) ? 0 :
                    #if !CYTHON_COMPILING_IN_PYPY && PY_MAJOR_VERSION >= 3
                        (PyUnicode_GET_SIZE(**argname) != PyUnicode_GET_SIZE(key)) ? 1 :
                    #endif
                        PyUnicode_Compare(**argname, key);
                    if (cmp < 0 && unlikely(PyErr_Occurred())) goto bad;
                    if (cmp == 0) goto arg_passed_twice;
                    argname++;
                }
            }
        } else
            goto invalid_keyword_type;
        if (kwds2) {
            if (unlikely(PyDict_SetItem(kwds2, key, value))) goto bad;
        } else {
            goto invalid_keyword;
        }
    }
    return 0;
arg_passed_twice:
    __Pyx_RaiseDoubleKeywordsError(function_name, key);
    goto bad;
invalid_keyword_type:
    PyErr_Format(PyExc_TypeError,
        "%s() keywords must be strings", function_name);
    goto bad;
invalid_keyword:
    PyErr_Format(PyExc_TypeError,
    #if PY_MAJOR_VERSION < 3
        "%s() got an unexpected keyword argument '%s'",
        function_name, PyString_AsString(key));
    #else
        "%s() got an unexpected keyword argument '%U'",
        function_name, key);
    #endif
bad:
    return -1;
}

static int __Pyx_ArgTypeTest(PyObject *obj, PyTypeObject *type, int none_allowed,
    const char *name, int exact)
{
    if (!type) {
        PyErr_Format(PyExc_SystemError, "Missing type object");
        return 0;
    }
    if (none_allowed && obj == Py_None) return 1;
    else if (exact) {
        if (Py_TYPE(obj) == type) return 1;
    }
    else {
        if (PyObject_TypeCheck(obj, type)) return 1;
    }
    PyErr_Format(PyExc_TypeError,
        "Argument '%s' has incorrect type (expected %s, got %s)",
        name, type->tp_name, Py_TYPE(obj)->tp_name);
    return 0;
}

static CYTHON_INLINE int __Pyx_IsLittleEndian(void) {
  unsigned int n = 1;
  return *(unsigned char*)(&n) != 0;
}
static void __Pyx_BufFmt_Init(__Pyx_BufFmt_Context* ctx,
                              __Pyx_BufFmt_StackElem* stack,
                              __Pyx_TypeInfo* type) {
  stack[0].field = &ctx->root;
  stack[0].parent_offset = 0;
  ctx->root.type = type;
  ctx->root.name = "buffer dtype";
  ctx->root.offset = 0;
  ctx->head = stack;
  ctx->head->field = &ctx->root;
  ctx->fmt_offset = 0;
  ctx->head->parent_offset = 0;
  ctx->new_packmode = '@';
  ctx->enc_packmode = '@';
  ctx->new_count = 1;
  ctx->enc_count = 0;
  ctx->enc_type = 0;
  ctx->is_complex = 0;
  ctx->is_valid_array = 0;
  ctx->struct_alignment = 0;
  while (type->typegroup == 'S') {
    ++ctx->head;
    ctx->head->field = type->fields;
    ctx->head->parent_offset = 0;
    type = type->fields->type;
  }
}
static int __Pyx_BufFmt_ParseNumber(const char** ts) {
    int count;
    const char* t = *ts;
    if (*t < '0' || *t > '9') {
      return -1;
    } else {
        count = *t++ - '0';
        while (*t >= '0' && *t < '9') {
            count *= 10;
            count += *t++ - '0';
        }
    }
    *ts = t;
    return count;
}
static int __Pyx_BufFmt_ExpectNumber(const char **ts) {
    int number = __Pyx_BufFmt_ParseNumber(ts);
    if (number == -1) /* First char was not a digit */
        PyErr_Format(PyExc_ValueError,\
                     "Does not understand character buffer dtype format string ('%c')", **ts);
    return number;
}
static void __Pyx_BufFmt_RaiseUnexpectedChar(char ch) {
  PyErr_Format(PyExc_ValueError,
               "Unexpected format string character: '%c'", ch);
}
static const char* __Pyx_BufFmt_DescribeTypeChar(char ch, int is_complex) {
  switch (ch) {
    case 'c': return "'char'";
    case 'b': return "'signed char'";
    case 'B': return "'unsigned char'";
    case 'h': return "'short'";
    case 'H': return "'unsigned short'";
    case 'i': return "'int'";
    case 'I': return "'unsigned int'";
    case 'l': return "'long'";
    case 'L': return "'unsigned long'";
    case 'q': return "'long long'";
    case 'Q': return "'unsigned long long'";
    case 'f': return (is_complex ? "'complex float'" : "'float'");
    case 'd': return (is_complex ? "'complex double'" : "'double'");
    case 'g': return (is_complex ? "'complex long double'" : "'long double'");
    case 'T': return "a struct";
    case 'O': return "Python object";
    case 'P': return "a pointer";
    case 's': case 'p': return "a string";
    case 0: return "end";
    default: return "unparseable format string";
  }
}
static size_t __Pyx_BufFmt_TypeCharToStandardSize(char ch, int is_complex) {
  switch (ch) {
    case '?': case 'c': case 'b': case 'B': case 's': case 'p': return 1;
    case 'h': case 'H': return 2;
    case 'i': case 'I': case 'l': case 'L': return 4;
    case 'q': case 'Q': return 8;
    case 'f': return (is_complex ? 8 : 4);
    case 'd': return (is_complex ? 16 : 8);
    case 'g': {
      PyErr_SetString(PyExc_ValueError, "Python does not define a standard format string size for long double ('g')..");
      return 0;
    }
    case 'O': case 'P': return sizeof(void*);
    default:
      __Pyx_BufFmt_RaiseUnexpectedChar(ch);
      return 0;
    }
}
static size_t __Pyx_BufFmt_TypeCharToNativeSize(char ch, int is_complex) {
  switch (ch) {
    case 'c': case 'b': case 'B': case 's': case 'p': return 1;
    case 'h': case 'H': return sizeof(short);
    case 'i': case 'I': return sizeof(int);
    case 'l': case 'L': return sizeof(long);
    #ifdef HAVE_LONG_LONG
    case 'q': case 'Q': return sizeof(PY_LONG_LONG);
    #endif
    case 'f': return sizeof(float) * (is_complex ? 2 : 1);
    case 'd': return sizeof(double) * (is_complex ? 2 : 1);
    case 'g': return sizeof(long double) * (is_complex ? 2 : 1);
    case 'O': case 'P': return sizeof(void*);
    default: {
      __Pyx_BufFmt_RaiseUnexpectedChar(ch);
      return 0;
    }
  }
}
typedef struct { char c; short x; } __Pyx_st_short;
typedef struct { char c; int x; } __Pyx_st_int;
typedef struct { char c; long x; } __Pyx_st_long;
typedef struct { char c; float x; } __Pyx_st_float;
typedef struct { char c; double x; } __Pyx_st_double;
typedef struct { char c; long double x; } __Pyx_st_longdouble;
typedef struct { char c; void *x; } __Pyx_st_void_p;
#ifdef HAVE_LONG_LONG
typedef struct { char c; PY_LONG_LONG x; } __Pyx_st_longlong;
#endif
static size_t __Pyx_BufFmt_TypeCharToAlignment(char ch, CYTHON_UNUSED int is_complex) {
  switch (ch) {
    case '?': case 'c': case 'b': case 'B': case 's': case 'p': return 1;
    case 'h': case 'H': return sizeof(__Pyx_st_short) - sizeof(short);
    case 'i': case 'I': return sizeof(__Pyx_st_int) - sizeof(int);
    case 'l': case 'L': return sizeof(__Pyx_st_long) - sizeof(long);
#ifdef HAVE_LONG_LONG
    case 'q': case 'Q': return sizeof(__Pyx_st_longlong) - sizeof(PY_LONG_LONG);
#endif
    case 'f': return sizeof(__Pyx_st_float) - sizeof(float);
    case 'd': return sizeof(__Pyx_st_double) - sizeof(double);
    case 'g': return sizeof(__Pyx_st_longdouble) - sizeof(long double);
    case 'P': case 'O': return sizeof(__Pyx_st_void_p) - sizeof(void*);
    default:
      __Pyx_BufFmt_RaiseUnexpectedChar(ch);
      return 0;
    }
}
/* These are for computing the padding at the end of the struct to align
   on the first member of the struct. This will probably the same as above,
   but we don't have any guarantees.
 */
typedef struct { short x; char c; } __Pyx_pad_short;
typedef struct { int x; char c; } __Pyx_pad_int;
typedef struct { long x; char c; } __Pyx_pad_long;
typedef struct { float x; char c; } __Pyx_pad_float;
typedef struct { double x; char c; } __Pyx_pad_double;
typedef struct { long double x; char c; } __Pyx_pad_longdouble;
typedef struct { void *x; char c; } __Pyx_pad_void_p;
#ifdef HAVE_LONG_LONG
typedef struct { PY_LONG_LONG x; char c; } __Pyx_pad_longlong;
#endif
static size_t __Pyx_BufFmt_TypeCharToPadding(char ch, CYTHON_UNUSED int is_complex) {
  switch (ch) {
    case '?': case 'c': case 'b': case 'B': case 's': case 'p': return 1;
    case 'h': case 'H': return sizeof(__Pyx_pad_short) - sizeof(short);
    case 'i': case 'I': return sizeof(__Pyx_pad_int) - sizeof(int);
    case 'l': case 'L': return sizeof(__Pyx_pad_long) - sizeof(long);
#ifdef HAVE_LONG_LONG
    case 'q': case 'Q': return sizeof(__Pyx_pad_longlong) - sizeof(PY_LONG_LONG);
#endif
    case 'f': return sizeof(__Pyx_pad_float) - sizeof(float);
    case 'd': return sizeof(__Pyx_pad_double) - sizeof(double);
    case 'g': return sizeof(__Pyx_pad_longdouble) - sizeof(long double);
    case 'P': case 'O': return sizeof(__Pyx_pad_void_p) - sizeof(void*);
    default:
      __Pyx_BufFmt_RaiseUnexpectedChar(ch);
      return 0;
    }
}
static char __Pyx_BufFmt_TypeCharToGroup(char ch, int is_complex) {
  switch (ch) {
    case 'c':
        return 'H';
    case 'b': case 'h': case 'i':
    case 'l': case 'q': case 's': case 'p':
        return 'I';
    case 'B': case 'H': case 'I': case 'L': case 'Q':
        return 'U';
    case 'f': case 'd': case 'g':
        return (is_complex ? 'C' : 'R');
    case 'O':
        return 'O';
    case 'P':
        return 'P';
    default: {
      __Pyx_BufFmt_RaiseUnexpectedChar(ch);
      return 0;
    }
  }
}
static void __Pyx_BufFmt_RaiseExpected(__Pyx_BufFmt_Context* ctx) {
  if (ctx->head == NULL || ctx->head->field == &ctx->root) {
    const char* expected;
    const char* quote;
    if (ctx->head == NULL) {
      expected = "end";
      quote = "";
    } else {
      expected = ctx->head->field->type->name;
      quote = "'";
    }
    PyErr_Format(PyExc_ValueError,
                 "Buffer dtype mismatch, expected %s%s%s but got %s",
                 quote, expected, quote,
                 __Pyx_BufFmt_DescribeTypeChar(ctx->enc_type, ctx->is_complex));
  } else {
    __Pyx_StructField* field = ctx->head->field;
    __Pyx_StructField* parent = (ctx->head - 1)->field;
    PyErr_Format(PyExc_ValueError,
                 "Buffer dtype mismatch, expected '%s' but got %s in '%s.%s'",
                 field->type->name, __Pyx_BufFmt_DescribeTypeChar(ctx->enc_type, ctx->is_complex),
                 parent->type->name, field->name);
  }
}
static int __Pyx_BufFmt_ProcessTypeChunk(__Pyx_BufFmt_Context* ctx) {
  char group;
  size_t size, offset, arraysize = 1;
  if (ctx->enc_type == 0) return 0;
  if (ctx->head->field->type->arraysize[0]) {
    int i, ndim = 0;
    if (ctx->enc_type == 's' || ctx->enc_type == 'p') {
        ctx->is_valid_array = ctx->head->field->type->ndim == 1;
        ndim = 1;
        if (ctx->enc_count != ctx->head->field->type->arraysize[0]) {
            PyErr_Format(PyExc_ValueError,
                         "Expected a dimension of size %zu, got %zu",
                         ctx->head->field->type->arraysize[0], ctx->enc_count);
            return -1;
        }
    }
    if (!ctx->is_valid_array) {
      PyErr_Format(PyExc_ValueError, "Expected %d dimensions, got %d",
                   ctx->head->field->type->ndim, ndim);
      return -1;
    }
    for (i = 0; i < ctx->head->field->type->ndim; i++) {
      arraysize *= ctx->head->field->type->arraysize[i];
    }
    ctx->is_valid_array = 0;
    ctx->enc_count = 1;
  }
  group = __Pyx_BufFmt_TypeCharToGroup(ctx->enc_type, ctx->is_complex);
  do {
    __Pyx_StructField* field = ctx->head->field;
    __Pyx_TypeInfo* type = field->type;
    if (ctx->enc_packmode == '@' || ctx->enc_packmode == '^') {
      size = __Pyx_BufFmt_TypeCharToNativeSize(ctx->enc_type, ctx->is_complex);
    } else {
      size = __Pyx_BufFmt_TypeCharToStandardSize(ctx->enc_type, ctx->is_complex);
    }
    if (ctx->enc_packmode == '@') {
      size_t align_at = __Pyx_BufFmt_TypeCharToAlignment(ctx->enc_type, ctx->is_complex);
      size_t align_mod_offset;
      if (align_at == 0) return -1;
      align_mod_offset = ctx->fmt_offset % align_at;
      if (align_mod_offset > 0) ctx->fmt_offset += align_at - align_mod_offset;
      if (ctx->struct_alignment == 0)
          ctx->struct_alignment = __Pyx_BufFmt_TypeCharToPadding(ctx->enc_type,
                                                                 ctx->is_complex);
    }
    if (type->size != size || type->typegroup != group) {
      if (type->typegroup == 'C' && type->fields != NULL) {
        size_t parent_offset = ctx->head->parent_offset + field->offset;
        ++ctx->head;
        ctx->head->field = type->fields;
        ctx->head->parent_offset = parent_offset;
        continue;
      }
      if ((type->typegroup == 'H' || group == 'H') && type->size == size) {
      } else {
          __Pyx_BufFmt_RaiseExpected(ctx);
          return -1;
      }
    }
    offset = ctx->head->parent_offset + field->offset;
    if (ctx->fmt_offset != offset) {
      PyErr_Format(PyExc_ValueError,
                   "Buffer dtype mismatch; next field is at offset %" CYTHON_FORMAT_SSIZE_T "d but %" CYTHON_FORMAT_SSIZE_T "d expected",
                   (Py_ssize_t)ctx->fmt_offset, (Py_ssize_t)offset);
      return -1;
    }
    ctx->fmt_offset += size;
    if (arraysize)
      ctx->fmt_offset += (arraysize - 1) * size;
    --ctx->enc_count; /* Consume from buffer string */
    while (1) {
      if (field == &ctx->root) {
        ctx->head = NULL;
        if (ctx->enc_count != 0) {
          __Pyx_BufFmt_RaiseExpected(ctx);
          return -1;
        }
        break; /* breaks both loops as ctx->enc_count == 0 */
      }
      ctx->head->field = ++field;
      if (field->type == NULL) {
        --ctx->head;
        field = ctx->head->field;
        continue;
      } else if (field->type->typegroup == 'S') {
        size_t parent_offset = ctx->head->parent_offset + field->offset;
        if (field->type->fields->type == NULL) continue; /* empty struct */
        field = field->type->fields;
        ++ctx->head;
        ctx->head->field = field;
        ctx->head->parent_offset = parent_offset;
        break;
      } else {
        break;
      }
    }
  } while (ctx->enc_count);
  ctx->enc_type = 0;
  ctx->is_complex = 0;
  return 0;
}
static CYTHON_INLINE PyObject *
__pyx_buffmt_parse_array(__Pyx_BufFmt_Context* ctx, const char** tsp)
{
    const char *ts = *tsp;
    int i = 0, number;
    int ndim = ctx->head->field->type->ndim;
;
    ++ts;
    if (ctx->new_count != 1) {
        PyErr_SetString(PyExc_ValueError,
                        "Cannot handle repeated arrays in format string");
        return NULL;
    }
    if (__Pyx_BufFmt_ProcessTypeChunk(ctx) == -1) return NULL;
    while (*ts && *ts != ')') {
        if (isspace(*ts))
            continue;
        number = __Pyx_BufFmt_ExpectNumber(&ts);
        if (number == -1) return NULL;
        if (i < ndim && (size_t) number != ctx->head->field->type->arraysize[i])
            return PyErr_Format(PyExc_ValueError,
                        "Expected a dimension of size %zu, got %d",
                        ctx->head->field->type->arraysize[i], number);
        if (*ts != ',' && *ts != ')')
            return PyErr_Format(PyExc_ValueError,
                                "Expected a comma in format string, got '%c'", *ts);
        if (*ts == ',') ts++;
        i++;
    }
    if (i != ndim)
        return PyErr_Format(PyExc_ValueError, "Expected %d dimension(s), got %d",
                            ctx->head->field->type->ndim, i);
    if (!*ts) {
        PyErr_SetString(PyExc_ValueError,
                        "Unexpected end of format string, expected ')'");
        return NULL;
    }
    ctx->is_valid_array = 1;
    ctx->new_count = 1;
    *tsp = ++ts;
    return Py_None;
}
static const char* __Pyx_BufFmt_CheckString(__Pyx_BufFmt_Context* ctx, const char* ts) {
  int got_Z = 0;
  while (1) {
    switch(*ts) {
      case 0:
        if (ctx->enc_type != 0 && ctx->head == NULL) {
          __Pyx_BufFmt_RaiseExpected(ctx);
          return NULL;
        }
        if (__Pyx_BufFmt_ProcessTypeChunk(ctx) == -1) return NULL;
        if (ctx->head != NULL) {
          __Pyx_BufFmt_RaiseExpected(ctx);
          return NULL;
        }
                return ts;
      case ' ':
      case 10:
      case 13:
        ++ts;
        break;
      case '<':
        if (!__Pyx_IsLittleEndian()) {
          PyErr_SetString(PyExc_ValueError, "Little-endian buffer not supported on big-endian compiler");
          return NULL;
        }
        ctx->new_packmode = '=';
        ++ts;
        break;
      case '>':
      case '!':
        if (__Pyx_IsLittleEndian()) {
          PyErr_SetString(PyExc_ValueError, "Big-endian buffer not supported on little-endian compiler");
          return NULL;
        }
        ctx->new_packmode = '=';
        ++ts;
        break;
      case '=':
      case '@':
      case '^':
        ctx->new_packmode = *ts++;
        break;
      case 'T': /* substruct */
        {
          const char* ts_after_sub;
          size_t i, struct_count = ctx->new_count;
          size_t struct_alignment = ctx->struct_alignment;
          ctx->new_count = 1;
          ++ts;
          if (*ts != '{') {
            PyErr_SetString(PyExc_ValueError, "Buffer acquisition: Expected '{' after 'T'");
            return NULL;
          }
          if (__Pyx_BufFmt_ProcessTypeChunk(ctx) == -1) return NULL;
          ctx->enc_type = 0; /* Erase processed last struct element */
          ctx->enc_count = 0;
          ctx->struct_alignment = 0;
          ++ts;
          ts_after_sub = ts;
          for (i = 0; i != struct_count; ++i) {
            ts_after_sub = __Pyx_BufFmt_CheckString(ctx, ts);
            if (!ts_after_sub) return NULL;
          }
          ts = ts_after_sub;
          if (struct_alignment) ctx->struct_alignment = struct_alignment;
        }
        break;
      case '}': /* end of substruct; either repeat or move on */
        {
          size_t alignment = ctx->struct_alignment;
          ++ts;
          if (__Pyx_BufFmt_ProcessTypeChunk(ctx) == -1) return NULL;
          ctx->enc_type = 0; /* Erase processed last struct element */
          if (alignment && ctx->fmt_offset % alignment) {
            ctx->fmt_offset += alignment - (ctx->fmt_offset % alignment);
          }
        }
        return ts;
      case 'x':
        if (__Pyx_BufFmt_ProcessTypeChunk(ctx) == -1) return NULL;
        ctx->fmt_offset += ctx->new_count;
        ctx->new_count = 1;
        ctx->enc_count = 0;
        ctx->enc_type = 0;
        ctx->enc_packmode = ctx->new_packmode;
        ++ts;
        break;
      case 'Z':
        got_Z = 1;
        ++ts;
        if (*ts != 'f' && *ts != 'd' && *ts != 'g') {
          __Pyx_BufFmt_RaiseUnexpectedChar('Z');
          return NULL;
        }        /* fall through */
      case 'c': case 'b': case 'B': case 'h': case 'H': case 'i': case 'I':
      case 'l': case 'L': case 'q': case 'Q':
      case 'f': case 'd': case 'g':
      case 'O': case 's': case 'p':
        if (ctx->enc_type == *ts && got_Z == ctx->is_complex &&
            ctx->enc_packmode == ctx->new_packmode) {
          ctx->enc_count += ctx->new_count;
        } else {
          if (__Pyx_BufFmt_ProcessTypeChunk(ctx) == -1) return NULL;
          ctx->enc_count = ctx->new_count;
          ctx->enc_packmode = ctx->new_packmode;
          ctx->enc_type = *ts;
          ctx->is_complex = got_Z;
        }
        ++ts;
        ctx->new_count = 1;
        got_Z = 0;
        break;
      case ':':
        ++ts;
        while(*ts != ':') ++ts;
        ++ts;
        break;
      case '(':
        if (!__pyx_buffmt_parse_array(ctx, &ts)) return NULL;
        break;
      default:
        {
          int number = __Pyx_BufFmt_ExpectNumber(&ts);
          if (number == -1) return NULL;
          ctx->new_count = (size_t)number;
        }
    }
  }
}
static CYTHON_INLINE void __Pyx_ZeroBuffer(Py_buffer* buf) {
  buf->buf = NULL;
  buf->obj = NULL;
  buf->strides = __Pyx_zeros;
  buf->shape = __Pyx_zeros;
  buf->suboffsets = __Pyx_minusones;
}
static CYTHON_INLINE int __Pyx_GetBufferAndValidate(
        Py_buffer* buf, PyObject* obj,  __Pyx_TypeInfo* dtype, int flags,
        int nd, int cast, __Pyx_BufFmt_StackElem* stack)
{
  if (obj == Py_None || obj == NULL) {
    __Pyx_ZeroBuffer(buf);
    return 0;
  }
  buf->buf = NULL;
  if (__Pyx_GetBuffer(obj, buf, flags) == -1) goto fail;
  if (buf->ndim != nd) {
    PyErr_Format(PyExc_ValueError,
                 "Buffer has wrong number of dimensions (expected %d, got %d)",
                 nd, buf->ndim);
    goto fail;
  }
  if (!cast) {
    __Pyx_BufFmt_Context ctx;
    __Pyx_BufFmt_Init(&ctx, stack, dtype);
    if (!__Pyx_BufFmt_CheckString(&ctx, buf->format)) goto fail;
  }
  if ((unsigned)buf->itemsize != dtype->size) {
    PyErr_Format(PyExc_ValueError,
      "Item size of buffer (%" CYTHON_FORMAT_SSIZE_T "d byte%s) does not match size of '%s' (%" CYTHON_FORMAT_SSIZE_T "d byte%s)",
      buf->itemsize, (buf->itemsize > 1) ? "s" : "",
      dtype->name, (Py_ssize_t)dtype->size, (dtype->size > 1) ? "s" : "");
    goto fail;
  }
  if (buf->suboffsets == NULL) buf->suboffsets = __Pyx_minusones;
  return 0;
fail:;
  __Pyx_ZeroBuffer(buf);
  return -1;
}
static CYTHON_INLINE void __Pyx_SafeReleaseBuffer(Py_buffer* info) {
  if (info->buf == NULL) return;
  if (info->suboffsets == __Pyx_minusones) info->suboffsets = NULL;
  __Pyx_ReleaseBuffer(info);
}

static CYTHON_INLINE int __Pyx_TypeTest(PyObject *obj, PyTypeObject *type) {
    if (unlikely(!type)) {
        PyErr_Format(PyExc_SystemError, "Missing type object");
        return 0;
    }
    if (likely(PyObject_TypeCheck(obj, type)))
        return 1;
    PyErr_Format(PyExc_TypeError, "Cannot convert %.200s to %.200s",
                 Py_TYPE(obj)->tp_name, type->tp_name);
    return 0;
}

static void __Pyx_RaiseBufferFallbackError(void) {
  PyErr_Format(PyExc_ValueError,
     "Buffer acquisition failed on assignment; and then reacquiring the old buffer failed too!");
}

static void __Pyx_RaiseBufferIndexError(int axis) {
  PyErr_Format(PyExc_IndexError,
     "Out of bounds on buffer access (axis %d)", axis);
}

static CYTHON_INLINE void __Pyx_RaiseTooManyValuesError(Py_ssize_t expected) {
    PyErr_Format(PyExc_ValueError,
                 "too many values to unpack (expected %" CYTHON_FORMAT_SSIZE_T "d)", expected);
}

static CYTHON_INLINE void __Pyx_RaiseNeedMoreValuesError(Py_ssize_t index) {
    PyErr_Format(PyExc_ValueError,
                 "need more than %" CYTHON_FORMAT_SSIZE_T "d value%s to unpack",
                 index, (index == 1) ? "" : "s");
}

static CYTHON_INLINE void __Pyx_RaiseNoneNotIterableError(void) {
    PyErr_SetString(PyExc_TypeError, "'NoneType' object is not iterable");
}

static CYTHON_INLINE int __Pyx_IterFinish(void) {
#if CYTHON_COMPILING_IN_CPYTHON
    PyThreadState *tstate = PyThreadState_GET();
    PyObject* exc_type = tstate->curexc_type;
    if (unlikely(exc_type)) {
        if (likely(exc_type == PyExc_StopIteration) || PyErr_GivenExceptionMatches(exc_type, PyExc_StopIteration)) {
            PyObject *exc_value, *exc_tb;
            exc_value = tstate->curexc_value;
            exc_tb = tstate->curexc_traceback;
            tstate->curexc_type = 0;
            tstate->curexc_value = 0;
            tstate->curexc_traceback = 0;
            Py_DECREF(exc_type);
            Py_XDECREF(exc_value);
            Py_XDECREF(exc_tb);
            return 0;
        } else {
            return -1;
        }
    }
    return 0;
#else
    if (unlikely(PyErr_Occurred())) {
        if (likely(PyErr_ExceptionMatches(PyExc_StopIteration))) {
            PyErr_Clear();
            return 0;
        } else {
            return -1;
        }
    }
    return 0;
#endif
}

static int __Pyx_IternextUnpackEndCheck(PyObject *retval, Py_ssize_t expected) {
    if (unlikely(retval)) {
        Py_DECREF(retval);
        __Pyx_RaiseTooManyValuesError(expected);
        return -1;
    } else {
        return __Pyx_IterFinish();
    }
    return 0;
}

static CYTHON_INLINE void __Pyx_ExceptionSave(PyObject **type, PyObject **value, PyObject **tb) {
#if CYTHON_COMPILING_IN_CPYTHON
    PyThreadState *tstate = PyThreadState_GET();
    *type = tstate->exc_type;
    *value = tstate->exc_value;
    *tb = tstate->exc_traceback;
    Py_XINCREF(*type);
    Py_XINCREF(*value);
    Py_XINCREF(*tb);
#else
    PyErr_GetExcInfo(type, value, tb);
#endif
}
static void __Pyx_ExceptionReset(PyObject *type, PyObject *value, PyObject *tb) {
#if CYTHON_COMPILING_IN_CPYTHON
    PyObject *tmp_type, *tmp_value, *tmp_tb;
    PyThreadState *tstate = PyThreadState_GET();
    tmp_type = tstate->exc_type;
    tmp_value = tstate->exc_value;
    tmp_tb = tstate->exc_traceback;
    tstate->exc_type = type;
    tstate->exc_value = value;
    tstate->exc_traceback = tb;
    Py_XDECREF(tmp_type);
    Py_XDECREF(tmp_value);
    Py_XDECREF(tmp_tb);
#else
    PyErr_SetExcInfo(type, value, tb);
#endif
}

#if PY_MAJOR_VERSION < 3
static int __Pyx_GetBuffer(PyObject *obj, Py_buffer *view, int flags) {
  #if PY_VERSION_HEX >= 0x02060000
    if (PyObject_CheckBuffer(obj)) return PyObject_GetBuffer(obj, view, flags);
  #endif
        if (PyObject_TypeCheck(obj, __pyx_ptype_5numpy_ndarray)) return __pyx_pw_5numpy_7ndarray_1__getbuffer__(obj, view, flags);
  #if PY_VERSION_HEX < 0x02060000
    if (obj->ob_type->tp_dict) {
        PyObject *getbuffer_cobj = PyObject_GetItem(
            obj->ob_type->tp_dict, __pyx_n_s____pyx_getbuffer);
        if (getbuffer_cobj) {
            getbufferproc func = (getbufferproc) PyCObject_AsVoidPtr(getbuffer_cobj);
            Py_DECREF(getbuffer_cobj);
            if (!func)
                goto fail;
            return func(obj, view, flags);
        } else {
            PyErr_Clear();
        }
    }
  #endif
    PyErr_Format(PyExc_TypeError, "'%100s' does not have the buffer interface", Py_TYPE(obj)->tp_name);
#if PY_VERSION_HEX < 0x02060000
fail:
#endif
    return -1;
}
static void __Pyx_ReleaseBuffer(Py_buffer *view) {
    PyObject *obj = view->obj;
    if (!obj) return;
  #if PY_VERSION_HEX >= 0x02060000
    if (PyObject_CheckBuffer(obj)) {
        PyBuffer_Release(view);
        return;
    }
  #endif
        if (PyObject_TypeCheck(obj, __pyx_ptype_5numpy_ndarray)) { __pyx_pw_5numpy_7ndarray_3__releasebuffer__(obj, view); return; }
  #if PY_VERSION_HEX < 0x02060000
    if (obj->ob_type->tp_dict) {
        PyObject *releasebuffer_cobj = PyObject_GetItem(
            obj->ob_type->tp_dict, __pyx_n_s____pyx_releasebuffer);
        if (releasebuffer_cobj) {
            releasebufferproc func = (releasebufferproc) PyCObject_AsVoidPtr(releasebuffer_cobj);
            Py_DECREF(releasebuffer_cobj);
            if (!func)
                goto fail;
            func(obj, view);
            return;
        } else {
            PyErr_Clear();
        }
    }
  #endif
    goto nofail;
#if PY_VERSION_HEX < 0x02060000
fail:
#endif
    PyErr_WriteUnraisable(obj);
nofail:
    Py_DECREF(obj);
    view->obj = NULL;
}
#endif /*  PY_MAJOR_VERSION < 3 */


          static PyObject *__Pyx_Import(PyObject *name, PyObject *from_list, int level) {
    PyObject *empty_list = 0;
    PyObject *module = 0;
    PyObject *global_dict = 0;
    PyObject *empty_dict = 0;
    PyObject *list;
    #if PY_VERSION_HEX < 0x03030000
    PyObject *py_import;
    py_import = __Pyx_PyObject_GetAttrStr(__pyx_b, __pyx_n_s____import__);
    if (!py_import)
        goto bad;
    #endif
    if (from_list)
        list = from_list;
    else {
        empty_list = PyList_New(0);
        if (!empty_list)
            goto bad;
        list = empty_list;
    }
    global_dict = PyModule_GetDict(__pyx_m);
    if (!global_dict)
        goto bad;
    empty_dict = PyDict_New();
    if (!empty_dict)
        goto bad;
    #if PY_VERSION_HEX >= 0x02050000
    {
        #if PY_MAJOR_VERSION >= 3
        if (level == -1) {
            if (strchr(__Pyx_MODULE_NAME, '.')) {
                #if PY_VERSION_HEX < 0x03030000
                PyObject *py_level = PyInt_FromLong(1);
                if (!py_level)
                    goto bad;
                module = PyObject_CallFunctionObjArgs(py_import,
                    name, global_dict, empty_dict, list, py_level, NULL);
                Py_DECREF(py_level);
                #else
                module = PyImport_ImportModuleLevelObject(
                    name, global_dict, empty_dict, list, 1);
                #endif
                if (!module) {
                    if (!PyErr_ExceptionMatches(PyExc_ImportError))
                        goto bad;
                    PyErr_Clear();
                }
            }
            level = 0; /* try absolute import on failure */
        }
        #endif
        if (!module) {
            #if PY_VERSION_HEX < 0x03030000
            PyObject *py_level = PyInt_FromLong(level);
            if (!py_level)
                goto bad;
            module = PyObject_CallFunctionObjArgs(py_import,
                name, global_dict, empty_dict, list, py_level, NULL);
            Py_DECREF(py_level);
            #else
            module = PyImport_ImportModuleLevelObject(
                name, global_dict, empty_dict, list, level);
            #endif
        }
    }
    #else
    if (level>0) {
        PyErr_SetString(PyExc_RuntimeError, "Relative import is not supported for Python <=2.4.");
        goto bad;
    }
    module = PyObject_CallFunctionObjArgs(py_import,
        name, global_dict, empty_dict, list, NULL);
    #endif
bad:
    #if PY_VERSION_HEX < 0x03030000
    Py_XDECREF(py_import);
    #endif
    Py_XDECREF(empty_list);
    Py_XDECREF(empty_dict);
    return module;
}

static CYTHON_INLINE PyObject *__Pyx_PyInt_to_py_Py_intptr_t(Py_intptr_t val) {
    const Py_intptr_t neg_one = (Py_intptr_t)-1, const_zero = (Py_intptr_t)0;
    const int is_unsigned = const_zero < neg_one;
    if ((sizeof(Py_intptr_t) == sizeof(char))  ||
        (sizeof(Py_intptr_t) == sizeof(short))) {
        return PyInt_FromLong((long)val);
    } else if ((sizeof(Py_intptr_t) == sizeof(int)) ||
               (sizeof(Py_intptr_t) == sizeof(long))) {
        if (is_unsigned)
            return PyLong_FromUnsignedLong((unsigned long)val);
        else
            return PyInt_FromLong((long)val);
    } else if (sizeof(Py_intptr_t) == sizeof(PY_LONG_LONG)) {
        if (is_unsigned)
            return PyLong_FromUnsignedLongLong((unsigned PY_LONG_LONG)val);
        else
            return PyLong_FromLongLong((PY_LONG_LONG)val);
    } else {
        int one = 1; int little = (int)*(unsigned char *)&one;
        unsigned char *bytes = (unsigned char *)&val;
        return _PyLong_FromByteArray(bytes, sizeof(Py_intptr_t),
                                     little, !is_unsigned);
    }
}

#if CYTHON_CCOMPLEX
  #ifdef __cplusplus
    static CYTHON_INLINE __pyx_t_float_complex __pyx_t_float_complex_from_parts(float x, float y) {
      return ::std::complex< float >(x, y);
    }
  #else
    static CYTHON_INLINE __pyx_t_float_complex __pyx_t_float_complex_from_parts(float x, float y) {
      return x + y*(__pyx_t_float_complex)_Complex_I;
    }
  #endif
#else
    static CYTHON_INLINE __pyx_t_float_complex __pyx_t_float_complex_from_parts(float x, float y) {
      __pyx_t_float_complex z;
      z.real = x;
      z.imag = y;
      return z;
    }
#endif

#if CYTHON_CCOMPLEX
#else
    static CYTHON_INLINE int __Pyx_c_eqf(__pyx_t_float_complex a, __pyx_t_float_complex b) {
       return (a.real == b.real) && (a.imag == b.imag);
    }
    static CYTHON_INLINE __pyx_t_float_complex __Pyx_c_sumf(__pyx_t_float_complex a, __pyx_t_float_complex b) {
        __pyx_t_float_complex z;
        z.real = a.real + b.real;
        z.imag = a.imag + b.imag;
        return z;
    }
    static CYTHON_INLINE __pyx_t_float_complex __Pyx_c_difff(__pyx_t_float_complex a, __pyx_t_float_complex b) {
        __pyx_t_float_complex z;
        z.real = a.real - b.real;
        z.imag = a.imag - b.imag;
        return z;
    }
    static CYTHON_INLINE __pyx_t_float_complex __Pyx_c_prodf(__pyx_t_float_complex a, __pyx_t_float_complex b) {
        __pyx_t_float_complex z;
        z.real = a.real * b.real - a.imag * b.imag;
        z.imag = a.real * b.imag + a.imag * b.real;
        return z;
    }
    static CYTHON_INLINE __pyx_t_float_complex __Pyx_c_quotf(__pyx_t_float_complex a, __pyx_t_float_complex b) {
        __pyx_t_float_complex z;
        float denom = b.real * b.real + b.imag * b.imag;
        z.real = (a.real * b.real + a.imag * b.imag) / denom;
        z.imag = (a.imag * b.real - a.real * b.imag) / denom;
        return z;
    }
    static CYTHON_INLINE __pyx_t_float_complex __Pyx_c_negf(__pyx_t_float_complex a) {
        __pyx_t_float_complex z;
        z.real = -a.real;
        z.imag = -a.imag;
        return z;
    }
    static CYTHON_INLINE int __Pyx_c_is_zerof(__pyx_t_float_complex a) {
       return (a.real == 0) && (a.imag == 0);
    }
    static CYTHON_INLINE __pyx_t_float_complex __Pyx_c_conjf(__pyx_t_float_complex a) {
        __pyx_t_float_complex z;
        z.real =  a.real;
        z.imag = -a.imag;
        return z;
    }
    #if 1
        static CYTHON_INLINE float __Pyx_c_absf(__pyx_t_float_complex z) {
          #if !defined(HAVE_HYPOT) || defined(_MSC_VER)
            return sqrtf(z.real*z.real + z.imag*z.imag);
          #else
            return hypotf(z.real, z.imag);
          #endif
        }
        static CYTHON_INLINE __pyx_t_float_complex __Pyx_c_powf(__pyx_t_float_complex a, __pyx_t_float_complex b) {
            __pyx_t_float_complex z;
            float r, lnr, theta, z_r, z_theta;
            if (b.imag == 0 && b.real == (int)b.real) {
                if (b.real < 0) {
                    float denom = a.real * a.real + a.imag * a.imag;
                    a.real = a.real / denom;
                    a.imag = -a.imag / denom;
                    b.real = -b.real;
                }
                switch ((int)b.real) {
                    case 0:
                        z.real = 1;
                        z.imag = 0;
                        return z;
                    case 1:
                        return a;
                    case 2:
                        z = __Pyx_c_prodf(a, a);
                        return __Pyx_c_prodf(a, a);
                    case 3:
                        z = __Pyx_c_prodf(a, a);
                        return __Pyx_c_prodf(z, a);
                    case 4:
                        z = __Pyx_c_prodf(a, a);
                        return __Pyx_c_prodf(z, z);
                }
            }
            if (a.imag == 0) {
                if (a.real == 0) {
                    return a;
                }
                r = a.real;
                theta = 0;
            } else {
                r = __Pyx_c_absf(a);
                theta = atan2f(a.imag, a.real);
            }
            lnr = logf(r);
            z_r = expf(lnr * b.real - theta * b.imag);
            z_theta = theta * b.real + lnr * b.imag;
            z.real = z_r * cosf(z_theta);
            z.imag = z_r * sinf(z_theta);
            return z;
        }
    #endif
#endif

#if CYTHON_CCOMPLEX
  #ifdef __cplusplus
    static CYTHON_INLINE __pyx_t_double_complex __pyx_t_double_complex_from_parts(double x, double y) {
      return ::std::complex< double >(x, y);
    }
  #else
    static CYTHON_INLINE __pyx_t_double_complex __pyx_t_double_complex_from_parts(double x, double y) {
      return x + y*(__pyx_t_double_complex)_Complex_I;
    }
  #endif
#else
    static CYTHON_INLINE __pyx_t_double_complex __pyx_t_double_complex_from_parts(double x, double y) {
      __pyx_t_double_complex z;
      z.real = x;
      z.imag = y;
      return z;
    }
#endif

#if CYTHON_CCOMPLEX
#else
    static CYTHON_INLINE int __Pyx_c_eq(__pyx_t_double_complex a, __pyx_t_double_complex b) {
       return (a.real == b.real) && (a.imag == b.imag);
    }
    static CYTHON_INLINE __pyx_t_double_complex __Pyx_c_sum(__pyx_t_double_complex a, __pyx_t_double_complex b) {
        __pyx_t_double_complex z;
        z.real = a.real + b.real;
        z.imag = a.imag + b.imag;
        return z;
    }
    static CYTHON_INLINE __pyx_t_double_complex __Pyx_c_diff(__pyx_t_double_complex a, __pyx_t_double_complex b) {
        __pyx_t_double_complex z;
        z.real = a.real - b.real;
        z.imag = a.imag - b.imag;
        return z;
    }
    static CYTHON_INLINE __pyx_t_double_complex __Pyx_c_prod(__pyx_t_double_complex a, __pyx_t_double_complex b) {
        __pyx_t_double_complex z;
        z.real = a.real * b.real - a.imag * b.imag;
        z.imag = a.real * b.imag + a.imag * b.real;
        return z;
    }
    static CYTHON_INLINE __pyx_t_double_complex __Pyx_c_quot(__pyx_t_double_complex a, __pyx_t_double_complex b) {
        __pyx_t_double_complex z;
        double denom = b.real * b.real + b.imag * b.imag;
        z.real = (a.real * b.real + a.imag * b.imag) / denom;
        z.imag = (a.imag * b.real - a.real * b.imag) / denom;
        return z;
    }
    static CYTHON_INLINE __pyx_t_double_complex __Pyx_c_neg(__pyx_t_double_complex a) {
        __pyx_t_double_complex z;
        z.real = -a.real;
        z.imag = -a.imag;
        return z;
    }
    static CYTHON_INLINE int __Pyx_c_is_zero(__pyx_t_double_complex a) {
       return (a.real == 0) && (a.imag == 0);
    }
    static CYTHON_INLINE __pyx_t_double_complex __Pyx_c_conj(__pyx_t_double_complex a) {
        __pyx_t_double_complex z;
        z.real =  a.real;
        z.imag = -a.imag;
        return z;
    }
    #if 1
        static CYTHON_INLINE double __Pyx_c_abs(__pyx_t_double_complex z) {
          #if !defined(HAVE_HYPOT) || defined(_MSC_VER)
            return sqrt(z.real*z.real + z.imag*z.imag);
          #else
            return hypot(z.real, z.imag);
          #endif
        }
        static CYTHON_INLINE __pyx_t_double_complex __Pyx_c_pow(__pyx_t_double_complex a, __pyx_t_double_complex b) {
            __pyx_t_double_complex z;
            double r, lnr, theta, z_r, z_theta;
            if (b.imag == 0 && b.real == (int)b.real) {
                if (b.real < 0) {
                    double denom = a.real * a.real + a.imag * a.imag;
                    a.real = a.real / denom;
                    a.imag = -a.imag / denom;
                    b.real = -b.real;
                }
                switch ((int)b.real) {
                    case 0:
                        z.real = 1;
                        z.imag = 0;
                        return z;
                    case 1:
                        return a;
                    case 2:
                        z = __Pyx_c_prod(a, a);
                        return __Pyx_c_prod(a, a);
                    case 3:
                        z = __Pyx_c_prod(a, a);
                        return __Pyx_c_prod(z, a);
                    case 4:
                        z = __Pyx_c_prod(a, a);
                        return __Pyx_c_prod(z, z);
                }
            }
            if (a.imag == 0) {
                if (a.real == 0) {
                    return a;
                }
                r = a.real;
                theta = 0;
            } else {
                r = __Pyx_c_abs(a);
                theta = atan2(a.imag, a.real);
            }
            lnr = log(r);
            z_r = exp(lnr * b.real - theta * b.imag);
            z_theta = theta * b.real + lnr * b.imag;
            z.real = z_r * cos(z_theta);
            z.imag = z_r * sin(z_theta);
            return z;
        }
    #endif
#endif

static CYTHON_INLINE unsigned char __Pyx_PyInt_AsUnsignedChar(PyObject* x) {
    const unsigned char neg_one = (unsigned char)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(unsigned char) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(unsigned char)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to unsigned char" :
                    "value too large to convert to unsigned char");
            }
            return (unsigned char)-1;
        }
        return (unsigned char)val;
    }
    return (unsigned char)__Pyx_PyInt_AsUnsignedLong(x);
}

static CYTHON_INLINE unsigned short __Pyx_PyInt_AsUnsignedShort(PyObject* x) {
    const unsigned short neg_one = (unsigned short)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(unsigned short) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(unsigned short)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to unsigned short" :
                    "value too large to convert to unsigned short");
            }
            return (unsigned short)-1;
        }
        return (unsigned short)val;
    }
    return (unsigned short)__Pyx_PyInt_AsUnsignedLong(x);
}

static CYTHON_INLINE unsigned int __Pyx_PyInt_AsUnsignedInt(PyObject* x) {
    const unsigned int neg_one = (unsigned int)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(unsigned int) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(unsigned int)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to unsigned int" :
                    "value too large to convert to unsigned int");
            }
            return (unsigned int)-1;
        }
        return (unsigned int)val;
    }
    return (unsigned int)__Pyx_PyInt_AsUnsignedLong(x);
}

static CYTHON_INLINE char __Pyx_PyInt_AsChar(PyObject* x) {
    const char neg_one = (char)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(char) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(char)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to char" :
                    "value too large to convert to char");
            }
            return (char)-1;
        }
        return (char)val;
    }
    return (char)__Pyx_PyInt_AsLong(x);
}

static CYTHON_INLINE short __Pyx_PyInt_AsShort(PyObject* x) {
    const short neg_one = (short)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(short) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(short)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to short" :
                    "value too large to convert to short");
            }
            return (short)-1;
        }
        return (short)val;
    }
    return (short)__Pyx_PyInt_AsLong(x);
}

static CYTHON_INLINE int __Pyx_PyInt_AsInt(PyObject* x) {
    const int neg_one = (int)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(int) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(int)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to int" :
                    "value too large to convert to int");
            }
            return (int)-1;
        }
        return (int)val;
    }
    return (int)__Pyx_PyInt_AsLong(x);
}

static CYTHON_INLINE signed char __Pyx_PyInt_AsSignedChar(PyObject* x) {
    const signed char neg_one = (signed char)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(signed char) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(signed char)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to signed char" :
                    "value too large to convert to signed char");
            }
            return (signed char)-1;
        }
        return (signed char)val;
    }
    return (signed char)__Pyx_PyInt_AsSignedLong(x);
}

static CYTHON_INLINE signed short __Pyx_PyInt_AsSignedShort(PyObject* x) {
    const signed short neg_one = (signed short)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(signed short) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(signed short)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to signed short" :
                    "value too large to convert to signed short");
            }
            return (signed short)-1;
        }
        return (signed short)val;
    }
    return (signed short)__Pyx_PyInt_AsSignedLong(x);
}

static CYTHON_INLINE signed int __Pyx_PyInt_AsSignedInt(PyObject* x) {
    const signed int neg_one = (signed int)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(signed int) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(signed int)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to signed int" :
                    "value too large to convert to signed int");
            }
            return (signed int)-1;
        }
        return (signed int)val;
    }
    return (signed int)__Pyx_PyInt_AsSignedLong(x);
}

static CYTHON_INLINE int __Pyx_PyInt_AsLongDouble(PyObject* x) {
    const int neg_one = (int)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(int) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(int)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to int" :
                    "value too large to convert to int");
            }
            return (int)-1;
        }
        return (int)val;
    }
    return (int)__Pyx_PyInt_AsLong(x);
}

#if CYTHON_COMPILING_IN_CPYTHON && PY_MAJOR_VERSION >= 3
#if CYTHON_USE_PYLONG_INTERNALS
#include "longintrepr.h"
#endif
#endif
static CYTHON_INLINE unsigned long __Pyx_PyInt_AsUnsignedLong(PyObject* x) {
    const unsigned long neg_one = (unsigned long)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
#if PY_MAJOR_VERSION < 3
    if (likely(PyInt_Check(x))) {
        long val = PyInt_AS_LONG(x);
        if (is_unsigned && unlikely(val < 0)) {
            PyErr_SetString(PyExc_OverflowError,
                            "can't convert negative value to unsigned long");
            return (unsigned long)-1;
        }
        return (unsigned long)val;
    } else
#endif
    if (likely(PyLong_Check(x))) {
        if (is_unsigned) {
#if CYTHON_COMPILING_IN_CPYTHON && PY_MAJOR_VERSION >= 3
#if CYTHON_USE_PYLONG_INTERNALS
            if (sizeof(digit) <= sizeof(unsigned long)) {
                switch (Py_SIZE(x)) {
                    case  0: return 0;
                    case  1: return (unsigned long) ((PyLongObject*)x)->ob_digit[0];
                }
            }
#endif
#endif
            if (unlikely(Py_SIZE(x) < 0)) {
                PyErr_SetString(PyExc_OverflowError,
                                "can't convert negative value to unsigned long");
                return (unsigned long)-1;
            }
            return (unsigned long)PyLong_AsUnsignedLong(x);
        } else {
#if CYTHON_COMPILING_IN_CPYTHON && PY_MAJOR_VERSION >= 3
#if CYTHON_USE_PYLONG_INTERNALS
            if (sizeof(digit) <= sizeof(unsigned long)) {
                switch (Py_SIZE(x)) {
                    case  0: return 0;
                    case  1: return +(unsigned long) ((PyLongObject*)x)->ob_digit[0];
                    case -1: return -(unsigned long) ((PyLongObject*)x)->ob_digit[0];
                }
            }
#endif
#endif
            return (unsigned long)PyLong_AsLong(x);
        }
    } else {
        unsigned long val;
        PyObject *tmp = __Pyx_PyNumber_Int(x);
        if (!tmp) return (unsigned long)-1;
        val = __Pyx_PyInt_AsUnsignedLong(tmp);
        Py_DECREF(tmp);
        return val;
    }
}

#if CYTHON_COMPILING_IN_CPYTHON && PY_MAJOR_VERSION >= 3
#if CYTHON_USE_PYLONG_INTERNALS
#include "longintrepr.h"
#endif
#endif
static CYTHON_INLINE unsigned PY_LONG_LONG __Pyx_PyInt_AsUnsignedLongLong(PyObject* x) {
    const unsigned PY_LONG_LONG neg_one = (unsigned PY_LONG_LONG)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
#if PY_MAJOR_VERSION < 3
    if (likely(PyInt_Check(x))) {
        long val = PyInt_AS_LONG(x);
        if (is_unsigned && unlikely(val < 0)) {
            PyErr_SetString(PyExc_OverflowError,
                            "can't convert negative value to unsigned PY_LONG_LONG");
            return (unsigned PY_LONG_LONG)-1;
        }
        return (unsigned PY_LONG_LONG)val;
    } else
#endif
    if (likely(PyLong_Check(x))) {
        if (is_unsigned) {
#if CYTHON_COMPILING_IN_CPYTHON && PY_MAJOR_VERSION >= 3
#if CYTHON_USE_PYLONG_INTERNALS
            if (sizeof(digit) <= sizeof(unsigned PY_LONG_LONG)) {
                switch (Py_SIZE(x)) {
                    case  0: return 0;
                    case  1: return (unsigned PY_LONG_LONG) ((PyLongObject*)x)->ob_digit[0];
                }
            }
#endif
#endif
            if (unlikely(Py_SIZE(x) < 0)) {
                PyErr_SetString(PyExc_OverflowError,
                                "can't convert negative value to unsigned PY_LONG_LONG");
                return (unsigned PY_LONG_LONG)-1;
            }
            return (unsigned PY_LONG_LONG)PyLong_AsUnsignedLongLong(x);
        } else {
#if CYTHON_COMPILING_IN_CPYTHON && PY_MAJOR_VERSION >= 3
#if CYTHON_USE_PYLONG_INTERNALS
            if (sizeof(digit) <= sizeof(unsigned PY_LONG_LONG)) {
                switch (Py_SIZE(x)) {
                    case  0: return 0;
                    case  1: return +(unsigned PY_LONG_LONG) ((PyLongObject*)x)->ob_digit[0];
                    case -1: return -(unsigned PY_LONG_LONG) ((PyLongObject*)x)->ob_digit[0];
                }
            }
#endif
#endif
            return (unsigned PY_LONG_LONG)PyLong_AsLongLong(x);
        }
    } else {
        unsigned PY_LONG_LONG val;
        PyObject *tmp = __Pyx_PyNumber_Int(x);
        if (!tmp) return (unsigned PY_LONG_LONG)-1;
        val = __Pyx_PyInt_AsUnsignedLongLong(tmp);
        Py_DECREF(tmp);
        return val;
    }
}

#if CYTHON_COMPILING_IN_CPYTHON && PY_MAJOR_VERSION >= 3
#if CYTHON_USE_PYLONG_INTERNALS
#include "longintrepr.h"
#endif
#endif
static CYTHON_INLINE long __Pyx_PyInt_AsLong(PyObject* x) {
    const long neg_one = (long)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
#if PY_MAJOR_VERSION < 3
    if (likely(PyInt_Check(x))) {
        long val = PyInt_AS_LONG(x);
        if (is_unsigned && unlikely(val < 0)) {
            PyErr_SetString(PyExc_OverflowError,
                            "can't convert negative value to long");
            return (long)-1;
        }
        return (long)val;
    } else
#endif
    if (likely(PyLong_Check(x))) {
        if (is_unsigned) {
#if CYTHON_COMPILING_IN_CPYTHON && PY_MAJOR_VERSION >= 3
#if CYTHON_USE_PYLONG_INTERNALS
            if (sizeof(digit) <= sizeof(long)) {
                switch (Py_SIZE(x)) {
                    case  0: return 0;
                    case  1: return (long) ((PyLongObject*)x)->ob_digit[0];
                }
            }
#endif
#endif
            if (unlikely(Py_SIZE(x) < 0)) {
                PyErr_SetString(PyExc_OverflowError,
                                "can't convert negative value to long");
                return (long)-1;
            }
            return (long)PyLong_AsUnsignedLong(x);
        } else {
#if CYTHON_COMPILING_IN_CPYTHON && PY_MAJOR_VERSION >= 3
#if CYTHON_USE_PYLONG_INTERNALS
            if (sizeof(digit) <= sizeof(long)) {
                switch (Py_SIZE(x)) {
                    case  0: return 0;
                    case  1: return +(long) ((PyLongObject*)x)->ob_digit[0];
                    case -1: return -(long) ((PyLongObject*)x)->ob_digit[0];
                }
            }
#endif
#endif
            return (long)PyLong_AsLong(x);
        }
    } else {
        long val;
        PyObject *tmp = __Pyx_PyNumber_Int(x);
        if (!tmp) return (long)-1;
        val = __Pyx_PyInt_AsLong(tmp);
        Py_DECREF(tmp);
        return val;
    }
}

#if CYTHON_COMPILING_IN_CPYTHON && PY_MAJOR_VERSION >= 3
#if CYTHON_USE_PYLONG_INTERNALS
#include "longintrepr.h"
#endif
#endif
static CYTHON_INLINE PY_LONG_LONG __Pyx_PyInt_AsLongLong(PyObject* x) {
    const PY_LONG_LONG neg_one = (PY_LONG_LONG)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
#if PY_MAJOR_VERSION < 3
    if (likely(PyInt_Check(x))) {
        long val = PyInt_AS_LONG(x);
        if (is_unsigned && unlikely(val < 0)) {
            PyErr_SetString(PyExc_OverflowError,
                            "can't convert negative value to PY_LONG_LONG");
            return (PY_LONG_LONG)-1;
        }
        return (PY_LONG_LONG)val;
    } else
#endif
    if (likely(PyLong_Check(x))) {
        if (is_unsigned) {
#if CYTHON_COMPILING_IN_CPYTHON && PY_MAJOR_VERSION >= 3
#if CYTHON_USE_PYLONG_INTERNALS
            if (sizeof(digit) <= sizeof(PY_LONG_LONG)) {
                switch (Py_SIZE(x)) {
                    case  0: return 0;
                    case  1: return (PY_LONG_LONG) ((PyLongObject*)x)->ob_digit[0];
                }
            }
#endif
#endif
            if (unlikely(Py_SIZE(x) < 0)) {
                PyErr_SetString(PyExc_OverflowError,
                                "can't convert negative value to PY_LONG_LONG");
                return (PY_LONG_LONG)-1;
            }
            return (PY_LONG_LONG)PyLong_AsUnsignedLongLong(x);
        } else {
#if CYTHON_COMPILING_IN_CPYTHON && PY_MAJOR_VERSION >= 3
#if CYTHON_USE_PYLONG_INTERNALS
            if (sizeof(digit) <= sizeof(PY_LONG_LONG)) {
                switch (Py_SIZE(x)) {
                    case  0: return 0;
                    case  1: return +(PY_LONG_LONG) ((PyLongObject*)x)->ob_digit[0];
                    case -1: return -(PY_LONG_LONG) ((PyLongObject*)x)->ob_digit[0];
                }
            }
#endif
#endif
            return (PY_LONG_LONG)PyLong_AsLongLong(x);
        }
    } else {
        PY_LONG_LONG val;
        PyObject *tmp = __Pyx_PyNumber_Int(x);
        if (!tmp) return (PY_LONG_LONG)-1;
        val = __Pyx_PyInt_AsLongLong(tmp);
        Py_DECREF(tmp);
        return val;
    }
}

#if CYTHON_COMPILING_IN_CPYTHON && PY_MAJOR_VERSION >= 3
#if CYTHON_USE_PYLONG_INTERNALS
#include "longintrepr.h"
#endif
#endif
static CYTHON_INLINE signed long __Pyx_PyInt_AsSignedLong(PyObject* x) {
    const signed long neg_one = (signed long)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
#if PY_MAJOR_VERSION < 3
    if (likely(PyInt_Check(x))) {
        long val = PyInt_AS_LONG(x);
        if (is_unsigned && unlikely(val < 0)) {
            PyErr_SetString(PyExc_OverflowError,
                            "can't convert negative value to signed long");
            return (signed long)-1;
        }
        return (signed long)val;
    } else
#endif
    if (likely(PyLong_Check(x))) {
        if (is_unsigned) {
#if CYTHON_COMPILING_IN_CPYTHON && PY_MAJOR_VERSION >= 3
#if CYTHON_USE_PYLONG_INTERNALS
            if (sizeof(digit) <= sizeof(signed long)) {
                switch (Py_SIZE(x)) {
                    case  0: return 0;
                    case  1: return (signed long) ((PyLongObject*)x)->ob_digit[0];
                }
            }
#endif
#endif
            if (unlikely(Py_SIZE(x) < 0)) {
                PyErr_SetString(PyExc_OverflowError,
                                "can't convert negative value to signed long");
                return (signed long)-1;
            }
            return (signed long)PyLong_AsUnsignedLong(x);
        } else {
#if CYTHON_COMPILING_IN_CPYTHON && PY_MAJOR_VERSION >= 3
#if CYTHON_USE_PYLONG_INTERNALS
            if (sizeof(digit) <= sizeof(signed long)) {
                switch (Py_SIZE(x)) {
                    case  0: return 0;
                    case  1: return +(signed long) ((PyLongObject*)x)->ob_digit[0];
                    case -1: return -(signed long) ((PyLongObject*)x)->ob_digit[0];
                }
            }
#endif
#endif
            return (signed long)PyLong_AsLong(x);
        }
    } else {
        signed long val;
        PyObject *tmp = __Pyx_PyNumber_Int(x);
        if (!tmp) return (signed long)-1;
        val = __Pyx_PyInt_AsSignedLong(tmp);
        Py_DECREF(tmp);
        return val;
    }
}

#if CYTHON_COMPILING_IN_CPYTHON && PY_MAJOR_VERSION >= 3
#if CYTHON_USE_PYLONG_INTERNALS
#include "longintrepr.h"
#endif
#endif
static CYTHON_INLINE signed PY_LONG_LONG __Pyx_PyInt_AsSignedLongLong(PyObject* x) {
    const signed PY_LONG_LONG neg_one = (signed PY_LONG_LONG)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
#if PY_MAJOR_VERSION < 3
    if (likely(PyInt_Check(x))) {
        long val = PyInt_AS_LONG(x);
        if (is_unsigned && unlikely(val < 0)) {
            PyErr_SetString(PyExc_OverflowError,
                            "can't convert negative value to signed PY_LONG_LONG");
            return (signed PY_LONG_LONG)-1;
        }
        return (signed PY_LONG_LONG)val;
    } else
#endif
    if (likely(PyLong_Check(x))) {
        if (is_unsigned) {
#if CYTHON_COMPILING_IN_CPYTHON && PY_MAJOR_VERSION >= 3
#if CYTHON_USE_PYLONG_INTERNALS
            if (sizeof(digit) <= sizeof(signed PY_LONG_LONG)) {
                switch (Py_SIZE(x)) {
                    case  0: return 0;
                    case  1: return (signed PY_LONG_LONG) ((PyLongObject*)x)->ob_digit[0];
                }
            }
#endif
#endif
            if (unlikely(Py_SIZE(x) < 0)) {
                PyErr_SetString(PyExc_OverflowError,
                                "can't convert negative value to signed PY_LONG_LONG");
                return (signed PY_LONG_LONG)-1;
            }
            return (signed PY_LONG_LONG)PyLong_AsUnsignedLongLong(x);
        } else {
#if CYTHON_COMPILING_IN_CPYTHON && PY_MAJOR_VERSION >= 3
#if CYTHON_USE_PYLONG_INTERNALS
            if (sizeof(digit) <= sizeof(signed PY_LONG_LONG)) {
                switch (Py_SIZE(x)) {
                    case  0: return 0;
                    case  1: return +(signed PY_LONG_LONG) ((PyLongObject*)x)->ob_digit[0];
                    case -1: return -(signed PY_LONG_LONG) ((PyLongObject*)x)->ob_digit[0];
                }
            }
#endif
#endif
            return (signed PY_LONG_LONG)PyLong_AsLongLong(x);
        }
    } else {
        signed PY_LONG_LONG val;
        PyObject *tmp = __Pyx_PyNumber_Int(x);
        if (!tmp) return (signed PY_LONG_LONG)-1;
        val = __Pyx_PyInt_AsSignedLongLong(tmp);
        Py_DECREF(tmp);
        return val;
    }
}

static int __Pyx_check_binary_version(void) {
    char ctversion[4], rtversion[4];
    PyOS_snprintf(ctversion, 4, "%d.%d", PY_MAJOR_VERSION, PY_MINOR_VERSION);
    PyOS_snprintf(rtversion, 4, "%s", Py_GetVersion());
    if (ctversion[0] != rtversion[0] || ctversion[2] != rtversion[2]) {
        char message[200];
        PyOS_snprintf(message, sizeof(message),
                      "compiletime version %s of module '%.100s' "
                      "does not match runtime version %s",
                      ctversion, __Pyx_MODULE_NAME, rtversion);
        #if PY_VERSION_HEX < 0x02050000
        return PyErr_Warn(NULL, message);
        #else
        return PyErr_WarnEx(NULL, message, 1);
        #endif
    }
    return 0;
}

#ifndef __PYX_HAVE_RT_ImportModule
#define __PYX_HAVE_RT_ImportModule
static PyObject *__Pyx_ImportModule(const char *name) {
    PyObject *py_name = 0;
    PyObject *py_module = 0;
    py_name = __Pyx_PyIdentifier_FromString(name);
    if (!py_name)
        goto bad;
    py_module = PyImport_Import(py_name);
    Py_DECREF(py_name);
    return py_module;
bad:
    Py_XDECREF(py_name);
    return 0;
}
#endif

#ifndef __PYX_HAVE_RT_ImportType
#define __PYX_HAVE_RT_ImportType
static PyTypeObject *__Pyx_ImportType(const char *module_name, const char *class_name,
    size_t size, int strict)
{
    PyObject *py_module = 0;
    PyObject *result = 0;
    PyObject *py_name = 0;
    char warning[200];
    Py_ssize_t basicsize;
#ifdef Py_LIMITED_API
    PyObject *py_basicsize;
#endif
    py_module = __Pyx_ImportModule(module_name);
    if (!py_module)
        goto bad;
    py_name = __Pyx_PyIdentifier_FromString(class_name);
    if (!py_name)
        goto bad;
    result = PyObject_GetAttr(py_module, py_name);
    Py_DECREF(py_name);
    py_name = 0;
    Py_DECREF(py_module);
    py_module = 0;
    if (!result)
        goto bad;
    if (!PyType_Check(result)) {
        PyErr_Format(PyExc_TypeError,
            "%s.%s is not a type object",
            module_name, class_name);
        goto bad;
    }
#ifndef Py_LIMITED_API
    basicsize = ((PyTypeObject *)result)->tp_basicsize;
#else
    py_basicsize = PyObject_GetAttrString(result, "__basicsize__");
    if (!py_basicsize)
        goto bad;
    basicsize = PyLong_AsSsize_t(py_basicsize);
    Py_DECREF(py_basicsize);
    py_basicsize = 0;
    if (basicsize == (Py_ssize_t)-1 && PyErr_Occurred())
        goto bad;
#endif
    if (!strict && (size_t)basicsize > size) {
        PyOS_snprintf(warning, sizeof(warning),
            "%s.%s size changed, may indicate binary incompatibility",
            module_name, class_name);
        #if PY_VERSION_HEX < 0x02050000
        if (PyErr_Warn(NULL, warning) < 0) goto bad;
        #else
        if (PyErr_WarnEx(NULL, warning, 0) < 0) goto bad;
        #endif
    }
    else if ((size_t)basicsize != size) {
        PyErr_Format(PyExc_ValueError,
            "%s.%s has the wrong size, try recompiling",
            module_name, class_name);
        goto bad;
    }
    return (PyTypeObject *)result;
bad:
    Py_XDECREF(py_module);
    Py_XDECREF(result);
    return NULL;
}
#endif

static int __pyx_bisect_code_objects(__Pyx_CodeObjectCacheEntry* entries, int count, int code_line) {
    int start = 0, mid = 0, end = count - 1;
    if (end >= 0 && code_line > entries[end].code_line) {
        return count;
    }
    while (start < end) {
        mid = (start + end) / 2;
        if (code_line < entries[mid].code_line) {
            end = mid;
        } else if (code_line > entries[mid].code_line) {
             start = mid + 1;
        } else {
            return mid;
        }
    }
    if (code_line <= entries[mid].code_line) {
        return mid;
    } else {
        return mid + 1;
    }
}
static PyCodeObject *__pyx_find_code_object(int code_line) {
    PyCodeObject* code_object;
    int pos;
    if (unlikely(!code_line) || unlikely(!__pyx_code_cache.entries)) {
        return NULL;
    }
    pos = __pyx_bisect_code_objects(__pyx_code_cache.entries, __pyx_code_cache.count, code_line);
    if (unlikely(pos >= __pyx_code_cache.count) || unlikely(__pyx_code_cache.entries[pos].code_line != code_line)) {
        return NULL;
    }
    code_object = __pyx_code_cache.entries[pos].code_object;
    Py_INCREF(code_object);
    return code_object;
}
static void __pyx_insert_code_object(int code_line, PyCodeObject* code_object) {
    int pos, i;
    __Pyx_CodeObjectCacheEntry* entries = __pyx_code_cache.entries;
    if (unlikely(!code_line)) {
        return;
    }
    if (unlikely(!entries)) {
        entries = (__Pyx_CodeObjectCacheEntry*)PyMem_Malloc(64*sizeof(__Pyx_CodeObjectCacheEntry));
        if (likely(entries)) {
            __pyx_code_cache.entries = entries;
            __pyx_code_cache.max_count = 64;
            __pyx_code_cache.count = 1;
            entries[0].code_line = code_line;
            entries[0].code_object = code_object;
            Py_INCREF(code_object);
        }
        return;
    }
    pos = __pyx_bisect_code_objects(__pyx_code_cache.entries, __pyx_code_cache.count, code_line);
    if ((pos < __pyx_code_cache.count) && unlikely(__pyx_code_cache.entries[pos].code_line == code_line)) {
        PyCodeObject* tmp = entries[pos].code_object;
        entries[pos].code_object = code_object;
        Py_DECREF(tmp);
        return;
    }
    if (__pyx_code_cache.count == __pyx_code_cache.max_count) {
        int new_max = __pyx_code_cache.max_count + 64;
        entries = (__Pyx_CodeObjectCacheEntry*)PyMem_Realloc(
            __pyx_code_cache.entries, new_max*sizeof(__Pyx_CodeObjectCacheEntry));
        if (unlikely(!entries)) {
            return;
        }
        __pyx_code_cache.entries = entries;
        __pyx_code_cache.max_count = new_max;
    }
    for (i=__pyx_code_cache.count; i>pos; i--) {
        entries[i] = entries[i-1];
    }
    entries[pos].code_line = code_line;
    entries[pos].code_object = code_object;
    __pyx_code_cache.count++;
    Py_INCREF(code_object);
}

#include "compile.h"
#include "frameobject.h"
#include "traceback.h"
static PyCodeObject* __Pyx_CreateCodeObjectForTraceback(
            const char *funcname, int c_line,
            int py_line, const char *filename) {
    PyCodeObject *py_code = 0;
    PyObject *py_srcfile = 0;
    PyObject *py_funcname = 0;
    #if PY_MAJOR_VERSION < 3
    py_srcfile = PyString_FromString(filename);
    #else
    py_srcfile = PyUnicode_FromString(filename);
    #endif
    if (!py_srcfile) goto bad;
    if (c_line) {
        #if PY_MAJOR_VERSION < 3
        py_funcname = PyString_FromFormat( "%s (%s:%d)", funcname, __pyx_cfilenm, c_line);
        #else
        py_funcname = PyUnicode_FromFormat( "%s (%s:%d)", funcname, __pyx_cfilenm, c_line);
        #endif
    }
    else {
        #if PY_MAJOR_VERSION < 3
        py_funcname = PyString_FromString(funcname);
        #else
        py_funcname = PyUnicode_FromString(funcname);
        #endif
    }
    if (!py_funcname) goto bad;
    py_code = __Pyx_PyCode_New(
        0,            /*int argcount,*/
        0,            /*int kwonlyargcount,*/
        0,            /*int nlocals,*/
        0,            /*int stacksize,*/
        0,            /*int flags,*/
        __pyx_empty_bytes, /*PyObject *code,*/
        __pyx_empty_tuple, /*PyObject *consts,*/
        __pyx_empty_tuple, /*PyObject *names,*/
        __pyx_empty_tuple, /*PyObject *varnames,*/
        __pyx_empty_tuple, /*PyObject *freevars,*/
        __pyx_empty_tuple, /*PyObject *cellvars,*/
        py_srcfile,   /*PyObject *filename,*/
        py_funcname,  /*PyObject *name,*/
        py_line,      /*int firstlineno,*/
        __pyx_empty_bytes  /*PyObject *lnotab*/
    );
    Py_DECREF(py_srcfile);
    Py_DECREF(py_funcname);
    return py_code;
bad:
    Py_XDECREF(py_srcfile);
    Py_XDECREF(py_funcname);
    return NULL;
}
static void __Pyx_AddTraceback(const char *funcname, int c_line,
                               int py_line, const char *filename) {
    PyCodeObject *py_code = 0;
    PyObject *py_globals = 0;
    PyFrameObject *py_frame = 0;
    py_code = __pyx_find_code_object(c_line ? c_line : py_line);
    if (!py_code) {
        py_code = __Pyx_CreateCodeObjectForTraceback(
            funcname, c_line, py_line, filename);
        if (!py_code) goto bad;
        __pyx_insert_code_object(c_line ? c_line : py_line, py_code);
    }
    py_globals = PyModule_GetDict(__pyx_m);
    if (!py_globals) goto bad;
    py_frame = PyFrame_New(
        PyThreadState_GET(), /*PyThreadState *tstate,*/
        py_code,             /*PyCodeObject *code,*/
        py_globals,          /*PyObject *globals,*/
        0                    /*PyObject *locals*/
    );
    if (!py_frame) goto bad;
    py_frame->f_lineno = py_line;
    PyTraceBack_Here(py_frame);
bad:
    Py_XDECREF(py_code);
    Py_XDECREF(py_frame);
}

static int __Pyx_InitStrings(__Pyx_StringTabEntry *t) {
    while (t->p) {
        #if PY_MAJOR_VERSION < 3
        if (t->is_unicode) {
            *t->p = PyUnicode_DecodeUTF8(t->s, t->n - 1, NULL);
        } else if (t->intern) {
            *t->p = PyString_InternFromString(t->s);
        } else {
            *t->p = PyString_FromStringAndSize(t->s, t->n - 1);
        }
        #else  /* Python 3+ has unicode identifiers */
        if (t->is_unicode | t->is_str) {
            if (t->intern) {
                *t->p = PyUnicode_InternFromString(t->s);
            } else if (t->encoding) {
                *t->p = PyUnicode_Decode(t->s, t->n - 1, t->encoding, NULL);
            } else {
                *t->p = PyUnicode_FromStringAndSize(t->s, t->n - 1);
            }
        } else {
            *t->p = PyBytes_FromStringAndSize(t->s, t->n - 1);
        }
        #endif
        if (!*t->p)
            return -1;
        ++t;
    }
    return 0;
}

static CYTHON_INLINE PyObject* __Pyx_PyUnicode_FromString(char* c_str) {
    return __Pyx_PyUnicode_FromStringAndSize(c_str, strlen(c_str));
}
static CYTHON_INLINE char* __Pyx_PyObject_AsString(PyObject* o) {
    Py_ssize_t ignore;
    return __Pyx_PyObject_AsStringAndSize(o, &ignore);
}
static CYTHON_INLINE char* __Pyx_PyObject_AsStringAndSize(PyObject* o, Py_ssize_t *length) {
#if __PYX_DEFAULT_STRING_ENCODING_IS_ASCII || __PYX_DEFAULT_STRING_ENCODING_IS_DEFAULT
    if (
#if PY_MAJOR_VERSION < 3 && __PYX_DEFAULT_STRING_ENCODING_IS_ASCII
            __Pyx_sys_getdefaultencoding_not_ascii &&
#endif
            PyUnicode_Check(o)) {
#if PY_VERSION_HEX < 0x03030000
        char* defenc_c;
        PyObject* defenc = _PyUnicode_AsDefaultEncodedString(o, NULL);
        if (!defenc) return NULL;
        defenc_c = PyBytes_AS_STRING(defenc);
#if __PYX_DEFAULT_STRING_ENCODING_IS_ASCII
        {
            char* end = defenc_c + PyBytes_GET_SIZE(defenc);
            char* c;
            for (c = defenc_c; c < end; c++) {
                if ((unsigned char) (*c) >= 128) {
                    PyUnicode_AsASCIIString(o);
                    return NULL;
                }
            }
        }
#endif /*__PYX_DEFAULT_STRING_ENCODING_IS_ASCII*/
        *length = PyBytes_GET_SIZE(defenc);
        return defenc_c;
#else /* PY_VERSION_HEX < 0x03030000 */
        if (PyUnicode_READY(o) == -1) return NULL;
#if __PYX_DEFAULT_STRING_ENCODING_IS_ASCII
        if (PyUnicode_IS_ASCII(o)) {
            *length = PyUnicode_GET_DATA_SIZE(o);
            return PyUnicode_AsUTF8(o);
        } else {
            PyUnicode_AsASCIIString(o);
            return NULL;
        }
#else /* __PYX_DEFAULT_STRING_ENCODING_IS_ASCII */
        return PyUnicode_AsUTF8AndSize(o, length);
#endif /* __PYX_DEFAULT_STRING_ENCODING_IS_ASCII */
#endif /* PY_VERSION_HEX < 0x03030000 */
    } else
#endif /* __PYX_DEFAULT_STRING_ENCODING_IS_ASCII  || __PYX_DEFAULT_STRING_ENCODING_IS_DEFAULT */
    {
        char* result;
        int r = PyBytes_AsStringAndSize(o, &result, length);
        if (r < 0) {
            return NULL;
        } else {
            return result;
        }
    }
}
static CYTHON_INLINE int __Pyx_PyObject_IsTrue(PyObject* x) {
   int is_true = x == Py_True;
   if (is_true | (x == Py_False) | (x == Py_None)) return is_true;
   else return PyObject_IsTrue(x);
}
static CYTHON_INLINE PyObject* __Pyx_PyNumber_Int(PyObject* x) {
  PyNumberMethods *m;
  const char *name = NULL;
  PyObject *res = NULL;
#if PY_MAJOR_VERSION < 3
  if (PyInt_Check(x) || PyLong_Check(x))
#else
  if (PyLong_Check(x))
#endif
    return Py_INCREF(x), x;
  m = Py_TYPE(x)->tp_as_number;
#if PY_MAJOR_VERSION < 3
  if (m && m->nb_int) {
    name = "int";
    res = PyNumber_Int(x);
  }
  else if (m && m->nb_long) {
    name = "long";
    res = PyNumber_Long(x);
  }
#else
  if (m && m->nb_int) {
    name = "int";
    res = PyNumber_Long(x);
  }
#endif
  if (res) {
#if PY_MAJOR_VERSION < 3
    if (!PyInt_Check(res) && !PyLong_Check(res)) {
#else
    if (!PyLong_Check(res)) {
#endif
      PyErr_Format(PyExc_TypeError,
                   "__%s__ returned non-%s (type %.200s)",
                   name, name, Py_TYPE(res)->tp_name);
      Py_DECREF(res);
      return NULL;
    }
  }
  else if (!PyErr_Occurred()) {
    PyErr_SetString(PyExc_TypeError,
                    "an integer is required");
  }
  return res;
}
static CYTHON_INLINE Py_ssize_t __Pyx_PyIndex_AsSsize_t(PyObject* b) {
  Py_ssize_t ival;
  PyObject* x = PyNumber_Index(b);
  if (!x) return -1;
  ival = PyInt_AsSsize_t(x);
  Py_DECREF(x);
  return ival;
}
static CYTHON_INLINE PyObject * __Pyx_PyInt_FromSize_t(size_t ival) {
#if PY_VERSION_HEX < 0x02050000
   if (ival <= LONG_MAX)
       return PyInt_FromLong((long)ival);
   else {
       unsigned char *bytes = (unsigned char *) &ival;
       int one = 1; int little = (int)*(unsigned char*)&one;
       return _PyLong_FromByteArray(bytes, sizeof(size_t), little, 0);
   }
#else
   return PyInt_FromSize_t(ival);
#endif
}
static CYTHON_INLINE size_t __Pyx_PyInt_AsSize_t(PyObject* x) {
   unsigned PY_LONG_LONG val = __Pyx_PyInt_AsUnsignedLongLong(x);
   if (unlikely(val != (unsigned PY_LONG_LONG)(size_t)val)) {
       if ((val != (unsigned PY_LONG_LONG)-1) || !PyErr_Occurred())
           PyErr_SetString(PyExc_OverflowError,
                           "value too large to convert to size_t");
       return (size_t)-1;
   }
   return (size_t)val;
}


#endif /* Py_PYTHON_H */
