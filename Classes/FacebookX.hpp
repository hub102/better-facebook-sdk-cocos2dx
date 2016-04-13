//
//  FacebookX.hpp
//  FacebookXTest
//
//  Created by Thuy Dong Xuan on 4/12/16.
//
//

#ifndef FacebookX_hpp
#define FacebookX_hpp

#include <string>
#include <vector>

namespace h102 {
  
  const std::string FB_PERM_READ_PUBLIC_PROFILE("public_profile");
  const std::string FB_PERM_READ_EMAIL("email");
  const std::string FB_PERM_READ_USER_FRIENDS("user_friends");
  const std::string FB_PERM_PUBLISH_POST("publish_actions");
  const std::string FB_API_ME_FRIENDS_TAG("__fb_me_friends__");
  const std::string FB_API_ME_FRIENDS_PATH("me/friends");
  
  const std::string kRI_ExcludeFromList("ExcludeFromListIds");
  const std::string kRI_PictureSize("PictureSize");
  const std::string kRI_PaginationLimit("PaginationLimit");
  const std::string kRI_ResponseFields("ResponseFields");
  
  
  class FacebookListener
  {
  public:
    virtual void onLogin(bool isLogin, const std::string& msg) = 0;
    virtual void onSharedSuccess(const std::string& message) = 0;
    virtual void onSharedFailed(const std::string& message) = 0;
    virtual void onSharedCancel() = 0;
    virtual void onAPI(const std::string& key, const std::string& jsonData) = 0;
    virtual void onPermission(bool isLogin, const std::string& msg) = 0;
    virtual void onFetchFriends(bool ok, const std::string& msg) = 0;
//    virtual void onRequestInvitableFriends( const FBInvitableFriendsInfo& friends ) = 0;
    virtual void onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg )= 0;
    virtual void onInviteFriendsResult( bool result, const std::string& msg )= 0;
    
//    virtual void onGetUserInfo( const FBGraphUser& userInfo ) = 0;
  };
  
  class FacebookX {
    
    static FacebookListener* listener;
    
  public:
    static void init();
    static void setListener(FacebookListener* listener);
    
    static void login();
    static void login( std::vector<std::string>& permissions );
    
    
    static std::vector<std::string> getPermissionList();
  };
}

#endif /* FacebookX_hpp */
