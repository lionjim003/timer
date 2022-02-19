#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#define GLLIB_INLINE static inline __attribute((always_inline))

GLLIB_INLINE
size_t
TypeSize (GLenum type) {
	switch (type) {
		case GL_BYTE: return sizeof (char);
		case GL_UNSIGNED_BYTE: return sizeof (unsigned char);
		case GL_SHORT: return sizeof (short);
		case GL_UNSIGNED_SHORT: return sizeof (unsigned short);
		case GL_INT: return sizeof (int);
		case GL_UNSIGNED_INT: return sizeof (unsigned int);
		case GL_FIXED: return sizeof (int);
		case GL_HALF_FLOAT: return sizeof (float)/2;
		case GL_FLOAT: return sizeof (float);
		case GL_DOUBLE: return sizeof (double);
	}
	return sizeof (int);
}

// Number about vertex array
GLuint VertexArrayCount;
GLuint VertexArrayAmount;
GLuint VertexArrayLength;

// Address of vertex array
GLuint *VertexArrayCurrent;
GLuint *VertexArrayFirst;


#define ID 0
#define USE_STATUS 1
#define ATTRIB_POINTER 2
#define ATTRIB_POINTER_SIZE 3
#define BYTE(i, ptr) (((unsigned char *)(ptr))+i)
#define WORD(i, ptr) (((unsigned short *)(ptr))+i)

void
VertexArrayReport () {
	printf ("VertexArrayCurrent %p\n", VertexArrayCurrent);
	printf ("VertexArrayFirst %p\n", VertexArrayFirst);
	printf ("VertexArrayCount %u\n", VertexArrayCount);
	printf ("VertexArrayAmount %u\n", VertexArrayAmount);
	printf ("VertexArrayLength %u\n\n", VertexArrayLength);
}

void 
GetVertexArray (GLuint *array, GLuint target, GLuint *ptr) {

	switch (target) {
		case ID:
			*ptr = *array;
			return;
		
		case USE_STATUS://!  0x[0]0000000
			*ptr = *BYTE (0, array + 1) & 0x01;
			return;
			
		case ATTRIB_POINTER://!  0x00[00]0000
			*ptr = *BYTE (1, array + 1);
			return;
			
		case ATTRIB_POINTER_SIZE://!  0x0000[0000]
			*ptr = *WORD (1, array + 1);
			return;
			 
		default: 
			return;
	}
}

void
SetVertexArray (GLuint *array, GLuint target, GLuint *ptr) {
	
	switch (target) {
		case ID:
			*array = *ptr;
			return;
		
		case USE_STATUS://!  0x[0]0000000
			*BYTE (0, array + 1) =
				*BYTE (0, array + 1) & ~0x01 | *ptr != 0;
			return;
			
		case ATTRIB_POINTER://!  0x00[00]0000
			*BYTE (1, array + 1) = *BYTE (0, ptr);
			return;
			
		case ATTRIB_POINTER_SIZE://!  0x0000[0000]
			*WORD (1, array + 1) = *WORD (0, ptr);
			return;
			 
		default:
			return;
	}
}

GLLIB_INLINE
void
VertexArrayDataBuffer (GLboolean init, GLsizei n, GLsizei length, void *buffer) {
	
	VertexArrayCount = 0;
	VertexArrayAmount = n;
	VertexArrayLength = length;
	
	VertexArrayCurrent = 0;
	VertexArrayFirst = (GLuint *)buffer;
	
	if (init == GL_TRUE) {
		for (n *= length; --n >= 0;)
			VertexArrayFirst[n] = 0;
		
	} else {
		GLuint i, id, use, *a = VertexArrayFirst;
		while (--n >= 0) {
			GetVertexArray (a, ID, &id);
			GetVertexArray (a, USE_STATUS, &use);
			
			if (id) VertexArrayCount++;
			if (!id || !use) {
				for (i = 1; i < VertexArrayLength; i++) // Not working if ID location (size) change***
					a[i] = 0;
			}
			a += VertexArrayLength;
		}
	}
	return;
}

