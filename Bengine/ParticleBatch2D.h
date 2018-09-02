#pragma once
#include <functional>
#include <glm/glm.hpp>
#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"


namespace Bengine{

	
	class Particle2D{
		//��� ������ �������, ��� ������ �� ����� � ����(64 �����)

	public:
		glm::vec2 p_position = glm::vec2(0.0f);
		glm::vec2 p_velocity = glm::vec2(0.0f);
		Color p_color;
		//���������� 0, � ���������, ������� �� �������, �� ���������( �� ���� >0)
		float p_life = 0.0f;
		//����������
		float p_width = 0.0f;

	};
	//inline ����� ���� ������� ��������� � ������, ����� ����� ������ ���� already defined in
	inline void defaultParticleUpdate(Particle2D& particle,float deltaTime){
		particle.p_position += particle.p_velocity * deltaTime;
	}

	//��� ������� ����� ����� ������ ��������
	class ParticleBatch2D
	{
	public:
		ParticleBatch2D();
		~ParticleBatch2D();

		void init(int maxParticles,
					float decayRate,
					GLTexture texture,
					//��������� � �������
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
		//��� ������ ������� ����� ��������
		float p_decayRate = 0.1f;
		//������ ��� ������� � ���������������� ������� ������
		Particle2D* p_particles = nullptr;
		int p_maxParticles = 0;
		//���������� ������� ������� ���� ������� ��������
		int p_lastFreeParticle = 0;
		//��������
		GLTexture p_texture;
	};

}