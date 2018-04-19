// Copyright 2011 Jike.com  All Rights Reserved.
// Author: wangmeng@jike.com (Meng Wang)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
    int* data = (int*)malloc(sizeof(int));
    char* enlarge = (char*)realloc(data, 10);
    char abc[] = "abc";
    strncpy(enlarge, abc, 3);
    printf("%s\n", enlarge);
    return 0;
}

/* vim: set expandtab ts=2 sw=2 sts=2 tw=80: */
