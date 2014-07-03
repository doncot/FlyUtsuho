#include<Collision.h>

namespace Inferno
{
void KeepSubInsideRect(Substance* s, const Rect& r)
{
	//s��r���Ɏ��܂邩�ǂ����`�F�b�N�B���܂�Ȃ��Ȃ�A�����𑫂�
	//s��r�̏㑤�ɂ���
	Vec2<int> result=s->GetPosition();
	if (s->GetRegion().Top() < r.Top())
	{
		result.y = s->GetRegion().Height() / 2;
	}
	//s��r�̉����ɂ���
	if (s->GetRegion().Bottom() > r.Bottom())
	{
		result.y = r.Bottom() - s->GetRegion().Height() / 2;
	}
	//s��r�̍����ɂ���
	if (s->GetRegion().Left() < r.Left())
	{
		result.x = s->GetRegion().Width() / 2;
	}
	//s��r�̉E���ɂ���
	if (s->GetRegion().Right() > r.Right())
	{
		result.x = r.Right() - s->GetRegion().Width() / 2;
	}
	s->AMove(result);
}


}