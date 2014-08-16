#include"ShooterScene.h"
#include"Inferno\Collision.h"

namespace Inferno
{
	ShooterScene::~ShooterScene()
	{
		for (auto i = m_substances.begin(); i != m_substances.end();)
		{
			SAFE_DELETE(i->second);
			i = m_substances.erase(i);
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
			//�X���b�v�g���b�N
			/*
			std::queue<BaseTask*> emptyTaskQueue;
			std::swap(m_taskQueue, emptyTaskQueue);
			*/
		}

	}

	void ShooterScene::InitializeScene(const Graphics& g, const Rect& clientSize)
	{
		//Graphics�͂����ŃL�[�v
		m_graphics = &g;
	}

	void ShooterScene::CreateEnemyInstance(const std::wstring& resourceName,const std::wstring& instanceName)
	{
		auto newEnemy = ResourceManager::CreateEnemyInstance(resourceName);
		m_substances.emplace(instanceName,newEnemy);
	}
	 
	void ShooterScene::CreateEnemyResourceFromFile(const ResourceHandle& hResource,
		const wstring& filename)
	{
		ResourceManager::SetEnemy(hResource, filename);
	}

	void ShooterScene::RegisterDeploy(const wstring& instance, const Millisec deployTime, const Vec2<int> deployCor)
	{
		//Deploy�^�X�N�𐶐�
		auto newTask = new DeployTask(instance);
		newTask->SetDeployTiming(deployTime);
		newTask->SetDeployCor(deployCor);

		m_taskList.push_back(newTask);
	}

	void ShooterScene::RegisterMove(const wstring& instance, const Millisec delay, const Millisec dur,
		const Vec2<int> from, const Vec2<int> to, const Animation::TransitType ttype)
	{
		//�ړ��^�X�N��o�^
		auto newTask = new MoveTask(instance);
		newTask->SetDeployTiming(delay);
		newTask->SetTimer(m_activeSceneTimer);
		newTask->SetMove(dur, from, to, ttype);

		m_taskList.push_back(newTask);
	}

	bool ShooterScene::ProcessPlayerBulletToEnemyHit(const Bullet& bullet)
	{
		for (auto sub_i = m_substances.begin(); sub_i != m_substances.end();)
		{
			if (IsRect1HittingRect2(sub_i->second->GetHitBox(),bullet.GetHitBox()))
			{
				//�������Ă�����
				//�̗͂Ƃ����炷�̂��Ȗ{����
				//�i�����܂܂��ᎀ�S���̃A�j�����Ȃ��ȁA�ǂ��`���悤�j
				SAFE_DELETE( sub_i->second );
				sub_i = m_substances.erase(sub_i);

				//��̑w�ŏ������邽�߂ɓ�����������m�点��
				return true;
			}
			sub_i++;
		}
		return false;
	}

	bool ShooterScene::ProcessEnemyBulletToPlayerHit(const Rect& rect)
	{
		for (auto sub_i = m_substances.begin(); sub_i != m_substances.end();)
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
		//�S�^�X�N����
		for (auto e_t = m_taskList.begin(); e_t != m_taskList.end();)
		{
			//�^�X�N�J�n���ԂłȂ��ꍇ�i�����Ő؂鎖�ɂ�肾���ԃp�t�H�[�}���X���ς��j
			if (m_activeSceneTimer.GetElapsed() < (*e_t)->GetDeployTiming())
			{
				e_t++; //���ꂪ�Ȃ��ƊJ�n���ԂɂȂ�܂Ń��[�v��������
				continue;
			}

			//������ӂ͑S��Task->DoTask��
			//TODO:task��list�͑S��substance�����ׂ��H
			//�b�菈���Ƃ���m_substances��task�ɓn�����H
			const wstring targetSub = (*e_t)->GetInstanceName();
			//������SubList��T��
			//���ɂ悭�Ȃ�O(n^2)
			for (auto e_s = m_substances.begin(); e_s != m_substances.end();)
			{
				//�Y���^�X�N�̏ꍇ
				if (targetSub == e_s->first)
				{
					//���s�i�����ԃl�X�g���Ђǂ��c�c�j
					bool result = (*e_t)->Do(e_s->second);
					if ( result )
					{
						//�I��������^�X�N������
						SAFE_DELETE(*e_t);
						e_t = m_taskList.erase(e_t);
					}
					//���̎��_�Ń^�X�N�͕K�����s����Ă���̂ŁA���ȍ~��Sub�ɏ������n�邱�Ƃ͂Ȃ�
					break;
				}
				e_s++;
			}
			//�^�X�N���X�g���I����Ă�̂ɂ܂��������Ƃ��Ă͂����Ȃ��i2�d���[�v��������ɂ������Ă���j
			if ( e_t != m_taskList.end() ) e_t++;
		}

		//�v���C���[�̈ړ�

	}

	void ShooterScene::Draw()
	{
		for (auto e : m_substances)
		{
			e.second->Draw(*m_graphics);
		}
	}


	bool ShooterScene::HasStarted() const
	{
		return m_sceneTimer.HasStarted();
	}
}