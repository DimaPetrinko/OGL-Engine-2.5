#pragma once

namespace Resources
{
	class Asset
	{
	protected:
		int mReferenceCount = 0;
	public:
		virtual ~Asset() = default;

		void IncreaseReferenceCount() { ++mReferenceCount; }
		void DecreaseReferenceCount() { if (--mReferenceCount < 0) mReferenceCount = 0; }
		int GetReferenceCount() const { return mReferenceCount; }
	};
}
