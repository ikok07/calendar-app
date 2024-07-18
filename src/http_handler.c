//
// Created by Kok_PC on 18.7.2024 г..
//
#include <stdlib.h>
#include <curl/curl.h>
#include "../include/http_handler.h"


size_t read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    MemoryStruct *upload = userdata;
    if (size == 0 || nmemb == 0 || size * nmemb < 1 || upload->size == 0) {
        return 0;
    }

    size_t real_size = size * nmemb < upload->size ? size * nmemb : upload->size;
    memcpy(ptr, upload->memory, real_size);
    upload->memory += real_size;
    upload->size -= real_size;

    return real_size;
}

size_t write_cb(void *data, size_t size, size_t nmemb, void *userdata) {
    const size_t real_size = size * nmemb;
    MemoryStruct *response = userdata;
    char *temp_memory = realloc(response->memory, response->size + real_size + 1);
    if (temp_memory == NULL) {
        perror("Failed to allocate memory for response data!");
        return CURL_WRITEFUNC_ERROR;
    }
    response->memory = temp_memory;
    memcpy(&response->memory[response->size], data, real_size);
    response->size += real_size;
    response->memory[response->size] = '\0';
    return real_size;
}

int make_request(char *url, rtype_t type, char *body, char **response) {
    CURL *curl = curl_easy_init();
    CURLcode result;

    // char *safeUrl = curl_easy_escape(curl, url, 0);
    // if (safeUrl == NULL) {
    //     perror("Failed to encode url!");
    //     return 1;
    // }

    struct curl_slist *list = NULL;
    list = curl_slist_append(list, "Content-Type: application/json");
    list = curl_slist_append(list, "Accept: application/json");

    MemoryStruct upload_data;
    if (body) {
        upload_data.memory = malloc(strlen(body) + 1);
        memcpy(upload_data.memory, body, strlen(body) + 1);
        upload_data.size = strlen(body);
    }
    MemoryStruct response_data = {malloc(1), 0};

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response_data);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
    if (body != NULL) {
        switch (type) {
            case POST:
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, upload_data.memory);
            break;
            case PUT:
                curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
                curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_cb);
                curl_easy_setopt(curl, CURLOPT_READDATA, (void*)&upload_data);
                curl_easy_setopt(curl, CURLOPT_INFILESIZE, (long)upload_data.size);
            break;
            default:
                perror("Inavlid request type! If you use GET do not provide body!");
            return 1;
        }
    }

    result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
        perror("HTTP Request failed!");
        return 1;
    }

    char *temp_memory = realloc(*response, response_data.size);
    if (temp_memory == NULL) {
        perror("Failed to allocate enough memory for response!");
        return 1;
    }
    *response = temp_memory;
    memcpy(*response, response_data.memory, response_data.size + 1);

    if (type != PUT) free(upload_data.memory);
    free(response_data.memory);
    curl_slist_free_all(list);
    curl_easy_cleanup(curl);
    return 0;
}