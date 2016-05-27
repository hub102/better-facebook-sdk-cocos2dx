//
//  FacebookGameRequestDelegate.h
//  FacebookXTest
//
//  Created by Thuy Dong Xuan on 4/14/16.
//
//

#import <FBSDKShareKit/FBSDKShareKit.h>

@interface FacebookGameRequestDelegate : NSObject<FBSDKGameRequestDialogDelegate>

- (id)initWithSucceedHandler:(void (^)(FBSDKGameRequestDialog*, NSDictionary*))succeedHandler
               failedHandler:(void (^)(FBSDKGameRequestDialog*, NSError*))failedHandler
               cancelHandler:(void (^)(FBSDKGameRequestDialog*)) cancelHandler;

- (void)setSucceedHandler:(void (^)(FBSDKGameRequestDialog*, NSDictionary*))succeedHandler;

- (void)setFailedHandler:(void (^)(FBSDKGameRequestDialog*, NSError*))failedHandler;

- (void)setCancelHandler:(void (^)(FBSDKGameRequestDialog*))cancelHandler;

@end