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

				m_taskList.push_back(newTask);
			}
		}
	}

	void ShooterScene::Update()
	{
		//全タスクを回す
		for (auto e : m_taskList)
		{
			//タスクの開始で

		}

	}

	//ShooterScene private
	void ShooterScene::OnTaskStart(BaseTask* task)
	{
		if (m_activeSceneTimer.GetElapsed() > task->GetDeployTiming())
		{

		}
	}
}