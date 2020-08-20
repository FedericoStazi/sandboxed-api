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

#include <syscall.h>

#include <cstdlib>

#include "curl_sapi.sapi.h"
#include "sandboxed_api/util/flag.h"

class CurlSapiSandbox : public CurlSandbox {
 protected:
  std::unique_ptr<sandbox2::Policy> ModifyPolicy(
      sandbox2::PolicyBuilder* policy_builder) override {
    // Return a new policy
    return (*policy_builder)
        .AllowDynamicStartup()
        .AllowExit()
        .AllowFork()
        .AllowHandleSignals()
        .AllowOpen()
        .AllowRead()
        .AllowSafeFcntl()
        .AllowWrite()
        .AllowSyscalls({
          __NR_accept,
          __NR_access,
          __NR_bind,
          __NR_connect,
          __NR_futex,
          __NR_getpeername,
          __NR_getsockname,
          __NR_getsockopt,
          __NR_ioctl,
          __NR_listen,
          __NR_madvise,
          __NR_mmap,
          __NR_poll,
          __NR_recvfrom,
          __NR_recvmsg,
          __NR_sendmmsg,
          __NR_sendto,
          __NR_setsockopt,
          __NR_socket
        })
        .AllowUnrestrictedNetworking()
        .AddDirectory("/lib")
        .BuildOrDie();
  }
};
