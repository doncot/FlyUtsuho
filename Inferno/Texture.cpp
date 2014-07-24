#include<Texture.h>
#include"Graphics.h"
#include"MyException.h"
#include"Common.h"

using namespace std;

namespace Inferno{
Texture::Texture()
:m_tex(nullptr), m_width(0), m_height(0)
{
}

Texture::~Texture()
{
	SAFE_RELEASE(m_tex);
}

void Texture::LoadImageFile(const Graphics& g, const wstring& filename)
{
	m_texName = filename;
	g.LoadTexture(filename, &m_width, &m_height, &m_tex);
}
/*
void Texture::InitializeFromImage(const wstring& filename, int width, int height, int rowN)
{
	//�t�@�C�������o���Ă����i�f�o�b�O�p�ƃf�o�C�X���X�g�p�j
	m_texName = filename;

	HRESULT hr = D3DXCreateTextureFromFile(
		m_d3dDevice,
		TString(filename), &m_tex
		);
	if (hr == D3DXERR_INVALIDDATA)
	{
		throw FileNotFound(filename);
	}


	// �e�N�X�`������T�[�t�F�C�X���擾
	IDirect3DSurface9 *pSurface;
	m_tex->GetSurfaceLevel(0, &pSurface);   // �T�[�t�F�C�X���擾
	{//�J���Y��ׂ̈̃u���b�N

		// �T�[�t�F�C�X��񂩂�摜�T�C�Y���擾
		D3DSURFACE_DESC SurfaceInfo;
		pSurface->GetDesc(&SurfaceInfo);

		if (width == 0 && height == 0 && (rowN == 0 || rowN == 1))
		{
			//�e�N�X�`���S�̂��g���ꍇ
			m_width = SurfaceInfo.Width;
			m_height = SurfaceInfo.Height;
		}
		else
		{
			//�������s��

			//�������~�����ӕ������̉摜�ƈ�v���Ȃ��Ƃ�
			if (rowN!=0&&width*rowN!=SurfaceInfo.Width)
			{
				TString str;
				str = filename+L"(���̓p�����[�^���s��)";
				throw CreationFailed(str);
			}

			//id=0�͑S�̂��������ʂȗ̈�
			SubRegion sr;
			sr.id = 0; 
			sr.rect.left = sr.rect.top = 0;
			sr.rect.right = SurfaceInfo.Width;
			sr.rect.bottom = SurfaceInfo.Height;
			m_subRegions.push_back(sr);

			//�����e�N�X�`����1����
			for (int i = 0; i < rowN; i++)
			{
				SubRegion sr;
				sr.id = i+1; //����0�͓���Ă���
				sr.rect.top = 0; //������0�
				sr.rect.bottom = SurfaceInfo.Height; //��s�Ȃ̂ō����͌Œ�
				sr.rect.left = width*i; //���[
				sr.rect.right = width*(i + 1); //�E�[�i���ۂ̗̈���+1���Ďw�肷��j
				m_subRegions.push_back(sr);
			}
		}

		m_rect = SetRectFromWH(SurfaceInfo.Width, SurfaceInfo.Height);
	}
	pSurface->Release();


}
*/

unsigned Texture::GetWidth() const
{
	return m_width;
}

unsigned Texture::GetHeight() const
{
	return m_height;
}

LP_TEXTURE Texture::GetTexture() const
{
	return m_tex;
}


Rect Texture::GetSubRegion(const int n) const
{
	return m_subRegions.at(n).rect;
}

Texture& Texture::operator=(const Texture& tex)
{
	//���ȑ�����`�F�b�N
	if (this == &tex) return *this;

	//LP_TEXTURE�̎Q�ƃJ�E���^�𑫂�
	m_tex = tex.m_tex;
	m_tex->AddRef();

	//�X�^�b�N�I�u�W�F�N�g�����̂܂܃R�s�[
	this->m_texName = tex.m_texName;
	this->m_width = tex.m_width;
	this->m_height = tex.m_height;
	this->m_subRegions = tex.m_subRegions;

	return *this;
}

}