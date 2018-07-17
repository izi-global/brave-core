/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_OBJ_MAP_H_
#define BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_OBJ_MAP_H_

#include <string>

namespace brave_sync {
namespace storage {

class BraveSyncObjMap {
public:
  BraveSyncObjMap();
  ~BraveSyncObjMap();

  std::string GetLocalIdByObjectId(const std::string &object_id);

  std::string GetObjectIdByLocalId(const std::string &localId);

  void SaveObjectId(
        const std::string &localId,
        const std::string &objectIdJSON,  //may be an order or empty
        const std::string &objectId);
  void DeleteByLocalId(const std::string &localId);
  void Close();
  void CloseDBHandle();
  void ResetSync(const std::string& key);
  void DestroyDB();
};

} // namespace storage
} // namespace brave_sync

#endif //BRAVE_COMPONENTS_BRAVE_SYNC_BRAVE_SYNC_OBJ_MAP_H_
