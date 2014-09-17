#ifndef __COLOR_H
#define __COLOR_H

#include "Agg\agg_color_rgba.h"

using namespace System;

namespace Clove
{
	public value class Color32
	{
	private:
		Byte m_r, m_g, m_b, m_a;

	internal:
		Color32(agg::rgba8 c) : m_r(c.r), m_g(c.g), m_b(c.b), m_a(c.a) {}
		agg::rgba8 ToAggColor() { return agg::rgba8(m_r, m_g, m_b, m_a); } 

	public:
		Color32(Byte r, Byte g, Byte b, Byte a) : 
			m_b(b), m_g(g), m_r(r), m_a(a)
		{

		}

		property Byte R { Byte get() { return m_r; } }
		property Byte G { Byte get() { return m_g; } }
		property Byte B { Byte get() { return m_b; } }
		property Byte A { Byte get() { return m_a; } }

	};
}

#endif