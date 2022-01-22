#ifndef CHATTER_FIRMWARE_REPO_H
#define CHATTER_FIRMWARE_REPO_H

#include <Arduino.h>
#include <FS.h>
#include "../Types.hpp"
#include "../Model/Entity.hpp"
#include "../Model/Message.h"
#include <type_traits>

template<typename T>
class Repo {
	static_assert(std::is_base_of<Entity, T>::value, "Specified repository type doesn't describe an entity!");

public:
	Repo(const char* directory);

	bool add(const T& object);
	bool update(const T& object);
	bool remove(UID_t uid);
	T get(UID_t uid);
	bool exists(UID_t uid);

protected:
	virtual bool write(fs::File& file, const T& object);
	virtual bool read(fs::File& file, T& object);

private:
	const String directory;

	String getPath(UID_t uid);

};

#endif //CHATTER_FIRMWARE_REPO_H
