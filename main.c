#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define STRINGIFY(x) #x

typedef struct __info {
    int id;
    const char *name;
} __info;

#define HIDE
void *_malloc(size_t sz)
{
#ifdef HIDE
    void *p = malloc(sz + sizeof(__info));
    assert(p != NULL && "Unable to malloc");
    __info i = {666, "devil"};
    p = &i;
    p += sizeof(__info);
    return (void*)p;
#else
    return malloc(sz);
#endif
}

void _free(void *p)
{
#ifdef HIDE
    p -= sizeof(__info);
#endif
    free(p);
}

int main(void)
{
#define malloc _malloc
#define free   _free
    int arrsz = 10;
    int *arr = malloc(arrsz*sizeof(int));
    assert(arr != NULL && "Unable to malloc");
    for (int i = 0; i < arrsz; ++i) {
        arr[i] = i + 1;
        printf("%s => %d ", STRINGIFY(arr[i]), arr[i]);
    }
    printf("\n");
    __info *_i = (__info*)(void*)&arr[-sizeof(__info)/sizeof(*arr)];
    printf("%s => %d\n", STRINGIFY(_i->id), _i->id);
    printf("%s => %s\n", STRINGIFY(_i->name), _i->name);
    return 0;
}
