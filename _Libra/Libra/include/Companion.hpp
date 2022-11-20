#pragma once

#include "Exception.hpp"

namespace Libra
{
	/// @brief オブジェクトに親子関係を持たせる
	/// @tparam T 管理するオブジェクト
	template<class T>
	class Companion
	{
	public:

		Companion(T& _self);

		Companion(const Companion&) = delete;

		Companion(Companion&&)noexcept = default;

		~Companion();

		explicit operator bool()const noexcept;

		Companion& operator=(const Companion&) = delete;

		Companion& operator=(Companion&&)noexcept = default;

		/// @brief 自身の参照を取得
		T& self()noexcept;

		/// @brief 自身の参照を取得
		const T& self()const noexcept;

		/// @brief parentのCompanionに参加する
		/// @param _parent 
		void join(Companion& _parent);

		/// @brief parentのCompanionから抜ける
		void withdraw();

		/// @brief 親オブジェクトを取得
		/// @return 取得できなかったらnullptr
		Companion* getParent()const noexcept;

		/// @brief Companionの最上位オブジェクトを取得
		/// @return 取得できなかったらnullptr
		Companion* getRoot()const noexcept;

		/// @brief 指定したインデックスの子を取得
		/// @param _index 
		/// @return 指定する子のインデックス
		Companion* getChildren(size_t _index = 0ui64)const;

		/// @brief 親オブジェクトを取得
		/// @return 取得出来たらtrue
		bool tryGetParent(Companion*& _out)const noexcept;

		/// @brief Companionの最上位オブジェクトを取得
		/// @return 取得出来たらtrue
		bool tryGetRoot(Companion*& _out)const noexcept;

		/// @brief 指定したインデックスの子を取得
		/// @param out 取得した子を格納するCompanionのポインタ
		/// @param _index 指定する子のインデックス
		bool tryGetChildren(Companion*& _out, size_t _index = 0ui64)const noexcept;

		size_t childCount()const noexcept;

	private:

		T& m_self;

		Companion* m_parent{ nullptr };
		Companion* m_prev{ nullptr };
		Companion* m_next{ nullptr };
		Companion* m_children{ nullptr };

		size_t m_childCount;

	private:

		void uncheckedGetChildren(Companion*& _out, size_t _index)const noexcept;

	};

	template<class T>
	Companion<T>::Companion(T& _self)
		: m_self(_self)
	{
	}

	template<class T>
	Companion<T>::~Companion()
	{
		//親から外れる
		withdraw();

		Companion* pChild = m_children;

		//全ての親子関係を断ち切る
		while (pChild)
		{
			pChild->withdraw();
			pChild = pChild->m_next;
		}
	}

	template<class T>
	Companion<T>::operator bool() const noexcept
	{
		return (m_parent != nullptr) or (m_prev != nullptr) or (m_next != nullptr) or (m_children != nullptr);
	}

	template<class T>
	T& Companion<T>::self() noexcept
	{
		return m_self;
	}

	template<class T>
	const T& Companion<T>::self() const noexcept
	{
		return m_self;
	}

	template<class T>
	void Companion<T>::join(Companion& _parent)
	{
		//既に同じ親に所属している
		if (m_parent == &_parent)
			return;

		//以前の親から離脱する
		withdraw();

		//既にいる親の子の手前に割り込む
		if (_parent.m_children)
		{
			_parent.m_children->m_next = this;
			m_prev = _parent.m_children;
		}
		//自身が最初の子
		else
		{
			_parent.m_children = this;
		}

		_parent.m_childCount += 1;

		m_parent = &_parent;
	}

	template<class T>
	void Companion<T>::withdraw()
	{
		//どこにも所属していない
		if (not m_parent)
			return;

		//自身が最後の子
		if ((not m_prev) and (not m_next))
		{
			m_parent->m_children = nullptr;
		}

		//先に所属した兄弟がいる
		if (m_prev)
		{
			m_prev->m_next = m_next;
		}

		//後に所属した兄弟がいる
		if (m_next)
		{
			m_next->m_prev = m_prev;
		}

		if (m_parent->m_childCount > 0)
		{
			m_parent->m_childCount -= 1;
		}

		m_parent = nullptr;
		m_next = nullptr;
		m_prev = nullptr;
	}

	template<class T>
	Companion<T>* Companion<T>::getParent() const noexcept
	{
		return m_parent;
	}

	template<class T>
	Companion<T>* Companion<T>::getRoot() const noexcept
	{
		//自分が最上位のオブジェクト
		if (not m_parent)
		{
			return nullptr;
		}

		Companion* pParent = m_parent;

		//親が最上位オブジェクト
		if (not pParent->m_parent)
		{
			return pParent;
		}

		while (pParent->m_parent)
		{
			pParent = pParent->m_parent;
		}

		return pParent;
	}

	template<class T>
	Companion<T>* Companion<T>::getChildren(size_t _index) const
	{
		//範囲外指定
		if (_index < *m_childCount)
		{
			Companion* pChild = nullptr;
			uncheckedGetChildren(pChild, _index);
			return pChild;
		}

		Throwing<Exceptions::OutOfRange>("out of range");
	}

	template<class T>
	bool Companion<T>::tryGetParent(Companion*& _out) const noexcept
	{
		if (m_parent)
		{
			_out = m_parent;
			return true;
		}

		return false;
	}

	template<class T>
	bool Companion<T>::tryGetRoot(Companion*& _out) const noexcept
	{
		//自分が最上位のオブジェクト
		if (not m_parent)
		{
			return false;
		}

		Companion* pParent = m_parent;

		//親が最上位オブジェクト
		if (not pParent->m_parent)
		{
			_out = pParent;
			return true;
		}

		while (pParent->m_parent)
		{
			pParent = pParent->m_parent;
		}

		_out = pParent;

		return true;
	}

	template<class T>
	bool Companion<T>::tryGetChildren(Companion*& _out, size_t _index)const noexcept
	{
		//範囲外指定
		if (not (_index < *m_childCount))
		{
			return false;
		}

		uncheckedGetChildren(_out, _index);

		return true;
	}

	template<class T>
	size_t Companion<T>::childCount() const noexcept
	{
		return m_childCount;
	}

	template<class T>
	void Companion<T>::uncheckedGetChildren(Companion*& _out, size_t _index) const noexcept
	{
		Companion* pChild = m_children;

		size_t index = 0ui64;

		while (pChild)
		{
			if (index == _index)
			{
				_out = pChild;
				break;
			}

			pChild = pChild->m_next;
			index += 1;
		}
	}
}
