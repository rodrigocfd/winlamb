#include "util.h"
using std::initializer_list;
using std::span;
using std::vector;
using std::wstring_view;
using std::wstring;
using namespace id3v2;

constexpr static const WORD BOM_BE = 0xfeff;
constexpr static const WORD BOM_LE = 0xfffe;

wstring id3v2::str_engine::parse_str_ascii(span<BYTE> src) {
	wstring s(src.size() + 1, L'\0');
	for (size_t i = 0; i < src.size(); ++i)
		s[i] = src[i];
	return s;
}

void id3v2::str_engine::serialize_str_ascii(vector<BYTE> &dest, wl::WStrPtr s) {
	size_t len = s.length();
	for (size_t i = 0; i < len; ++i) // won't serialize terminating null
		dest.emplace_back(static_cast<BYTE>(s[i]));
}

Enc id3v2::str_engine::parse_enc(span<BYTE> &src) {
	Enc encByte = static_cast<Enc>(src[0]);
	if (encByte != Enc::iso88591 && encByte != Enc::unicode) [[unlikely]] {
		wstring msg = wl::str::fmt(L"Unknown encoding: %d.", encByte);
		throw std::runtime_error(wl::str::to_ansi(msg));
	}
	src = src.subspan(1);
	return encByte;
}

Enc id3v2::str_engine::serializable_enc(initializer_list<wl::WStrPtr> strs) {
	for (auto str : strs) {
		for (auto ch : wstring_view{str}) {
			if (ch > 0xff)
				return Enc::unicode;
		}
	}
	return Enc::iso88591;
}

void id3v2::str_engine::serialize_enc(vector<BYTE> &dest, Enc encByte) {
	dest.emplace_back(static_cast<BYTE>(encByte));
}

wstring id3v2::str_engine::parse_str(Enc encByte, span<BYTE> &src) {
	switch (encByte) {
		case Enc::iso88591: {
			size_t idxZero = wl::vec::index(src, 0x00).value_or(src.size()); // if no zero byte, simply consider the whole slice
			wstring s = parse_str_iso88591(src.subspan(0, idxZero));
			src = src.subspan(std::min(src.size(), idxZero + 1));
			return s;
		}
		case Enc::unicode: {
			span<WORD> wsrc{reinterpret_cast<WORD*>(src.data()), src.size() / 2}; // will discard an odd byte
			size_t idxZero = wl::vec::index(wsrc, 0x0000).value_or(wsrc.size()); // if no zero word, simply consider the whole slice
			wstring s = parse_str_unicode(wsrc);
			src = src.subspan(std::min(src.size(), (idxZero + 1) * 2));
			return s;
		}
		[[unlikely]] default: {
			wstring msg = wl::str::fmt(L"Unknown encoding: %d.", encByte);
			throw std::runtime_error(wl::str::to_ansi(msg)); // should never happen
		}
	}
}

wstring id3v2::str_engine::parse_str_iso88591(span<BYTE> src) {
	if (src.empty()) return {};

	wstring buf(src.size(), L'\0');
	for (UINT i = 0; i < src.size(); ++i) {
		buf[i] = src[i];
	}
	return buf;
}

wstring id3v2::str_engine::parse_str_unicode(span<WORD> src) {
	bool isLE = true;
	if (src[0] == BOM_LE || src[0] == BOM_BE) { // we have a BOM
		if (src[0] == BOM_BE) isLE = false;
		src = src.subspan(1); // skip BOM
	}

	if (src.empty()) return {};

	wstring buf(src.size(), L'\0');
	for (UINT i = 0; i < src.size(); ++i) {
		buf[i] = isLE ?  MAKEWORD(HIWORD(src[i]), LOWORD(src[i])) : src[i];
	}
	return buf;
}

size_t id3v2::str_engine::serializable_size(Enc encByte, wl::WStrPtr s) {
	switch (encByte) {
	case Enc::iso88591:
		return s.length() + 1; // plus terminating null
	case Enc::unicode:
		return (s.length() + 1 + 1) * 2; // plus BOM and terminating null
	[[unlikely]] default:
		return 0; // never happens
	}
}

void id3v2::str_engine::serialize_str(Enc encByte, vector<BYTE> &dest, wl::WStrPtr s) {
	if (encByte == Enc::unicode)
		wl::vec::append(dest, LOBYTE(BOM_LE), HIBYTE(BOM_LE)); // BOM bytes serialized as little-endian

	for (auto ch : wstring_view{s}) {
		switch (encByte) {
		case Enc::iso88591:
			dest.emplace_back(static_cast<BYTE>(ch));
			break;
		case Enc::unicode:
			wl::vec::append(dest, LOBYTE(ch), HIBYTE(ch)); // 2 bytes, little endian
		}
	}

	switch (encByte) {
	case Enc::iso88591:
		dest.emplace_back(0x00);
		break;
	case Enc::unicode:
		wl::vec::append(dest, 0x00, 0x00);
	}
}

UINT id3v2::synch_safe::encode(UINT num) {
	int out, mask = 0x7f;
	while (mask ^ 0x7fff'ffff) {
		out = num & ~mask;
		out <<= 1;
		out |= num & mask;
		mask = ((mask + 1) << 8) - 1;
		num = out;
	}
	return out;
}

UINT id3v2::synch_safe::decode(UINT num) {
	int out = 0, mask = 0x7f00'0000;
	while (mask) {
		out >>= 1;
		out |= num & mask;
		mask >>= 8;
	}
	return out;
}

UINT id3v2::conv::uint_from_be_bytes(span<BYTE> src) {
	if (src.size() < 4) [[unlikely]] {
		throw std::invalid_argument("UINT must be converted from at least 4 bytes.");
	}
	return MAKELONG(MAKEWORD(src[3], src[2]), MAKEWORD(src[1], src[0]));
}
