/****************************************************************************
 *            prerequisites.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_a9b0cc4b_b046_4138_b8f4_d747aac1179a
#define	UUID_a9b0cc4b_b046_4138_b8f4_d747aac1179a

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_THREAD_USE_LIB
#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_LIST_SIZE 30

#ifdef	OSX
#include <GLEW/glew.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else	/* OSX */
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif	/* OSX */
#include <string>
#include <limits>
#include <bitset>
#include <iostream>
#include <boost/array.hpp>
#include <boost/assign.hpp>
#include <boost/cast.hpp>
#include <boost/cstdint.hpp>
#include <boost/flyweight.hpp>
#include <boost/foreach.hpp>
#include <boost/utility.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/exception/all.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/ptr_container/ptr_container.hpp>
#include <boost/format.hpp>
#include <boost/variant.hpp>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/epsilon.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlreader.h>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <cal3d/cal3d.h>
#include <SDL.h>
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"

namespace eternal_lands
{

        typedef boost::uint8_t Uint8;
        typedef boost::uint16_t Uint16;
        typedef boost::uint32_t Uint32;
        typedef boost::uint64_t Uint64;
        typedef boost::int8_t Sint8;
        typedef boost::int16_t Sint16;
        typedef boost::int32_t Sint32;
        typedef boost::int64_t Sint64;

	#define SHARED_PTR_NAME(type, name) typedef boost::shared_ptr<type> name##SharedPtr
	#define ARRAY_NAME(type, N, name) typedef boost::array<type, N> name##Array##N
	#define VECTOR_NAME(type, name) typedef std::vector<type> name##Vector

	#define SHARED_PTR(type) typedef boost::shared_ptr<type> type##SharedPtr
	#define WEAK_PTR(type) typedef boost::weak_ptr<type> type##WeakPtr
	#define ARRAY(type, N) ARRAY_NAME(type, N, type)
	#define VECTOR(type) VECTOR_NAME(type, type)

	typedef wchar_t Utf32Char;
	typedef std::wstring Utf32String;
	#define UTF8(str) str

	typedef std::string StringType;
	typedef std::stringstream StringStream;
	typedef std::ostream OutStream;
	typedef std::istream InStream;
	typedef boost::format BoostFormat;
	typedef boost::flyweights::flyweight<StringType> String;
	typedef std::bitset<32> BitSet32;
	typedef std::bitset<16> BitSet16;
	typedef std::bitset<8> BitSet8;
	typedef std::bitset<4> BitSet4;
	typedef boost::scoped_array<Uint8> Uint8ScopedArray;
	typedef std::set<Uint32> Uint32Set;
	typedef std::set<String> StringSet;

	ARRAY_NAME(float, 2, Float);
	ARRAY_NAME(float, 4, Float);
	ARRAY_NAME(float, 6, Float);
	ARRAY_NAME(float, 8, Float);
	ARRAY(Uint32, 2);
	ARRAY(Uint32, 3);
	ARRAY(Uint32, 4);
	ARRAY(Uint32, 32);
	ARRAY(Uint16, 2);
	ARRAY(Uint16, 3);
	ARRAY(Uint16, 4);
	ARRAY(Uint16, 16);
	ARRAY(Uint8, 2);
	ARRAY(Uint8, 3);
	ARRAY(Uint8, 4);
	ARRAY(Uint8, 5);
	ARRAY(Uint8, 6);
	ARRAY(Uint8, 8);
	ARRAY(Uint8, 16);
	ARRAY(Uint8, 20);
	ARRAY(Uint8, 32);
	ARRAY(Sint8, 4);
	ARRAY(Sint8, 8);
	ARRAY_NAME(glm::vec2, 2, Vec2);
	ARRAY_NAME(glm::vec2, 3, Vec2);
	ARRAY_NAME(glm::vec3, 2, Vec3);
	ARRAY_NAME(glm::vec3, 3, Vec3);
	ARRAY_NAME(glm::vec3, 8, Vec3);
	ARRAY_NAME(glm::vec4, 2, Vec4);
	ARRAY_NAME(glm::vec4, 3, Vec4);
	ARRAY_NAME(glm::vec4, 4, Vec4);
	ARRAY_NAME(glm::vec4, 8, Vec4);
	ARRAY_NAME(glm::vec4, 16, Vec4);
	ARRAY(String, 2);
	ARRAY(String, 3);
	ARRAY(String, 4);
	ARRAY(String, 8);
	ARRAY(String, 16);
	ARRAY(String, 24);

