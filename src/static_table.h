#ifndef STATIC_TABLE_H_
#define STATIC_TABLE_H_

#include "table.h"
#include <cmath>

template <typename K, typename V>
class StaticHashTable : public HashTable<K, V> {
 private:
	class Rehasher {
	 private:
		std::size_t current;
		std::size_t size;
		uint64_t rnd;
		uint64_t mask;
	 public:
		Rehasher(std::size_t base, std::size_t size) : current(base), size(size), rnd(1) {
			this->mask = static_cast<uint64_t>(std::pow(2, std::ceil(std::log2(static_cast<double>(size))) + 2)) - 1;
			this->next();
		}
		std::size_t next() {
			this->rnd *= 5;
			this->rnd &= this->mask;
			this->current = (this->current + (this->rnd >> 2)) % this->size;
			return this->current;
		}
	};

	struct Entry {
		bool not_empty;
		K key;
		V value;
		Entry() : not_empty(false) {}
	};

	const std::function<uint64_t(const K &)> hash;
	const std::size_t size;
	std::size_t current_size;
	Entry *table;
	std::size_t collisions;
 public:
	class OutOfMemory : public std::exception {
	 public:
		OutOfMemory() : msg("Out of memory") {}
		const char *what() const noexcept override {
			return msg.c_str();
		}
	 private:
		std::string msg;
	};

	StaticHashTable(std::function<uint64_t(const K &)> hash, std::size_t sz) : hash(hash), size(sz), current_size(0), collisions(0) {
		this->table = new Entry[sz];
	}

	virtual ~StaticHashTable() {
		delete[] this->table;
	}

	bool has(const K &key) const override {
		std::size_t index = this->hash(key) % this->size;
		if (!this->table[index].not_empty) {
			return false;
		} else {
			Rehasher rehasher(index, this->size);
			for (std::size_t counter = 0; counter < this->size; counter++) {
				if (this->table[index].key == key) {
					return true;
				}
				index = rehasher.next();
			}
			return false;
		}
	}

	const V &get(const K &key) const override {
		std::size_t index = this->hash(key) % this->size;
		if (!this->table[index].not_empty) {
			throw typename HashTable<K, V>::KeyNotFound();
		} else {
			Rehasher rehasher(index, this->size);
			for (std::size_t counter = 0; counter < this->size; counter++) {
				if (this->table[index].key == key) {
					return this->table[index].value;
				}
				index = rehasher.next();
			}
			throw typename HashTable<K, V>::KeyNotFound();
		}
	}

	void put(const K &key, const V &value) override {
		if (this->current_size == this->size) {
			throw OutOfMemory();
		}
		std::size_t index = this->hash(key) % this->size;
		bool collision = false;
		Rehasher rehasher(index, this->size);
		while (this->table[index].not_empty) {
			index = rehasher.next();
			collision = true;
		}
		this->table[index].key = key;
		this->table[index].value = value;
		this->table[index].not_empty = true;
		this->current_size++;
		if (collision) {
			this->collisions++;
		}
	}

	std::size_t getCollisions() const override {
		return this->collisions;
	}
};

#endif
