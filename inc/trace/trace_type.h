#ifndef TRACE_TYPE_H_
#define TRACE_TYPE_H_

#include <cstdint>

// NOLINTBEGIN

#define TO_FTRACE_TID(pid, tid) ((pid << 8) | (tid & 0x000000FF))
#define TO_QNX_TID(tid) (tid & 0x000000FF)

// kernel state
enum {
  __KER_NOP,         /*  0		0x00 */
  __KER_TRACE_EVENT, /*  1		0x01 */
  __KER_RING0,       /*  2		0x02 */
  __KER_SPARE1,      /*  3		0x03 */
  __KER_SPARE2,      /*  4		0x04 */
  __KER_SPARE3,      /*  5		0x05 */
  __KER_SPARE4,      /*  6		0x06 */

  __KER_SYS_CPUPAGE_GET, /*  7		0x07 */
  __KER_SYS_CPUPAGE_SET, /*  8		0x08 */
  __KER_SYS_SPARE1,      /*  9		0x09 */

  __KER_MSG_CURRENT,       /* 10		0x0a */
  __KER_MSG_SENDV,         /* 11		0x0b */
  __KER_MSG_SENDVNC,       /* 12		0x0c */
  __KER_MSG_ERROR,         /* 13		0x0d */
  __KER_MSG_RECEIVEV,      /* 14		0x0e */
  __KER_MSG_REPLYV,        /* 15		0x0f */
  __KER_MSG_READV,         /* 16		0x10 */
  __KER_MSG_WRITEV,        /* 17		0x11 */
  __KER_MSG_READWRITEV,    /* 18       0x12 */
  __KER_MSG_INFO,          /* 19		0x13 */
  __KER_MSG_SEND_PULSE,    /* 20		0x14 */
  __KER_MSG_DELIVER_EVENT, /* 21		0x15 */
  __KER_MSG_KEYDATA,       /* 22		0x16 */
  __KER_MSG_READIOV,       /* 23		0x17 */
  __KER_MSG_RECEIVEPULSEV, /* 24		0x18 */
  __KER_MSG_VERIFY_EVENT,  /* 25		0x19 */

  __KER_SIGNAL_KILL,     /* 26		0x1a */
  __KER_SIGNAL_RETURN,   /* 27		0x1b */
  __KER_SIGNAL_FAULT,    /* 28		0x1c */
  __KER_SIGNAL_ACTION,   /* 29		0x1d */
  __KER_SIGNAL_PROCMASK, /* 30		0x1e */
  __KER_SIGNAL_SUSPEND,  /* 31		0x1f */
  __KER_SIGNAL_WAITINFO, /* 32		0x20 */
  __KER_SIGNAL_SPARE1,   /* 33		0x21 */
  __KER_SIGNAL_SPARE2,   /* 34		0x22 */

  __KER_CHANNEL_CREATE,  /* 35		0x23 */
  __KER_CHANNEL_DESTROY, /* 36		0x24 */
  __KER_CHANCON_ATTR,    /* 37		0x25 */
  __KER_CHANNEL_SPARE1,  /* 38		0x26 */

  __KER_CONNECT_ATTACH,      /* 39		0x27 */
  __KER_CONNECT_DETACH,      /* 40		0x28 */
  __KER_CONNECT_SERVER_INFO, /* 41		0x29 */
  __KER_CONNECT_CLIENT_INFO, /* 42		0x2a */
  __KER_CONNECT_FLAGS,       /* 43		0x2b */
  __KER_CONNECT_SPARE1,      /* 44		0x2c */
  __KER_CONNECT_SPARE2,      /* 45		0x2d */

  __KER_THREAD_CREATE,     /* 46		0x2e */
  __KER_THREAD_DESTROY,    /* 47		0x2f */
  __KER_THREAD_DESTROYALL, /* 48		0x30 */
  __KER_THREAD_DETACH,     /* 49		0x31 */
  __KER_THREAD_JOIN,       /* 50		0x32 */
  __KER_THREAD_CANCEL,     /* 51		0x33 */
  __KER_THREAD_CTL,        /* 52		0x34 */
  __KER_THREAD_SPARE1,     /* 53		0x35 */
  __KER_THREAD_SPARE2,     /* 54		0x36 */

