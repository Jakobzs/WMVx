#pragma once
#include "GameDataset.h"
#include "DFDatasetAdaptors.h"
#include "DB2File.h"
#include "DFRecordDefinitions.h"
#include "DB2BackedDataset.h"

#define BOOST_METAPARSE_LIMIT_STRING_SIZE 64

#include "boost/mpl/string.hpp"
#include "boost/metaparse/string.hpp"

#include "GenericDB2Dataset.h"
#include "ReferenceSource.h"

#include <algorithm>
#include <execution>

#include "BFADatasets.h"

namespace core {


	using DFAnimationDataDataset = BFAAnimationDataDataset;

	using DFCharRacesDataset = GenericDB2Dataset<DatasetCharacterRaces, DFCharRacesRecordAdaptor, boost::mpl::c_str<BOOST_METAPARSE_STRING("dbfilesclient/chrraces.db2")>::value>;

	class DFCharacterComponentTextureDataset : public DatasetCharacterComponentTextures {
	public:
		DFCharacterComponentTextureDataset(CascFileSystem* fs) {
			db2_sections = std::make_unique<DB2File<DFDB2CharComponentTextureSectionsRecord>>("dbfilesclient/charcomponenttexturesections.db2");
			db2_layouts = std::make_unique<DB2File<DFDB2CharComponentTextureLayoutsRecord>>("dbfilesclient/charcomponenttexturelayouts.db2");

			db2_sections->open(fs);
			db2_layouts->open(fs);

			const auto& layout_sections = db2_layouts->getSections();
			for (auto it = layout_sections.begin(); it != layout_sections.end(); ++it) {
				for (auto it2 = it->records.cbegin(); it2 != it->records.cend(); ++it2) {
					adaptors.push_back(
						std::make_unique<DFCharacterComponentTextureAdaptor>(&(*it2), findSections(it2->data.id))
					);
				}
			}
		}

		DFCharacterComponentTextureDataset(DFCharacterComponentTextureDataset&&) = default;
		virtual ~DFCharacterComponentTextureDataset() { }

		const std::vector<CharacterComponentTextureAdaptor*>& all() const override {
			return reinterpret_cast<const std::vector<CharacterComponentTextureAdaptor*>&>(adaptors);
		}

	protected:
		std::unique_ptr<DB2File<DFDB2CharComponentTextureSectionsRecord>> db2_sections;
		std::unique_ptr<DB2File<DFDB2CharComponentTextureLayoutsRecord>> db2_layouts;

		std::vector<std::unique_ptr<DFCharacterComponentTextureAdaptor>> adaptors;

		const std::vector<const DFDB2CharComponentTextureSectionsRecord*> findSections(uint32_t layoutId) const {
			auto sections = std::vector<const DFDB2CharComponentTextureSectionsRecord*>();

			const auto& section_sections = db2_sections->getSections();
			for (auto it = section_sections.begin(); it != section_sections.end(); ++it) {
				for (auto it2 = it->records.cbegin(); it2 != it->records.cend(); ++it2) {
					if (it2->data.charComponentTexturelayoutId == layoutId) {
						sections.push_back(&(*it2));
					}
				}
			}

			return sections;
		}
	};

	using DFCharacterFacialHairStylesDataset = BFACharacterFacialHairStylesDataset;

	using DFCharHairGeosetsDataset = BFACharHairGeosetsDataset;

	using DFCreatureModelDataDataset = GenericDB2Dataset<DatasetCreatureModelData, DFCreatureModelDataRecordAdaptor, boost::mpl::c_str<BOOST_METAPARSE_STRING("dbfilesclient/creaturemodeldata.db2")>::value >;

	using DFCreatureDisplayDataset = GenericDB2Dataset<DatasetCreatureDisplay, DFCreatureDisplayRecordAdaptor, boost::mpl::c_str<BOOST_METAPARSE_STRING("dbfilesclient/creaturedisplayinfo.db2")>::value >;


	using DFItemDisplayInfoDataset = ModernItemDisplayInfoDataset< DFItemDisplayInfoRecordAdaptor, DFDB2ItemDisplayInfoMaterialResRecord>;

	using DFItemDataset = ModernItemDataset<DFItemRecordAdaptor, DFDB2ItemSparseRecord, DFDB2ItemAppearanceRecord, DFDB2ItemModifiedAppearanceRecord>;

	using DFItemDataset = ModernItemDataset<DFItemRecordAdaptor, DFDB2ItemSparseRecord, DFDB2ItemAppearanceRecord, DFDB2ItemModifiedAppearanceRecord>;

	using DFNPCsDataset = GenericDB2Dataset<DatasetNPCs, DFNPCRecordAdaptor, boost::mpl::c_str<BOOST_METAPARSE_STRING("dbfilesclient/creature.db2")>::value >;

};