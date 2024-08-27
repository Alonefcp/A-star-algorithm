#pragma once

namespace configuration
{
  static constexpr float s_fCellFill = 18.f;
  static constexpr float s_fCellMargin = 1.f;
  static constexpr float s_fCellSize = s_fCellFill + s_fCellMargin * 2.f;

  enum class ENeighborsType
  {
    Cross,
    All
  };
  static constexpr ENeighborsType s_eNeighborsType = ENeighborsType::Cross;
}
