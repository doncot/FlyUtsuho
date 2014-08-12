#include<Collision.h>
//Õ“Ë”»’èŒn‚ ‚ê‚â‚±‚ê‚â‚ÌÀ‘•

namespace Inferno{

bool IsPointInsideRect(const int x, const int y, const Rect& r)
{
	if (r.Left() <= x && x <= r.Right() &&
		r.Top() <= y && y <= r.Bottom())
	{
		return true;
	}
	else return false;
}
	
bool IsPointInsideRect(const Vec2<int>& v, const Rect& r)
{
	return IsPointInsideRect(v.x, v.y, r);
}

bool IsRect1NotHittingRect2(const Rect& r1, const Rect& r2)
{
	if ((r1.Left() < r2.Right() || r1.Right() > r2.Left()) ||
		(r1.Top() < r2.Bottom() || r1.Bottom() > r2.Top()))
	{
		return true;
	}
	else return false;
}


bool IsRect1HittingRect2(const Rect& r1, const Rect& r2)
{
	if ((r1.Left() < r2.Right() && r1.Right() > r2.Left()) &&
		(r1.Top() < r2.Bottom() && r1.Bottom() > r2.Top()))
	{
		return true;
	}
	else return false;
}


}