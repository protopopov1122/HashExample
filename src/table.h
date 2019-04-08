#ifndef TABLE_H_
#define TABLE_H_

#include <cinttypes>
#include <memory>
#include <functional>
#include <exception>
#include <vector>

template <typename K, typename V>
class HashTable {
 public:
	virtual ~HashTable() = default;
	virtual bool has(const K &) const = 0;
	virtual const V &get(const K&) const = 0;
	virtual void put(const K &, const V &) = 0;
	virtual std::size_t getCollisions() const = 0;
	virtual void getLayout(std::vector<uint32_t> &) const = 0;

	class KeyNotFound : public std::exception {
	 public:
		KeyNotFound() : msg("Key not found") {}
		const char *what() const noexcept override {
			return msg.c_str();
		}
	 private:
		std::string msg;
	};
};

#endif
