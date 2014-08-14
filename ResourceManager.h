/*
ResourceManager.h
�Q�[���ɕK�v�Ȃ��ׂẴ��\�[�X���Ǘ�����
�e���\�[�X�ւ̃A�N�Z�X�͕�����ōs���i�Ȃ̂ŁA�p�ɂɎg���悤�ȋ@�\�̓��\�[�X�A�N�Z�X�����Ȃ��悤�ɂ���j
���̃N���X�����\�[�X�̎����ł���
�V���O���g���p�^�[��
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
	//Set�n�͒��ڃp�����[�^�[��ݒ肷��BLoad�n��XML�t�@�C�������Ă���p����ǂݍ���

	void SetEnemy(const std::wstring& resourceName, const std::wstring& imageFile);
	void SetBullet(const std::wstring& resourceName, const std::wstring& imageFile,
		const Rect& moveLimitScreen, const int moveLimitMargin);
	//void LoadBullet(const wstring name, const wstring imageFile);

	//�e�̃C���X�^���X��n��
	//�C���X�^���X�̏����͌Ăяo�����ɔC����
	static Enemy* CreateEnemyInstance(const std::wstring& reasourceName);
	static Bullet* CreateBulletInstance(const std::wstring& resourceName);

private:
	const Graphics* m_g;
	static std::list<Idea*> m_bulletResources;
	static std::list<Idea*> m_enemyResources;
	//�Q�[���Ŏg�p����S�Ẵe�N�X�`���B�e�N�X�`���Ǘ��N���X�͍�炸�Ɏ�芸���������őS�����
	static std::list<Texture*> m_textures;

private:
	//���ɓǂݍ���ł����瓯������ǂݍ���
	//Texture* LoadTexture(const wstring& resourceName);
};
}