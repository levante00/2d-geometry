#include "geometry.hpp"

// -------------------------------------------
// Vector

Vector::Vector() : x_coord_(0), y_coord_(0) {}

Vector::Vector(int64_t x_coord, int64_t y_coord)
    : x_coord_(x_coord), y_coord_(y_coord) {}

int64_t Vector::GetX() const { return x_coord_; }

int64_t Vector::GetY() const { return y_coord_; }

int64_t operator*(const Vector& vector_1, const Vector& vector_2) {
  return vector_1.GetX() * vector_2.GetX() + vector_1.GetY() * vector_2.GetY();
}

int64_t operator^(const Vector& vector_1, const Vector& vector_2) {
  return vector_1.GetX() * vector_2.GetY() - vector_1.GetY() * vector_2.GetX();
}

Vector& Vector::operator+=(const Vector& other_vector) {
  x_coord_ += other_vector.GetX();
  y_coord_ += other_vector.GetY();

  return *this;
}

Vector& Vector::operator-=(const Vector& other_vector) {
  x_coord_ -= other_vector.GetX();
  y_coord_ -= other_vector.GetY();

  return *this;
}

Vector operator+(const Vector& vector_1, const Vector& vector_2) {
  return Vector(vector_1.GetX() + vector_2.GetX(),
                vector_1.GetY() + vector_2.GetY());
}

Vector operator-(const Vector& vector_1, const Vector& vector_2) {
  return Vector(vector_1.GetX() - vector_2.GetX(),
                vector_1.GetY() - vector_2.GetY());
}

Vector& Vector::operator*=(int64_t multiplier) {
  x_coord_ *= multiplier;
  y_coord_ *= multiplier;

  return *this;
}

Vector operator*(const Vector& vector, int64_t multiplier) {
  return Vector(vector.GetX() * multiplier, vector.GetY() * multiplier);
}

Vector operator*(int64_t multiplier, const Vector& vector) {
  return Vector(vector.GetX() * multiplier, vector.GetY() * multiplier);
}

Vector operator-(const Vector& vector) {
  return Vector(-vector.GetX(), -vector.GetY());
}

// -------------------------------------------
// Point

Point::Point() : x_coord_(0), y_coord_(0) {}

Point::Point(int64_t x_coord, int64_t y_coord)
    : x_coord_(x_coord), y_coord_(y_coord) {}

int64_t Point::GetX() const { return x_coord_; }

int64_t Point::GetY() const { return y_coord_; }

void Point::Move(const Vector& vec) {
  x_coord_ += vec.GetX();
  y_coord_ += vec.GetY();
}

bool Point::ContainsPoint(const Point& point) const { return *this == point; }

bool Point::CrossSegment(const Segment& segment) const {
  return segment.ContainsPoint(*this);
}

IShape* Point::Clone() const { return new Point(*this); }

bool operator==(const Point& point_1, const Point& point_2) {
  return (point_1.GetX() == point_2.GetX() && point_1.GetY() == point_2.GetY());
}

bool operator!=(const Point& point_1, const Point& point_2) {
  return !(point_1 == point_2);
}

Vector operator-(const Point& point_1, const Point& point_2) {
  return Vector(point_1.GetX() - point_2.GetX(),
                point_1.GetY() - point_2.GetY());
}

// -------------------------------------------
// Segment

Segment::Segment(Point start, Point end) : start_(start), end_(end) {}

Point Segment::GetA() const { return start_; }

Point Segment::GetB() const { return end_; }

void Segment::Move(const Vector& vec) {
  start_.Move(vec);
  end_.Move(vec);
}

bool Segment::ContainsPoint(const Point& point) const {
  Vector segment_vec = end_ - start_;
  Vector point_start_formed_vec = point - start_;
  int64_t segment_length_sqaured = segment_vec * segment_vec;

  if (((point_start_formed_vec ^ segment_vec) != 0) ||
      ((point_start_formed_vec * segment_vec) < 0)) {
    return false;
  }

  return ((point_start_formed_vec * segment_vec <= segment_length_sqaured) &&
          (segment_length_sqaured != 0 || start_ == point));
}

bool Segment::CrossSegment(const Segment& segment) const {
  // start_, end_. segment.GetA(), segment.GetB() => 1,2,3,4
  bool orientations_123 = ((segment.GetA() - start_) ^ (end_ - start_)) > 0;
  bool orientations_124 = ((segment.GetB() - start_) ^ (end_ - start_)) > 0;
  bool orientations_341 =
      ((start_ - segment.GetA()) ^ (segment.GetB() - segment.GetA())) > 0;
  bool orientations_342 =
      ((end_ - segment.GetA()) ^ (segment.GetB() - segment.GetA())) > 0;

  if (orientations_123 != orientations_124 &&
      orientations_341 != orientations_342) {
    return true;
  }

  return segment.ContainsPoint(start_) || segment.ContainsPoint(end_) ||
         ContainsPoint(segment.GetA()) || ContainsPoint(segment.GetB());
}

IShape* Segment::Clone() const { return new Segment(*this); }

// -------------------------------------------
// Line

Line::Line(Point point_1, Point point_2)
    : point_1_(point_1),
      point_2_(point_2),
      a_coeff_(point_1.GetY() - point_2.GetY()),
      b_coeff_(point_2.GetX() - point_1.GetX()),
      c_coeff_((point_2.GetY() - point_1.GetY()) * point_1.GetX() +
               (point_1.GetX() - point_2.GetX()) * point_1.GetY()) {}

