/*
�A�N�V�����Q�[���ȂǂŃV�[����\���N���X
*/
#pragma once

namespace Inferno
{
class Scene
{
public:
	void Create();
	void Destroy();

	//�V�[���̖{�́B1�t���[�����Ƃ�game���[�v���Ăяo���B
	//�Ԃ�l�F���̃V�[����Ԃ��i�V�[��ID�ŁB���ƂŒ�`�j
	bool SceneLoop();
//	bool AddEvent();

};

}