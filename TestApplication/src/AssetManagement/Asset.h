#pragma once

namespace Resources
{
	class Asset
	{
	protected:
		int _referenceCount = 0;
		char* _data;
	public:
		// Asset(const std::string& fileName);
		~Asset() { delete _data; }

		inline void IncreaseReferenceCount() { ++_referenceCount; }
		inline void DecreaseReferenceCount() { if (--_referenceCount < 0) _referenceCount = 0; }
		inline int GetReferenceCount() { return _referenceCount; }
		inline void* GetData() const { return _data; }
	};
}