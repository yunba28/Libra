#pragma once

#include <Siv3D.hpp>

namespace Libra
{
	/// @brief 固有の名前を作成・管理する
	class UniqueTag
	{
	public:

		UniqueTag(const StringView _name);

		UniqueTag(const String& _name);

		UniqueTag(const UniqueTag& _other);

		UniqueTag(UniqueTag&&) = default;

		~UniqueTag();

		UniqueTag& operator=(const UniqueTag& _other);

		UniqueTag& operator=(UniqueTag&&) = default;

		void set(const StringView _name);

		void set(const String& _name);

		const String& get()const;

		operator const String& ()const;

		uint64 hash()const noexcept;

		friend bool operator==(const UniqueTag& name, const UniqueTag& other)noexcept;

		friend bool operator==(const UniqueTag& name, const StringView other)noexcept;

		friend bool operator==(const UniqueTag& name, const String& other)noexcept;

		friend bool operator!=(const UniqueTag& name, const UniqueTag& other)noexcept;

		friend bool operator!=(const UniqueTag& name, const StringView other)noexcept;

		friend bool operator!=(const UniqueTag& name, const String& other)noexcept;

	private:

		UniqueTag(uint64 _hash);

		uint64 m_hash;

		friend class UniqueTagIssuer;
	};
}
