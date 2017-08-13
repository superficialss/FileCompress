#pragma once
#include"HuffmanTree.h"

typedef long long LongType;
struct CharInfo
{
	unsigned char _ch;          //�ַ�
	LongType _count;          //�ַ����ֵĴ���
	string _code;           //huffman����

	CharInfo(LongType count = 0)
		:_count(count)
		, _ch(0)
		, _code("")
	{}

	bool operator<(const CharInfo& info)const
	{
		return _count < info._count;
	}

	CharInfo operator+(const CharInfo& info)
	{
		return CharInfo(_count + info._count);
	}

	bool operator!=(const CharInfo& info)const
	{
		return _count != info._count;
	}
};

struct CountInfo
{
	unsigned char _ch;        //�ַ�
	LongType _count;          //�ַ����ֵĴ���
};

class FileCompress
{
public:
	FileCompress()
	{
		for (size_t i = 0; i < 256; ++i)
		{
			_info[i]._ch = i;
		}
	}

	void Compress(const char* filename)
	{
		assert(filename);
		//ͳ���ַ����ֵĴ���
		FILE* fout = fopen(filename, "rb");
		assert(fout);
		int ch = fgetc(fout);

		string CompressFile = filename;
		CompressFile += ".huffman";
		FILE* fin = fopen(CompressFile.c_str(), "wb");
		assert(fin);
		CountInfo info;


		while (ch != EOF)
		{
			_info[(unsigned char)ch]._count++;
			ch = fgetc(fout);
		}

		//����huffman tree
		CharInfo invalid;
		invalid._count = 0;
		HuffmanTree<CharInfo> tree(_info, 256, invalid);

		//����huffman code
		GetHuffmanCode(tree.GetRoot());

		//ѹ��
		//д�����ļ�
		for (size_t i = 0; i < 256; ++i)
		{
			if (_info[i]._count)
			{
				info._ch = _info[i]._ch;
				info._count = _info[i]._count;
				fwrite(&info, sizeof(info), 1, fin);
			}
		}
		info._count = -1;
		fwrite(&info, sizeof(info), 1, fin);

		fseek(fout, 0, SEEK_SET);      //���ص��ļ���ʼ
		ch = fgetc(fout);
		char value = 0;          //������
		int pos = 0;        //����λ��
		while (ch != EOF)
		{
			string& code = _info[(unsigned char)ch]._code;
			size_t i = 0;
			for (i = 0; i < code.size(); ++i)
			{
				value <<= 1;
				++pos;
				if (code[i] == '1')
				{
					value |= 1;
				}
				if (pos == 8)             //��8λд���ļ���
				{
					fputc(value, fin);
					value = 0;
					pos = 0;
				}
			}
			ch = fgetc(fout);
		}
		if (pos)
		{
			value <<= (8 - pos);
			fputc(value, fin);
		}
		fclose(fin);
		fclose(fout);
	}

	void GetHuffmanCode(HuffmanTreeNode<CharInfo>* root)        //huffman����
	{
		if (root == NULL)
		{
			return;
		}

		if (root->_left == NULL && root->_right == NULL)
		{
			HuffmanTreeNode<CharInfo> *parent, *cur;
			cur = root;
			parent = cur->_parent;
			string& code = _info[(unsigned char)root->_w._ch]._code;
			while (parent)
			{
				if (cur == parent->_left)
				{
					code += '0';
				}
				else
				{
					code += '1';
				}
				cur = parent;
				parent = cur->_parent;
			}
			reverse(code.begin(), code.end());
		}
		GetHuffmanCode(root->_left);
		GetHuffmanCode(root->_right);
	}

	//��ѹ
	void UnCompress(const char* filename)
	{
		assert(filename);
		string UnCompressFile = filename;
		size_t index = UnCompressFile.rfind('.');
		assert(index != string::npos);
		UnCompressFile = UnCompressFile.substr(0, index);
		UnCompressFile += ".unhuffman";
		FILE* fout = fopen(filename, "rb");
		assert(fout);
		FILE* fin = fopen(UnCompressFile.c_str(), "wb");
		assert(fin);
		CountInfo info;
		fread(&info, sizeof(CountInfo), 1, fout);

			//��������Ϣ
		while (1)
		{
			fread(&info, sizeof(CountInfo), 1, fout);
			if (info._count == -1)
			{
				break;
			}
			_info[(unsigned char)info._ch]._ch = info._ch;
			_info[(unsigned char)info._ch]._count = info._count;
		}

			//�ؽ�huffman��
		CharInfo invalid;
		invalid._count = 0;
		HuffmanTree<CharInfo> tree(_info, 256, invalid);
		HuffmanTreeNode<CharInfo>* root = tree.GetRoot();
		HuffmanTreeNode<CharInfo>* cur = root;
		LongType count = root->_w._count;
		int value = fgetc(fout);

		if (value == EOF)              //ֻ��һ���ַ�
		{
			if (info._ch != 0)         
			{
				while (cur->_w._count--)
				{
					fputc(cur->_w._ch, fin);
				}
			}
		}
		else
		{
			while (value != EOF)
			{
				int pos = 7;
				char test = 1;
				while (pos >= 0)
				{
					if (value & (test << pos))
					{
						cur = cur->_right;
					}
					else
					{
						cur = cur->_left;
					}
					if (cur->_left == NULL && cur->_right == NULL)
					{
						fputc(cur->_w._ch, fin);
						cur = root;
						if (--count == 0)
						{
							break;
						}
					}
					--pos;
				}
				value = fgetc(fout);
			}
		}

		fclose(fout);
		fclose(fin);
	}

protected:
	CharInfo _info[256];     //�����ַ���Ϣ
};

void TestFileCompress()
{
	FileCompress fc;
	//fc.Compress("Input.txt");
	//fc.UnCompress("Input.txt.huffman");
	//fc.Compress("291154.jpg");
	//fc.UnCompress("291154.jpg.huffman");
	fc.Compress("IMG_5788.mp4");
	fc.UnCompress("IMG_5788.mp4.huffman");
}