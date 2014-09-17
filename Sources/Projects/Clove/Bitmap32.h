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
		Buffer32^ m_buffer;
		UInt32 m_x;
		UInt32 m_y;

	internal:
		NativeBitmap32* m_bitmap;

	public:
	
		Bitmap32(UInt32 width, UInt32 height);
		Bitmap32(Bitmap32^ parent, UInt32 x, UInt32 y, UInt32 width, UInt32 height);

		~Bitmap32();

		property UInt32 Width { UInt32 get() { return m_bitmap->m_pixf.width(); } }
		property UInt32 Height { UInt32 get() { return m_bitmap->m_pixf.height(); } }

		void Save(String^ filename);
	};
}

#endif