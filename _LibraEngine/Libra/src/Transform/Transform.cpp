#include "../../include/Transform.hpp"


namespace Libra
{
	float MGet(const Mat4x4& m, int32 y, int32 x)
	{
		if (InRange(y, 0, 3) and InRange(x, 0, 3))
		{
			return m.value.r[y].m128_f32[x];
		}

		Throwing<Exceptions::OutOfRange>("out of range ");
	}

	void MSet(Mat4x4& m, int32 y, int32 x, float val)
	{
		if (InRange(y, 0, 3) and InRange(x, 0, 3))
		{
			m.value.r[y].m128_f32[x] = val;
		}

		Throwing<Exceptions::OutOfRange>("out of range ");
	}

	Quaternion QLookAt(const Mat4x4& m)
	{
		float elem[4]{};
		elem[0] = MGet(m, 0, 0) - MGet(m, 1, 1) - MGet(m, 2, 2) + 1.0f;
		elem[1] = -MGet(m, 0, 0) + MGet(m, 1, 1) - MGet(m, 2, 2) + 1.0f;
		elem[2] = -MGet(m, 0, 0) - MGet(m, 1, 1) + MGet(m, 2, 2) + 1.0f;
		elem[3] = MGet(m, 0, 0) + MGet(m, 1, 1) + MGet(m, 2, 2) + 1.0f;

		int32 biggestIdx = 0;
		for (int32 i = 0; i < std::size(elem); i++)
		{
			if (elem[i] > elem[biggestIdx])
			{
				biggestIdx = i;
			}
		}

		if (elem[biggestIdx] < 0)
		{
			return Quaternion{};
		}

		float q[4]{};
		float v = Sqrt(elem[biggestIdx]) * 0.5f;
		q[biggestIdx] = v;
		float mult = 0.25f / v;

		switch (biggestIdx)
		{
		case 0:
			q[1] = (MGet(m, 1, 0) + MGet(m, 0, 1)) * mult;
			q[2] = (MGet(m, 0, 2) + MGet(m, 2, 0)) * mult;
			q[3] = (MGet(m, 2, 1) - MGet(m, 1, 2)) * mult;
			break;
		case 1:
			q[0] = (MGet(m, 1, 0) + MGet(m, 0, 1)) * mult;
			q[2] = (MGet(m, 2, 1) + MGet(m, 1, 2)) * mult;
			q[3] = (MGet(m, 0, 2) - MGet(m, 2, 0)) * mult;
			break;
		case 2:
			q[0] = (MGet(m, 0, 2) + MGet(m, 2, 0)) * mult;
			q[1] = (MGet(m, 2, 1) + MGet(m, 1, 2)) * mult;
			q[3] = (MGet(m, 1, 0) - MGet(m, 0, 1)) * mult;
			break;
		case 3:
			q[0] = (MGet(m, 2, 1) - MGet(m, 1, 2)) * mult;
			q[1] = (MGet(m, 0, 2) - MGet(m, 2, 0)) * mult;
			q[2] = (MGet(m, 1, 0) - MGet(m, 0, 1)) * mult;
			break;
		}

		return Quaternion(q[0], q[1], q[2], q[3]);
	}
}

namespace Libra
{
	Transform::Transform()
		: m_position(0, 0, 0)
		, m_rotation(Rotation_t::Identity())
		, m_scale(1, 1, 1)
		, m_companion(*this)
		, m_isChanged(false)
	{
	}

	Transform::~Transform()
	{

	}

	void Transform::set(Vec3 _pos, Quaternion _rot, Vec3 _scale, Space _relativeTo)
	{
		if (_relativeTo == Space::Local)
		{
			m_position = _pos;
			m_rotation = _rot;
			m_scale = _scale;
		}
		else
		{
			m_position = worldToLocalPos(_pos);
			m_rotation = worldToLocalRot(_rot);
			m_scale = worldToLocalScale(_scale);
		}

		m_isChanged = true;
	}

	void Transform::setPos(double _x, double _y, double _z, Space _relativeTo)
	{
		if (_relativeTo == Space::Local)
		{
			m_position.set(_x, _y, _z);
		}
		else
		{
			m_position = worldToLocalPos(Vec3{ _x,_y,_z });
		}

		m_isChanged = true;
	}

	void Transform::setPos(Vec3 _pos, Space _relativeTo)
	{
		if (_relativeTo == Space::Local)
		{
			m_position = _pos;
		}
		else
		{
			m_position = worldToLocalPos(_pos);
		}

		m_isChanged = true;
	}

	void Transform::setRot(double _roll, double _pitch, double _yaw, Space _relativeTo)
	{
		if (_relativeTo == Space::Local)
		{
			m_rotation = Rotation_t::RollPitchYaw(_roll, _pitch, _yaw);
		}
		else
		{
			m_rotation = worldToLocalRot(Rotation_t::RollPitchYaw(_roll, _pitch, _yaw));
		}

		m_isChanged = true;
	}