	VECTOR(Uint16);
	VECTOR(Uint32);
	VECTOR(Sint32);
	VECTOR(String);
	VECTOR(StringType);
	VECTOR(Uint16Array2);
	VECTOR(Uint32Array2);

	VECTOR_NAME(glm::ivec4, Ivec4);
	VECTOR_NAME(glm::ivec3, Ivec3);
	VECTOR_NAME(glm::ivec2, Ivec2);
	VECTOR_NAME(glm::uvec4, Uvec4);
	VECTOR_NAME(glm::uvec3, Uvec3);
	VECTOR_NAME(glm::uvec2, Uvec2);
	VECTOR_NAME(glm::mat2x2, Mat2x2);
	VECTOR_NAME(glm::mat2x3, Mat2x3);
	VECTOR_NAME(glm::mat2x4, Mat2x4);
	VECTOR_NAME(glm::mat3x2, Mat3x2);
	VECTOR_NAME(glm::mat3x3, Mat3x3);
	VECTOR_NAME(glm::mat3x4, Mat3x4);
	VECTOR_NAME(glm::mat4x2, Mat4x2);
	VECTOR_NAME(glm::mat4x3, Mat4x3);
	VECTOR_NAME(glm::mat4x4, Mat4x4);
	VECTOR_NAME(glm::vec4, Vec4);
	VECTOR_NAME(glm::vec3, Vec3);
	VECTOR_NAME(glm::vec2, Vec2);
	VECTOR_NAME(float, Float);
	VECTOR_NAME(bool, Bool);

	class AbstractArchive;
	class AbstractFrameBuffer;
	class AbstractMesh;
	class AbstractReadMemory;
	class AbstractReadWriteMemory;
	class AbstractRStarTreeVisitor;
	class AbstractWriteMemory;
	class Actor;
	class ActorDataCache;
	class ActorTextureBuilder;
	class AlignedVec4Array;
	class Atlas;
	class BoundedObject;
	class BoundingBox;
	class CodecManager;
	class ConvexBody;
	class Effect;
	class EffectCache;
	class EffectDescription;
	class FileSystem;
	class Filter;
	class FrameBufferBuilder;
	class FreeIds;
	class Frustum;
	class GlobalVars;
	class GlslProgram;
	class HardwareBuffer;
	class Image;
	class IndexUpdateSource;
	class InstanceBuilder;
	class InstanceData;
	class InstancesBuilder;
	class InstancingData;
	class Light;
	class Lua;
	class Map;
	class MaterialDescription;
	class MaterialDescriptionCache;
	class MaterialEffect;
	class MaterialEffectDescription;
	class MeshBuilder;
	class MeshCache;
	class MeshDataCache;
	class MeshDataTool;
	class MeshDrawData;
	class Object;
	class ObjectData;
	class ObjectDescription;
	class Plane;
	class Polygon;
	class Ray;
	class Reader;
	class ReadWriteMemory;
	class RenderObjectData;
	class RStarTree;
	class RStarTreeNode;
	class SceneResources;
	class ShaderSourceBuilder;
	class ShaderSourceData;
	class ShaderSourceDescriptionLoader;
	class ShaderSourceParameter;
	class StateManager;
	class SubMesh;
	class SubObject;
	class Text;
	class TextAttribute;
	class Texture;
	class TextureCache;
	class TextureGlyphe;
	class TextureFont;
	class TextureFontCache;
	class VertexElement;
	class VertexElements;
	class VertexFormat;
	class VertexBuffers;
	class VertexStream;
	class VertexStreams;
	class XmlReader;
	class XmlWriter;

