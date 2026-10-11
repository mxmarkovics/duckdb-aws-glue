#pragma once

#include "duckdb/common/multi_file/multi_file_list.hpp"
#include "duckdb/common/open_file_info.hpp"
#include "duckdb/function/table_function.hpp"
#include "duckdb/storage/statistics/node_statistics.hpp"

namespace duckdb {
class BaseStatistics;
struct GluePartitionInfo;
struct HiveScanInfo;

//! The recursive listing of 'directory', fetched page by page as it is read: every scan and the sample share it, so a
//! directory is listed once per query. With 'cache_in_connection' the listing is kept beyond the query, until a write
//! through this connection, a rollback, or (under glue_metadata_cache_scope='transaction') the end of the transaction
//! clears it.
shared_ptr<MultiFileList> GetDirectoryListing(ClientContext &context, const string &directory,
                                              bool cache_in_connection = false);
//! Drop the connection's cached directory listings (after a write through this connection)
void ClearHiveListingCache(ClientContext &context);
//! The partitions Glue registers for the table of 'info' in this query, fetched once for every scan of the table
shared_ptr<const vector<GluePartitionInfo>> GetTablePartitions(ClientContext &context, const HiveScanInfo &info);

//! Cardinality of a Hive scan: one directory of the table, measured once per query, scaled by the partitions read
unique_ptr<NodeStatistics> HiveScanCardinality(ClientContext &context, const FunctionData *bind_data_p);
//! Partition columns are answered from the partition values, every other column by the format's own function
unique_ptr<BaseStatistics> HivePartitionStatistics(ClientContext &context, TableFunctionGetStatisticsInput &input);

} // namespace duckdb
