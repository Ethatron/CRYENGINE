// Copyright 2001-2017 Crytek GmbH / Crytek Group. All rights reserved. 

//
//	File:VertexFormats.h -
//
//	History:
//	-Feb 23,2001:Created by Marco Corbetta
//
//////////////////////////////////////////////////////////////////////

#ifndef VERTEXFORMATS_H
#define VERTEXFORMATS_H

#if _MSC_VER > 1000
	#pragma once
#endif

#include <CryCore/Containers/CryArray.h>

//! Stream Configuration options
#define ENABLE_NORMALSTREAM_SUPPORT 1

//////////////////////////////////////////////////////////////////////
struct InputLayoutHandle
{
	typedef uint8 ValueType;
	ValueType value;

	constexpr InputLayoutHandle() : value(Unspecified) { }
	constexpr InputLayoutHandle(ValueType v) : value(v) { }

	// Test operators
	template<typename T> bool operator ==(const T other) const { return value == other; }
	template<typename T> bool operator !=(const T other) const { return value != other; }
	// Range operators
	template<typename T> bool operator <=(const T other) const { return value <= other; }
	template<typename T> bool operator >=(const T other) const { return value >= other; }
	// Sorting operators
	template<typename T> bool operator < (const T other) const { return value <  other; }
	template<typename T> bool operator > (const T other) const { return value >  other; }

	// Auto cast for array access operator []
	operator ValueType() const { return value; }

	// Not an enum, because of SWIG
	static constexpr ValueType Unspecified = ValueType(~0);
};

// InputLayout API (pre-allocated sampler states)
// TODO: Move to DeviceObjects.h
struct EDefaultInputLayouts : InputLayoutHandle
{
	enum PreDefs : ValueType
	{
		Empty                 =  0,    //!< Empty layout for pull-model shaders

		// Base stream
		P3F_C4B_T2F           =  1,
		P3F_C4B_T2H           =  2,    //!< Road elements
		P3H_C4B_T2H           =  3,
		P3H_C4B_T2H_N4C       =  4,

		P3F_C4B_T4B_N3F2      =  5,    //!< Particles.
		P3F_T3F               =  6,    //!< AuxGeometryObj
		P3F_T2F_T3F           =  7,    //!< WorldPos full-screen quad

		P3F                   =  8,    //!< Raw Geometry
		P3H                   =  9,    //!< Raw Geometry

		P2H_C4B_T1F_N4C       = 10,    //!< Terrain sector

		// Additional streams
#ifdef NORM_FLOATS
		T4F_B4F               = 11,    //!< Tangent/Bitangent
		Q4F                   = 12,    //!< QTangent
		N3F                   = 13,    //!< Normal
#else
		T4S_B4S               = 11,    //!< Tangent/Bitangent
		Q4S                   = 12,    //!< QTangent
		N3S                   = 13,    //!< Normal
#endif

		W4B_I4U               = 14,    //!< Skinned weights/indices stream
		V3F                   = 15,    //!< Velocity stream
		W2F                   = 16,    //!< Morph-buddy weights

		V4Fi                  = 17,    //!< Instanced Vec4 stream

		PreAllocated          = 18,    // from this value and up custom input layouts are assigned
		MaxRenderMesh         = PreAllocated
	};
};

//////////////////////////////////////////////////////////////////////
typedef Vec4_tpl<int16> Vec4i16;   //!< Used for tangents only.

//! bNeedNormals=1 - float normals; bNeedNormals=2 - byte normals
inline InputLayoutHandle VertFormatForComponents(bool bNeedCol, bool bHasTC, bool bHasPS, bool bHasNormal)
{
	InputLayoutHandle RequestedVertFormat;

	if (bHasPS)
		RequestedVertFormat = EDefaultInputLayouts::P3F_C4B_T4B_N3F2;
	else if (bHasNormal)
		RequestedVertFormat = EDefaultInputLayouts::P3H_C4B_T2H_N4C;
	else
		RequestedVertFormat = EDefaultInputLayouts::P3H_C4B_T2H;

	return RequestedVertFormat;
}

struct UCol
{
	union
	{
		uint32 dcolor;
		uint8  bcolor[4];

		struct { uint8 b, g, r, a; };
		struct { uint8 z, y, x, w; };
	};

