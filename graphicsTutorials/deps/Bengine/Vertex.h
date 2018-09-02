#pragma once

#include <GL/glew.h>
namespace Bengine{
	struct Position{
		float x, y;
	};
	struct Color{
		//структуры в отличии от классов публичны по дефолту
		Color() : r(0), g(0), b(0), a(0){}
		Color(GLubyte R, GLubyte G, GLubyte B, GLubyte A) :
			r(R), g(G), b(B), a(A){}
		GLubyte r, g, b, a;
	};
	//or can use something like this if we don't want to modify
	// struct Position{
	//float x, y;
	//	} position;

	struct UV{
		float u, v;
	};
	struct Vertex{
		Position position;
		Color color;
		// It is called Composition = store struct in other sruct. Or ClassInClass
		// GLubyte is good for memory. Only 3 bytes, not 12 bytes like with float
		//red, green, blue, alpha(transparency)
		UV uv;

		void setPosition(float x, float y){
			position.x = x;
			position.y = y;
		}

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a){
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void setUV(float u, float v){
			uv.u = u;
			uv.v = v;
		}
	};
}