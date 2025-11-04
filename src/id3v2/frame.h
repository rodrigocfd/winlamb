#pragma once
#include <memory>
#include "../winlamb/lib.h"

namespace id3v2 {

	/** FramePicture types. */
	enum class PicType : BYTE {
		other = 0x00,
		file_icon_png_32,
		file_icon_other,
		cover_front,
		cover_back,
		leaflet_page,
		cd_label_side,
		lead_artist,
		artist,
		conductor,
		band,
		composer,
		lyricist,
		rec_location,
		during_recording,
		during_performance,
		movie_capture,
		bright_coloured_fish,
		illustration,
		band_logo,
		publisher_logo,
	};

	/** Descriptions for PicType enumeration. */
	extern const LPCWSTR PIC_TYPE_NAMES[];

	/////////////////////////////////////////////////////////////////////////////

	/** Pure abstract class, base to all frames. */
	class Frame {
	public:
		virtual ~Frame() { }

	protected:
		Frame(std::wstring &&name4, UINT declaredSize, std::array<BYTE, 2> flags)
			: _name4{std::move(name4)}, _declaredSize{declaredSize}, _flags{flags} { }

	public:
		/** Static method; returns a new polymorphic Frame. */
		static std::unique_ptr<Frame> parse(std::span<BYTE> src);

		/** Static method; tries to create a new polymorphic frame with the simple text. */
		static std::unique_ptr<Frame> new_simple_text(wl::WStrPtr name4, wl::WStrPtr text);

		[[nodiscard]] constexpr LPCWSTR name4() const             { return _name4.c_str(); }
		[[nodiscard]] constexpr UINT declared_size() const        { return _declaredSize; }
		[[nodiscard]] constexpr std::array<BYTE, 2> flags() const { return _flags; }

		[[nodiscard]] virtual std::wstring as_simple_text() const = 0;
		virtual void force_simple_text(wl::WStrPtr text) = 0;
		[[nodiscard]] virtual size_t serializable_size() const = 0;
		virtual void serialize(std::vector<BYTE> &dest) const = 0;

	protected:
		void serialize_header(std::vector<BYTE> &dest) const;

		std::wstring _name4{};
		UINT _declaredSize = 0; // used only at tag parsing
		std::array<BYTE, 2> _flags{};
	};

	/////////////////////////////////////////////////////////////////////////////

	class FrameText final : public Frame {
	public:
		virtual ~FrameText() { }

		FrameText(std::wstring &&name4, UINT declaredSize, std::array<BYTE, 2> flags, std::span<BYTE> src);
		FrameText(std::wstring &&name4, wl::WStrPtr simpleText)
			: Frame{std::move(name4), 0, {{0, 0}}}, _text{simpleText} { }

		[[nodiscard]] std::wstring as_simple_text() const override { return _text; }
		void force_simple_text(wl::WStrPtr text) override          { _text = text; }
		[[nodiscard]] size_t serializable_size() const override;
		void serialize(std::vector<BYTE> &dest) const override;

		[[nodiscard]] constexpr LPCWSTR text() const { return _text.c_str(); }
		void set_text(wl::WStrPtr text)              { _text = text; }

	private:
		std::wstring _text{};
	};

	/////////////////////////////////////////////////////////////////////////////

	class FrameUserText final : public Frame {
	public:
		virtual ~FrameUserText() { }

		FrameUserText(std::wstring &&name4, UINT declaredSize, std::array<BYTE, 2> flags, std::span<BYTE> src);

		[[nodiscard]] std::wstring as_simple_text() const override;
		void force_simple_text(wl::WStrPtr text) override;
		[[nodiscard]] size_t serializable_size() const override;
		void serialize(std::vector<BYTE> &dest) const override;

		[[nodiscard]] constexpr LPCWSTR descr() const { return _descr.c_str(); }
		void set_descr(wl::WStrPtr descr)             { _descr = descr; }
		[[nodiscard]] constexpr LPCWSTR text() const  { return _text.c_str(); }
		void set_text(wl::WStrPtr text)               { _text = text; }

	private:
		std::wstring _descr{};
		std::wstring _text{};
	};

	/////////////////////////////////////////////////////////////////////////////

	class FrameBinary final : public Frame {
	public:
		virtual ~FrameBinary() { }

		FrameBinary(std::wstring &&name4, UINT declaredSize, std::array<BYTE, 2> flags, std::span<BYTE> src);

