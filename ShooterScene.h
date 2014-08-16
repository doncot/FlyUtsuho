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

	//描画はこのクラスが担当するので、Graphicsを持つのは自然
	//当面はこの中で自機の設定をする（敵と違ってこれはあんま変わらんだろう）
	void InitializeScene(const Graphics& g, const Rect& clientSize);

	//登録関数（シーンがスタートする前に使う。命令によってはスタート後にも使える）
	void CreateEnemyInstance(const std::wstring& resourceName, const std::wstring& instanceName);
	void DeleteEnemy(const int id);

	void CreateEnemyResourceFromFile(const ResourceHandle& hResource, const wstring& filename);

	void RegisterDeploy(const wstring& instance, const Millisec deployTime, const Vec2<int> deployCor);

	void RegisterMove(const wstring& instance, const Millisec delay, const Millisec dur,
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

	std::unordered_map<std::wstring,Substance*> m_substances;
	//std::list<EnemyBase*> m_baseList;
	std::list<BaseTask*> m_taskList;
	//std::queue<GMessage> m_messageQueue;

	//シーンが開始してからの経過時間
	Timer m_sceneTimer;
	//シーンが開始してから操作無効時間を除いた経過時間のタイマー
	Timer m_activeSceneTimer;
};


}