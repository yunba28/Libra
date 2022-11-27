#pragma once

#include "Companion.hpp"
#include "Component.hpp"

namespace Libra
{
	enum class Space
	{
		World,
		Local
	};

	class Actor;

	class Transform : public Component
	{
	public:

		using Position_t = Vec3;
		using Rotation_t = Quaternion;
		using Scale_t = Vec3;

		Transform();

		~Transform();

		/*---------------------------------------- 設定関数 -----------------------------------------------*/

		/// @brief Transformの座標・回転・拡縮を設定
		void set(Vec3 _pos, Quaternion _rot, Vec3 _scale, Space _relativeTo = Space::Local);

		void setPos(double _x, double _y, double _z, Space _relativeTo = Space::Local);

		void setPos(Vec3 _pos, Space _relativeTo = Space::Local);

		void setRot(double _roll, double _pitch, double _yaw, Space _relativeTo = Space::Local);

		void setRot(Vec3 _rollPitchYaw, Space _relativeTo = Space::Local);

		void setRot(const Mat4x4& _mat, Space _relativeTo = Space::Local);

		void setRot(Quaternion _rot, Space _relativeTo = Space::Local);

		void setScale(double _x, double _y, double _z, Space _relativeTo = Space::Local);

		void setScale(Vec3 _scale, Space _relativeTo = Space::Local);

		/*---------------------------------------- 取得関数 -----------------------------------------------*/

		[[nodiscard]]
		Vec3 getPos(Space _relativeTo = Space::Local)const;

		[[nodiscard]]
		Quaternion getRot(Space _relativeTo = Space::Local)const;

		[[nodiscard]]
		Vec3 getScale(Space _relativeTo = Space::Local)const;

		[[nodiscard]]
		Vec3 getWorldPos()const;

		[[nodiscard]]
		Quaternion getWorldRot()const;

		[[nodiscard]]
		Vec3 getWorldScale()const;

		/*---------------------------------------- 汎用関数 -----------------------------------------------*/

		void translate(double _x, double _y, double _z);

		void translate(Vec3 _translation);

		void rotate(double _roll, double _pitch, double _yaw);

		void rotate(Vec3 _rollPithcYaw);

		void rotate(const Mat4x4& _mat);

		void rotate(Quaternion _rotation);

		void rotate(Vec3 _axis, double _angle);

		void rotate2D(double _angle);

		void scaling(double _sx, double _sy, double _sz);

		void scaling(Vec3 _scale);

		/// @brief トランスフォームのforward(Z)軸を取得する
		[[nodiscard]]
		Vec3 forward()const;

		/// @brief トランスフォームのup(Y)軸を取得
		[[nodiscard]]
		Vec3 up()const;
		
		/// @brief トランスフォームのright(X)軸を取得
		[[nodiscard]]
		Vec3 right()const;

		/// @brief トランスフォームの回転からオイラー角を取得する
		[[nodiscard]]
		Vec3 eulerAngles()const;

		void lookAt(const Transform& _target);

		void lookAt(Vec3 _worldPos);

		void lookAtDirection(Vec3 _direction);

		/*---------------------------------------- 変換関数 -----------------------------------------------*/

		[[nodiscard]]
		Vec3 localToWorldPos(Vec3 _localPos)const;

		[[nodiscard]]
		Quaternion localToWorldRot(Quaternion _localRot)const;

		[[nodiscard]]
		Vec3 localToWorldScale(Vec3 _localScale)const;

		[[nodiscard]]
		Vec3 worldToLocalPos(Vec3 _worldPos)const;

		[[nodiscard]]
		Quaternion worldToLocalRot(Quaternion _worldRot)const;

		[[nodiscard]]
		Vec3 worldToLocalScale(Vec3 _worldScale)const;

		/*---------------------------------------- その他関数 ---------------------------------------------*/

		/// @brief parentの子になる
		void joinCompanion(Transform& _parent);

		/// @brief 親との連携を切る
		void withdrawCompanion();

		/// @brief transformの更新が行われてから座標・回転・拡縮のいずれかが変更されたか 
		bool isChanged()const noexcept;

	private:

		Position_t m_position;

		Rotation_t m_rotation;

		Scale_t m_scale;

		Companion<Transform> m_companion;

		bool m_isChanged;

	private:

		void destruct()override;

		void start()override {}

		void update()override;
	};
}
