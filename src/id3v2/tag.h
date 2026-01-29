#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "../winlamb/winlamb.hpp"
#include "frame.h"

namespace id3v2 {

	/** Each MP3 file has one tag. */
	class Tag final : wl::NoCopyNoMove {
	public:
		explicit Tag(wl::WStrView mp3File);
		explicit Tag(std::span<BYTE> src) { parse(src); }

		[[nodiscard]] constexpr size_t mp3_offset() const { return _mp3Offset; }
		[[nodiscard]] constexpr size_t padding() const    { return _padding; }

		void add_frame_with_text(wl::WStrView name4, wl::WStrView text);
		[[nodiscard]] const Frame* frame_by_name4(wl::WStrView name4) const;
		[[nodiscard]] Frame* frame_by_name4(wl::WStrView name4);
		void remove_frame_if(std::function<bool(const Frame&)> cb);
		[[nodiscard]] LPCWSTR replay_gain_status() const;
		[[nodiscard]] std::vector<BYTE> serialize() const;
		void save_to_file(wl::WStrView mp3File);

	private:
		void parse(std::span<BYTE>);
		size_t parse_header(std::span<BYTE> src);
		void parse_frames(std::span<BYTE> src);

		size_t _mp3Offset = 0;
		size_t _padding = 0;
		std::vector<std::unique_ptr<Frame>> _frames{};
	};

}
