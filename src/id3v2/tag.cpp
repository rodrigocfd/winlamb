#include <stdexcept>
#include "tag.h"
#include "util.h"
using std::span;
using std::unique_ptr;
using namespace id3v2;

Tag::Tag(wl::WStrPtr mp3File) {
	const wl::FileMapped fin{mp3File, wl::FileMapped::Access::ExistingReadOnly};
	parse(fin.view());
}

void Tag::parse(span<BYTE> src) {
	size_t declaredSize = parse_header(src);
	if (!declaredSize) return; // no tag

	parse_frames(src.subspan(10)); // skip 10-byte tag header
}

void Tag::add_frame_with_text(wl::WStrPtr name4, wl::WStrPtr text) {
	unique_ptr<Frame> frame = Frame::new_simple_text(name4, text);
	_frames.emplace_back(std::move(frame));
}

Frame* Tag::frame_by_name4(wl::WStrPtr name4) {
	for (auto& frame : _frames) {
		if (wl::str::eq_i(frame->name4(), name4))
			return frame.get();
	}
	return nullptr;
}

size_t Tag::parse_header(span<BYTE> src) {
	// Check ID3 magic bytes.
	if (!wl::vec::same(src.subspan(0, 3), {'I', 'D', '3'})) {
		return 0; // MP3 has no tag
	}

	// Validate tag version 2.3.0.
	if (!wl::vec::same(src.subspan(3, 3), {3, 0})) [[unlikely]] { // the first "2" is not stored
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
		if (wl::vec::same(src.subspan(0, 2), MP3_MAGIC[i]))
			return true;
	}
	return false;
}

void Tag::parse_frames(span<BYTE> src) {
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
