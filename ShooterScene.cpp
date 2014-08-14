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
		//TODO:Idea�ɏd�����������甲��

		//�V����Base��o�^
		m_baseList.push_back(newBase);


		//�Y����id������Substance��EnemyBase���Z�b�g
		for (auto e : m_subList)
		{
			//id����v������
			if (e->GetID() == id)
			{
				e->SetIdea(newBase->m_idea);
			}
		}
	}

	void ShooterScene::RegisterDeploy(const int id, const Millisec deployTime, const Vec2<int> deployCor)
	{
		//Deploy�^�X�N�𐶐�
		auto newTask = new DeployTask(id);
		newTask->SetDeployTiming(deployTime);
		newTask->SetDeployCor(deployCor);

		m_taskList.push_back(newTask);
	}

	void ShooterScene::RegisterMove(const int id, const Millisec delay, const Millisec dur,
		const Vec2<int> from, const Vec2<int> to, const Animation::TransitType ttype)
	{
		//�ړ��^�X�N��o�^
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
				//�������Ă�����
				//�̗͂Ƃ����炷�̂��Ȗ{����
				//�i�����܂܂��ᎀ�S���̃A�j�����Ȃ��ȁA�ǂ��`���悤�j
				SAFE_DELETE(*sub_i);
				sub_i = m_subList.erase(sub_i);

				//��̑w�ŏ������邽�߂ɓ�����������m�点��
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
			//�b�菈���Ƃ���m_subList��task�ɓn�����H
			int targetSub = (*e_t)->GetID();
			//������SubList��T��
			//���ɂ悭�Ȃ�O(n^2)
			for (auto e_s = m_subList.begin(); e_s != m_subList.end();)
			{
				//�Y���^�X�N�̏ꍇ
				if (targetSub == (*e_s)->GetID())
				{
					//���s�i�����ԃl�X�g���Ђǂ��c�c�j
					bool result = (*e_t)->Do((*e_s));
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