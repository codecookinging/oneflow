#include <vector>
#include "tensorflow/compiler/xla/shape.h"
#include "tensorflow/compiler/xla/layout_util.h"
#include "tensorflow/compiler/xla/shape_util.h"
#include "oneflow/core/common/shape.h"
#include "oneflow/core/common/data_type.h"
#include "oneflow/core/compiler/of2xla/xla_data_type.h"
#include "oneflow/core/compiler/of2xla/xla_shape.h"

namespace oneflow {
namespace mola {

Shape XlaShapeToOfShape(const xla::Shape &xla_shape) {
  CHECK(!xla_shape.IsTuple());
  int rank = xla_shape.rank();
  std::vector<int64_t> dimensions(rank);
  for (int i = 0; i < rank; ++i) {
    dimensions[i] = xla_shape.dimensions(i);
  }
  return Shape(dimensions);
}

xla::Shape OfShapeToXlaShape(const Shape &shape, DataType dtype) {
  xla::PrimitiveType type = DataTypeToPrimitiveType(dtype);
  return OfShapeToXlaShape(shape, type);
}

xla::Shape OfShapeToXlaShape(const Shape &shape, xla::PrimitiveType type) {
  int rank = shape.NumAxes();
  std::vector<long long> layout(rank);
  std::vector<long long> dimensions(rank);
  for (int i = 0; i < rank; ++i) {
    dimensions[i] = shape.At(i);
  }

  std::iota(layout.rbegin(), layout.rend(), 0);
  return xla::ShapeUtil::MakeShapeWithLayout(type, dimensions, layout);
}

}  // namespace mola
}  // namespace oneflow
