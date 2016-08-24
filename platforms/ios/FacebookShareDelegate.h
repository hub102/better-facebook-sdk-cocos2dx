//
//  FacebookShareDelegate.h
//  FacebookXTest
//
//  Created by Thuy Dong Xuan on 4/13/16.
//
//

#import <FBSDKShareKit/FBSDKShareKit.h>

@interface FacebookShareDelegate : NSObject<FBSDKSharingDelegate>

- (id)initWithSucceedHandler:(void (^)(id<FBSDKSharing> sharer, NSDictionary* result))succeedHandler
               failedHandler:(void (^)(id<FBSDKSharing> sharer, NSError* error))failedHandler
               cancelHandler:(void (^)(id<FBSDKSharing> sharer)) cancelHandler;

- (void)setSucceedHandler:(void (^)(id<FBSDKSharing>, NSDictionary*))succeedHandler;

- (void)setFailedHandler:(void (^)(id<FBSDKSharing>, NSError*))failedHandler;

- (void)setCancelHandler:(void (^)(id<FBSDKSharing>))cancelHandler;

@end
