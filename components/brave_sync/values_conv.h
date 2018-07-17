/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef BRAVE_COMPONENTS_BRAVE_SYNC_VALUES_CONV_H_
#define BRAVE_COMPONENTS_BRAVE_SYNC_VALUES_CONV_H_

#include <memory>
#include <vector>
#include <string>

namespace base {
  class Value;
}

namespace brave_sync {

struct BraveSyncSettings;

std::unique_ptr<base::Value> BraveSyncSettingsToValue(BraveSyncSettings *brave_sync_settings);

std::string ExtractObjectId(const base::Value *val);

// see brave-sync/lib/api.proto

// exported
std::unique_ptr<base::Value> CreateBookmarkSyncRecordValue(
  int action, // kActionCreate/kActionUpdate/kActionDelete 0/1/2
  const std::string &device_id,
  const std::string &object_id,
  //object data - site
  const std::string &location,
  const std::string &title,
  const std::string &customTitle,
  const uint64_t &lastAccessedTime,
  const uint64_t &creationTime,
  const std::string &favicon,
  //object data - bookmark
  bool isFolder,
  const std::string &parentFolderObjectId,
  //repeated string fields = 6;
  bool hideInToolbar,
  const std::string &order
);

std::unique_ptr<base::Value> VecToListValue(const std::vector<char> &v);
std::unique_ptr<base::Value> BytesListFromString(const std::string &data_string);
std::unique_ptr<base::Value> SingleIntStrToListValue(const std::string &data_string);

std::string GetAction(const base::Value &sync_record);

std::string ExtractBookmarkLocation(const base::Value *sync_record);
std::string ExtractBookmarkTitle(const base::Value *sync_record);

} // namespace brave_sync

#endif //BRAVE_COMPONENTS_BRAVE_SYNC_VALUES_CONV_H_
