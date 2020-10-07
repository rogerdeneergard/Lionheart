#pragma once
#include "DynamicArray.h"
template <class TypeIndex, class TypeValue>
class Dictionary
{
public:
	Dictionary(size_t size = 1) : size(size), len(0), indexes(new DynamicArray<TypeIndex>(size)),
		values(new DynamicArray<TypeValue>(size)) {}

	void Add(TypeIndex index, TypeValue value) {
		indexes->Add(index);
		values->Add(value);
		size = indexes->GetSize();
		len = indexes->GetLength();
	}

	void Remove(TypeIndex index, TypeValue value) {
		indexes.Remove(index);
		values.Remove(value);
		SetSize(indexes->GetSize());
		SetLength(indexes->GetLength());
	}

	void SetSize(size_t _size) { size = _size; }

	size_t GetSize() { return size; }

	void SetLength(size_t _len) { len = _len; }

	size_t GetLength() { return len; }

	TypeValue Translate(TypeIndex index) {
		for (int i = 0; i < len; ++i) {
			if (indexes->Get(i) == index) {
				return values->Get(i);
			}
		}
	}

	~Dictionary() {
		~indexes();
		~values();
	}

private:
	size_t size;
	size_t len;
	DynamicArray<TypeIndex> * indexes;
	DynamicArray<TypeValue> * values;
};

