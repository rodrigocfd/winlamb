#include <stdexcept>
#include "frame.h"
#include "util.h"
using std::array;
using std::span;
using std::unique_ptr;
using std::vector;
using std::wstring;
using namespace id3v2;

// Descriptions for PicType enumeration.
const LPCWSTR id3v2::PIC_TYPE_NAMES[] = {
	L"Other",
	L"32x32 pixels 'file icon' (PNG only)",
	L"Other file icon",
	L"Cover (front)",
	L"Cover (back)",
	L"Leaflet page",
	L"Media (e.g. label side of CD)",
	L"Lead artist/lead performer/soloist",
	L"Artist/performer",
	L"Conductor",
	L"Band/Orchestra",
	L"Composer",
	L"Lyricist/text writer",
	L"Recording Location",
	L"During recording",
	L"During performance",
	L"Movie/video screen capture",
	L"A bright coloured fish",
	L"Illustration",
	L"Band/artist logotype",
	L"Publisher/Studio logotype",
};

////////////////////////////////////////////////////////////////////////////////

unique_ptr<Frame> Frame::parse(span<BYTE> src) {
	wstring name4 = str_engine::parse_str_ascii(src.subspan(0, 4));

	UINT declaredSize = conv::uint_from_be_bytes(src.subspan(4, 4)) + 10; // also count 10-byte tag header
	array<BYTE, 2> flags{{src[8], src[9]}};

	if (declaredSize > src.size()) [[unlikely]] {
		declaredSize = static_cast<UINT>(src.size()); // if serialized with error, be complacent
	}

	src = src.subspan(10, declaredSize - 10); // skip frame header, truncate to frame size

	if (wl::str::eq_i(name4, L"COMM")) {
		return std::make_unique<FrameComment>(std::move(name4), declaredSize, flags, src);
	} else if (wl::str::eq_i(name4, L"APIC")) {
		return std::make_unique<FramePicture>(std::move(name4), declaredSize, flags, src);
	} else if (wl::str::eq_i(name4, L"GEOB")) {
		return std::make_unique<FrameGeob>(std::move(name4), declaredSize, flags, src);
	} else if (wl::str::eq_i(name4, L"TXXX")) {
		return std::make_unique<FrameUserText>(std::move(name4), declaredSize, flags, src);
	} else if (wl::str::starts_with_i(name4, L"T") && !wl::str::eq_i(name4, L"TDAT")) {
		return std::make_unique<FrameText>(std::move(name4), declaredSize, flags, src);
	} else { // everything else is treated as raw binary
		return std::make_unique<FrameBinary>(std::move(name4), declaredSize, flags, src);
	}
}

unique_ptr<Frame> Frame::new_simple_text(wl::WStrPtr name4, wl::WStrPtr text) {
	if (wl::str::eq_i(name4, L"COMM")) {
		return std::make_unique<FrameComment>(wstring{name4}, text);
	} else if (wl::str::starts_with_i(name4, L"T") && !wl::str::eq_i(name4, L"TDAT")) {
		return std::make_unique<FrameText>(wstring{name4}, text);
	} else {
		throw std::invalid_argument(wl::str::to_ansi(
			wl::str::fmt(L"Cannot create simple text frame for %s.", name4)));
	}
}

void Frame::serialize_header(vector<BYTE> &dest) const {
	str_engine::serialize_str_ascii(dest, _name4);

	size_t szBody = serializable_size(); // don't count 10-byte header
	conv::serialize_be(dest, static_cast<DWORD>(szBody));

	wl::vec::append(dest, _flags);
}

////////////////////////////////////////////////////////////////////////////////

FrameText::FrameText(wstring &&name4, UINT declaredSize, array<BYTE, 2> flags, span<BYTE> src)
	: Frame{std::move(name4), declaredSize, flags}
{
	Enc encByte = str_engine::parse_enc(src);
	_text = str_engine::parse_str(encByte, src);
}

size_t FrameText::serializable_size() const {
	Enc encByte = str_engine::serializable_enc({_text});
	size_t szText = str_engine::serializable_size(encByte, _text);
	return 1 + szText;
}

void FrameText::serialize(vector<BYTE> &dest) const {
	serialize_header(dest);
	Enc encByte = str_engine::serializable_enc({_text});
	str_engine::serialize_enc(dest, encByte);
	str_engine::serialize_str(encByte, dest, _text);
}

////////////////////////////////////////////////////////////////////////////////

FrameUserText::FrameUserText(wstring &&name4, UINT declaredSize, array<BYTE, 2> flags, span<BYTE> src)
	: Frame{std::move(name4), declaredSize, flags}
{
	Enc encByte = str_engine::parse_enc(src);
	_descr = str_engine::parse_str(encByte, src);
	_text = str_engine::parse_str(encByte, src);
}

wstring FrameUserText::as_simple_text() const {
	return _descr.empty() ? _text : wl::str::fmt(L"%s %s", _descr.c_str(), _text.c_str());
}

void FrameUserText::force_simple_text(wl::WStrPtr text) {
	_descr = L"";
	_text = text;
}

size_t FrameUserText::serializable_size() const {
	Enc encByte = str_engine::serializable_enc({_descr, _text});
	size_t szDescr = str_engine::serializable_size(encByte, _descr);
	size_t szText = str_engine::serializable_size(encByte, _text);
	return 1 + szDescr + szText;
}

void FrameUserText::serialize(vector<BYTE> &dest) const {
	serialize_header(dest);
	Enc encByte = str_engine::serializable_enc({_descr, _text});
	str_engine::serialize_enc(dest, encByte);
	str_engine::serialize_str(encByte, dest, _descr);
	str_engine::serialize_str(encByte, dest, _text);
}

////////////////////////////////////////////////////////////////////////////////

