#include "Marker.h"
#include <GTAVInfrastructure\SHV\natives.h>



void CMarker::Render(bool bob /*= false*/, bool faceCam /*= true*/, bool drawOnEnter /*= false*/)
{
	GRAPHICS::DRAW_MARKER(GetMarkerType(), Pos().GetX(), Pos().GetY(), Pos().GetZ(), Dir().GetX(), Dir().GetY(), Dir().GetZ(), Rot().GetX(), Rot().GetY(), Rot().GetZ(), Scale().GetX(), Scale().GetY(), Scale().GetZ(), (int)Col().GetR(), (int)Col().GetG(), (int)Col().GetB(), (int)Col().GetA(), bob, faceCam, 0, faceCam, GetTextureDict(), GetTextureName(), drawOnEnter);
}
