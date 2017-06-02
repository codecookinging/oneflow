#include "register/register_manager.h"
#include "register/blob.h"
#include "common/id_manager.h"
#include "common/job_desc.h"

namespace oneflow {

void RegstMgr::NewRegsts(const RegstDescProto& regst_desc_proto,
                         std::function<void(Regst*)> OneRegstDone) {
  std::shared_ptr<RtRegstDesc> runtime_regst_desc(new RtRegstDesc(regst_desc_proto));
  for (uint64_t i = 0; i < regst_desc_proto.register_num(); ++i) {
    Regst* regst = new Regst;
    regst->regst_desc_ = runtime_regst_desc;
    regst->regst_id_ =
        IDMgr::Singleton().NewRegstId(regst_desc_proto.regst_desc_id());

    uint64_t elem_size = sizeof(float);
    if (JobDesc::Singleton().floating_point_type() == kDouble) {
      elem_size = sizeof(double);
    }
    uint64_t elem_cnt = 0;
    std::vector<std::string> lbns;
    lbns.reserve(regst_desc_proto.lbn2shape().size());
    for (const auto& pair : regst_desc_proto.lbn2shape()) {
      const Shape* shape_ptr = runtime_regst_desc->GetShapePtrFromLbn(pair.first);
      lbns.push_back(pair.first);
      elem_cnt += shape_ptr->elem_cnt();
    }
    std::sort(lbns.begin(), lbns.end());
    std::pair<char*, std::function<void()>> allocation =
        MemoryAllocator::Singleton().Allocate(
            regst_desc_proto.mem_case(), elem_cnt * elem_size);

    uint64_t blob_idx = 0;
    for (const std::string& lbn : lbns) {
      const Shape* shape_ptr = runtime_regst_desc->GetShapePtrFromLbn(lbn);
      auto blob_ptr = of_make_unique<Blob>(allocation.first + blob_idx, shape_ptr);
      CHECK(regst->lbn2blob_.emplace(lbn, std::move(blob_ptr)).second);
      blob_idx += shape_ptr->elem_cnt() * elem_size;
    }
    regst->deleter_ = allocation.second;
    OneRegstDone(regst);
  }
}

}
