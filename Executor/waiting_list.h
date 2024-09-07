#pragma once

#include <QSemaphore>

struct wait_item {
  int can_id;
  int index;
  int subindex;
  QSemaphore *sem;
  int result;
};

void insert_wait_item(const wait_item &item);
void remove_wait_item(const wait_item &item);
wait_item find_item(int can_id, int index, int subindex);

wait_item *find_item2(int can_id, int index, int subindex);

// static std::list<wait_item> wait_list;
