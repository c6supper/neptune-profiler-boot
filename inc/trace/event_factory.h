#ifndef EVENT_FACTORY_H_
#define EVENT_FACTORY_H_

#include <map>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "json.hpp"
#include "logger.h"
#include "trace/trace_type.h"
#include "trace_clock.h"
#include "trace_event.h"

namespace coding_nerd::boot_perf {

template <typename C, typename T>
class EventFactory {
 public:
  using type = EventFactory;
  using ConverterType = void(C &, std::vector<T> &, const TraceClock &);
  EventFactory() = default;
  virtual ~EventFactory() = default;

  static std::shared_ptr<EventFactory<C, T>> Instance() {
    static std::shared_ptr<EventFactory<C, T>> const kTheFactory =
        std::make_shared<EventFactory<C, T>>();
    return kTheFactory;
  };

  void RegisterConverter(const uint32_t &c, ConverterType *converter) {
    const std::lock_guard<std::mutex> lock(converterMapMutex_);
    converterMap_[c] = converter;
  };

  // void Convert(C &c, T &e, const TraceClock &clock) {
  //   uint32_t ext_class;
  //   uint32_t ext_event;
  //   TraceEvent<T>::ToExt(e, ext_class, ext_event);
  //   const std::lock_guard<std::mutex> lock(converterMapMutex_);
  //   if (auto search = converterMap_.find(ext_class);
  //       search != converterMap_.end())
  //     search->second(c, e, clock);
  //   else
  //     VerboseLogger() << "Unknown e, class = " << ClassName[ext_class]
  //                     << ", event = " << ext_event;
  // };

  void Convert(C &c, std::vector<T> &e, const TraceClock &clock) {
    uint32_t ext_class;
    uint32_t ext_event;
    TraceEvent<T>::ToExt(e[0], ext_class, ext_event);
    const std::lock_guard<std::mutex> lock(converterMapMutex_);
    if (auto search = converterMap_.find(ext_class);
        search != converterMap_.end())
      search->second(c, e, clock);
  };

 private:
  std::unordered_map<uint32_t, ConverterType *> converterMap_;
  std::mutex converterMapMutex_;
};
template <typename C, typename T>
struct DoRegisterConverter {
  DoRegisterConverter(const uint32_t &c,
                      typename EventFactory<C, T>::ConverterType *converter) {
    EventFactory<C, T>::Instance()->RegisterConverter(c, converter);
  };
};
}  // namespace coding_nerd::boot_perf

#endif  // EVENT_FACTORY_H_