	bool operator != (const UCol& other) const { return dcolor != other.dcolor; }
	bool operator == (const UCol& other) const { return dcolor == other.dcolor; }

	//! Get vector from unsigned 8bit integers.
	ILINE Vec4 GetV() const
	{
		return Vec4
		(
			bcolor[0] * (1.0f / 255.0f),
			bcolor[1] * (1.0f / 255.0f),
			bcolor[2] * (1.0f / 255.0f),
			bcolor[3] * (1.0f / 255.0f)
		);
	}

	AUTO_STRUCT_INFO;
};

struct SCol
{
	union
	{
		uint32 icolor;
		int8   ccolor[4];

		struct { uint8 b, g, r, a; };
		struct {  int8 z, y, x, w; };
	};

	bool operator != (const SCol& other) const { return icolor != other.icolor; }
	bool operator == (const SCol& other) const { return icolor == other.icolor; }

	//! Get normal vector from signed 8bit integers (can point up/down but is not normal).
	ILINE Vec3 GetN() const
	{
		return Vec3
		(
			ccolor[0] * (1.0f / 127.0f),
			ccolor[1] * (1.0f / 127.0f),
			ccolor[2] * (1.0f / 127.0f)
		);
	}

	//! Put normal vector to signed 8bit integers (can point up/down but is not normal).
	ILINE void PutN(Vec3& n, int8 w)
	{
		ccolor[0] = int8(n.x * 127.0f);
		ccolor[1] = int8(n.y * 127.0f);
		ccolor[2] = int8(n.z * 127.0f);
		ccolor[3] = w;
	}

	AUTO_STRUCT_INFO;
};

struct Vec2f16 : public CryHalf2
{
	inline Vec2f16()
	{
	}
	inline Vec2f16(f32 _x, f32 _y)
	{
		x = CryConvertFloatToHalf(_x);
		y = CryConvertFloatToHalf(_y);
	}
	Vec2f16& operator=(const Vec2& sl)
	{
		x = CryConvertFloatToHalf(sl.x);
		y = CryConvertFloatToHalf(sl.y);
		return *this;
	}
	float operator[](int i) const
	{
		assert(i <= 1);
		return CryConvertHalfToFloat(((CryHalf*)this)[i]);
	}
	inline Vec2 ToVec2() const
	{
		Vec2 v;
		v.x = CryConvertHalfToFloat(x);
		v.y = CryConvertHalfToFloat(y);
		return v;
	}
};

struct Vec3f16 : public CryHalf4
{
	inline Vec3f16()
	{
	}
	inline Vec3f16(f32 _x, f32 _y, f32 _z)
	{
		x = CryConvertFloatToHalf(_x);
		y = CryConvertFloatToHalf(_y);
		z = CryConvertFloatToHalf(_z);
		w = CryConvertFloatToHalf(1.0f);
	}
	float operator[](int i) const
	{
		assert(i <= 2);
		return CryConvertHalfToFloat(((CryHalf*)this)[i]);
	}
	inline Vec3f16& operator=(const Vec3& sl)
	{
		x = CryConvertFloatToHalf(sl.x);
		y = CryConvertFloatToHalf(sl.y);
		z = CryConvertFloatToHalf(sl.z);
		w = CryConvertFloatToHalf(1.0f);
		return *this;
	}
	inline Vec3f16& operator=(const Vec4& sl)
	{
		x = CryConvertFloatToHalf(sl.x);
		y = CryConvertFloatToHalf(sl.y);
		z = CryConvertFloatToHalf(sl.z);
		w = CryConvertFloatToHalf(1.0f);
		return *this;
	}
	inline Vec3 ToVec3() const
	{
		Vec3 v;
		v.x = CryConvertHalfToFloat(x);
		v.y = CryConvertHalfToFloat(y);
		v.z = CryConvertHalfToFloat(z);
		return v;
	}
};

