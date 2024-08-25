#pragma once
#include "M2Defintions.h"
#include "../filesystem/GameFileSystem.h"
#include "../utility/Exceptions.h"
#include "AnimationCommon.h"
#include "ModelAdaptors.h"
#include <memory>
#include <optional>
#include <span>
#include <variant>
#include <cstdint>

namespace core {


	struct M2Header {
	public:

		// Reads the header from the buffer, returns the header and number of bytes read.
		static std::pair<M2Header, size_t> create(std::span<uint8_t> buffer);

		std::array<uint8_t, 4> magic;
		uint32_t version;
		M2Array name;
		std::underlying_type_t<GlobalFlags> globalFlags;
		M2Array globalSequences;
		M2Array animations;
		M2Array animationLookup;
		std::optional<M2Array> playableAnimationLookup; // <= BC
		M2Array bones;
		M2Array keyBoneLookup;
		M2Array vertices;
		std::variant<uint32_t, M2Array> views;	//(M2Array <= BC) (uint32_t > BC)
		M2Array colors;
		M2Array textures;
		M2Array transparency;
		std::optional<M2Array> textureFlipbooks;
		M2Array uvAnimations;
		M2Array textureReplace;
		M2Array renderFlags;
		M2Array boneLookup;
		M2Array textureLookup;
		M2Array textureUnits;
		M2Array transparencyLookup;
		M2Array uvAnimationLookup;

		M2Box boundingBox;
		float boundingSphereRadius;
		M2Box collisionBox;
		float collisionSphereRadius;

		M2Array boundingTriangles;
		M2Array boundingVertices;
		M2Array boundingNormals;

		M2Array attachments;
		M2Array attachmentLookup;
		M2Array events;
		M2Array lights;
		M2Array cameras;
		M2Array cameraLookup;
		M2Array ribbonEmitters;
		M2Array particleEmitters;

		std::optional<M2Array> blendMapOverrides; // >= BC && USE_BLEND_MAP_OVERRIDES
	};





	class M2Model {
	public:
		M2Model(GameFileSystem* fs, GameFileUri uri);

		const M2Header getHeader() const {
			return _header;
		}

	protected:

		// apply a callback for each skeleton file, starting the at top most parent.
		template<typename fn>
		void processSkelFiles(GameFileSystem* fs, ArchiveFile* file, const ChunkedFile2::Chunks& chunks, fn callback, int32_t file_index = 0) {
			if (file != nullptr) {
				const auto skpd_chunk = chunks.find(Signatures::SKPD);
				if (skpd_chunk != chunks.end()) {
					Chunks::SKPD skpd;
					assert(sizeof(skpd) <= skpd_chunk->second.size);
					file->read(&skpd, sizeof(skpd), skpd_chunk->second.offset);

					if (skpd.parentSkelFileId) {
						std::unique_ptr<ArchiveFile> parent_file(fs->openFile(skpd.parentSkelFileId));
						if (parent_file != nullptr) {
							ChunkedFile2::Chunks parent_chunks = ChunkedFile2::getChunks(parent_file.get());
							processSkelFiles(fs, parent_file.get(), parent_chunks, callback, ++file_index);
						}
					}
				}

				callback(file, chunks, file_index);
			}
		}


		M2Header _header;
		ChunkedFile2::Chunks _chunks;

		std::vector<std::unique_ptr<ModelGeosetAdaptor>> geosetAdaptors;
		std::vector<std::unique_ptr<ModelAnimationSequenceAdaptor>> animationSequenceAdaptors;

		std::vector<std::unique_ptr<ModelTextureAnimationAdaptor>> textureAnimationAdaptors;

		std::vector<std::unique_ptr<ModelColorAdaptor>> colorAdaptors;
		std::vector<std::unique_ptr<ModelTransparencyAdaptor>> transparencyAdaptors;

		std::vector<std::unique_ptr<ModelBoneAdaptor>> boneAdaptors;

		std::vector<std::unique_ptr<ModelRibbonEmitterAdaptor>> ribbonAdaptors;
		std::vector<std::unique_ptr<ModelParticleEmitterAdaptor>> particleAdaptors;

		std::vector<std::unique_ptr<ModelAttachmentDefinitionAdaptor>> attachmentDefinitionAdaptors;

		std::vector<uint32_t> globalSequences;

		std::vector<Vector3> vertices;
		std::vector<Vector3> normals;
		std::vector<Vector2> textureCoords;
		std::vector<uint16_t> indices;

		std::vector<ModelVertexM2> rawVertices;

		std::vector<Vector3> bounds;
		std::vector<uint16_t> boundTriangles; //TODO CHECK IF CORRECT TYPE

		std::vector<ModelTextureM2> textureDefinitions;

		std::vector<uint16_t> attachmentLookups;

		std::vector<int16_t> keyBoneLookup;

		std::vector<uint16_t> animationLookups; //TODO check correct type, wmv and wiki use different types

		//std::vector<ModelRenderPass> renderPasses;
	};


}