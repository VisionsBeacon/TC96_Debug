#pragma once

#include <QString>

#ifdef __cplusplus
extern "C" {
#endif

#include "canfestival.h"

#ifdef __cplusplus
};
#endif

class args_item {
public:
    QString name;
    int index;
    int sub_index;
    int data_type;

public:
    virtual void *data() { return nullptr; }
    virtual int size() { return 0; };
    virtual int get_data_type() { return 0; }
};



class args_int : public args_item {
private:
    int value;

public:
    args_int(const QString &name, int index, int sub_index, int data) {
        this->name = name;
        this->index = index;
        this->sub_index = sub_index;
        this->value = data;
    };
    void *data() { return &value; };
    int size() { return sizeof(value); };
    int get_data_type() { return uint32; }
};



class args_array : public args_item {
private:
    uint8_t value[32];

public:
    args_array(const QString &name, int index, int sub_index, void *data) {
        this->name = name;
        this->index = index;
        this->sub_index = sub_index;
        memcpy(value, data, sizeof(value));
    };

    void *data() { return value; }
    int size() { return sizeof(value); };
    int get_data_type() { return octet_string; }
};