struct Vec4f16 : public CryHalf4
{
	inline Vec4f16()
	{
	}
	inline Vec4f16(f32 _x, f32 _y, f32 _z, f32 _w)
	{
		x = CryConvertFloatToHalf(_x);
		y = CryConvertFloatToHalf(_y);
		z = CryConvertFloatToHalf(_z);
		w = CryConvertFloatToHalf(_w);
	}
	float operator[](int i) const
	{
		assert(i <= 3);
		return CryConvertHalfToFloat(((CryHalf*)this)[i]);
	}
	inline Vec4f16& operator=(const Vec4& sl)
	{
		x = CryConvertFloatToHalf(sl.x);
		y = CryConvertFloatToHalf(sl.y);
		z = CryConvertFloatToHalf(sl.z);
		w = CryConvertFloatToHalf(sl.w);
		return *this;
	}
	inline Vec4 ToVec4() const
	{
		Vec4 v;
		v.x = CryConvertHalfToFloat(x);
		v.y = CryConvertHalfToFloat(y);
		v.z = CryConvertHalfToFloat(z);
		v.w = CryConvertHalfToFloat(w);
		return v;
	}
};

struct SVF_P3F_C4B_T2F
{
	Vec3 xyz;
	UCol color;
	Vec2 st;
};

struct SVF_P3H_C4B_T2H
{
	Vec3f16 xyz;
	UCol    color;
	Vec2f16 st;

	AUTO_STRUCT_INFO;
};

struct SVF_P3F_C4B_T2H
{
	Vec3    xyz;
	UCol    color;
	Vec2f16 st;

	AUTO_STRUCT_INFO;
};

struct SVF_P3H_C4B_T2H_N4C
{
	Vec3f16 xyz;
	UCol    color;
	Vec2f16 st;
	SCol    normal;
};

struct SVF_P2H_C4B_T1F_N4C
{
	Vec2f16 xy;
	UCol    color;
	float   z;
	SCol    normal;
};

struct SVF_T2F
{
	Vec2 st;
};

struct SVF_W4B_I4U
{
	UCol   weights;
	uint16 indices[4];
};

struct SVF_C4B_C4B
{
	UCol coef0;
	UCol coef1;
};

struct SVF_P3F_P3F_I4B
{
	Vec3 thin;
	Vec3 fat;
	UCol index;
};

struct SVF_P3F
{
	Vec3 xyz;
};

struct SVF_P3H
{
	Vec3f16 xyz;
};

struct SVF_P3F_T3F
{
	Vec3 p;
	Vec3 st;
};

struct SVF_P3F_T2F_T3F
{
	Vec3 p;
	Vec2 st0;
	Vec3 st1;
};

struct SVF_TP3F_T2F_T3F
{
	Vec4 p;
	Vec2 st0;
	Vec3 st1;
};

struct SVF_P2F_T4F_C4F
{
	Vec2 p;
	Vec4 st;
	Vec4 color;
};

struct SVF_P3F_C4B_T4B_N3F2
{
	Vec3 xyz;
	UCol color;
	UCol st;
	Vec3 xaxis;
	Vec3 yaxis;
};

struct SVF_C4B_T2S
{
	UCol    color;
	Vec2f16 st;
};

//! Signed norm value packing [-1,+1].
namespace PackingSNorm
{
ILINE int16 tPackF2B(const float f)
{
	return (int16)(f * 32767.0f);
}

ILINE int16 tPackS2B(const int16 s)
{
	return (int16)(s * 32767);
}

ILINE float tPackB2F(const int16 i)
{
	return (float)((float)i / 32767.0f);
}

ILINE int16 tPackB2S(const int16 s)
{
	// OPT: "(s >> 15) + !(s >> 15)" works as well
	return (int16)(s / 32767);
}

#ifdef CRY_TYPE_SIMD4

ILINE Vec4i16 tPackF2Bv(const Vec4H<f32>& v)
{
	Vec4i16 vs;

	vs.x = tPackF2B(v.x);
	vs.y = tPackF2B(v.y);
	vs.z = tPackF2B(v.z);
	vs.w = tPackF2B(v.w);

	return vs;
}

#endif

ILINE Vec4i16 tPackF2Bv(const Vec4f& v)
{
	Vec4i16 vs;

	vs.x = tPackF2B(v.x);
	vs.y = tPackF2B(v.y);
	vs.z = tPackF2B(v.z);
	vs.w = tPackF2B(v.w);

	return vs;
}

ILINE Vec4i16 tPackF2Bv(const Vec3& v)
{
	Vec4i16 vs;

	vs.x = tPackF2B(v.x);
	vs.y = tPackF2B(v.y);
	vs.z = tPackF2B(v.z);
	vs.w = tPackF2B(1.0f);

	return vs;
}

ILINE Vec4 tPackB2F(const Vec4i16& v)
{
	Vec4 vs;

	vs.x = tPackB2F(v.x);
	vs.y = tPackB2F(v.y);
	vs.z = tPackB2F(v.z);
	vs.w = tPackB2F(v.w);

	return vs;
}

ILINE void tPackB2F(const Vec4i16& v, Vec4& vDst)
{
	vDst.x = tPackB2F(v.x);
	vDst.y = tPackB2F(v.y);
	vDst.z = tPackB2F(v.z);
	vDst.w = 1.0f;
}

ILINE void tPackB2FScale(const Vec4i16& v, Vec4& vDst, const Vec3& vScale)
{
	vDst.x = (float)v.x * vScale.x;
	vDst.y = (float)v.y * vScale.y;
	vDst.z = (float)v.z * vScale.z;
	vDst.w = 1.0f;
}

ILINE void tPackB2FScale(const Vec4i16& v, Vec3& vDst, const Vec3& vScale)
{
	vDst.x = (float)v.x * vScale.x;
	vDst.y = (float)v.y * vScale.y;
	vDst.z = (float)v.z * vScale.z;
}

ILINE void tPackB2F(const Vec4i16& v, Vec3& vDst)
{
	vDst.x = tPackB2F(v.x);
	vDst.y = tPackB2F(v.y);
	vDst.z = tPackB2F(v.z);
}
};

