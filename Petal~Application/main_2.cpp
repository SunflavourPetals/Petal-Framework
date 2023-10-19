#include "Petal~Main.h"
#include "Petal~BasicTypes.h"
#include "Petal~VSDebugOutput.h"
#include "Petal~KeyboardEventProcess.h"
#include "Petal~PerformanceCounter.h"

#define XAUDIO2_HELPER_FUNCTIONS


#include <wrl.h>

#include <mmreg.h>
#include <xaudio2.h>
// #include <hrtfapoapi.h>
#include <x3daudio.h>
#include <xapo.h>
#include <xapobase.h>
#include <xapofx.h>
#include <xaudio2fx.h>

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
	void ReadRIFFWave(std::wstring& filename, ::WAVEFORMATEXTENSIBLE& wave_format_struct, ::XAUDIO2_BUFFER& buffer)
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
		wave_format_struct = {};
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
		buffer = {};
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
	}

};

class VoiceCallback : public IXAudio2VoiceCallback
{
public:
	HANDLE hBufferEndEvent;
	VoiceCallback() : hBufferEndEvent(CreateEvent(NULL, FALSE, FALSE, NULL)) {}
	~VoiceCallback() { ::CloseHandle(hBufferEndEvent); }
	//Called when the voice has just finished playing a contiguous audio stream.
	void __declspec(nothrow) OnStreamEnd() { ::SetEvent(hBufferEndEvent); }
	//Unused methods are stubs
	void OnVoiceProcessingPassEnd() noexcept {}
	void OnVoiceProcessingPassStart(::UINT32 SamplesRequired) noexcept {}
	void OnBufferEnd(void* pBufferContext) noexcept {}
	void OnBufferStart(void* pBufferContext) noexcept {}
	void OnLoopEnd(void* pBufferContext) noexcept {}
	void OnVoiceError(void* pBufferContext, ::HRESULT Error) noexcept {}
};

class AudioInterface
{
public:
	template <typename Ty>
	using ComPtr = Microsoft::WRL::ComPtr<Ty>;
	ComPtr<::IXAudio2> xaudio2{};
	Petal::ptr<::IXAudio2MasteringVoice> master_voice{};
	::XAUDIO2_VOICE_DETAILS master_details{};
	Petal::ptr<::IXAudio2SourceVoice> source_voice{};
	Petal::ptr<::IXAudio2SourceVoice> source_voice2{};
	Petal::ptr<IXAudio2SubmixVoice> submix_voice{};

