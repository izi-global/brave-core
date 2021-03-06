/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/brave_profile_prefs.h"

#include "brave/common/pref_names.h"
#include "brave/components/brave_shields/browser/brave_shields_web_contents_observer.h"
#include "chrome/browser/prefs/session_startup_pref.h"
#include "chrome/common/pref_names.h"
#include "components/content_settings/core/common/pref_names.h"
#include "components/pref_registry/pref_registry_syncable.h"
#include "components/safe_browsing/common/safe_browsing_prefs.h"
#include "components/signin/core/browser/signin_pref_names.h"
#include "components/spellcheck/browser/pref_names.h"

namespace brave {

void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry) {
  brave_shields::BraveShieldsWebContentsObserver::RegisterProfilePrefs(registry);

  registry->RegisterBooleanPref(kWidevineOptedIn, false);

  // No sign into Brave functionality
  registry->SetDefaultPrefValue(prefs::kSigninAllowed, base::Value(false));

  // Restore last profile on restart
  registry->SetDefaultPrefValue(prefs::kRestoreOnStartup,
      base::Value(SessionStartupPref::kPrefValueLast));

  // Show download prompt by default
  registry->SetDefaultPrefValue(prefs::kPromptForDownload, base::Value(true));

  // Not using chrome's web service for resolving navigation errors
  registry->SetDefaultPrefValue(prefs::kAlternateErrorPagesEnabled, base::Value(false));

  // Disable spell check service
  registry->SetDefaultPrefValue(spellcheck::prefs::kSpellCheckUseSpellingService, base::Value(false));

  // Disable safebrowsing reporting
  registry->SetDefaultPrefValue(prefs::kSafeBrowsingExtendedReportingOptInAllowed, base::Value(false));

  // Disable search suggestion
  registry->SetDefaultPrefValue(prefs::kSearchSuggestEnabled, base::Value(false));
}

}  // namespace brave
