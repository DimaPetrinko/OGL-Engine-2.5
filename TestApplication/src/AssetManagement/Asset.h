#pragma once

namespace Resources
{
	class Asset
	{
	protected:
		int _referenceCount = 0;
	public:
		//Asset(const std::string& fileName) :_data(nullptr) {}
		virtual ~Asset() = default;

		void IncreaseReferenceCount() { ++_referenceCount; }
		void DecreaseReferenceCount() { if (--_referenceCount < 0) _referenceCount = 0; }
		int GetReferenceCount() const { return _referenceCount; }
		virtual void* GetData() const = 0;
	};
}
