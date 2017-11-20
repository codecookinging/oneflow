#ifndef ONEFLOW_CORE_THREAD_GPU_THREAD_H_
#define ONEFLOW_CORE_THREAD_GPU_THREAD_H_

#include "oneflow/core/thread/thread.h"

namespace oneflow {

class GpuThread final : public Thread {
 public:
  OF_DISALLOW_COPY_AND_MOVE(GpuThread);
  GpuThread() = delete;
  ~GpuThread() = default;

  GpuThread(int64_t thrd_id, int64_t dev_id);

 private:
};

}  // namespace oneflow

#endif  // ONEFLOW_CORE_THREAD_GPU_THREAD_H_
