use itertools::{Itertools, MultiProduct};

#[derive(Clone, Copy, Debug, Eq, Hash, Ord, PartialEq, PartialOrd)]
pub struct Point2d {
    pub x: i32,
    pub y: i32,
}

#[allow(dead_code)]
impl Point2d {
    pub const NORTH: Point2d = Point2d { x: 0, y: -1 };
    pub const SOUTH: Point2d = Point2d { x: 0, y: 1 };
    pub const WEST: Point2d = Point2d { x: -1, y: 0 };
    pub const EAST: Point2d = Point2d { x: 1, y: 0 };

    pub const fn new(x: i32, y: i32) -> Self {
        Point2d { x: x, y: y }
    }

    pub fn rotate90_left(&self) -> Self {
        Point2d::new(self.y, -self.x)
    }

    pub fn rotate90_right(&self) -> Self {
        Point2d::new(-self.y, self.x)
    }

    pub fn distance(&self, other: &Point2d) -> u32 {
        self.x.abs_diff(other.x) + self.y.abs_diff(other.y)
    }
}

impl std::ops::Add<Point2d> for Point2d {
    type Output = Point2d;
    fn add(self, rhs: Self) -> Self {
        Point2d::new(self.x + rhs.x, self.y + rhs.y)
    }
}

#[allow(dead_code)]
pub fn product_repeat<I>(it: I, repeat: usize) -> MultiProduct<I>
where
    I: Iterator + Clone,
    I::Item: Clone,
{
    std::iter::repeat(it).take(repeat).multi_cartesian_product()
}

#[allow(dead_code)]
pub trait ProductRepeat: Iterator + Clone
where
    Self::Item: Clone,
{
    fn product_repeat(self, repeat: usize) -> MultiProduct<Self> {
        std::iter::repeat(self)
            .take(repeat)
            .multi_cartesian_product()
    }
}

impl<T: Iterator + Clone> ProductRepeat for T where T::Item: Clone {}
