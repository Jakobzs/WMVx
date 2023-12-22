#pragma once
#include <vector>
#include <algorithm>
#include "../utility/Matrix.h"
#include "AnimationCommon.h"
#include "ModelAdaptors.h"

namespace core {

//TODO remove static variable -  probably needs to be moved into the emitter classes? looks like it needs to be kept between calls
	static Matrix	SpreadMat;
	void CalcSpreadMatrix(float Spread1, float Spread2, float w, float l);


	class ParticleFactory {
	public:

		using Generator = std::function<ModelParticleEmitterAdaptor::Particle(
			ModelParticleEmitterAdaptor*, size_t, const AnimationTickArgs&, std::vector<ModelBoneAdaptor*>&,
			float, float, float, float, float, float)>;

		//TODO tidy these

		static ModelParticleEmitterAdaptor::Particle plane(ModelParticleEmitterAdaptor* emitter,
			size_t animation_index, const AnimationTickArgs& tick, std::vector<ModelBoneAdaptor*>& allbones,
			float w, float l, float spd, float var, float spr, float spr2);

		static ModelParticleEmitterAdaptor::Particle sphere(ModelParticleEmitterAdaptor* emitter,
			size_t animation_index, const AnimationTickArgs& tick, std::vector<ModelBoneAdaptor*>& allbones,
			float w, float l, float spd, float var, float spr, float spr2);

	};

}