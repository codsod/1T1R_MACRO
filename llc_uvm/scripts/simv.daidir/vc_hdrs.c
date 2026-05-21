#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <dlfcn.h>
#include "svdpi.h"

#ifdef __cplusplus
extern "C" {
#endif

/* VCS error reporting routine */
extern void vcsMsgReport1(const char *, const char *, int, void *, void*, const char *);

#ifndef _VC_TYPES_
#define _VC_TYPES_
/* common definitions shared with DirectC.h */

typedef unsigned int U;
typedef unsigned char UB;
typedef unsigned char scalar;
typedef struct { U c; U d;} vec32;

#define scalar_0 0
#define scalar_1 1
#define scalar_z 2
#define scalar_x 3

extern long long int ConvUP2LLI(U* a);
extern void ConvLLI2UP(long long int a1, U* a2);
extern long long int GetLLIresult();
extern void StoreLLIresult(const unsigned int* data);
typedef struct VeriC_Descriptor *vc_handle;

#ifndef SV_3_COMPATIBILITY
#define SV_STRING const char*
#else
#define SV_STRING char*
#endif

#endif /* _VC_TYPES_ */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_scope_add_logicvec_attribute
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_scope_add_logicvec_attribute
__attribute__((weak)) void fsdbTransDPI_scope_add_logicvec_attribute(/* OUTPUT */int *A_1, /* INPUT */const char* A_2, /* INPUT */const char* A_3, const /* INPUT */svLogicVecVal *A_4, /* INPUT */int A_5, /* INPUT */const char* A_6)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */const char* A_2, /* INPUT */const char* A_3, const /* INPUT */svLogicVecVal *A_4, /* INPUT */int A_5, /* INPUT */const char* A_6) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, const char* A_2, const char* A_3, const svLogicVecVal* A_4, int A_5, const char* A_6)) dlsym(RTLD_NEXT, "fsdbTransDPI_scope_add_logicvec_attribute");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5, A_6);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_scope_add_logicvec_attribute");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_scope_add_logicvec_attribute */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_scope_add_int_attribute
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_scope_add_int_attribute
__attribute__((weak)) void fsdbTransDPI_scope_add_int_attribute(/* OUTPUT */int *A_1, /* INPUT */const char* A_2, /* INPUT */const char* A_3, /* INPUT */int A_4, /* INPUT */const char* A_5)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */const char* A_2, /* INPUT */const char* A_3, /* INPUT */int A_4, /* INPUT */const char* A_5) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, const char* A_2, const char* A_3, int A_4, const char* A_5)) dlsym(RTLD_NEXT, "fsdbTransDPI_scope_add_int_attribute");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_scope_add_int_attribute");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_scope_add_int_attribute */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_scope_add_string_attribute
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_scope_add_string_attribute
__attribute__((weak)) void fsdbTransDPI_scope_add_string_attribute(/* OUTPUT */int *A_1, /* INPUT */const char* A_2, /* INPUT */const char* A_3, /* INPUT */const char* A_4, /* INPUT */const char* A_5)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */const char* A_2, /* INPUT */const char* A_3, /* INPUT */const char* A_4, /* INPUT */const char* A_5) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, const char* A_2, const char* A_3, const char* A_4, const char* A_5)) dlsym(RTLD_NEXT, "fsdbTransDPI_scope_add_string_attribute");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_scope_add_string_attribute");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_scope_add_string_attribute */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_scope_add_real_attribute
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_scope_add_real_attribute
__attribute__((weak)) void fsdbTransDPI_scope_add_real_attribute(/* OUTPUT */int *A_1, /* INPUT */const char* A_2, /* INPUT */const char* A_3, /* INPUT */double A_4, /* INPUT */const char* A_5)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */const char* A_2, /* INPUT */const char* A_3, /* INPUT */double A_4, /* INPUT */const char* A_5) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, const char* A_2, const char* A_3, double A_4, const char* A_5)) dlsym(RTLD_NEXT, "fsdbTransDPI_scope_add_real_attribute");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_scope_add_real_attribute");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_scope_add_real_attribute */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_scope_add_enum_int_attribute
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_scope_add_enum_int_attribute
__attribute__((weak)) void fsdbTransDPI_scope_add_enum_int_attribute(/* OUTPUT */int *A_1, /* INPUT */const char* A_2, /* INPUT */const char* A_3, /* INPUT */unsigned int A_4, /* INPUT */int A_5, /* INPUT */const char* A_6)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */const char* A_2, /* INPUT */const char* A_3, /* INPUT */unsigned int A_4, /* INPUT */int A_5, /* INPUT */const char* A_6) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, const char* A_2, const char* A_3, unsigned int A_4, int A_5, const char* A_6)) dlsym(RTLD_NEXT, "fsdbTransDPI_scope_add_enum_int_attribute");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5, A_6);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_scope_add_enum_int_attribute");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_scope_add_enum_int_attribute */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_create_stream_begin
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_create_stream_begin
__attribute__((weak)) int fsdbTransDPI_create_stream_begin(/* OUTPUT */int *A_1, /* INPUT */const char* A_2, /* INPUT */const char* A_3, /* INPUT */const char* A_4)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static int (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */const char* A_2, /* INPUT */const char* A_3, /* INPUT */const char* A_4) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (int (*)(int* A_1, const char* A_2, const char* A_3, const char* A_4)) dlsym(RTLD_NEXT, "fsdbTransDPI_create_stream_begin");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        return _vcs_dpi_fp_(A_1, A_2, A_3, A_4);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_create_stream_begin");
        return 0;
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_create_stream_begin */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_define_logicvec_attribute
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_define_logicvec_attribute
__attribute__((weak)) void fsdbTransDPI_define_logicvec_attribute(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3, const /* INPUT */svLogicVecVal *A_4, /* INPUT */int A_5, /* INPUT */const char* A_6)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3, const /* INPUT */svLogicVecVal *A_4, /* INPUT */int A_5, /* INPUT */const char* A_6) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, int A_2, const char* A_3, const svLogicVecVal* A_4, int A_5, const char* A_6)) dlsym(RTLD_NEXT, "fsdbTransDPI_define_logicvec_attribute");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5, A_6);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_define_logicvec_attribute");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_define_logicvec_attribute */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_define_int_attribute
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_define_int_attribute
__attribute__((weak)) void fsdbTransDPI_define_int_attribute(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3, /* INPUT */int A_4, /* INPUT */const char* A_5)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3, /* INPUT */int A_4, /* INPUT */const char* A_5) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, int A_2, const char* A_3, int A_4, const char* A_5)) dlsym(RTLD_NEXT, "fsdbTransDPI_define_int_attribute");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_define_int_attribute");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_define_int_attribute */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_define_string_attribute
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_define_string_attribute
__attribute__((weak)) void fsdbTransDPI_define_string_attribute(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3, /* INPUT */const char* A_4, /* INPUT */const char* A_5)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3, /* INPUT */const char* A_4, /* INPUT */const char* A_5) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, int A_2, const char* A_3, const char* A_4, const char* A_5)) dlsym(RTLD_NEXT, "fsdbTransDPI_define_string_attribute");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_define_string_attribute");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_define_string_attribute */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_define_real_attribute
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_define_real_attribute
__attribute__((weak)) void fsdbTransDPI_define_real_attribute(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3, /* INPUT */double A_4, /* INPUT */const char* A_5)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3, /* INPUT */double A_4, /* INPUT */const char* A_5) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, int A_2, const char* A_3, double A_4, const char* A_5)) dlsym(RTLD_NEXT, "fsdbTransDPI_define_real_attribute");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_define_real_attribute");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_define_real_attribute */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_define_enum_int_attribute
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_define_enum_int_attribute
__attribute__((weak)) void fsdbTransDPI_define_enum_int_attribute(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3, /* INPUT */unsigned int A_4, /* INPUT */int A_5, /* INPUT */const char* A_6)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3, /* INPUT */unsigned int A_4, /* INPUT */int A_5, /* INPUT */const char* A_6) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, int A_2, const char* A_3, unsigned int A_4, int A_5, const char* A_6)) dlsym(RTLD_NEXT, "fsdbTransDPI_define_enum_int_attribute");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5, A_6);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_define_enum_int_attribute");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_define_enum_int_attribute */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_stream_add_logicvec_attribute
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_stream_add_logicvec_attribute
__attribute__((weak)) void fsdbTransDPI_stream_add_logicvec_attribute(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3, const /* INPUT */svLogicVecVal *A_4, /* INPUT */int A_5, /* INPUT */const char* A_6)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3, const /* INPUT */svLogicVecVal *A_4, /* INPUT */int A_5, /* INPUT */const char* A_6) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, int A_2, const char* A_3, const svLogicVecVal* A_4, int A_5, const char* A_6)) dlsym(RTLD_NEXT, "fsdbTransDPI_stream_add_logicvec_attribute");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5, A_6);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_stream_add_logicvec_attribute");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_stream_add_logicvec_attribute */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_stream_add_int_attribute
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_stream_add_int_attribute
__attribute__((weak)) void fsdbTransDPI_stream_add_int_attribute(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3, /* INPUT */int A_4, /* INPUT */const char* A_5)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3, /* INPUT */int A_4, /* INPUT */const char* A_5) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, int A_2, const char* A_3, int A_4, const char* A_5)) dlsym(RTLD_NEXT, "fsdbTransDPI_stream_add_int_attribute");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_stream_add_int_attribute");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_stream_add_int_attribute */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_stream_add_string_attribute
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_stream_add_string_attribute
__attribute__((weak)) void fsdbTransDPI_stream_add_string_attribute(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3, /* INPUT */const char* A_4, /* INPUT */const char* A_5)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3, /* INPUT */const char* A_4, /* INPUT */const char* A_5) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, int A_2, const char* A_3, const char* A_4, const char* A_5)) dlsym(RTLD_NEXT, "fsdbTransDPI_stream_add_string_attribute");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_stream_add_string_attribute");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_stream_add_string_attribute */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_stream_add_real_attribute
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_stream_add_real_attribute
__attribute__((weak)) void fsdbTransDPI_stream_add_real_attribute(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3, /* INPUT */double A_4, /* INPUT */const char* A_5)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3, /* INPUT */double A_4, /* INPUT */const char* A_5) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, int A_2, const char* A_3, double A_4, const char* A_5)) dlsym(RTLD_NEXT, "fsdbTransDPI_stream_add_real_attribute");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_stream_add_real_attribute");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_stream_add_real_attribute */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_stream_add_enum_int_attribute
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_stream_add_enum_int_attribute
__attribute__((weak)) void fsdbTransDPI_stream_add_enum_int_attribute(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3, /* INPUT */unsigned int A_4, /* INPUT */int A_5, /* INPUT */const char* A_6)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3, /* INPUT */unsigned int A_4, /* INPUT */int A_5, /* INPUT */const char* A_6) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, int A_2, const char* A_3, unsigned int A_4, int A_5, const char* A_6)) dlsym(RTLD_NEXT, "fsdbTransDPI_stream_add_enum_int_attribute");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5, A_6);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_stream_add_enum_int_attribute");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_stream_add_enum_int_attribute */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_create_stream_end
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_create_stream_end
__attribute__((weak)) void fsdbTransDPI_create_stream_end(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, int A_2, const char* A_3)) dlsym(RTLD_NEXT, "fsdbTransDPI_create_stream_end");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_create_stream_end");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_create_stream_end */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_begin
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_begin
__attribute__((weak)) long long fsdbTransDPI_begin(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3, /* INPUT */const char* A_4)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static long long (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */int A_2, /* INPUT */const char* A_3, /* INPUT */const char* A_4) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (long long (*)(int* A_1, int A_2, const char* A_3, const char* A_4)) dlsym(RTLD_NEXT, "fsdbTransDPI_begin");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        return _vcs_dpi_fp_(A_1, A_2, A_3, A_4);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_begin");
        return 0;
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_begin */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_set_label
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_set_label
__attribute__((weak)) void fsdbTransDPI_set_label(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, /* INPUT */const char* A_4)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, /* INPUT */const char* A_4) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, long long A_2, const char* A_3, const char* A_4)) dlsym(RTLD_NEXT, "fsdbTransDPI_set_label");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_set_label");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_set_label */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_tag
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_tag
__attribute__((weak)) void fsdbTransDPI_add_tag(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, /* INPUT */const char* A_4)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, /* INPUT */const char* A_4) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, long long A_2, const char* A_3, const char* A_4)) dlsym(RTLD_NEXT, "fsdbTransDPI_add_tag");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_add_tag");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_tag */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_logicvec_attribute
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_logicvec_attribute
__attribute__((weak)) void fsdbTransDPI_add_logicvec_attribute(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, const /* INPUT */svLogicVecVal *A_4, /* INPUT */int A_5, /* INPUT */const char* A_6)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, const /* INPUT */svLogicVecVal *A_4, /* INPUT */int A_5, /* INPUT */const char* A_6) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, long long A_2, const char* A_3, const svLogicVecVal* A_4, int A_5, const char* A_6)) dlsym(RTLD_NEXT, "fsdbTransDPI_add_logicvec_attribute");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5, A_6);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_add_logicvec_attribute");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_logicvec_attribute */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_int_attribute
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_int_attribute
__attribute__((weak)) void fsdbTransDPI_add_int_attribute(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, /* INPUT */int A_4, /* INPUT */const char* A_5)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, /* INPUT */int A_4, /* INPUT */const char* A_5) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, long long A_2, const char* A_3, int A_4, const char* A_5)) dlsym(RTLD_NEXT, "fsdbTransDPI_add_int_attribute");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_add_int_attribute");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_int_attribute */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_string_attribute
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_string_attribute
__attribute__((weak)) void fsdbTransDPI_add_string_attribute(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, /* INPUT */const char* A_4, /* INPUT */const char* A_5)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, /* INPUT */const char* A_4, /* INPUT */const char* A_5) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, long long A_2, const char* A_3, const char* A_4, const char* A_5)) dlsym(RTLD_NEXT, "fsdbTransDPI_add_string_attribute");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_add_string_attribute");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_string_attribute */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_real_attribute
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_real_attribute
__attribute__((weak)) void fsdbTransDPI_add_real_attribute(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, /* INPUT */double A_4, /* INPUT */const char* A_5)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, /* INPUT */double A_4, /* INPUT */const char* A_5) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, long long A_2, const char* A_3, double A_4, const char* A_5)) dlsym(RTLD_NEXT, "fsdbTransDPI_add_real_attribute");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_add_real_attribute");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_real_attribute */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_enum_int_attribute
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_enum_int_attribute
__attribute__((weak)) void fsdbTransDPI_add_enum_int_attribute(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, /* INPUT */unsigned int A_4, /* INPUT */int A_5, /* INPUT */const char* A_6)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, /* INPUT */unsigned int A_4, /* INPUT */int A_5, /* INPUT */const char* A_6) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, long long A_2, const char* A_3, unsigned int A_4, int A_5, const char* A_6)) dlsym(RTLD_NEXT, "fsdbTransDPI_add_enum_int_attribute");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5, A_6);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_add_enum_int_attribute");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_enum_int_attribute */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_logicvec_attribute_with_expected_value
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_logicvec_attribute_with_expected_value
__attribute__((weak)) void fsdbTransDPI_add_logicvec_attribute_with_expected_value(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, const /* INPUT */svLogicVecVal *A_4, /* INPUT */int A_5, const /* INPUT */svLogicVecVal *A_6, /* INPUT */const char* A_7)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, const /* INPUT */svLogicVecVal *A_4, /* INPUT */int A_5, const /* INPUT */svLogicVecVal *A_6, /* INPUT */const char* A_7) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, long long A_2, const char* A_3, const svLogicVecVal* A_4, int A_5, const svLogicVecVal* A_6, const char* A_7)) dlsym(RTLD_NEXT, "fsdbTransDPI_add_logicvec_attribute_with_expected_value");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5, A_6, A_7);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_add_logicvec_attribute_with_expected_value");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_logicvec_attribute_with_expected_value */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_int_attribute_with_expected_value
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_int_attribute_with_expected_value
__attribute__((weak)) void fsdbTransDPI_add_int_attribute_with_expected_value(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, /* INPUT */int A_4, /* INPUT */int A_5, /* INPUT */const char* A_6)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, /* INPUT */int A_4, /* INPUT */int A_5, /* INPUT */const char* A_6) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, long long A_2, const char* A_3, int A_4, int A_5, const char* A_6)) dlsym(RTLD_NEXT, "fsdbTransDPI_add_int_attribute_with_expected_value");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5, A_6);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_add_int_attribute_with_expected_value");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_int_attribute_with_expected_value */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_string_attribute_with_expected_value
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_string_attribute_with_expected_value
__attribute__((weak)) void fsdbTransDPI_add_string_attribute_with_expected_value(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, /* INPUT */const char* A_4, /* INPUT */const char* A_5, /* INPUT */const char* A_6)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, /* INPUT */const char* A_4, /* INPUT */const char* A_5, /* INPUT */const char* A_6) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, long long A_2, const char* A_3, const char* A_4, const char* A_5, const char* A_6)) dlsym(RTLD_NEXT, "fsdbTransDPI_add_string_attribute_with_expected_value");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5, A_6);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_add_string_attribute_with_expected_value");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_string_attribute_with_expected_value */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_real_attribute_with_expected_value
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_real_attribute_with_expected_value
__attribute__((weak)) void fsdbTransDPI_add_real_attribute_with_expected_value(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, /* INPUT */double A_4, /* INPUT */double A_5, /* INPUT */const char* A_6)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, /* INPUT */double A_4, /* INPUT */double A_5, /* INPUT */const char* A_6) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, long long A_2, const char* A_3, double A_4, double A_5, const char* A_6)) dlsym(RTLD_NEXT, "fsdbTransDPI_add_real_attribute_with_expected_value");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5, A_6);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_add_real_attribute_with_expected_value");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_real_attribute_with_expected_value */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_enum_int_attribute_with_expected_value
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_enum_int_attribute_with_expected_value
__attribute__((weak)) void fsdbTransDPI_add_enum_int_attribute_with_expected_value(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, /* INPUT */unsigned int A_4, /* INPUT */int A_5, /* INPUT */int A_6, /* INPUT */const char* A_7)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3, /* INPUT */unsigned int A_4, /* INPUT */int A_5, /* INPUT */int A_6, /* INPUT */const char* A_7) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, long long A_2, const char* A_3, unsigned int A_4, int A_5, int A_6, const char* A_7)) dlsym(RTLD_NEXT, "fsdbTransDPI_add_enum_int_attribute_with_expected_value");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5, A_6, A_7);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_add_enum_int_attribute_with_expected_value");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_enum_int_attribute_with_expected_value */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_end
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_end
__attribute__((weak)) void fsdbTransDPI_end(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */long long A_2, /* INPUT */const char* A_3) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, long long A_2, const char* A_3)) dlsym(RTLD_NEXT, "fsdbTransDPI_end");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_end");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_end */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_relation
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_relation
__attribute__((weak)) void fsdbTransDPI_add_relation(/* OUTPUT */int *A_1, /* INPUT */const char* A_2, /* INPUT */long long A_3, /* INPUT */long long A_4, /* INPUT */const char* A_5)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */const char* A_2, /* INPUT */long long A_3, /* INPUT */long long A_4, /* INPUT */const char* A_5) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (void (*)(int* A_1, const char* A_2, long long A_3, long long A_4, const char* A_5)) dlsym(RTLD_NEXT, "fsdbTransDPI_add_relation");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1, A_2, A_3, A_4, A_5);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_add_relation");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_add_relation */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_get_enum_id
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_get_enum_id
__attribute__((weak)) unsigned int fsdbTransDPI_get_enum_id(/* OUTPUT */int *A_1, /* INPUT */const char* A_2)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static unsigned int (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */const char* A_2) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (unsigned int (*)(int* A_1, const char* A_2)) dlsym(RTLD_NEXT, "fsdbTransDPI_get_enum_id");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        return _vcs_dpi_fp_(A_1, A_2);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_get_enum_id");
        return 0;
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_get_enum_id */

