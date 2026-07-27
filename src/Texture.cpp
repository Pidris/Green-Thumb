#include <Texture.h>
#include <Reader.h>

void TXE::Read(Reader& reader) {
	m_Width = reader.ReadU16();
	m_Height = reader.ReadU16();
	m_Format = static_cast<TexFmt>(reader.ReadU32());
	m_ImageCount = reader.ReadU32();

	reader.ReadU32();
	reader.ReadU32();
	reader.ReadU32();
	reader.ReadU32();

	m_DataSize = reader.ReadU32();
	m_TextureData = new u8[m_DataSize];
	reader.read(reinterpret_cast<char*>(m_TextureData), m_DataSize);
}