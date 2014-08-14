#include"ShooterScene.h"
#include"Inferno\Collision.h"

namespace Inferno
{
	ShooterScene::~ShooterScene()
	{
		for (auto i = m_subList.begin(); i != m_subList.end();)
		{
			SAFE_DELETE(*i);
			i = m_subList.erase(i);
		}

		for (auto i = m_baseList.begin(); i != m_baseList.end();)
		{
			SAFE_DELETE(*i);
			i = m_baseList.erase(i);
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

	void ShooterScene::CreateEnemy(const int id)
	{
		auto insert = new Enemy();
		insert->SetID(id);
		insert->SetAttribute(GEAttribute::Visible, false);
		m_subList.push_back(insert);
	}
	 
	void ShooterScene::CreateResourceFromFile(const int id, const wstring& filename)
	{
		auto newBase = new EnemyBase();
		newBase->m_tex.LoadImageFile(*m_graphics, filename);
		newBase->m_idea.SetTexture(newBase->m_tex);
		//TODO:Ideaに重複があったら抜く

		//新しいBaseを登録
		m_baseList.push_back(newBase);


		//該当のidを持つSubstanceにEnemyBaseをセット
		for (auto e : m_subList)
		{
			//idが一致したら
			if (e->GetID() == id)
			{
				e->SetIdea(newBase->m_idea);
			}
		}
	}

	void ShooterScene::RegisterDeploy(const int id, const Millisec deployTime, const Vec2<int> deployCor)
	{
		//Deployタスクを生成
		auto newTask = new DeployTask(id);
		newTask->SetDeployTiming(deployTime);
		newTask->SetDeployCor(deployCor);

		m_taskList.push_back(newTask);
	}

	void ShooterScene::RegisterMove(const int id, const Millisec delay, const Millisec dur,
		const Vec2<int> from, const Vec2<int> to, const Animation::TransitType ttype)
	{
		//移動タスクを登録
		auto newTask = new MoveTask(id);
		newTask->SetDeployTiming(delay);
		newTask->SetTimer(m_activeSceneTimer);
		newTask->SetMove(dur, from, to, ttype);

		m_taskList.push_back(newTask);
	}

	bool ShooterScene::ProcessPlayerBulletToEnemyHit(const Bullet& bullet)
	{
		for (auto sub_i = m_subList.begin(); sub_i != m_subList.end();)
		{
			if (IsRect1HittingRect2((*sub_i)->GetHitBox(),bullet.GetHitBox()))
			{
				//当たっていたら
				//体力とか減らすのかな本当は
				//（ここままじゃ死亡時のアニメがないな、どう伝えよう）
				SAFE_DELETE(*sub_i);
				sub_i = m_subList.erase(sub_i);

				//上の層で処理するために当たった事を知らせる
				return true;
			}
			sub_i++;
		}
		return false;
	}

	bool ShooterScene::ProcessEnemyBulletToPlayerHit(const Rect& rect)
	{
		for (auto sub_i = m_subList.begin(); sub_i != m_subList.end();)
		{

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
			//暫定処理としてm_subListをtaskに渡すか？
			int targetSub = (*e_t)->GetID();
			//ここでSubListを探索
			//非常によくないO(n^2)
			for (auto e_s = m_subList.begin(); e_s != m_subList.end();)
			{
				//該当タスクの場合
				if (targetSub == (*e_s)->GetID())
				{
					//実行（だいぶネストがひどい……）
					bool result = (*e_t)->Do((*e_s));
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
		for (auto e : m_subList)
		{
			e->Draw(*m_graphics);
		}
	}


	bool ShooterScene::HasStarted() const
	{
		return m_sceneTimer.HasStarted();
	}
}