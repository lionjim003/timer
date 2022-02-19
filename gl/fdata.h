/***************************************************************
 * Function 'fdata_type' reads array of data from the stream. 
 *--------------------------------------------------------------
 * 'FDATA_TYPE' 
 *  require define to specify the type of data read from stream.
 *
 * 'FDATA_IMPLEMENTATION' 
 *  define to implementation function, otherwise just prototype.
 *
 **************************************************************/
 
#if !(defined (FILE) || defined(EOF))
 #include <stdio.h>
#endif

#if !defined (FDATA_TYPE)
 #error required define 'FDATA_TYPE' before include header
#else

/*! Function prototype */
 #define FDATA(type) \
  unsigned fdata_##type (FILE *file, const char *format, const char *delims, unsigned num, type *ptr)

/*! Create function implementation or prototype */
 #define CREATE_FDATA_FUNCTION(type) FDATA(type) 
 
 
 #if !defined (FDATA_IMPLEMENTATION)
  CREATE_FDATA_FUNCTION (FDATA_TYPE);
 #else
  CREATE_FDATA_FUNCTION (FDATA_TYPE) {

	if (!file || !format || !delims || num < 0 || !ptr)
		return 0;
	
	unsigned c, i = 0, n = 0, in_data = 0;

	while ((c = fgetc (file)) != EOF && i < num) {

		if (c == '\r')
			continue; 

		if (in_data) {
			for (n = 0; c != delims[n] && delims[n] != '\0'; n++)
				;
			if (delims[n] != '\0')
				in_data = 0;

			continue;
		}

		for (n = 0; c != delims[n]; n++) {

			if (delims[n] != '\0')
				continue;

			ungetc (c, file);
			if (fscanf (file, format, ptr + i))
				i++;

			in_data = 1;
			break;
		}
	}
	return i;
  }
 #endif /* FDATA_IMPLEMENTATION */
	
 /*! Clear temporary marcos */
 #undef CREATE_FDATA_FUNCTION
 #undef FDATA_TYPE
 #undef FDATA_BODY
 #undef FDATA

#endif /* FDATA_TYPE */
