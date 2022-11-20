# Reference
全てのクラス、関数はLibra名前空間に属しています。<br>
また、_internalから始まる関数およびInternal名前空間に属するものは基本的にユーザーが使うことはありません。

## Classes

### Object
本フレームワークで使われるActor、Component、SceneBaseの基底クラスになります。
|Public Function|Summary|
|:------------------|:------------------------------|
|virtual void construct()|Objectの初期化関数。<br>Objectの構築時に呼び出される仮想関数です。|
|virtual void destruct()|Objectの終了関数です。<br>isPendingDestroyがtrueになった際に呼び出されます。|
|virtual void start()|Objectの最初の更新で呼ばれます。|
|virtual void update()|毎フレーム呼びだされる関数。|
|virtual void fixedUpdate()|１秒間に決まった回数だけ呼ばれます。<br>construct()等の中でapplyPhysics()を呼ぶことで使えます。|
|virtual void draw()const|毎フレーム呼びだされる描画関数。<br>construct()等の中でapplyPlaneRendering()もしくはapplySpaceRendering()を呼ぶことで使えます。|
|virtual void onActive()|Objectが有効になった際に呼ばれます。|
|virtual void onInactive()|Objectが無効になった際に呼ばれます。|
|virtual void onCollisionEnter(const Collision&)|Objectが衝突した時に呼ばれます。|
|virtual void onCollisionStay(const Collision&)|Objectが衝突している間は常に呼ばれます。|
|virtual void onCollisionExit(const Collision&)|Objectの衝突が終わった時に呼ばれます。|
|virtual void setRenderingpriority(const uint64)noexcept|Objectの描画優先度を設定します。<br>値が大きいほど画面の全面に描画されますが、3Dでは効果はありません。|
|virtual uint64 getRenderingpriority()const noexcept|Objectの描画優先度を取得します。|
|void setActive(bool)|Objectの有効・無効を設定します。<br>falseの場合、全てのメッセージ関数が無視されます。|
|bool isActive()const noexcept|Objectが有効かどうかをbool値で返します。|
|bool isStarted()const noexcept|Objectが最初の更新を行ったかどうかをbool値で返します。|
|bool isPendingDestroy()const noexcept|Objectが破棄されるのを待っているかどうかをbool値で返します。<br>これがtrueの場合、isActive()は常にfalseを返します。|
|void setEnableUpdate(bool)noexcept|falseを設定するとupdate関数が無視されます。<br>デフォルトはtrueです。|
|void isEnabledUpdate()const noexcept|update関数が有効かどうか、bool値で返します。|
|void setEnableFixedUpdate(bool)noexcept|falseを設定するとfixedUpdate関数が無視されます。<br>デフォルトはfalseです。|
|void isEnabledFixedUpdate()const noexcept|fixedUpdate関数が有効かどうか、bool値で返します。|
|void setEnableDra(bool)noexcept|falseを設定するとdraw関数が無視されます。<br>デフォルトはfalseです。|
|void isEnabledDraw()const noexcept|draw関数が有効かどうか、bool値で返します。|

|Protected Function|Summary|
|:------------------|:------------------------------|
|void stateSet(const uint64, const bool)noexcept|[9,63]の範囲でObjectが管理しているビットのフラグを設定します。<br>[0,8]の範囲はObjectによって既に定義されているので使えません。|
|void stateOn(const uint64)noexcept|[9,63]の範囲でObjectが管理しているビットのフラグを有効にします。<br>[0,8]の範囲はObjectによって既に定義されているので使えません。|
|void stateOff(const uint64)noexcept|[9,63]の範囲でObjectが管理しているビットのフラグを無効にします。<br>[0,8]の範囲はObjectによって既に定義されているので使えません。|
|bool stateTest(const uint64)const noexcept|Objectが管理しているi番目のビットが有効かどうかをbool値で返します。|
|bool stateNone(const uint64)const noexcept|Objectが管理しているi番目のビットが無効かどうかをbool値で返します。|
|bool stateAll(const uint64)const noexcept|指定したマスクビットがすべて有効になっているかをbool値で返します。|
|bool stateAny(const uint64)const noexcept|指定したマスクビットのいずれかが有効になっているかをbool値で返します。|
|virtual void applyPlaneRendering()|Objectが2D描画できるように設定します。|
|virtual void applySpaceRendering()|Objectが3D描画できるように設定します。|
|virtual void applyPhysics()|ObjectがfixedUpdateの更新を行えるように設定します。|

