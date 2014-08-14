/*
ResourceManager.h
ゲームに必要なすべてのリソースを管理する
各リソースへのアクセスは文字列で行う（なので、頻繁に使うような機能はリソースアクセスをしないようにする）
このクラスがリソースの寿命である
シングルトンパターン
*/
#pragma once
#include<list>
#include<string>
#include"Inferno\Graphics.h"
#include"Inferno\Texture.h"
#include"Inferno\Common.h"

namespace Inferno
{
	class Bullet;
	class Enemy;
	class Idea;

class ResourceManager
{
public:
	ResourceManager(const Graphics& g) :m_g(&g){}
	~ResourceManager();
	//Set系は直接パラメーターを設定する。Load系はXMLファイル書いてあるパラを読み込む

	void SetEnemy(const std::wstring& resourceName, const std::wstring& imageFile);
	void SetBullet(const std::wstring& resourceName, const std::wstring& imageFile,
		const Rect& moveLimitScreen, const int moveLimitMargin);
	//void LoadBullet(const wstring name, const wstring imageFile);

	//弾のインスタンスを渡す
	//インスタンスの消去は呼び出し元に任せる
	static Enemy* CreateEnemyInstance(const std::wstring& reasourceName);
	static Bullet* CreateBulletInstance(const std::wstring& resourceName);

private:
	const Graphics* m_g;
	static std::list<Idea*> m_bulletResources;
	static std::list<Idea*> m_enemyResources;
	//ゲームで使用する全てのテクスチャ。テクスチャ管理クラスは作らずに取り敢えずここで全部やる
	static std::list<Texture*> m_textures;

private:
	//既に読み込んでいたら同じ物を読み込む
	//Texture* LoadTexture(const wstring& resourceName);
};
}