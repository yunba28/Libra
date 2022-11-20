#pragma once

#include <stdexcept>
#include <string>

#include "Utility.hpp"
#include "Concepts.hpp"

namespace Libra
{
	/// @brief 例外クラス
	namespace Exceptions
	{
		using Exception = std::exception;

		using LogicError = std::logic_error;

		using RuntimeError = std::runtime_error;

#define DECLARE_EXCEPTION_CLASS( ExceptionsName )\
class ExceptionsName : public Exception{\
public:\
	using _MyBase = Exception;\
	explicit ExceptionsName(const char* _message) : _MyBase(_message){}\
	explicit ExceptionsName(const std::string& _message) : _MyBase(_message.c_str()){}\
}

#define DECLARE_LOGIC_ERROR_CLASS( ExceptionsName )\
class ExceptionsName : public LogicError{\
public:\
	using _MyBase = LogicError;\
	explicit ExceptionsName(const char* _message) : _MyBase(_message){}\
	explicit ExceptionsName(const std::string& _message) : _MyBase(_message.c_str()){}\
}

#define DECLARE_RUNTIME_ERROR_CLASS( ExceptionsName )\
class ExceptionsName : public RuntimeError{\
public:\
	using _MyBase = RuntimeError;\
	explicit ExceptionsName(const char* _message) : _MyBase(_message){}\
	explicit ExceptionsName(const std::string& _message) : _MyBase(_message.c_str()){}\
}

		DECLARE_LOGIC_ERROR_CLASS(OutOfRange);

		DECLARE_LOGIC_ERROR_CLASS(InvalidArgment);

		DECLARE_LOGIC_ERROR_CLASS(LengthError);

		DECLARE_LOGIC_ERROR_CLASS(DomainError);

		DECLARE_RUNTIME_ERROR_CLASS(OverflowError);

		DECLARE_RUNTIME_ERROR_CLASS(UnderflowError);

		DECLARE_RUNTIME_ERROR_CLASS(RangeError);

		DECLARE_EXCEPTION_CLASS(BadCast);

		DECLARE_EXCEPTION_CLASS(NullReference);

		DECLARE_EXCEPTION_CLASS(BadCopyAssignment);

		DECLARE_EXCEPTION_CLASS(BadMoveAssignment);

		template<class T>
		concept ModelException = std::derived_from<T, Exception>;
	}

	/// @brief 例外クラスを投げる
	/// @tparam T 例外クラス
	/// @param message メッセージ文
	/// @return この関数は戻り値を決して返さない
	template<Exceptions::ModelException T>
	[[noreturn]]
	void Throwing(const std::string& message)
	{
		throw T{ message.c_str() };
	}
}
