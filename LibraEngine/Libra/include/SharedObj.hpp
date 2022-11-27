#pragma once

#include "Common.hpp"

namespace Libra
{
	/// @brief SharedObjの弱参照
	/// @tparam Type Objectを継承するクラス
	template<class Type>
	class WeakObj;

	/// @brief WeakObjから一時的な参照を得るためのプロキシクラス
	template<class Type>
	class ObjHandle;

	/// @brief 共有型のスマートポインタ
	/// @tparam Type 管理する型
	template<class Type>
	class SharedObj final
	{
	public:

		SharedObj();

		SharedObj(std::nullptr_t);

		template<class Other>
		explicit SharedObj(Other* _other)requires(Concepts::BaseOf<Type, Other>);

		template<class Other>
		explicit SharedObj(const SharedPtr<Other>& _other)requires(Concepts::BaseOf<Type, Other>);

		SharedObj(const SharedObj&) = default;

		template<class Other>
		SharedObj(const SharedObj<Other>& _other)requires(Concepts::BaseOf<Type, Other>);

		SharedObj(SharedObj&&) = default;

		template<class Other>
		SharedObj(SharedObj<Other>&& _other)requires(Concepts::BaseOf<Type, Other>);

		template<class Other>
		SharedObj(const WeakObj<Other>& _ref)requires(Concepts::Lineage<Type, Other>);

		~SharedObj() = default;

		SharedObj& operator=(const SharedObj&) = default;

		template<class Other>
		SharedObj& operator=(const SharedObj<Other>& _other)requires(Concepts::BaseOf<Type, Other>);

		SharedObj& operator=(SharedObj&&) = default;

		template<class Other>
		SharedObj& operator=(SharedObj<Other>&& _other)requires(Concepts::BaseOf<Type, Other>);

		template<class Other>
		SharedObj& operator=(const WeakObj<Other>& _ref)requires(Concepts::Lineage<Type, Other>);

		SharedObj& operator=(std::nullptr_t);

		[[nodiscard]] Type& operator*();

		[[nodiscard]] const Type& operator*()const;

		[[nodiscard]] Type* operator->()const;

		[[nodiscard]] explicit operator bool()const noexcept;

		/// @brief Other型のObjectにキャストしたSharedObjを生成する
		/// @tparam Other Typeを継承しているクラスあるいはTypeに継承されているクラス
		template<class Other>
		[[nodiscard]] SharedObj<Other> castTo()const requires(Concepts::Lineage<Type, Other>);

		/// @brief SharedObjからWeakObjを生成する
		[[nodiscard]] WeakObj<Type> makeRef()const noexcept;

		template<class Other>
		[[nodiscard]] bool operator==(const SharedObj<Other>& _other)const;

		template<class Other>
		[[nodiscard]] bool operator==(const WeakObj<Other>& _other)const;

		template<class Other>
		[[nodiscard]] bool operator==(const ObjHandle<Other>& _other)const;

		[[nodiscard]] bool operator==(std::nullptr_t)const;

		template<class Other>
		[[nodiscard]] bool operator!=(const SharedObj<Other>& _other)const;

		template<class Other>
		[[nodiscard]] bool operator!=(const WeakObj<Other>& _other)const;

		template<class Other>
		[[nodiscard]] bool operator!=(const ObjHandle<Other>& _other)const;

		[[nodiscard]] bool operator!=(std::nullptr_t)const;

	private:

		SharedPtr<Type> m_object;

		template<class Other>
		friend class SharedObj;

		template<class Other>
		friend class WeakObj;

		template<class Other>
		friend class ObjHandle;

	};

	template<class Type>
	class ObjHandle final
	{
	public:

		ObjHandle() = delete;
		ObjHandle(const ObjHandle&) = delete;
		ObjHandle& operator=(const ObjHandle&) = delete;
		ObjHandle(ObjHandle&&) = default;
		ObjHandle& operator=(ObjHandle&&) = default;
		~ObjHandle() = default;

		Type& operator*() { return *m_sp; }
		Type* operator->()const noexcept { return m_sp.get(); }

		explicit operator bool()const noexcept { return m_sp.operator bool(); }

		template<class Other>
		[[nodiscard]] bool operator==(const SharedObj<Other>& _other)const { return m_sp == _other.m_object; }

