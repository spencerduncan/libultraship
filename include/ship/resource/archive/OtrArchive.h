#ifdef INCLUDE_MPQ_SUPPORT

#pragma once

#undef _DLL

#include <string>

#include <stdint.h>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>
#include <mutex>
#include <StormLib.h>

#include "ship/resource/Resource.h"
#include "ship/resource/archive/Archive.h"

namespace Ship {
struct File;

class OtrArchive final : virtual public Archive {
  public:
    OtrArchive(const std::string& archivePath);
    ~OtrArchive();

    bool Open();
    bool Close();
    bool WriteFile(const std::string& filename, const std::vector<uint8_t>& data);

    std::shared_ptr<File> LoadFile(const std::string& filePath);
    std::shared_ptr<File> LoadFile(uint64_t hash);

  private:
    // StormLib gives no thread-safety guarantees for concurrent reads through a single archive
    // handle: reads go through the archive's shared TFileStream, whose FilePos/seek+read pair is
    // unsynchronized (BaseFile_Read). Every access to mHandle must hold mMutex, for the same
    // reason O2rArchive serializes its zip_t.
    std::mutex mMutex;
    HANDLE mHandle;
};
} // namespace Ship

#endif // INCLUDE_MPQ_SUPPORT
