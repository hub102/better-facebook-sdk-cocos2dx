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
#import "FacebookGameRequestDelegate.h"

#import "FacebookX.hpp"
#import "FacebookX-impl.h"

using namespace std;

#import <iostream>

namespace h102 {
    FBSDKLoginManager* loginManager = NULL;
    
    NSString* toJSONString(id data)
    {
        NSError *error;
        NSData *jsonData = [NSJSONSerialization dataWithJSONObject:data
                                                           options:0 // Pass 0 if you don't care about the readability of the generated string
                                                             error:&error];
        
        if (! jsonData) {
            NSLog(@"Got an error: %@", error);
            return @"{}";
        } else {
            return [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
        }
    }
    
    NSString* toDecodedString(NSString* encodedString)
    {
        NSData* decodedData = [[NSData alloc] initWithBase64EncodedString:encodedString options:0];
        return [[NSString alloc] initWithData:decodedData encoding:NSUTF8StringEncoding];
    }
    
    void FacebookX::login() {
        vector<string> permissions;
        permissions.push_back(h102::FB_PERM_READ_PUBLIC_PROFILE);
        permissions.push_back(h102::FB_PERM_READ_EMAIL);
        permissions.push_back(h102::FB_PERM_READ_USER_FRIENDS);
        login(permissions);
    }
    
    void FacebookX::login( std::vector<std::string>& permissions ) {
        if (!loginManager) {
            loginManager = [[FBSDKLoginManager alloc] init];
        }
        NSMutableArray *permissionArray = [NSMutableArray new];
        for (auto str : permissions) {
            id nsstr = [NSString stringWithUTF8String:str.c_str()];
            [permissionArray addObject:nsstr];
        }
        
        [loginManager logInWithReadPermissions: permissionArray
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
                                               if (FacebookX::listener) {
//                                                   [[[FBSDKGraphRequest alloc] initWithGraphPath:@"/me/" parameters:nil]
//                                                    startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
//                                                        if (!error) {
//                                                            if (FacebookX::listener) {
//                                                                NSString* myName = [result objectForKey:@"name"];
//                                                                NSString* myID = [result objectForKey:@"id"];
//                                                                FBGraphUser myInfo = FBGraphUser();
//                                                                myInfo.setField(FBGraphUser::kGU_NAME, std::string([myName UTF8String]));
//                                                                myInfo.setField(FBGraphUser::kGU_USERID, std::string([myID UTF8String]));
//                                                                listener->onGetUserInfo(myInfo);
//                                                            }
//                                                        }
//                                                    }];
                                                   listener->onLogin(true, "LoggedIn");
                                               }
                                           }
                                       }];
    }
    
    std::string FacebookX::getAccessToken() {
        return [[[FBSDKAccessToken currentAccessToken] tokenString] UTF8String];
    }
    
    std::string FacebookX::getUserID() {
        if (!FacebookX::isLoggedIn())
            return "";
        return [[[FBSDKAccessToken currentAccessToken] userID] UTF8String];
    }
    
    std::string FacebookX::getName() {
        return "";
    }
    
    bool FacebookX::isLoggedIn() {
        return [[FBSDKAccessToken currentAccessToken] tokenString] != nil;
    }
    
    void FacebookX::logout() {
        if (!loginManager) {
            loginManager = [[FBSDKLoginManager alloc] init];
        }
        [loginManager logOut];
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
            if (FacebookX::listener) {
                listener->onSharedSuccess([ret UTF8String]);
            }
        } failedHandler:^(id<FBSDKSharing> sharer, NSError *error) {
            if (FacebookX::listener) {
                listener->onSharedFailed([error.localizedDescription UTF8String]);
            }
        } cancelHandler:^(id<FBSDKSharing> sharer) {
            if (FacebookX::listener) {
                listener->onSharedCancel();
            }
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
        FacebookX::api(path, "", FBAPIParam(), tag);
    }
    
    void FacebookX::api(const std::string& path, const FBAPIParam& params, const std::string& tag) {
        FacebookX::api(path, "", params, tag);
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
            
            if (![_method  isEqualToString:@""] && _params.count > 0) {
                [[[FBSDKGraphRequest alloc] initWithGraphPath:_path parameters:_params HTTPMethod:_method]
                 startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
                     if (!error) {
                         NSLog(@"API executed OK!!");
                         if (FacebookX::listener) {
                             NSString* stringifiedResult = toJSONString(result);
                             std::string cStringResult = [stringifiedResult UTF8String];
                             if (FacebookX::listener) {
                                 listener->onAPI([_tag UTF8String], cStringResult);
                             }
                         }
                     } else
                         NSLog(@"%@", error);
                 }];
            } else if (![_method isEqualToString:@""]) {
                [[[FBSDKGraphRequest alloc] initWithGraphPath:_path parameters:_params]
                 startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
                     if (!error) {
                         NSLog(@"API executed OK!!");
                         if (FacebookX::listener) {
                             NSString* stringifiedResult = toJSONString(result);
                             std::string cStringResult = [stringifiedResult UTF8String];
                             if (FacebookX::listener) {
                                 listener->onAPI([_tag UTF8String], cStringResult);
                             }
                         }
                     } else
                         NSLog(@"%@", error);
                 }];
            } else {
                [[[FBSDKGraphRequest alloc] initWithGraphPath:_path parameters:nil]
                 startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
                     if (!error) {
                         NSLog(@"API executed OK!!, tag = %@", _tag);
                         if (FacebookX::listener) {
                             NSString* stringifiedResult = toJSONString(result);
                             std::string cStringResult = [stringifiedResult UTF8String];
                             if (FacebookX::listener) {
                                 listener->onAPI([_tag UTF8String], cStringResult);
                             }
                         }
                     } else
                         NSLog(@"%@", error);
                 }];
            }
        }
    }
    
    void FacebookX::shareOpenGraphStory(const FBGraphStoryProperties& properties, const std::string& actionType, const std::string& previewPropertyName) {
        NSString* objectType = [NSString stringWithUTF8String:properties.type.c_str()];
        NSString* title = [NSString stringWithUTF8String:properties.title.c_str()];
        NSString* description = [NSString stringWithUTF8String:properties.description.c_str()];
        NSString* image = [NSString stringWithUTF8String:properties.image.c_str()];
        NSString* url = [NSString stringWithUTF8String:properties.url.c_str()];
        
        NSURL *imageURL = [NSURL URLWithString:image];
        FBSDKSharePhoto *photo = [FBSDKSharePhoto photoWithImageURL:imageURL userGenerated:NO];
        
        NSDictionary* _properties = @{
                                      @"og:type": objectType,
                                      @"og:title": title,
                                      @"og:description": description,
                                      @"og:image": photo,
                                      @"og:url": url
                                      };
        
        FBSDKShareOpenGraphObject* object = [FBSDKShareOpenGraphObject objectWithProperties:_properties];
        
        FBSDKShareOpenGraphAction* action = [[FBSDKShareOpenGraphAction alloc] init];
        action.actionType = [NSString stringWithUTF8String:actionType.c_str()];
        
        NSString* _previewPropertyName = [NSString stringWithUTF8String:previewPropertyName.c_str()];
        
        [action setObject:object forKey:_previewPropertyName];
        
        FBSDKShareOpenGraphContent* content = [[FBSDKShareOpenGraphContent alloc] init];
        content.action = action;
        content.previewPropertyName = _previewPropertyName;
        
        FacebookShareDelegate* delegate = [[FacebookShareDelegate alloc] initWithSucceedHandler:^(id<FBSDKSharing> sharer, NSDictionary *result) {
            NSString* ret = [NSString stringWithFormat:@"%@", result];
            //            if ([result objectForKey:@"postId"])
            //                ret = [ret stringByAppendingString:[NSString stringWithFormat:@"{\"postId\":\"%@\"}", [result objectForKey:@"postId"]]];
            if (FacebookX::listener) {
                listener->onSharedSuccess([ret UTF8String]);
            }
        } failedHandler:^(id<FBSDKSharing> sharer, NSError *error) {
            if (FacebookX::listener) {
                listener->onSharedFailed([error.localizedDescription UTF8String]);
            }
        } cancelHandler:^(id<FBSDKSharing> sharer) {
            if (FacebookX::listener) {
                listener->onSharedCancel();
            }
        }];
        
        [FBSDKShareDialog showFromViewController:[UIViewController topViewController] withContent:content delegate:delegate];
    }
    
    void FacebookX::shareEncodedOpenGraphStory(const FBGraphStoryProperties& properties, const std::string& actionType, const std::string& previewPropertyName) {
        NSString* objectType = [NSString stringWithUTF8String:properties.type.c_str()];
        NSString* title = [NSString stringWithUTF8String:properties.title.c_str()];
        NSString* description = [NSString stringWithUTF8String:properties.description.c_str()];
        NSString* image = [NSString stringWithUTF8String:properties.image.c_str()];
        NSString* url = [NSString stringWithUTF8String:properties.url.c_str()];
        NSString* _actionType = [NSString stringWithUTF8String:actionType.c_str()];
        _actionType = toDecodedString(_actionType);
        NSString* _previewPropertyName = [NSString stringWithUTF8String:previewPropertyName.c_str()];
        _previewPropertyName = toDecodedString(_previewPropertyName);
        
        objectType = toDecodedString(objectType);
        title = toDecodedString(title);
        description = toDecodedString(description);
        image = toDecodedString(image);
        url = toDecodedString(url);
        
        NSURL *imageURL = [NSURL URLWithString:image];
        FBSDKSharePhoto *photo = [FBSDKSharePhoto photoWithImageURL:imageURL userGenerated:NO];
        
        NSDictionary* _properties = @{
                                      @"og:type": objectType,
                                      @"og:title": title,
                                      @"og:description": description,
                                      @"og:image": photo,
                                      @"og:url": url
                                      };
        
        FBSDKShareOpenGraphObject* object = [FBSDKShareOpenGraphObject objectWithProperties:_properties];
        
        FBSDKShareOpenGraphAction* action = [[FBSDKShareOpenGraphAction alloc] init];
        action.actionType = _actionType;
        
        [action setObject:object forKey:_previewPropertyName];
        
        FBSDKShareOpenGraphContent* content = [[FBSDKShareOpenGraphContent alloc] init];
        content.action = action;
        content.previewPropertyName = _previewPropertyName;
        
        FacebookShareDelegate* delegate = [[FacebookShareDelegate alloc] initWithSucceedHandler:^(id<FBSDKSharing> sharer, NSDictionary *result) {
            NSString* ret = [NSString stringWithFormat:@"%@", result];
            //            if ([result objectForKey:@"postId"])
            //                ret = [ret stringByAppendingString:[NSString stringWithFormat:@"{\"postId\":\"%@\"}", [result objectForKey:@"postId"]]];
            if (FacebookX::listener) {
                listener->onSharedSuccess([ret UTF8String]);
            }
        } failedHandler:^(id<FBSDKSharing> sharer, NSError *error) {
            if (FacebookX::listener) {
                listener->onSharedFailed([error.localizedDescription UTF8String]);
            }
        } cancelHandler:^(id<FBSDKSharing> sharer) {
            if (FacebookX::listener) {
                listener->onSharedCancel();
            }
        }];
        
        [FBSDKShareDialog showFromViewController:[UIViewController topViewController] withContent:content delegate:delegate];
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
                         if (FacebookX::listener) {
                             listener->onRequestInvitableFriends(invitableFriends);
                         }
                     } else {
                         NSLog(@"Got an error: %@", error);
                     }
                     
                 }
             }
             else
                 NSLog(@"%@", error);
         }];
    }
    
    void FacebookX::inviteFriendsWithInviteIds(const std::vector<std::string> &invite_ids, const std::string &title, const std::string &invite_text) {
        FBSDKAccessToken* token = [FBSDKAccessToken currentAccessToken];
        if ([token hasGranted:[NSString stringWithFormat:@"%s", FB_PERM_READ_USER_FRIENDS.c_str()]]) {
            NSMutableArray* recipients = [NSMutableArray new];
            for(auto str : invite_ids)
                [recipients addObject:[NSString stringWithUTF8String:str.c_str()]];
            
            FBSDKGameRequestContent* content = [[FBSDKGameRequestContent alloc] init];
            content.title = [NSString stringWithUTF8String:title.c_str()];
            content.message = [NSString stringWithUTF8String:invite_text.c_str()];
            content.recipients = recipients;
            
            FacebookGameRequestDelegate* delegate = [[FacebookGameRequestDelegate alloc] initWithSucceedHandler:^(FBSDKGameRequestDialog* sharer, NSDictionary *result) {
                
                NSMutableDictionary* dict = [NSMutableDictionary new];
                [dict setObject:[result objectForKey:@"request"] forKey:@"request"];
                [dict setObject:[result objectForKey:@"to"] forKey:@"to"];
                
                NSError *error;
                NSData *jsonData = [NSJSONSerialization dataWithJSONObject:dict
                                                                   options:0 // Pass 0 if you don't care about the readability of the generated string
                                                                     error:&error];
                
                if (jsonData) {
                    NSString *jsonString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
                    if (FacebookX::listener) {
                        listener->onInviteFriendsWithInviteIdsResult(true, [jsonString UTF8String]);
                    }
                } else {
                    NSLog(@"Got an error: %@", error);
                }
            } failedHandler:^(FBSDKGameRequestDialog* sharer, NSError *error) {
                if (FacebookX::listener) {
                    listener->onInviteFriendsWithInviteIdsResult(false, [error.localizedDescription UTF8String]);
                }
            } cancelHandler:^(FBSDKGameRequestDialog* sharer) {
                if (FacebookX::listener) {
                    listener->onInviteFriendsWithInviteIdsResult(false, "Cancelled");
                }
            }];
            
            [FBSDKGameRequestDialog showWithContent:content delegate:delegate];
            
        } else {
            NSLog(@"Error: inviteFriendsWithInviteIds - Cannot grant permission: %s", FB_PERM_READ_USER_FRIENDS.c_str());
        }
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

+ (void)activateApp {
  [FBSDKAppEvents activateApp];
}

@end