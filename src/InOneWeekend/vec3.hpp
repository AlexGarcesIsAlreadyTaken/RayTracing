#ifndef VEC3_H
#define VEC3_H

class vec3 {
	private:
		double e[3];
	
	public:
		vec3() : e{0, 0, 0} {}
		vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}
		vec3(double v) : e{v, v, v} {} 

		double x() const { return e[0]; }
		double y() const { return e[1]; }
		double z() const { return e[2]; }

		vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
		double operator[](int i) const { return e[i]; }
		double& operator[](int i) { return e[i]; }

		vec3& operator+=(const vec3& v) {
			e[0] += v[0]; e[1] += v[1]; e[2] += v[2];
			return *this;
		}

		vec3& operator*=(double t) {
			e[0] *= t; e[1] *= t; e[2] *= t;
			return *this;
		}

		vec3& operator/=(double t) {
			return *this *= (1/t);
		}

		double length() const {
			return std::sqrt(length_squared());
		}

		double length_squared() const {
			return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
		}

		bool near_zero() {
			double s = 1e-8;
			return std::fabs(e[0]) < s and std::fabs(e[1]) < s and std::fabs(e[2]) < s;
		}

		static vec3 random() {
			return vec3(utilities::random_double(), utilities::random_double(), utilities::random_double());
		}

		static vec3 random(double min, double max) {
			return vec3(utilities::random_double(min, max), utilities::random_double(min, max), utilities::random_double(min, max));
		}
};

using point3 = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << '(' <<  v.x() << ", " << v.y() << ", " << v.z() << ')'; 
}

inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

inline vec3 operator/(const vec3& u, const vec3& v) {
	return vec3(u.x() / v.x(), u.y() / v.y(), u.z() / v.z());
}

inline vec3 operator*(const vec3& u, double t) {
	return vec3(u.x()*t, u.y()*t, u.z()*t);
}

inline vec3 operator*(double t, const vec3& u) {
	return u * t;
}


inline vec3 operator/(const vec3& u, double d) {
	return u * (1/d);
}

inline double dot(const vec3& u, const vec3& v) {
	return u.x()*v.x() + u.y()*v.y() + u.z()*v.z();
}

inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(
		u.y()*v.z() - u.z()*v.y(),
		u.z()*v.x() - u.x()*v.z(),
		u.x()*v.y() - u.y()*v.x()
	);
}

inline vec3 unit_vector(const vec3& v) {
	return v / v.length();
}

inline vec3 random_unit_vector() {
	while (true) {
		point3 random_point = vec3::random(-1, 1);
		double lensq = random_point.length_squared();
		if (lensq > 1e-160 && lensq <= 1.0) {
			return unit_vector(random_point);
		}
	}
}

inline vec3 random_on_hemisphere(const vec3& normal) {
	vec3 on_unit_sphere = random_unit_vector();
	if (dot(on_unit_sphere, normal) > 0.0) return on_unit_sphere;
	return -on_unit_sphere;
}

inline vec3 random_in_unit_disk() {
	while(true) {
		vec3 p = vec3(utilities::random_double(), utilities::random_double(), 0);
		if (p.length_squared() < 1) return p;
	}
}

inline vec3 reflect(const vec3& v, const vec3& n) {
	vec3 b = dot(v, n)*n;
	return v - 2*b;
}

inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
	double cos_theta = std::fmin(dot(-uv, n), 1.0);
	vec3 r_out_perp = etai_over_etat * (uv + cos_theta*n);
	vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}

#endif
