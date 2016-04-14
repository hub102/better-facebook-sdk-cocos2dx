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
#import "FacebookX-impl.h"

using namespace std;

#import <iostream>

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
        return [[[FBSDKAccessToken currentAccessToken] tokenString] UTF8String];
    }
    
    std::string FacebookX::getUserID() {
        return [[[FBSDKAccessToken currentAccessToken] userID] UTF8String];
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
    
        id<FBSDKSharingContent> content = [FacebookXImpl getContent:info];
    
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
                                     withContent:content
                                        delegate:delegate];
    }
    
    bool FacebookX::canPresentWithFBApp(const h102::FBShareInfo &info) {
        id<FBSDKSharingContent> content = [FacebookXImpl getContent:info];
        
        FBSDKShareDialog *dialog = [[FBSDKShareDialog alloc] init];
        dialog.fromViewController = [UIViewController topViewController];
        dialog.shareContent = content;
        dialog.mode = FBSDKShareDialogModeShareSheet;
        
        return [dialog canShow];
    }
    
    void FacebookX::api(const std::string& path, const std::string& tag) {
        if ([FBSDKAccessToken currentAccessToken]) {
            NSString* _tag = [NSString stringWithUTF8String:tag.c_str()];
            NSString* _path = [NSString stringWithUTF8String:path.c_str()];
            [[[FBSDKGraphRequest alloc] initWithGraphPath:_path parameters:nil]
                          startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
                              if (!error) {
                                  NSLog(@"API executed OK!!, tag = %@", _tag);
                                  if (FacebookX::listener) {
                                      NSString* stringifiedResult = [NSString stringWithFormat:@"%@", result];
                                      std::string cStringResult = [stringifiedResult UTF8String];
                                      listener->onAPI([_tag UTF8String], cStringResult);
                                  }
                              }
                          }];
        }
    }
    
    void FacebookX::api(const std::string& path, const FBAPIParam& params, const std::string& tag) {
        if ([FBSDKAccessToken currentAccessToken]) {
            NSString* _tag = [NSString stringWithUTF8String:tag.c_str()];
            NSString* _path = [NSString stringWithUTF8String:path.c_str()];
            NSMutableDictionary* _params = [NSMutableDictionary dictionary];
            for (auto i = params.begin(); i != params.end(); i++) {
                NSString* key = (0 == i->first.length()) ? @"" : @(i->first.c_str());
                NSString* value = (0 == i->second.length()) ? @"" : @(i->second.c_str());
                [_params setObject:value forKey:key];
            }
            
            [[[FBSDKGraphRequest alloc] initWithGraphPath:_path parameters:_params]
                          startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
                              if (!error) {
                                  NSLog(@"API executed OK!!");
                                  if (FacebookX::listener) {
                                      NSString* stringifiedResult = [NSString stringWithFormat:@"%@", result];
                                      std::string cStringResult = [stringifiedResult UTF8String];
                                      listener->onAPI([_tag UTF8String], cStringResult);
                                  }
                              }
                          }];
        }
    }
    
    void FacebookX::api(const std::string& path, const std::string& method, const FBAPIParam& params, const std::string& tag)
    {
        if ([FBSDKAccessToken currentAccessToken]) {
            NSString* _tag = [NSString stringWithUTF8String:tag.c_str()];
            NSString* _path = [NSString stringWithUTF8String:path.c_str()];
            NSString* _method = [NSString stringWithUTF8String:method.c_str()];
            NSMutableDictionary* _params = [NSMutableDictionary dictionary];
            for (auto i = params.begin(); i != params.end(); i++) {
                NSString* key = (0 == i->first.length()) ? @"" : @(i->first.c_str());
                NSString* value = (0 == i->second.length()) ? @"" : @(i->second.c_str());
                [_params setObject:value forKey:key];
            }
            
            [[[FBSDKGraphRequest alloc] initWithGraphPath:_path parameters:_params HTTPMethod:_method]
                startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
                 if (!error) {
                     NSLog(@"API executed OK!!");
                     if (FacebookX::listener) {
                         NSString* stringifiedResult = [NSString stringWithFormat:@"%@", result];
                         std::string cStringResult = [stringifiedResult UTF8String];
                         listener->onAPI([_tag UTF8String], cStringResult);
                     }
                 } else
                     NSLog(@"%@", error);
             }];
        }
    }
    
    void FacebookX::requestInvitableFriends(const FBAPIParam &params) {
        NSMutableDictionary* _params = [NSMutableDictionary dictionary];
        if (params.size() > 0) {
            if (params.find(kRI_ResponseFields) != params.end())
                [_params setObject:@(params.at(kRI_ResponseFields).c_str()) forKey:@"fields"];
            else
                [_params setObject:@"name,id,picture" forKey:@"fields"];
            
            if (params.find(kRI_PictureSize) != params.end()) {
                NSString* _fields = [_params objectForKey:@"fields"];
                if (_fields == nil)
                    _fields = @"";
                else
                    _fields = [_fields stringByAppendingString:@","];
                
                _fields = [_fields stringByAppendingString:[NSString stringWithFormat:@"picture.width(\"%s\")", params.at(kRI_PictureSize).c_str()]];
                [_params setObject:_fields forKey:@"fields"];
            }
            
            if (params.find(kRI_PaginationLimit) != params.end())
                [_params setObject:@(params.at(kRI_PaginationLimit).c_str()) forKey:@"limit"];
            
            if (params.find(kRI_ExcludeFromList) != params.end())
                [_params setObject:@(params.at(kRI_ExcludeFromList).c_str()) forKey:@"excluded_ids"];
        }
        else
            [_params setObject:@"name,picture.width(300)" forKey:@"fields"];
        
        [[[FBSDKGraphRequest alloc] initWithGraphPath:@"me/invitable_friends"
                                           parameters:_params
                                           HTTPMethod:@"GET"]
         startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
             
             if (!error) {
                 NSLog(@"API executed OK!!");
                 if (FacebookX::listener) {
                     NSError *error;
                     NSData *jsonData = [NSJSONSerialization dataWithJSONObject:result
                                                                        options:0 // Pass 0 if you don't care about the readability of the generated string
                                                                          error:&error];
                     
                     if (jsonData) {
                         NSString *jsonString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
                         FBInvitableFriendsInfo invitableFriends ([jsonString UTF8String]);
                         listener->onRequestInvitableFriends(invitableFriends);
                     } else {
                         NSLog(@"Got an error: %@", error);
                     }
                     
                 }
             }
             else
                 NSLog(@"%@", error);
          }];
        
    }
}

