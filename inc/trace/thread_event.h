#ifndef THREAD_EVENT_H_
#define THREAD_EVENT_H_

#include <boost/format.hpp>
#include <boost/format/format_fwd.hpp>
#include <cmath>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "event_factory.h"
#include "json.hpp"
#include "logger.h"
#include "proc_info.h"
#include "trace/trace_clock.h"
#include "trace_event.h"
#include "trace_type.h"
namespace coding_nerd::boot_perf {

template <typename T>
struct ThreadEvent : TraceEvent<T> {
  ThreadEvent() = delete;
  ThreadEvent(ThreadEvent& other) = delete;
  ThreadEvent& operator=(ThreadEvent& other) = delete;
  ThreadEvent(ThreadEvent&& other) = delete;
  ~ThreadEvent() override = default;
  explicit ThreadEvent(std::shared_ptr<T>& event,
                       std::shared_ptr<TraceClock>& clock)
      : TraceEvent<T>(event, clock){};

  static _THREAD_STATE EventToState(const uint32_t event) {
    return static_cast<_THREAD_STATE>(std::log(event));
  };

  static const std::string& EventToLinuxState(const _THREAD_STATE state) {
    // PROCESS STATE CODES
    //    Here are the different values that the s, stat and state output
    //    specifiers (header "STAT" or "S") will display to describe the state
    //    of a
    //    process:

    //            D    uninterruptible sleep (usually IO)
    //            R    running or runnable (on run queue)
    //            S    interruptible sleep (waiting for an event to complete)
    //            T    stopped by job control signal
    //            t    stopped by debugger during the tracing
    //            W    paging (not valid since the 2.6.xx kernel)
    //            X    dead (should never be seen)
    //            Z    defunct ("zombie") process, terminated but not reaped
    //            by its parent

    //    For BSD formats and when the stat keyword is used, additional
    //    characters may be displayed:

    //            <    high-priority (not nice to other users)
    //            N    low-priority (nice to other users)
    //            L    has pages locked into memory (for real-time and custom
    //            IO)
    //            s    is a session leader
    //            l    is multi-threaded (using CLONE_THREAD, like NPTL
    //            pthreads do)
    //            +    is in the foreground process group

    static std::unordered_map<_THREAD_STATE, std::string> to_linux_state{
        {STATE_DEAD, "X"},       {STATE_RUNNING, "R"},
        {STATE_READY, "R+"},     {STATE_STOPPED, "T"},
        {STATE_SEND, "S"},       {STATE_RECEIVE, "S"},
        {STATE_REPLY, "S"},      {STATE_STACK, "D"},
        {STATE_WAITTHREAD, "D"}, {STATE_WAITPAGE, "D"},
        {STATE_SIGSUSPEND, "S"}, {STATE_SIGWAITINFO, "S"},
        {STATE_NANOSLEEP, "S"},  {STATE_MUTEX, "S"},
        {STATE_CONDVAR, "S"},    {STATE_JOIN, "S"},
        {STATE_INTR, "S"},       {STATE_SEM, "S"},
        {STATE_WAITCTX, "D"},    {STATE_NET_SEND, "S"},
        {STATE_NET_REPLY, "S"}};

    return to_linux_state[state];
  };

  static void ToJson(nlohmann::json& j, std::vector<T>& e,
                     const TraceClock& trace_clock) {
    TraceEvent<T>::ToJson(j, e, trace_clock);
    VerboseLogger() << "ThreadEvent" << j;
  };

