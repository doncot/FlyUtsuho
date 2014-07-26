#pragma once
#include"Inferno\GameElements.h"
#include<list>
#include<queue>
#include"Inferno\Graphics.h"

namespace Inferno
{

class ShooterScene
{
private:
	struct EnemyBase
	{
		Inferno::Idea m_idea;
		Inferno::Texture m_tex;
	};

	class BaseTask
	{
	public:
		BaseTask(const int taskId) : m_id(taskId) {}
		//new/delete����\�肪�Ȃ��̂ŁA���z�f�X�g���N�^�͗p�ӂ��Ȃ�
		//virtual ~BaseTask();

		void SetDeployTiming(const Millisec& msec) { m_deployTiming = msec; }
		Millisec GetDeployTiming() const { return m_deployTiming; }

	protected:
		const int m_id = 0;
		Millisec m_deployTiming = 0;
	};

	class DeployTask : public BaseTask
	{
	public:
		DeployTask(const int taskId): Base(taskId) {}

	private:
		typedef BaseTask Base;
	};

public:
	~ShooterScene();

	//�`��͂��̃N���X���S������̂ŁAGraphics�����͎̂��R
	void SetGraphics(const Graphics& g);

	//�o�^�֐��i�V�[�����X�^�[�g����O�Ɏg���B���߂ɂ���Ă̓X�^�[�g��ɂ��g����j
	void CreateEnemy(const int id);
	void DeleteEnemy(const int id);

	void SetImageFromFile(const int id, const wstring& filename);

	void DeployEnemy(const int id, const Millisec deployTime, const Vec2<int> deployCor);

	//�V�[�����X�^�[�g
	void Start();
	void Update();

	void Draw();

private:
	void OnTaskStart(BaseTask* task);


private:
	const Graphics* m_graphics;

	std::list<Substance*> m_subList;
	std::list<EnemyBase*> m_baseList;
	std::list<BaseTask*> m_taskList;
	//std::queue<GMessage> m_messageQueue;

	//�V�[�����J�n���Ă���̌o�ߎ���
	Timer m_sceneTimer;
	//�V�[�����J�n���Ă��瑀�얳�����Ԃ��������o�ߎ��Ԃ̃^�C�}�[
	Timer m_activeSceneTimer;
};


}