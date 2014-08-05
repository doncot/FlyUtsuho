#pragma once
#include"Inferno\GameElements.h"
#include<list>
#include<queue>
#include"Inferno\Graphics.h"
#include"Animation.h"
#include"Inferno\MyException.h"

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

		virtual ~BaseTask(){}

		void SetDeployTiming(const Millisec& msec) { m_deployTiming = msec; }
		Millisec GetDeployTiming() const { return m_deployTiming; }
		
		int GetID() const { return m_id; }

		//�^�X�N�����s����
		//�I��������true��Ԃ�
		virtual bool Do(Substance* sub) = 0;

	protected:
		const int m_id = 0;
		Millisec m_deployTiming = 0;
	};

	class DeployTask : public BaseTask
	{
	public:
		DeployTask(const int taskId): Base(taskId) {}

		void SetDeployCor(const Vec2<int> c) { m_pos = c; }
		const Vec2<int> GetDeployCor() const { return m_pos; }

		bool Do(Substance* sub)
		{
			sub->AMove(m_pos);
			sub->SetAttribute(GEAttribute::Draw, true);
			return true;
		}

	private:
		typedef BaseTask Base;
		Vec2<int> m_pos;
	};

	class MoveTask : public BaseTask
	{
	public:
		MoveTask(const int taskId) : Base(taskId) {}
		void SetMove(const Millisec dur, const Vec2<int> from, const Vec2<int> to,
			const Animation::TransitType ttype)
		{
			m_xmove.Set(this->GetDeployTiming(),
				dur, from.x, to.x, ttype);
			m_ymove.Set(this->GetDeployTiming(),
				dur, from.y, to.y, ttype);
		}

		//�����Ȃ�Ƃ��Ȃ�񂩂�
		void SetTimer(const Timer& timer) { m_timer = &timer; }

		bool Do(Substance* sub)
		{
			if (m_timer == nullptr) throw L"Timer is not set.";
			sub->AMove(m_xmove.GetValue(*m_timer), sub->GetPosition().y);
			sub->AMove(sub->GetPosition().x, m_ymove.GetValue(*m_timer));
			return m_xmove.HasEnded(*m_timer);
		}

	private:
		typedef BaseTask Base;
		const Timer* m_timer = nullptr;
		Millisec m_dur;
		//�ėp�����Ȃ�������
		Animation m_xmove;
		Animation m_ymove;
		Vec2<int> m_from;
		Vec2<int> m_to;
		Animation::TransitType m_ttype;
	};

public:
	~ShooterScene();

	//�`��͂��̃N���X���S������̂ŁAGraphics�����͎̂��R
	void SetGraphics(const Graphics& g);

	//�o�^�֐��i�V�[�����X�^�[�g����O�Ɏg���B���߂ɂ���Ă̓X�^�[�g��ɂ��g����j
	void CreateEnemy(const int id);
	void DeleteEnemy(const int id);

	void SetImageFromFile(const int id, const wstring& filename);

	void RegisterDeploy(const int id, const Millisec deployTime, const Vec2<int> deployCor);

	void RegisterMove(const int id, const Millisec delay, const Millisec dur,
		const Vec2<int> from, const Vec2<int> to, const Animation::TransitType ttype);


	//�V�[�����X�^�[�g
	void Start();
	void Update();

	void Draw();

	bool HasStarted() const;

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