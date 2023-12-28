#ifndef EVENT_FACTORY_H_
#define EVENT_FACTORY_H_

#include <map>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "json.hpp"
#include "trace/trace_type.h"
#include "trace_clock.h"

namespace coding_nerd::boot_perf {

template <typename C, typename T>
class EventFactory {
  using type = EventFactory;
  using ConverterType = void(C &, const T &, const TraceClock &);

 public:
  EventFactory() = default;
  virtual ~EventFactory() = default;

  static std::shared_ptr<EventFactory<C, T>> Instance() {
    static std::shared_ptr<EventFactory<C, T>> const kTheFactory =
        std::make_shared<EventFactory<C, T>>();
    return kTheFactory;
  };

  void RegisterConverter(const uint32_t &c, ConverterType &converter) {
    const std::lock_guard<std::mutex> lock(converterMapMutex_);
    converterMap_[c][converter];
  };

 private:
  std::unordered_map<uint32_t, ConverterType> converterMap_;
  std::mutex converterMapMutex_;
};
template <typename C, typename T>
struct DoRegisterConverter {
  DoRegisterConverter(const uint32_t &c,
                      typename EventFactory<C, T>::ConverterType &converter) {
    EventFactory<C, T>::Instance()->RegisterConverter(c, converter);
  };
};
}  // namespace coding_nerd::boot_perf

#endif  // EVENT_FACTORY_H_
