#include <random>

PlainObject sort(bool ascending = true) const {
	EIGEN_STATIC_ASSERT_VECTOR_ONLY(MatrixBase<Derived>);
	PlainObject result(derived());
	result.sortInPlace(ascending);
	return result;
}

MatrixBase<Derived>& sortInPlace(bool ascending = true) {
	EIGEN_STATIC_ASSERT_VECTOR_ONLY(MatrixBase<Derived>);
	std::sort(derived().data(), derived().data() + derived().size());
	if (!ascending)
		this->reverseInPlace();
	return *this;
}

// If template not specified, min and max arguments must be real-typed.
template <typename RealType, typename GeneratorType>
MatrixBase<Derived>& uniformRealRand(const RealType & min, const RealType & max, GeneratorType & generator) {
	std::uniform_real_distribution<RealType> d(min, max);
	for (int r = 0; r < (int)rows(); r++) {
		for (int c = 0; c < (int)cols(); c++) {
			derived().operator()(r, c) = (Scalar)d(generator);
		}
	}
	return *this;
}

// If template not specified, min and max arguments must not be real-typed.
template <typename IntType, typename GeneratorType>
MatrixBase<Derived>& uniformIntRand(const IntType & min, const IntType & max, GeneratorType & generator) {
	std::uniform_int_distribution<IntType> d(min, max);
	for (int r = 0; r < (int)rows(); r++) {
		for (int c = 0; c < (int)cols(); c++) {
			derived().operator()(r, c) = (Scalar)d(generator);
		}
	}
	return *this;
}

// Check if all elements of the matrix are finite (no NaN of INF values).
bool isFinite() const {
	for (int r = 0; r < (int)rows(); r++) {
		for (int c = 0; c < (int)cols(); c++) {
			if ((std::isnan(derived().operator()(r,c))) || (std::isinf(derived().operator()(r, c))))
				return false;
		}
	}
	return true;
}

// Get the correlation between two vectors (Eigen vectors).
double corr(const MatrixBase<Derived> & b) const {
	EIGEN_STATIC_ASSERT_VECTOR_ONLY(MatrixBase<Derived>);
	assert(derived().size() == b.size());
	double num = 0, den1 = 0, den2 = 0, aTerm, bTerm, 
		aMean = derived().mean(), bMean = b.mean();
	for (int i = 0; i < derived().size(); i++) {
		aTerm = (double)derived().operator()(i) - aMean;
		bTerm = (double)b(i) - bMean;
		num += aTerm*bTerm;
		den1 += aTerm*aTerm;
		den2 += bTerm*bTerm;
	}
	if (den1*den2 == 0)
		return 1;	// The correlation is actually not defined in this case!
	return num / (double)std::sqrt(den1*den2);
}

// Get the covariance between two vectors (Eigen vectors).
double cov(const MatrixBase<Derived> & b) const {
	EIGEN_STATIC_ASSERT_VECTOR_ONLY(MatrixBase<Derived>);
	assert(derived().size() == b.size());
	if ((int)derived().size() <= 1)
		return 0;
	double muA = derived().mean(), muB = b.mean(), result = 0;
	for (int i = 0; i < (int)derived().size(); i++)
		result += (derived().operator()(i) - muA)*(b(i) - muB);
	result /= (double)(derived().size() - 1);
	return result;
}

// Sample variance of vector, using Bessel's correction
double var() const {
	EIGEN_STATIC_ASSERT_VECTOR_ONLY(MatrixBase<Derived>);
	double mu = derived().mean(), result = 0;
	for (int i = 0; i < (int)derived().size(); i++) {
		double temp = (derived().operator()(i) - mu);
		result += temp*temp;
	}
	return result / (double)(derived().size() - 1);
}

// Standard Deviation of a vector
double stddev() const {
	EIGEN_STATIC_ASSERT_VECTOR_ONLY(MatrixBase<Derived>);
	return std::sqrt(derived().var());
}

// Standard Error of a vector
double stderror() const {
	EIGEN_STATIC_ASSERT_VECTOR_ONLY(MatrixBase<Derived>);
	return std::sqrt(derived().var()) / std::sqrt(derived().size());
}

// Get the angle between two vectors in radians
double angleBetween(const MatrixBase<Derived> & b) const {
	EIGEN_STATIC_ASSERT_VECTOR_ONLY(MatrixBase<Derived>);
	assert(derived().size() == b.size());
	return (double)std::acos(derived().dot(b) / (derived().norm()*b.norm()));
}

// Sigmoid (logistic function) of a vector or matrix
PlainObject sigmoid() const {
	return 1.0 / (1.0 + (-derived()).array().exp());
}

// Sigmoid (logistic function) of a vector or matrix, in place
MatrixBase<Derived>& sigmoidInPlace() {
	derived() = 1.0 / (1.0 + (-derived()).array().exp());
	return *this;
}

// Append two vectors
PlainObject append(const MatrixBase<Derived> & other) const {
	EIGEN_STATIC_ASSERT_VECTOR_ONLY(MatrixBase<Derived>);
	int n = derived().size(), m = other.size();
	PlainObject result(n + m);
	result.block(0, 0, n, 1) = derived();
	result.block(n, 0, m, 1) = other;
	return result;
}