//! Pip => Graphics Pipeline structures, used for inputs for the GPU's Input Assembler.
//! These structures are optimized for fast decoding (ALU and bandwidth) and might be slow to encode on-the-fly
struct SPipTangents
{
	SPipTangents() {}

private:
	Vec4i16 Tangent;
	Vec4i16 Bitangent;

public:
	explicit SPipTangents(const Vec4i16& othert, const Vec4i16& otherb, const int16& othersign)
	{
		using namespace PackingSNorm;
		Tangent = othert;
		Tangent.w = PackingSNorm::tPackS2B(othersign);
		Bitangent = otherb;
		Bitangent.w = PackingSNorm::tPackS2B(othersign);
	}

	explicit SPipTangents(const Vec4i16& othert, const Vec4i16& otherb, const SPipTangents& othersign)
	{
		Tangent = othert;
		Tangent.w = othersign.Tangent.w;
		Bitangent = otherb;
		Bitangent.w = othersign.Bitangent.w;
	}

	explicit SPipTangents(const Vec4i16& othert, const Vec4i16& otherb)
	{
		Tangent = othert;
		Bitangent = otherb;
	}

	explicit SPipTangents(const Vec3& othert, const Vec3& otherb, const int16& othersign)
	{
		Tangent = Vec4i16(PackingSNorm::tPackF2B(othert.x), PackingSNorm::tPackF2B(othert.y), PackingSNorm::tPackF2B(othert.z), PackingSNorm::tPackS2B(othersign));
		Bitangent = Vec4i16(PackingSNorm::tPackF2B(otherb.x), PackingSNorm::tPackF2B(otherb.y), PackingSNorm::tPackF2B(otherb.z), PackingSNorm::tPackS2B(othersign));
	}

	explicit SPipTangents(const Vec3& othert, const Vec3& otherb, const SPipTangents& othersign)
	{
		Tangent = Vec4i16(PackingSNorm::tPackF2B(othert.x), PackingSNorm::tPackF2B(othert.y), PackingSNorm::tPackF2B(othert.z), othersign.Tangent.w);
		Bitangent = Vec4i16(PackingSNorm::tPackF2B(otherb.x), PackingSNorm::tPackF2B(otherb.y), PackingSNorm::tPackF2B(otherb.z), othersign.Bitangent.w);
	}

	explicit SPipTangents(const Quat& other, const int16& othersign)
	{
		Vec3 othert = other.GetColumn0();
		Vec3 otherb = other.GetColumn1();

		Tangent = Vec4i16(PackingSNorm::tPackF2B(othert.x), PackingSNorm::tPackF2B(othert.y), PackingSNorm::tPackF2B(othert.z), PackingSNorm::tPackS2B(othersign));
		Bitangent = Vec4i16(PackingSNorm::tPackF2B(otherb.x), PackingSNorm::tPackF2B(otherb.y), PackingSNorm::tPackF2B(otherb.z), PackingSNorm::tPackS2B(othersign));
	}

	void ExportTo(Vec4i16& othert, Vec4i16& otherb) const
	{
		othert = Tangent;
		otherb = Bitangent;
	}

