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

#ifndef GUETZLI_SANDBOXED_GUETZLI_SANDBOX_H_
#define GUETZLI_SANDBOXED_GUETZLI_SANDBOX_H_

#include <libgen.h>
#include <syscall.h>

#include "sandboxed_api/sandbox2/policy.h"
#include "sandboxed_api/sandbox2/policybuilder.h"
#include "sandboxed_api/util/flag.h"

#include "guetzli_sapi.sapi.h"

namespace guetzli {
namespace sandbox {

class GuetzliSapiSandbox : public GuetzliSandbox {
  public:
  std::unique_ptr<sandbox2::Policy> ModifyPolicy(
      sandbox2::PolicyBuilder*) override {

      return sandbox2::PolicyBuilder()
        .AllowStaticStartup()
        .AllowRead()
        .AllowSystemMalloc()
        .AllowWrite()
        .AllowExit()
        .AllowStat()
        .AllowSyscalls({
          __NR_futex,
          __NR_close,
          __NR_recvmsg // Seems like this one needed to work with remote file descriptors
        })
        .BuildOrDie();
    }
};

}  // namespace sandbox
}  // namespace guetzli

#endif  // GUETZLI_SANDBOXED_GUETZLI_SANDBOX_H_
