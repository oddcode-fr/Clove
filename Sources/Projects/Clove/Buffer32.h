#ifndef __BUFFER_32
#define __BUFFER_32

#include "Agg\agg_basics.h"

using namespace System;

namespace Clove {

	ref class Buffer32
	{
	private:
		agg::int8u* m_data;
		Int32 m_width;
		Int32 m_height;

	public:
		Buffer32(Int32 width, Int32 height)
		{
			m_data = new agg::int8u[width * height * 4];
			m_width = width;
			m_height = height;
		}

		~Buffer32()
		{
			delete[] m_data;
		}

		property Int32 Width { Int32 get() { return m_width; } }
		property Int32 Height { Int32 get() { return m_height; } }
		property Int32 Stride { Int32 get() { return m_width * 4; } }

		property agg::int8u* Data { agg::int8u* get() { return m_data; }}
	};
}

#endif