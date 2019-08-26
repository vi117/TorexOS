#pragma once
#include<stddef.h>
#include<stdint.h>
#include<stringutil.h>

namespace util
{
	template<typename _elem, size_t _size>
	class CircularQueue
	{
	public:
		typedef _elem elementType;
		enum
		{
			size = _size,
		};
		CircularQueue():first(0),last(0),isfull(false),data{0,}{}
		
		bool full(){
			return isfull;
		}
		bool empty(){
			if (isfull)
				return false;
			else return (first == last);
		}
		elementType & front(){ return data[first]; }
		elementType pop(){
			if (!empty()){
				auto ret = data[first];
				first++;
				if (size == first)
					first = 0;
				isfull = false;
				return ret;
			}
			return elementType();
		}
		void push(const elementType & elem)
		{
			if (!full()){
				data[last] = elem;
				++last;
				if (last == size)
					last = 0;
				if (first == last) isfull = true;
			}
		}

	private:
		uint32_t first;
		uint32_t last;
		bool isfull;
		uint8_t padding[3];
		elementType data[size];
	};
}