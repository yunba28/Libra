#include "DefaultScene.hpp"

// シーンの生成時に呼ばれる
void DefaultScene::construct()
{
	// Componentを取り付けるActorを作成
	auto actor = createActor(U"Cat", Scene::CenterF().xy0()).lock();
	{
		// 矩形と枠を描画するComponentをAttachする
		auto mesh = actor->attachComponent<Libra::RectFrameMesh>().lock();
		mesh->setLocalRect(RectF{ Arg::center = Vec2{0,0},SizeF{200,200} }); // 矩形の大きさを設定
		mesh->setFrameColor(Palette::Goldenrod); // 枠の色を設定
		mesh->setThickness(10.0); // 枠の太さを設定
		mesh->setColor(Palette::White); // 矩形の色を設定
		mesh->setTexture(Texture{U"🐱"_emoji}); // 矩形の貼り付けるテクスチャを設定
	}
}

// シーンの破棄時に呼ばれる
void DefaultScene::destruct()
{
	
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
