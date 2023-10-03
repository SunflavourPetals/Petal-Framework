#include "Petal~String.h"

namespace Petal
{
	const String null_str{ "" };
	const WString null_wstr{ L"" };
	const StringU8 null_u8str{ u8"" };
	const StringU16 null_u16str{ u"" };
	const StringU32 null_u32str{ U"" };
#if defined(Petal_Enable_Unicode)
	const TString& null_tstr{ null_wstr };
#else
	const TString& null_tstr{ null_str };
#endif
}
