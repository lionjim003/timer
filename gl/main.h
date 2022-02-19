#ifndef main_h
#define main_h

#include "log.h"

#define FDATA_TYPE float
#include "fdata.h"

//=========================== window.c
GLFWwindow *window_context (int width, int height, char *title, GLFWmonitor *monitor, GLFWwindow *share);
void window_callback (GLFWwindow *window);
typedef void (GLFWrender (GLFWwindow *, float, void *));
void window_render (GLFWwindow *window, void *args, GLFWrender render_function);


//=========================== buffer.c
int 
BindBuffersData (unsigned n, unsigned *targets, unsigned *buffers, 
				 const char *filename, const char **head, const char **end, 
				 unsigned *num, unsigned *type, unsigned *usage);
				 

//=========================== shader.c
int shader_file (char *filename, int num, char **buffers);
unsigned shader_program (char *filename, unsigned num, unsigned *shader_type);
void shader_setf (unsigned shader, const char *name, float value);
void shader_seti (unsigned shader, const char *name, int value);
void shader_set3fv (unsigned shader, const char *name, float *value);
void shader_set_mat4fv (unsigned shader, const char *name, float *value);


//=========================== texture.c
GLuint texture_image (const char *filename, GLint internalFormat, GLenum dataFormat);


//=========================== matrix.c
void camera_key_input (int key, int action);
void camera_matrix (mat4 camera, float deltaTime);


#endif /* main_h */