		template<class Other>
		[[nodiscard]] bool operator==(const WeakObj<Other>& _other)const { return m_sp == _other.m_ref.lock(); }

		template<class Other>
		[[nodiscard]] bool operator==(const ObjHandle<Other>& _other)const { return m_sp == _other.m_sp; };

		[[nodiscard]] bool operator==(std::nullptr_t)const { return m_sp == nullptr; };

		template<class Other>
		[[nodiscard]] bool operator!=(const SharedObj<Other>& _other)const { return m_sp != _other.m_object; }

		template<class Other>
		[[nodiscard]] bool operator!=(const WeakObj<Other>& _other)const { return m_sp != _other.m_ref.lock(); }

		template<class Other>
		[[nodiscard]] bool operator!=(const ObjHandle<Other>& _other)const { return m_sp != _other.m_sp; };

		[[nodiscard]] bool operator!=(std::nullptr_t)const { return m_sp != nullptr; };

	private:

		ObjHandle(const SharedPtr<Type>& _sp) :m_sp(_sp) {}

		template<class Other>
		friend class SharedObj;

		template<class Other>
		friend class WeakObj;

		template<class Other>
		friend class ObjHandle;

		SharedPtr<Type> m_sp{ nullptr };
	};

	template<class Type>
	class WeakObj final
	{
	public:

		WeakObj();

		WeakObj(std::nullptr_t);

		WeakObj(const SharedObj<Type>& _ptr);

		WeakObj(const ObjHandle<Type>& _handle);

		WeakObj(const WeakObj&) = default;

		template<class Other>
		WeakObj(const WeakObj<Other>& _other)requires(Concepts::BaseOf<Type, Other>);

		WeakObj(WeakObj&&) = default;

		template<class Other>
		WeakObj(WeakObj<Other>&& _other)requires(Concepts::BaseOf<Type, Other>);

		~WeakObj() = default;

		WeakObj& operator=(const WeakObj&) = default;

		WeakObj& operator=(WeakObj&&) = default;

		WeakObj& operator=(const SharedObj<Type>& _ptr);

		WeakObj& operator=(std::nullptr_t);

		/// @brief WeakObjが管理しているObjectをdynamic_castする
		template<class Other>
		[[nodiscard]] WeakObj<Other> castTo()const requires(Concepts::Lineage<Type, Other>);

		/// @brief WeakObjが管理しているObjectをdynamic_castしてからlockする
		template<class Other>
		[[nodiscard]] ObjHandle<Other> castToLock()const requires(Concepts::Lineage<Type, Other>);

		/// @brief WeakObjが管理しているObjectの参照を得る 
		[[nodiscard]] ObjHandle<Type> lock()const noexcept;

		/// @brief WeakObjがSharedObjを参照しているか
		[[nodiscard]] bool isLinked()const noexcept;

		[[nodiscard]] explicit operator bool()const noexcept;

		template<class Other>
		[[nodiscard]] bool operator==(const SharedObj<Other>& _other)const;

		template<class Other>
		[[nodiscard]] bool operator==(const WeakObj<Other>& _other)const;

		template<class Other>
		[[nodiscard]] bool operator==(const ObjHandle<Other>& _other)const;

		[[nodiscard]] bool operator==(std::nullptr_t)const;

		template<class Other>
		[[nodiscard]] bool operator!=(const SharedObj<Other>& _other)const;

		template<class Other>
		[[nodiscard]] bool operator!=(const WeakObj<Other>& _other)const;

		template<class Other>
		[[nodiscard]] bool operator!=(const ObjHandle<Other>& _other)const;

		[[nodiscard]] bool operator!=(std::nullptr_t)const;

	private:

		WeakPtr<Type> m_ref;

		template<class Other>
		friend class SharedObj;

		template<class Other>
		friend class WeakObj;

		template<class Other>
		friend class ObjHandle;
	};

	template<class Type>
	SharedObj<Type>::SharedObj()
		:m_object(nullptr)
	{

	}

	template<class Type>
	SharedObj<Type>::SharedObj(std::nullptr_t)
		: m_object(nullptr)
	{

	}

	template<class Type>
	template<class Other>
	SharedObj<Type>::SharedObj(Other* _other)requires(Concepts::BaseOf<Type, Other>)
		: m_object(_other)
	{
	}