		[[nodiscard]] std::wstring as_simple_text() const override;
		void force_simple_text(wl::WStrPtr text) override;
		[[nodiscard]] size_t serializable_size() const override { return _bin.size(); }
		void serialize(std::vector<BYTE> &dest) const override;

		[[nodiscard]] constexpr std::span<const BYTE> bin() const { return _bin; }
		void set_bin(std::span<BYTE> bin)                         { std::vector(bin.begin(), bin.end()).swap(_bin); }

	private:
		std::vector<BYTE> _bin{};
	};

	/////////////////////////////////////////////////////////////////////////////

	class FrameComment final : public Frame {
	public:
		virtual ~FrameComment() { }

		FrameComment(std::wstring &&name4, UINT declaredSize, std::array<BYTE, 2> flags, std::span<BYTE> src);
		FrameComment(std::wstring &&name4, wl::WStrPtr simpleText)
			: Frame{std::move(name4), 0, {{0, 0}}}, _lang3{L"eng"}, _text{simpleText} { }

		[[nodiscard]] std::wstring as_simple_text() const override;
		void force_simple_text(wl::WStrPtr text) override;
		[[nodiscard]] size_t serializable_size() const override;
		void serialize(std::vector<BYTE> &dest) const override;

		[[nodiscard]] constexpr LPCWSTR lang3() const { return _lang3.c_str(); }
		[[nodiscard]] constexpr LPCWSTR descr() const { return _descr.c_str(); }
		void set_descr(wl::WStrPtr descr)             { _descr = descr; }
		[[nodiscard]] constexpr LPCWSTR text() const  { return _text.c_str(); }
		void set_text(wl::WStrPtr text)               { _text = text; }

	private:
		std::wstring _lang3{};
		std::wstring _descr{};
		std::wstring _text{};
	};

	/////////////////////////////////////////////////////////////////////////////

	class FramePicture final : public Frame {
	public:
		virtual ~FramePicture() { }

		FramePicture(std::wstring &&name4, UINT declaredSize, std::array<BYTE, 2> flags, std::span<BYTE> src);

		[[nodiscard]] std::wstring as_simple_text() const override;
		void force_simple_text(wl::WStrPtr text) override;
		[[nodiscard]] size_t serializable_size() const override;
		void serialize(std::vector<BYTE> &dest) const override;

		[[nodiscard]] constexpr LPCWSTR mime() const     { return _mime.c_str(); }
		void set_mime(wl::WStrPtr mime)                  { _mime = mime; }
		[[nodiscard]] constexpr PicType pic_type() const { return _picType; }
		void set_pic_type(PicType picType)               { _picType = picType; }
		[[nodiscard]] constexpr LPCWSTR descr() const    { return _descr.c_str(); }
		void set_descr(wl::WStrPtr descr)                { _descr = descr; }
		[[nodiscard]] constexpr std::span<const BYTE> bin() const { return _bin; }
		void set_bin(std::span<BYTE> bin)                         { std::vector(bin.begin(), bin.end()).swap(_bin); }

	private:
		std::wstring _mime{};
		PicType _picType = PicType::other;
		std::wstring _descr{};
		std::vector<BYTE> _bin{};
	};

	/////////////////////////////////////////////////////////////////////////////

	class FrameGeob final : public Frame {
	public:
		virtual ~FrameGeob() { }

		FrameGeob(std::wstring &&name4, UINT declaredSize, std::array<BYTE, 2> flags, std::span<BYTE> src);

		[[nodiscard]] std::wstring as_simple_text() const override;
		void force_simple_text(wl::WStrPtr text) override;
		[[nodiscard]] size_t serializable_size() const override;
		void serialize(std::vector<BYTE> &dest) const override;

		[[nodiscard]] constexpr LPCWSTR mime() const      { return _mime.c_str(); }
		void set_mime(wl::WStrPtr mime)                   { _mime = mime; }
		[[nodiscard]] constexpr LPCWSTR file_name() const { return _fileName.c_str(); }
		void set_file_name(wl::WStrPtr fileName)          { _fileName = fileName; }
		[[nodiscard]] constexpr LPCWSTR descr() const     { return _descr.c_str(); }
		void set_descr(wl::WStrPtr descr)                 { _descr = descr; }
		[[nodiscard]] constexpr std::span<const BYTE> enc_obj() const { return _encObj; }
		void set_enc_obj(std::span<BYTE> encObj)                      { std::vector(encObj.begin(), encObj.end()).swap(_encObj); }

	private:
		std::wstring _mime{};
		std::wstring _fileName{};
		std::wstring _descr{};
		std::vector<BYTE> _encObj{};
	};

}
