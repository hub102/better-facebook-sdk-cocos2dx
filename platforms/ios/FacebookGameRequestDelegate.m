//
//  FacebookGameRequestDelegate.m
//  FacebookXTest
//
//  Created by Thuy Dong Xuan on 4/14/16.
//
//

#import "FacebookGameRequestDelegate.h"

@implementation FacebookGameRequestDelegate {
  void(^succeedCallback)(FBSDKGameRequestDialog*, NSDictionary*);
  void(^failedCallback)(FBSDKGameRequestDialog*, NSError *);
  void(^cancelCallback)(FBSDKGameRequestDialog*);
}

- (id)initWithSucceedHandler:(void (^)(FBSDKGameRequestDialog*, NSDictionary *))succeedHandler
               failedHandler:(void (^)(FBSDKGameRequestDialog*, NSError *))failedHandler
               cancelHandler:(void (^)(FBSDKGameRequestDialog*))cancelHandler {
  if ([super init]) {
    succeedCallback = succeedHandler;
    failedCallback = failedHandler;
    cancelCallback = cancelHandler;
  }
  
  return self;
}

- (void)setSucceedHandler:(void (^)(FBSDKGameRequestDialog*, NSDictionary*))succeedHandler {
  succeedCallback = succeedHandler;
}

- (void)setFailedHandler:(void (^)(FBSDKGameRequestDialog*, NSError*))failedHandler {
  failedCallback = failedHandler;
}

- (void)setCancelHandler:(void (^)(FBSDKGameRequestDialog*))cancelHandler {
  cancelCallback = cancelHandler;
}

- (void)gameRequestDialog:(FBSDKGameRequestDialog *)gameRequestDialog didCompleteWithResults:(NSDictionary *)results {
  succeedCallback(gameRequestDialog, results);
}

- (void)gameRequestDialog:(FBSDKGameRequestDialog *)gameRequestDialog didFailWithError:(NSError *)error {
  failedCallback(gameRequestDialog, error);
}

- (void)gameRequestDialogDidCancel:(FBSDKGameRequestDialog *)gameRequestDialog {
  cancelCallback(gameRequestDialog);
}

@end
