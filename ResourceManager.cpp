#include"ResourceManager.h"
#include"Inferno\GameElements.h"
#include"ShooterElements.h"
#include<list>

namespace Inferno
{
	//static�����o�ϐ�������
	class Idea; class Texture;
	const Graphics* ResourceManager::m_g;
	std::unordered_map<std::wstring, Idea*> ResourceManager::m_resources;
	std::list<Texture*> ResourceManager::m_textures;

	ResourceManager::~ResourceManager()
	{
		for (auto i = m_resources.begin(); i != m_resources.end();)
		{
			delete i->second;
			i = m_resources.erase(i);
		}

		for (auto i = m_textures.begin(); i != m_textures.end();)
		{
			SAFE_DELETE(*i);
			i = m_textures.erase(i);
		}
	}

	void ResourceManager::SetEnemy(const ResourceHandle& hResource, const wstring& imageName)
	{
		//�e�N�X�`��
		auto newTex = new Texture();
		newTex->LoadImageFile(*m_g, imageName);
		m_textures.push_back(newTex);
		//�C�f�A�̐ݒ�
		auto newResource = new Idea();
		newResource->SetTexture(*newTex);

		//�����蔻��̐ݒ�

		//�Z�b�g
		m_resources.emplace(hResource,newResource);
	}


	void ResourceManager::SetBullet(const ResourceHandle& hResource, const wstring& imageName,
		const Rect& screen, const int margin)
	{
		//�e�N�X�`��
		auto newTex = new Texture();
		newTex->LoadImageFile(*m_g,imageName);
		m_textures.push_back(newTex);
		//�C�f�A�̐ݒ�
		auto newResource = new Idea();
		newResource->SetTexture(*newTex);

		//�����蔻��̐ݒ�

		//��ʊO�̐ݒ�
		newResource->SetActiveRange(screen, margin);

		//�Z�b�g
		m_resources.emplace(hResource,newResource);
	}

	Bullet* ResourceManager::CreateBulletInstance(const std::wstring& resourceName)
	{
		Bullet* pNewInstance = nullptr;
		try
		{
			pNewInstance = new Bullet(*m_resources.at(resourceName));
		}
		catch (std::out_of_range& e)
		{
			//TODO:������Ȃ������ꍇ�C���^�v���^�G���[���o��

		}
		return pNewInstance;
	}

	Enemy* ResourceManager::CreateEnemyInstance(const std::wstring& resourceName)
	{
		Enemy* pNewInstance = nullptr;
		try
		{
			pNewInstance = new Enemy(*m_resources.at(resourceName));
		}
		catch (std::out_of_range& e)
		{
			//TODO:������Ȃ������ꍇ�C���^�v���^�G���[���o��

		}
		return pNewInstance;
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