#ifndef __VCS_IMPORT_DPI_STUB_fsdbTransDPI_get_class_str
#define __VCS_IMPORT_DPI_STUB_fsdbTransDPI_get_class_str
__attribute__((weak)) SV_STRING fsdbTransDPI_get_class_str(/* OUTPUT */int *A_1, /* INPUT */const char* A_2, /* INPUT */const char* A_3)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static SV_STRING (*_vcs_dpi_fp_)(/* OUTPUT */int *A_1, /* INPUT */const char* A_2, /* INPUT */const char* A_3) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_fp_ = (SV_STRING (*)(int* A_1, const char* A_2, const char* A_3)) dlsym(RTLD_NEXT, "fsdbTransDPI_get_class_str");
        _vcs_dpi_stub_initialized_ = 1;
    }
    if (_vcs_dpi_fp_) {
        return _vcs_dpi_fp_(A_1, A_2, A_3);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "fsdbTransDPI_get_class_str");
        return 0;
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_fsdbTransDPI_get_class_str */

#ifndef __VCS_EXPORT_DPI_DUMMY_REFERENCES__
#define __VCS_EXPORT_DPI_DUMMY_REFERENCES__
/* Dummy references to those export DPI routines.
 * The symbols will be then exported, so the
 * import DPI routines in another shared
 * libraries can call.
 */
