// Copyright 2011 Jike.com  All Rights Reserved.
// Author: wangmeng@jike.com (Meng Wang)
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

struct block_meta {
    size_t size;
    struct block_meta* next;
    int free;
    int magic;
};
#define META_SIZE sizeof(struct block_meta)
void * global_base = NULL;
struct block_meta* find_free_block(struct block_meta** last, size_t size) {
    struct block_meta *current = (struct block_meta*)global_base;
    while (current && !(current->free && current->size >= size)) {
        *last = current;
        current = current->next;
    }
    return current;
}
struct block_meta* request_space(struct block_meta* last, size_t size) {
    struct block_meta* block;
    block = (struct block_meta*)sbrk(0);
    void* request = sbrk(size + META_SIZE);
    assert((void*)block == request);
    if (request == (void*) -1) {
        return NULL;
    }
    if (last) {
        last->next = block;
    }
    block->size = size;
    block->next = NULL;
    block->free = 0;
    block->magic = 0x12345678;
    return block;
}

void *malloc(size_t size) {
    printf("in tiny malloc\n");
    struct block_meta* block;
    if (size <= 0) {
        return NULL;
    }
    if (!global_base) {
        block = request_space(NULL, size);
        if (!block) {
            return NULL;
        }
        global_base = (void*)block;
    } else {
        struct block_meta* last = (struct block_meta*)global_base;
        block = find_free_block(&last, size);
        if (!block) {
            block = request_space(last, size);
            if (!block) {
                return NULL;
            }
        } else {
            block->free = 0;
            block->magic = 0x77777777;
        }
    }
    return block + 1;
}
struct block_meta* get_block_ptr(void* ptr) {
    return (struct block_meta*)ptr - 1;
}
void free(void* ptr) {
    if (!ptr) {
        return; 
    }
    struct block_meta* block_ptr = get_block_ptr(ptr);
    assert(block_ptr->free == 0);
    assert(block_ptr->magic == 0x77777777 || block_ptr->magic == 0x12345678);
    block_ptr->free = 1;
    block_ptr->magic = 0x55555555;
}

void* realloc(void* ptr, size_t size) {
    if (ptr == (void*)-1) {
        return malloc(size);
    }
    struct block_meta* prev_block = get_block_ptr(ptr);
    if (prev_block == (void*) -1) {
        return NULL;
    }
    // not thread safe
    if (prev_block->size > size) {
        return ptr;
    }
    void* ret = malloc(size);
    if (ret == (void*) -1) {
        return ret;
    }
    memcpy(ret, ptr, prev_block->size);
    return ret;
}

void* calloc(size_t nelem, size_t elsize) {
    void* ret = malloc(nelem * elsize);
    if (ret == (void*) -1) {
        return ret;
    }
    memset(ret, 0, nelem * elsize);
    return ret;
}

/* vim: set expandtab ts=2 sw=2 sts=2 tw=80: */
