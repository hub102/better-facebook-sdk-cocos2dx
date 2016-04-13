//
//  FacebookShareDelegate.m
//  FacebookXTest
//
//  Created by Thuy Dong Xuan on 4/13/16.
//
//

#import "FacebookShareDelegate.h"

@implementation FacebookShareDelegate {
  void(^succeedCallback)(id<FBSDKSharing>, NSDictionary*);
  void(^failedCallback)(id<FBSDKSharing>, NSError *);
  void(^cancelCallback)(id<FBSDKSharing>);
}

- (id)initWithSucceedHandler:(void (^)(id<FBSDKSharing>, NSDictionary *))succeedHandler
               failedHandler:(void (^)(id<FBSDKSharing>, NSError *))failedHandler
               cancelHandler:(void (^)(id<FBSDKSharing>))cancelHandler {
  if ([super init]) {
    succeedCallback = succeedHandler;
    failedCallback = failedHandler;
    cancelCallback = cancelHandler;
  }
  
  return self;
}

- (void)setSucceedHandler:(void (^)(id<FBSDKSharing>, NSDictionary *))succeedHandler {
  succeedCallback = succeedHandler;
}

- (void)setFailedHandler:(void (^)(id<FBSDKSharing>, NSError *))failedHandler {
  failedCallback = failedHandler;
}

- (void)setCancelHandler:(void (^)(id<FBSDKSharing>))cancelHandler {
  cancelCallback = cancelHandler;
}

- (void)sharer:(id<FBSDKSharing>)sharer didCompleteWithResults:(NSDictionary *)results {
  succeedCallback(sharer, results);
}

- (void)sharer:(id<FBSDKSharing>)sharer didFailWithError:(NSError *)error {
  failedCallback(sharer, error);
}

- (void)sharerDidCancel:(id<FBSDKSharing>)sharer {
  cancelCallback(sharer);
}

@end
