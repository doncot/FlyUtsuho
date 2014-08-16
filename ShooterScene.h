#pragma once
#include"Inferno\GameElements.h"
#include"ShooterElements.h"
#include<list>
#include<queue>
#include"Animation.h"
#include"Inferno\MyException.h"
#include"Task.h"
#include<unordered_map>

class Graphics;

namespace Inferno
{
class ShooterScene
{
public:
	~ShooterScene();

	//�`��͂��̃N���X���S������̂ŁAGraphics�����͎̂��R
	//���ʂ͂��̒��Ŏ��@�̐ݒ������i�G�ƈ���Ă���͂���ܕς��񂾂낤�j
	void InitializeScene(const Graphics& g, const Rect& clientSize);

	//�o�^�֐��i�V�[�����X�^�[�g����O�Ɏg���B���߂ɂ���Ă̓X�^�[�g��ɂ��g����j
	void CreateEnemyInstance(const std::wstring& resourceName, const std::wstring& instanceName);
	void DeleteEnemy(const int id);

	void CreateEnemyResourceFromFile(const ResourceHandle& hResource, const wstring& filename);

	void RegisterDeploy(const wstring& instance, const Millisec deployTime, const Vec2<int> deployCor);

	void RegisterMove(const wstring& instance, const Millisec delay, const Millisec dur,
		const Vec2<int> from, const Vec2<int> to, const Animation::TransitType ttype);

	//���@�e�ƓG�̏Փˏ����i���̒��œG�����ʂƂ��܂ł��j
	//���{�����̏����̓V�[�����ł�邽�߁Aplayer��scene�Ɉڂ�����͂���Ȃ��֐�
	bool ProcessPlayerBulletToEnemyHit(const Bullet& bullet);
	//�G�e�Ǝw��̕��̂Ƃ̏Փ˔���i�����ŗ���̂�player�j
	bool ProcessEnemyBulletToPlayerHit(const Rect& player);

	//�V�[�����X�^�[�g
	void Start();
	void Update();

	void Draw();

	bool HasStarted() const;

private:
	const Graphics* m_graphics;

	std::unordered_map<std::wstring,Substance*> m_substances;
	//std::list<EnemyBase*> m_baseList;
	std::list<BaseTask*> m_taskList;
	//std::queue<GMessage> m_messageQueue;

	//�V�[�����J�n���Ă���̌o�ߎ���
	Timer m_sceneTimer;
	//�V�[�����J�n���Ă��瑀�얳�����Ԃ��������o�ߎ��Ԃ̃^�C�}�[
	Timer m_activeSceneTimer;
};


}