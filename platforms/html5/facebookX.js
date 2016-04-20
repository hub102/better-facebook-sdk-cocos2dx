var h102 = h102 || {};
h102.facebookX = h102.facebookX || {};

h102.facebookX.shareOpenGraphStory = function(info, actionType, previewPropertyName) {
	if (cc.sys.isNative) {
		info["url"] = Base64.encode(info["url"]);
		info["type"] = Base64.encode(info["type"]);
		info["title"] = Base64.encode(info["title"]);
		info["image"] = Base64.encode(info["image"]);
		info["description"] = Base64.encode(info["description"]);
		actionType = Base64.encode(actionType);
		previewPropertyName = Base64.encode(previewPropertyName);
		h102.facebookX.shareEncodedOpenGraphStory(info, actionType, previewPropertyName)
	}
}