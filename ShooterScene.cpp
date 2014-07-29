#include"ShooterScene.h"

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

	void ShooterScene::SetGraphics(const Graphics& g) {	m_graphics = &g; }

	void ShooterScene::CreateEnemy(const int id)
	{
		auto insert = new Substance();
		insert->SetID(id);
		insert->SetAttribute(GEAttribute::Draw, false);
		m_subList.push_back(insert);
	}

	void ShooterScene::SetImageFromFile(const int id, const wstring& filename)
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

	void ShooterScene::DeployEnemy(const int id, const Millisec deployTime, const Vec2<int> deployCor)
	{
		//該当のidを持つSubstanceをデプロイ
		for (auto e : m_subList)
		{
			//idが一致したら
			if ( e->GetID() == id)
			{
				auto newTask = new DeployTask(id);
				newTask->SetDeployTiming(deployTime);
				newTask->SetDeployCor(deployCor);

				m_taskList.push_back(newTask);
			}
		}
	}

	void ShooterScene::Start()
	{
		m_sceneTimer.Start();
		m_activeSceneTimer.Start();
	}

	void ShooterScene::Update()
	{
		//全タスクを回す
		for (auto e = m_taskList.begin(); e != m_taskList.end();)
		{
			//タスクの開始で
			if (m_activeSceneTimer.GetElapsed() > (*e)->GetDeployTiming())
			{
				OnTaskStart( dynamic_cast<DeployTask*>(*e) );

				//このタスクはここで消去
				SAFE_DELETE(*e);
				e = m_taskList.erase(e);

				continue;
			}
			e++;
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

	//ShooterScene private
	void ShooterScene::OnTaskStart(DeployTask* task)
	{
		//このサーチは後々IDを直接探せるように再実装
		for (auto e : m_subList)
		{
			if (task->GetID() == e->GetID())
			{
				e->AMove(task->GetDeployCor());
				//表示
				e->SetAttribute(GEAttribute::Draw, true);
			}
		}
	}


}