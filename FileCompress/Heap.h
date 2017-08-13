#pragma once
#include<vector>

template<typename T>
struct Less
{
	bool operator()(const T& l, const T& r) const
	{
		return l < r;
	}
};

template<typename T>
struct Greater
{
	bool operator()(const T& l, const T& r) const
	{
		return l > r;
	}
};

template<typename T, typename Compare>
class Heap
{
public:
	Heap()
	{}

	Heap(T* array, size_t n)
	{
		_array.reserve(n);
		for (size_t i = 0; i < n; i++)
		{
			_array.push_back(array[i]);
		}
		for (int i = (_array.size() - 2) >> 1; i >= 0; --i)
		{
			_AdjustDown(i);
		}
	}

	const T& Top()const
	{
		return _array[0];
	}

	void Push(const T& x)
	{
		_array.push_back(x);
		_AdjustUp(_array.size() - 1);
	}

	size_t Size()
	{
		return _array.size();
	}

	void Pop()
	{
		assert(_array.size() > 0);
		swap(_array[0], _array[_array.size() - 1]);
		_array.pop_back();
		_AdjustDown(0);
	}

	bool Empty()
	{
		return _array.size() == 0;
	}

	void Print()
	{
		for (size_t i = 0; i < _array.size(); ++i)
		{
			cout << _array[i] << " ";
		}
		cout << endl;
	}

protected:
	void _AdjustUp(int child)
	{
		Compare ComFunc;
		int parent = (child - 1) >> 1;
		while (child)
		{
			//if (_array[child] > _array[parent])
			if(ComFunc(_array[child], _array[parent]))
			{
				swap(_array[child], _array[parent]);
				child = parent;
				parent = (child - 1) >> 1;
			}
			else
			{
				break;
			}
		}
	}

	void _AdjustDown(int root)
	{
		Compare ComFunc;
		int parent = root;
		int child = root * 2 + 1;
		while (child < _array.size())
		{
			//if (child + 1 < _array.size() && _array[child + 1] > _array[child])
			if (child + 1 < _array.size() && ComFunc(_array[child + 1], _array[child]))

			{
				++child;
			}
			//if (_array[child] > _array[parent])
			if (ComFunc(_array[child], _array[parent]))
			{
				swap(_array[child], _array[parent]);
				parent = child;
				child = parent * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}

protected:
	vector<T> _array;
};

template<typename T, typename Compare>
class PriorityQueue
{
public:
	void Push(const T& x)
	{
		_hp.Push(x);
	}

	void Pop()
	{
		_hp.Pop();
	}

	const T& Top()
	{
		return _hp.Top();
	}

	bool IsEmpty()
	{
		return _hp.Empty();
	}

	size_t Size()
	{
		return _hp.Size();
	}

protected:
	Heap<T, Compare> _hp;
};


void TestHeap()
{
	int a[] = { 10,11,13,12,16,18,15,17,14,19 };
	//Heap<int> heap(a, sizeof(a) / sizeof(a[0]));
	//Heap<int, Less<int>> heap(a, sizeof(a) / sizeof(a[0]));
	Heap<int, Greater<int>> heap(a, sizeof(a) / sizeof(a[0]));
	heap.Print();
	heap.Push(25);
	heap.Print();
	heap.Pop();
	heap.Print();
}