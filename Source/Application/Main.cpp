
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene


    SDL_Event e;
    bool quit = false;

    //OPENGL Initialization
    std::vector<neu::vec3> vertices{ {1, -1, 0}, {-1, -1, 0}, {0, 1, 0} };
	std::vector<neu::vec3> colors{ {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };

    GLuint vbo[2];
	glGenBuffers(2, vbo);

	//color buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * colors.size(), colors.data(), GL_STATIC_DRAW);


    //vertex buffer (position)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    //vertex array
    GLuint vao;
	glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    

	glEnableVertexAttribArray(0); //position
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(1); //colour
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //vertex shader
    std::string vs_source;
	neu::file::ReadTextFile("Shaders/basic.vert", vs_source);
	const char* vs_source_cstr = vs_source.c_str();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_source_cstr, NULL);
	glCompileShader(vs);

    int success;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetShaderInfoLog(vs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Shader compilation failed: {}", infoLog);
    }

	//fragment shader
	std::string fs_source;
	neu::file::ReadTextFile("Shaders/basic.frag", fs_source);
	const char* fs_source_cstr = fs_source.c_str();
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_source_cstr, NULL);
	glCompileShader(fs);

    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetShaderInfoLog(fs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Shader compilation failed: {}", infoLog);
    }

	//shader program
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vs);
	glAttachShader(shader_program, fs);
	glLinkProgram(shader_program);
	glUseProgram(shader_program);

    //uniform
	GLint uniform = glGetUniformLocation(shader_program, "u_time");
	ASSERT(uniform != -1);



    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetProgramInfoLog(shader_program, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Shader link failed: {}", infoLog);
    }


    // MAIN LOOP
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        // update
        neu::GetEngine().Update();

        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;

		glUniform1f(uniform, neu::GetEngine().GetTime().GetTime());
        /*
        neu::vec2 position;
        neu::vec2 mouse = neu::GetEngine().GetInput().GetMousePosition();
		float angle = neu::GetEngine().GetTime().GetTime() * 90.0f;
		float scale = neu::math::Remap(-1.0f, 1.0f, 0.0f, 2.0f, sinf(neu::GetEngine().GetTime().GetTime()));
		position.x = neu::math::Remap(0.0f, (float)neu::GetEngine().GetRenderer().GetWidth(), -1.0f, 1.0f, mouse.x);
		position.y = neu::math::Remap(0.0f, (float)neu::GetEngine().GetRenderer().GetWidth(), 1.0f, -1.0f, mouse.y);
          */
        // draw
        neu::GetEngine().GetRenderer().Clear();
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());
              /*
		glLoadIdentity();
        glPushMatrix();

        glTranslatef(position.x, position.y, 0);
		glRotatef(angle, 0, 0, 1);
		glScalef(scale, scale, 0);

        glBegin(GL_TRIANGLES);

        for (size_t i = 0; i < vertices.size(); i++) {
            glColor3f(colors[i].r, colors[i].g, colors[i].b);
            glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
		}

		glPopMatrix();
        glEnd();
                */
        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
