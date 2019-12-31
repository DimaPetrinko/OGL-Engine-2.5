#include <vector>
#include "AssetDatabase.h"

namespace Resources
{
	std::unordered_map<std::string, Asset*> AssetDatabase::_assets;

	void AssetDatabase::PutBack(Asset** asset)
	{
		(*asset)->DecreaseReferenceCount();
		*asset = nullptr;
	}


	void AssetDatabase::UnloadUnusedAssets()
	{
		std::vector<std::string> keysToErase;
		for (auto pair : _assets)
		{
			if (pair.second->GetReferenceCount() > 0) continue;
			delete(pair.second);
			keysToErase.push_back(pair.first);
		}

		for (auto key : keysToErase) _assets.erase(key);
	}

	void AssetDatabase::UnloadAllAssets()
	{
		for (auto pair : _assets)
		{
			delete(pair.second);
		}
		_assets.clear();
	}
}
