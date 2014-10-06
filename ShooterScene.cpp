#include"ShooterScene.h"
#include"Inferno\Collision.h"
#include<algorithm>

namespace Inferno
{
	ShooterScene::~ShooterScene()
	{
		for (auto i = m_enemies.begin(); i != m_enemies.end();)
		{
			SAFE_DELETE(i->second);
			i = m_enemies.erase(i);
		}

		for (auto i = m_taskList.begin(); i != m_taskList.end();)
		{
			SAFE_DELETE(*i);
			i = m_taskList.erase(i);
		}

		//queue
		{
			/*
			for (auto& e = m_taskQueue.front(); m_taskQueue.empty()!=true; e++)
			{
				SAFE_DELETE(e);
				m_taskQueue.pop();
			}
			*/
			//スワップトリック
			/*
			std::queue<BaseTask*> emptyTaskQueue;
			std::swap(m_taskQueue, emptyTaskQueue);
			*/
		}

	}

	void ShooterScene::InitializeScene(const Graphics& g, const Rect& clientSize)
	{
		//Graphicsはここでキープ
		m_graphics = &g;
	}

	void ShooterScene::CreateEnemyInstance(const std::wstring& resourceName,const std::wstring& instanceName)
	{
		auto newEnemy = ResourceManager::CreateEnemyInstance(resourceName);
		m_enemies.emplace(instanceName,newEnemy);
	}
	 
	void ShooterScene::CreateEnemyResourceFromFile(const ResourceHandle& hResource,
		const wstring& filename)
	{
		ResourceManager::SetEnemy(hResource, filename);
	}

	void ShooterScene::RegisterDeploy(const wstring& instance, const Millisec deployTime, const Vec2<int> deployCor)
	{
		//Deployタスクを生成
		auto newTask = new DeployTask(instance);
		newTask->SetDeployTiming(deployTime);
		newTask->SetDeployCor(deployCor);

		m_taskList.push_back(newTask);
	}

	void ShooterScene::RegisterMove(const wstring& instance, const Millisec delay, const Millisec dur,
		const Vec2<int> from, const Vec2<int> to, const Animation::TransitType ttype)
	{
		//移動タスクを登録
		auto newTask = new MoveTask(instance);
		newTask->SetDeployTiming(delay);
		newTask->SetTimer(m_activeSceneTimer);
		newTask->SetMove(dur, from, to, ttype);

		m_taskList.push_back(newTask);
	}

	bool ShooterScene::ProcessPlayerBulletToEnemyHit(const Bullet& bullet)
	{
		for (auto sub_i = m_enemies.begin(); sub_i != m_enemies.end();)
		{
			if (IsRect1HittingRect2(sub_i->second->GetHitBox(),bullet.GetHitBox()))
			{
				//当たっていたら
				//体力とか減らすのかな本当は
				//（ここままじゃ死亡時のアニメがないな、どう伝えよう）
				SAFE_DELETE( sub_i->second );
				sub_i = m_enemies.erase(sub_i);

				//上の層で処理するために当たった事を知らせる
				return true;
			}
			sub_i++;
		}
		return false;
	}

	bool ShooterScene::ProcessPlayerToEnemyHit(const Rect& player)
	{

		//TODO:アルゴリズムを使いたいが、分からない
		//auto enemyItr = std::find_if(m_enemies.begin(), m_enemies.end(),[&](std::unordered_map<std::wstring,Enemy*> enemies) -> bool
		//{
		//	return true;
		//}
		//);
		for (auto i = m_enemies.begin(); i != m_enemies.end(); i++)
		{
			if (IsRect1HittingRect2(player, (*i).second->GetHitBox()))
			{
				return true;
			}
		}

		return false;
	}

	bool ShooterScene::ProcessEnemyBulletToPlayerHit(const Rect& player)
	{
		for (auto sub_i = m_enemies.begin(); sub_i != m_enemies.end();)
		{
			if (IsRect1HittingRect2(player, sub_i->second->GetHitBox()))
			{
				//敵へのダメージは無し、自機へヒットを伝える為に上に伝達
				return true;
			}

			if (sub_i->second->CheckBulletHit(player))
			{
				//弾を消し
				SAFE_DELETE(sub_i->second);
				//自機がダメージを受けた事を伝達
				return true;
			}

			sub_i++;
		}
		return false;
	}

	void ShooterScene::Start()
	{
		m_sceneTimer.Start();
		m_activeSceneTimer.Start();
	}

	void ShooterScene::Update()
	{
		//全タスクを回す
		for (auto e_t = m_taskList.begin(); e_t != m_taskList.end();)
		{
			//タスク開始時間でない場合（ここで切る事によりだいぶパフォーマンスが変わる）
			if (m_activeSceneTimer.GetElapsed() < (*e_t)->GetDeployTiming())
			{
				e_t++; //これがないと開始時間になるまでループし続ける
				continue;
			}

			//ここら辺は全部Task->DoTaskで
			//TODO:taskのlistは全部substanceが持つべき？
			//暫定処理としてm_enemiesをtaskに渡すか？
			const wstring targetSub = (*e_t)->GetInstanceName();
			//ここでSubListを探索
			//非常によくないO(n^2)
			for (auto e_s = m_enemies.begin(); e_s != m_enemies.end();)
			{
				//該当タスクの場合
				if (targetSub == e_s->first)
				{
					//実行（だいぶネストがひどい……）
					bool result = (*e_t)->Do(e_s->second);
					if ( result )
					{
						//終了したらタスクを消す
						SAFE_DELETE(*e_t);
						e_t = m_taskList.erase(e_t);
					}
					//この時点でタスクは必ず実行されているので、次以降のSubに処理が渡ることはない
					break;
				}
				e_s++;
			}
			//タスクリストが終わってるのにまだ消そうとしてはいけない（2重ループが分かりにくさせている）
			if ( e_t != m_taskList.end() ) e_t++;
		}

		//プレイヤーの移動

	}

	void ShooterScene::Draw()
	{
		for (auto e : m_enemies)
		{
			e.second->Draw(*m_graphics);
		}
	}


	bool ShooterScene::HasStarted() const
	{
		return m_sceneTimer.HasStarted();
	}
}