int64_t Line::GetA() const { return a_coeff_; }

int64_t Line::GetB() const { return b_coeff_; }

int64_t Line::GetC() const { return c_coeff_; }

void Line::Move(const Vector& vec) {
  point_1_.Move(vec);
  point_2_.Move(vec);
  c_coeff_ = -(a_coeff_ * point_1_.GetX() + b_coeff_ * point_1_.GetY());
}

bool Line::ContainsPoint(const Point& point) const {
  return (a_coeff_ * point.GetX() + b_coeff_ * point.GetY() + c_coeff_ == 0);
}

bool Line::CrossSegment(const Segment& segment) const {
  int64_t line_eq_val_a = a_coeff_ * segment.GetA().GetX() +
                          b_coeff_ * segment.GetA().GetY() + c_coeff_;
  int64_t line_eq_val_b = a_coeff_ * segment.GetB().GetX() +
                          b_coeff_ * segment.GetB().GetY() + c_coeff_;

  return !((line_eq_val_a * (line_eq_val_b)) > 0);
}

IShape* Line::Clone() const { return new Line(*this); }

// -------------------------------------------
// Ray

Ray::Ray(Point point_1, Point point_2)
    : point_1_(point_1), directional_vec_(point_2 - point_1) {}

Point Ray::GetA() const { return point_1_; }

Vector Ray::GetVector() const { return directional_vec_; }

void Ray::Move(const Vector& vec) { point_1_.Move(vec); }

bool Ray::ContainsPoint(const Point& point) const {
  Vector formed_directional_vec = point - point_1_;

  return (((formed_directional_vec ^ directional_vec_) == 0) &&
          ((formed_directional_vec * directional_vec_) >= 0));
}

bool Ray::CrossSegment(const Segment& segment) const {
  Line ray_line(point_1_, Point(point_1_.GetX() + directional_vec_.GetX(),
                                point_1_.GetY() + directional_vec_.GetY()));
  Line segment_line(segment.GetA(), segment.GetB());

  if (ray_line.CrossSegment(segment)) {
    if (ContainsPoint(segment.GetA()) || ContainsPoint(segment.GetB()) ||
        segment.ContainsPoint(point_1_)) {
      return true;
    }

    int64_t x_multiplied_denom = ray_line.GetB() * segment_line.GetC() -
                                 segment_line.GetB() * ray_line.GetC();
    int64_t y_multiplied_denom = segment_line.GetA() * ray_line.GetC() -
                                 ray_line.GetA() * segment_line.GetC();

    Point intersection_point_multiplied_denom(x_multiplied_denom,
                                              y_multiplied_denom);

    Vector intersection_vec_multiplied_denom =
        intersection_point_multiplied_denom - point_1_;
    if ((directional_vec_ ^ intersection_vec_multiplied_denom) == 0 &&
        (directional_vec_ * intersection_vec_multiplied_denom) >= 0) {
      return true;
    }
  }

  return false;
}

IShape* Ray::Clone() const { return new Ray(*this); }

// -------------------------------------------
// Circle

Circle::Circle(Point center, size_t radius)
    : centre_(center), radius_(radius) {}

Point Circle::GetCentre() const { return centre_; }

size_t Circle::GetRadius() const { return radius_; }

void Circle::Move(const Vector& vec) { centre_.Move(vec); }

bool Circle::ContainsPoint(const Point& point) const {
  Vector formed_central_vec = point - centre_;

  return (formed_central_vec.GetX() * formed_central_vec.GetX() +
              formed_central_vec.GetY() * formed_central_vec.GetY() <=
          static_cast<int64_t>(radius_) * static_cast<int64_t>(radius_));
}

bool Circle::CrossSegment(const Segment& segment) const {
  Vector segment_vec = segment.GetB() - segment.GetA();
  Vector formed_central_vec_a = centre_ - segment.GetA();
  Vector formed_central_vec_b = centre_ - segment.GetB();

  int64_t closest_point_dist_squared = 0;

  int64_t radius_squared =
      static_cast<int64_t>(radius_) * static_cast<int64_t>(radius_);
  int64_t formed_central_vec_a_length_squared =
      formed_central_vec_a * formed_central_vec_a;
  int64_t formed_central_vec_b_length_squared =
      formed_central_vec_b * formed_central_vec_b;

  if ((radius_squared > formed_central_vec_a_length_squared) &&
      (radius_squared > formed_central_vec_b_length_squared)) {
    return false;
  }

  if ((formed_central_vec_a * segment_vec) <= 0) {
    closest_point_dist_squared = (formed_central_vec_a_length_squared);
  } else if ((formed_central_vec_b * segment_vec) >= 0) {
    closest_point_dist_squared = (formed_central_vec_b_length_squared);
  } else {
    closest_point_dist_squared = (formed_central_vec_a ^ segment_vec) *
                                 (formed_central_vec_a ^ segment_vec);
    return closest_point_dist_squared <=
           (segment_vec * segment_vec) * (radius_squared);
  }

  return closest_point_dist_squared <= radius_squared;
}

IShape* Circle::Clone() const { return new Circle(*this); }
