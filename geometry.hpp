#pragma once
#include <cstdint>
#include <iostream>

class Vector {
 public:
  Vector();

  Vector(int64_t x_coord, int64_t y_coord);

  int64_t GetX() const;

  int64_t GetY() const;

  Vector& operator+=(const Vector& other_vector);

  Vector& operator-=(const Vector& other_vector);

  Vector& operator*=(int64_t multiplier);

 private:
  int64_t x_coord_;
  int64_t y_coord_;
};

int64_t operator*(const Vector& vector_1, const Vector& vector_2);

int64_t operator^(const Vector& vector_1, const Vector& vector_2);

Vector operator+(const Vector& vector_1, const Vector& vector_2);

Vector operator-(const Vector& vector_1, const Vector& vector_2);

Vector operator*(const Vector& vector_1, int64_t multiplier);

Vector operator*(int64_t multiplier, const Vector& vector_1);

Vector operator-(const Vector& vector);

class IShape {
 public:
  virtual void Move(const Vector& vec) = 0;

  virtual bool ContainsPoint(const class Point& point) const = 0;

  virtual bool CrossSegment(const class Segment& segment) const = 0;

  virtual IShape* Clone() const = 0;

  virtual ~IShape() = default;
};

class Point : public IShape {
 public:
  Point();

  Point(int64_t x_coord, int64_t y_coord);

  int64_t GetX() const;

  int64_t GetY() const;

  void Move(const Vector& vec) override;

  bool ContainsPoint(const class Point& point) const override;

  bool CrossSegment(const class Segment& segment) const override;

  IShape* Clone() const override;

 private:
  int64_t x_coord_;
  int64_t y_coord_;
};

bool operator==(const Point& point_1, const Point& point_2);

bool operator!=(const Point& point_1, const Point& point_2);

Vector operator-(const Point& point_1, const Point& point_2);

class Segment : public IShape {
 public:
  Segment(Point start, Point end);

  Point GetA() const;

  Point GetB() const;

  void Move(const Vector& vec) override;

  bool ContainsPoint(const class Point& point) const override;

  bool CrossSegment(const class Segment& segment) const override;

  IShape* Clone() const override;

 private:
  Point start_;
  Point end_;
};

class Line : public IShape {
 public:
  Line(Point point_1, Point point_2);

  int64_t GetA() const;

  int64_t GetB() const;

  int64_t GetC() const;

  void Move(const Vector& vec) override;

  bool ContainsPoint(const class Point& point) const override;

  bool CrossSegment(const class Segment& segment) const override;

  IShape* Clone() const override;

 private:
  Point point_1_;
  Point point_2_;
  int64_t a_coeff_;
  int64_t b_coeff_;
  int64_t c_coeff_;
};

class Ray : public IShape {
 public:
  Ray(Point point_1, Point point_2);

  Point GetA() const;

  Vector GetVector() const;

  void Move(const Vector& vec) override;

  bool ContainsPoint(const class Point& point) const override;

  bool CrossSegment(const class Segment& segment) const override;

  IShape* Clone() const override;

 private:
  Point point_1_;
  Vector directional_vec_;
};

class Circle : public IShape {
 public:
  Circle(Point center, size_t radius);

  Point GetCentre() const;

  size_t GetRadius() const;

  void Move(const Vector& vec) override;

  bool ContainsPoint(const class Point& point) const override;

  bool CrossSegment(const class Segment& segment) const override;

  IShape* Clone() const override;

 private:
  Point centre_;
  size_t radius_;
};
