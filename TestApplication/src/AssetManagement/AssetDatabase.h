#pragma once

#include <unordered_map>
#include "Asset.h"

#define ToAssetReference(x) (Resources::Asset**)(void**)&x

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
		auto iterator = _assets.find(filePath);
		Asset* asset;
		if (iterator != AssetDatabase::_assets.end())
		{
			asset = iterator->second;
		}
		else
		{
			asset = new T(filePath);
			_assets.insert(std::pair<std::string, Asset*>(filePath, asset));
		}
		asset->IncreaseReferenceCount();
		return (T*)asset;
	}
}
