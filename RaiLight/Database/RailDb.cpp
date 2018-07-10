#include "RaiLight\Database\RailDb.h"
#include "RaiLight\Database\DatabaseKeys.h"

#include "RaiLight\Utilities\Conversions.h"

#include <QDebug>
#include <QMessageLogger>
#include <QCoreApplication>

#include <leveldb\db.h>

namespace rail
{
    RailDb::~RailDb()
    {
        if (db)
        {
            delete db;
            db = nullptr;
        }
    }

    RailDb::RailDb()
    {
        leveldb::Options options;
        options.create_if_missing = true;
        leveldb::Status status = leveldb::DB::Open(options, "/storage/raildb", &db);

        if (!status.ok())
        {
            QMessageLogger().critical() << "Failed to open Database, closing application.";
            QCoreApplication::instance()->exit(1);
        }
    }

    bool RailDb::deleteValue(const char * key, const bool safeWrite)
    {
        auto writeOptions = leveldb::WriteOptions();
        writeOptions.sync = safeWrite;
        const auto status = db->Delete(leveldb::WriteOptions(), key);
        return status.ok();
    }

    bool RailDb::deleteValue(const char* key, const uint32_t index, const bool safeWrite)
    {
        return deleteValue(keyWithIndex(key, index), safeWrite);
    }

    const char * RailDb::keyWithIndex(const char * key, const uint32_t index)
    {
        return std::string(key + std::to_string(index)).c_str();
    }
}
