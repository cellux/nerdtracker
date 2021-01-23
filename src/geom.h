#ifndef NT_GEOM_H
#define NT_GEOM_H

template <typename T>
struct TPoint {
  T x { 0 };
  T y { 0 };
};

using Point = TPoint<int>;

template <typename T>
struct TRect {
  T x { 0 };
  T y { 0 };
  T w { 0 };
  T h { 0 };
};

using Rect = TRect<int>;

#endif
