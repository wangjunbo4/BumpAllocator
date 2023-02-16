/*
 * @Description: 
 * @Author: Gtylcara
 * @Github: https://github.com/wangjunbo4
 * @Date: 2023-02-16 23:46:34
 * @LastEditors: Gtylcara
 * @LastEditTime: 2023-02-17 02:00:33
 * @FilePath: /mempool/src/allocator.cpp
 */

#include "allocator.h"
#include <cstddef>
#include <new>

BumpAllocator::BumpAllocator() {
    head = allocSegment(nullptr, InitialSize);
    end = reinterpret_cast<std::byte *>(head) + InitialSize;
}

BumpAllocator::~BumpAllocator() {
    auto seg = head;
    while (seg) {
        auto pre = seg->pre;
        operator delete(seg);
        seg = pre;
    } 
}

std::byte *BumpAllocator::allocate(size_t size, size_t alignment) {
    auto base = alignPtr(head->current, alignment);
    auto next = base + size;
    if (next > end) {
        return allocateSlow(size, alignment);
    }
    head->current = next;
    return base;
}

std::byte *BumpAllocator::allocateSlow(size_t size, size_t alignment) {
    if (size > (SegmentSize >> 1)) {
        size = (size + alignment - 1) & ~(alignment - 1);
        head->pre = allocSegment(head->pre, size + sizeof(Segment));
        return alignPtr(head->pre->current, alignment);
    }

    head = allocSegment(head, SegmentSize);
    end = reinterpret_cast<std::byte *>(head) + SegmentSize;
    return allocate(size, alignment);
}

// Segment(size) -> [ (pre + current *)(sizeof(Segment)) | data ]
BumpAllocator::Segment *BumpAllocator::allocSegment(Segment *pre, size_t size) {
    auto seg = reinterpret_cast<Segment *>(operator new(size));
    seg->pre = pre;
    seg->current = reinterpret_cast<std::byte *>(seg) + sizeof(Segment);
    return seg;
}

