/*
�I���W�i���e�N�X�`���i�������R������j�N���X
�����C�u����������͎�����ˑ��ɂ���
*/
#pragma once
#include<d3d9.h> //������͖{���͒ǂ��o������(�����B��)
#include<d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include<string>
#include<vector>
#include"Graphics.h"
#include"TString.h"
#include"MyTypes.h"

namespace Inferno{
//�e�N�X�`��������I�ɋ�؂邽�߂Ɏg��
struct SubRegion
{
	int id; //ID�B0��`��Ŏw�肷��ƑS�̂�\������
	Rect rect;
};

class Texture{
public:
	Texture();
	~Texture();

	//�e�N�X�`�����摜�t�@�C����胍�[�h�B�����͂Ȃ�
	void LoadImageFile(const Graphics& g, const wstring& filename);

	unsigned GetWidth() const;
	unsigned GetHeight() const;
	LP_TEXTURE GetTexture() const;
	Rect GetSubRegion(const int n) const;

private:
	LP_TEXTURE m_tex; //LP_TEXTURE���̂�Itexture�C���^�[�t�F�C�X�ւ̃|�C���^
	TString m_texName;
	unsigned m_width;	//���ǂ���g���񂩂��I�I
	unsigned m_height; 
	vector<SubRegion> m_subRegions; //�e�N�X�`���̒��̕�����������L��
};
}