	ARRAY(String, 2);
	ARRAY(String, 4);
	ARRAY(String, 8);
	ARRAY(String, 16);

	const Uint16 vertex_stream_count = 16;
	const Uint16 material_texture_count = 14;
	const float epsilon = 1e-5f;

	typedef boost::array<String, material_texture_count>
		MaterialStringArray;

	typedef boost::array<VertexElements, vertex_stream_count>
		VertexElementsArray;

	typedef RStarTree* RStarTreePtr;
	typedef SubObject* SubObjectPtr;
	typedef VertexElements* VertexElementsPtr;

	typedef std::pair<Sint16, Sint16> Sint16Sint16Pair;
	typedef std::map<Uint32, Uint32> Uint32Uint32Map;
	typedef std::map<String, Uint16> StringUint16Map;
	typedef std::map<String, Uint32> StringUint32Map;
	typedef std::map<String, String> StringStringMap;

	SHARED_PTR(AbstractFrameBuffer);
	SHARED_PTR(AbstractMesh);
	SHARED_PTR(AbstractReadMemory);
	SHARED_PTR(AbstractReadWriteMemory);
	SHARED_PTR(AbstractWriteMemory);
	SHARED_PTR(Actor);
	SHARED_PTR(ActorDataCache);
	SHARED_PTR(ActorTextureBuilder);
	SHARED_PTR(Atlas);
	SHARED_PTR(BoundedObject);
	SHARED_PTR(CodecManager);
	SHARED_PTR(Effect);
	SHARED_PTR(EffectCache);
	SHARED_PTR(FileSystem);
	SHARED_PTR(Filter);
	SHARED_PTR(FrameBufferBuilder);
	SHARED_PTR(GlobalVars);
	SHARED_PTR(GlslProgram);
	SHARED_PTR(HardwareBuffer);
	SHARED_PTR(Image);
	SHARED_PTR(IndexUpdateSource);
	SHARED_PTR(Light);
	SHARED_PTR(MaterialDescriptionCache);
	SHARED_PTR(MeshBuilder);
	SHARED_PTR(MeshCache);
	SHARED_PTR(MeshDataCache);
	SHARED_PTR(MeshDataTool);
	SHARED_PTR(Object);
	SHARED_PTR(Reader);
	SHARED_PTR(ReadWriteMemory);
	SHARED_PTR(RStarTreeNode);
	SHARED_PTR(SceneResources);
	SHARED_PTR(ShaderSourceBuilder);
	SHARED_PTR(ShaderSourceDescriptionLoader);
	SHARED_PTR(Texture);
	SHARED_PTR(TextureCache);
	SHARED_PTR(VertexFormat);
	SHARED_PTR(VertexBuffers);
	SHARED_PTR(VertexStreams);
	SHARED_PTR(XmlReader);
	SHARED_PTR(XmlWriter);

	WEAK_PTR(ActorDataCache);
	WEAK_PTR(CodecManager);
	WEAK_PTR(EffectCache);
	WEAK_PTR(FileSystem);
	WEAK_PTR(MaterialDescriptionCache);
	WEAK_PTR(MeshBuilder);
	WEAK_PTR(MeshCache);
	WEAK_PTR(MeshDataCache);
	WEAK_PTR(SceneResources);
	WEAK_PTR(ShaderSourceBuilder);
	WEAK_PTR(TextureCache);