  static void ToFtrace(std::string& trace, std::vector<T>& e,
                       const TraceClock& trace_clock) {
    uint32_t ext_class;
    uint32_t ext_event;
    TraceEvent<T>::ToExt(_NTO_TRACE_GETEVENT_C(e[0].header),
                         _NTO_TRACE_GETEVENT(e[0].header), ext_class,
                         ext_event);

    switch (ext_event) {
      case _NTO_TRACE_THRUNNING: {
        if (!GetRunningProcess(e[0].data[1])) {
          break;
        }
        auto& thread_info = GetRunningThread(e[0].data[1], e[0].data[2]);
        if (!thread_info) {
          thread_info.reset(new ThreadInfo());
        }
        thread_info->tgid = e[0].data[1];
        thread_info->tid = e[0].data[2];
        thread_info->priority = e[1].data[1];
        thread_info->policy = e[1].data[2];
        thread_info->cpu = _NTO_TRACE_GETCPU(e[0].header);
        thread_info->state = STATE_RUNNING;

        auto& cpu_thread_info = GetCPURunningThread(thread_info->cpu);
        if (!cpu_thread_info) {
          cpu_thread_info = thread_info;
          break;
        }
        auto& cpu_process_info = GetRunningProcess(cpu_thread_info->tgid);
        auto& next_process_info = GetRunningProcess(thread_info->tgid);

        const float sec =
            static_cast<float>(
                trace_clock.MicroSinceBootFromCycle(e[0].data[0]).count()) /
            1000 / 1000;
        // clang-format off
        trace += boost::str(
            boost::format("%s-%d (-----) [00%1d] .... %7.6f: \
sched_switch: prev_comm=%s prev_pid=%d \
prev_prio=%d prev_state=%s ==> next_comm=%s \
next_pid=%d next_prio=%d\\n ") %
            cpu_process_info->name % cpu_process_info->pid %
            _NTO_TRACE_GETCPU(e[0].header) % sec % cpu_process_info->name %
            cpu_process_info->pid % cpu_thread_info->priority %
            EventToLinuxState(cpu_thread_info->state) % next_process_info->name %
            next_process_info->pid % thread_info->priority);
        // clang-format on
        cpu_thread_info = thread_info;
        InfoLogger() << "_NTO_TRACE_THRUNNING " << *thread_info;
        break;
      }
      case _NTO_TRACE_THCREATE: {
        auto& thread_info = GetRunningThread(e[0].data[1], e[0].data[2]);
        if (!thread_info) {
          thread_info.reset(new ThreadInfo());
        }
        thread_info->tgid = e[0].data[1];
        thread_info->tid = e[0].data[2];
        thread_info->cpu = _NTO_TRACE_GETCPU(e[0].header);
        InfoLogger() << "_NTO_TRACE_THCREATE " << *thread_info;
        break;
      }
      case _NTO_TRACE_THDESTROY: {
        auto& thread_info = GetRunningThread(e[0].data[1], e[0].data[2]);
        thread_info->state = STATE_DEAD;
        RunningThreadDead(e[0].data[1], e[0].data[2]);
        InfoLogger() << "_NTO_TRACE_THDESTROY " << *thread_info;
        break;
      }
      default:
        break;
    }

    switch (ext_event) {
      case _NTO_TRACE_THDEAD:
      case _NTO_TRACE_THRUNNING:
      case _NTO_TRACE_THREADY:
      case _NTO_TRACE_THSTOPPED:
      case _NTO_TRACE_THSEND:
      case _NTO_TRACE_THRECEIVE:
      case _NTO_TRACE_THREPLY:
      case _NTO_TRACE_THSTACK:
      case _NTO_TRACE_THWAITTHREAD:
      case _NTO_TRACE_THWAITPAGE:
      case _NTO_TRACE_THSIGSUSPEND:
      case _NTO_TRACE_THSIGWAITINFO:
      case _NTO_TRACE_THNANOSLEEP:
      case _NTO_TRACE_THMUTEX:
      case _NTO_TRACE_THCONDVAR:
      case _NTO_TRACE_THJOIN:
      case _NTO_TRACE_THINTR:
      case _NTO_TRACE_THSEM:
      case _NTO_TRACE_THWAITCTX:
      case _NTO_TRACE_THNET_SEND:
      case _NTO_TRACE_THNET_REPLY:
      case _NTO_TRACE_THCREATE: {
        if (!GetRunningProcess(e[0].data[1])) {
          break;
        }
        auto& thread_info = GetRunningThread(e[0].data[1], e[0].data[2]);
        if (!thread_info) {
          break;
        }

        thread_info->state = EventToState(ext_event);
        break;
      }
      default:
        break;
    }
  };
};
}  // namespace coding_nerd::boot_perf
#endif  // THREAD_EVENT_H_
