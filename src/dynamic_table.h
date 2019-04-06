#ifndef DYNAMIC_TABLE_H_
#define DYNAMIC_TABLE_H_

#include "table.h"

template <typename K, typename V>
class DynamicHashTable : public HashTable<K, V> {
 private:
	struct Entry {
		bool not_empty;
		K key;
		V value;
		std::unique_ptr<Entry> next;

		Entry() : not_empty(false), next(nullptr) {}
	};
	Entry *entries;
	const std::size_t size;
	const std::function<uint64_t(const K &)> hash;
	std::size_t collisions;
 public:
	DynamicHashTable(std::function<uint64_t(const K &)> hash, std::size_t sz) : size(sz), hash(hash), collisions(0) {
		this->entries = new Entry[sz];
	}

	virtual ~DynamicHashTable() {
		delete[] this->entries;
	}

	bool has(const K &key) const override {
		uint64_t i = this->hash(key) % this->size;
		if (!this->entries[i].not_empty) {
			return false;
		} else if (this->entries[i].key == key) {
			return true;
		} else {
			Entry *next = this->entries[i].next.get();
			while (next) {
				if (next->key == key) {
					return true;
				}
				next = next->next.get();
			}
		}
		return false;
	}

	const V &get(const K &key) const override {
		uint64_t i = this->hash(key) % this->size;
		if (!this->entries[i].not_empty) {
			throw typename HashTable<K, V>::KeyNotFound();
		} else if (this->entries[i].key == key) {
			return this->entries[i].value;
		} else {
			Entry *next = this->entries[i].next.get();
			while (next) {
				if (next->key == key) {
					return next->value;
				}
				next = next->next.get();
			}
		}
		std::terminate();
	}

	void put(const K &key, const V &value) override {
		uint64_t i = this->hash(key) % this->size;
		if (!this->entries[i].not_empty) {
			this->entries[i].not_empty = true;
			this->entries[i].key = key;
			this->entries[i].value = value;
		} else if (this->entries[i].key == key) {
			this->entries[i].value = value;
		} else {
			Entry *next = this->entries[i].next.get();
			while (next) {
				if (next->key == key) {
					next->value = value;
				}
				next = next->next.get();
			}
			if (next == nullptr) {
				std::unique_ptr<Entry> nEntry = std::make_unique<Entry>();
				nEntry->key = key;
				nEntry->value = value;
				nEntry->next = std::move(this->entries[i].next);
				this->entries[i].next = std::move(nEntry);
				this->collisions++;
			}
		}
	}

	std::size_t getCollisions() const override {
		return this->collisions;
	}
};

#endif
