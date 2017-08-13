#pragma once
#include"Heap.h"

template<typename T>
struct HuffmanTreeNode
{
	T _w;             //权值
	HuffmanTreeNode<T>* _left;
	HuffmanTreeNode<T>* _right;
	HuffmanTreeNode<T>* _parent;

	HuffmanTreeNode(const T& x)
		:_w(x)
		,_left(NULL)
		,_right(NULL)
		,_parent(NULL)
	{}

};

template<typename T>
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node;
public:
	HuffmanTree()
		:_root(NULL)
	{}

	HuffmanTree(T* a, size_t n, const T& invalid = T())
	{
		struct Compare
		{
			bool operator()(Node* l, Node* r) const
			{
				return l->_w < r->_w;
			}
		};

		Heap<Node*, Compare> minHeap;
		for (size_t i = 0; i < n; ++i)
		{
			if (a[i] != invalid)
			{
				minHeap.Push(new Node(a[i]));
			}
		}
		while (minHeap.Size() > 1)
		{
			Node* left = minHeap.Top();
			minHeap.Pop();
			Node* right = minHeap.Top();
			minHeap.Pop();
			Node* parent = new Node(left->_w + right->_w);
			parent->_left = left;
			parent->_right = right;
			left->_parent = parent;
			right->_parent = parent;
			minHeap.Push(parent);
		}
		_root = minHeap.Top();
	}

	Node* GetRoot()
	{
		return _root;
	}

	~HuffmanTree()
	{
		_Destory(_root);
	}

protected:
	void _Destory(Node* root)
	{
		if (root == NULL)
		{
			return;
		}
		_Destory(root->_left);
		_Destory(root->_right);
		delete root;
	}
	HuffmanTree(const HuffmanTree<T>& tree);
	HuffmanTree& operator=(const HuffmanTree<T>& tree);

protected:
	Node* _root;
};

void TestHuffmanTree()
{
	int array[10] = { 2,4,6,9,7,8,5,0,1,3 };
	HuffmanTree<int> t(array, 10);
}