  __KER_INTERRUPT_ATTACH,      /* 55		0x37 */
  __KER_INTERRUPT_DETACH_FUNC, /* 56		0x38 */
  __KER_INTERRUPT_DETACH,      /* 57		0x39 */
  __KER_INTERRUPT_WAIT,        /* 58		0x3a */
  __KER_INTERRUPT_MASK,        /* 59		0x3b */
  __KER_INTERRUPT_UNMASK,      /* 60		0x3c */
  __KER_INTERRUPT_SPARE1,      /* 61		0x3d */
  __KER_INTERRUPT_SPARE2,      /* 62		0x3e */
  __KER_INTERRUPT_SPARE3,      /* 63		0x3f */
  __KER_INTERRUPT_SPARE4,      /* 64		0x40 */

  __KER_CLOCK_TIME,   /* 65		0x41 */
  __KER_CLOCK_ADJUST, /* 66		0x42 */
  __KER_CLOCK_PERIOD, /* 67		0x43 */
  __KER_CLOCK_ID,     /* 68		0x44 */
  __KER_CLOCK_SPARE2, /* 69		0x45 */

  __KER_TIMER_CREATE,  /* 70		0x46 */
  __KER_TIMER_DESTROY, /* 71		0x47 */
  __KER_TIMER_SETTIME, /* 72		0x48 */
  __KER_TIMER_INFO,    /* 73		0x49 */
  __KER_TIMER_ALARM,   /* 74		0x4a */
  __KER_TIMER_TIMEOUT, /* 75		0x4b */
  __KER_TIMER_SPARE1,  /* 76		0x4c */
  __KER_TIMER_SPARE2,  /* 77		0x4d */

  __KER_SYNC_CREATE,         /* 78		0x4e */
  __KER_SYNC_DESTROY,        /* 79		0x4f */
  __KER_SYNC_MUTEX_LOCK,     /* 80		0x50 */
  __KER_SYNC_MUTEX_UNLOCK,   /* 81		0x51 */
  __KER_SYNC_CONDVAR_WAIT,   /* 82		0x52 */
  __KER_SYNC_CONDVAR_SIGNAL, /* 83		0x53 */
  __KER_SYNC_SEM_POST,       /* 84		0x54 */
  __KER_SYNC_SEM_WAIT,       /* 85		0x55 */
  __KER_SYNC_CTL,            /* 86		0x56 */
  __KER_SYNC_MUTEX_REVIVE,   /* 87		0x57 */

  __KER_SCHED_GET,   /* 88		0x58 */
  __KER_SCHED_SET,   /* 89		0x59 */
  __KER_SCHED_YIELD, /* 90		0x5a */
  __KER_SCHED_INFO,  /* 91		0x5b */
  __KER_SCHED_CTL,   /* 92		0x5c */

  __KER_NET_CRED,        /* 93		0x5d */
  __KER_NET_VTID,        /* 94		0x5e */
  __KER_NET_UNBLOCK,     /* 95		0x5f */
  __KER_NET_INFOSCOID,   /* 96		0x60 */
  __KER_NET_SIGNAL_KILL, /* 97		0x61 */

  __KER_NET_SPARE1, /* 98		0x62 */
  __KER_NET_SPARE2, /* 99		0x63 */

  __KER_MT_CTL, /* 100          0x64 */

  __KER_BAD /* 101 		0x65 */
};

/*
 * Define the states of a thread
 * THREAD.state
 */
const char* const task_state[] = {
    "THDEAD",       "THRUNNING",  "THREADY",      "THSTOPPED",
    "THSEND",       "THRECEIVE",  "THREPLY",      "THSTACK",
    "THWAITTHREAD", "THWAITPAGE", "THSIGSUSPEND", "THSIGWAITINFO",
    "THNANOSLEEP",  "THMUTEX",    "THCONDVAR",    "THJOIN",
    "THINTR",       "THSEM",      "THWAITCTX",    "THNET_SEND",
    "THNET_REPLY"};

// Linux thread state
// D    uninterruptible sleep (usually IO)
// R    running or runnable (on run queue)
// R+   Runnable (Preempted)
// S    interruptible sleep (waiting for an event to complete)
// T    stopped by job control signal
// t    stopped by debugger during the tracing
// W    paging (not valid since the 2.6.xx kernel)
// X    dead (should never be seen)
// Z    defunct ("zombie") process, terminated but not reaped by its parent

enum _THREAD_STATE {
  STATE_DEAD,    /* 0	0x00 */
  STATE_RUNNING, /* 1	0x01 */
  STATE_READY,   /* 2	0x02 */
  STATE_STOPPED, /* 3	0x03 */

  STATE_SEND,    /* 4	0x04 */
  STATE_RECEIVE, /* 5	0x05 */
  STATE_REPLY,   /* 6	0x06 */

  STATE_STACK,      /* 7	0x07 */
  STATE_WAITTHREAD, /* 8	0x08 */
  STATE_WAITPAGE,   /* 9	0x09 */

