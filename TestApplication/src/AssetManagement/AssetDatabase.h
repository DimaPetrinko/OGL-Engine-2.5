#pragma once

#include <iostream>
#include <unordered_map>
#include "Asset.h"

namespace Resources
{
	class AssetDatabase
	{
	private:
		static std::unordered_map<std::string, Asset*> _assets;
	public:
		template<class T> static T* GetAsset(const std::string& filePath);
		static void PutBack(Asset*& asset);
		static void UnloadUnusedAssets();
		static void UnloadAllAssets();
	private:
		/*template<typename T>*/ static void* LoadData(const std::string& filePath);
	};
}