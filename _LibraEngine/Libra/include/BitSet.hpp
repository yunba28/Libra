#pragma once

#include "Utility.hpp"
#include "Concepts.hpp"

namespace Libra
{
	template<Concepts::Integer Int>
	[[nodiscard]]
	constexpr Int BitOn(const Int src, const size_t index)noexcept
	{
		return src | (1 << index);
	}

	template<Concepts::Integer Int>
	[[nodiscard]]
	constexpr Int BitOff(const Int src, const size_t index)noexcept
	{
		return src & ~(1 << index);
	}

	template<Concepts::Integer Int>
	[[nodiscard]]
	constexpr bool BitTest(const Int src, const size_t index)noexcept
	{
		return src & (1 << index);
	}

	class BitSet64
	{
	public:

		using value_type = std::uint64_t;
		using difference_type = std::uint64_t;

		static constexpr value_type All = 0b11111111'11111111'11111111'11111111'11111111'11111111'11111111'11111111;

		constexpr BitSet64()noexcept :m_bits(0) {}

		constexpr BitSet64(const value_type _bits)noexcept : m_bits(_bits) {}

		constexpr BitSet64(const BitSet64&)noexcept = default;

		constexpr BitSet64(BitSet64&&)noexcept = default;

		constexpr ~BitSet64()noexcept = default;

		constexpr BitSet64& operator=(const BitSet64&)noexcept = default;

		constexpr BitSet64& operator=(BitSet64&&)noexcept = default;

		constexpr BitSet64& operator=(const value_type _bits)noexcept;

		/// @brief index番目のbitを設定する
		/// @param _index 設定するbitの場所
		/// @param _flag [true]:on  [false]:off
		constexpr void set(const difference_type _index, const bool _flag)noexcept;

		/// @brief マスクビットを設定する
		constexpr void setBits(const value_type _bits)noexcept;

		/// @brief index番目のbitを有効にする
		constexpr void on(const difference_type _index)noexcept;

		/// @brief 指定したマスクビットで表される部分をオンにする
		constexpr void onBits(const value_type _bits)noexcept;

		/// @brief index番目のbitを無効にする 
		constexpr void off(const difference_type _index)noexcept;

		/// @brief 指定したマスクビットで表される部分をオフにする
		constexpr void offBits(const value_type _bits)noexcept;

		/// @brief index番目のbitが有効かどうか
		[[nodiscard]]
		constexpr bool test(const difference_type _index)const noexcept;

		/// @brief index番目のbitが無効かどうか
		[[nodiscard]]
		constexpr bool none(const difference_type _index)const noexcept;

		/// @brief 全てのビットが有効かどうか
		[[nodiscard]]
		constexpr bool all()const noexcept;

		/// @brief 指定したマスクビットが全て有効かどうか
		[[nodiscard]]
		constexpr bool all(const value_type _mask)const noexcept;

		/// @brief いずれかのbitが有効かどうか
		[[nodiscard]]
		constexpr bool any()const noexcept;

		/// @brief 指定したマスクビットのいずれかが有効かどうか
		[[nodiscard]]
		constexpr bool any(const value_type _mask)const noexcept;

		/// @brief same any()
		[[nodiscard]]
		explicit constexpr operator bool()const noexcept;

		class Proxy
		{
		private:

			constexpr Proxy(BitSet64& _bitset, const difference_type _index)noexcept : m_bitset(_bitset), m_index(_index) {}

		public:

			constexpr Proxy& operator=(const bool _flag)noexcept
			{
				m_bitset.set(m_index, _flag);
				return *this;
			}

			[[nodiscard]] explicit constexpr operator bool()const noexcept
			{
				return m_bitset.test(m_index);
			}

		private:

			BitSet64& m_bitset;

			const difference_type m_index;

			friend class BitSet64;

		};

		[[nodiscard]]
		constexpr Proxy operator[](const difference_type _index)noexcept
		{
			return Proxy{ *this,_index };
		}

		class ConstProxy
		{
		private:

			constexpr ConstProxy(const BitSet64& _bitset, const difference_type _index)noexcept : m_bitset(_bitset), m_index(_index) {}

		public:

			[[nodiscard]] explicit constexpr operator bool()const noexcept
			{
				return m_bitset.test(m_index);
			}

		private:

			const BitSet64& m_bitset;

			const difference_type m_index;

			friend class BitSet64;

		};

		[[nodiscard]]
		constexpr ConstProxy operator[](const difference_type _index)const noexcept
		{
			return ConstProxy{ *this,_index };
		}

	private:

		std::uint64_t m_bits;

	};

	constexpr BitSet64& BitSet64::operator=(const value_type _bits)noexcept
	{
		m_bits = _bits;
		return *this;
	}

	constexpr void BitSet64::set(const difference_type _index, const bool _flag) noexcept
	{
		_flag ? on(_index) : off(_index);
	}

	constexpr void BitSet64::setBits(const value_type _bits) noexcept
	{
		m_bits = _bits;
	}

	constexpr void BitSet64::on(const difference_type _index) noexcept
	{
		if (_index < 64ui64)
		{
			m_bits |= (1ui64 << _index);
		}
	}

	constexpr void BitSet64::onBits(const value_type _bits) noexcept
	{
		m_bits |= _bits;
	}

	constexpr void BitSet64::off(const difference_type _index) noexcept
	{
		if (_index < 64ui64)
		{
			m_bits &= ~(1ui64 << _index);
		}
	}

	constexpr void BitSet64::offBits(const value_type _bits) noexcept
	{
		m_bits &= ~(_bits);
	}

	constexpr bool BitSet64::test(const difference_type _index) const noexcept
	{
		if (_index < 64ui64)
		{
			return m_bits & (1ui64 << _index);
		}

		return false;
	}

	constexpr bool BitSet64::none(const difference_type _index) const noexcept
	{
		if (_index < 64ui64)
		{
			return not (m_bits & (1ui64 << _index));
		}

		return false;
	}

	constexpr bool BitSet64::all() const noexcept
	{
		return m_bits == BitSet64::All;
	}

	constexpr bool BitSet64::all(const value_type _mask) const noexcept
	{
		return (m_bits & _mask) == _mask;
	}

	constexpr bool BitSet64::any() const noexcept
	{
		return m_bits != 0;
	}

	constexpr bool BitSet64::any(const value_type _mask) const noexcept
	{
		return m_bits & _mask;
	}

	constexpr BitSet64::operator bool() const noexcept
	{
		return m_bits != 0;
	}
}
