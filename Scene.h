/*
アクションゲームなどでシーンを表すクラス
*/
#pragma once

namespace Inferno
{
class Scene
{
public:
	void Create();
	void Destroy();

	//シーンの本体。1フレームごとにgameループより呼び出し。
	//返り値：次のシーンを返す（シーンIDで。あとで定義）
	bool SceneLoop();
//	bool AddEvent();

};

}