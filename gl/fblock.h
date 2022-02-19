/***************************************************************
 * Function 'fblock' reads block of text from the stream. 
 *--------------------------------------------------------------
 * 'FBLOCK_IMPLEMENTATION'
 *  define to implementation function, otherwise just prototype.
 *
 **************************************************************/
 
#if !defined (FBLOCK_H) || defined (FBLOCK_IMPLEMENTATION)

 #if !(defined (FILE) || defined (NULL) || defined(EOF))
  #include <stdio.h>
 #endif
 
 #define FBLOCK() \
 char *fblock (FILE *stream, const char *head, const char *end, \
				int contain_head, int contain_end, unsigned index, char *ptr)
 
 #if defined (FBLOCK_IMPLEMENTATION)
  FBLOCK () {

	if (!stream || !head || !end || !ptr)
		return NULL;
	
	unsigned c, e, i = 0, n = 0, t = 0, in_block = 0;
	
	while ((c = fgetc (stream)) != EOF) {
		
		if (!in_block) {
			if (head[t] != c) {
				if (contain_head) n = 0;
				t = 0;
				continue;
			}
			if (head[++t] == '\0') {
				in_block = 1;
				t = 0;
			}
			if (!contain_head)
				continue;
			
			
		} else {
			t = end[t] == c ? t + 1 : 0;
			
			if (!contain_end && i == index && t == 1)
				e = n;
			
			if (end[t] == '\0') {
				if (i++ != index) {
					in_block = 0;
					t = 0;
					continue;
				}
				contain_end ? 
					ptr[n++] = c 
					: (n = e);
				break;
			}
		}
		
		if (i == index)
			ptr[n++] = c;
	}
	
	if (i >= index)
		ptr[n] = '\0';
	
	return i > index ? ptr : NULL;
 }
 #else
  #define FBLOCK_H
  FBLOCK ();
 #endif /* FBLOCK_IMPLEMENTATION */
 
 /*! Clear temporary marcos */
 #undef FBLOCK
 
#endif /* FBLOCK_H */
