#pragma once

#include <unordered_map>
#include "Asset.h"

namespace Resources
{
	class AssetDatabase
	{
	private:
		static std::unordered_map<std::string, Asset*> _assets;
	public:
		template<typename T> static T* GetAsset(const std::string& filePath);

		static void PutBack(Asset** asset);
		static void UnloadUnusedAssets();
		static void UnloadAllAssets();
	};

	template<typename T> T* AssetDatabase::GetAsset(const std::string& filePath)
	{
		if (_assets.find(filePath) != AssetDatabase::_assets.end())
		{
			T* asset = (T*)_assets.at(filePath);
			asset->IncreaseReferenceCount();
			return asset;
		}
		T* asset = new T(filePath);
		asset->IncreaseReferenceCount();
		_assets.insert(std::pair<std::string, Asset*>(filePath, asset));
		return asset;
	}
}