void __vcs_export_dpi_dummy_references__();
void __vcs_export_dpi_dummy_references__()
{
    extern void m__uvm_report_dpi(void);
    void (*fp0)(void) = (void (*)(void)) m__uvm_report_dpi;
    fp0 = fp0;
    extern void pli_dhier_begin_event(void);
    void (*fp1)(void) = (void (*)(void)) pli_dhier_begin_event;
    fp1 = fp1;
    extern void pli_trans_add_class_name_attr(void);
    void (*fp2)(void) = (void (*)(void)) pli_trans_add_class_name_attr;
    fp2 = fp2;
    extern void pli_trans_add_vif_attr(void);
    void (*fp3)(void) = (void (*)(void)) pli_trans_add_vif_attr;
    fp3 = fp3;
    extern void pli_dhier_set_label(void);
    void (*fp4)(void) = (void (*)(void)) pli_dhier_set_label;
    fp4 = fp4;
    extern void pli_dhier_add_attribute(void);
    void (*fp5)(void) = (void (*)(void)) pli_dhier_add_attribute;
    fp5 = fp5;
    extern void pli_dhier_add_attribute_int(void);
    void (*fp6)(void) = (void (*)(void)) pli_dhier_add_attribute_int;
    fp6 = fp6;
    extern void pli_dhier_end_event(void);
    void (*fp7)(void) = (void (*)(void)) pli_dhier_end_event;
    fp7 = fp7;
}
#endif /* __VCS_EXPORT_DPI_DUMMY_REFERENCES_ */

#ifdef __cplusplus
}
#endif

