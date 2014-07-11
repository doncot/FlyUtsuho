#include<Collision.h>

namespace Inferno
{
Vec2<int> KeepSubInsideRect(const Rect& r1, const Rect& r2)
{
	//s��r���Ɏ��܂邩�ǂ����`�F�b�N�B���܂�Ȃ��Ȃ�A�����𑫂�
	//s��r�̏㑤�ɂ���
	Vec2<int> result=r1.Center();
	//s��r�̍����ɂ���
	if (r1.Left() < r2.Left())
	{
		result.x = r1.Width() / 2;
	}
	//s��r�̉E���ɂ���
	else if (r1.Right() > r2.Right())
	{
		result.x = r2.Right() - r1.Width() / 2;
	}
	if (r1.Top() < r2.Top())
	{
		result.y = r1.Height() / 2;
	}
	//s��r�̉����ɂ���
	else if (r1.Bottom() > r2.Bottom())
	{
		result.y = r2.Bottom() - r1.Height() / 2;
	}
	return result;
}


}