#include "ParticleEngine2D.h"
#include "ParticleBatch2D.h"
#include "SpriteBatch.h"
namespace Bengine{

ParticleEngine2D::ParticleEngine2D()
{
	//empty
}


ParticleEngine2D::~ParticleEngine2D()
{
	for (auto & b : p_batches){
		delete b;
	}

}


void ParticleEngine2D::addParticleBatch(ParticleBatch2D* particleBatch){
	p_batches.push_back(particleBatch);
}

void ParticleEngine2D::update(float deltaTime){
	for (auto & b : p_batches){
		b->update(deltaTime);
	}
}

void ParticleEngine2D::draw(SpriteBatch* spriteBatch){
	//нарисует все батчи
	for (auto &b : p_batches){
		spriteBatch->begin();
		b->draw(spriteBatch);
		spriteBatch->end();
		spriteBatch->renderBatch();
	}
}
}