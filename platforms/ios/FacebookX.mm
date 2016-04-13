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

#import "UIViewController+Utils.h"

#import "FacebookX.hpp"

using namespace std;

namespace h102 {
  void FacebookX::login() {
    vector<string> permissions;
    permissions.push_back(h102::FB_PERM_READ_PUBLIC_PROFILE);
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
//         NSLog(@"Process error");
         if (FacebookX::listener)
           listener->onLogin(false, error.localizedDescription.UTF8String);
       } else if (result.isCancelled) {
//         NSLog(@"Cancelled");
         if (FacebookX::listener)
           listener->onLogin(false, "Cancelled");
       } else {
         listener->onLogin(true, "LoggedIn");
       }
     }];
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
}