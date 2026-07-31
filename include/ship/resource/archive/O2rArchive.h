#pragma once

#undef _DLL

#include <string>
#include <stdint.h>
#include <mutex>
#include <string>

#include "zip.h"

#include "ship/resource/File.h"
#include "ship/resource/Resource.h"
#include "ship/resource/archive/Archive.h"

namespace Ship {
struct File;

class O2rArchive final : virtual public Archive {
  public:
    O2rArchive(const std::string& archivePath);
    ~O2rArchive();

    bool Open();
    bool Close();
    bool WriteFile(const std::string& filename, const std::vector<uint8_t>& data);

    std::shared_ptr<File> LoadFile(const std::string& filePath);
    std::shared_ptr<File> LoadFile(uint64_t hash);

  private:
    // libzip gives no thread-safety guarantees for concurrent operations on a single zip_t.
    // Every access to mZipArchive must hold mMutex: the resource thread pool and the render
    // thread (via LoadResourceProcess) can both read this archive at the same time, and
    // unsynchronized zip_fread calls corrupt both readers' buffers.
    std::mutex mMutex;
    zip_t* mZipArchive = nullptr;
};
} // namespace Ship
