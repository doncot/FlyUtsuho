#pragma once
#include"Inferno\GameElements.h"
#include<list>
#include<queue>
#include"Inferno\Graphics.h"
#include"Animation.h"

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

	private:
		typedef BaseTask Base;
		Vec2<int> m_pos;
	};

	class MoveTask : public BaseTask
	{
	public:
		enum class ValueType
		{
			trans_x,
			trans_y
		};

	public:
		MoveTask(const int taskId) : Base(taskId) {}
		void SetKeyFrameSet(const Animation::KeyFrameSet set, const ValueType type, const Timer& gameTimer)
		{
			m_keyFrameSet = set;
			m_valueType = type;
			m_registeredTime = gameTimer.GetElapsed();
		}

	private:
		typedef BaseTask Base;
		Millisec m_registeredTime;
		Animation::KeyFrameSet m_keyFrameSet;
		ValueType m_valueType;
	};

public:
	~ShooterScene();

	//描画はこのクラスが担当するので、Graphicsを持つのは自然
	void SetGraphics(const Graphics& g);

	//登録関数（シーンがスタートする前に使う。命令によってはスタート後にも使える）
	void CreateEnemy(const int id);
	void DeleteEnemy(const int id);

	void SetImageFromFile(const int id, const wstring& filename);

	void RegisterDeploy(const int id, const Millisec deployTime, const Vec2<int> deployCor);

	void RegisterRMove(const int id, const Millisec delay, const Millisec dur,
		const Vec2<int> cor, const Animation::TransitType ttype);

	//シーンをスタート
	void Start();
	void Update();

	void Draw();

	bool HasStarted() const;

private:
	void OnTaskStart(DeployTask* task);


private:
	const Graphics* m_graphics;

	std::list<Substance*> m_subList;
	std::list<EnemyBase*> m_baseList;
	std::list<BaseTask*> m_taskList;
	//std::queue<GMessage> m_messageQueue;

	//シーンが開始してからの経過時間
	Timer m_sceneTimer;
	//シーンが開始してから操作無効時間を除いた経過時間のタイマー
	Timer m_activeSceneTimer;
};


}