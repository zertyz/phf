// Map-Like implementation using the phf (perfect hash map) library

#include "phf.h"

namespace PHF {

	template <typename _KeyType, typename _ValueType>
	class PerfectMap {

	private:

		// perfect hash structure
		phf _phf;

		// keys
		_KeyType* keysArray;
		unsigned  keysArrayLength;

		// values
		_ValueType* valuesArray;
		unsigned    valuesArrayLength;

	public:

		PerfectMap(_KeyType* keysArray, unsigned keysArrayLength, phf_seed_t seed)
				: keysArray(keysArray)
				, keysArrayLength(keysArrayLength) {

			size_t numberOfUniqueKeys = PHF::uniq(keysArray, keysArrayLength);

			size_t keysPerIndex = 1;	// every key will correspond to a unique index
			size_t loadFactor = 100;	// 100% load factor
			PHF::init(&_phf, keysArray, numberOfUniqueKeys, keysPerIndex, loadFactor, seed);
			PHF::compact(&_phf);

			valuesArrayLength = _phf.m;
			valuesArray       = new _ValueType[valuesArrayLength];
		}

		~PerfectMap() {
			delete[] valuesArray;
			PHF::destroy(&_phf);
		}

		const _ValueType& operator [] (_KeyType key) const {
			phf_hash_t index = hash(&_phf, key);
	        return valuesArray[index];
	    }

	};
}