#include "DefaultScene.hpp"

// シーンの生成時に呼ばれる
void DefaultScene::construct()
{
	
}

// シーンの破棄時に呼ばれる
void DefaultScene::destruct()
{
	//ActorやComponentは自動的に破棄されるので
	//明示的なリソースの解放等をここで行う
	//がリソースはほとんどSiv3Dが解放してくれるのであまり出番はない
}

// シーンが最初の更新を行う際に呼ばれる
void DefaultScene::start()
{

}

// 毎フレーム呼びだされる
void DefaultScene::update()
{

}

// 毎フレーム呼びだされる
void DefaultScene::draw() const
{

}
