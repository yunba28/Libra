#pragma once

#include <Siv3D.hpp>

namespace Libra
{
	class CommonTag
	{
	public:

		CommonTag();

		CommonTag(const StringView _name);

		CommonTag(const String& _name);

		CommonTag(const CommonTag& _other);

		CommonTag(CommonTag&&) = default;

		~CommonTag();

		CommonTag& operator=(None_t);

		CommonTag& operator=(const CommonTag& _other);

		CommonTag& operator=(CommonTag&&) = default;

		void set(None_t);

		void set(const StringView _name);

		void set(const String& _name);

		const String& get()const;

		operator const String& ()const;

		uint64 hash()const noexcept;

		size_t count()const noexcept;

		friend bool operator==(const CommonTag& name, const CommonTag& other)noexcept;

		friend bool operator==(const CommonTag& name, const StringView other)noexcept;

		friend bool operator==(const CommonTag& name, const String& other)noexcept;

		friend bool operator!=(const CommonTag& name, const CommonTag& other)noexcept;

		friend bool operator!=(const CommonTag& name, const StringView other)noexcept;

		friend bool operator!=(const CommonTag& name, const String& other)noexcept;

	private:

		uint64 m_hash;

	};
}

namespace std
{
	template<>
	struct hash<Libra::CommonTag>
	{
		size_t operator()(const Libra::CommonTag& tag)const noexcept
		{
			return tag.hash();
		}
	};
}
