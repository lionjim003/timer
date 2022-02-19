/***************************************************************
 * Function 'glerror' get error code from getGetError()
 *--------------------------------------------------------------
 * 'DEFS_IMPLEMENTATION'
 *  define to implementation function, otherwise just prototype.
 *
 **************************************************************/

#define LOG(a,b,...) printf(a": "b,##__VA_ARGS__)

#define GL(a) a;if(gl(__FILE__,__LINE__)) exit(0)

static unsigned
gl (char *file, int line) {
	unsigned e, i = 0;
	for (; (e = glGetError()); i++)
		LOG ("GL", "\b%s:%d: error %#.4x\n", file, line, e);
	return i;
}