	//! Get normal tangent and bitangent vectors.
	void GetTB(Vec4& othert, Vec4& otherb) const
	{
		othert = PackingSNorm::tPackB2F(Tangent);
		otherb = PackingSNorm::tPackB2F(Bitangent);
	}

	//! Get normal vector (perpendicular to tangent and bitangent plane).
	ILINE Vec3 GetN() const
	{
		Vec4 tng, btg;
		GetTB(tng, btg);

		Vec3 tng3(tng.x, tng.y, tng.z),
		btg3(btg.x, btg.y, btg.z);

		// assumes w 1 or -1
		return tng3.Cross(btg3) * tng.w;
	}

	//! Get normal vector (perpendicular to tangent and bitangent plane).
	void GetN(Vec3& othern) const
	{
		othern = GetN();
	}

	//! Get the tangent-space basis as individual normal vectors (tangent, bitangent and normal).
	void GetTBN(Vec3& othert, Vec3& otherb, Vec3& othern) const
	{
		Vec4 tng, btg;
		GetTB(tng, btg);

		Vec3 tng3(tng.x, tng.y, tng.z),
		btg3(btg.x, btg.y, btg.z);

		// assumes w 1 or -1
		othert = tng3;
		otherb = btg3;
		othern = tng3.Cross(btg3) * tng.w;
	}

	//! Get normal vector sign (reflection).
	ILINE int16 GetR() const
	{
		return PackingSNorm::tPackB2S(Tangent.w);
	}

	//! Get normal vector sign (reflection).
	void GetR(int16& sign) const
	{
		sign = GetR();
	}

	void TransformBy(const Matrix34& trn)
	{
		Vec4 tng, btg;
		GetTB(tng, btg);

		Vec3 tng3(tng.x, tng.y, tng.z),
		btg3(btg.x, btg.y, btg.z);

		tng3 = trn.TransformVector(tng3);
		btg3 = trn.TransformVector(btg3);

		*this = SPipTangents(tng3, btg3, PackingSNorm::tPackB2S(Tangent.w));
	}

	void TransformSafelyBy(const Matrix34& trn)
	{
		Vec4 tng, btg;
		GetTB(tng, btg);

		Vec3 tng3(tng.x, tng.y, tng.z),
		btg3(btg.x, btg.y, btg.z);

		tng3 = trn.TransformVector(tng3);
		btg3 = trn.TransformVector(btg3);

		// normalize in case "trn" wasn't length-preserving
		tng3.Normalize();
		btg3.Normalize();

		*this = SPipTangents(tng3, btg3, PackingSNorm::tPackB2S(Tangent.w));
	}

	friend struct SMeshTangents;

	AUTO_STRUCT_INFO;
};

struct SPipQTangents
{
	SPipQTangents() {}

private:
	Vec4i16 QTangent;

public:
	explicit SPipQTangents(const Vec4i16& other)
	{
		QTangent = other;
	}

	bool operator==(const SPipQTangents& other) const
	{
		return
		  QTangent[0] == other.QTangent[0] ||
		  QTangent[1] == other.QTangent[1] ||
		  QTangent[2] == other.QTangent[2] ||
		  QTangent[3] == other.QTangent[3];
	}

	bool operator!=(const SPipQTangents& other) const
	{
		return !(*this == other);
	}

	//! Get quaternion.
	ILINE Quat GetQ() const
	{
		Quat q;

		q.v.x = PackingSNorm::tPackB2F(QTangent.x);
		q.v.y = PackingSNorm::tPackB2F(QTangent.y);
		q.v.z = PackingSNorm::tPackB2F(QTangent.z);
		q.w = PackingSNorm::tPackB2F(QTangent.w);

		return q;
	}

	//! Get normal vector from quaternion.
	ILINE Vec3 GetN() const
	{
		const Quat q = GetQ();

		return q.GetColumn2() * (q.w < 0.0f ? -1.0f : +1.0f);
	}

	//! Get the tangent-space basis as individual normal vectors (tangent, bitangent and normal).
	void GetTBN(Vec3& othert, Vec3& otherb, Vec3& othern) const
	{
		const Quat q = GetQ();

		othert = q.GetColumn0();
		otherb = q.GetColumn1();
		othern = q.GetColumn2() * (q.w < 0.0f ? -1.0f : +1.0f);
	}