FrameBinary::FrameBinary(wstring &&name4, UINT declaredSize, array<BYTE, 2> flags, span<BYTE> src)
	: Frame{std::move(name4), declaredSize, flags}
{
	Enc encByte = str_engine::parse_enc(src);
	vector(src.begin(), src.end()).swap(_bin);
}

wstring FrameBinary::as_simple_text() const {
	return wl::str::fmt_bytes(_bin.size());
}

void FrameBinary::force_simple_text(wl::WStrPtr text) {
	throw std::logic_error("Cannot force text to binary frame.");
}

void FrameBinary::serialize(vector<BYTE> &dest) const {
	serialize_header(dest);
	wl::vec::append(dest, _bin);
}

////////////////////////////////////////////////////////////////////////////////

FrameComment::FrameComment(wstring &&name4, UINT declaredSize, array<BYTE, 2> flags, span<BYTE> src)
	: Frame{std::move(name4), declaredSize, flags}
{
	Enc encByte = str_engine::parse_enc(src);

	_lang3 = str_engine::parse_str_ascii(src.subspan(0, 3));
	src = src.subspan(3);

	_descr = str_engine::parse_str(encByte, src);
	_text = str_engine::parse_str(encByte, src);
}

wstring FrameComment::as_simple_text() const {
	return _descr.empty() ? _text : wl::str::fmt(L"%s %s", _descr.c_str(), _text.c_str());
}

void FrameComment::force_simple_text(wl::WStrPtr text) {
	lstrcpyW(_lang3.data(), L"");
	_descr = L"";
	_text = text;
}

size_t FrameComment::serializable_size() const {
	Enc encByte = str_engine::serializable_enc({_descr, _text});
	size_t szDescr = str_engine::serializable_size(encByte, _descr);
	size_t szText = str_engine::serializable_size(encByte, _text);
	return 1 + 3 + szDescr + szText;
}

void FrameComment::serialize(vector<BYTE> &dest) const {
	serialize_header(dest);
	Enc encByte = str_engine::serializable_enc({_descr, _text});
	str_engine::serialize_enc(dest, encByte);
	str_engine::serialize_str_ascii(dest, _lang3);
	str_engine::serialize_str(encByte, dest, _descr);
	str_engine::serialize_str(encByte, dest, _text);
}

////////////////////////////////////////////////////////////////////////////////

FramePicture::FramePicture(wstring &&name4, UINT declaredSize, array<BYTE, 2> flags, span<BYTE> src)
	: Frame{std::move(name4), declaredSize, flags}
{
	Enc encByte = str_engine::parse_enc(src);
	_mime = str_engine::parse_str(Enc::iso88591, src);

	_picType = static_cast<PicType>(src[0]);
	src = src.subspan(1);

	_descr = str_engine::parse_str(encByte, src);
	vector(src.begin(), src.end()).swap(_bin);
}

wstring FramePicture::as_simple_text() const {
	return wl::str::fmt(L"%s %s %s",
		PIC_TYPE_NAMES[static_cast<BYTE>(_picType)], _mime, wl::str::fmt_bytes(_bin.size()));
}

void FramePicture::force_simple_text(wl::WStrPtr text) {
	throw std::logic_error("Cannot force text to picture frame.");
}

size_t FramePicture::serializable_size() const {
	Enc encByte = str_engine::serializable_enc({_descr});
	size_t szMime = str_engine::serializable_size(Enc::iso88591, _mime);
	size_t szDescr = str_engine::serializable_size(encByte, _descr);
	return 1 + szMime + 1 + szDescr + _bin.size();
}

void FramePicture::serialize(vector<BYTE> &dest) const {
	serialize_header(dest);
	Enc encByte = str_engine::serializable_enc({_descr});
	str_engine::serialize_enc(dest, encByte);
	str_engine::serialize_str(Enc::iso88591, dest, _mime);
	dest.emplace_back(static_cast<BYTE>(_picType));
	str_engine::serialize_str(encByte, dest, _descr);
	wl::vec::append(dest, _bin);
}

////////////////////////////////////////////////////////////////////////////////

FrameGeob::FrameGeob(wstring &&name4, UINT declaredSize, array<BYTE, 2> flags, span<BYTE> src)
	: Frame{std::move(name4), declaredSize, flags}
{
	Enc encByte = str_engine::parse_enc(src);
	_mime = str_engine::parse_str(Enc::iso88591, src);
	_fileName = str_engine::parse_str(encByte, src);
	_descr = str_engine::parse_str(encByte, src);
	vector(src.begin(), src.end()).swap(_encObj);
}

wstring FrameGeob::as_simple_text() const {
	return wl::str::fmt(L"%s %s %s",
		_mime, _descr, wl::str::fmt_bytes(_encObj.size()));
}

void FrameGeob::force_simple_text(wl::WStrPtr text) {
	throw std::logic_error("Cannot force text to general encapsulated object frame.");
}

size_t FrameGeob::serializable_size() const {
	Enc encByte = str_engine::serializable_enc({_fileName, _descr});
	size_t szMime = str_engine::serializable_size(Enc::iso88591, _mime);
	size_t szFileName = str_engine::serializable_size(encByte, _fileName);
	size_t szDescr = str_engine::serializable_size(encByte, _descr);
	return 1 + szMime + szFileName + szDescr + _encObj.size();
}

void FrameGeob::serialize(vector<BYTE> &dest) const {
	serialize_header(dest);
	Enc encByte = str_engine::serializable_enc({_fileName, _descr});
	str_engine::serialize_enc(dest, encByte);
	str_engine::serialize_str(Enc::iso88591, dest, _mime);
	str_engine::serialize_str(encByte, dest, _fileName);
	str_engine::serialize_str(encByte, dest, _descr);
	wl::vec::append(dest, _encObj);
}
