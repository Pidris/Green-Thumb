#pragma once

enum class TexFmt {
	NONE = -1, ///< -1, invalid format.
	RGB565 = 0,  ///< 0, 16-bit color, max alpha (5 red, 6 green, 5 blue).
	S3TC = 1,  ///< 1, S3 texture compression (block-based, supports alpha).
	RGB5A3 = 2,  ///< 2, either 15-bit color max alpha, or 12-bit color with 3-bit alpha, depending on first bit (1=max alpha).
	I4 = 3,  ///< 3, grey with 4-bit intensities + max alpha (each RGB is value * 0x11).
	I8 = 4,  ///< 4, grey with 8-bit intensities + max alpha.
	IA4 = 5,  ///< 5, grey with 4-bit intensities + 4-bit alpha (each RGB is lower 4 * 0x11, alpha is higher 4 * 0x11).
	IA8 = 6,  ///< 6, grey with 8-bit intensities + 8-bit alpha (alpha = first byte, RGB = second byte).
	RGBA8 = 7,  ///< 7, 24-bit depth true color + 8-bit alpha channel (AR alternating block, then GB alternating block).
	Z8 = 8,  ///< 8, unsigned 8-bit Z values.
	COUNT,       ///< 9, number of supported texture formats.
};

struct Reader;

struct TXE {
	public:
		void Read(Reader&);

	private:
		int m_Index;
		TexFmt m_Format;
		int m_Width;
		int m_Height;
		int m_ImageCount;
		int m_DataSize;
		void* m_TextureData;
		void* m_PixelData;
};