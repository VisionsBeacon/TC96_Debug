#include <iostream>
#include <mutex>
#include <list>

#include "waiting_list.h"

static std::list<wait_item> wait_list;
static std::mutex wait_list_mutex;

// 添加等待项
void insert_wait_item(const wait_item &item)
{
    std::lock_guard<std::mutex> lock(wait_list_mutex);
    // qDebug() << "Insert" << hex << item.can_id << "," << item.index << "," << item.subindex << "," << endl;
    wait_list.push_back(item);
}

// 删除等待项
void remove_wait_item(const wait_item &item)
{
    std::lock_guard<std::mutex> lock(wait_list_mutex);
    for (auto it = wait_list.begin(); it != wait_list.end(); it++)
    {
        if (it->can_id == item.can_id && it->index == item.index && it->subindex == item.subindex)
        {
            wait_list.erase(it);
            break;
        }
    }
}

// 查找等待项
wait_item find_item(int can_id, int index, int subindex)
{
    wait_item r = {0, 0, 0, nullptr};
    std::lock_guard<std::mutex> lock(wait_list_mutex);
    for (auto &i : wait_list)
    {
        if (i.can_id == can_id && i.index == index && i.subindex == subindex)
        {
            r = i;
            break;
        }
    }
    return r;
}

wait_item *find_item2(int can_id, int index, int subindex)
{
    wait_item *r = nullptr;

    std::lock_guard<std::mutex> lock(wait_list_mutex);
    for (auto &i : wait_list)
    {
        if (i.can_id == can_id && i.index == index && i.subindex == subindex)
        {
            r = &i;
            break;
        }
    }

    return r;
}
