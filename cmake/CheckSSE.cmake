# check for SSE extensions
include(CheckCXXSourceCompiles)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	set(SSE_FLAGS "-msse")
	set(SSE2_FLAGS "-msse2")
endif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)

set(CMAKE_REQUIRED_FLAGS ${SSE2_FLAGS})
check_cxx_source_compiles("
	#include <emmintrin.h>

	int main()
	{
		__m128d a, b;
		double vals[2] = {0};
		a = _mm_loadu_pd(vals);
		b = _mm_add_pd(a,a);
		_mm_storeu_pd(vals,b);
		return 0;
	}"
	HAS_SSE2_EXTENSIONS)

set(CMAKE_REQUIRED_FLAGS ${SSE_FLAGS})
check_cxx_source_compiles("
	#include <emmintrin.h>

	int main()
	{
		__m128 a, b;
		float vals[4] = {0};
		a = _mm_loadu_ps(vals);
		b = _mm_add_ps(a,a);
		_mm_storeu_ps(vals,b);
		return 0;
	}"
	HAS_SSE_EXTENSIONS)

set(CMAKE_REQUIRED_FLAGS)

if (HAS_SSE2_EXTENSIONS)
	message(STATUS "Using SSE2 extensions")
elseif (HAS_SSE_EXTENSIONS)
	message(STATUS "Using SSE extensions")
endif()

