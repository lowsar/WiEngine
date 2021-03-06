/* Copyright (c) 2007 Scott Lembcke
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef CHIPMUNK_HEADER
#define CHIPMUNK_HEADER

// export definition
#if WINDOWS
	#if LIBCHIPMUNK_EXPORTS
		#define CHIPMUNK_API __declspec(dllexport)
	#else
		#define CHIPMUNK_API __declspec(dllimport)
	#endif
#else
	#define CHIPMUNK_API
#endif

#if ANDROID
	#include <jni.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if ANDROID
typedef struct wyHandlerWrap {
	jobject handler;
	jobject data;
} wyHandlerWrap;
#endif

#ifndef CP_ALLOW_PRIVATE_ACCESS
	#define CP_ALLOW_PRIVATE_ACCESS 1
#endif

#if CP_ALLOW_PRIVATE_ACCESS == 1
	#define CP_PRIVATE(symbol) symbol
#else
	#define CP_PRIVATE(symbol) symbol##_private
#endif

CHIPMUNK_API void cpMessage(const char *message, const char *condition, const char *file, int line, int isError);
#ifdef NDEBUG
	#define	cpAssertWarn(condition, message)
#else
	#define cpAssertWarn(condition, message) if(!(condition)) cpMessage(message, #condition, __FILE__, __LINE__, 0)
#endif

#ifdef NDEBUG
	#define	cpAssert(condition, message)
#else
	#define cpAssert(condition, message) if(!(condition)) cpMessage(message, #condition, __FILE__, __LINE__, 1)
#endif

#include "chipmunk_types.h"
	
#ifndef INFINITY
	#ifdef _MSC_VER
		union MSVC_EVIL_FLOAT_HACK
		{
			unsigned __int8 Bytes[4];
			float Value;
		};
		static union MSVC_EVIL_FLOAT_HACK INFINITY_HACK = {{0x00, 0x00, 0x80, 0x7F}};
		#define INFINITY (INFINITY_HACK.Value)
	#endif
	
	#ifdef __GNUC__
		#include <math.h>
	#endif
	
	#ifndef INFINITY
		#define INFINITY (1e1000)
	#endif
#endif

// Maximum allocated size for various Chipmunk buffer sizes
#define CP_BUFFER_BYTES (32*1024)

#define cpmalloc malloc
#define cpcalloc calloc
#define cprealloc realloc
#define cpfree free

#include "cpVect.h"
#include "cpBB.h"
#include "cpArray.h"
#include "cpHashSet.h"
#include "cpSpaceHash.h"

#include "cpBody.h"
#include "cpShape.h"
#include "cpPolyShape.h"

#include "cpArbiter.h"
#include "cpCollision.h"
	
#include "constraints/cpConstraint.h"

#include "cpSpace.h"

#include <math.h>

#define CP_HASH_COEF (3344921057ul)
#define CP_HASH_PAIR(A, B) ((cpHashValue)(A)*CP_HASH_COEF ^ (cpHashValue)(B)*CP_HASH_COEF)

extern const char *cpVersionString;
CHIPMUNK_API void cpInitChipmunk(void);

/**
	Calculate the moment of inertia for a circle.
	r1 and r2 are the inner and outer diameters. A solid circle has an inner diameter of 0.
*/
CHIPMUNK_API cpFloat cpMomentForCircle(cpFloat m, cpFloat r1, cpFloat r2, cpVect offset);

/**
	Calculate area of a hollow circle.
*/
CHIPMUNK_API cpFloat cpAreaForCircle(cpFloat r1, cpFloat r2);

/**
	Calculate the moment of inertia for a line segment.
	Beveling radius is not supported.
*/
CHIPMUNK_API cpFloat cpMomentForSegment(cpFloat m, cpVect a, cpVect b);

/**
	Calculate the area of a fattened (capsule shaped) line segment.
*/
CHIPMUNK_API cpFloat cpAreaForSegment(cpVect a, cpVect b, cpFloat r);

/**
	Calculate the moment of inertia for a solid polygon shape assuming it's center of gravity is at it's centroid. The offset is added to each vertex.
*/
CHIPMUNK_API cpFloat cpMomentForPoly(cpFloat m, int numVerts, const cpVect *verts, cpVect offset);

/**
	Calculate the signed area of a polygon.
*/
CHIPMUNK_API cpFloat cpAreaForPoly(const int numVerts, const cpVect *verts);

/**
	Calculate the natural centroid of a polygon.
*/
CHIPMUNK_API cpVect cpCentroidForPoly(const int numVerts, const cpVect *verts);

/**
	Center the polygon on the origin. (Subtracts the centroid of the polygon from each vertex)
*/
CHIPMUNK_API void cpRecenterPoly(const int numVerts, cpVect *verts);

/**
	Calculate the moment of inertia for a solid box.
*/
CHIPMUNK_API cpFloat cpMomentForBox(cpFloat m, cpFloat width, cpFloat height);

#ifdef __cplusplus
}

static inline cpVect operator *(const cpVect v, const cpFloat s){return cpvmult(v, s);}
static inline cpVect operator +(const cpVect v1, const cpVect v2){return cpvadd(v1, v2);}
static inline cpVect operator -(const cpVect v1, const cpVect v2){return cpvsub(v1, v2);}
static inline cpBool operator ==(const cpVect v1, const cpVect v2){return cpveql(v1, v2);}
static inline cpVect operator -(const cpVect v){return cpvneg(v);}

#endif

#endif
