#include "../../include/UniqueTag.hpp"
#include "../../include/Utility.hpp"

namespace Libra
{
	struct UniqueTagInfo
	{
		String name{ U"" };

		uint64 baseHash{ 0ui64 };

		int32 count{ 0 };

		int32 maxCount{ 0 };
	};

	HashTable<uint64, UniqueTagInfo> g_DataBase;

	uint64 MakeUniqueTagHash(const StringView name)
	{
		uint64 hash = name.hash();

		//同名が既に存在する
		if (g_DataBase.contains(hash))
		{
			auto& info = g_DataBase[hash];

			String result{ name };

			if (info.maxCount == 0)
			{
				info.baseHash = hash;
				info.count = 1;
				info.maxCount = 1;
				return hash;
			}

			//名前が被らないように番号を末尾に付け足す
			result += U" #{}"_fmt(info.maxCount);

			info.count += 1;
			info.maxCount += 1;

			UniqueTagInfo info2;
			{
				info2.name = result;
				info2.baseHash = hash;
				info2.count = 1;
				info2.maxCount = 1;
			}

			g_DataBase[info2.name.hash()] = info2;

			return info2.name.hash();
		}
		else
		{
			UniqueTagInfo info;
			{
				info.name = name;
				info.baseHash = hash;
				info.count = 1;
				info.maxCount = 1;
			}

			g_DataBase[hash] = info;

			return hash;
		}
	}

	void RepaymentUniqueTagByHash(const uint64 hash)
	{
		//返却する名前がない
		if (not g_DataBase.contains(hash))
			return;

		auto& info = g_DataBase[hash];

		info.count -= 1;

		//使用者がいなくなった
		if (info.count == 0)
		{
			info.maxCount = 0;
		}

		if (info.baseHash == hash)
		{
			return;
		}

		//元になった名前の情報
		auto& baseInfo = g_DataBase[info.baseHash];

		baseInfo.count -= 1;

		if (baseInfo.count <= 0)
		{
			baseInfo.count = 0;
			baseInfo.maxCount = 0;
		}
	}

	UniqueTag::UniqueTag(const StringView _name)
		: m_hash(MakeUniqueTagHash(_name))
	{
	}

	UniqueTag::UniqueTag(const String& _name)
		: m_hash(MakeUniqueTagHash(_name))
	{
	}

	UniqueTag::UniqueTag(const UniqueTag& _other)
	{
		set(_other.get());
	}

	UniqueTag::~UniqueTag()
	{
		RepaymentUniqueTagByHash(m_hash);
	}

	UniqueTag& UniqueTag::operator=(const UniqueTag& _other)
	{
		set(_other.get());
		return *this;
	}

	void UniqueTag::set(const StringView _name)
	{
		RepaymentUniqueTagByHash(m_hash);
		m_hash = MakeUniqueTagHash(_name);
	}

	void UniqueTag::set(const String& _name)
	{
		RepaymentUniqueTagByHash(m_hash);
		m_hash = MakeUniqueTagHash(_name);
	}

	const String& UniqueTag::get() const
	{
		return g_DataBase[m_hash].name;
	}

	UniqueTag::operator const String& () const
	{
		return g_DataBase[m_hash].name;
	}

	uint64 UniqueTag::hash() const noexcept
	{
		return m_hash;
	}

	UniqueTag::UniqueTag(uint64 _hash)
		: m_hash(_hash)
	{
	}

	bool operator==(const UniqueTag& name, const UniqueTag& other)noexcept
	{
		return name.m_hash == other.m_hash;
	}

	bool operator==(const UniqueTag& name, const StringView other)noexcept
	{
		return name.m_hash == other.hash();
	}

	bool operator==(const UniqueTag& name, const String& other)noexcept
	{
		return name.m_hash == other.hash();
	}

	bool operator!=(const UniqueTag& name, const UniqueTag& other)noexcept
	{
		return name.m_hash != other.m_hash;
	}

	bool operator!=(const UniqueTag& name, const StringView other)noexcept
	{
		return name.m_hash != other.hash();
	}

	bool operator!=(const UniqueTag& name, const String& other)noexcept
	{
		return name.m_hash != other.hash();
	}
}
