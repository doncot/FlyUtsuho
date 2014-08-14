#include"ResourceManager.h"
#include"Inferno\GameElements.h"
#include"ShooterElements.h"
#include<list>

namespace Inferno
{
	//static�����o�ϐ�������
	class Idea; class Texture;
	std::list<Idea*> ResourceManager::m_bulletResources;
	std::list<Idea*> ResourceManager::m_enemyResources;
	std::list<Texture*> ResourceManager::m_textures;

	ResourceManager::~ResourceManager()
	{
		for (auto i = m_bulletResources.begin(); i != m_bulletResources.end();)
		{
			SAFE_DELETE(*i);
			i = m_bulletResources.erase(i);
		}

		for (auto i = m_textures.begin(); i != m_textures.end();)
		{
			SAFE_DELETE(*i);
			i = m_textures.erase(i);
		}
	}

	void ResourceManager::SetEnemy(const wstring& resourceName, const wstring& imageName)
	{
		//�e�N�X�`��
		auto newTex = new Texture();
		newTex->LoadImageFile(*m_g, imageName);
		m_textures.push_back(newTex);
		//�C�f�A�̐ݒ�
		auto newResource = new Idea();
		newResource->SetName(resourceName);
		newResource->SetTexture(*newTex);

		//�����蔻��̐ݒ�

		//�Z�b�g
		m_enemyResources.push_back(newResource);
	}


	void ResourceManager::SetBullet(const wstring& resourceName, const wstring& imageName,
		const Rect& screen, const int margin)
	{
		//�e�N�X�`��
		auto newTex = new Texture();
		newTex->LoadImageFile(*m_g,imageName);
		m_textures.push_back(newTex);
		//�C�f�A�̐ݒ�
		auto newResource = new Idea();
		newResource->SetName(resourceName);
		newResource->SetTexture(*newTex);

		//�����蔻��̐ݒ�

		//��ʊO�̐ݒ�
		newResource->SetActiveRange(screen, margin);

		//�Z�b�g
		m_bulletResources.push_back(newResource);
	}

	Enemy* ResourceManager::CreateEnemyInstance(const wstring& resourceName)
	{
		Enemy* newEnemy = nullptr;
		for (auto e : m_enemyResources)
		{
			if (e->CheckName(resourceName))
			{
				newEnemy = new Enemy(*e);
			}
		}

		//������Ȃ������ꍇnull��Ԃ�
		return newEnemy;
	}

	Bullet* ResourceManager::CreateBulletInstance(const wstring& resourceName)
	{
		Bullet* newBullet = nullptr;
		for (auto e : m_bulletResources)
		{
			if (e->CheckName(resourceName))
			{
				newBullet = new Bullet(*e);
			}
		}
		
		//������Ȃ������ꍇnull��Ԃ�
		return newBullet;
	}

	//private
	/*
	Texture* ResourceManager::LoadTexture(const wstring& resourceName)
	{
		//�e�N�X�`�������Ƀ��[�h���Ă��邩����
		for (auto e : m_textures)
		{
			if (e->GetName() == resourceName)
			{
				return e;
			}
		}

		//�V�����e�N�X�`�������[�h
		auto newTex = new Texture();
		newTex->LoadImageFile(*m_g, resourceName);
		m_textures.push_back(newTex);

		return newTex;
	}
	*/
}