#ifndef OBJ_H
#define OBJ_H

typedef void (*destructor)(void*);
typedef struct _Obj Obj;

#define delete(ptr) \
	do { \
		Obj* obj = (Obj*)((char*) ptr - sizeof(Obj)); \
		obj->destructor(ptr); \
		free(obj); \
	} while(0)

struct _Obj {
	int ref_count;
	destructor destructor;
};

#endif
