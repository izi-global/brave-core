#include "brave/browser/ui/webui/sync/sync_ui_impl.h"

#include "base/bind.h"
#include "base/values.h"

#include "content/public/browser/web_ui_message_handler.h"

#include "brave/components/brave_sync/brave_sync_controller.h"
#include "brave/components/brave_sync/brave_sync_controller_impl.h"
#include "brave/components/brave_sync/brave_sync_settings.h"
#include "brave/components/brave_sync/brave_sync_devices.h"
#include "brave/components/brave_sync/values_conv.h"
#include "brave/components/brave_sync/value_debug.h"

SyncUIImpl::SyncUIImpl(content::WebUI* web_ui, const std::string& host,
    const std::string& js_file, int js_resource_id, int html_resource_id)
    : BasicUI(web_ui, host, js_file, js_resource_id, html_resource_id) {
LOG(ERROR) << "TAGAB SyncUIImpl::SyncUIImpl CTOR";

  RegisterCallbacks();

  sync_controller_ = static_cast<brave_sync::BraveSyncController*>(brave_sync::BraveSyncControllerImpl::GetInstance());
  sync_controller_->SetupUi(this);
}

SyncUIImpl::~SyncUIImpl() {
LOG(ERROR) << "TAGAB SyncUIImpl::~SyncUIImpl DTOR";
}

void SyncUIImpl::RegisterCallbacks() {
  this->web_ui()->RegisterMessageCallback("setupSyncHaveCode",
     base::Bind(&SyncUIImpl::SetupSyncHaveCode,
                base::Unretained(this)));

  this->web_ui()->RegisterMessageCallback("setupSyncNewToSync",
     base::Bind(&SyncUIImpl::SetupSyncNewToSync,
                base::Unretained(this)));

  this->web_ui()->RegisterMessageCallback("pageLoaded",
     base::Bind(&SyncUIImpl::PageLoaded,
                base::Unretained(this)));

  this->web_ui()->RegisterMessageCallback("needSyncWords",
     base::Bind(&SyncUIImpl::NeedSyncWords,
                base::Unretained(this)));

  this->web_ui()->RegisterMessageCallback("needSyncQRcode",
     base::Bind(&SyncUIImpl::NeedSyncQRcode,
                base::Unretained(this)));

  this->web_ui()->RegisterMessageCallback("deleteDevice",
     base::Bind(&SyncUIImpl::DeleteDevice,
                base::Unretained(this)));

  this->web_ui()->RegisterMessageCallback("resetSync",
     base::Bind(&SyncUIImpl::ResetSync,
                base::Unretained(this)));
}

void SyncUIImpl::SetupSyncHaveCode(const base::ListValue* args) {
  LOG(ERROR) << "SyncUIImpl::SetupSyncHaveCode";
  std::string sync_words, device_name;
  if (!args->GetString(0, &sync_words) || !args->GetString(1, &device_name))
   return;

  LOG(ERROR) << "SyncUIImpl::SetupSyncHaveCode sync_words=" << sync_words;
  LOG(ERROR) << "SyncUIImpl::SetupSyncHaveCode device_name=" << device_name;

  sync_controller_->OnSetupSyncHaveCode(sync_words, device_name);
}

void SyncUIImpl::SetupSyncNewToSync(const base::ListValue* args) {
  LOG(ERROR) << "SyncUIImpl::SetupSyncNewToSync";
  std::string sync_words, device_name;
  if (!args->GetString(0, &device_name)) {
   return;
  }

  LOG(ERROR) << "SyncUIImpl::SetupSyncHaveCode device_name=" << device_name;

  sync_controller_->OnSetupSyncNewToSync(device_name);
}

void SyncUIImpl::OnSyncStateChanged() {
LOG(ERROR) << "SyncUIImpl::OnSyncStateChanged";
  LoadSyncSettingsView();
}

void SyncUIImpl::OnHaveSyncWords(const std::string &sync_words) {
LOG(ERROR) << "SyncUIImpl::OnHaveSyncWords sync_words="<<sync_words;
  web_ui()->CallJavascriptFunctionUnsafe("sync_ui_exports.haveSyncWords", base::Value(sync_words));
}

void SyncUIImpl::LoadSyncSettingsView() {
LOG(ERROR) << "SyncUIImpl::LoadSyncSettingsView";
  brave_sync::BraveSyncSettings settings;
  sync_controller_->GetSettings(settings);

  brave_sync::SyncDevices devices;
  sync_controller_->GetDevices(devices);
  std::unique_ptr<base::Value> bv_devices = devices.ToValueArrOnly();

LOG(ERROR) << "SyncUIImpl::LoadSyncSettingsView bv_devices: " << brave::debug::ToPrintableString(*bv_devices);

  std::unique_ptr<base::Value> bv_settings = brave_sync::BraveSyncSettingsToValue(&settings);
  web_ui()->CallJavascriptFunctionUnsafe("sync_ui_exports.showSettings", *bv_settings, *bv_devices);
}

void SyncUIImpl::PageLoaded(const base::ListValue* args) {
LOG(ERROR) << "SyncUIImpl::PageLoaded";
  LoadSyncSettingsView();
}

void SyncUIImpl::NeedSyncWords(const base::ListValue* args) {
  LOG(ERROR) << "SyncUIImpl::NeedSyncWords";
  sync_controller_->GetSyncWords();
  // sync_controller will fire async sync_ui_exports.haveSyncWords when it will
  // have the words ready
}

void SyncUIImpl::NeedSyncQRcode(const base::ListValue* args) {
  LOG(ERROR) << "SyncUIImpl::NeedSyncQRcode";
  std::string seed = sync_controller_->GetSeed();
  web_ui()->CallJavascriptFunctionUnsafe("sync_ui_exports.haveSeedForQrCode", base::Value(seed));
}

void SyncUIImpl::DeleteDevice(const base::ListValue* args) {
  LOG(ERROR) << "SyncUIImpl::DeleteDevice args=" << brave::debug::ToPrintableString(*args);

  std::string device_id;
  if (!args->GetString(0, &device_id)) {
    return;
  }
  LOG(ERROR) << "SyncUIImpl::DeleteDevice device_id=" << device_id;
  sync_controller_->OnDeleteDevice(device_id);
}

void SyncUIImpl::ResetSync(const base::ListValue* args) {
  LOG(ERROR) << "SyncUIImpl::ResetSync args=" << brave::debug::ToPrintableString(*args);

  sync_controller_->OnResetSync();
}
