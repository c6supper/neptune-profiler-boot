#ifndef DUMPER_H_
#define DUMPER_H_

namespace coding_nerd::boot_perf {
template <typename In, typename Out>
class Dumper {
  using type = Dumper<In, Out>;

 public:
  Dumper() = default;
  virtual ~Dumper() = default;
  virtual void dump() = 0;
};
}  // namespace coding_nerd::boot_perf

#endif  // DUMPER_H_
