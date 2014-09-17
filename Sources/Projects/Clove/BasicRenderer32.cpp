
#include "BasicRenderer32.h"


namespace Clove
{

	BasicRenderer32::BasicRenderer32(Bitmap32^ bitmap)
	{
		m_bitmap = bitmap;
		m_renderer = new NativeBasicRenderer32(bitmap->m_bitmap);
	}

	BasicRenderer32::~BasicRenderer32()
	{
		delete m_renderer;
	}

}