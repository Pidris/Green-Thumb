#include <MOD.h>

#include <Reader.h>
#include <iostream>
#include <Vector.h>
#include <Texture.h>

int MOD::ReadFile(const char* filePath) {
	unsigned int chunkType = 0;
	unsigned int chunkLength = 0;
	const unsigned int chunkMagicSize = 0x8; // ima be fr here im just gonna guess this is the right name for it

	/*std::ifstream file;
	file.open(filePath, std::ios::binary | std::ios::in);*/

	Reader file;
	file.open(filePath, std::ios::binary | std::ios::in);
	file.SetEndianness(Reader::Endianness::Big);

	if (!file.is_open())
		return 0;

	while (chunkType != MODChunk::EndOfFile) {
		int chunkPos = file.tellg();

		chunkType = file.ReadU32();
		chunkLength = file.ReadU32();

		if (chunkPos & 0x1F) {
			printf("chunk start not on boundary %08x!\n", chunkPos);
		}

		switch (chunkType) {
			case MODChunk::Header:
				file.seekg(chunkPos + 0x20);
				file.seekg(sizeof(int), std::ifstream::cur); // skipping reading an unused int
				m_ShapeFlags = file.ReadU32();
				//file.seekg(chunkPos + chunkLength + chunkMagicSize);
				break;

			case MODChunk::Vertex:
				m_VertexCount = file.ReadU32();
				file.seekg(chunkPos + 0x20);
				m_Vertices = new Vector3f[m_VertexCount];
				for (int i = 0; i < m_VertexCount; i++) {
					m_Vertices[i].Read(file);
				}
				break;

			case MODChunk::VertexNormal:
				m_NormalCount = file.ReadU32();
				file.seekg(chunkPos + 0x20);
				m_Normals = new Vector3f[m_NormalCount];
				for (int i = 0; i < m_NormalCount; i++) {
					m_Normals[i].Read(file);
				}
				break;

			case MODChunk::VertexNBT:
				m_NBTCount = file.ReadU32();
				file.seekg(chunkPos + 0x20);
				m_NBTs = new Vector3f[m_NBTCount];
				for (int i = 0; i < m_NBTCount; i++) {
					m_NBTs[i].Read(file);
				}
				break;

			case MODChunk::VertexColour:
				m_VtxColorCount = file.ReadU32();
				file.seekg(chunkPos + 0x20);
				m_VtxColors = new Vector3f[m_VtxColorCount];
				for (int i = 0; i < m_VtxColorCount; i++) {
					m_VtxColors[i].Read(file);
				}
				break;

			case MODChunk::TexCoord0:
			case MODChunk::TexCoord1:
			case MODChunk::TexCoord2:
			case MODChunk::TexCoord3:
			case MODChunk::TexCoord4:
			case MODChunk::TexCoord5:
			case MODChunk::TexCoord6:
			case MODChunk::TexCoord7:
				int index = chunkType - MODChunk::TexCoord0;
				m_TexCoordCounts[index] = file.ReadU32();
				file.seekg(chunkPos + 0x20);
				m_TexCoords[index] = new Vector2f[m_TexCoordCounts[index]];
				for (int i = 0; i < m_TexCoordCounts[index]; i++) {
					m_TexCoords[index][i].Read(file);
				}

				m_TotalActiveTexCoords++;
				break;

			case MODChunk::Texture:
				m_TextureCount = file.ReadU32();
				file.seekg(chunkPos + 0x20);
				m_Textures = new TXE[m_TextureCount];
				for (int i = 0; i < m_TextureCount; i++) {
					m_Textures[i].Read(file);
				}
				break;

			case MODChunk::TextureAttribute:
				mTexAttrCount = file.ReadU32();
				file.seekg(chunkPos + 0x20);
				mTexAttrList = new TexAttr[mTexAttrCount];
				for (int i = 0; i < mTexAttrCount; i++) {
					mTexAttrList[i].read(stream);
				}
				break;

			case MODChunk::Material:
				mMaterialCount = stream.readInt();
				mTevInfoCount = stream.readInt();

				stream.skipPadding(0x20);

				if (mTevInfoCount) {
					mTevInfoList = new PVWTevInfo[mTevInfoCount];
					for (int i = 0; i < mTevInfoCount; i++) {
						mTevInfoList[i].read(stream);
					}
				}

				if (mMaterialCount) {
					mMaterialList = new Material[mMaterialCount];
					for (int i = 0; i < mMaterialCount; i++) {
						mMaterialList[i].mIndex = i;
						mMaterialList[i].read(stream);
						mMaterialList[i].mTevInfo = &mTevInfoList[mMaterialList[i].mTevInfoIndex];

						MatobjInfo* info = new MatobjInfo;
						info->mTarget = &mMaterialList[i];
						gsys->addGfxObject(info);
					}
				}
				break;

			case MODChunk::VertexMatrix:

				mVtxMatrixCount = stream.readInt();
				stream.skipPadding(0x20);
				mVtxMatrixList = new VtxMatrix[mVtxMatrixCount];
				for (int i = 0; i < mVtxMatrixCount; i++) {
					mVtxMatrixList[i].read(stream);
				}
				break;

			case MODChunk::MatrixEnvelope:

				mEnvelopeCount = stream.readInt();
				stream.skipPadding(0x20);
				mEnvelopeList = new Envelope[mEnvelopeCount];
				for (int i = 0; i < mEnvelopeCount; i++) {
					mEnvelopeList[i].read(stream);
				}
				break;

			case MODChunk::Mesh:

				mMeshCount = stream.readInt();
				stream.skipPadding(0x20);
				mMeshList = new Mesh[mMeshCount];
				for (int i = 0; i < mMeshCount; i++) {
					mMeshList[i].read(stream);
				}
				break;

			case MODChunk::Joint:

				mJointCount = stream.readInt();
				stream.skipPadding(0x20);
				mJointList = new Joint[mJointCount];
				for (int i = 0; i < mMeshCount; i++) {
					if (mMeshList[i].mParentJoint != -1) {
						mMeshList[i].mJointList = &mJointList[mMeshList[i].mParentJoint];
					}
				}

				for (int i = 0; i < mJointCount; i++) {
					mJointList[i].mParentShape = this;
					mJointList[i].read(stream);
					mCourseExtents.expandBound(mJointList[i].mBounds);
				}

				stream.skipPadding(0x20);

				mTotalMatpolyCount = 0;
				for (int i = 0; i < mJointCount; i++) {
					mTotalMatpolyCount += mJointList[i].mMatPolyCount;
				}

				mMatpolyList = new Joint::MatPoly * [mTotalMatpolyCount];

				for (int i = 0; i < mJointCount; i++) {
					mJointList[i].mIndex = i;
					if (mJointList[i].mParentIndex != -1) {
						mJointList[mJointList[i].mParentIndex].add(&mJointList[i]);
					}
				}

				_dlindx = 0;

				// Sorts materials in the following order:
				// 1. Alpha-blended materials (transparent, needs to be last).
				// 2. Alpha-test materials (cutout transparency).
				// 3. Opaque materials (no transparency, needs to be first).
				// This is to handle transparency correctly without per-frame depth sorting.
				recAddMatpoly(mJointList, 4); // MATFLAG_AlphaBlend
				recAddMatpoly(mJointList, 2); // MATFLAG_AlphaTest
				recAddMatpoly(mJointList, 1); // MATFLAG_Opaque

				for (int i = 0; i < mTotalMatpolyCount; i++) {
					mMatpolyList[i]->mJointList = mMatpolyList[i]->mMesh->mJointList;
				}
				break;

			case MODChunk::JointName:

				mJointCount = stream.readInt();
				stream.skipPadding(0x20);
				for (int i = 0; i < mJointCount; i++) {
					String name(0);
					stream.readString(name);
					mJointList[i].setName(name.mString);
				}

				stream.skipPadding(0x20);
				break;

			case MODChunk::CollisionPrism:

				mTriCount = stream.readInt();
				mBaseRoomCount = stream.readInt();
				stream.skipPadding(0x20);
				mRoomInfoList = new RoomInfo[mBaseRoomCount];

				for (int i = 0; i < mBaseRoomCount; i++) {
					mRoomInfoList[i].read(stream);
				}
				stream.skipPadding(0x20);

				mTriList = new CollTriInfo[mTriCount];

				for (int i = 0; i < mTriCount; i++) {
					mTriList[i].read(stream);
					mTriList[i].init(mRoomInfoList, mVertexList);
				}
				break;

			case MODChunk::CollisionGrid:
				stream.skipPadding(0x20);
				mCourseExtents.read(stream);
				mGridSize = stream.readFloat();
				mGridSizeX = stream.readInt();
				mGridSizeY = stream.readInt();

				mCollGroups = new CollGroup * [mGridSizeX * mGridSizeY];
				int groupCount = stream.readInt();
				int maxTrisPerGroup = 0;
				CollGroup* tmpGroups = new CollGroup[groupCount];

				for (int i = 0; i < groupCount; i++) {
					tmpGroups[i].mFarCulledTriCount = stream.readShort();
					tmpGroups[i].mTriCount = stream.readShort();
					tmpGroups[i].mTriangleList = new CollTriInfo * [tmpGroups[i].mTriCount];

					if (tmpGroups[i].mTriCount > maxTrisPerGroup) {
						maxTrisPerGroup = tmpGroups[i].mTriCount;
					}

					for (int j = 0; j < tmpGroups[i].mTriCount; j++) {
						int idx = stream.readInt();
						tmpGroups[i].mTriangleList[j] = &mTriList[idx];
					}

					if (tmpGroups[i].mFarCulledTriCount) {
						tmpGroups[i].mFarCulledTriDistances = new u8[tmpGroups[i].mFarCulledTriCount];
						for (int j = 0; j < tmpGroups[i].mFarCulledTriCount; j++) {
							tmpGroups[i].mFarCulledTriDistances[j] = stream.readByte();
						}
					}
				}

				PRINT("got a max of %d col tris in one block!\n", maxTrisPerGroup);

				CollGroup* group = new CollGroup();
				group->mTriCount = 0;
				group->mTriangleList = nullptr;
				int validTriCount = 0;
				f32 maxDist = 0.0f;
				for (int i = 0; i < mGridSizeY; i++) {
					for (int j = 0; j < mGridSizeX; j++) {
						int groupIdx = stream.readInt();
						if (groupIdx == -1) {
							mCollGroups[j + i * mGridSizeX] = group;
							continue;
						}

						mCollGroups[j + i * mGridSizeX] = &tmpGroups[groupIdx];
						f32 cellSize = 64.0f;
						f32 cellMinX = mCourseExtents.mMin.x + f32(j) * cellSize;
						f32 cellMinZ = mCourseExtents.mMin.z + f32(i) * cellSize;

						f32 expansionSize = 64.0f * 1.0f;
						BoundBox cellBox;
						Vector3f lower(cellMinX - 64.0f, mCourseExtents.mMin.y - expansionSize, cellMinZ - 64.0f);
						cellBox.expandBound(lower);
						Vector3f upper(cellMinX + cellSize + expansionSize, mCourseExtents.mMax.y + expansionSize,
							cellMinZ + cellSize + expansionSize);
						cellBox.expandBound(upper);

						for (int k = 0; k < tmpGroups[groupIdx].mTriCount; k++) {
							CollTriInfo* tri = tmpGroups[groupIdx].mTriangleList[k];
							f32 dist = triRectDistance(&mVertexList[tri->mVertexIndices[0]], &mVertexList[tri->mVertexIndices[1]],
								&mVertexList[tri->mVertexIndices[2]], cellBox, false);
							if (dist >= 0.001f) {
								validTriCount++;
								if (dist > maxDist) {
									maxDist = dist;
								}
							}
						}
					}
				}
				stream.skipPadding(0x20);
				break;
			}

		file.seekg(chunkPos + chunkLength + chunkMagicSize);
	}

	std::cout <<
		"Model file: " << filePath << "\n" <<
		"Flags: " << m_ShapeFlags << "\n" <<
		"Vertex count: " << m_VertexCount << "\n" <<
		"Normal count: " << m_NormalCount << "\n";

	file.close();
	return 1;
}