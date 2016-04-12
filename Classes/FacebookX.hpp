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

namespace h102 {
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
  public:
    static void init();
    static void setListener(FacebookListener* listener);
  };
}

#endif /* FacebookX_hpp */