  STATE_SIGSUSPEND,  /* 10	0x0a */
  STATE_SIGWAITINFO, /* 11	0x0b */
  STATE_NANOSLEEP,   /* 12	0x0c */
  STATE_MUTEX,       /* 13	0x0d */
  STATE_CONDVAR,     /* 14	0x0e */
  STATE_JOIN,        /* 15	0x0f */
  STATE_INTR,        /* 16	0x10 */
  STATE_SEM,         /* 17	0x11 */
  STATE_WAITCTX,     /* 18	0x12 */

  STATE_NET_SEND,  /* 19	0x13 */
  STATE_NET_REPLY, /* 20	0x14 */

  STATE_MAX = 24 /* This cannot be changed. It is the highest we can support */
};

const char* const process_state[] = {"PRUNNING", "PDESTROY"};

enum _PROCESS_STATE {
  STATE_CREATE, /* 0	0x00 */
  STATE_DESTROY,
  /* 1	0x01 */
};

/* TraceEvent() - modes */
#define _NTO_TRACE_ADDALLCLASSES ((0x00000001 << 28) | 1)
#define _NTO_TRACE_DELALLCLASSES ((0x00000001 << 28) | 2)
#define _NTO_TRACE_ADDCLASS ((0x00000002 << 28) | 3)
#define _NTO_TRACE_DELCLASS ((0x00000002 << 28) | 4)
#define _NTO_TRACE_ADDEVENT ((0x00000003 << 28) | 5)
#define _NTO_TRACE_DELEVENT ((0x00000003 << 28) | 6)
#define _NTO_TRACE_ADDEVENTHANDLER ((0x00000005 << 28) | 7)
#define _NTO_TRACE_DELEVENTHANDLER ((0x00000003 << 28) | 8)
#define _NTO_TRACE_INSERTEVENT ((0x00000005 << 28) | 9)
#define _NTO_TRACE_START ((0x00000001 << 28) | 10)
#define _NTO_TRACE_STOP ((0x00000001 << 28) | 11)
#define _NTO_TRACE_ALLOCBUFFER ((0x00000003 << 28) | 12)
#define _NTO_TRACE_DEALLOCBUFFER ((0x00000001 << 28) | 13)
#define _NTO_TRACE_FLUSHBUFFER ((0x00000001 << 28) | 14)
#define _NTO_TRACE_QUERYEVENTS ((0x00000001 << 28) | 15)
#define _NTO_TRACE_SETALLCLASSESFAST ((0x00000001 << 28) | 16)
#define _NTO_TRACE_SETALLCLASSESWIDE ((0x00000001 << 28) | 17)
#define _NTO_TRACE_SETCLASSFAST ((0x00000002 << 28) | 18)
#define _NTO_TRACE_SETCLASSWIDE ((0x00000002 << 28) | 19)
#define _NTO_TRACE_SETEVENTFAST ((0x00000003 << 28) | 20)
#define _NTO_TRACE_SETEVENTWIDE ((0x00000003 << 28) | 21)
#define _NTO_TRACE_SETCLASSPID ((0x00000003 << 28) | 22)
#define _NTO_TRACE_SETCLASSTID ((0x00000004 << 28) | 23)
#define _NTO_TRACE_SETEVENTPID ((0x00000004 << 28) | 24)
#define _NTO_TRACE_SETEVENTTID ((0x00000005 << 28) | 25)
#define _NTO_TRACE_CLRCLASSPID ((0x00000002 << 28) | 26)
#define _NTO_TRACE_CLRCLASSTID ((0x00000002 << 28) | 27)
#define _NTO_TRACE_CLREVENTPID ((0x00000003 << 28) | 28)
#define _NTO_TRACE_CLREVENTTID ((0x00000003 << 28) | 29)
#define _NTO_TRACE_INSERTSUSEREVENT ((0x00000004 << 28) | 30)
#define _NTO_TRACE_INSERTCUSEREVENT ((0x00000004 << 28) | 31)
#define _NTO_TRACE_INSERTUSRSTREVENT ((0x00000003 << 28) | 32)
#define _NTO_TRACE_STARTNOSTATE ((0x00000001 << 28) | 33)
#define _NTO_TRACE_SETRINGMODE ((0x00000001 << 28) | 34)
#define _NTO_TRACE_SETLINEARMODE ((0x00000001 << 28) | 35)
#define _NTO_TRACE_ADDCLASSEVHANDLER ((0x00000004 << 28) | 36)
#define _NTO_TRACE_DELCLASSEVHANDLER ((0x00000002 << 28) | 37)
#define _NTO_TRACE_SKIPBUFFER ((0x00000001 << 28) | 38)
#define _NTO_TRACE_OVERWRITEBUFFER ((0x00000001 << 28) | 39)
#define _NTO_TRACE_QUERYVERSION ((0x00000001 << 28) | 40)
#define _NTO_TRACE_QUERYSUPPORT ((0x00000001 << 28) | 41)
#define _NTO_TRACE_INSERTSCLASSEVENT ((0x00000005 << 28) | 42)
#define _NTO_TRACE_INSERTCCLASSEVENT ((0x00000005 << 28) | 43)
#define _NTO_TRACE_SETBUFFER ((0x00000003 << 28) | 44)

