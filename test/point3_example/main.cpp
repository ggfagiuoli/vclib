/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include <iostream>

#include <mgp/geometry/point.h>

int main()
{
	mgp::Point3<double> p1;
	p1 << 1, 0, 0;

	mgp::Point3<double> p2(0, 1, 0);

	std::cout << "Point p1 is [" << p1 << "].\n";
	std::cout << "Point p2 is [" << p2 << "].\n";

	std::cout << "p1 norm is " << p1.norm() << ".\n";
	std::cout << "p1 squared norm is " << p1.squaredNorm() << "\n";

	std::cout << "p1 dot p2: " << p1.dot(p2) << "\n";

	mgp::Point3 p3 = p1.cross(p2);

	std::cout << "p1 cross p2: [" << p3 << "]\n";

	return 0;
}
