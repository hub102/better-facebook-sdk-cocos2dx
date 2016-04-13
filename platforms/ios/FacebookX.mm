//
//  FacebookX.m
//  ;
//
//  Created by Thuy Dong Xuan on 4/12/16.
//
//

#import <Foundation/Foundation.h>

#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKLoginKit/FBSDKLoginKit.h>
#import <FBSDKShareKit/FBSDKShareKit.h>

#import "UIViewController+Utils.h"
#import "FacebookShareDelegate.h"

#import "FacebookX.hpp"

using namespace std;

namespace h102 {
	void FacebookX::login() {
        vector<string> permissions;
        permissions.push_back(h102::FB_PERM_READ_PUBLIC_PROFILE);
        permissions.push_back(h102::FB_PERM_READ_EMAIL);
        permissions.push_back(h102::FB_PERM_READ_USER_FRIENDS);
        login(permissions);
    }
  
  	void FacebookX::login( std::vector<std::string>& permissions ) {
		FBSDKLoginManager *login = [[FBSDKLoginManager alloc] init];
	
		NSMutableArray *permissionArray = [NSMutableArray new];
		for (auto str : permissions) {
	  		id nsstr = [NSString stringWithUTF8String:str.c_str()];
	  		[permissionArray addObject:nsstr];
		}
	
		[login logInWithReadPermissions: permissionArray
				fromViewController:[UIViewController topViewController]
							handler:^(FBSDKLoginManagerLoginResult *result, NSError *error) {
			if (error) {
				if (FacebookX::listener) {
					listener->onLogin(false, error.localizedDescription.UTF8String);
				}
			} else if (result.isCancelled) {
				if (FacebookX::listener) {
				   	listener->onLogin(false, "Cancelled");
				}
		   	} else {
                [FBSDKProfile enableUpdatesOnAccessTokenChange:YES];
				listener->onLogin(true, "LoggedIn");
			}
		}];
  	}

    std::string FacebookX::getAccessToken() {
        return std::string([[[FBSDKAccessToken currentAccessToken] tokenString] UTF8String]);
    }
    
    std::string FacebookX::getUserID() {
        auto profile = [FBSDKProfile currentProfile];
        return std::string([[FBSDKProfile currentProfile].userID UTF8String]);
    }
    
    bool FacebookX::isLoggedIn() {
        return [[FBSDKAccessToken currentAccessToken] tokenString] != nil;
    }
    
    void FacebookX::logout() {
        [[[FBSDKLoginManager alloc] init] logOut];
    }

	vector<string> FacebookX::getPermissionList() {
    	vector<string> permissions;
    
	    FBSDKAccessToken* token = [FBSDKAccessToken currentAccessToken];
	    if (token) {
	      NSSet* permissionSet = [token permissions];
	      for (NSString* p in permissionSet) {
	        permissions.push_back([p UTF8String]);
	      }
	    }
	    
	    return permissions;
  	}
  
    void FacebookX::share(const FBShareInfo& info) {
        if (info.type == FB_NONE)
            return;
    
        id<FBSDKSharingContent> shareContent;
        if (info.type == FB_LINK) {
            FBSDKShareLinkContent* content = [[FBSDKShareLinkContent alloc] init];
            content.contentTitle = [NSString stringWithUTF8String:info.title.c_str()];
            content.contentDescription = [NSString stringWithUTF8String:info.text.c_str()];
            content.contentURL = [NSURL URLWithString:[NSString stringWithUTF8String:info.link.c_str()]];
            if (info.media.compare("") != 0)
                content.imageURL = [NSURL URLWithString:[NSString stringWithUTF8String:info.media.c_str()]];
      
            shareContent = content;
        }
        else if (info.type == FB_PHOTO) {
            FBSDKSharePhoto* photo = [[FBSDKSharePhoto alloc] init];
            photo.image = [UIImage imageWithContentsOfFile:[NSString stringWithUTF8String:info.media.c_str()]];
            photo.userGenerated = YES;

            FBSDKSharePhotoContent* content = [[FBSDKSharePhotoContent alloc] init];
            content.photos = @[photo];

            shareContent = content;
        }
        else if (info.type == FB_VIDEO) {
            FBSDKShareVideo* video = [[FBSDKShareVideo alloc] init];
            video.videoURL = [NSURL URLWithString:[NSString stringWithUTF8String:info.media.c_str()]];
          
            FBSDKShareVideoContent* content = [[FBSDKShareVideoContent alloc] init];
            content.video = video;
          
            shareContent = content;
        }
    
        FacebookShareDelegate* delegate = [[FacebookShareDelegate alloc] initWithSucceedHandler:^(id<FBSDKSharing> sharer, NSDictionary *result) {
            NSString* ret = @"";
            if ([result objectForKey:@"postId"])
                ret = [ret stringByAppendingString:[NSString stringWithFormat:@"{\"postId\":\"%@\"}", [result objectForKey:@"postId"]]];
            listener->onSharedSuccess([ret UTF8String]);
        } failedHandler:^(id<FBSDKSharing> sharer, NSError *error) {
            listener->onSharedFailed([error.localizedDescription UTF8String]);
        } cancelHandler:^(id<FBSDKSharing> sharer) {
            listener->onSharedCancel();
        }];
    
        [FBSDKShareDialog showFromViewController:[UIViewController topViewController]
                                     withContent:shareContent
                                        delegate:delegate];
    }
}