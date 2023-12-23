#ifndef TRACE_TYPE_H_
#define TRACE_TYPE_H_

#include <sys/types.h>

/* tracefile header keywords */
#define _TRACE_MAX_HEADER_LEN (512)
#define _TRACE_HEADER_PREFIX "TRACE_"
#define _TRACE_HEADER_POSTFIX "::"

/* tracefile header */
#if defined(_TRACE_MK_HK)
#undef _TRACE_MK_HK
#endif
#define _TRACE_MK_HK(k) _TRACE_HEADER_PREFIX #k _TRACE_HEADER_POSTFIX

#define _TRACE_HEADER_KEYWORDS()                                            \
  _TRACE_MK_HK(HEADER_BEGIN), _TRACE_MK_HK(FILE_NAME), _TRACE_MK_HK(DATE),  \
      _TRACE_MK_HK(VER_MAJOR), _TRACE_MK_HK(VER_MINOR),                     \
      _TRACE_MK_HK(LITTLE_ENDIAN), _TRACE_MK_HK(BIG_ENDIAN),                \
      _TRACE_MK_HK(MIDDLE_ENDIAN), _TRACE_MK_HK(ENCODING),                  \
      _TRACE_MK_HK(BOOT_DATE), _TRACE_MK_HK(CYCLES_PER_SEC),                \
      _TRACE_MK_HK(CPU_NUM), _TRACE_MK_HK(SYSNAME), _TRACE_MK_HK(NODENAME), \
      _TRACE_MK_HK(SYS_RELEASE), _TRACE_MK_HK(SYS_VERSION),                 \
      _TRACE_MK_HK(MACHINE), _TRACE_MK_HK(SYSPAGE_LEN),                     \
      _TRACE_MK_HK(HEADER_END)

namespace coding_nerd::boot_perf {

typedef uint32_t __trace_entry;

static const char* const head_keywords[] = {_TRACE_HEADER_KEYWORDS()};

typedef struct TRACE_EVENT {
  __trace_entry header;  /* CPU, event, format */
  __trace_entry data[3]; /* event data         */
} TRACE_EVENT;
}  // namespace coding_nerd::boot_perf

#endif  // TRACE_TYPE_H_
