#pragma once

template <class Type>
class DynamicArray{
public:
	DynamicArray(size_t size = 1) : size(size), len(0), content(new Type[size]){}

	void LoadData(Type ** dataPtr, size_t * size)
	{
		(*dataPtr) = content;
		(*size) = len;
	}

	void Add(Type item) {
		if (len >= size) {
			size *= 2;
			Type *temp = new Type[size];
			for (int i = 0; i < len; ++i) {
				temp[i] = content[i];
			}
			delete[] content;
			content = temp;
		}

		content[len] = item;
		len++;
	}

	void Remove(size_t index) {
		if (index < size) {
			size_t j = 0;
			Type *temp = new Type[size];
			for (int i = 0; i < len; ++i) {
				if (i != index) {
					temp[j] = content[i];
					++j;
				}
			}
			content = temp;
		}
	}

	void Remove(Type object) {
		Type *temp = new Type[size];
		size_t j = 0;
		for (int i = 0; i < len; ++i) {
			if (content[i] != object) {
				temp[j] = content[i];
				++j;
			}
		}
		delete[] content;
		content = temp;
	}

	void SetSize(size_t _size) { size = _size; }

	size_t GetSize() { return size; }

	void SetLength(size_t _len) { len = _len; }

	size_t GetLength() { return len; }

	int Find(Type object) {
		for (int i = 0; i < len; ++i) {
			if (content[i] == object) {
				return i;
			}
		}
		return -1;
	}

	Type Get(size_t index) {
		return content[index];
	}

	~DynamicArray() {
		delete[] content;
	}

private:
	Type *content;
	size_t size;
	size_t len;
};

