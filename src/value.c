#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "betree.h"
#include "value.h"
#include "utils.h"

struct betree_integer_list* make_integer_list()
{
    struct betree_integer_list* value = calloc(1, sizeof(*value));
    if(value == NULL) {
        fprintf(stderr, "%s calloc failed", __func__);
        abort();
    }
    return value;
}

struct betree_string_list* make_string_list()
{
    struct betree_string_list* value = calloc(1, sizeof(*value));
    if(value == NULL) {
        fprintf(stderr, "%s calloc failed", __func__);
        abort();
    }
    return value;
}

struct betree_segments* make_segments()
{
    struct betree_segments* value = calloc(1, sizeof(*value));
    if(value == NULL) {
        fprintf(stderr, "%s calloc failed", __func__);
        abort();
    }
    return value;
}

struct betree_frequency_caps* make_frequency_caps()
{
    struct betree_frequency_caps* value = calloc(1, sizeof(*value));
    if(value == NULL) {
        fprintf(stderr, "%s calloc failed", __func__);
        abort();
    }
    return value;
}

void add_integer_list_value(int64_t integer, struct betree_integer_list* list)
{
    if(list->count == 0) {
        list->integers = calloc(1, sizeof(*list->integers));
        if(list->integers == NULL) {
            fprintf(stderr, "%s calloc failed", __func__);
            abort();
        }
    }
    else {
        int64_t* integers = realloc(list->integers, sizeof(*list->integers) * (list->count + 1));
        if(integers == NULL) {
            fprintf(stderr, "%s realloc failed", __func__);
            abort();
        }
        list->integers = integers;
    }
    list->integers[list->count] = integer;
    list->count++;
}

const char* integer_list_value_to_string(struct betree_integer_list* list)
{
    char* string = NULL;
    for(size_t i = 0; i < list->count; i++) {
        char* new_string;
        if(i != 0) {
            if(asprintf(&new_string, "%s, %ld", string, list->integers[i]) < 0) {
                abort();
            }
            free(string);
        }
        else {
            if(asprintf(&new_string, "%ld", list->integers[i]) < 0) {
                abort();
            }
        }
        string = new_string;
    }
    return string;
}

void add_string_list_value(struct string_value string, struct betree_string_list* list)
{
    if(list->count == 0) {
        list->strings = calloc(1, sizeof(*list->strings));
        if(list->strings == NULL) {
            fprintf(stderr, "%s calloc failed", __func__);
            abort();
        }
    }
    else {
        struct string_value* strings
            = realloc(list->strings, sizeof(*list->strings) * (list->count + 1));
        if(strings == NULL) {
            fprintf(stderr, "%s realloc failed", __func__);
            abort();
        }
        list->strings = strings;
    }
    list->strings[list->count] = string;
    list->count++;
}

const char* string_list_value_to_string(struct betree_string_list* list)
{
    char* string = NULL;
    for(size_t i = 0; i < list->count; i++) {
        char* new_string;
        if(i != 0) {
            if(asprintf(&new_string, "%s, \"%s\"", string, list->strings[i].string) < 0) {
                abort();
            }
            free(string);
        }
        else {
            if(asprintf(&new_string, "\"%s\"", list->strings[i].string) < 0) {
                abort();
            }
        }
        string = new_string;
    }
    return string;
}

void add_segment(struct betree_segment* segment, struct betree_segments* list)
{
    if(list->size == 0) {
        list->content = calloc(1, sizeof(*list->content));
        if(list->content == NULL) {
            fprintf(stderr, "%s calloc failed", __func__);
            abort();
        }
    }
    else {
        struct betree_segment** content = realloc(list->content, sizeof(*list->content) * (list->size + 1));
        if(content == NULL) {
            fprintf(stderr, "%s realloc failed", __func__);
            abort();
        }
        list->content = content;
    }
    list->content[list->size] = segment;
    list->size++;
}