/* TraceEvent() - external classes */
enum {
  _NTO_TRACE_EMPTY,
  _NTO_TRACE_CONTROL,
  _NTO_TRACE_KERCALL,
  _NTO_TRACE_KERCALLENTER,
  _NTO_TRACE_KERCALLEXIT,
  _NTO_TRACE_KERCALLINT,
  _NTO_TRACE_INT,
  _NTO_TRACE_INTENTER,
  _NTO_TRACE_INTEXIT,
  _NTO_TRACE_PROCESS,
  _NTO_TRACE_THREAD,
  _NTO_TRACE_VTHREAD,
  _NTO_TRACE_USER,
  _NTO_TRACE_SYSTEM,
  _NTO_TRACE_COMM,
  _NTO_TRACE_INT_HANDLER_ENTER,
  _NTO_TRACE_INT_HANDLER_EXIT,
  _NTO_TRACE_QUIP,
  _NTO_TRACE_SEC,
  _NTO_TRACE_QVM,
  _NTO_TRACE_UNKOWN
};

static const char* ClassName[] = {"NTO_TRACE_EMPTY",
                                  "NTO_TRACE_CONTROL",
                                  "NTO_TRACE_KERCALL",
                                  "NTO_TRACE_KERCALLENTER",
                                  "NTO_TRACE_KERCALLEXIT",
                                  "NTO_TRACE_KERCALLINT",
                                  "NTO_TRACE_INT",
                                  "NTO_TRACE_INTENTER",
                                  "NTO_TRACE_INTEXIT",
                                  "NTO_TRACE_PROCESS",
                                  "NTO_TRACE_THREAD",
                                  "NTO_TRACE_VTHREAD",
                                  "NTO_TRACE_USER",
                                  "NTO_TRACE_SYSTEM",
                                  "NTO_TRACE_COMM",
                                  "NTO_TRACE_INT_HANDLER_ENTER",
                                  "NTO_TRACE_INT_HANDLER_EXIT",
                                  "NTO_TRACE_QUIP",
                                  "NTO_TRACE_SEC",
                                  "NTO_TRACE_QVM",
                                  "_NTO_TRACE_UNKOWN"};

/* TraceEvent() - external events */
#define _NTO_TRACE_EMPTYEVENT (0x00000000u)

#define _NTO_TRACE_CONTROLTIME (0x00000001u << 0)
#define _NTO_TRACE_CONTROLBUFFER (0x00000001u << 1)

#define _NTO_TRACE_KERCALLFIRST (0x00000000u)
#define _NTO_TRACE_KERCALLLAST (__KER_BAD)
#define _NTO_TRACE_KERCALL64 (0x00000001u << 9)

#define _NTO_TRACE_INTFIRST (0x00000000u)
#define _NTO_TRACE_INTLAST (0xffffffffu)

#define _NTO_TRACE_PROCCREATE (0x00000001u << 0)
#define _NTO_TRACE_PROCDESTROY (0x00000001u << 1)
#define _NTO_TRACE_PROCCREATE_NAME (0x00000001u << 2)
#define _NTO_TRACE_PROCDESTROY_NAME (0x00000001u << 3)
#define _NTO_TRACE_PROCTHREAD_NAME (0x00000001u << 4)

