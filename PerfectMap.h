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

			cerr << "Instantiating PerfectMap with "<<keysArrayLength<<" keys..." << endl << flush;

			cerr << "Traversing all keys... " << flush;
			unsigned keysLengthSum = 0;
			for (unsigned i=0; i<keysArrayLength; i++) {
				keysLengthSum += keysArray[i].length();
				cerr << keysArray[i] << endl << flush;
			}
			cerr << keysLengthSum << " sum of keys length\n" << flush;

			size_t numberOfUniqueKeys = PHF::uniq(keysArray, keysArrayLength);
			cerr << "\tsorted " << numberOfUniqueKeys << " unique keys\n" << flush;

			size_t keysPerIndex = 1;	// every key will correspond to a unique index
			size_t loadFactor = 100;	// 100% load factor
			PHF::init<_KeyType, false>(&_phf, keysArray, numberOfUniqueKeys, keysPerIndex, loadFactor, seed);
			cerr << "\tinitialized\n" << flush;
			PHF::compact(&_phf);
			cerr << "\tcompacted\n" << flush;

			valuesArrayLength = _phf.m;
			valuesArray       = new _ValueType[valuesArrayLength];
			clear();
			cerr << "\tcleared\n" << flush;
		}

		~PerfectMap() {
			delete[] valuesArray;
			PHF::destroy(&_phf);
		}

		// map methods
		//////////////

		_ValueType& operator [] (_KeyType key) {
			phf_hash_t index = PHF::hash<_KeyType>(&_phf, key);
	        return valuesArray[index];
	    }

		/** fill with zeroes all values */
		void clear() {
			memset(valuesArray, valuesArrayLength, sizeof(valuesArray[0]));
		}

		void erase(_KeyType key) {
			phf_hash_t index = PHF::hash<_KeyType>(&_phf, key);
			memset(&valuesArray[index], 1, sizeof(valuesArray[0]));
		}

	};
}