	template<class Type>
	template<class Other>
	SharedObj<Type>::SharedObj(const SharedPtr<Other>& _other)requires(Concepts::BaseOf<Type, Other>)
		: m_object(_other)
	{
	}

	template<class Type>
	template<class Other>
	SharedObj<Type>::SharedObj(const SharedObj<Other>& _other)requires(Concepts::BaseOf<Type, Other>)
		: m_object(_other.m_object)
	{

	}

	template<class Type>
	template<class Other>
	SharedObj<Type>::SharedObj(SharedObj<Other>&& _other)requires(Concepts::BaseOf<Type, Other>)
		: m_object(std::forward<SharedPtr<Other>>(_other.m_object))
	{

	}

	template<class Type>
	template<class Other>
	SharedObj<Type>::SharedObj(const WeakObj<Other>& _ref)requires(Concepts::Lineage<Type, Other>)
		: m_object(std::dynamic_pointer_cast<Type>(_ref.m_ref.lock()))
	{
	}

	template<class Type>
	template<class Other>
	SharedObj<Type>& SharedObj<Type>::operator=(const SharedObj<Other>& _other)requires(Concepts::BaseOf<Type, Other>)
	{
		(*this) = _other.castTo<Type>();
		return *this;
	}

	template<class Type>
	template<class Other>
	SharedObj<Type>& SharedObj<Type>::operator=(SharedObj<Other>&& _other)requires(Concepts::BaseOf<Type, Other>)
	{
		(*this) = std::move(_other.castTo<Type>());
		return *this;
	}

	template<class Type>
	template<class Other>
	SharedObj<Type>& SharedObj<Type>::operator=(const WeakObj<Other>& _ref)requires(Concepts::Lineage<Type, Other>)
	{
		m_object = std::dynamic_pointer_cast<Type>(_ref.m_ref.lock());
		return *this;
	}

	template<class Type>
	SharedObj<Type>& SharedObj<Type>::operator=(std::nullptr_t)
	{
		m_object.reset();
		return *this;
	}

	template<class Type>
	Type& SharedObj<Type>::operator*()
	{
		return *m_object;
	}

	template<class Type>
	const Type& SharedObj<Type>::operator*()const
	{
		return *m_object;
	}

	template<class Type>
	Type* SharedObj<Type>::operator->()const
	{
		return m_object.get();
	}

	template<class Type>
	SharedObj<Type>::operator bool()const noexcept
	{
		return m_object.operator bool();
	}

	template<class Type>
	template<class Other>
	SharedObj<Other> SharedObj<Type>::castTo()const requires(Concepts::Lineage<Type, Other>)
	{
		return SharedObj<Other>{std::static_pointer_cast<Other>(m_object)};
	}

	template<class Type>
	WeakObj<Type> SharedObj<Type>::makeRef() const noexcept
	{
		return WeakObj<Type>{*this};
	}

	template<class Type>
	template<class Other>
	bool SharedObj<Type>::operator==(const SharedObj<Other>& _other) const
	{
		return m_object == _other.m_object;
	}

	template<class Type>
	template<class Other>
	bool SharedObj<Type>::operator==(const WeakObj<Other>& _other) const
	{
		return m_object == _other.m_ref.lock();
	}

	template<class Type>
	template<class Other>
	bool SharedObj<Type>::operator==(const ObjHandle<Other>& _other) const
	{
		return m_object == _other.m_sp;
	}

	template<class Type>
	bool SharedObj<Type>::operator==(std::nullptr_t) const
	{
		return m_object == nullptr;
	}

	template<class Type>
	template<class Other>
	bool SharedObj<Type>::operator!=(const SharedObj<Other>& _other) const
	{
		return m_object != _other.m_object;
	}

	template<class Type>
	template<class Other>
	bool SharedObj<Type>::operator!=(const WeakObj<Other>& _other) const
	{
		return m_object != _other.m_ref.lock();
	}

	template<class Type>
	template<class Other>
	bool SharedObj<Type>::operator!=(const ObjHandle<Other>& _other) const
	{
		return m_object != _other.m_sp;
	}

	template<class Type>
	bool SharedObj<Type>::operator!=(std::nullptr_t) const
	{
		return m_object != nullptr;
	}


