/*
 * @Description:
 * @Author: Gtylcara
 * @Github: https://github.com/wangjunbo4
 * @Date: 2023-02-16 23:46:41
 * @LastEditors: Gtylcara
 * @LastEditTime: 2023-02-17 02:06:45
 * @FilePath: /mempool/src/allocator.h
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <new>
#include <utility>

struct BumpAllocator {

	BumpAllocator();
	~BumpAllocator();

	std::byte *allocate(size_t size, size_t alignment);

	// protected:	
	static constexpr size_t InitialSize = 512;
	static constexpr size_t SegmentSize = 4096;

	struct Segment {
		Segment *pre;
		std::byte *current;
	};

	Segment *head;
	std::byte *end;


	static std::byte *alignPtr(std::byte *ptr, size_t alignment) {
    	return reinterpret_cast<std::byte *>(
       		(reinterpret_cast<uintptr_t>(ptr) + alignment - 1) & ~(alignment - 1));
  	}

	std::byte *allocateSlow(size_t size, size_t alignment);

	static Segment *allocSegment(Segment *pre, size_t size);
};

template<typename T>
struct TypedBumpAllocator : public BumpAllocator {
	TypedBumpAllocator() = default;
	~TypedBumpAllocator() {
		auto seg = head;
		while (seg) {
			for (T *curr = reinterpret_cast<T *>(seg + 1); curr != reinterpret_cast<T *>(seg->current); curr++) {
				curr->~T();
			}
			seg = seg->pre;
		} 
	}

	template<typename ...Args>
	T *emplace(Args &&...args) {
		return new (allocate(sizeof(T), alignof(T))) T(std::forward<Args>(args)...);
	}
	
};