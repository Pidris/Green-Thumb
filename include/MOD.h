#pragma once

enum MODChunk : int {
	Header = 0x00,
	Vertex = 0x10,
	VertexNormal = 0x11,
	VertexNBT = 0x12,
	VertexColour = 0x13,
	TexCoord0 = 0x18,
	TexCoord1 = 0x19,
	TexCoord2 = 0x1A,
	TexCoord3 = 0x1B,
	TexCoord4 = 0x1C,
	TexCoord5 = 0x1D,
	TexCoord6 = 0x1E,
	TexCoord7 = 0x1F,
	Texture = 0x20,
	TextureAttribute = 0x22,
	Material = 0x30,
	VertexMatrix = 0x40,
	MatrixEnvelope = 0x41,
	Mesh = 0x50,
	Joint = 0x60,
	JointName = 0x61,
	CollisionPrism = 0x100,
	CollisionGrid = 0x110,
	EndOfFile = 0xFFFF,
};

struct Vector3f;
struct TXE;

class MOD { // this thing is MAJORATIVELY source code i scalped straight off the Pikmin 1 decomp
	public:


		int ReadFile(const char*);


		int m_ShapeFlags;
		unsigned int m_VertexCount;
		Vector3f* m_Vertices;
		unsigned int m_NormalCount;
		Vector3f* m_Normals;
		unsigned int m_NBTCount;
		Vector3f* m_NBTs;
		unsigned int m_VtxColorCount;
		Vector3b* m_VtxColors;
		unsigned int m_TexCoordCounts[8];
		Vector2f* m_TexCoords[8];
		unsigned int m_TotalActiveTexCoords = 0;
		unsigned int m_TextureCount;
		TXE* m_Textures;
		unsigned int m_TexAttrCount;

};