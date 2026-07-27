#pragma once

#include <fstream>
#include <Types.h>

struct Reader : public std::ifstream {
	enum class Endianness : u8 { Little, Big };

	inline void SetEndianness(Endianness end) { m_Endianness = end; }
	inline Endianness& endianness() { return m_Endianness; }

	inline u8 ReadU8() {
		u8 byte = 0;
		read(reinterpret_cast<char*>(&byte), 1);
		return byte;
	}

	inline u16 ReadU16() {
		u16 v = 0;
		read(reinterpret_cast<char*>(&v), 2);
		return ((v & 0xff00) >> 8) | ((v & 0x00ff) << 8);
	}

	inline u32 ReadU32() {
		u32 v = 0;
		read(reinterpret_cast<char*>(&v), 4);
		if (m_Endianness == Endianness::Little)
			return v;
		return (((v & 0xff000000) >> 24) | ((v & 0x00ff0000) >> 8) | ((v & 0x0000ff00) << 8) | ((v & 0x000000ff) << 24));
	}

	inline s32 ReadS32() {
		s32 v = 0;
		read(reinterpret_cast<char*>(&v), 4);
		if (m_Endianness == Endianness::Little)
			return v;
		return (((v & 0xff000000) >> 24) | ((v & 0x00ff0000) >> 8) | ((v & 0x0000ff00) << 8) | ((v & 0x000000ff) << 24));
	}

	inline f32 ReadF32() {
		u32 word = ReadU32();
		return *reinterpret_cast<float*>(&word);
	}

	private:
		Endianness m_Endianness;
};