#define _NTO_TRACE_THDEAD (0x00000001u << STATE_DEAD)
#define _NTO_TRACE_THRUNNING (0x00000001u << STATE_RUNNING)
#define _NTO_TRACE_THREADY (0x00000001u << STATE_READY)
#define _NTO_TRACE_THSTOPPED (0x00000001u << STATE_STOPPED)
#define _NTO_TRACE_THSEND (0x00000001u << STATE_SEND)
#define _NTO_TRACE_THRECEIVE (0x00000001u << STATE_RECEIVE)
#define _NTO_TRACE_THREPLY (0x00000001u << STATE_REPLY)
#define _NTO_TRACE_THSTACK (0x00000001u << STATE_STACK)
#define _NTO_TRACE_THWAITTHREAD (0x00000001u << STATE_WAITTHREAD)
#define _NTO_TRACE_THWAITPAGE (0x00000001u << STATE_WAITPAGE)
#define _NTO_TRACE_THSIGSUSPEND (0x00000001u << STATE_SIGSUSPEND)
#define _NTO_TRACE_THSIGWAITINFO (0x00000001u << STATE_SIGWAITINFO)
#define _NTO_TRACE_THNANOSLEEP (0x00000001u << STATE_NANOSLEEP)
#define _NTO_TRACE_THMUTEX (0x00000001u << STATE_MUTEX)
#define _NTO_TRACE_THCONDVAR (0x00000001u << STATE_CONDVAR)
#define _NTO_TRACE_THJOIN (0x00000001u << STATE_JOIN)
#define _NTO_TRACE_THINTR (0x00000001u << STATE_INTR)
#define _NTO_TRACE_THSEM (0x00000001u << STATE_SEM)
#define _NTO_TRACE_THWAITCTX (0x00000001u << STATE_WAITCTX)
#define _NTO_TRACE_THNET_SEND (0x00000001u << STATE_NET_SEND)
#define _NTO_TRACE_THNET_REPLY (0x00000001u << STATE_NET_REPLY)
#define _NTO_TRACE_THCREATE (0x00000001u << _TRACE_THREAD_CREATE)
#define _NTO_TRACE_THDESTROY (0x00000001u << _TRACE_THREAD_DESTROY)

#define _NTO_TRACE_VTHDEAD (0x00000001u << STATE_DEAD)
#define _NTO_TRACE_VTHRUNNING (0x00000001u << STATE_RUNNING)
#define _NTO_TRACE_VTHREADY (0x00000001u << STATE_READY)
#define _NTO_TRACE_VTHSTOPPED (0x00000001u << STATE_STOPPED)
#define _NTO_TRACE_VTHSEND (0x00000001u << STATE_SEND)
#define _NTO_TRACE_VTHRECEIVE (0x00000001u << STATE_RECEIVE)
#define _NTO_TRACE_VTHREPLY (0x00000001u << STATE_REPLY)
#define _NTO_TRACE_VTHSTACK (0x00000001u << STATE_STACK)
#define _NTO_TRACE_VTHWAITTHREAD (0x00000001u << STATE_WAITTHREAD)
#define _NTO_TRACE_VTHWAITPAGE (0x00000001u << STATE_WAITPAGE)
#define _NTO_TRACE_VTHSIGSUSPEND (0x00000001u << STATE_SIGSUSPEND)
#define _NTO_TRACE_VTHSIGWAITINFO (0x00000001u << STATE_SIGWAITINFO)
#define _NTO_TRACE_VTHNANOSLEEP (0x00000001u << STATE_NANOSLEEP)
#define _NTO_TRACE_VTHMUTEX (0x00000001u << STATE_MUTEX)
#define _NTO_TRACE_VTHCONDVAR (0x00000001u << STATE_CONDVAR)
#define _NTO_TRACE_VTHJOIN (0x00000001u << STATE_JOIN)
#define _NTO_TRACE_VTHINTR (0x00000001u << STATE_INTR)
#define _NTO_TRACE_VTHSEM (0x00000001u << STATE_SEM)
#define _NTO_TRACE_VTHWAITCTX (0x00000001u << STATE_WAITCTX)
#define _NTO_TRACE_VTHNET_SEND (0x00000001u << STATE_NET_SEND)
#define _NTO_TRACE_VTHNET_REPLY (0x00000001u << STATE_NET_REPLY)
#define _NTO_TRACE_VTHCREATE (0x00000001u << _TRACE_THREAD_CREATE)
#define _NTO_TRACE_VTHDESTROY (0x00000001u << _TRACE_THREAD_DESTROY)

#define _NTO_TRACE_USERFIRST (0x00000000u)
#define _NTO_TRACE_USERLAST (0x000003ffu)

#define _NTO_TRACE_QUIPFIRST (0x00000000)
#define _NTO_TRACE_QUIPLAST (0x000003ff)

#define _NTO_TRACE_SECFIRST (0x00000000)
#define _NTO_TRACE_SECLAST (0x000003ff)

#define _NTO_TRACE_QVMFIRST (0x00000000u)
#define _NTO_TRACE_QVMLAST (0x000003ffu)

