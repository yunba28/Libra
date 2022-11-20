# LibraEngine
Siv3Dを用いたゲーム開発用のフレームワークです。<br>
Component指向を採用しており、自由な制作を手助けします。<br>
元となったSiv3Dとは異なったUnityライクな書き方を行うことで、<br>
規模の大きな開発をよりに簡易的に記述することができるようになります。

## Summary
このフレームワークにはWorld、Scene、Actor、Componentの概念があり、<br>
Worldを除いたそれぞれのオブジェクトは所属先となるクラスを持ちます。<br>
ActorであればSceneが該当し、ComponentであればActorが所属先のクラスになります。<br>
基本的な制作工程は<br>
1. 目的の動作を行う自作Componentを作成
1. Sceneのconstruct関数の中でActorを生成
1. 生成したActorにComponentを取り付ける

## Detail
|Folder Name|what|
|:-----|:-----|
|\_Libra|Libraのプロジェクトテンプレートを作成するための編集用フォルダ|
|Libra|Libraのプロジェクトフォルダ|

|File Name|what|
|:-----|:-----|
|README.md|ReadMe|
|REFERENCE.md|Libraのクラスや関数のリファレンスを記載<br>随時更新|

## Demo
任意の文字列を表示するComponentを作成。<br>
~~~cpp
// DefaultScene.cpp

#include "DefaultScene.hpp"

class PrintStringComponent : public Libra::Component
{
public:

  void start()override
  {
    m_text = U"Hello World";
  }
  
  void update()override
  {
    Print << m_text;
  }
  
private:

  String m_text;

};

void DefaultScene::construct()
{
  //PrintStringComponentをアタッチするためのActorを作成
  auto actor = createActor(U"PrintStringActor").lock();
  
  //作成したActorに作ったComponentをアタッチする
  actor->attachComponent<PrintStringComponent>();
}

void DefaultScene::destruct()
{

}

void DefaultScene::start()
{

}

void DefaultScene::update()
{

}

void DefaultScene::draw() const
{

}

~~~