	void Transform::setRot(Vec3 _rollPitchYaw, Space _relativeTo)
	{
		if (_relativeTo == Space::Local)
		{
			m_rotation = Rotation_t::RollPitchYaw(_rollPitchYaw.x, _rollPitchYaw.y, _rollPitchYaw.z);
		}
		else
		{
			m_rotation = worldToLocalRot(Rotation_t::RollPitchYaw(_rollPitchYaw.x, _rollPitchYaw.y, _rollPitchYaw.z));
		}

		m_isChanged = true;
	}

	void Transform::setRot(const Mat4x4& _mat, Space _relativeTo)
	{
		if (_relativeTo == Space::Local)
		{
			m_rotation = Rotation_t{ _mat };
		}
		else
		{
			m_rotation = worldToLocalRot(Rotation_t{ _mat });
		}

		m_isChanged = true;
	}

	void Transform::setRot(Quaternion _rot, Space _relativeTo)
	{
		if (_relativeTo == Space::Local)
		{
			m_rotation = _rot;
		}
		else
		{
			m_rotation = worldToLocalRot(_rot);
		}

		m_isChanged = true;
	}

	void Transform::setScale(double _x, double _y, double _z, Space _relativeTo)
	{
		if (_relativeTo == Space::Local)
		{
			m_scale.set(_x, _y, _z);
		}
		else
		{
			m_scale = worldToLocalScale(Vec3{ _x,_y,_z });
		}

		m_isChanged = true;
	}

	void Transform::setScale(Vec3 _scale, Space _relativeTo)
	{
		if (_relativeTo == Space::Local)
		{
			m_scale = _scale;
		}
		else
		{
			m_scale = worldToLocalScale(_scale);
		}

		m_isChanged = true;
	}

	Vec3 Transform::getPos(Space _relativeTo) const
	{
		if (_relativeTo == Space::Local)
		{
			return m_position;
		}

		return getWorldPos();
	}

	Quaternion Transform::getRot(Space _relativeTo) const
	{
		if (_relativeTo == Space::Local)
		{
			return m_rotation;
		}

		return getWorldRot();
	}

	Vec3 Transform::getScale(Space _relativeTo) const
	{
		if (_relativeTo == Space::Local)
		{
			return m_scale;
		}

		return getWorldScale();
	}

	Vec3 Transform::getWorldPos() const
	{
		return localToWorldPos(m_position);
	}

	Quaternion Transform::getWorldRot() const
	{
		return localToWorldRot(m_rotation);
	}

	Vec3 Transform::getWorldScale() const
	{
		return localToWorldScale(m_scale);
	}

	void Transform::translate(double _x, double _y, double _z)
	{
		m_position.moveBy(_x, _y, _z);

		m_isChanged = true;
	}

	void Transform::translate(Vec3 _translation)
	{
		m_position.moveBy(_translation);

		m_isChanged = true;
	}

	void Transform::rotate(double _roll, double _pitch, double _yaw)
	{
		m_rotation *= Rotation_t::RollPitchYaw(_roll, _pitch, _yaw);

		m_isChanged = true;
	}

	void Transform::rotate(Vec3 _rollPithcYaw)
	{
		m_rotation *= Rotation_t::RollPitchYaw(_rollPithcYaw.x, _rollPithcYaw.y, _rollPithcYaw.z);

		m_isChanged = true;
	}

	void Transform::rotate(const Mat4x4& _mat)
	{
		m_rotation *= Rotation_t{ _mat };

		m_isChanged = true;
	}

	void Transform::rotate(Quaternion _rotation)
	{
		m_rotation *= _rotation;

		m_isChanged = true;
	}

	void Transform::rotate(Vec3 _axis, double _angle)
	{
		m_rotation *= Quaternion::RotationAxis(_axis, _angle);

		m_isChanged = true;
	}

	void Transform::rotate2D(double _angle)
	{
		rotate(Mat4x4::RotateZ(_angle));
	}

	void Transform::scaling(double _sx, double _sy, double _sz)
	{
		m_scale *= Vec3{ _sx,_sy,_sz };

		m_isChanged = true;
	}

	void Transform::scaling(Vec3 _scale)
	{
		m_scale *= _scale;

		m_isChanged = true;
	}

	Vec3 Transform::forward() const
	{
		return getWorldRot() * Vec3::Forward();
	}

	Vec3 Transform::up() const
	{
		return getWorldRot() * Vec3::Up();
	}

	Vec3 Transform::right() const
	{
		return getWorldRot() * Vec3::Right();
	}

