#include "FacebookX.hpp"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

using namespace std;

namespace h102 {

    void FacebookX::login() {

    }

    void FacebookX::login( std::vector<std::string>& permissions ) {

    }

    std::string FacebookX::getAccessToken() {

    }

    std::string FacebookX::getUserID() {

    }

    std::string FacebookX::getName() {

    }

    bool FacebookX::isLoggedIn() {

    }

    void FacebookX::logout() {

    }

    vector<string> FacebookX::getPermissionList() {

    }

    void FacebookX::share(const FBShareInfo& info) {

    }

    bool FacebookX::canPresentWithFBApp(const h102::FBShareInfo &info) {

    }

    void FacebookX::api(const std::string& path, const std::string& tag) {

    }

    void FacebookX::api(const std::string& path, const FBAPIParam& params, const std::string& tag) {

    }

    void FacebookX::api(const std::string& path, const std::string& method, const FBAPIParam& params, const std::string& tag) {

    }

    void FacebookX::shareOpenGraphStory(const FBGraphStoryProperties& properties, const std::string& actionType, const std::string& previewPropertyName) {

    }

    void FacebookX::shareEncodedOpenGraphStory(const FBGraphStoryProperties& properties, const std::string& actionType, const std::string& previewPropertyName) {

    }

    void FacebookX::requestInvitableFriends(const FBAPIParam &params) {

    }

    void FacebookX::inviteFriendsWithInviteIds(const std::vector<std::string> &invite_ids, const std::string &title, const std::string &invite_text) {

    }
}

#endif