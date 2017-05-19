#ifndef BLOCK_LIST_H
#define BLOCK_LIST_H

/*
A block list is a list implemented as a linked list of fixed size arrays called "blocks".
Each block can hold the number of elements specified in the macro.
*/

#define declare_block_list(type_prefix, type_name, block_size)                             \
    struct type_name##_list_block                                                          \
    {                                                                                      \
        struct type_name##_list_block *next;                                               \
        unsigned count;                                                                    \
        type_prefix type_name array[block_size];                                           \
    };                                                                                     \
    struct type_name##_list                                                                \
    {                                                                                      \
        struct type_name##_list_block *head;                                               \
        struct type_name##_list_block *tail;                                               \
    };                                                                                     \
    void type_name##_list_init(struct type_name##_list *queue);                            \
    char type_name##_list_push(struct type_name##_list *queue,                             \
        type_prefix type_name item);                                                       \
    struct {int trash;} /*force macro caller to put in a semi-colon*/


#define implement_block_list(type_prefix, type_name, block_size)                           \
    void type_name##_list_init(struct type_name##_list *queue)                             \
    {                                                                                      \
        queue->head = 0;                                                                   \
        queue->tail = 0;                                                                   \
    }                                                                                      \
    char type_name##_list_push(struct type_name##_list *queue,                             \
        type_prefix type_name item)                                                        \
    {                                                                                      \
        if(queue->tail == 0 || queue->tail->count >= block_size)                           \
        {                                                                                  \
            struct type_name##_list_block *new_block = (struct type_name##_list_block*)    \
                malloc(sizeof(struct type_name##_list_block));                             \
            if(new_block == 0)                                                             \
            {                                                                              \
                return -1; /* fail */                                                      \
            }                                                                              \
            new_block->next = 0;                                                           \
            new_block->count = 0;                                                          \
            if(queue->head == 0)                                                           \
            {                                                                              \
                queue->head = new_block;                                                   \
            }                                                                              \
            queue->tail = new_block;                                                       \
        }                                                                                  \
        queue->tail->array[queue->tail->count++] = item;                                   \
        return 0; /* success */                                                            \
    }                                                                                      \
    struct {int trash;} /*force macro caller to put in a semi-colon*/

#endif