	AudioInterface()
	{
		using namespace Petal;
		if (FAILED(::XAudio2Create(xaudio2.GetAddressOf(), 0, XAUDIO2_DEFAULT_PROCESSOR)))
		{
			throw std::exception{ "failed in XAudio2Create" };
		}

		if (FAILED(xaudio2->CreateMasteringVoice(&master_voice)))
		{
			throw std::exception{ "failed in IXAudio2::CreateMasteringVoice" };
		}

		
		master_voice->GetVoiceDetails(&master_details);

		if (FAILED(xaudio2->CreateSubmixVoice(&submix_voice, master_details.InputChannels, 44100)))
		{
			throw std::exception{ "failed in IXAudio2::CreateSubmixVoice" };
		}
	}
	void Play(WAVEFORMATEXTENSIBLE& wave_format, XAUDIO2_BUFFER& wave_buffer)
	{

		using namespace Petal;

		VoiceCallback voice_callback{};

		XAUDIO2_SEND_DESCRIPTOR sfx_send{ 0, submix_voice };
		XAUDIO2_VOICE_SENDS sfx_send_list{ 1, &sfx_send };

		if (FAILED(xaudio2->CreateSourceVoice(
			&source_voice,
			reinterpret_cast<ptr<::WAVEFORMATEX>>(&wave_format),
			0,
			XAUDIO2_DEFAULT_FREQ_RATIO,
			&voice_callback,
		//	&sfx_send_list,
			NULL,
			NULL
		)))
		{
			throw std::exception{ "failed in IXAudio2::CreateSourceVoice" };
		}
		if (FAILED(source_voice->SetOutputVoices(&sfx_send_list)))
		{
			throw std::exception{ "failed in IXAudio2SourceVoice::SetOutputVoices" };
		}
		bool over_flag{ false };
		std::thread wait_thread([](VoiceCallback& voice_callback, bool& flag) { ::WaitForSingleObjectEx(voice_callback.hBufferEndEvent, INFINITE, TRUE); std::cout << "THREAD END\n"; flag = true; }, std::ref(voice_callback), std::ref(over_flag));
		if (FAILED(source_voice->SubmitSourceBuffer(&wave_buffer)))
		{
			throw std::exception{ "failed in IXAudio2SourceVoice::SubmitSourceBuffer" };
		}

		// new things...
		DWORD dwChannelMask;
		master_voice->GetChannelMask(&dwChannelMask);
		dout + std::format("master channel mask: {:x}", dwChannelMask) + ln;
		float outputMatrix[8]{};
		// pan of -1.0 indicates all left speaker, 
		// 1.0 is all right speaker, 0.0 is split between left and right
		float pan = 0.0f;
		float left = 0.5f - pan / 2;
		float right = 0.5f + pan / 2;
		switch (dwChannelMask)
		{
		case SPEAKER_MONO:
			outputMatrix[0] = 1.0;
			break;
		case SPEAKER_STEREO:
		case SPEAKER_2POINT1:
		case SPEAKER_SURROUND:
			outputMatrix[0] = left;
			outputMatrix[1] = right;
			break;
		case SPEAKER_QUAD:
			outputMatrix[0] = outputMatrix[2] = left;
			outputMatrix[1] = outputMatrix[3] = right;
			break;
		case SPEAKER_4POINT1:
			outputMatrix[0] = outputMatrix[3] = left;
			outputMatrix[1] = outputMatrix[4] = right;
			break;
		case SPEAKER_5POINT1:
		case SPEAKER_7POINT1:
		case SPEAKER_5POINT1_SURROUND:
			outputMatrix[0] = outputMatrix[4] = left;
			outputMatrix[1] = outputMatrix[5] = right;
			break;
		case SPEAKER_7POINT1_SURROUND:
			outputMatrix[0] = outputMatrix[4] = outputMatrix[6] = left;
			outputMatrix[1] = outputMatrix[5] = outputMatrix[7] = right;
			break;
		}
		// Assuming pVoice sends to pMasteringVoice

		XAUDIO2_VOICE_DETAILS VoiceDetails;
		source_voice->GetVoiceDetails(&VoiceDetails);

		XAUDIO2_VOICE_DETAILS MasterVoiceDetails;
		master_voice->GetVoiceDetails(&MasterVoiceDetails);

		source_voice->SetOutputMatrix(NULL, VoiceDetails.InputChannels, MasterVoiceDetails.InputChannels, outputMatrix);

		// ---------------------------------

		ComPtr<IUnknown> pXAPO;
	//	if (FAILED(::CreateFX(__uuidof(FXReverb), pXAPO.GetAddressOf())))
	//	if (FAILED(::CreateFX(__uuidof(FXEcho), pXAPO.GetAddressOf())))
	//	if (FAILED(::CreateFX(__uuidof(FXMasteringLimiter), pXAPO.GetAddressOf())))
		if (FAILED(::CreateFX(__uuidof(FXEQ), pXAPO.GetAddressOf())))
		{
			throw std::exception{ "failed in CreateFX" };
		}

		::XAUDIO2_EFFECT_DESCRIPTOR descriptor{};
		descriptor.InitialState = true;
		descriptor.OutputChannels = master_details.InputChannels;
		descriptor.pEffect = pXAPO.Get();

		::XAUDIO2_EFFECT_CHAIN chain;
		chain.EffectCount = 1;
		chain.pEffectDescriptors = &descriptor;

		::FXECHO_PARAMETERS fx_echo_parameters;
		fx_echo_parameters.Delay = FXECHO_DEFAULT_DELAY; (FXECHO_MIN_DELAY + FXECHO_MAX_DELAY) / 2.0f;
		fx_echo_parameters.WetDryMix = FXECHO_DEFAULT_WETDRYMIX;
		fx_echo_parameters.Feedback = FXECHO_DEFAULT_FEEDBACK;

		::FXREVERB_PARAMETERS XAPOParameters;
		XAPOParameters.Diffusion = FXREVERB_DEFAULT_DIFFUSION; FXREVERB_MAX_DIFFUSION; (FXREVERB_MIN_DIFFUSION + FXREVERB_MAX_DIFFUSION) / 1.5f;
		XAPOParameters.RoomSize = FXREVERB_DEFAULT_ROOMSIZE; FXREVERB_MAX_ROOMSIZE; (FXREVERB_MIN_ROOMSIZE + FXREVERB_MAX_ROOMSIZE) / 4.f;

		::FXMASTERINGLIMITER_PARAMETERS fx_ml_parameters{};
		fx_ml_parameters.Loudness = FXMASTERINGLIMITER_DEFAULT_LOUDNESS;
		fx_ml_parameters.Release = FXMASTERINGLIMITER_DEFAULT_RELEASE;

		::FXEQ_PARAMETERS fx_eq_parameters{};
		fx_eq_parameters.Bandwidth0 = FXEQ_DEFAULT_BANDWIDTH;
		fx_eq_parameters.Bandwidth1 = FXEQ_DEFAULT_BANDWIDTH;
		fx_eq_parameters.Bandwidth2 = FXEQ_DEFAULT_BANDWIDTH;
		fx_eq_parameters.Bandwidth3 = FXEQ_DEFAULT_BANDWIDTH;
		fx_eq_parameters.Gain0 = FXEQ_DEFAULT_GAIN;
		fx_eq_parameters.Gain1 = FXEQ_DEFAULT_GAIN;
		fx_eq_parameters.Gain2 = FXEQ_DEFAULT_GAIN;
		fx_eq_parameters.Gain3 = FXEQ_DEFAULT_GAIN;
		fx_eq_parameters.FrequencyCenter0 = FXEQ_DEFAULT_FREQUENCY_CENTER_0;
		fx_eq_parameters.FrequencyCenter1 = FXEQ_DEFAULT_FREQUENCY_CENTER_1;
		fx_eq_parameters.FrequencyCenter2 = FXEQ_DEFAULT_FREQUENCY_CENTER_2;
		fx_eq_parameters.FrequencyCenter3 = FXEQ_DEFAULT_FREQUENCY_CENTER_3;
		
		// ----------------------------------

		if (FAILED(source_voice->Start(0)))
		{
			throw std::exception{ "failed in IXAudio2SourceVoice::Start" };
		}


	//	XAudio2FrequencyRatioToSemitones(1);
	//	source_voice->SetFrequencyRatio(XAudio2SemitonesToFrequencyRatio(1.f)); /* 变调 升高一个半音 */

		
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

		// X3DAudio ------------------------------------------------

		X3DAUDIO_HANDLE X3DInstance;
		if (FAILED(::X3DAudioInitialize(dwChannelMask, X3DAUDIO_SPEED_OF_SOUND, X3DInstance)))
		{
			throw std::exception{ "failed in X3DAudioInitialize" };
		}

		X3DAUDIO_LISTENER Listener{};
		X3DAUDIO_EMITTER Emitter{};

		Emitter.ChannelCount = VoiceDetails.InputChannels; 1; master_details.InputChannels;
		Emitter.CurveDistanceScaler = Emitter.DopplerScaler = 1.0f;

		X3DAUDIO_DSP_SETTINGS DSPSettings = {};
		FLOAT32* matrix = new FLOAT32[master_details.InputChannels]{};
		DSPSettings.SrcChannelCount = VoiceDetails.InputChannels;
		DSPSettings.DstChannelCount = master_details.InputChannels;
		DSPSettings.pMatrixCoefficients = matrix;

		DirectX::XMFLOAT3 emitter_pos{ 0.0f, 0.0f, 1.0f };
		DirectX::XMFLOAT3 listener_pos{ 0.0f, 0.0f, 0.0f };
		Emitter.OrientFront = DirectX::XMFLOAT3{ 0.0f, 0.0f, -1.0f }; // EmitterOrientFront;
		Emitter.OrientTop = DirectX::XMFLOAT3{ 0.0f, 1.0f, 0.0f }; // EmitterOrientTop;
		Emitter.Position = emitter_pos; DirectX::XMFLOAT3{ 0.0f, 0.0f, 5.0f }; // EmitterPosition;
		Emitter.Velocity = DirectX::XMFLOAT3{ 0.0f, 0.0f, 0.0f }; // EmitterVelocity;
		Emitter.ChannelRadius = 1.0f;
		FLOAT32* p_azimuths = new FLOAT32[VoiceDetails.InputChannels]{ 0.0f };
		for (u32 i = 0; i < VoiceDetails.InputChannels; ++i)
		{
			p_azimuths[i] = 3.1415927f;
		}
		Emitter.pChannelAzimuths = p_azimuths;
		Listener.OrientFront = DirectX::XMFLOAT3{ 0.0f, 0.0f, 1.0f }; // ListenerOrientFront;
		Listener.OrientTop = DirectX::XMFLOAT3{ 0.0f, 1.0f, 0.0f }; // ListenerOrientTop;
		Listener.Position = listener_pos; DirectX::XMFLOAT3{ 0.0f, 0.0f, 0.0f }; // ListenerPosition;
		Listener.Velocity = DirectX::XMFLOAT3{ 0.0f, 0.0f, 0.0f }; // ListenerVelocity;

		class GotoXY final
		{
		public:
			using X = decltype(::COORD::X);
			using Y = decltype(::COORD::Y);
		public:
			inline void operator()(X x, Y y)
			{
				this->operator()({ x, y });
			}
			inline void operator()(COORD coordinate)
			{
				::SetConsoleCursorPosition(this->output_handle, coordinate);
			}
			GotoXY()
			{
				output_handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
			}
		private:
			HANDLE output_handle{};
		} goto_xy{};

		Keyboard::Controller ctrl{};
		ctrl.Register('W');
		ctrl.Register('A');
		ctrl.Register('S');
		ctrl.Register('D');
		PerformanceCounter c;

		class BasicK : public Keyboard::KeyPositiveProcess
		{
		public:
			BasicK(VirtualKey::Type k, DirectX::XMFLOAT3& pos_, PerformanceCounter& c_) : KeyPositiveProcess(k), pos(pos_), c(c_) {}
			DirectX::XMFLOAT3& pos;
			PerformanceCounter& c;
		};
		class W : public BasicK
		{
		public:
			W(DirectX::XMFLOAT3& pos, PerformanceCounter& c) : BasicK('W', pos, c) {}
			void Execution(const Resource& resource) override
			{
				pos.z += (resource.delta_count / static_cast<float>(c.Frequency())) * 8.0f;
			}
		};
		class S : public BasicK
		{
		public:
			S(DirectX::XMFLOAT3& pos, PerformanceCounter& c) : BasicK('S', pos, c) {}
			void Execution(const Resource& resource) override
			{
				pos.z -= (resource.delta_count / static_cast<float>(c.Frequency())) * 8.0f;
			}
		};
		class A : public BasicK
		{
		public:
			A(DirectX::XMFLOAT3& pos, PerformanceCounter& c) : BasicK('A', pos, c) {}
			void Execution(const Resource& resource) override
			{
				pos.x -= (resource.delta_count / static_cast<float>(c.Frequency())) * 4.0f;
			}
		};
		class D : public BasicK
		{
		public:
			D(DirectX::XMFLOAT3& pos, PerformanceCounter& c) : BasicK('D', pos, c) {}
			void Execution(const Resource& resource) override
			{
				pos.x += (resource.delta_count / static_cast<float>(c.Frequency())) * 4.0f;
			}
		};
		std::vector<std::unique_ptr<Abstract::KeyboardEventProcess>> v;
		v.push_back(std::make_unique<W>(Emitter.Position, c));
		v.push_back(std::make_unique<A>(Emitter.Position, c));
		v.push_back(std::make_unique<S>(Emitter.Position, c));
		v.push_back(std::make_unique<D>(Emitter.Position, c));
		c.Count();
		for (; over_flag != true; )
		{
			c.Count();
			ctrl.Update(v.begin(), v.end(), c.DeltaCounts());

			goto_xy({ 0, 2 });
			std::cout << std::format("Emitter pos: (x:{:08.3f}, y:{:08.3f}, z:{:08.3f})    \n", Emitter.Position.x, Emitter.Position.y, Emitter.Position.z);

			::X3DAudioCalculate(X3DInstance, &Listener, &Emitter,
				X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_REVERB,
				&DSPSettings);
			
		//	dout + "DEBUG 0001" + ln;
			if (FAILED(source_voice->SetOutputMatrix(submix_voice, VoiceDetails.InputChannels, master_details.InputChannels, DSPSettings.pMatrixCoefficients)))
			{
				throw std::exception{ "failed in source_voice->SetOutputMatrix(master_voice, 1, master_details.InputChannels, DSPSettings.pMatrixCoefficients)" };
			}
		//	dout + "DEBUG 0002" + ln;
			if (FAILED(source_voice->SetFrequencyRatio(DSPSettings.DopplerFactor)))
			{
				throw std::exception{ "failed in source_voice->SetFrequencyRatio(DSPSettings.DopplerFactor)" };
			}
		//	dout + "DEBUG 0003" + ln;
		//	if (FAILED(source_voice->SetOutputMatrix(submix_voice, VoiceDetails.InputChannels, master_details.InputChannels, &DSPSettings.ReverbLevel)))
		//	{
		//		throw std::exception{ "failed in source_voice->SetOutputMatrix(submix_voice, 1, 1, &DSPSettings.ReverbLevel)" };
		//	}
			//	dout + "DEBUG 0004" + ln;
		//	XAUDIO2_FILTER_PARAMETERS FilterParameters = { LowPassFilter, 2.0f * std::sinf(X3DAUDIO_PI / 6.0f * DSPSettings.LPFDirectCoefficient), 1.0f };
		//	HRESULT hr_sp{};
		//	if (FAILED(hr_sp = source_voice->SetFilterParameters(&FilterParameters)))
		//	{
		//		dout + std::format("he_sp: {:x}", static_cast<unsigned long>(hr_sp)) + ln;
		//		throw std::exception{ "failed in source_voice->SetFilterParameters(&FilterParameters)" };
		//	}
			//	dout + "DEBUG 0005" + ln;

			Sleep(5);
				// ---------------------------------------------------------
		}

		/*
		std::cout << " playing     ";
		for (int i{}; i < 11; ++i) { std::cout << "        "; }
		std::cout << "||-finish\n ";
		for (int i{}; i < 100; ++i)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds{ static_cast<i64>((len * 1000) / 100) });
			std::cout << ".";
			
			if (i == 3)
			{
			//	float vols[2]{ 1.85f, 1.75f };
			//	auto err = submix_voice->SetChannelVolumes(master_details.InputChannels, vols); // 设置 Channels 的 Volume
			//	if (FAILED(err))
			//	{
			//		dout + "FAILED IN IF BRANCH" + ln;
			//	}
			//	else dout + "SUCCEED IN IF BRANCH" + ln;
			//	if (FAILED(submix_voice->SetEffectChain(&chain)))
			//	{
			//		throw std::exception{ "failed in IXAudio2Voice::SetEffectChain" };
			//	}
			//	if (FAILED(submix_voice->SetEffectParameters(0, &fx_eq_parameters, sizeof(fx_eq_parameters))))
			//	{
			//		throw std::exception{ "failed in IXAudio2Voice::SetEffectParameters" };
			//	}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds{ 1000 });

		std::cout << "||\n end\n";
		*/

	//	wait_thread.join();
	}
	void Play2(WAVEFORMATEXTENSIBLE& wave_format, XAUDIO2_BUFFER& wave_buffer, WAVEFORMATEXTENSIBLE& wave_format2, XAUDIO2_BUFFER& wave_buffer2)
	{

		using namespace Petal;

		VoiceCallback voice_callback{};

		XAUDIO2_SEND_DESCRIPTOR sfx_send{ 0, submix_voice };
		XAUDIO2_VOICE_SENDS sfx_send_list{ 1, &sfx_send };

		if (FAILED(xaudio2->CreateSourceVoice(
			&source_voice,
			reinterpret_cast<ptr<::WAVEFORMATEX>>(&wave_format),
			0,
			XAUDIO2_DEFAULT_FREQ_RATIO,
			&voice_callback,
			&sfx_send_list,
			NULL
		)))
		{
			throw std::exception{ "failed in IXAudio2::CreateSourceVoice" };
		}
		std::thread wait_thread([](VoiceCallback& voice_callback) { ::WaitForSingleObjectEx(voice_callback.hBufferEndEvent, INFINITE, TRUE); std::cout << "THREAD END\n"; }, std::ref(voice_callback));
		if (FAILED(source_voice->SubmitSourceBuffer(&wave_buffer)))
		{
			throw std::exception{ "failed in IXAudio2SourceVoice::SubmitSourceBuffer" };
		}


		if (FAILED(xaudio2->CreateSourceVoice(
			&source_voice2,
			reinterpret_cast<ptr<::WAVEFORMATEX>>(&wave_format2),
			0,
			XAUDIO2_DEFAULT_FREQ_RATIO,
			NULL,
			&sfx_send_list,
			NULL
		)))
		{
			throw std::exception{ "failed in IXAudio2::CreateSourceVoice" };
		}
		if (FAILED(source_voice2->SubmitSourceBuffer(&wave_buffer2)))
		{
			throw std::exception{ "failed in IXAudio2SourceVoice::SubmitSourceBuffer" };
		}


		decltype(source_voice) source_voice3{};

		if (FAILED(xaudio2->CreateSourceVoice(
			&source_voice3,
			reinterpret_cast<ptr<::WAVEFORMATEX>>(&wave_format2),
			0,
			XAUDIO2_DEFAULT_FREQ_RATIO,
			NULL,
			&sfx_send_list,
			NULL
		)))
		{
			throw std::exception{ "failed in IXAudio2::CreateSourceVoice (3)" };
		}
		if (FAILED(source_voice2->SubmitSourceBuffer(&wave_buffer2)))
		{
			throw std::exception{ "failed in IXAudio2SourceVoice::SubmitSourceBuffer (3)" };
		}

		// new things...
		DWORD dwChannelMask;
		master_voice->GetChannelMask(&dwChannelMask);
		dout + std::format("master channel mask: {:x}", dwChannelMask) + ln;
		float outputMatrix[8]{};
		// pan of -1.0 indicates all left speaker, 
		// 1.0 is all right speaker, 0.0 is split between left and right
		float pan = -0.00625f;
		float left = 0.5f - pan / 2;
		float right = 0.5f + pan / 2;
		switch (dwChannelMask)
		{
		case SPEAKER_MONO:
			outputMatrix[0] = 1.0;
			break;
		case SPEAKER_STEREO:
		case SPEAKER_2POINT1:
		case SPEAKER_SURROUND:
			outputMatrix[0] = left;
			outputMatrix[1] = right;
			break;
		case SPEAKER_QUAD:
			outputMatrix[0] = outputMatrix[2] = left;
			outputMatrix[1] = outputMatrix[3] = right;
			break;
		case SPEAKER_4POINT1:
			outputMatrix[0] = outputMatrix[3] = left;
			outputMatrix[1] = outputMatrix[4] = right;
			break;
		case SPEAKER_5POINT1:
		case SPEAKER_7POINT1:
		case SPEAKER_5POINT1_SURROUND:
			outputMatrix[0] = outputMatrix[4] = left;
			outputMatrix[1] = outputMatrix[5] = right;
			break;
		case SPEAKER_7POINT1_SURROUND:
			outputMatrix[0] = outputMatrix[4] = outputMatrix[6] = left;
			outputMatrix[1] = outputMatrix[5] = outputMatrix[7] = right;
			break;
		}
		// Assuming pVoice sends to pMasteringVoice

		XAUDIO2_VOICE_DETAILS VoiceDetails;
		source_voice->GetVoiceDetails(&VoiceDetails);

		XAUDIO2_VOICE_DETAILS MasterVoiceDetails;
		master_voice->GetVoiceDetails(&MasterVoiceDetails);

		if (FAILED(source_voice->SetOutputMatrix(submix_voice, VoiceDetails.InputChannels, 2, outputMatrix)))
		{
			dout + "FAILED IN SetOutputMatrix 1";
		}

		// --------------------------------- 2

		float outputMatrix2[8]{};
		// pan of -1.0 indicates all left speaker, 
		// 1.0 is all right speaker, 0.0 is split between left and right
		pan = -0.00625f;
		left = 0.5f - pan / 2;
		right = 0.5f + pan / 2;
		switch (dwChannelMask)
		{
		case SPEAKER_MONO:
			outputMatrix2[0] = 1.0;
			break;
		case SPEAKER_STEREO:
		case SPEAKER_2POINT1:
		case SPEAKER_SURROUND:
			outputMatrix2[0] = left;
			outputMatrix2[1] = right;
			break;
		case SPEAKER_QUAD:
			outputMatrix2[0] = outputMatrix2[2] = left;
			outputMatrix2[1] = outputMatrix2[3] = right;
			break;
		case SPEAKER_4POINT1:
			outputMatrix2[0] = outputMatrix2[3] = left;
			outputMatrix2[1] = outputMatrix2[4] = right;
			break;
		case SPEAKER_5POINT1:
		case SPEAKER_7POINT1:
		case SPEAKER_5POINT1_SURROUND:
			outputMatrix2[0] = outputMatrix2[4] = left;
			outputMatrix2[1] = outputMatrix2[5] = right;
			break;
		case SPEAKER_7POINT1_SURROUND:
			outputMatrix2[0] = outputMatrix2[4] = outputMatrix2[6] = left;
			outputMatrix2[1] = outputMatrix2[5] = outputMatrix2[7] = right;
			break;
		}
		// Assuming pVoice sends to pMasteringVoice

		XAUDIO2_VOICE_DETAILS VoiceDetails2;
		source_voice2->GetVoiceDetails(&VoiceDetails2);

		if (FAILED(source_voice2->SetOutputMatrix(submix_voice, VoiceDetails2.InputChannels, 2, outputMatrix2)))
		{
			dout + "FAILED IN SetOutputMatrix 2";
		}
		// -------------------------------------- END


	//	if (FAILED(source_voice->Start(0)))
	//	{
	//		throw std::exception{ "failed in IXAudio2SourceVoice::Start" };
	//	}
		if (FAILED(source_voice2->Start(0)))
		{
			throw std::exception{ "failed in IXAudio2SourceVoice::Start" };
		}
		
		::HRESULT err{};
		/*
		float vols[8]{ 1.0f, 1.0f };
		err = source_voice->SetChannelVolumes(VoiceDetails.InputChannels, vols); // 设置 Channels 的 Volume
		if (FAILED(err))
		{
			dout + "FAILED IN IF 1 BRANCH" + ln;
		}
		else dout + "SUCCEED IN IF 1 BRANCH" + ln;
		float vols2[8]{ 1.0f, 1.0f };
		err = source_voice2->SetChannelVolumes(VoiceDetails2.InputChannels, vols2); // 设置 Channels 的 Volume
		if (FAILED(err))
		{
			dout + "FAILED IN IF 2 BRANCH" + ln;
		}
		else dout + "SUCCEED IN IF 2 BRANCH" + ln;
		*/

		// 从音频图删除source voice 2 到 submix voice 的节点，不过现在好像没啥用
	//	XAUDIO2_SEND_DESCRIPTOR sfx_send2{ 0, master_voice };
	//	XAUDIO2_VOICE_SENDS sfx_send_list2{ 1, &sfx_send };
	//	if (FAILED(source_voice2->SetOutputVoices(&sfx_send_list2)))
	//	{
	//		throw std::exception{ "failed in IXAudio2SourceVoice::SetOutputVoices" };
	//	}

		/*
		float vols_s[2]{ 1.0f, 0.25f };
		err = submix_voice->SetChannelVolumes(2, vols_s);
		if (FAILED(err))
		{
			dout + "FAILED IN IF S BRANCH" + ln;
		}
		else dout + "SUCCEED IN IF S BRANCH" + ln;
		*/

		//	XAudio2FrequencyRatioToSemitones(1);
		//	source_voice->SetFrequencyRatio(XAudio2SemitonesToFrequencyRatio(1.f)); /* 变调 升高一个半音 */


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
			
			if (i == 3)
			{
				float vols[2]{ 1.25f, 0.125 };
				auto err = submix_voice->SetChannelVolumes(2, vols); // 设置 Channels 的 Volume
				if (FAILED(err))
				{
					dout + "FAILED IN IF BRANCH" + ln;
				}
				else dout + "SUCCEED IN IF BRANCH" + ln;

				if (FAILED(source_voice3->Start()))
				{
					throw std::exception{ "failed in IXAudio2SourceVoice::Start (3)" };
				}
			}
			
		}
		std::this_thread::sleep_for(std::chrono::milliseconds{ 1000 });

