#ifndef MERKLE_H
#define MERKLE_H

#include <bitcoin/system.hpp>

bc::hash_digest create_merkle(bc::hash_list& merkle);

#endif