#pragma once

#ifndef Petal_Header_Log
#define Petal_Header_Log

#include "Petal~BasicTypes.h"
#include "Petal~String.h"
#include "Petal~Output.h"
#include "Petal~Bom.h"

#include <xstring>
#include <fstream>

namespace Petal::Log
{
	template <typename CharT, typename Traits = ::std::char_traits<CharT>, typename Alloc = ::std::allocator<CharT>>
	class LogStream : public Abstract::BasicOutput<CharT, Traits, Alloc>
	{
	public:
		using InnerChar = typename Abstract::BasicOutput<CharT, Traits, Alloc>::InnerChar;
		using InnerString = typename Abstract::BasicOutput<CharT, Traits, Alloc>::InnerString;
		using InnerStringView = ::std::basic_string_view<InnerChar>;
		using FileStream = typename ::std::basic_ofstream<byte, ::std::char_traits<byte>>;
	public:
		void Open(const ::std::string& file_name, const BOM::Bom& bom = BOM::RecommendBom<InnerChar>());
		void Open(const ::std::wstring& file_name, const BOM::Bom& bom = BOM::RecommendBom<InnerChar>());
		void AppendOpen(const ::std::string& file_name);
		void AppendOpen(const ::std::wstring& file_name);
		boolean Opened() const noexcept;
		boolean Valid() const noexcept;
		void Close();
		void ByteWrite(ptrc<byte> data, tsize size);
		void WriteBom(const BOM::Bom& bom);
		virtual void Output(InnerStringView str);
	public:
		LogStream() = default;
		LogStream(const ::std::string& file_name, const BOM::Bom& bom);
		LogStream(const ::std::wstring& file_name, const BOM::Bom& bom);
		LogStream(const ::std::string& file_name);
		LogStream(const ::std::wstring& file_name);
		LogStream(const LogStream&) = delete;
		LogStream(LogStream&&) = delete;
		LogStream& operator=(const LogStream&) = delete;
		virtual ~LogStream();
	private:
		FileStream file;
		static constexpr auto open_mode_trunc{ ::std::ios_base::out | ::std::ios_base::binary | ::std::ios_base::trunc };
		static constexpr auto open_mode_append{ ::std::ios_base::out | ::std::ios_base::binary | ::std::ios_base::app };
	};

	using LogA = LogStream<Char>;
	using LogW = LogStream<WChar>;
	using LogU8 = LogStream<CharU8>;
	using LogU16 = LogStream<CharU16>;
	using LogU32 = LogStream<CharU32>;
}

namespace Petal::Log
{
	template <typename CharT, typename Traits, typename Alloc>
	inline LogStream<CharT, Traits, Alloc>::LogStream(const ::std::string& file_name, const BOM::Bom& bom) :
		Abstract::BasicOutput<InnerChar, Traits, Alloc>()
	{
		this->Open(file_name, bom);
	}
	template <typename CharT, typename Traits, typename Alloc>
	inline LogStream<CharT, Traits, Alloc>::LogStream(const ::std::wstring& file_name, const BOM::Bom& bom) :
		Abstract::BasicOutput<InnerChar, Traits, Alloc>()
	{
		this->Open(file_name, bom);
	}
	template <typename CharT, typename Traits, typename Alloc>
	inline LogStream<CharT, Traits, Alloc>::LogStream(const ::std::string& file_name) :
		Abstract::BasicOutput<InnerChar, Traits, Alloc>()
	{
		this->AppendOpen(file_name);
	}
	template <typename CharT, typename Traits, typename Alloc>
	inline LogStream<CharT, Traits, Alloc>::LogStream(const ::std::wstring& file_name) :
		Abstract::BasicOutput<InnerChar, Traits, Alloc>()
	{
		this->AppendOpen(file_name);
	}
	template <typename CharT, typename Traits, typename Alloc>
	inline LogStream<CharT, Traits, Alloc>::~LogStream()
	{
		this->Close();
	}
	template <typename CharT, typename Traits, typename Alloc>
	inline void LogStream<CharT, Traits, Alloc>::Open(const ::std::string& file_name, const BOM::Bom& bom)
	{
		this->file.open(file_name, this->open_mode_trunc);
		this->WriteBom(bom);
	}
	template <typename CharT, typename Traits, typename Alloc>
	inline void LogStream<CharT, Traits, Alloc>::Open(const ::std::wstring& file_name, const BOM::Bom& bom)
	{
		this->file.open(file_name, this->open_mode_trunc);
		this->WriteBom(bom);
	}
	template <typename CharT, typename Traits, typename Alloc>
	inline void LogStream<CharT, Traits, Alloc>::AppendOpen(const ::std::string& file_name)
	{
		this->file.open(file_name, this->open_mode_append);
	}
	template <typename CharT, typename Traits, typename Alloc>
	inline void LogStream<CharT, Traits, Alloc>::AppendOpen(const ::std::wstring& file_name)
	{
		this->file.open(file_name, this->open_mode_append);
	}
	template <typename CharT, typename Traits, typename Alloc>
	inline boolean LogStream<CharT, Traits, Alloc>::Opened() const noexcept
	{
		return this->file.is_open();
	}
	template <typename CharT, typename Traits, typename Alloc>
	inline boolean LogStream<CharT, Traits, Alloc>::Valid() const noexcept
	{
		return (this->file.is_open() && this->file.good());
	}
	template <typename CharT, typename Traits, typename Alloc>
	inline void LogStream<CharT, Traits, Alloc>::Close()
	{
		if (this->file.is_open() == true) this->file.close();
	}
	template <typename CharT, typename Traits, typename Alloc>
	inline void LogStream<CharT, Traits, Alloc>::ByteWrite(ptrc<byte> data, tsize size)
	{
		if (this->Valid() == true && data != nullptr && size > 0)
		{
			this->file.write(data, size);
		}
	}
	template <typename CharT, typename Traits, typename Alloc>
	inline void LogStream<CharT, Traits, Alloc>::WriteBom(const BOM::Bom& bom)
	{
		this->ByteWrite(bom.Data(), bom.Size());
	}
	template <typename CharT, typename Traits, typename Alloc>
	inline void LogStream<CharT, Traits, Alloc>::Output(InnerStringView str)
	{
		this->ByteWrite(reinterpret_cast<ptrc<byte>>(str.data()), sizeof(typename InnerStringView::value_type) * str.size());
	}
}

#endif // !Petal_Header_Log
