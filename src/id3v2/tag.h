#pragma once
#include <memory>
#include <string>
#include <vector>
#include "../winlamb/lib.h"
#include "frame.h"

namespace id3v2 {

	// Each MP3 file has one tag.
	class Tag final {
	public:
		Tag() = delete;
		DEL_COPY(Tag);
		DEF_MOVE(Tag);

		explicit Tag(wl::WStrPtr mp3File);
		explicit Tag(std::span<BYTE> src) { parse(src); }

		[[nodiscard]] constexpr size_t mp3_offset() const { return _mp3Offset; }
		[[nodiscard]] constexpr size_t padding() const    { return _padding; }

		void add_frame_with_text(wl::WStrPtr name4, wl::WStrPtr text);
		[[nodiscard]] Frame* frame_by_name4(wl::WStrPtr name4);

	private:
		void parse(std::span<BYTE>);
		size_t parse_header(std::span<BYTE> src);
		void parse_frames(std::span<BYTE> src);

		size_t _mp3Offset = 0;
		size_t _padding = 0;
		std::vector<std::unique_ptr<Frame>> _frames{};
	};

}
