#pragma once


#include <string.h>
#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif

#include "canfestival.h"

//防止def.h中的Tx与qmath.h中的Tx混乱
// #undef Tx

#ifdef __cplusplus
};
#endif

class args_item
{
public:
    std::string name;
    int index;
    int sub_index;
    int data_type;

public:
    virtual void *data() { return nullptr; }
    virtual int size() { return 0; };
    virtual int get_data_type() { return 0; }
};

class args_int : public args_item
{
private:
    int value;

public:
    args_int(const std::string &name, int index, int sub_index, int data) {
        this->name = name;
        this->index = index;
        this->sub_index = sub_index;
        this->value = data;
    };
    void *data() { return &value; };
    int size() { return sizeof(value); };
    int get_data_type() { return uint32; }
};

class args_array : public args_item
{
private:
    uint8_t value[32];

public:
    args_array(const std::string &name, int index, int sub_index, void *data) {
        this->name = name;
        this->index = index;
        this->sub_index = sub_index;
        memcpy(value, data, sizeof(value));
    };

    void *data() { return value; }
    int size() { return sizeof(value); };
    int get_data_type() { return octet_string; }
};