// Append two vectors, in place
MatrixBase<Derived> & appendInPlace(const MatrixBase<Derived> & other) {
	EIGEN_STATIC_ASSERT_VECTOR_ONLY(MatrixBase<Derived>);
	int n = derived().size(), m = other.size();
	derived().conservativeResize(n + m);
	derived().block(n, 0, m, 1) = other;
	return *this; 
}

// Append a number to the end of this vector. NOT EFFICIENT.
PlainObject append(const RealScalar & element) const {
	EIGEN_STATIC_ASSERT_VECTOR_ONLY(MatrixBase<Derived>);
	int n = derived().size();
	PlainObject result(n + 1);
	result.block(0, 0, n, 1) = derived();
	result(n) = element;
	return result;
}

// Append a number to the end of this vector, in place. NOT EFFICIENT.
MatrixBase<Derived> & appendInPlace(const RealScalar & element) {
	EIGEN_STATIC_ASSERT_VECTOR_ONLY(MatrixBase<Derived>);
	int n = derived().size();
	derived().conservativeResize(n + 1);
	derived().operator()(n) = element;
	return *this;
}

// Assignment operator for std vectors
MatrixBase<Derived>& assign(std::vector<double> & v) {
	derived().resize((int)v.size());
	for (int i = 0; i < (int)v.size(); i++)
		derived().operator()(i) = v[i];
	return *this;
}

// Naive Moore-Penrose pseudoinverse implementation
PlainObject pinv() const {
	if (rows() < cols()) {
		JacobiSVD<Derived> svd = Derived(this->transpose()).jacobiSvd(ComputeThinU | ComputeThinV);
		RealScalar tolerance = (RealScalar)std::numeric_limits<RealScalar>::epsilon() * std::max((RealScalar)cols(), (RealScalar)rows()) * svd.singularValues().array().abs().maxCoeff();
		return (svd.matrixV() * Derived((svd.singularValues().array().abs() > tolerance).select(svd.singularValues().array().inverse(), 0)).asDiagonal() * svd.matrixU().adjoint()).transpose();
	}
	JacobiSVD<Derived> svd = jacobiSvd(ComputeThinU | ComputeThinV);
	RealScalar tolerance = (RealScalar)std::numeric_limits<RealScalar>::epsilon() * std::max((RealScalar)cols(), (RealScalar)rows()) * svd.singularValues().array().abs().maxCoeff();
	return svd.matrixV() * Derived((svd.singularValues().array().abs() > tolerance).select(svd.singularValues().array().inverse(), 0)).asDiagonal() * svd.matrixU().adjoint();
}

// Solve system of linear equations in naive way
template<typename OtherDerived>
const Derived linSolve(const MatrixBase<OtherDerived> & b) const {
	// If there are fewer features for linear fit than there are points, then
	// Eigen's jacobiSvd.solve function won't work - use our pinv isntead.
	if (rows() < cols()) {
		PlainObject temp = pinv();
		return temp*b;
	}
	return jacobiSvd(ComputeThinU | ComputeThinV).solve(b);
}

// Element-wise min operator for vectors and matrices
MatrixBase<Derived>& minInPlace(const Scalar & value) {
	for (int i = 0; i < (int)rows(); i++)
		for (int j = 0; j < (int)cols(); j++)
			derived()(i,j) = std::min(derived()(i,j), value);
	return *this;
}

// Element-wise min operator for vectors and matrices
PlainObject min(const Scalar & value) const {
	PlainObject result = derived();
	for (int i = 0; i < (int)rows(); i++)
		for (int j = 0; j < (int)cols(); j++)
			result(i,j) = std::min(derived()(i,j), value);
	return result;
}

// Element-wise maximum operator for vectors and matrices
MatrixBase<Derived>& maxInPlace(const Scalar & value) {
	for (int i = 0; i < (int)rows(); i++)
		for (int j = 0; j < (int)cols(); j++)
			derived()(i,j) = std::max(derived()(i,j), value);
	return *this;
}

// Element-wise maximum operator for vectors and matrices
PlainObject max(const Scalar & value) const {
	PlainObject result = derived();
	for (int i = 0; i < (int)rows(); i++)
		for (int j = 0; j < (int)cols(); j++)
			result(i,j) = std::max(derived()(i,j), value);
	return result;
}

// Element-wise bound (max and min) operator for vectors and matrices
MatrixBase<Derived>& boundInPlace(const Scalar & minValue, 
	const Scalar & maxValue) {
	for (int i = 0; i < (int)rows(); i++)
		for (int j = 0; j < (int)cols(); j++)
			derived()(i, j) = std::min(maxValue, std::max(derived()(i, j), minValue));
	return *this;
}

// Element-wise bound (max and min) operator for vectors and matrices
PlainObject bound(const Scalar & minValue, const Scalar & maxValue) const {
	PlainObject result = derived();
	for (int i = 0; i < (int)rows(); i++) {
		for (int j = 0; j < (int)cols(); j++) {
			result(i, j) = std::min(maxValue, std::max(derived()(i,j), minValue));
		}
	}
	return result;
}