@implementation FacebookXImpl : NSObject

+ (id<FBSDKSharingContent>)getContent:(h102::FBShareInfo)info {
    id<FBSDKSharingContent> shareContent;
    if (info.type == h102::FB_LINK) {
        FBSDKShareLinkContent* content = [[FBSDKShareLinkContent alloc] init];
        content.contentTitle = [NSString stringWithUTF8String:info.title.c_str()];
        content.contentDescription = [NSString stringWithUTF8String:info.text.c_str()];
        content.contentURL = [NSURL URLWithString:[NSString stringWithUTF8String:info.link.c_str()]];
        if (info.media.compare("") != 0)
            content.imageURL = [NSURL URLWithString:[NSString stringWithUTF8String:info.media.c_str()]];
        
        shareContent = content;
    }
    else if (info.type == h102::FB_PHOTO) {
        FBSDKSharePhoto* photo = [[FBSDKSharePhoto alloc] init];
        photo.image = [UIImage imageWithContentsOfFile:[NSString stringWithUTF8String:info.media.c_str()]];
        photo.userGenerated = YES;
        
        FBSDKSharePhotoContent* content = [[FBSDKSharePhotoContent alloc] init];
        content.photos = @[photo];
        
        shareContent = content;
    }
    else if (info.type == h102::FB_VIDEO) {
        FBSDKShareVideo* video = [[FBSDKShareVideo alloc] init];
        video.videoURL = [NSURL URLWithString:[NSString stringWithUTF8String:info.media.c_str()]];
        
        FBSDKShareVideoContent* content = [[FBSDKShareVideoContent alloc] init];
        content.video = video;
        
        shareContent = content;
    }
    
    return shareContent;
}

@end