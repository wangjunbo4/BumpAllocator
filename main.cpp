/*
 * @Description: 
 * @Author: Gtylcara
 * @Github: https://github.com/wangjunbo4
 * @Date: 2023-02-16 23:46:22
 * @LastEditors: Gtylcara
 * @LastEditTime: 2023-02-17 02:14:14
 * @FilePath: /mempool/main.cpp
 */

#include "src/allocator.h"
#include <iostream>
#include <string>
#include <vector>

int main() {
    TypedBumpAllocator<int> tba;
    auto i = tba.emplace(3);
    auto j = tba.emplace(3);
    std::cout << *i << " " << i;
    std::cout << *j << " " << j;

    TypedBumpAllocator<BumpAllocator> tba1;
    tba.emplace();
}