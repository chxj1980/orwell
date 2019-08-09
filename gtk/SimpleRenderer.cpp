#include "SimpleRenderer.h"

void SimpleRenderer::init()
{
}
/*
~OpenGLArea::OpenGLArea()
{
	delete program;
}
*/
//void OpenGLArea::receiveVideo(unsigned char **videoBuffer, int frameWidth, int frameHeight)

void SimpleRenderer::glInit()
{
}

bool SimpleRenderer::render(const Glib::RefPtr<Gdk::GLContext> &context)
{
	try
	{
		glArea.throw_if_error();
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glDraw();

		glFlush();
	}
	catch (const Gdk::GLError &gle)
	{
		std::cerr << "An error occurred in the render callback of the GLArea" << std::endl;
		std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
		return false;
	}
}

void SimpleRenderer::glDraw()
{
	unsigned int VBO, VAO;
	if (firstRun)
	{
		std::cout << "glDraw()" << std::endl;
		Shader vertex_shader(ShaderType::Vertex, "vertex_yuv.shader");
		Shader fragment_shader(ShaderType::Fragment, "fragment_yuv.shader");

		program = std::make_unique<Program>();
		program->attach_shader(vertex_shader);
		program->attach_shader(fragment_shader);
		program->link();
		firstRun = false;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0);
	}
	program->use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}