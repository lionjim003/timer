#include "depend.h"

#define FBLOCK_IMPLEMENTATION
#include "fblock.h"

#define FDATA_IMPLEMENTATION
#define FDATA_TYPE float
#include "fdata.h"
#define FDATA_TYPE GLuint
#include "fdata.h"

int 
BindBuffersData (unsigned n, unsigned *targets, unsigned *buffers, 
				 const char *filename, const char **head, const char **end, 
				 unsigned *num, unsigned *type, unsigned *usage) {
	
	FILE *file = fopen (filename, "rb");
	if (!file) return -1;
	
	fseek (file, 0L, SEEK_END);
	char text [ftell (file) + 1];
	rewind (file);
	
	for (int i = 0; i < n; i++) {
		
		if (fblock (file, head[i], end[i], 0, 0, 0, text) == NULL)
			return -1;
		
		FILE *tmp = tmpfile ();
		fputs (text, tmp);
		rewind (tmp);
		
		glBindBuffer (targets[i], buffers[i]);
		
		if (type[i] == GL_FLOAT) {
			float data[num[i]];
			num[i] = fdata_float (tmp, "%f", " \t\n", num[i], data);
			glBufferData (targets[i], sizeof(float) * num[i], data, usage[i]);
			continue;
		}
		if (type[i] == GL_UNSIGNED_INT) {
			unsigned int data[num[i]];
			num[i] = fdata_GLuint (tmp, "%u", " \t\n", num[i], data);
			glBufferData (targets[i], sizeof(unsigned int) * num[i], data, usage[i]);
			continue;
		}
	}
	return 0;
}