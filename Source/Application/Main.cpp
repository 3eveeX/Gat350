



int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    SDL_Event e;
    bool quit = false;

    struct vertex {
        neu::vec3 position;
        neu::vec3 colour;
        neu::vec2 texcoord;
    };

    std::vector<vertex> vertices{
        {{-1, -1, 0}, {1, 0, 0}, {0, 0} },
        {{-1, 1, 0}, {0, 1, 0}, {0, 1} },
        {{1, 1, 0}, {0, 0, 1}, {1, 1} },
        {{1, -1, 0}, {0, 0, 0}, {1, 0} }
    };

    std::vector<GLushort> indices{
        0, 1, 2,
        0, 2, 3
	};

    auto model3d = std::make_shared<neu::Model>();
    model3d->Load("models/spot.obj");

	
    
    //shaders
    /*auto vs_shader = neu::Resources().Get<neu::Shader>("shaders/basic_lit.vert", GL_VERTEX_SHADER);
    auto fs_shader = neu::Resources().Get<neu::Shader>("shaders/basic_lit.frag", GL_FRAGMENT_SHADER);
    GLuint vs = vs_shader->m_shader; 
    GLuint fs = fs_shader->m_shader;*/

    //program
	auto program = neu::Resources().Get<neu::Program>("shaders/basic_lit.prog");
	program->Use();

    //texture
	neu::res_t<neu::Texture> texture = neu::Resources().Get<neu::Texture>("Textures/spot_diffuse.png");

    //uniform
	program->SetUniform("u_texture", 0);

    //lights
	program->SetUniform("u_ambient_light", glm::vec3{ 0.1f });
    neu::Transform light{ {2, 4, 0} };
	
    //transform

    neu::Transform transform{ {0, 0, 0} };
    neu::Transform camera{ {0, 0, 5} };

    //projection matrix
    float aspect = (float)neu::GetEngine().GetRenderer().GetWidth() / neu::GetEngine().GetRenderer().GetHeight();
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspect, 0.01f, 100.0f);
    program->SetUniform("u_projection", projection);

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
        float dt = neu::GetEngine().GetTime().GetDeltaTime();

		//rotation += dt * 90;

		//model matrix
		transform.rotation.y += dt * 90.0f;
        program->SetUniform("u_model", transform.GetMatrix());


        // fill in the rest of the controls (WS and QE)
		//view matrix
            float speed = 10.0f;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_A)) camera.position.x += speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_D)) camera.position.x -= speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_W)) camera.position.z -= speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_S)) camera.position.z += speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_Q)) camera.position.y += speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_E)) camera.position.y -= speed * dt;
        //eye.x += neu::GetEngine().GetInput().GetMouseDelta().x * -0.01f;
		glm::mat4 view = glm::lookAt(camera.position, camera.position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		program->SetUniform("u_view", view);
        program->SetUniform("u_light.color", glm::vec3{2.0f, 0.0f, 0.3f});
		//light.position.x = -5.0f * sinf(20 * neu::GetEngine().GetTime().GetTime());
		//light.position.z = 5.0f * sinf(neu::GetEngine().GetTime().GetTime());
		program->SetUniform("u_light.position", (glm::vec3)(view * glm::vec4(light.position, 1)));

       /* program->SetUniform("u_time", neu::GetEngine().GetTime().GetTime());*/
        
       // draw
        neu::GetEngine().GetRenderer().Clear();
        // start new ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        // set ImGui
        ImGui::Begin("Editor");
        ImGui::Text("Hello World");
        ImGui::Text("Press 'Esc' to quit.");
        ImGui::End();
        model3d->Draw(GL_TRIANGLES);
        // draw ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
