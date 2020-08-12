// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef GUETZLI_SANDBOXED_GUETZLI_TRANSACTION_H_
#define GUETZLI_SANDBOXED_GUETZLI_TRANSACTION_H_

#include <libgen.h>
#include <syscall.h>

#include "sandboxed_api/transaction.h"
#include "sandboxed_api/vars.h"

#include "guetzli_sandbox.h"

namespace guetzli {
namespace sandbox {

enum class ImageType {
  kJpeg,
  kPng
};

struct TransactionParams {
  int in_fd = -1;
  int out_fd = -1;
  int verbose = 0;
  int quality = 0;
  int memlimit_mb = 0;
};

// Instance of this transaction shouldn't be reused
// Create a new one for each processing operation
class GuetzliTransaction : public sapi::Transaction {
 public:
  GuetzliTransaction(TransactionParams&& params)
      : sapi::Transaction(std::make_unique<GuetzliSapiSandbox>())
      , params_(std::move(params))
      , in_fd_(params_.in_fd)
      , out_fd_(params_.out_fd)
  {
    //TODO: Add retry count as a parameter
    sapi::Transaction::set_retry_count(kDefaultTransactionRetryCount);
    //TODO: Try to use sandbox().set_wall_limit instead of infinite time limit
    sapi::Transaction::SetTimeLimit(0);
  }

 private:
  absl::Status Init() override;
  absl::Status Main() final;

  sapi::StatusOr<ImageType> GetImageTypeFromFd(int fd) const;

  // As guetzli takes roughly 1 minute of CPU per 1 MPix we need to calculate 
  // approximate time for transaction to complete
  time_t CalculateTimeLimitFromImageSize(uint64_t pixels) const;

  const TransactionParams params_;
  sapi::v::Fd in_fd_;
  sapi::v::Fd out_fd_;
  ImageType image_type_ = ImageType::kJpeg;

  static const int kDefaultTransactionRetryCount = 0;
  static const uint64_t kMpixPixels = 1'000'000;
};

}  // namespace sandbox
}  // namespace guetzli

#endif  // GUETZLI_SANDBOXED_GUETZLI_TRANSACTION_H_
