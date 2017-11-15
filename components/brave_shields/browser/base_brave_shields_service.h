/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_SHIELDS_BROWSER_BASE_BRAVE_SHIELDS_SERVICE_H_
#define BRAVE_COMPONENTS_BRAVE_SHIELDS_BROWSER_BASE_BRAVE_SHIELDS_SERVICE_H_

#include <stdint.h>

#include <memory>
#include <string>
#include <vector>
#include <mutex>

#include "content/public/common/resource_type.h"

namespace brave_shields {

// The brave shields service in charge of checking brave shields like ad-block,
// tracking protection, etc.
class BaseBraveShieldsService {
 public:
   BaseBraveShieldsService();
   virtual ~BaseBraveShieldsService();
   bool Start();
   void Stop();
   virtual bool Check(const std::string &spec,
    content::ResourceType resource_type,
    const std::string &initiator_host) = 0;

 protected:
   virtual bool Init() = 0;
   virtual void Cleanup() = 0;

 private:
  bool initialized_;
  std::mutex init_mutex_;
  std::mutex initialized_mutex_;
};

}  // namespace brave_shields

#endif  // BRAVE_COMPONENTS_BRAVE_SHIELDS_BROWSER_BASE_BRAVE_SHIELDS_SERVICE_H_