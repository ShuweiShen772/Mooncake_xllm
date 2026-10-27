// HA metadata rebuild: shared types for client<->master metadata rebuild.
#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "replica.h"
#include "types.h"

namespace mooncake {

using RebuildVersion = std::pair<int64_t, uint64_t>;

// A locally owned allocation and its object metadata.
struct LocalReplicaMeta {
    Replica::Descriptor replica;
    uint64_t size{0};
    ObjectDataType data_type{ObjectDataType::UNKNOWN};
    std::string group_id;
    std::string tenant_id{"default"};
    RebuildVersion version{};
};

// One key's rebuild entry: key + its replica location(s). Descriptor is already
// serializable (YLT_REFL at replica.h:477), so it travels over RPC/notify
// as-is.
struct KeyReplicaEntry {
    std::string key;
    std::string tenant_id{"default"};
    uint64_t size{0};
    ObjectDataType data_type{ObjectDataType::UNKNOWN};
    std::string group_id;
    std::vector<Replica::Descriptor> replicas;
    RebuildVersion version{};
    KeyReplicaEntry() = default;
};
YLT_REFL(KeyReplicaEntry, key, tenant_id, size, data_type, group_id, replicas,
         version);

enum class RebuildNotifyOp : uint8_t { UPSERT = 0 };

struct RebuildNotify {
    std::string sender_client_id;
    RebuildNotifyOp op{RebuildNotifyOp::UPSERT};
    std::vector<KeyReplicaEntry> entries;
    RebuildNotify() = default;
};
YLT_REFL(RebuildNotify, sender_client_id, op, entries);

}  // namespace mooncake
