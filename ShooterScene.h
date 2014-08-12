#pragma once
#include"Inferno\GameElements.h"
#include"ShooterElements.h"
#include<list>
#include<queue>
#include"Animation.h"
#include"Inferno\MyException.h"

class Graphics;

namespace Inferno
{
class ShooterScene
{
private:
	//これはリソースマネージャーが管理するので将来的にはいらない
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

		//タスクを実行する
		//終了したらtrueを返す
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
			sub->SetAttribute(GEAttribute::Visible, true);
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

		//ここなんとかならんかな
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
		//汎用性がないやり方だ
		Animation m_xmove;
		Animation m_ymove;
		Vec2<int> m_from;
		Vec2<int> m_to;
		Animation::TransitType m_ttype;
	};

public:
	~ShooterScene();

	//描画はこのクラスが担当するので、Graphicsを持つのは自然
	//当面はこの中で自機の設定をする（敵と違ってこれはあんま変わらんだろう）
	void InitializeScene(const Graphics& g, const Rect& clientSize);

	//登録関数（シーンがスタートする前に使う。命令によってはスタート後にも使える）
	void CreateEnemy(const int id);
	void DeleteEnemy(const int id);

	void SetImageFromFile(const int id, const wstring& filename);

	void RegisterDeploy(const int id, const Millisec deployTime, const Vec2<int> deployCor);

	void RegisterMove(const int id, const Millisec delay, const Millisec dur,
		const Vec2<int> from, const Vec2<int> to, const Animation::TransitType ttype);

	//自機弾と敵の衝突処理（この中で敵が死ぬとこまでやる）
	//※本来この処理はシーン内でやるため、playerをsceneに移した後はいらない関数
	bool ProcessPlayerBulletToEnemyHit(const Bullet& bullet);
	//敵弾と指定の物体との衝突判定（ここで来るのはplayer）
	bool ProcessEnemyBulletToPlayerHit(const Rect& player);

	//シーンをスタート
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

	//シーンが開始してからの経過時間
	Timer m_sceneTimer;
	//シーンが開始してから操作無効時間を除いた経過時間のタイマー
	Timer m_activeSceneTimer;
};


}