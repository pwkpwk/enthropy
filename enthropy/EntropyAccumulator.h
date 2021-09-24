#pragma once

#include "EntropyBitCount.h"

class EntropyAccumulator
{
private:
	typedef std::list<EntropyBitCount> collection;
	const collection::size_type mMaxBlocks;
	collection mBlocks;
	EntropyBitCount mEntropy;

public:
	EntropyAccumulator(const collection::size_type maxBlocks) : mMaxBlocks(maxBlocks)
	{
	}

	void add(const EntropyBitCount &block)
	{
		mEntropy.add(block);
		mBlocks.push_back(block);
		if (mBlocks.size() > mMaxBlocks)
		{
			mEntropy.subtract(mBlocks.front());
			mBlocks.pop_front();
		}
	}

	const EntropyBitCount &bits() { return mEntropy; }
};