	VECTOR(AbstractReadWriteMemorySharedPtr);
	VECTOR(ActorSharedPtr);
	VECTOR(BoundedObjectSharedPtr);
	VECTOR(BoundingBox);
	VECTOR(GlslProgramSharedPtr);
	VECTOR(ImageSharedPtr);
	VECTOR(LightSharedPtr);
	VECTOR(MaterialDescription);
	VECTOR(MaterialEffect);
	VECTOR(MaterialEffectDescription);
	VECTOR(ReadWriteMemorySharedPtr);
	VECTOR(MeshDrawData);
	VECTOR(ObjectData);
	VECTOR(ObjectDescription);
	VECTOR(ObjectSharedPtr);
	VECTOR(Plane);
	VECTOR(RenderObjectData);
	VECTOR(RStarTreeNodeSharedPtr);
	VECTOR(ShaderSourceData);
	VECTOR(ShaderSourceParameter);
	VECTOR(SubMesh);
	VECTOR(SubObject);
	VECTOR(SubObjectPtr);
	VECTOR(TextureSharedPtr);
	VECTOR(VertexElement);

	ARRAY(TextureSharedPtr, 2);
	ARRAY(TextureSharedPtr, 3);
	ARRAY(TextureSharedPtr, 4);
	ARRAY(TextureSharedPtr, 8);
	ARRAY(TextureSharedPtr, 16);
	ARRAY(TextureSharedPtr, 32);

	typedef boost::array<TextureSharedPtr, material_texture_count>
		MaterialTextureSharedPtrArray;

	typedef std::map<Uint32, ActorSharedPtr> Uint32ActorSharedPtrMap;
	typedef std::map<Uint32, ObjectSharedPtr> Uint32ObjectSharedPtrMap;
	typedef std::map<Uint32, LightSharedPtr> Uint32LightSharedPtrMap;
	typedef std::map<Utf32Char, float> Utf32CharFloatMap;
	typedef std::stack<RStarTreeNodeSharedPtr> RStarTreeNodeSharedPtrStack;
	typedef boost::ptr_vector<InstanceData> InstanceDataVector;
	typedef boost::ptr_vector<InstancingData> InstancingDataVector;
	typedef boost::ptr_vector<AbstractArchive> AbstractArchiveVector;
	typedef boost::ptr_array<VertexStream, vertex_stream_count>
		VertexStreamArray;
	typedef boost::array<AbstractWriteMemorySharedPtr,
		vertex_stream_count> AbstractWriteMemorySharedPtrArray;
	typedef boost::ptr_vector<VertexStream> VertexStreamVector;

	typedef boost::variant<bool, Sint64, float,
		glm::ivec2, glm::ivec3, glm::ivec4,
		glm::uvec2, glm::uvec3, glm::uvec4,
		glm::bvec2, glm::bvec3, glm::bvec4,
		glm::vec2, glm::vec3, glm::vec4,
		glm::mat2x2, glm::mat3x3, glm::mat4x4,
		glm::mat2x3, glm::mat2x4, glm::mat3x2,
		glm::mat3x4, glm::mat4x2, glm::mat4x3,
		Mat2x4Vector, Mat3x4Vector, Mat4x4Vector,
		Ivec4Vector, Uvec4Vector, Vec4Vector> Variant;

	typedef std::map<String, Variant> StringVariantMap;
	typedef std::pair<String, Variant> StringVariantPair;

	typedef boost::array<BoundingBox, 4> SubFrustumsBoundingBoxes;
	typedef boost::array<Vec3Array8, 4> SubFrustumsConerPoints;
	typedef boost::array<ConvexBody, 4> SubFrustumsConvexBodys;
	typedef std::bitset<4> SubFrustumsMask;
	typedef std::bitset<32> PlanesMask;
	typedef std::pair<TextAttribute, Uint32> TextAttributeUint32Pair;
	typedef std::pair<Uint32, TextAttributeUint32Pair>
		Uint32TextAttributeUint32PairPair;
	typedef std::pair<Utf32Char, TextureGlyphe> Utf32CharTextureGlyphePair;
	typedef std::map<Utf32Char, TextureGlyphe> Utf32CharTextureGlypheMap;
	typedef std::map<Uint32, TextAttributeUint32Pair>
		Uint32TextAttributeUint32PairMap;

	typedef boost::shared_array<double> DoubleSharedArray;

}

#endif	/* UUID_a9b0cc4b_b046_4138_b8f4_d747aac1179a */
