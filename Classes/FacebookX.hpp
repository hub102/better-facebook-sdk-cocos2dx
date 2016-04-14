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
#include <map>

#include "json/json.hpp"

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
  
    enum FBShareType
    {
        FB_NONE  = 0,
        FB_LINK  = 1,
        FB_PHOTO = 2,
        FB_VIDEO = 3,
    };
  
    typedef std::map<std::string, std::string> FBAPIParam;
    
    struct FBShareInfo
    {
        FBShareInfo()
        : type(FB_NONE), link(""), title("")
        , text(""), media("")
        {}
        
        FBShareType type;
        std::string link;
        std::string title;
        std::string text;
        std::string media;
    };
    
    class FBGraphUser
    {
        
    public:
        
        // konstants for Graph User
        static const std::string kGU_USERID;
        static const std::string kGU_NAME;
        static const std::string kGU_FIRST_NAME;
        static const std::string kGU_LAST_NAME;
        static const std::string kGU_EMAIL;
        static const std::string kGU_PICTURE_URL;
        static const std::string kGU_PICTURE_IS_SILHOUETTE;
        static const std::string kGU_SCORE;
        static const std::string kGU_INSTALLED;
        
        static const std::string kGU_STR_TRUE;
        static const std::string kGU_STR_FALSE;
        
        FBGraphUser();
        explicit FBGraphUser( const nlohmann::json& json );
        explicit FBGraphUser( const std::string& json );
        
        std::string getUserId() const;
        std::string getName() const;
        std::string getFirstName() const;
        std::string getLastName() const;
        std::string getEmail() const;
        std::string getPictureURL() const;
        bool        isAppInstalled() const;
        bool        isPictureSilhouette() const;
        bool        asBoolean( const std::string& ) const;
        
        const std::string getField( const std::string& field ) const;
        void setField( const std::string& field, const std::string& value );
        
        /**
         * the bool value will be converted to 'true' (kGU_STR_TRUE) or 'false' (kGU_STR_FALSE)
         */
        void setField( const std::string& field, bool value );
        
        nlohmann::json toJSON() const;
        std::string toJSONString() const;
        
        std::map<std::string, std::string> getFields() const;
        
    private:
        
        std::map<std::string, std::string>  _fields;
        
        void __initFromJSON( const nlohmann::json& json );
        
    };
    
    struct FBInvitableUsersCursor {
        std::string next_url;
        std::string prev_url;
        std::string next_cursor;
        std::string prev_cursor;
        
        FBInvitableUsersCursor( const nlohmann::json& json );
        FBInvitableUsersCursor( );
    };
    
    class FBInvitableFriendsInfo {
        
    private:
        std::vector<FBGraphUser>    _invitableUsersInfo;
        FBInvitableUsersCursor      _cursorInfo;
        
        // the originally returned string for the FB graph call.
        std::string                 _originalJSONString;
        
    public:
        FBInvitableFriendsInfo( const std::string& json );
        
        typedef std::vector<FBGraphUser>::iterator iterator;
        typedef std::vector<FBGraphUser>::const_iterator const_iterator;
        
        size_t getNumInvitationTokens() const {
            return _invitableUsersInfo.size();
        }
        
        iterator begin() { return _invitableUsersInfo.begin(); }
        const_iterator begin() const { return _invitableUsersInfo.begin(); }
        iterator end() { return _invitableUsersInfo.end(); }
        const_iterator end() const { return _invitableUsersInfo.end(); }
        
        const std::string& getOriginalString() const {
            return _originalJSONString;
        }
        
        /**
         * Request this url to get the next invitable friends document info.
         * Maybe empty.
         */
        const std::string& getNextURL() const {
            return _cursorInfo.next_url;
        }
        
        /**
         * Request this url to get the prev invitable friends document info.
         * Maybe empty;
         */
        const std::string& getPrevURL() const {
            return _cursorInfo.prev_url;
        }
        
        /**
         * Use this string to build the NextURL.
         */
        const std::string& getNextCursor() const {
            return _cursorInfo.next_cursor;
        }
        
        /**
         * Use this string to build the PrevURL.
         */
        const std::string& getPrevCursor() const {
            return _cursorInfo.prev_cursor;
        }
    };
    
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
        virtual void onRequestInvitableFriends( const FBInvitableFriendsInfo& friends ) = 0;
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
        static std::string getAccessToken();
        static std::string getUserID();
        static bool isLoggedIn();
        static void logout();
        static std::vector<std::string> getPermissionList();
        
        static void share(const FBShareInfo& info);
        
        static void api(const std::string& path, const std::string& tag);
        static void api(const std::string& path, const FBAPIParam& params, const std::string& tag);
        static void api(const std::string& path, const std::string& method, const FBAPIParam& params, const std::string& tag);
        
        static bool canPresentWithFBApp(const FBShareInfo& info);
        
        static void requestInvitableFriends( const FBAPIParam& params);
        static void inviteFriendsWithInviteIds( const std::vector<std::string>& invite_ids, const std::string& title, const std::string& invite_text );
    };
}

#endif /* FacebookX_hpp */
