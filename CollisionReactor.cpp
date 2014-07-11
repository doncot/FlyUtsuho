#include<Collision.h>

namespace Inferno
{
Vec2<int> KeepSubInsideRect(const Rect& r1, const Rect& r2)
{
	//sがr内に収まるかどうかチェック。収まらないなら、差分を足す
	//sがrの上側にいる
	Vec2<int> result=r1.Center();
	//sがrの左側にいる
	if (r1.Left() < r2.Left())
	{
		result.x = r1.Width() / 2;
	}
	//sがrの右側にいる
	else if (r1.Right() > r2.Right())
	{
		result.x = r2.Right() - r1.Width() / 2;
	}
	if (r1.Top() < r2.Top())
	{
		result.y = r1.Height() / 2;
	}
	//sがrの下側にいる
	else if (r1.Bottom() > r2.Bottom())
	{
		result.y = r2.Bottom() - r1.Height() / 2;
	}
	return result;
}


}