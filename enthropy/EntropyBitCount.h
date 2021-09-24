#pragma once

class EntropyBitCount
{
private:
	int mOnes;
	int mZeros;

public:
	EntropyBitCount() : mOnes(0), mZeros(0)
	{
	}

	EntropyBitCount(const int ones, const int zeros) : mOnes(ones), mZeros(zeros)
	{
	}

	EntropyBitCount(const EntropyBitCount &init) : mOnes(init.mOnes), mZeros(init.mZeros)
	{
	}

	const EntropyBitCount &operator = (const EntropyBitCount &value)
	{
		mOnes = value.mOnes;
		mZeros = value.mZeros;
		return *this;
	}

	void addBlock(const int ones, const int zeros)
	{
		mOnes += ones;
		mZeros += zeros;
	}

	void add(const EntropyBitCount &block)
	{
		addBlock(block.mOnes, block.mZeros);
	}

	void subtract(const EntropyBitCount& block)
	{
		mOnes -= block.mOnes;
		mZeros -= block.mZeros;
	}

	int zeros() const { return mZeros; }
	int ones() const { return mOnes; }
};