void add_frequency(struct betree_frequency_cap* frequency, struct betree_frequency_caps* list)
{
    if(list->size == 0) {
        list->content = calloc(1, sizeof(*list->content));
        if(list->content == NULL) {
            fprintf(stderr, "%s calloc failed", __func__);
            abort();
        }
    }
    else {
        struct betree_frequency_cap** content
            = realloc(list->content, sizeof(*list->content) * (list->size + 1));
        if(content == NULL) {
            fprintf(stderr, "%s realloc failed", __func__);
            abort();
        }
        list->content = content;
    }
    list->content[list->size] = frequency;
    list->size++;
}

struct betree_segment* make_segment(int64_t id, int64_t timestamp)
{
    struct betree_segment* segment = malloc(sizeof(*segment));
    segment->id = id;
    segment->timestamp = timestamp;
    return segment;
}

struct betree_frequency_cap* make_frequency_cap(const char* stype,
    uint32_t id,
    struct string_value namespace,
    int64_t timestamp,
    uint32_t value)
{
    struct betree_frequency_cap* frequency_cap = malloc(sizeof(*frequency_cap));
    enum frequency_type_e type = get_type_from_string(stype);
    frequency_cap->type = type;
    frequency_cap->id = id;
    frequency_cap->namespace = namespace;
    frequency_cap->timestamp_defined = true;
    frequency_cap->timestamp = timestamp;
    frequency_cap->value = value;
    return frequency_cap;
}

enum frequency_type_e get_type_from_string(const char* stype)
{
    if(strcmp(stype, "advertiser") == 0) {
        return FREQUENCY_TYPE_ADVERTISER;
    }
    else if(strcmp(stype, "advertiser:ip") == 0) {
        return FREQUENCY_TYPE_ADVERTISERIP;
    }
    else if(strcmp(stype, "campaign") == 0) {
        return FREQUENCY_TYPE_CAMPAIGN;
    }
    else if(strcmp(stype, "campaign:ip") == 0) {
        return FREQUENCY_TYPE_CAMPAIGNIP;
    }
    else if(strcmp(stype, "product") == 0) {
        return FREQUENCY_TYPE_PRODUCT;
    }
    else if(strcmp(stype, "product:ip") == 0) {
        return FREQUENCY_TYPE_PRODUCTIP;
    }
    else if(strcmp(stype, "flight") == 0) {
        return FREQUENCY_TYPE_FLIGHT;
    }
    else if(strcmp(stype, "flight:ip") == 0) {
        return FREQUENCY_TYPE_FLIGHTIP;
    }
    else {
        fprintf(stderr, "Invalid frequency type");
        abort();
    }
}

void free_integer_list(struct betree_integer_list* value)
{
    free(value->integers);
    free(value);
}

void free_string_list(struct betree_string_list* value)
{
    for(size_t i = 0; i < value->count; i++) {
        free((char*)value->strings[i].string);
    }
    free(value->strings);
    free(value);
}

void free_segment(struct betree_segment* value)
{
    free(value);
}

void free_segments(struct betree_segments* value)
{
    for(size_t i = 0; i < value->size; i++) {
        free(value->content[i]);
    }
    free(value->content);
    free(value);
}

void free_frequency_cap(struct betree_frequency_cap* value)
{
    free((char*)value->namespace.string);
    free(value);
}

void free_frequency_caps(struct betree_frequency_caps* value)
{
    for(size_t i = 0; i < value->size; i++) {
        free_frequency_cap(value->content[i]);
    }
    free(value->content);
    free(value);
}

void free_value(struct value value)
{
    switch(value.value_type) {
        case VALUE_IL: {
            free_integer_list(value.ilvalue);
            break;
        }
        case VALUE_SL: {
            free_string_list(value.slvalue);
            break;
        }
        case VALUE_S: {
            free((char*)value.svalue.string);
        }
        case VALUE_B:
        case VALUE_I:
        case VALUE_F: {
            break;
        }
        case VALUE_SEGMENTS: {
            free_segments(value.segments_value);
            break;
        }
        case VALUE_FREQUENCY: {
            free_frequency_caps(value.frequency_value);
            break;
        }
        default: {
            switch_default_error("Invalid value value type");
        }
    }
}

