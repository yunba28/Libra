#pragma once

#include "../ProjectCommons.hpp"

class DefaultScene : public Libra::SceneBase
{
public:

	// シーンの生成時に呼ばれる
	void construct()override;

	// シーンの破棄時に呼ばれる
	void destruct()override;

	// シーンが最初の更新を行う際に呼ばれる
	void start()override;

	// 毎フレーム呼びだされる
	void update()override;

	// 毎フレーム呼びだされる
	void draw()const override;

};
