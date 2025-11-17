#include <stdexcept>
#include "tag.h"
#include "util.h"
using std::function;
using std::span;
using std::unique_ptr;
using std::vector;
using std::wstring;
using namespace id3v2;

Tag::Tag(wl::WStrView mp3File) {
	const wl::FileMapped fin{mp3File, wl::FileMapped::Access::existing_read_only};
	parse(fin.view());
}

void Tag::parse(span<BYTE> src) {
	size_t declaredSize = parse_header(src);
	if (!declaredSize) return; // no tag

	parse_frames(src.subspan(10)); // skip 10-byte tag header
}

void Tag::add_frame_with_text(wl::WStrView name4, wl::WStrView text) {
	unique_ptr<Frame> frame = Frame::new_simple_text(name4, text);
	_frames.emplace_back(std::move(frame));
}

const Frame* Tag::frame_by_name4(wl::WStrView name4) const {
	for (auto &&frame : _frames) {
		if (wl::str::eq_i(frame->name4(), name4))
			return frame.get();
	}
	return nullptr;
}

Frame* Tag::frame_by_name4(wl::WStrView name4) {
	return const_cast<Frame*>(std::as_const(*this).frame_by_name4(name4)); // https://stackoverflow.com/a/47369227/6923555
}

void Tag::remove_frame_if(function<bool(const Frame&)> cb) {
	wl::vec::remove_if(_frames, [cb = std::move(cb)](const unique_ptr<Frame>& f) {
		return cb(*f.get());
	});
}

LPCWSTR Tag::replay_gain_status() const {
	bool hasTrack = false, hasAlbum = false;
	for (auto &&frame : _frames) {
		if (hasTrack && hasAlbum)
			break;

		if (wl::str::eq_i(frame->name4(), L"TXXX")) {
			if (FrameUserText *pUserText = dynamic_cast<FrameUserText*>(frame.get()); pUserText) {
				wstring descr = wl::str::to_lower(pUserText->descr());
				if (wl::str::starts_with(descr, L"replaygain_track_")) {
					hasTrack = true;
				} else if (wl::str::starts_with(descr, L"replaygain_album_")) {
					hasAlbum = true;
				}
			}
		}
	}

	if (hasTrack && hasAlbum) return L"TA";
	else if (hasTrack) return L"T";
	else if (hasAlbum) return L"A";
	else return L"";
}

vector<BYTE> Tag::serialize() const {
	size_t szTag = 10; // start with 10-byte tag header
	for (auto &&frame : _frames)
		szTag += frame->serializable_size() + 10; // plus 10-byte frame header

	vector<BYTE> blob{};
	blob.reserve(szTag);

	wl::vec::append(blob, {'I', 'D', '3'}); // magic bytes
	wl::vec::append(blob, {0x03, 0x00}); // tag version
	blob.emplace_back(0x00); // flags

	UINT szTagSeri = synch_safe::encode(static_cast<DWORD>(szTag) - 10); // don't count 10-byte tag header
	conv::uint_serialize_be(blob, szTagSeri);

	for (auto &&frame : _frames)
		frame->serialize(blob);

	return blob;
}

void Tag::save_to_file(wl::WStrView mp3File) {
	wl::File fout{mp3File, wl::File::Access::existing_rw};
	vector<BYTE> currentContents = fout.read_all(); // copy the whole file into memory
	Tag oldTag{currentContents}; // so we can have the MP3 offset
	fout.truncate();

	if (!_frames.empty()) {
		vector<BYTE> tagBlob = serialize();
		fout.write(tagBlob);
	}

	fout.write(currentContents.begin() + oldTag.mp3_offset(), currentContents.end());
	_padding = 0; // we write no padding
}

size_t Tag::parse_header(span<BYTE> src) {
	// Check ID3 magic bytes.
	if (!wl::vec::eq(src.subspan(0, 3), {'I', 'D', '3'})) {
		return 0; // MP3 has no tag
	}

	// Validate tag version 2.3.0.
	if (!wl::vec::eq(src.subspan(3, 2), {3, 0})) [[unlikely]] { // the first "2" is not stored
		throw std::runtime_error(wl::str::to_ansi(
			wl::str::fmt(L"Tag version 2.%d.%d not supported, only 2.3.0.", src[3], src[4])));
	}

	// Validate unsupported flags.
	if (src[5] & 0b1000'0000) [[unlikely]] {
		throw std::runtime_error("Unsynchronised tag not supported.");
	} else if (src[5] & 0b0100'0000) [[unlikely]] {
		throw std::runtime_error("Tag extended header not supported.");
	}

	// Read declared tag size.
	size_t declaredSize = synch_safe::decode(conv::uint_from_be_bytes(src.subspan(6, 4)));

	return declaredSize + 10; // also count 10-byte tag header
}

[[nodiscard]] static constexpr bool is_mp3_magic(std::span<BYTE> src) {
	// Known magic byte sequences that identify the beginning of a MP3.
	// https://stackoverflow.com/a/7302482/6923555
	// https://en.wikipedia.org/wiki/List_of_file_signatures
	// https://github.com/sindresorhus/file-type/issues/75#issuecomment-320650344
	const BYTE MP3_MAGIC[][2] = {{0xff, 0xfb}, {0xff, 0xfb}, {0xff, 0xf2}, {0xff, 0xfa}, {0xff, 0xf3}};
	for (size_t i = 0; i < ARRAYSIZE(MP3_MAGIC); ++i) {
		if (wl::vec::eq(src.subspan(0, 2), MP3_MAGIC[i]))
			return true;
	}
	return false;
}

void Tag::parse_frames(span<BYTE> src) {
	_mp3Offset = 10; // start with the tag header
	for (;;) {
		if (is_mp3_magic(src))
			return;// we found the beginning of the MP3 file, no padding

		if (src[0] == 0x00) { // we entered a padding region after all frames
			for (size_t i = 1; i < src.size() - 1; ++i) { // skip the 1st byte, which is 0x00; don't count last, we're checking 2
				if (is_mp3_magic(src.subspan(i))) {
					_mp3Offset += i;
					return;
				}
			}
			throw std::logic_error("MP3 offset not found.");
		}

		unique_ptr<Frame> frame = Frame::parse(src);
		if (frame->declared_size() > src.size()) [[unlikely]] { // means the size was serialized with error
			throw std::runtime_error(wl::str::to_ansi(
				wl::str::fmt(L"Declared frame size greater than block: %d vs %d",
					frame->declared_size(), src.size())));
		}

		_mp3Offset += frame->declared_size();
		src = src.subspan(frame->declared_size());
		_frames.emplace_back(std::move(frame));
	}
}
