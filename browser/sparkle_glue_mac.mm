/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#import "brave/browser/sparkle_glue_mac.h"

#include <string>

#include "base/mac/bundle_locations.h"
#include "base/strings/sys_string_conversions.h"
#include "brave/browser/update_util.h"

namespace {

std::string GetDescriptionFromAppcastItem(id item) {
  NSString* description =
      [NSString stringWithFormat:@"AppcastItem(Date: %@, Version: %@)",
          [item performSelector:@selector(dateString)],
          [item performSelector:@selector(versionString)]];
  return [description UTF8String];
}

id GetSUUpdater() {
  NSString* sparkle_path =
      [[base::mac::FrameworkBundle() privateFrameworksPath]
          stringByAppendingPathComponent:@"Sparkle.framework"];
  DCHECK(sparkle_path);

  NSBundle* sparkle_bundle = [NSBundle bundleWithPath:sparkle_path];
  [sparkle_bundle load];

  Class sparkle_class = [sparkle_bundle classNamed:@"SUUpdater"];
  return [sparkle_class performSelector:@selector(sharedUpdater)];
}

}  // namespace

@implementation SparkleGlue
{
  id su_updater_;
}

#pragma mark - SparkleGlue

+ (instancetype)sharedSparkleGlue {
  static SparkleGlue* shared;
  if (brave::UpdateEnabled() && shared == nil)
    shared = [[SparkleGlue alloc] init];
  return shared;
}

- (instancetype)init {
  if (self = [super init]) {
    su_updater_ = GetSUUpdater();
    return self;
  } else {
    return nil;
  }
}

- (void)initializeBraveUpdater {
  DCHECK(brave::UpdateEnabled());

  [self setDelegate:self];
  [self setAutomaticallyChecksForUpdates:YES];
  [self setAutomaticallyDownloadsUpdates:YES];

  // Background update check interval.
  // TODO(simonhong): 3 hour inverval is ok?
  constexpr int kBraveUpdateCheckIntervalInSec = 3 * 60 * 60;
  [self setUpdateCheckInterval:kBraveUpdateCheckIntervalInSec];

  // Start background update.
  [self performSelector:@selector(checkForUpdatesInBackground)];
}

- (void)setDelegate:(id)delegate {
  [su_updater_ setDelegate:delegate];
}

- (void)checkForUpdates:(id)__unused sender {
  [su_updater_ checkForUpdates:nil];
}

- (void)checkForUpdatesInBackground {
  [su_updater_ checkForUpdatesInBackground];
}

- (void)setUpdateCheckInterval:(NSTimeInterval)interval {
  [su_updater_ setUpdateCheckInterval:interval];
}

- (void)setAutomaticallyChecksForUpdates:(BOOL)enable {
  [su_updater_ setAutomaticallyChecksForUpdates:enable];
}

- (void)setAutomaticallyDownloadsUpdates:(BOOL)enable {
  [su_updater_ setAutomaticallyDownloadsUpdates:enable];
}

#pragma mark - SUUpdaterDelegate

- (void)updater:(id)updater didFinishLoadingAppcast:(id)appcast {
  VLOG(0) << "brave update: did finish loading appcast";
}

- (void)updater:(id)updater didFindValidUpdate:(id)item {
  VLOG(0) << "brave update: did finish valid update with " +
             GetDescriptionFromAppcastItem(item);
}

- (void)updaterDidNotFindUpdate:(id)updater {
  VLOG(0) << "brave update: did not find update";
}

- (void)updater:(id)updater
    willDownloadUpdate:(id)item
           withRequest:(NSMutableURLRequest *)request {
  VLOG(0) << "brave update: willDownloadUpdate with " +
             GetDescriptionFromAppcastItem(item);
}

- (void)updater:(id)updater
    failedToDownloadUpdate:(id)item
                     error:(NSError *)error {
  VLOG(0) << "brave update: failed to download update with " +
             GetDescriptionFromAppcastItem(item) +
             " with error - " + [[error description] UTF8String];
}

- (void)userDidCancelDownload:(id)updater {
  VLOG(0) << "brave update: user did cancel download";
}

- (void)updater:(id)updater willInstallUpdate:(id)item {
  VLOG(0) << "brave update: will install update with " +
             GetDescriptionFromAppcastItem(item);
}

- (void)updaterWillRelaunchApplication:(id)updater {
  VLOG(0) << "brave update: will relaunch application";
}

- (void)updaterDidRelaunchApplication:(id)updater {
  VLOG(0) << "brave update: did relaunch application";
}

- (void)updater:(id)updater
    willInstallUpdateOnQuit:(id)item
    immediateInstallationInvocation:(NSInvocation *)invocation {
  VLOG(0) << "brave update: will install update on quit with " +
             GetDescriptionFromAppcastItem(item);
}

- (void)updater:(id)updater didCancelInstallUpdateOnQuit:(id)item {
  VLOG(0) << "brave update: did cancel install update on quit with " +
             GetDescriptionFromAppcastItem(item);
}

- (void)updater:(id)updater didAbortWithError:(NSError *)error {
  VLOG(0) << [[error description] UTF8String];
}
@end