	friend struct SMeshQTangents;
};

struct SPipNormal
{
	SPipNormal() {}

private:
	Vec4i16 Normal;

public:
	explicit SPipNormal(const Vec4i16& other)
	{
		Normal = other;
	}

	explicit SPipNormal(const Vec3& other)
	{
		Normal = Vec4i16(PackingSNorm::tPackF2B(other.x), PackingSNorm::tPackF2B(other.y), PackingSNorm::tPackF2B(other.z), 0);
	}

	bool operator==(const SPipNormal& other) const
	{
		return
		  Normal[0] == other.Normal[0] ||
		  Normal[1] == other.Normal[1] ||
		  Normal[2] == other.Normal[2];
	}

	bool operator!=(const SPipNormal& other) const
	{
		return !(*this == other);
	}

	void ExportTo(Vec4i16& other) const
	{
		other = Normal;
	}

	//! Get normal vector.
	ILINE Vec3 GetN() const
	{
		Vec3 n;

		n.x = PackingSNorm::tPackB2F(Normal.x);
		n.y = PackingSNorm::tPackB2F(Normal.y);
		n.z = PackingSNorm::tPackB2F(Normal.z);

		return n;
	}

	//! Get normal vector.
	void GetN(Vec3& othern) const
	{
		othern = GetN();
	}

	void TransformBy(const Matrix34& trn)
	{
		Vec3 nrm;
		GetN(nrm);

		nrm = trn.TransformVector(nrm);

		*this = SPipNormal(nrm);
	}

	void TransformSafelyBy(const Matrix34& trn)
	{
		Vec3 nrm;
		GetN(nrm);

		nrm = trn.TransformVector(nrm);

		// normalize in case "trn" wasn't length-preserving
		nrm.Normalize();

		*this = SPipNormal(nrm);
	}

	friend struct SMeshNormal;
};

//==================================================================================================

typedef SVF_P3F_C4B_T2F SAuxVertex;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Vertex Sizes
//extern const int m_VertexSize[];

// we don't care about truncation of the struct member offset, because
// it's a very small integer (even fits into a signed byte)
#pragma warning(push)
#pragma warning(disable:4311)

//============================================================================
// Custom vertex streams definitions
// NOTE: If you add new stream ID also include vertex declarations creating in
//       CD3D9Renderer::EF_InitD3DVertexDeclarations (D3DRendPipeline.cpp)

//! Stream IDs.
enum EStreamIDs
{
	VSF_GENERAL,                 //!< General vertex buffer.
	VSF_TANGENTS,                //!< Tangents buffer.
	VSF_QTANGENTS,               //!< Tangents buffer.
	VSF_HWSKIN_INFO,             //!< HW skinning buffer.
	VSF_VERTEX_VELOCITY,         //!< Velocity buffer.
#if ENABLE_NORMALSTREAM_SUPPORT
	VSF_NORMALS,                 //!< Normals, used for skinning.
#endif
	//   <- Insert new stream IDs here.
	VSF_NUM,                     //!< Number of vertex streams.

	VSF_MORPHBUDDY         = 8,  //!< Morphing (from m_pMorphBuddy).
	VSF_INSTANCED          = 9,  //!< Data is for instance stream.
	VSF_MORPHBUDDY_WEIGHTS = 15, //!< Morphing weights.
};

//! Stream Masks (Used during updating).
enum EStreamMasks
{
	VSM_GENERAL            = BIT(VSF_GENERAL),
	VSM_TANGENTS           = BIT(VSF_TANGENTS) | BIT( VSF_QTANGENTS),
	VSM_HWSKIN             = BIT(VSF_HWSKIN_INFO),
	VSM_VERTEX_VELOCITY    = BIT(VSF_VERTEX_VELOCITY),
#if ENABLE_NORMALSTREAM_SUPPORT
	VSM_NORMALS            = BIT(VSF_NORMALS),
#endif

	VSM_MORPHBUDDY         = BIT(VSF_MORPHBUDDY),
	VSM_INSTANCED          = BIT(VSF_INSTANCED),
	VSM_MORPHBUDDY_WEIGHTS = BIT(VSF_MORPHBUDDY_WEIGHTS),

	VSM_MASK               = MASK(VSF_NUM),
};

//==================================================================================================================

#pragma warning(pop)

#endif
