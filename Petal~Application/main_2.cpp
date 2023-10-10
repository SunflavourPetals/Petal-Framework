#include "Petal~Main.h"
#include "Petal~BasicTypes.h"
#include "Petal~VSDebugOutput.h"

#include <mmreg.h>
#include <xaudio2.h>
#include <wrl.h>
#pragma comment(lib, "xaudio2.lib")

#include <thread>
#include <chrono>
#include <string>
#include <format>
#include <fstream>
#include <iostream>

class RIFF
{
public:
	using dword = Petal::dword;
	class FOURCC
	{
	public:
#ifdef _XBOX //Big-Endian
		static constexpr dword RIFF = 'RIFF';
		static constexpr dword DATA = 'data';
		static constexpr dword FMT = 'fmt ';
		static constexpr dword WAVE = 'WAVE';
		static constexpr dword XWMA = 'XWMA';
		static constexpr dword DPDS = 'dpds';
#else //Little-Endian
		static constexpr dword RIFF = 'FFIR';
		static constexpr dword DATA = 'atad';
		static constexpr dword FMT = ' tmf';
		static constexpr dword WAVE = 'EVAW';
		static constexpr dword XWMA = 'AMWX';
		static constexpr dword DPDS = 'sdpd';
#endif
	};
};

class FileInterface
{
	Petal::ptr<Petal::byte> rec_buffer{ nullptr };
public:
	FileInterface() = default;
	~FileInterface() { if (rec_buffer) delete[] rec_buffer; }
	void ReadRIFFWave(std::wstring& filename)
	{
		using namespace Petal;
		std::locale::global(std::locale{ "chs" });
		struct File final
		{
			std::ifstream file;
			File(std::wstring& filename)
			{
				this->file.open(filename, std::ios_base::binary | std::ios_base::in);
			}
			~File()
			{
				this->file.close();
			}
		};
		File f(filename);
		if (f.file.is_open() == false) throw std::exception{ "failed in open file" };
		
		// RIFF
		dword fourcc_RIFF;
		f.file.read(reinterpret_cast<char*>(&fourcc_RIFF), sizeof(fourcc_RIFF));
		char* fourcc_c = reinterpret_cast<char*>(&fourcc_RIFF);
		dout + std::format("FOURCC: {}{}{}{}", fourcc_c[0], fourcc_c[1], fourcc_c[2], fourcc_c[3]) + ln;
		
		dword size{};
		f.file.read(reinterpret_cast<char*>(&size), sizeof(size));
		dout + std::format("SIZE: {}", size) + ln;
		
		dword file_type{};
		f.file.read(reinterpret_cast<char*>(&file_type), sizeof(file_type));
		char* file_type_sz = reinterpret_cast<char*>(&file_type);
		dout + std::format("FILE TYPE: {}{}{}{}", file_type_sz[0], file_type_sz[1], file_type_sz[2], file_type_sz[3]) + ln;

		if (fourcc_RIFF != RIFF::FOURCC::RIFF)
		{
			throw std::exception{ "not RIFF file" };
		}

		// fmt
		dword fourcc_fmt{};
		dword fmt_size{};
		WAVEFORMATEXTENSIBLE wave_format_struct{};
		for (; ; )
		{
			f.file.read(reinterpret_cast<char*>(&fourcc_fmt), sizeof(fourcc_fmt));
			f.file.read(reinterpret_cast<char*>(&fmt_size), sizeof(fmt_size));

			if (fourcc_fmt == RIFF::FOURCC::FMT)
			{
				fourcc_c = reinterpret_cast<char*>(&fourcc_fmt);
				dout + std::format("FOURCC(fmt ): {}{}{}{}", fourcc_c[0], fourcc_c[1], fourcc_c[2], fourcc_c[3]) + ln;
				dout + std::format("SIZE(fmt ): {}", fmt_size) + ln;
				f.file.read(reinterpret_cast<char*>(&wave_format_struct), fmt_size);
			}
			else
			{
				f.file.seekg(fmt_size, std::ios_base::cur);
			}
			if (f.file.eof())
			{
				throw std::exception{ "cannot find chunk 'fmt '" };
			}
			if (fourcc_fmt == RIFF::FOURCC::FMT) break;
		}
		
		// data
		dword fourcc_data{};
		dword data_size{};
		::XAUDIO2_BUFFER buffer{};
		for (; ; )
		{
			f.file.read(reinterpret_cast<char*>(&fourcc_data), sizeof(fourcc_data));
			f.file.read(reinterpret_cast<char*>(&data_size), sizeof(data_size));

			if (fourcc_data == RIFF::FOURCC::DATA)
			{
				fourcc_c = reinterpret_cast<char*>(&fourcc_data);
				dout + std::format("FOURCC(data): {}{}{}{}", fourcc_c[0], fourcc_c[1], fourcc_c[2], fourcc_c[3]) + ln;
				dout + std::format("SIZE(data): {}", data_size) + ln;
				ptr<Petal::byte> p_buffer = new Petal::byte[data_size];
				f.file.read(reinterpret_cast<char*>(p_buffer), data_size);
				buffer.AudioBytes = data_size;
				buffer.pAudioData = p_buffer;
				buffer.Flags = XAUDIO2_END_OF_STREAM;
			}
			else
			{
				f.file.seekg(data_size, std::ios_base::cur);
			}
			if (f.file.eof())
			{
				throw std::exception{ "cannot find chunk 'data'" };
			}
			if (fourcc_data == RIFF::FOURCC::DATA) break;
		}
		
		Play(wave_format_struct, buffer);
	}
	void Play(WAVEFORMATEXTENSIBLE& wave_format, XAUDIO2_BUFFER& wave_buffer)
	{
		using namespace Microsoft::WRL;
		using namespace Petal;

		ComPtr<::IXAudio2> xaudio2{};
		ptr<::IXAudio2MasteringVoice> master_voice{};
		ptr<::IXAudio2SourceVoice> source_voice;
		if (FAILED(::XAudio2Create(xaudio2.GetAddressOf(), 0, XAUDIO2_DEFAULT_PROCESSOR)))
		{
			throw std::exception{ "failed in XAudio2Create" };
		}
		
		if (FAILED(xaudio2->CreateMasteringVoice(&master_voice)))
		{
			throw std::exception{ "failed in IXAudio2::CreateMasteringVoice" };
		}

		if (FAILED(xaudio2->CreateSourceVoice(&source_voice, reinterpret_cast<ptr<::WAVEFORMATEX>>(&wave_format))))
		{
			throw std::exception{ "failed in IXAudio2::CreateSourceVoice" };
		}

		if (FAILED(source_voice->SubmitSourceBuffer(&wave_buffer)))
		{
			throw std::exception{ "failed in IXAudio2SourceVoice::SubmitSourceBuffer" };
		}

		if (FAILED(source_voice->Start(0)))
		{
			throw std::exception{ "failed in IXAudio2SourceVoice::Start" };
		}

		f64 len = (wave_buffer.AudioBytes * 8.0) / (
			wave_format.Format.nSamplesPerSec * 
			wave_format.Format.wBitsPerSample *
			wave_format.Format.nChannels);

		std::string info = std::format("Length: {} seconds, AudioBytes: {} bytes, Rate: {}Hz, Precision: {}bits, Num of Channels: {}",
			len,
			wave_buffer.AudioBytes,
			wave_format.Format.nSamplesPerSec,
			wave_format.Format.wBitsPerSample,
			wave_format.Format.nChannels);
		dout + info.c_str() + ln;
		std::cout << " " << info << "\n";

		std::cout << " playing     ";
		for (int i{}; i < 11; ++i) { std::cout << "        "; }
		std::cout << "||-finish\n ";
		for (int i{}; i < 100; ++i)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds{ static_cast<i64>((len * 1000) / 100) });
			std::cout << ".";
		}
		std::this_thread::sleep_for(std::chrono::milliseconds{ 1000 });
		
		std::cout << "||\n end\n";
	}
};

class XAudio2Demo
{
public:
	static int main() try
	{
		struct Co
		{
			Co()
			{
				auto unused{ ::CoInitializeEx(0, COINIT::COINIT_MULTITHREADED) };
				unused;
			}
			~Co()
			{
				::CoUninitialize();
			}
		};
		Co co;
		FileInterface fi;
		std::wstring filename;
		std::cout << "请输入要播放的.WAV文件: ";
		std::wcin.imbue(std::locale{ "chs" });
		std::wcin >> filename;
		fi.ReadRIFFWave(filename);
		return 0;
	}
	catch (const std::exception& e)
	{
		std::cout << "! exception: " << e.what() << "\n";
		return 0;
	}
};

Petal_SetMainClass(XAudio2Demo)