		std::cout << "||\n end\n";
		wait_thread.join();
	}
};

class XAudio2Demo
{
public:
	static int main_02() try
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
		FileInterface fi, fi2;
		std::wstring filename, filename2;
		std::cout << "请输入要播放的.WAV文件: ";
		std::wcin.imbue(std::locale{ "chs" });
		std::wcin >> filename;
		std::cout << "请输入要播放的.WAV文件: ";
		std::wcin >> filename2;

		::WAVEFORMATEXTENSIBLE format;
		::XAUDIO2_BUFFER buffer;
		fi.ReadRIFFWave(filename, format, buffer);
		::WAVEFORMATEXTENSIBLE format2;
		::XAUDIO2_BUFFER buffer2;
		fi2.ReadRIFFWave(filename2, format2, buffer2);

		AudioInterface ai;
		ai.Play2(format, buffer, format2, buffer2);

		return 0;
	}
	catch (const std::exception& e)
	{
		std::cout << "! exception: " << e.what() << "\n";
		return 0;
	}
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
		::WAVEFORMATEXTENSIBLE format;
		::XAUDIO2_BUFFER buffer;
		fi.ReadRIFFWave(filename, format, buffer);
		AudioInterface ai;
		try
		{
			ai.Play(format, buffer);
		}
		catch (const std::exception& e)
		{
			std::cout << "! exception: " << e.what() << "\n";
			return 0;
		}
		return 0;
	}
	catch (const std::exception& e)
	{
		std::cout << "! exception: " << e.what() << "\n";
		return 0;
	}
	catch (...)
	{
		std::cout << "! exception: Unknown" << "\n";
		return 0;
	}
};

// Petal_SetMainClass(XAudio2Demo)