|Static Function|Summary|
|:------------------|:------------------------------|
|void Destroy(Object*)noexcept|Objectを破棄します。|

### Actor
シーンで動かすことのできる基本的なクラスです。Componentをアタッチすることによってさまざまな働きをさせることができます。
|Public Function|Summary|
|:------------------|:------------------------------|
|ObjHandle\<SceneBase\> getScene()const noexcept|Actorが所属しているシーンの参照を得ます。|
|void setupTransform(const WeakObj\<Transform\>&)|ActorにTransformを設定します。<br>設定したTransformはアタッチした全てのComponentから参照されます。|
|ObjHandle\<Transform\> getTransform()const noexcept|Actorが所有しているTransformの参照を得ます。<br>createEmptyActorで生成された場合、デフォルトで空のObjHandleが返ります。|
|void setName(const String&)|Actorの名前を設定します。<br>既に同じ名前が存在する場合はName \#Nという表記になります。|
|const String& getName()const|Actorの名前を取得します。|
|bool compareName(const String&)const noexcept|Actorの名前と文字列を比較します。<br>文字列が名前と一致する場合、trueが返ります。|
|void setTag(const String&)|Actorのタグを設定します。|
|const String& getTag()const|Actorのタグを取得します。|
|bool compareTag(const String&)const noexcept|Actorのタグと文字列を比較します。<br>文字列がタグと一致する場合、trueが返ります。|
|template\<class ComponentT\><br>WeakObj\<ComponentT\> attachComponent()|ActorにComponentをアタッチします。|
|template\<class ComponentT\><br>WeakObj\<ComponentT\> getComponent()const|Actorが持っているComponentを取得します。<br>持っていない場合は空のWeakObjが返ります。|
|Array\<WeakObj\<ComponentT\>\> getComponentAll()const|Actorが持っているすべてのComponentを取得します。|
|template\<class ComponentT\><br>void detachComponent()|Actorが持つComponentをディタッチします。|
|void detachComponentAll()|Actorが持つすべてのComponentをディタッチします。|

|Protected Function|Summary|
|:------------------|:------------------------------|
|void applyPlaneRendering() final|Actorが2D描画できるように設定します。|
|void applySpaceRendering() final|Actorが3D描画できるように設定します。|
|void applyPhysics() final|ActorがfixedUpdateの更新を行えるように設定します。|

|Static Function|Summary|
|:------------------|:------------------------------|
|void Destroy(const WeakObj\<Actor\>&)|Actorを破棄します。|
|void Destroy(ObjHandle\<Actor\>&&)|Actorを破棄します。|

### Component
Actorにアタッチされる基本的なクラス。<br>
update()とstart()が純粋仮想関数のインターフェースクラスのため、必ず継承してください。

|Public Function|Summary|
|:------------------|:------------------------------|
|ObjHandle\<Actor\> getActor()const noexcept|Componentが所属するActorの参照を得ます。|
|ObjHandle\<Transform\> getTransform()const noexcept|ActorのTransformの参照を得ます。<br>ActorがTransformを持っていない場合、空のObjHandleが返ります。|
|bool compareName(const String&)const noexcept|Actorの名前と文字列を比較します。<br>文字列が名前と一致する場合、trueが返ります。|
|bool compareTag(const String&)const noexcept|Actorのタグと文字列を比較します。<br>文字列がタグと一致する場合、trueが返ります。|

|Protected Function|Summary|
|:------------------|:------------------------------|
|void applyPlaneRendering() final|Componentが2D描画できるように設定します。|
|void applySpaceRendering() final|Componentが3D描画できるように設定します。|
|void applyPhysics() final|ComponentがfixedUpdateの更新を行えるように設定します。|

|Static Function|Summary|
|:------------------|:------------------------------|
|void Destroy(const WeakObj\<Component\>&)|Componentを破棄します。|
|void Destroy(ObjHandle\<Component\>&&)|Componentを破棄します。|

