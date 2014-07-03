#include<Collision.h>

namespace Inferno
{
void KeepSubInsideRect(Substance* s, const Rect& r)
{
	//sがr内に収まるかどうかチェック。収まらないなら、差分を足す
	//sがrの上側にいる
	Vec2<int> result=s->GetPosition();
	if (s->GetRegion().Top() < r.Top())
	{
		result.y = s->GetRegion().Height() / 2;
	}
	//sがrの下側にいる
	if (s->GetRegion().Bottom() > r.Bottom())
	{
		result.y = r.Bottom() - s->GetRegion().Height() / 2;
	}
	//sがrの左側にいる
	if (s->GetRegion().Left() < r.Left())
	{
		result.x = s->GetRegion().Width() / 2;
	}
	//sがrの右側にいる
	if (s->GetRegion().Right() > r.Right())
	{
		result.x = r.Right() - s->GetRegion().Width() / 2;
	}
	s->AMove(result);
}


}