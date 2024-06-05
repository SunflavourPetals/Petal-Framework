#pragma once

#ifndef Petal_Header_Log
#define Petal_Header_Log

#include "Petal~BasicTypes.h"
#include "Petal~String.h"
#include "Petal~Output.h"
#include "Petal~Bom.h"

#include <fstream>

namespace Petal
{
	template <typename CharT, typename Traits = ::std::char_traits<CharT>>
	class BasicLog : public Abstract::Output<CharT, Traits>
	{
	private:
		using Base = Abstract::Output<CharT, Traits>;
	public:
		using typename Base::CharType;
		using typename Base::TraitsType;
		using FileStream = typename ::std::basic_ofstream<byte, ::std::char_traits<byte>>;
	public:
		void Open(const ::std::string& file_name, const BOM::Bom& bom = BOM::RecommendBom<CharType>());
		void Open(const ::std::wstring& file_name, const BOM::Bom& bom = BOM::RecommendBom<CharType>());
		void AppendOpen(const ::std::string& file_name);
		void AppendOpen(const ::std::wstring& file_name);
		boolean Opened() const noexcept;
		boolean Valid() const noexcept;
		void Close();
		void WriteBytes(ptrc<byte> data, tsize size);
		void WriteBom(const BOM::Bom& bom);
		virtual void Write(BasicStringView<CharType, TraitsType> str) override;
		virtual LineBreakMode LnMode() noexcept override;
	public:
		BasicLog() = default;
		BasicLog(const ::std::string& file_name, const BOM::Bom& bom);
		BasicLog(const ::std::wstring& file_name, const BOM::Bom& bom);
		BasicLog(const ::std::string& file_name);
		BasicLog(const ::std::wstring& file_name);
		BasicLog(const BasicLog&) = delete;
		BasicLog& operator=(const BasicLog&) = delete;
		explicit operator bool();
		virtual ~BasicLog() = default;
	private:
		FileStream file{};
		LineBreakMode line_break_mode{};
		static constexpr auto open_mode_trunc{ ::std::ios_base::out | ::std::ios_base::binary | ::std::ios_base::trunc };
		static constexpr auto open_mode_append{ ::std::ios_base::out | ::std::ios_base::binary | ::std::ios_base::app };
	};

	using LogA = BasicLog<Char>;
	using LogW = BasicLog<WChar>;
	using LogU8 = BasicLog<U8Char>;
	using LogU16 = BasicLog<U16Char>;
	using LogU32 = BasicLog<U32Char>;
}

namespace Petal
{
	template <typename CharT, typename Traits>
	inline BasicLog<CharT, Traits>::BasicLog(const ::std::string& file_name, const BOM::Bom& bom) :
		Abstract::Output<CharT, Traits>()
	{
		this->Open(file_name, bom);
	}
	template <typename CharT, typename Traits>
	inline BasicLog<CharT, Traits>::BasicLog(const ::std::wstring& file_name, const BOM::Bom& bom) :
		Abstract::Output<CharT, Traits>()
	{
		this->Open(file_name, bom);
	}
	template <typename CharT, typename Traits>
	inline BasicLog<CharT, Traits>::BasicLog(const ::std::string& file_name) :
		Abstract::Output<CharT, Traits>()
	{
		this->AppendOpen(file_name);
	}
	template <typename CharT, typename Traits>
	inline BasicLog<CharT, Traits>::BasicLog(const ::std::wstring& file_name) :
		Abstract::Output<CharT, Traits>()
	{
		this->AppendOpen(file_name);
	}
	template <typename CharT, typename Traits>
	inline BasicLog<CharT, Traits>::operator bool()
	{
		return file.operator bool();
	}
	template <typename CharT, typename Traits>
	inline void BasicLog<CharT, Traits>::Open(const ::std::string& file_name, const BOM::Bom& bom)
	{
		this->file.open(file_name, this->open_mode_trunc);
		this->WriteBom(bom);
	}
	template <typename CharT, typename Traits>
	inline void BasicLog<CharT, Traits>::Open(const ::std::wstring& file_name, const BOM::Bom& bom)
	{
		this->file.open(file_name, this->open_mode_trunc);
		this->WriteBom(bom);
	}
	template <typename CharT, typename Traits>
	inline void BasicLog<CharT, Traits>::AppendOpen(const ::std::string& file_name)
	{
		this->file.open(file_name, this->open_mode_append);
	}
	template <typename CharT, typename Traits>
	inline void BasicLog<CharT, Traits>::AppendOpen(const ::std::wstring& file_name)
	{
		this->file.open(file_name, this->open_mode_append);
	}
	template <typename CharT, typename Traits>
	inline boolean BasicLog<CharT, Traits>::Opened() const noexcept
	{
		return this->file.is_open();
	}
	template <typename CharT, typename Traits>
	inline boolean BasicLog<CharT, Traits>::Valid() const noexcept
	{
		return (this->Opened() && this->file.good());
	}
	template <typename CharT, typename Traits>
	inline void BasicLog<CharT, Traits>::Close()
	{
		this->file.close();
	}
	template <typename CharT, typename Traits>
	inline void BasicLog<CharT, Traits>::WriteBytes(ptrc<byte> data, tsize size)
	{
		if (this->Valid() && size && data)
		{
			this->file.write(data, size);
		}
	}
	template <typename CharT, typename Traits>
	inline void BasicLog<CharT, Traits>::WriteBom(const BOM::Bom& bom)
	{
		this->WriteBytes(bom.Data(), bom.Size());
	}
	template <typename CharT, typename Traits>
	inline void BasicLog<CharT, Traits>::Write(BasicStringView<CharType, TraitsType> str)
	{
		this->WriteBytes(reinterpret_cast<ptrc<byte>>(str.data()), sizeof(CharType) * str.size());
	}
	template <typename CharT, typename Traits>
	inline LineBreakMode BasicLog<CharT, Traits>::LnMode() noexcept
	{
		return this->line_break_mode;
	}
}

#endif // !Petal_Header_Log
