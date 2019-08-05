#ifndef PROGRAM_H
#define PROGRAM_H

#include "Shader.h"

#include <GL/glew.h>

class Program
{
  private:
	bool linked = false;
	GLuint id = 0;

  public:
	Program();
	Program(const std::string &vertex_path, const std::string &fragment_path);
	~Program();

	GLuint get_id() const;
	bool create();
	bool attach_shader(const Shader &shader);
	bool is_linked() const;
	bool is_created() const;
	bool link();
	void use() const;
	void remove();
};

#endif //PROGRAM_H
