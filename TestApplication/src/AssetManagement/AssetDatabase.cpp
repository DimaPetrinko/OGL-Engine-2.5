#include <vector>
#include <fstream>
#include <limits>
#include "AssetDatabase.h"

namespace Resources
{
	std::unordered_map<std::string, Asset*> AssetDatabase::_assets;

	template<class T > T* AssetDatabase::GetAsset(const std::string& filePath)
	{
		if (AssetDatabase::_assets.find(filePath) != AssetDatabase::_assets.end())
		{
			Asset* asset = AssetDatabase::_assets.at(filePath);
			asset->IncreaseReferenceCount();
			return asset;
		}
		void* data = LoadData(filePath);
		Asset* asset = new T(data);
		asset->IncreaseReferenceCount();
		AssetDatabase::_assets.insert(std::pair<std::string, Asset*>(filePath, asset));
		return asset;
	}


	void AssetDatabase::PutBack(Asset*& asset)
	{
		asset->DecreaseReferenceCount();
		asset = nullptr;
	}


	void AssetDatabase::UnloadUnusedAssets()
	{
		std::cout << _assets.size() << std::endl;
		std::vector<std::string> keysToErase;
		for(auto pair : _assets)
		{
			if (pair.second->GetReferenceCount() > 0) continue;
			delete(pair.second);
			keysToErase.push_back(pair.first);
		}

		for(auto key : keysToErase) _assets.erase(key);
		std::cout << _assets.size() << std::endl;
	}

	void AssetDatabase::UnloadAllAssets()
	{
		for(auto pair : _assets)
		{
			delete(pair.second);
		}
		_assets.clear();
	}

	/*template<typename T>*/ void* AssetDatabase::LoadData(const std::string& filePath)
	{
		std::ifstream stream(filePath);
		if (stream.fail()) printf("%s doesn't exist\n", filePath);

		stream.seekg(0, std::ios::end);
		auto size = stream.tellg();
		char* data = new char[size];
		stream.seekg(0, std::ios::beg);
		stream.read(data, size);
		stream.close();

		return data;
	}
}