use core::fmt;

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
    pub const ZERO: Point2d = Point2d { x: 0, y: 0 };

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

impl std::fmt::Display for Point2d {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "({}, {})", self.x, self.y)
    }
}

impl std::ops::Add for Point2d {
    type Output = Point2d;
    fn add(self, rhs: Self) -> Self {
        Point2d::new(self.x + rhs.x, self.y + rhs.y)
    }
}

impl std::ops::AddAssign for Point2d {
    fn add_assign(&mut self, rhs: Self) {
        self.x += rhs.x;
        self.y += rhs.y;
    }
}

impl std::ops::Sub for Point2d {
    type Output = Point2d;
    fn sub(self, rhs: Self) -> Self {
        Point2d::new(self.x - rhs.x, self.y - rhs.y)
    }
}

impl std::ops::SubAssign for Point2d {
    fn sub_assign(&mut self, rhs: Self) {
        self.x -= rhs.x;
        self.y -= rhs.y;
    }
}

impl std::ops::Mul<i32> for Point2d {
    type Output = Point2d;
    fn mul(self, rhs: i32) -> Self {
        Point2d::new(self.x * rhs, self.y * rhs)
    }
}

impl std::ops::MulAssign<i32> for Point2d {
    fn mul_assign(&mut self, rhs: i32) {
        self.x *= rhs;
        self.y *= rhs;
    }
}

impl std::ops::Div<i32> for Point2d {
    type Output = Point2d;
    fn div(self, rhs: i32) -> Self {
        Point2d::new(self.x / rhs, self.y / rhs)
    }
}

impl std::ops::DivAssign<i32> for Point2d {
    fn div_assign(&mut self, rhs: i32) {
        self.x /= rhs;
        self.y /= rhs;
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

pub struct Monitor {
    start: std::time::Instant,
}

impl Monitor {
    #[allow(dead_code)]
    pub fn start() -> Self {
        Self {
            start: std::time::Instant::now(),
        }
    }

    fn print_elapsed(&self) {
        let elapsed = self.start.elapsed();
        let ms = elapsed.as_millis();
        if ms < 1 {
            println!("  Elapsed: {}µs", elapsed.as_micros());
        } else if ms < 10 {
            println!("  Elapsed: {:.2}ms", elapsed.as_secs_f64() * 1000.0);
        } else if elapsed.as_secs() < 1 {
            println!("  Elapsed: {}ms", elapsed.as_millis());
        } else {
            println!("  Elapsed: {:.2}s", elapsed.as_secs_f64());
        }
    }
}

impl Drop for Monitor {
    fn drop(&mut self) {
        self.print_elapsed();
    }
}
