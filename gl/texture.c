#include "depend.h"
#include "log.h"

GLuint texture_image (const char *filename, GLint internalFormat, GLenum dataFormat) {

	int width, height, nrChannels;
	unsigned char *data;
	
	stbi_set_flip_vertically_on_load (1);
	
	if ((data = stbi_load (filename, &width, &height, &nrChannels, 0)) == NULL) {
		LOG ("stb_image", "\b%s: Failed to load texture from file\n", filename);
		return 0;
	}
	
	GLuint texture;
	GL (glGenTextures (1, &texture));
	GL (glBindTexture (GL_TEXTURE_2D, texture));
	
	GL (glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
	GL (glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));
	GL (glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
	GL (glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	
	GL (glTexImage2D (GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data));
	GL (glGenerateMipmap (GL_TEXTURE_2D));
	
	stbi_image_free (data);
	return texture;
}