	Vec3 Transform::eulerAngles() const
	{
		Mat4x4 m{ getWorldRot() };
		Vec3 euler{ 0,0,0 };
		euler.x = Atan2(MGet(m, 2, 1), MGet(m, 2, 2));
		euler.y = Asin(MGet(m, 2, 0));
		euler.z = -Atan2(MGet(m, 1, 0), MGet(m, 0, 0));
		return euler;
	}

	void Transform::lookAt(const Transform& _target)
	{
		lookAt(_target.getWorldPos());
	}

	void Transform::lookAt(Vec3 _worldPos)
	{
		Float3 z = (_worldPos - getWorldPos()).normalized();
		Float3 x = Vec3::Up().cross(z).normalized();
		Float3 y = z.cross(x).normalized();

		Mat4x4 m = Mat4x4::Set(
			x.x, y.x, z.x, 0.f,
			x.y, y.y, z.y, 0.f,
			x.z, y.z, z.z, 0.f,
			0.f, 0.f, 0.f, 1.f
		);

		m_rotation = QLookAt(m);
	}

	void Transform::lookAtDirection(Vec3 _direction)
	{
		Float3 z = _direction.normalized();
		Float3 x = Vec3::Up().cross(z).normalized();
		Float3 y = z.cross(x).normalized();

		Mat4x4 m = Mat4x4::Set(
			x.x, y.x, z.x, 0.f,
			x.y, y.y, z.y, 0.f,
			x.z, y.z, z.z, 0.f,
			0.f, 0.f, 0.f, 1.f
		);

		m_rotation = QLookAt(m);
	}

	Vec3 Transform::localToWorldPos(Vec3 _localPos) const
	{
		Companion<Transform>* parent = m_companion.getParent();

		if (not parent)
		{
			return _localPos;
		}

		Quaternion rot{ m_rotation };

		Mat4x4 m{ rot };

		_localPos = m.transformPoint(_localPos);

		while (parent)
		{
			m = Mat4x4{ parent->self().m_rotation };

			_localPos = m.transformPoint(_localPos);

			_localPos += parent->self().m_position;

			parent = parent->getParent();
		}

		return _localPos;
	}

	Quaternion Transform::localToWorldRot(Quaternion _localRot) const
	{
		Companion<Transform>* parent = m_companion.getParent();

		if (not parent)
		{
			return _localRot;
		}

		do
		{
			//親のローカル回転を加算
			_localRot *= parent->self().m_rotation;

			parent = parent->getParent();

		} while (parent);

		return _localRot;
	}

	Vec3 Transform::localToWorldScale(Vec3 _localScale) const
	{
		Companion<Transform>* parent = m_companion.getParent();

		if (not parent)
		{
			return _localScale;
		}

		do
		{
			//親のローカルスケールを乗算
			_localScale *= parent->self().m_scale;

			parent = parent->getParent();

		} while (parent);

		return _localScale;
	}

	Vec3 Transform::worldToLocalPos(Vec3 _worldPos) const
	{
		Companion<Transform>* parent = m_companion.getParent();

		if (not parent)
		{
			return _worldPos;
		}

		//親のワールド座標
		Vec3 parentWorldPos = parent->self().getWorldPos();

		return _worldPos - parentWorldPos;
	}

	Quaternion Transform::worldToLocalRot(Quaternion _worldRot) const
	{
		Companion<Transform>* parent = m_companion.getParent();

		if (not parent)
		{
			return _worldRot;
		}

		//親のワールド回転
		Quaternion parentWorldRot = parent->self().getWorldRot();

		return _worldRot * parentWorldRot.conjugate();
	}

	Vec3 Transform::worldToLocalScale(Vec3 _worldScale) const
	{
		Companion<Transform>* parent = m_companion.getParent();

		if (not parent)
		{
			return _worldScale;
		}

		//親のワールド軸スケール
		Vec3 parentWorldScale = parent->self().getWorldScale();

		//ゼロ除算の防止
		if (parentWorldScale.x == 0.0 or
			parentWorldScale.y == 0.0 or
			parentWorldScale.z == 0.0)
		{
			return Vec3{ 0,0,0 };
		}

		return _worldScale / parentWorldScale;
	}

	void Transform::joinCompanion(Transform& _parent)
	{
		m_companion.join(_parent.m_companion);

		m_position = worldToLocalPos(m_position);
		m_rotation = worldToLocalRot(m_rotation);
		m_scale = worldToLocalScale(m_scale);
	}

	void Transform::withdrawCompanion()
	{
		m_position = localToWorldPos(m_position);
		m_rotation = localToWorldRot(m_rotation);
		m_scale = localToWorldScale(m_scale);

		m_companion.withdraw();
	}

	bool Transform::isChanged() const noexcept
	{
		return m_isChanged;
	}

	void Transform::destruct()
	{
		m_companion.withdraw();
	}

	void Transform::update()
	{
		m_isChanged = false;
	}
}
