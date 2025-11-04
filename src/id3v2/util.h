#pragma once
#include <span>
#include <string>
#include <vector>
#include "../winlamb/lib.h"

namespace id3v2 {

	/** String encoding. */
	enum class Enc : BYTE {
		iso88591 = 0x00,
		unicode  = 0x01,
	};

}

namespace id3v2::str_engine {

	[[nodiscard]] std::wstring parse_str_ascii(std::span<BYTE> src);
	void serialize_str_ascii(std::vector<BYTE> &dest, wl::WStrPtr s);

	[[nodiscard]] Enc parse_enc(std::span<BYTE> &src);
	[[nodiscard]] Enc serializable_enc(std::initializer_list<wl::WStrPtr> strs);
	void serialize_enc(std::vector<BYTE> &dest, Enc encByte);

	[[nodiscard]] std::wstring parse_str(Enc encByte, std::span<BYTE> &src);
	[[nodiscard]] std::wstring parse_str_iso88591(std::span<BYTE> src);
	[[nodiscard]] std::wstring parse_str_unicode(std::span<WORD> src);
	[[nodiscard]] size_t serializable_size(Enc encByte, wl::WStrPtr s);
	void serialize_str(Enc encByte, std::vector<BYTE> &dest, wl::WStrPtr s);

}

namespace id3v2::synch_safe {

	[[nodiscard]] UINT encode(UINT num);
	[[nodiscard]] UINT decode(UINT num);

}

namespace id3v2::conv {

	[[nodiscard]] UINT uint_from_be_bytes(std::span<BYTE> src);
	void uint_serialize_be(std::vector<BYTE> &dest, UINT n);

}
