#pragma once

namespace Resources
{
	class Asset
	{
	protected:
		int _referenceCount = 0;
	public:
		virtual ~Asset() = default;

		void IncreaseReferenceCount() { ++_referenceCount; }
		void DecreaseReferenceCount() { if (--_referenceCount < 0) _referenceCount = 0; }
		int GetReferenceCount() const { return _referenceCount; }
	};
}
