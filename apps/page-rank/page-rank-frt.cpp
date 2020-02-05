#include <stdlib.h>

#include <algorithm>

#include <frt.h>
#include <tlp.h>

#include "page-rank.h"

void PageRank(
    Pid num_partitions, tlp::mmap<uint64_t> metadata,
    tlp::async_mmap<tlp::vec_t<VertexAttrAligned, kVertexVecLen>> vertices,
    tlp::async_mmap<tlp::vec_t<Edge, kEdgeVecLen>> edges_0,
    tlp::async_mmap<tlp::vec_t<Edge, kEdgeVecLen>> edges_1,
    tlp::async_mmap<tlp::vec_t<Update, kUpdateVecLen>> updates_0,
    tlp::async_mmap<tlp::vec_t<Update, kUpdateVecLen>> updates_1) {
  auto instance =
      fpga::Invoke(getenv("BITSTREAM"), num_partitions,
                   fpga::ReadWrite(metadata.get(), metadata.size()),
                   fpga::ReadWrite(vertices.get(), vertices.size()),
                   fpga::WriteOnly(edges_0.get(), edges_0.size()),
                   fpga::WriteOnly(edges_1.get(), edges_1.size()),
                   fpga::ReadOnly(updates_0.get(), updates_0.size()),
                   fpga::ReadOnly(updates_1.get(), updates_1.size()));
  LOG(INFO) << "kernel time: " << instance.ComputeTimeSeconds() << "s";
}
