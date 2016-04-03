#pragma once
#include <map>

template <typename IndexType, typename ValueType>
class BidirectionalIndexed
{
public:
	using IndexType_t = IndexType;
	using ValueType_t = ValueType;
private:
	IndexType counter;
	std::map<IndexType, ValueType> indexToValue;
	std::map<ValueType, IndexType> valueToIndex;

public:
	BidirectionalIndexed(IndexType initial_index = 0);

	IndexType	declare_new(const ValueType v);
	bool		has_index(const ValueType v) const;
	IndexType	get_index(const ValueType v);

	bool		has_value(IndexType index) const;
	ValueType	get_value(IndexType index) const;

	IndexType	reserve_id();
};

template <typename IndexType, typename ValueType>
BidirectionalIndexed<IndexType, ValueType>::BidirectionalIndexed(IndexType initial_index) :
	counter(initial_index)
{

}

template <typename IndexType, typename ValueType>
bool BidirectionalIndexed<IndexType, ValueType>::has_index(const ValueType v) const
{
	return valueToIndex.find(v) != valueToIndex.end();
}

template <typename IndexType, typename ValueType>
IndexType BidirectionalIndexed<IndexType, ValueType>::get_index(const ValueType v)
{
	if (has_index(v))
	{
		return valueToIndex[v];
	}

	return declare_newval(v);
}

template <typename IndexType, typename ValueType>
bool BidirectionalIndexed<IndexType, ValueType>::has_value(IndexType index) const
{
	return index >= counter && (indexToValue.find(index) != indexToValue.end());
}

template <typename IndexType, typename ValueType>
ValueType BidirectionalIndexed<IndexType, ValueType>::get_value(IndexType index) const
{
	return indexToValue[index];
}

template <typename IndexType, typename ValueType>
IndexType BidirectionalIndexed<IndexType, ValueType>::reserve_id()
{
	++counter;
	return counter;
}

template <typename IndexType, typename ValueType>
IndexType BidirectionalIndexed<IndexType, ValueType>::declare_new(const ValueType v)
{
	counter++;
	indexToValue[counter] = v;
	valueToIndex[v] = counter;
	return counter;
}