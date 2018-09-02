#pragma once
#include <functional>
#include <glm/glm.hpp>
#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"


namespace Bengine{

	
	class Particle2D{
		//чем меньше частицы, тем больше их будет в кеше(64 байта)

	public:
		glm::vec2 p_position = glm::vec2(0.0f);
		glm::vec2 p_velocity = glm::vec2(0.0f);
		Color p_color;
		//изначально 0, а провер€ем, активна ли частицы, по лайфтайму( да если >0)
		float p_life = 0.0f;
		//квадратные
		float p_width = 0.0f;

	};
	//inline нужен если функци€ объ€влена в хедере, иначе будет ошбика типа already defined in
	inline void defaultParticleUpdate(Particle2D& particle,float deltaTime){
		particle.p_position += particle.p_velocity * deltaTime;
	}

	//все частицы будут иметь данные свойства
	class ParticleBatch2D
	{
	public:
		ParticleBatch2D();
		~ParticleBatch2D();

		void init(int maxParticles,
					float decayRate,
					GLTexture texture,
					//указатель к функции
					std::function<void(Particle2D&, float)>updateFunc = defaultParticleUpdate);

		void update(float deltaTime);

		void draw(SpriteBatch* spriteBatch);

		void addParticle(const glm::vec2& position,
						const glm::vec2& velocity,
						const Color& color,
						float width);
	private:
		int findFreeParticle();

		std::function<void(Particle2D&, float)>p_updateFunc;
		//как быстро частицы будут исчезать
		float p_decayRate = 0.1f;
		//держим все частицы в последовательном буффере пам€ти
		Particle2D* p_particles = nullptr;
		int p_maxParticles = 0;
		//неактивна€ частица которую надо сделать активной
		int p_lastFreeParticle = 0;
		//текстура
		GLTexture p_texture;
	};

}