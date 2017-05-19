#ifndef VECTOR_H
#define VECTOR_H

/*
A vector is a flat array of elements.
*/

#define declare_vector(type_prefix, type_name)                                    \
    struct type_name##_vector                                                     \
    {                                                                             \
        type_prefix type_name *array;                                             \
        unsigned count;                                                           \
        unsigned capacity;                                                        \
    };                                                                            \
    void type_name##_vector_init(struct type_name##_vector *vector);              \
    char type_name##_vector_expand(struct type_name##_vector *vector,             \
        unsigned new_min_capacity);                                               \
    char type_name##_vector_push(struct type_name##_vector *vector,               \
        type_prefix type_name item);                                              \
    struct {int trash;} /*force macro caller to put in a semi-colon*/


#define implement_vector(type_prefix, type_name, initial_size)                       \
    void type_name##_vector_init(struct type_name##_vector *vector)                  \
    {                                                                                \
        vector->array = NULL;                                                        \
        vector->count = 0;                                                           \
        vector->capacity = 0;                                                        \
    }                                                                                \
    char type_name##_vector_expand(struct type_name##_vector *vector,                \
        unsigned new_min_capacity)                                                   \
    {                                                                                \
        if(new_min_capacity > vector->capacity)                                      \
        {                                                                            \
            type_prefix type_name *new_array;                                        \
            unsigned new_capacity = initial_size + (vector->capacity*2);             \
            if(new_min_capacity > new_capacity)                                      \
            {                                                                        \
                new_capacity = new_min_capacity;                                     \
            }                                                                        \
            new_array = (type_prefix type_name*)malloc                               \
                (sizeof(type_prefix type_name) * new_capacity);                      \
            if(new_array == 0)                                                       \
            {                                                                        \
                return -1; /* fail */                                                \
            }                                                                        \
            memcpy(new_array, vector->array, vector->count);                         \
            vector->array = new_array;                                               \
            vector->capacity = new_capacity;                                         \
        }                                                                            \
        return 0; /* success */                                                      \
    }                                                                                \
    char type_name##_vector_push(struct type_name##_vector *vector,                  \
        type_prefix type_name item)                                                  \
    {                                                                                \
        if(type_name##_vector_expand(vector, vector->count + 1))                     \
        {                                                                            \
            return -1; /* fail */                                                    \
        }                                                                            \
        vector->array[vector->count++] = item;                                       \
        return 0; /* success */                                                      \
    }                                                                                \
    struct {int trash;} /*force macro caller to put in a semi-colon*/

#endif