// glGenVertexArrays
GLLIB_INLINE
GLsizei 
GenVertexArrays (GLsizei n, GLuint *arrays) {
	
	GLuint *a, i, id, use, U, u, N = n;
	
	if (VertexArrayCount + n > VertexArrayAmount)
		n = VertexArrayAmount - VertexArrayCount;
	
	if (n) {
		glGenVertexArrays (n, arrays);
		VertexArrayCount += n;
	}
	
	a = VertexArrayFirst;
	for (i = 0, u = 0, U = n; i < VertexArrayAmount; i++) {
		
		/**  Write new_id to VertexArrays  */
		GetVertexArray (a, ID, &id);
		if (!id && u < U) {
			use = 1;
			SetVertexArray (a, USE_STATUS, &use);
			SetVertexArray (a, ID, arrays + u);
			u++;
			
		} else {// Not clear un_use vertex array data. Must call 'VertexArrayDataBuffer' function is wear.
			/**  Read reuse_id to arrays  */
			GetVertexArray (a, USE_STATUS, &use);
			if (!use && n < N) {
				use = 1;
				SetVertexArray (a, USE_STATUS, &use);
				arrays[n] = id;
				n++;
			}
			
			/**  break if new_id and reuse_id finsh  */
			else if (u == U && n == N) break;
		}
		a += VertexArrayLength;
	}
	return n;
}

// glBindVertexArray
GLLIB_INLINE
GLboolean
BindVertexArray (GLuint array) {
	
	GLuint i, id, *a = VertexArrayFirst;
	
	for (i = 0; i < VertexArrayAmount; i++) {
		GetVertexArray (a, ID, &id);
		if (id == array) {
			if (VertexArrayCurrent != a) {
				glBindVertexArray (array);
				VertexArrayCurrent = a;
			}
			return GL_TRUE;
		}
		a += VertexArrayLength;
	}
	return GL_FALSE;
}

/*
// check buffer bind or not
GLLIB_INLINE
int
VertexArrayFindBuffer (GLenum target, GLuint buffer) {
	
	int i, _TARGET = VertexArrayTargetIndex (target);
	
	for (i = 0; i < ARRAY_ITEM_AMOUNT; i++) {
		if (ARRAY [_CURRENT][_TARGET][i] == buffer) {
			return i;
		}
	}
	
	return -1;
}*/


// glBindVertexArray  &&  glBindBuffer
/*GLLIB_INLINE
GLsizei
VertexArrayBindBuffers (GLuint array, GLsizei n, GLenum *targets, GLuint *buffers) {
	
	BindVertexArray (array);
	int target, index;
	
	
	
}*/
	
	
// glEnableVertexAttribArray && glVertexAttribPointer
GLLIB_INLINE
void
VertexAttribPointers (GLsizei n, GLint *size, GLenum *type) {
	
	GLuint slot, offset;
	GetVertexArray (VertexArrayCurrent, ATTRIB_POINTER, &slot);
	GetVertexArray (VertexArrayCurrent, ATTRIB_POINTER_SIZE, &offset);
	
	for (int i = 0; i < n; i++) {
		glEnableVertexAttribArray (slot);
		glVertexAttribPointer (slot, size[i], type[i], GL_FALSE, 0, (void *)offset);
		
		offset += size[i] * TypeSize (type[i]);
		slot++;
	}
	SetVertexArray (VertexArrayCurrent, ATTRIB_POINTER, &slot);
	SetVertexArray (VertexArrayCurrent, ATTRIB_POINTER_SIZE, &offset);
	return;
}

/*
#undef VERTEX_ARRAY_AMOUNT
#undef ARRAY_ITEM_AMOUNT

#undef ARRAY		
#undef ARRAY_COUNT 
#undef ARRAY_AMOUNT

#undef _CURRENT

#undef _STATUS
#undef _USEING

#undef _POINTER
#undef _COUNT
#undef _SIZE
*/

#endif /* VERTEX_ARRAY_H */