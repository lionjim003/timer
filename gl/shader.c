#include "depend.h"
#include "log.h"

int shader_file (char *filename, int num, char **buffers) {
	
	if (filename == NULL || num <= 0 || buffers == NULL)
		return 0;
	
	FILE *file = fopen (filename, "r");
	if (file == NULL)
		return 0;
	
	fpos_t pos, start;
	char *version = "#version";
	int c, i = -1, j = 0, n = 0, w = 0, v = 0, end = 0;
	
	// Loop shader file
	while ((c = fgetc (file)) != EOF && i < num) {
	
		// Line feet
		if (c == '\n') {
			w += w ? 1 : 2; n++; 
			continue;
		}
		
		// White space
		if (c == ' ' || c == '\t') {
			if (!w) w = 1;
			continue;
		}
		
		// Comment
		if (c == '/') {
			fgetpos (file, &pos);
			c = fgetc (file);
			
			// Comment line
			if (c == '/') {
				while ((c = fgetc (file)) != EOF)
					if (c == '\n') {
						w += w ? 1 : 2; n++;
						break;
					}
				continue;
			}
			// Comment block
			else if (c == '*') {
				while ((c = fgetc (file)) != EOF) {
					if (c == '*' && fgetc (file) == '/') {
						if (!w) w = 1;
						break;
					} 
					else if (c == '\n') {
						w += w ? 1 : 2; n++;
					}
				}
				continue;
			}
			fsetpos (file, &pos);
			c = '/';
		}
		
		// Shader start
		{
			if (v >= 2)
				v = (!w && c == version[v]) ? v + 1 : 0;
			
			else if (v == 1)
				v = (w < 2 && c == 'v') ? 2 : 0;
			
			else if (v == 0 && c == '#') {
				fgetpos (file, &start);
				end = j;
				v = 1;
			}
			if (v == 8) {
				fsetpos (file, &start);
				if (++i > 0)
					buffers[i - 1][end] = '\0';
				if (i >= num)
					break;
				for (j = 0, end = n; end > 0; end--)
					buffers[i][j++] = '\n';
				c = '#';
				v = 0;
			}
		}

		// First shader
		if (i < 0) {
			w = 0;
			continue;
		}
				
		// Write buffer
		if (w) {
			if (w < 2) {
				buffers[i][j++] = ' ';
				w = 0;
			} 
			else for (w--; w > 0; w--)
				buffers[i][j++] = '\n';
		}
		buffers[i][j++] = c;
	}
	
	buffers[i][j] = '\0';
	
	fclose (file);
	return (i >= num) ? num : i + 1;
}

unsigned shader_program (char *filename, unsigned num, unsigned *shader_type) {
	
	FILE *file = fopen (filename, "r");
	if (!file) return -1;
	
	fseek (file, 0L, SEEK_END);
	size_t size = ftell (file);
	fclose (file);
	
	char buffer[size * num + num], *shader_source[num];
	unsigned i, shader[num + 1], success;	
	
	for (i = 0; i < num; i++)
		shader_source[i] = buffer + (size * i) + (i != 0);
	
	if (shader_file (filename, num, shader_source) != num) {
		LOG ("ERROR", "\b%s: Failed to read %d shader in one file\n", filename, num);
		return -1;
	}
	
	for (i = 0; i < num; i++)
		shader[i] = glCreateShader (shader_type[i]);
	shader[num] = glCreateProgram ();
	
	for (i = 0; i < num; i++) {
		glShaderSource (shader[i], 1, (const char * const*) (shader_source + i), NULL);
		glCompileShader (shader[i]);
		glGetShaderiv (shader[i], GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog (shader[i], size, NULL, buffer);
			LOG ("GL", "\b%s: shader %d: Compilation failed\n%s", filename, ++i, buffer);
			return -1;
		}
		glAttachShader (shader[num], shader[i]);
		glLinkProgram (shader[num]);
		glDeleteShader (shader[i]);
	}
	
	glGetProgramiv (shader[num], GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog (shader[num], size, NULL, buffer);
		LOG ("GL", "\b%s: shader program: Linking failed\n%s", filename, buffer);
		return -1;
	}
	
	return shader[num];
}

void shader_setf (unsigned shader, const char *name, float value) {
	glUniform1f (glGetUniformLocation (shader, name), value);
}

void shader_seti (unsigned shader, const char *name, int value) {
	glUniform1i (glGetUniformLocation (shader, name), value);
}

void shader_set3fv (unsigned shader, const char *name, float *value) {
	glUniform3fv (glGetUniformLocation (shader, name), 1, value);
}

void shader_set_mat4fv (unsigned shader, const char *name, float *value) {
	glUniformMatrix4fv (glGetUniformLocation (shader, name), 1, GL_FALSE, value);
}
