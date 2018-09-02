#pragma once
#include <vector>
//это лучше сказывается на времени компиляции

namespace Bengine{

	class ParticleBatch2D;
	class SpriteBatch;

class ParticleEngine2D
{
public:
	ParticleEngine2D();
	~ParticleEngine2D();
	//въебываем те, с которыми должно работать.После добавления, ParticleEngine2D берет ответственность за деаллокацию(удаление)
	void addParticleBatch(ParticleBatch2D* particleBatch);

	void update(float deltaTime);

	void draw(SpriteBatch* spriteBatch);

private:
	//въебываем сюда каждый  новый, а движок уже потом сам с ними ебаться будет
	std::vector<ParticleBatch2D*> p_batches;
};

}