#define _NTO_TRACE_SYS_RESERVED (0x00000001u)
#define _NTO_TRACE_SYS_PATHMGR (0x00000002u)
#define _NTO_TRACE_SYS_APS_NAME (0x00000003u)
#define _NTO_TRACE_SYS_APS_BUDGETS (0x00000004u)
#define _NTO_TRACE_SYS_APS_BNKR \
  (0x00000005) /* when APS scheduler detects bankruptcy */
#define _NTO_TRACE_SYS_MMAP (0x00000006u)
#define _NTO_TRACE_SYS_MUNMAP (0x00000007u)
#define _NTO_TRACE_SYS_MAPNAME (0x00000008u)
#define _NTO_TRACE_SYS_ADDRESS (0x00000009u)
#define _NTO_TRACE_SYS_FUNC_ENTER (0x0000000au)
#define _NTO_TRACE_SYS_FUNC_EXIT (0x0000000bu)
#define _NTO_TRACE_SYS_SLOG (0x0000000cu)
#define _NTO_TRACE_SYS_DEFRAG_START (0x0000000du)
#define _NTO_TRACE_SYS_RUNSTATE (0x0000000eu)
#define _NTO_TRACE_SYS_POWER (0x0000000fu)
#define _NTO_TRACE_SYS_IPI (0x00000010u)
#define _NTO_TRACE_SYS_PAGEWAIT (0x00000011u)
#define _NTO_TRACE_SYS_TIMER (0x00000012u)
#define _NTO_TRACE_SYS_DEFRAG_END (0x00000013u)
#define _NTO_TRACE_SYS_PROFILE (0x00000014u)
#define _NTO_TRACE_SYS_MAPNAME_64 (0x00000015u)
#define _NTO_TRACE_SYS_LAST _NTO_TRACE_SYS_MAPNAME_64
#define _NTO_TRACE_SYS_IPI_64 (_NTO_TRACE_SYS_IPI | _NTO_TRACE_KERCALL64)
#define _NTO_TRACE_SYS_PROFILE_64 \
  (_NTO_TRACE_SYS_PROFILE | _NTO_TRACE_KERCALL64)

// For backwards compatibility
#define _NTO_TRACE_SYS_COMPACTION _NTO_TRACE_SYS_DEFRAG_START

#define _NTO_TRACE_COMM_SMSG (0x00000000u)
#define _NTO_TRACE_COMM_SPULSE (0x00000001u)
#define _NTO_TRACE_COMM_RMSG (0x00000002u)
#define _NTO_TRACE_COMM_RPULSE (0x00000003u)
#define _NTO_TRACE_COMM_SPULSE_EXE (0x00000004u)
#define _NTO_TRACE_COMM_SPULSE_DIS (0x00000005u)
#define _NTO_TRACE_COMM_SPULSE_DEA (0x00000006u)
#define _NTO_TRACE_COMM_SPULSE_UN (0x00000007u)
#define _NTO_TRACE_COMM_SPULSE_QUN (0x00000008u)
#define _NTO_TRACE_COMM_SIGNAL (0x00000009u)
#define _NTO_TRACE_COMM_REPLY (0x0000000au)
#define _NTO_TRACE_COMM_ERROR (0x0000000bu)
#define _NTO_TRACE_COMM_LAST _NTO_TRACE_COMM_ERROR

#define _NTO_TRACE_MAX_CLASS (0x0000001fu)
#define _NTO_TRACE_MAX_CODES (0x000003ffu)

#define _NTO_TRACE_SETEVENT_C(c, cl) (c &= ~(0x1fu << 10), c |= (cl))
#define _NTO_TRACE_GETEVENT_C(c) ((c) & (0x1fu << 10))
#define _NTO_TRACE_SETEVENT(c, e) (c &= ~(0x3ffu), c |= (e))
#define _NTO_TRACE_GETEVENT(c) ((unsigned)(c) & 0x3ffu)
#define _NTO_TRACE_GETCPU(h) (((h) & 0x3f000000u) >> 24)

/* Query support flags */
#define _NTO_TRACE_NOINSTRSUPP (0x00000000u)
#define _NTO_TRACE_SUPPINSTR (0x00000001u << 0)

enum _TRACE_PATH_ATTACH_STATUS {
  _TRACE_PATH_ATTACH_OK,
  _TRACE_PATH_ATTACH_NO_RULE,
  _TRACE_PATH_ATTACH_CONFLICT,
  _TRACE_PATH_ATTACH_BAD_OP,
};

enum _TRACE_QNET_CONNECT_STATUS {
  _TRACE_QNET_CONNECT_OK,
  _TRACE_QNET_CONNECT_SERVER_FAILED,
  _TRACE_QNET_CONNECT_CLIENT_FAILED,
};

