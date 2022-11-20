#include "../../include/CommonTag.hpp"
#include "../../include/Utility.hpp"

namespace Libra
{
	struct CommonTagInfo
	{
		String tag{ U"" };

		int32 count{ 0 };
	};

	HashTable<uint64, CommonTagInfo> g_DataBase;

	uint64 MakeCommonTagHash(const StringView tag)
	{
		uint64 hash = tag.hash();

		//同名が既に存在する
		if (g_DataBase.contains(hash))
		{
			auto& info = g_DataBase[hash];

			info.count += 1;

			return hash;
		}
		else
		{
			CommonTagInfo info;
			{
				info.tag = tag;
				info.count = 1;
			}

			g_DataBase[hash] = info;

			return hash;
		}
	}

	void RepaymentCommonTagByHash(const uint64 hash)
	{
		//返却する名前がない
		if (not g_DataBase.contains(hash))
			return;

		auto& info = g_DataBase[hash];

		info.count -= 1;

		if (info.count < 0)
		{
			info.count = 0;
		}
	}

	CommonTag::CommonTag()
		: m_hash(MakeCommonTagHash(U""))
	{
	}

	CommonTag::CommonTag(const StringView _tag)
		: m_hash(MakeCommonTagHash(_tag))
	{
	}

	CommonTag::CommonTag(const String& _tag)
		: m_hash(MakeCommonTagHash(_tag))
	{
	}

	CommonTag::CommonTag(const CommonTag& _other)
	{
		set(_other.get());
	}

	CommonTag::~CommonTag()
	{
		RepaymentCommonTagByHash(m_hash);
	}

	CommonTag& CommonTag::operator=(None_t)
	{
		set(unspecified);
		return *this;
	}

	CommonTag& CommonTag::operator=(const CommonTag& _other)
	{
		set(_other.get());
		return *this;
	}

	void CommonTag::set(None_t)
	{
		RepaymentCommonTagByHash(m_hash);
		m_hash = MakeCommonTagHash(U"");
	}

	void CommonTag::set(const StringView _name)
	{
		RepaymentCommonTagByHash(m_hash);
		m_hash = MakeCommonTagHash(_name);
	}

	void CommonTag::set(const String& _name)
	{
		RepaymentCommonTagByHash(m_hash);
		m_hash = MakeCommonTagHash(_name);
	}

	const String& CommonTag::get() const
	{
		return g_DataBase[m_hash].tag;
	}

	CommonTag::operator const String& () const
	{
		return g_DataBase[m_hash].tag;
	}

	uint64 CommonTag::hash() const noexcept
	{
		return m_hash;
	}

	size_t CommonTag::count() const noexcept
	{
		return g_DataBase[m_hash].count;
	}

	bool operator==(const CommonTag& name, const CommonTag& other)noexcept
	{
		return name.m_hash == other.m_hash;
	}

	bool operator==(const CommonTag& name, const StringView other)noexcept
	{
		return name.m_hash == other.hash();
	}

	bool operator==(const CommonTag& name, const String& other)noexcept
	{
		return name.m_hash == other.hash();
	}

	bool operator!=(const CommonTag& name, const CommonTag& other)noexcept
	{
		return name.m_hash != other.m_hash;
	}

	bool operator!=(const CommonTag& name, const StringView other)noexcept
	{
		return name.m_hash != other.hash();
	}

	bool operator!=(const CommonTag& name, const String& other)noexcept
	{
		return name.m_hash != other.hash();
	}
}
