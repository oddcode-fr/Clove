#ifndef __BITMAP32_H
#define __BITMAP32_H

#include "Agg\agg_rendering_buffer.h"
#include "Agg\agg_pixfmt_rgba.h"
#include "Agg\agg_renderer_base.h"

#include "Buffer32.h"

using namespace System;

namespace Clove
{
	struct NativeBitmap32
	{
		agg::rendering_buffer m_rbuf;
		agg::pixfmt_bgra32 m_pixf;

		NativeBitmap32(agg::int8u* buf, agg::int32u width, agg::int32u height, agg::int32 stride) :
			m_rbuf(buf, width, height, stride),
			m_pixf(m_rbuf)
		{

		}
	};

	public ref class Bitmap32
	{
	private:
		Int32 m_x;
		Int32 m_y;

	internal:
		Buffer32^ m_buffer;
		NativeBitmap32* m_bitmap;

	public:
	
		Bitmap32(Int32 width, Int32 height);
		Bitmap32(Bitmap32^ parent, Int32 x, Int32 y, Int32 width, Int32 height);

		~Bitmap32();

		property Int32 Width { Int32 get() { return m_bitmap->m_pixf.width(); } }
		property Int32 Height { Int32 get() { return m_bitmap->m_pixf.height(); } }

		void Save(String^ filename);
	};
}

#endif