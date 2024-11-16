#include "serialization/serialization.h"

namespace Serialization {
	void SaveCallback(SKSE::SerializationInterface* a_intfc)
	{
		if (!a_intfc->OpenRecord(StoredVersion, Version)) {
			logger::error("Failed to open Stored Version record.");
			return;
		}
	}

	void LoadCallback(SKSE::SerializationInterface* a_intfc)
	{
		std::uint32_t type;
		std::uint32_t version;
		std::uint32_t length;
		while (a_intfc->GetNextRecordInfo(type, version, length)) {
			if (version != Version) {
				logger::error("Loaded data is incompatible with plugin version!");
				return;
			}

			if (type == StoredVersion) {
				
			}
		}
	}

	void RevertCallback(SKSE::SerializationInterface* a_intfc)
	{
		(void)a_intfc;
	}

	std::string DecodeTypeCode(std::uint32_t a_typeCode)
	{
		constexpr std::size_t SIZE = sizeof(std::uint32_t);

		std::string sig;
		sig.resize(SIZE);
		char* iter = reinterpret_cast<char*>(&a_typeCode);
		for (std::size_t i = 0, j = SIZE - 2; i < SIZE - 1; ++i, --j) {
			sig[j] = iter[i];
		}
		return sig;
	}
}