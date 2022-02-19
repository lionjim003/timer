#include "depend.h"
#include "main.h"

void buffer_setup (const char *file, unsigned n, unsigned *targets, unsigned *buffers) {
	
	unsigned int
	amount[]  = {300},
	types[]   = {GL_FLOAT},
	usage[]   = {GL_STATIC_DRAW};
	
	const char *block[2];
	block[0] = "# Vertices";
	block[1] = "#";
	
	
	GL (glGenBuffers (n, buffers));
	int result =
	GL (BindBuffersData (n, targets, buffers, file, block+0, block+1, amount, types, usage));
	printf ("BindBufferData return %d\n", result);
}

void vertex_array_setup (unsigned *array, unsigned n, unsigned *targets, unsigned *buffers) {

	GL (glGenVertexArrays (1, array));
	GL (glBindVertexArray (*array));
	
	GL (glBindBuffer (targets[0], buffers[0]));
	GL (glEnableVertexAttribArray (0));
	GL (glEnableVertexAttribArray (1));
	GL (glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)0));
	GL (glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(sizeof(float) * 3)));
}

void matrix_setup (unsigned shader, vec3 pos, float deltaTime) {
	
	mat4 model = GLM_MAT4_IDENTITY_INIT,
		 proj  = GLM_MAT4_IDENTITY_INIT,
		 view;
		 
	// Model
	glm_translate (model, pos);
	glm_rotate	  (model, glm_rad(pos[0]*20.0f) * (float)glfwGetTime(), (vec3){1.0f, 0.5f, 0.0f});
	
	// View
	camera_matrix (view, deltaTime);
	
	// Projection
	glm_perspective (glm_rad (45.0f), (float)(800/600), 0.1f, 100.0f, proj);
	
	GL (shader_set_mat4fv (shader, "model", (float *)model));
	GL (shader_set_mat4fv (shader, "view",  (float *)view));
	GL (shader_set_mat4fv (shader, "proj",  (float *)proj));
}

int main () {
	
	GLFWwindow *window = window_context (800, 600, "OpenGL", NULL, NULL);
	window_callback (window);
	void key_callback (GLFWwindow *window, int key, int scancode, int action, int mods);
	glfwSetKeyCallback (window, key_callback);
	
	unsigned 
	shader[2],  shader_type[] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
	
	shader[0] = shader_program ("resource/object.glsl", 2, shader_type);
	shader[1] = shader_program ("resource/light.glsl", 2, shader_type);
	
	unsigned n = 1, BO[n], 
	targets[] =  {GL_ARRAY_BUFFER};
	buffer_setup ("resource/shader.dat", n, targets, BO);
	
	unsigned vao[2];
	vertex_array_setup (vao, n, targets, BO);
	vertex_array_setup (vao + 1, n, targets, BO);
	
	FILE *file;
	float color[6];
	file = fopen ("resource/object.glsl", "r");
	fdata_float (file, "%f", "/* \t\n", 3, color+0);
	fclose (file);
	
	file = fopen ("resource/light.glsl", "r");
	fdata_float (file, "%f", "/* \t\n", 3, color+3);
	fclose (file);
	
	//======================================
	GL (glUseProgram (shader[0]));
	GL (shader_set3fv (shader[0], "color_object", color+0));
	GL (shader_set3fv (shader[0], "color_light", color+3));
	
	GL (glUseProgram (shader[1]));
	GL (shader_set3fv (shader[1], "color_light", color+3));
	
	void *data[4] = {shader, vao};
	
	GL (glEnable (GL_DEPTH_TEST));
	GL (glClearColor (0.0f, 0.0f, 0.0f, 1.0f));
	
	void 
	render (GLFWwindow *window, float deltaTime, void *args);
	window_render (window, data, render);
	return 0;
}


float pos_light[] = {1.0f, 3.0f, 1.0f};
float key_input = 0.1f;
void render (GLFWwindow *window, float deltaTime, void *args) {
	
	GL (glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	
	unsigned
	*shader = *(unsigned **)args,
	*vao	= *((unsigned **)args + 1);
	
	// GL (shader_setf (shader, "time", (float)glfwGetTime ()));
	
	//================================ Object draw
	GL (glBindVertexArray (vao[0]));
	GL (glUseProgram (shader[0]));
	GL (shader_setf (shader[0], "key" , key_input));
	GL (shader_set3fv (shader[0], "pos_light", pos_light));
	
	for (int i = 0; i < 10; i++) {
		vec3 pos[] = {
			0.3f,  0.0f,  0.0f, 
			-2.0f,  5.0f, -15.0f, 
			-1.5f, -2.2f, -2.5f,  
			3.8f, -2.0f, -12.3f,  
			-2.4f, -0.4f, -3.5f,  
			-1.7f,  3.0f, -7.5f,  
			1.3f, -2.0f, -2.5f,  
			1.5f,  2.0f, -2.5f, 
			-1.5f,  0.2f, -1.5f, 
			1.3f,  1.0f, -1.5f 
		};
		matrix_setup (shader[0], pos[i], deltaTime);
		GL (glDrawArrays (GL_TRIANGLES, 0, 36));
	}
	
	//================================ Light draw
	GL (glBindVertexArray (vao[1]));
	GL (glUseProgram (shader[1]));
	
	matrix_setup (shader[1], pos_light, deltaTime);
	GL (glDrawArrays (GL_TRIANGLES, 0, 36));
}

void key_callback (GLFWwindow *window, int key, int scancode, int action, int mods) {
	
	camera_key_input (key, action);
	
	if (action != GLFW_RELEASE) {
		switch (key) {
			case GLFW_KEY_KP_2:
				pos_light[2] += 0.1f;
				break;
				
			case GLFW_KEY_KP_8:
				pos_light[2] -= 0.1f;
				break;
				
			case GLFW_KEY_KP_4:
				pos_light[1] -= 0.1f;
				break;
				
			case GLFW_KEY_KP_6:
				pos_light[1] += 0.1f;
				break;
		}
	}
}

