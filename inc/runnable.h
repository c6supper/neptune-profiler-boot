#ifndef RUNNABLE_H_
#define RUNNABLE_H_

namespace coding_nerd::boot_perf {

class Runnable {
  using type = Runnable;

 public:
  Runnable() = default;
  virtual ~Runnable() = 0;
  virtual void run() = 0;
};
}  // namespace coding_nerd::boot_perf

#endif  // RUNNABLE_H_