enum _TRACE_SEC_ABLE_FLAGS {
  _TRACE_SEC_ABLE_ALLOW = 1,
  _TRACE_SEC_ABLE_IS_ROOT = 2,
  _TRACE_SEC_ABLE_UNCREATED = 4,
};

/* Security event flags */
#define _NTO_TRACE_SEC_ABLE (0x00000000u)
#define _NTO_TRACE_SEC_ABLE_LOOKUP (0x00000001u)
#define _NTO_TRACE_SEC_PATH_ATTACH (0x00000002u)
#define _NTO_TRACE_SEC_QNET_CONNECT (0x00000003u)
#define _NTO_TRACE_SEC_PERM_LOOKUP (0x00000004u)
#define _NTO_TRACE_SEC_PERM_TEST (0x00000005u)

/* Power event flags */
#define _NTO_TRACE_POWER_CPUMASK 0x0000ffffu
#define _NTO_TRACE_POWER_IDLE 0x00010000u
#define _NTO_TRACE_POWER_START 0x00020000u
#define _NTO_TRACE_POWER_IDLE_REACHED \
  0x00040000u /* for _NTO_TRACE_POWER_IDLE */
#define _NTO_TRACE_POWER_VFS_OVERDRIVE \
  0x00040000u /* for !_NTO_TRACE_POWER_IDLE */
#define _NTO_TRACE_POWER_VFS_DYNAMIC \
  0x00080000u /* for !_NTO_TRACE_POWER_IDLE */
#define _NTO_TRACE_POWER_VFS_STEP_UP \
  0x00100000u /* for !_NTO_TRACE_POWER_IDLE */

/* Hypervisor events */
#define _NTO_TRACE_QVM_GUEST_ENTER (0x00000000u)
#define _NTO_TRACE_QVM_GUEST_EXIT (0x00000001u)
#define _NTO_TRACE_QVM_CREATE_VCPU_THREAD (0x00000002u)
#define _NTO_TRACE_QVM_RAISE_INTR (0x00000003u)
#define _NTO_TRACE_QVM_LOWER_INTR (0x00000004u)
#define _NTO_TRACE_QVM_TIMER_CREATE (0x00000005u)
#define _NTO_TRACE_QVM_TIMER_FIRE (0x00000006u)
#define _NTO_TRACE_QVM_CYCLES (0x00000007u)

/* feature index parameters */
enum { _NTO_TRACE_FIPID, _NTO_TRACE_FITID, _NTO_TRACE_FI_NUM };

/* feature parameter masks */
#define _NTO_TRACE_FMEMPTY (0x00000000u)
#define _NTO_TRACE_FMPID (1u << _NTO_TRACE_FIPID)
#define _NTO_TRACE_FMTID (1u << _NTO_TRACE_FITID)

#define _TRACE_MAX_EVENT_NUM (0x00000400u)

enum _TRACE_THREAD_STATE { /* Two add. thread-states */
                           _TRACE_THREAD_CREATE = STATE_MAX,
                           _TRACE_THREAD_DESTROY,
                           _TRACE_MAX_TH_STATE_NUM
};

#define _TRACE_MAX_STR_SIZE (128)

typedef uint32_t __traceentry;

typedef struct traceevent {
  __traceentry header;  /* CPU, event, format */
  __traceentry data[3]; /* event data         */
} traceevent_t;

/* tracefile header keywords */
#define _TRACE_MAX_HEADER_LEN (512)
#define _TRACE_HEADER_PREFIX "TRACE_"
#define _TRACE_HEADER_POSTFIX "::"

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

/* buffer/sequence extraction macros */
#define _TRACE_GET_BUFFNUM(b) (b >> 13)
#define _TRACE_GET_BUFFSEQ(b) (b & 0x000007FFu)

#define _TRACE_GET_FLAG(c) ((unsigned)(c) & 0x0fffffffu)
#define _TRACE_GET_COUNT(c) (((unsigned)(c) >> 28) & 0x7u)
#define _TRACE_FLAGS_RING (0x00010000u)
#define _TRACE_FLAGS_WRITING (0x00020000u)
#define _TRACE_FLAGS_WRITTING _TRACE_FLAGS_WRITING
#define _TRACE_FLAGS_FLUSH (0x00040000u)

