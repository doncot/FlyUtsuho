#include"ResourceManager.h"
#include"Inferno\GameElements.h"
#include"ShooterElements.h"
#include<list>

namespace Inferno
{
	//staticメンバ変数初期化
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
		//テクスチャ
		auto newTex = new Texture();
		newTex->LoadImageFile(*m_g, imageName);
		m_textures.push_back(newTex);
		//イデアの設定
		auto newResource = new Idea();
		newResource->SetTexture(*newTex);

		//当たり判定の設定

		//セット
		m_resources.emplace(hResource,newResource);
	}


	void ResourceManager::SetBullet(const ResourceHandle& hResource, const wstring& imageName,
		const Rect& screen, const int margin)
	{
		//テクスチャ
		auto newTex = new Texture();
		newTex->LoadImageFile(*m_g,imageName);
		m_textures.push_back(newTex);
		//イデアの設定
		auto newResource = new Idea();
		newResource->SetTexture(*newTex);

		//当たり判定の設定

		//画面外の設定
		newResource->SetActiveRange(screen, margin);

		//セット
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
			//TODO:見つからなかった場合インタプリタエラーを出す

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
			//TODO:見つからなかった場合インタプリタエラーを出す

		}
		return pNewInstance;
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