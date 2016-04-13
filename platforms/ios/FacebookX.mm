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
	//         	NSLog(@"Process error");
				if (FacebookX::listener) {
					listener->onLogin(false, error.localizedDescription.UTF8String);
				}
			} else if (result.isCancelled) {
	//         	NSLog(@"Cancelled");
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
}