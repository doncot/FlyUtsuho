#include"ResourceManager.h"
#include"Inferno\GameElements.h"
#include"ShooterElements.h"
#include<list>

namespace Inferno
{
	//staticメンバ変数初期化
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
		//テクスチャ
		auto newTex = new Texture();
		newTex->LoadImageFile(*m_g, imageName);
		m_textures.push_back(newTex);
		//イデアの設定
		auto newResource = new Idea();
		newResource->SetName(resourceName);
		newResource->SetTexture(*newTex);

		//当たり判定の設定

		//セット
		m_enemyResources.push_back(newResource);
	}


	void ResourceManager::SetBullet(const wstring& resourceName, const wstring& imageName,
		const Rect& screen, const int margin)
	{
		//テクスチャ
		auto newTex = new Texture();
		newTex->LoadImageFile(*m_g,imageName);
		m_textures.push_back(newTex);
		//イデアの設定
		auto newResource = new Idea();
		newResource->SetName(resourceName);
		newResource->SetTexture(*newTex);

		//当たり判定の設定

		//画面外の設定
		newResource->SetActiveRange(screen, margin);

		//セット
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

		//見つからなかった場合nullを返す
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
		
		//見つからなかった場合nullを返す
		return newBullet;
	}

	//private
	/*
	Texture* ResourceManager::LoadTexture(const wstring& resourceName)
	{
		//テクスチャが既にロードしているか見る
		for (auto e : m_textures)
		{
			if (e->GetName() == resourceName)
			{
				return e;
			}
		}

		//新しいテクスチャをロード
		auto newTex = new Texture();
		newTex->LoadImageFile(*m_g, resourceName);
		m_textures.push_back(newTex);

		return newTex;
	}
	*/
}