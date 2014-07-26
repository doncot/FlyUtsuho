/*
�Q�[���v�f�͑S�Ă����ɒu��
Idea:�C�f�A�B�{����\���B�ȉ��̃N���X�͑S�ăC�f�A������
Substance:���݁B�u������v�G�B��{�`
Rigidbody:���́B�u�����Փˁv������Substance�B
�S�ẴQ�[���v�f�͎��g�̒��S�����_���W�ɂȂ�
��Windows�ˑ�
*/
#pragma once
#include<Windows.h>
#include"Graphics.h"
#include<Texture.h>
#include"MyTypes.h"
#include"Common.h"
#include<Timer.h>

namespace Inferno
{
//�Q�[���v�f�̏�ԁi�A�N�V�����Q�[���p�Ȃ̂Ōp����ɖ{������ׂ�
enum GEAttribute
{
	UserLock = 1, //���[�U�[����̑�������b�N
	Draw = 2, //�`�悷�邩�ۂ�
	NoHit = 4, //�Փ˔�������邩�ۂ�
};

class Idea
{
public:
	Idea();
	~Idea();

	int GetWidth() const;
	int GetHeight() const;
	Vec2<int> GetDrawBase() const;
	//�e�N�X�`���̐ݒ��n�����Ƃ��l���Ẵe�N�X�`���N���X�n������
	///�璷�Ȃ̂Ńt�@�C���������ɂ��������悢
	void SetTexture(const Texture& tex);
	const Texture* GetTexture() const;

private:
	const Texture* m_tex;

	Vec2<int> m_drawBase; //�`���i�����ł͊G�̒��S�j
	int m_width;
	int m_height;

private:
	//�R�s�[�R���X�g���N�^��������Z�q�͕s�K�v
	Idea(const Idea&) = delete;
	Idea& operator=(const Idea&) = delete;
};

//�Q�[���̌X�v�f
//�񎟌����W�œ��삷�邱�ƑO��
class Substance
{
public:
	Substance();
	Substance(const Idea& idea);
	virtual ~Substance();

	void SetIdea(const Idea& idea);

	//��΍��W�w��ړ�
	virtual void AMove(const int x, const int y);
	//��΍��W�w��ړ�
	virtual void AMove(const Vec2<int>& c);
	//���ΕW�w��ړ�
	virtual void RMove(const int x, const int y);
	virtual void RMove(const Vec2<int>& c);
	//������W�ňʒu���w��
	void SetPosofULCorner(const int x, const int y);
	//���v����ɉ�]�i�x���w��j
	void Rotate(const int degree);
	void Rotate(const float degree);

	Vec2<int> GetPosition() const;

	//�ʒu���W�ƃT�C�Y���猻�ݗ̈�����߂�
	Rect GetRegion() const;

	//�����i�`��\�Ȃǁj���w��
	void SetAttribute(GEAttribute attr, bool value);
	bool CheckAttribute(GEAttribute attr) const;
	
	void SetAlpha(const int alpha);

	void SetID(const int id);
	int GetID() const;

	//DispatchMessage(GMessageQueue)
	//InterpretMessage(GMessageQueue);

	virtual void Update();

	void Draw(const Graphics& g) const;

	//�R�s�[�R���X�g���N�^��������Z�q�K�v
	Substance& operator=(const Substance& s);

protected:
	int m_id;
	const Idea* m_idea;
	Vec2<int> m_pos; //���ݍ��W
	float m_angle; //�x���@�ŉ�]���
	int m_alpha;
	char m_attribute; //�����i�`�摮���Ȃǁj���w��
};

//�P�������Ȃ��v�f�̏ꍇ�A���������C�f�A������̂����̂��ʓ|�Ȃ̂ł��������g��
//���g�̓C�f�A������Ɏ����̂ł���B
class Instant: public Substance
{
typedef Substance Base;
public:
	~Instant()
	{
		SAFE_DELETE(m_idea);
		SAFE_DELETE(m_tex);
	}

	void Initialize()
	{
		m_tex = new Texture();
		m_idea = new Idea();
		SetAttribute(GEAttribute::Draw, true);
	}
	void LoadTextureFromFile(const Graphics& g, const wstring& str)
	{
		m_tex->LoadImageFile(g, str);
		m_idea->SetTexture(*m_tex);
		Base::m_idea = m_idea;
	}

private:
	//�����ŏ������ޕK�v������̂ŁAconst��substace��idea�͎g���Ȃ�
	Idea* m_idea;
	Texture* m_tex;

};

}