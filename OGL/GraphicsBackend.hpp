#ifndef GraphicsBackend_hpp
#define GraphicsBackend_hpp

struct GLFWwindow;

class GraphicsBackend
{
public:
	GraphicsBackend();
	~GraphicsBackend();

	void start();
private:
	GLFWwindow* window;

};


#endif // !GraphicsBackend_hpp
