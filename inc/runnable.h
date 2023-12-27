#ifndef RUNNABLE_H_
#define RUNNABLE_H_

namespace coding_nerd::boot_perf {

class Runnable {
  using type = Runnable;

 public:
  Runnable() = default;
  Runnable(Runnable&& other) = default;
  Runnable(Runnable& other) = delete;
  Runnable& operator=(Runnable& other) = delete;
  virtual ~Runnable() = default;
  virtual void Run() = 0;
};
}  // namespace coding_nerd::boot_perf

#endif  // RUNNABLE_H_