	template<class Type>
	WeakObj<Type>::WeakObj()
		: m_ref()
	{

	}

	template<class Type>
	WeakObj<Type>::WeakObj(std::nullptr_t)
		: m_ref()
	{

	}

	template<class Type>
	WeakObj<Type>::WeakObj(const SharedObj<Type>& _ptr)
		: m_ref(_ptr.m_object)
	{

	}

	template<class Type>
	WeakObj<Type>::WeakObj(const ObjHandle<Type>& _handle)
		: m_ref(_handle.m_sp)
	{
	}

	template<class Type>
	template<class Other>
	WeakObj<Type>::WeakObj(const WeakObj<Other>& _other)requires(Concepts::BaseOf<Type, Other>)
		: m_ref(_other.m_ref.lock())
	{

	}

	template<class Type>
	template<class Other>
	WeakObj<Type>::WeakObj(WeakObj<Other>&& _other)requires(Concepts::BaseOf<Type, Other>)
		: m_ref(_other.m_ref.lock())
	{

	}

	template<class Type>
	WeakObj<Type>& WeakObj<Type>::operator=(const SharedObj<Type>& _ptr)
	{
		m_ref = _ptr.m_object;
		return *this;
	}

	template<class Type>
	WeakObj<Type>& WeakObj<Type>::operator=(std::nullptr_t)
	{
		m_ref.reset();
		return *this;
	}

	template<class Type>
	template<class Other>
	WeakObj<Other> WeakObj<Type>::castTo() const requires(Concepts::Lineage<Type, Other>)
	{
		if constexpr (std::is_base_of_v<Other, Type>)
		{
			return WeakObj<Other>{std::static_pointer_cast<Other>(m_ref.lock())};
		}
		else
		{
			return WeakObj<Other>{ std::dynamic_pointer_cast<Other>(m_ref.lock())};
		}
	}

	template<class Type>
	template<class Other>
	ObjHandle<Other> WeakObj<Type>::castToLock() const requires(Concepts::Lineage<Type, Other>)
	{
		if constexpr (std::is_base_of_v<Other, Type>)
		{
			return ObjHandle<Other>{std::static_pointer_cast<Other>(m_ref.lock())};
		}
		else
		{
			return ObjHandle<Other>{std::dynamic_pointer_cast<Other>(m_ref.lock())};
		}
	}

	template<class Type>
	ObjHandle<Type> WeakObj<Type>::lock() const noexcept
	{
		return ObjHandle<Type>{m_ref.lock()};
	}

	template<class Type>
	bool WeakObj<Type>::isLinked() const noexcept
	{
		return not m_ref.expired();
	}

	template<class Type>
	WeakObj<Type>::operator bool() const noexcept
	{
		return isLinked();
	}

	template<class Type>
	template<class Other>
	bool WeakObj<Type>::operator==(const SharedObj<Other>& _other) const
	{
		return m_ref.lock() == _other.m_object;
	}

	template<class Type>
	template<class Other>
	bool WeakObj<Type>::operator==(const WeakObj<Other>& _other) const
	{
		return m_ref.lock() == _other.m_ref.lock();
	}

	template<class Type>
	template<class Other>
	bool WeakObj<Type>::operator==(const ObjHandle<Other>& _other) const
	{
		return m_ref.lock() == _other.m_sp;
	}

	template<class Type>
	bool WeakObj<Type>::operator==(std::nullptr_t) const
	{
		return m_ref.lock() == nullptr;
	}

	template<class Type>
	template<class Other>
	bool WeakObj<Type>::operator!=(const SharedObj<Other>& _other) const
	{
		return m_ref.lock() != _other.m_object;
	}

	template<class Type>
	template<class Other>
	bool WeakObj<Type>::operator!=(const WeakObj<Other>& _other) const
	{
		return m_ref.lock() != _other.m_ref.lock();
	}

	template<class Type>
	template<class Other>
	bool WeakObj<Type>::operator!=(const ObjHandle<Other>& _other) const
	{
		return m_ref.lock() != _other.m_sp;
	}

	template<class Type>
	bool WeakObj<Type>::operator!=(std::nullptr_t) const
	{
		return m_ref.lock() != nullptr;
	}
}
