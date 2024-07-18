//
// Created by Kok_PC on 18.7.2024 г..
//

#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

typedef enum {
    GET, POST, PUT, DEL
} rtype_t;

typedef struct {
    char *memory;
    size_t size;
} MemoryStruct;

int make_request(char *url, rtype_t type, char *body, char **response);

#endif //HTTP_HANDLER_H