#define _TRACE_MAX_KER_CALL_NUM (128)
#define _TRACE_MAX_INT_NUM (256)
#define _TRACE_MAX_COMM_NUM (_NTO_TRACE_COMM_LAST + 1)
#define _TRACE_MAX_SYSTEM_NUM (_NTO_TRACE_SYS_LAST + 1)
#define _TRACE_MAX_USER_NUM (_NTO_TRACE_USERLAST + 1)
#define _TRACE_MAX_QUIP_NUM (_NTO_TRACE_QUIPLAST + 1)
#define _TRACE_MAX_SEC_NUM (_NTO_TRACE_SECLAST + 1)
#define _TRACE_MAX_QVM_NUM (_NTO_TRACE_QVMLAST + 1)

#define _NTO_TRACE_KERCALLMASK (_TRACE_MAX_KER_CALL_NUM - 1)

/* event data filled by an event handler */
typedef struct {
  __traceentry header;                 /* same as traceevent header       */
  uint32_t* data_array;                /* initialized by the user         */
  uint32_t el_num;                     /* number of elements returned     */
  void* area;                          /* user data                       */
  uint32_t feature_mask;               /* bits indicate valid features    */
  uint32_t feature[_NTO_TRACE_FI_NUM]; /* feature array - additional data */
} event_data_t;

/* S-simple, C-comb., CC-comb.-cont, B-begin, E-end */
#define _TRACE_STRUCT_S (0x00000000u)
#define _TRACE_STRUCT_CB (0x00000001u << 30)
#define _TRACE_STRUCT_CC (0x00000002u << 30)
#define _TRACE_STRUCT_CE (0x00000003u << 30)
#define _TRACE_GET_STRUCT(c) ((c) & (0x3u << 30))

/* internal cleasses (C-class) */
#define _TRACE_EMPTY_C (0x00000000u << 10)
#define _TRACE_CONTROL_C (0x00000001u << 10)
#define _TRACE_KER_CALL_C (0x00000002u << 10)
#define _TRACE_INT_C (0x00000003u << 10)
#define _TRACE_PR_TH_C (0x00000004u << 10)
#define _TRACE_SYSTEM_C (0x00000005u << 10)
#define _TRACE_CONTAINER_C _TRACE_SYSTEM_C  // Container class never defined
#define _TRACE_USER_C (0x00000006u << 10)
#define _TRACE_COMM_C (0x00000007u << 10)
#define _TRACE_QUIP_C (0x00000008u << 10)
#define _TRACE_SEC_C (0x00000009u << 10)
#define _TRACE_QVM_C (0x0000000au << 10)
#define _TRACE_TOT_CLASS_NUM (11)

/* pseudo-events since we can't use direct int. num */
#define _TRACE_INT_ENTRY (1)
#define _TRACE_INT_EXIT (2)
#define _TRACE_INT_HANDLER_ENTRY (3)
#define _TRACE_INT_HANDLER_EXIT (4)
#define _TRACE_INT_ENTRY_64 (5)
#define _TRACE_INT_HANDLER_ENTRY_64 (6)

#define _TRACE_CONTROL_TIME (0x00000001u)
#define _TRACE_CONTROL_BUFFER (0x00000002u)

/* thread create/destroy */
#define _TRACE_PR_TH_CREATE_T (_TRACE_THREAD_CREATE)
#define _TRACE_PR_TH_DESTROY_T (_TRACE_THREAD_DESTROY)
#define _TRACE_PR_TH_CREATE_VT (_TRACE_PR_TH_CREATE_T + _TRACE_MAX_TH_STATE_NUM)
#define _TRACE_PR_TH_DESTROY_VT \
  (_TRACE_PR_TH_DESTROY_T + _TRACE_MAX_TH_STATE_NUM)

/* process create/destroy => (event<64) */
#define _TRACE_PR_TH_CREATE_P (0x00000001u << 6)
#define _TRACE_PR_TH_DESTROY_P (0x00000002u << 6)
#define _TRACE_PR_TH_CREATE_P_NAME (0x00000003u << 6)
#define _TRACE_PR_TH_DESTROY_P_NAME (0x00000004u << 6)
/* additional thread information */
#define _TRACE_PR_TH_NAME_T (0x00000005u << 6)

/* size of individual trace buffers */
#define _TRACEBUFSIZE (16 * 1024)
#define _TRACELEMENTS \
  ((_TRACEBUFSIZE - sizeof(struct traceheader)) / sizeof(struct traceevent))
#define _TRACEBUF_MAX_EVENTS \
  ((70U * _TRACELEMENTS) / 100U)  // trigger buffer flush at 70% full buffer
#define _TRACEBUF_MAX_EVENTS_RING \
  ((95U * _TRACELEMENTS) / 100U)  // trigger next buffer  at 95% full buffer
// NOLINTEND

#endif  // TRACE_TYPE_H_
