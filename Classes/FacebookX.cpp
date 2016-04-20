//
//  FacebookX.cpp
//  FacebookXTest
//
//  Created by Thuy Dong Xuan on 4/12/16.
//
//

#include "FacebookX.hpp"

using namespace std;

using Json = nlohmann::json;

namespace h102 {
  
    FacebookListener* FacebookX::listener = NULL;

    void FacebookX::init() {

    }

    void FacebookX::setListener(h102::FacebookListener *listener) {
        FacebookX::listener = listener;
    }
    
#pragma make FBGraphUser
    const string FBGraphUser::kGU_USERID = "id";
    const string FBGraphUser::kGU_NAME = "name";
    const string FBGraphUser::kGU_FIRST_NAME = "first_name";
    const string FBGraphUser::kGU_LAST_NAME = "last_name";
    const string FBGraphUser::kGU_EMAIL = "email";
    const string FBGraphUser::kGU_PICTURE_URL = "picture_url";
    const string FBGraphUser::kGU_PICTURE_IS_SILHOUETTE = "is_silhouette";
    const string FBGraphUser::kGU_SCORE = "score";
    const string FBGraphUser::kGU_INSTALLED = "installed";
    
    const string FBGraphUser::kGU_STR_TRUE = "true";
    const string FBGraphUser::kGU_STR_FALSE = "false";
    
    FBGraphUser::FBGraphUser() {
        
    }
    
    FBGraphUser::FBGraphUser(const Json& json) {
        __initFromJSON(json);
    }
    
    FBGraphUser::FBGraphUser(const std::string& json) {
        auto j = Json::parse(json);
        if (j)
            __initFromJSON(j);
    }
    
     string FBGraphUser::getUserId() const {
         return _fields.at(kGU_USERID);
     }
    
    std::string FBGraphUser::getName() const {
        return _fields.at(kGU_NAME);
    }
    
    std::string FBGraphUser::getFirstName() const {
        return _fields.at(kGU_FIRST_NAME);
    }
    
    std::string FBGraphUser::getLastName() const {
        return _fields.at(kGU_LAST_NAME);
    }
    
    std::string FBGraphUser::getEmail() const {
        return _fields.at(kGU_EMAIL);
    }
    
    std::string FBGraphUser::getPictureURL() const {
        return _fields.at(kGU_PICTURE_URL);
    }
    
    bool FBGraphUser::isAppInstalled() const {
        // TODO
        return false;
    }
    
    bool FBGraphUser::isPictureSilhouette() const {
        return _fields.at(kGU_PICTURE_IS_SILHOUETTE) == kGU_STR_TRUE;
    }
    
    bool FBGraphUser::asBoolean( const std::string& s) const {
        return s == kGU_STR_TRUE;
    }
    
    const std::string FBGraphUser::getField( const std::string& field ) const {
        return _fields.at(field);
    }
    
    void FBGraphUser::setField( const std::string& field, const std::string& value ) {
        _fields[field] = value;
    }
    
    void FBGraphUser::setField( const std::string& field, bool value ) {
        _fields[field] = value ? kGU_STR_TRUE : kGU_STR_FALSE;
    }
    
    nlohmann::json FBGraphUser::toJSON() const {
        return Json(_fields);
    }
    
    std::string FBGraphUser::toJSONString() const {
        return toJSON().dump();
    }
    
    std::map<std::string, std::string> FBGraphUser::getFields() const {
        return _fields;
    }
    
    void FBGraphUser::__initFromJSON( const nlohmann::json& json ) {
        if (json.find("id") != json.end() && json["id"].is_string())
            setField(kGU_USERID, json["id"].get<string>());
        
        if (json.find("name") != json.end() && json["name"].is_string())
            setField(kGU_NAME, json["name"].get<string>());
        
        if (json.find("picture") != json.end() && json["picture"].is_object()) {
            setField(kGU_PICTURE_URL, json["picture"]["data"]["url"].get<string>());
            setField(kGU_PICTURE_IS_SILHOUETTE, json["picture"]["data"]["is_silhouette"].get<bool>());
        }
        
        if (json.find("last_name") != json.end() && json["last_name"].is_string())
            setField(kGU_LAST_NAME, json["last_name"].get<string>());
        
        if (json.find("first_name") != json.end() && json["first_name"].is_string())
            setField(kGU_LAST_NAME, json["first_name"].get<string>());
        
        if (json.find("email") != json.end() && json["email"].is_string())
            setField(kGU_LAST_NAME, json["email"].get<string>());
    }
    
#pragma make FBInvitableUsersCursor
    FBInvitableUsersCursor::FBInvitableUsersCursor(const nlohmann::json& json) {
        if (json.find("cursors") != json.end() && json["cursors"].is_object()) {
            Json cursors = json["cursors"];
            if (json.find("after") != json.end() && cursors["after"].is_string())
                next_cursor = cursors["after"].get<string>();
            
            if (json.find("before") != json.end() && cursors["before"].is_string())
                prev_cursor = cursors["before"].get<string>();
        }
        
        if (json.find("next") != json.end() && json["next"].is_string())
            next_url = json["next"].get<string>();
        
        if (json.find("previous") != json.end() && json["previous"].is_string())
            next_url = json["previous"].get<string>();
    }
    
    FBInvitableUsersCursor::FBInvitableUsersCursor() {
        
    }
    
#pragma make FBInvitableFriendsInfo
    FBInvitableFriendsInfo::FBInvitableFriendsInfo(const std::string& json) {
        
        _originalJSONString = string(json);
        
        auto invitableFriendsObj = Json::parse(json);
        auto friendData = invitableFriendsObj["data"];
        
        for (Json::iterator it = friendData.begin(); it != friendData.end(); ++it) {
            FBGraphUser graphUser(*it);
            _invitableUsersInfo.push_back(graphUser);
        }
        
        FBInvitableUsersCursor cursor(invitableFriendsObj["paging"]);
        _cursorInfo = cursor;
    }
}