/****************************************************************************
 *            prerequisites.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
#include <boost/multi_array.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/ptr_container/ptr_container.hpp>
#include <boost/format.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
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
#include "angelscript/include/angelscript.h"

#if (__cplusplus < 201103L)
#define override
#define noexcept throw()
#define nullptr 0
#define UTF8(str) str
#else
#define UTF8(str) u8##str
#endif

#if (GLM_VERSION > 93)
#define	notEqualEpsilon	epsilonNotEqual
#define	equalEpsilon	epsilonEqual
#endif

namespace eternal_lands
{

	#define SHARED_PTR_NAME(type, name) typedef boost::shared_ptr<type> name##SharedPtr
	#define ARRAY_NAME(type, N, name) typedef boost::array<type, N> name##Array##N
	#define VECTOR_NAME(type, name) typedef std::vector<type> name##Vector

	#define SHARED_PTR(type) typedef boost::shared_ptr<type> type##SharedPtr
	#define WEAK_PTR(type) typedef boost::weak_ptr<type> type##WeakPtr
	#define CONST_SHARED_PTR(type) typedef boost::shared_ptr<const type> type##ConstSharedPtr
	#define CONST_WEAK_PTR(type) typedef boost::weak_ptr<const type> type##ConstWeakPtr
	#define SMART_PTR(type)	\
		SHARED_PTR(type);	\
		WEAK_PTR(type);	\
		CONST_SHARED_PTR(type);	\
		CONST_WEAK_PTR(type)
	#define ARRAY(type, N) ARRAY_NAME(type, N, type)
	#define VECTOR(type) VECTOR_NAME(type, type)

#if (__cplusplus < 201103L)
	typedef Uint32 Utf32Char;
#else
	typedef char32_t Utf32Char;
#endif

	typedef std::stringstream StringStream;
	typedef std::ostream OutStream;
	typedef std::istream InStream;
	typedef boost::format BoostFormat;
	typedef boost::flyweights::flyweight<std::string> String;
	typedef std::bitset<64> BitSet64;
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
	ARRAY(Uint16, 8);
	ARRAY(Uint16, 16);
	ARRAY(Sint16, 8);
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
	ARRAY(Sint8, 16);
	ARRAY_NAME(glm::vec2, 2, Vec2);
	ARRAY_NAME(glm::vec2, 3, Vec2);
	ARRAY_NAME(glm::vec2, 4, Vec2);
	ARRAY_NAME(glm::vec2, 8, Vec2);
	ARRAY_NAME(glm::vec3, 2, Vec3);
	ARRAY_NAME(glm::vec3, 3, Vec3);
	ARRAY_NAME(glm::vec3, 8, Vec3);
	ARRAY_NAME(glm::vec4, 2, Vec4);
	ARRAY_NAME(glm::vec4, 3, Vec4);
	ARRAY_NAME(glm::vec4, 4, Vec4);
	ARRAY_NAME(glm::vec4, 8, Vec4);
	ARRAY_NAME(glm::vec4, 16, Vec4);
	ARRAY_NAME(glm::mat2x4, 2, Mat2x4);
	ARRAY_NAME(glm::mat2x4, 4, Mat2x4);
	ARRAY_NAME(glm::mat2x3, 2, Mat2x3);
	ARRAY_NAME(glm::mat2x3, 4, Mat2x3);
	ARRAY_NAME(glm::mat2x3, 8, Mat2x3);
	ARRAY_NAME(glm::uvec2, 4, Uvec2);
	ARRAY_NAME(glm::uvec4, 4, Uvec4);
	ARRAY(String, 2);
	ARRAY(String, 3);
	ARRAY(String, 4);
	ARRAY(String, 8);
	ARRAY(String, 16);
	ARRAY(String, 24);

	VECTOR(Uint16);
	VECTOR(Uint32);
	VECTOR(Sint16);
	VECTOR(Sint32);
	VECTOR(String);

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
	class AbstractBoundedObjectVisitor;
	class AbstractFrameBuffer;
	class AbstractLogger;
	class AbstractMesh;
	class AbstractNodeVisitor;
	class AbstractProgress;
	class AbstractReadMemory;
	class AbstractReadWriteMemory;
	class AbstractRenderTarget;
	class AbstractShaderSource;
	class AbstractTerrain;
	class AbstractThreadTask;
	class AbstractWriteMemory;
	class Actor;
	class ActorDataCache;
	class ActorTextureBuilder;
	class AngelScript;
	class Atlas;
	class BoundedObject;
	class BoundingBox;
	class CdLodQuadTree;
	class ClipmapTerrain;
	class ColorCorrection;
	class ConvexBody;
	class CpuRasterizer;
	class Decal;
	class DecalDescription;
	class Effect;
	class EffectCache;
	class EffectDescription;
	class EffectNode;
	class EffectNodePort;
	class EffectNodes;
	class FreeIds;
	class FreeIdsManager;
	class FileSystem;
	class Filter;
	class FrameBufferBuilder;
	class FreeIds;
	class Frustum;
	class GlobalVars;
	class GlslProgram;
	class GlslProgramCache;
	class HardwareBuffer;
	class HardwareBufferMapper;
	class Image;
	class ImageUpdate;
	class IndexUpdateSource;
	class InstanceBuilder;
	class InstanceData;
	class InstancesBuilder;
	class InstancingData;
	class Light;
	class LightData;
	class LightVisitor;
	class Map;
	class MappedUniformBuffer;
	class Material;
	class MaterialBuilder;
	class MaterialCache;
	class MaterialData;
	class MaterialDescription;
	class MaterialDescriptionCache;
	class MaterialScript;
	class MaterialScriptCache;
	class MaterialScriptManager;
	class MeshBuilder;
	class MeshCache;
	class MeshDataCache;
	class MeshDataTool;
	class MeshDrawData;
	class Object;
	class ObjectData;
	class ObjectDescription;
	class ObjectVisitor;
	class ParticleData;
	class Plane;
	class Polygon;
	class Ray;
	class Reader;
	class ReadWriteMemory;
	class RenderLightData;
	class RenderObjectData;
	class RStarTree;
	class RStarTreeNode;
	class Scene;
	class SceneResources;
	class SceneView;
	class ScriptEngine;
	class ShaderSource;
	class ShaderSourceBuilder;
	class ShaderSourceData;
	class ShaderSourceDescriptionLoader;
	class ShaderSourceParameter;
	class ShaderSourceTerrain;
	class ShadowObject;
	class StateManager;
	class SubMesh;
	class SubObject;
	class TerrainBuilder;
	class TerrainData;
	class TerrainMaterialData;
	class TerrainRenderingData;
	class TerrainVisitor;
	class Text;
	class TextAttribute;
	class Texture;
	class TextureCache;
	class TextureGlyphe;
	class TextureFont;
	class TextureFontCache;
	class TileBuilder;
	class ThreadPool;
	class Transformation;
	class UniformBuffer;
	class UniformBufferDescription;
	class UniformBufferDescriptionCache;
	class UniformBufferUsage;
	class UniformDescription;
	class UvTool;
	class VertexElement;
	class VertexElements;
	class VertexFormat;
	class VertexBuffers;
	class VertexStream;
	class VertexStreams;
	class WalkHeightBuilder;
	class WaterData;
	class Writer;
	class XmlBuffer;
	class XmlReader;
	class XmlWriter;

	ARRAY(String, 2);
	ARRAY(String, 4);
	ARRAY(String, 8);
	ARRAY(String, 16);

	const Uint16 vertex_stream_count = 16;
	const Uint16 material_texture_count = 16;
	const float epsilon = 1e-5f;
	const Uint64 all_bits_set = 0xFFFFFFFFFFFFFFFF;

	typedef boost::array<String, material_texture_count>
		MaterialStringArray;

	typedef boost::array<VertexElements, vertex_stream_count>
		VertexElementsArray;

	typedef std::bitset<vertex_stream_count> VertexStreamBitset;

	typedef RStarTree* RStarTreePtr;
	typedef SubObject* SubObjectPtr;
	typedef VertexElements* VertexElementsPtr;
	typedef EffectNode* EffectNodePtr;
	typedef EffectNodePort* EffectNodePortPtr;

	typedef std::pair<Uint16, Uint16> Uint16Uint16Pair;
	typedef std::pair<Sint16, Sint16> Sint16Sint16Pair;
	typedef std::map<Uint32, Uint32> Uint32Uint32Map;
	typedef std::map<String, Uint16> StringUint16Map;
	typedef std::map<String, Uint32> StringUint32Map;
	typedef std::map<String, String> StringStringMap;
	typedef std::map<String, BitSet16> StringBitSet16Map;
	typedef std::map<Uint16, String> Uint16StringMap;
	typedef std::map<boost::uuids::uuid, EffectNodePortPtr>
		UuidEffectNodePortPtrMap;
	typedef boost::multi_array<bool, 2> BoolMultiArray2;
	typedef boost::multi_array<float, 2> FloatMultiArray2;
	typedef boost::multi_array<Uint8, 2> Uint8MultiArray2;
	typedef boost::multi_array<Uint16, 2> Uint16MultiArray2;
	typedef boost::multi_array<Vec3Array2, 2> Vec3Array2MultiArray2;

	typedef boost::uuids::basic_random_generator<boost::mt19937>
		Mt19937RandomUuidGenerator;

	SMART_PTR(AbstractFrameBuffer);
	SMART_PTR(AbstractMesh);
	SMART_PTR(AbstractProgress);
	SMART_PTR(AbstractReadMemory);
	SMART_PTR(AbstractReadWriteMemory);
	SMART_PTR(AbstractShaderSource);
	SMART_PTR(AbstractRenderTarget);
	SMART_PTR(AbstractTerrain);
	SMART_PTR(AbstractWriteMemory);
	SMART_PTR(Actor);
	SMART_PTR(ActorDataCache);
	SMART_PTR(ActorTextureBuilder);
	SMART_PTR(Atlas);
	SMART_PTR(BoundedObject);
	SMART_PTR(ColorCorrection);
	SMART_PTR(CpuRasterizer);
	SMART_PTR(Effect);
	SMART_PTR(EffectCache);
	SMART_PTR(EffectNodes);
	SMART_PTR(FreeIds);
	SMART_PTR(FreeIdsManager);
	SMART_PTR(FileSystem);
	SMART_PTR(Filter);
	SMART_PTR(FrameBufferBuilder);
	SMART_PTR(GlobalVars);
	SMART_PTR(GlslProgram);
	SMART_PTR(GlslProgramCache);
	SMART_PTR(HardwareBuffer);
	SMART_PTR(HardwareBufferMapper);
	SMART_PTR(Image);
	SMART_PTR(IndexUpdateSource);
	SMART_PTR(Light);
	SMART_PTR(Map);
	SMART_PTR(MappedUniformBuffer);
	SMART_PTR(Material);
	SMART_PTR(MaterialBuilder);
	SMART_PTR(MaterialCache);
	SMART_PTR(MaterialDescriptionCache);
	SMART_PTR(MaterialScript);
	SMART_PTR(MaterialScriptCache);
	SMART_PTR(MaterialScriptManager);
	SMART_PTR(MeshBuilder);
	SMART_PTR(MeshCache);
	SMART_PTR(MeshDataCache);
	SMART_PTR(MeshDataTool);
	SMART_PTR(Object);
	SMART_PTR(OutStream);
	SMART_PTR(Reader);
	SMART_PTR(ReadWriteMemory);
	SMART_PTR(RStarTreeNode);
	SMART_PTR(SceneResources);
	SMART_PTR(ScriptEngine);
	SMART_PTR(ShaderSource);
	SMART_PTR(ShaderSourceBuilder);
	SMART_PTR(ShaderSourceDescriptionLoader);
	SMART_PTR(ShaderSourceTerrain);
	SMART_PTR(TerrainBuilder);
	SMART_PTR(Texture);
	SMART_PTR(TextureCache);
	SMART_PTR(TileBuilder);
	SMART_PTR(ThreadPool);
	SMART_PTR(UniformBuffer);
	SMART_PTR(UniformBufferDescription);
	SMART_PTR(UniformBufferDescriptionCache);
	SMART_PTR(VertexFormat);
	SMART_PTR(VertexBuffers);
	SMART_PTR(VertexStream);
	SMART_PTR(VertexStreams);
	SMART_PTR(WalkHeightBuilder);
	SMART_PTR(Writer);
	SMART_PTR(XmlBuffer);
	SMART_PTR(XmlReader);
	SMART_PTR(XmlWriter);

	VECTOR(AbstractReadWriteMemorySharedPtr);
	VECTOR(ActorSharedPtr);
	VECTOR(ActorConstSharedPtr);
	VECTOR(BoundedObjectSharedPtr);
	VECTOR(BoundedObjectConstSharedPtr);
	VECTOR(BoundingBox);
	VECTOR(ConvexBody);
	VECTOR(EffectNodePtr);
	VECTOR(EffectNodePortPtr);
	VECTOR(GlslProgramSharedPtr);
	VECTOR(GlslProgramConstSharedPtr);
	VECTOR(ImageSharedPtr);
	VECTOR(ImageConstSharedPtr);
	VECTOR(ImageUpdate);
	VECTOR(LightData);
	VECTOR(LightSharedPtr);
	VECTOR(LightConstSharedPtr);
	VECTOR(MaterialSharedPtr);
	VECTOR(MaterialConstSharedPtr);
	VECTOR(MaterialDescription);
	VECTOR(ReadWriteMemorySharedPtr);
	VECTOR(MeshDrawData);
	VECTOR(ObjectData);
	VECTOR(ObjectDescription);
	VECTOR(ObjectSharedPtr);
	VECTOR(ObjectConstSharedPtr);
	VECTOR(ParticleData);
	VECTOR(Plane);
	VECTOR(RenderLightData);
	VECTOR(RenderObjectData);
	VECTOR(RStarTreeNodeSharedPtr);
	VECTOR(RStarTreeNodeConstSharedPtr);
	VECTOR(ShaderSourceData);
	VECTOR(ShaderSourceParameter);
	VECTOR(ShadowObject);
	VECTOR(SubMesh);
	VECTOR(SubObject);
	VECTOR(SubObjectPtr);
	VECTOR(TextureSharedPtr);
	VECTOR(TextureConstSharedPtr);
	VECTOR(UniformDescription);
	VECTOR(UniformBufferDescriptionSharedPtr);
	VECTOR(UniformBufferDescriptionConstSharedPtr);
	VECTOR(Vec3Array8);
	VECTOR(VertexElement);
	VECTOR(VertexElements);
	VECTOR(WaterData);

	ARRAY(TextureSharedPtr, 2);
	ARRAY(TextureSharedPtr, 3);
	ARRAY(TextureSharedPtr, 4);
	ARRAY(TextureSharedPtr, 8);
	ARRAY(TextureSharedPtr, 16);
	ARRAY(TextureSharedPtr, 32);

	ARRAY(ImageSharedPtr, 2);
	ARRAY(ImageSharedPtr, 3);
	ARRAY(ImageSharedPtr, 4);
	ARRAY(ImageSharedPtr, 8);

	ARRAY(MaterialSharedPtr, 2);
	ARRAY(MaterialSharedPtr, 3);
	ARRAY(MaterialSharedPtr, 4);
	ARRAY(MaterialSharedPtr, 8);

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
	typedef boost::ptr_vector<AbstractLogger> AbstractLoggerVector;
	typedef boost::ptr_array<VertexStream, vertex_stream_count>
		VertexStreamArray;
	typedef boost::array<AbstractWriteMemorySharedPtr,
		vertex_stream_count> AbstractWriteMemorySharedPtrArray;
	typedef boost::ptr_vector<VertexStream> VertexStreamVector;
	typedef boost::ptr_vector<EffectNode> EffectNodeVector;
	typedef boost::ptr_vector<EffectNodePort> EffectNodePortVector;
	typedef boost::ptr_vector<AbstractThreadTask> AbstractThreadTaskVector;

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

	typedef std::pair<TextAttribute, Uint32> TextAttributeUint32Pair;
	typedef std::pair<Uint32, TextAttributeUint32Pair>
		Uint32TextAttributeUint32PairPair;
	typedef std::pair<Utf32Char, TextureGlyphe> Utf32CharTextureGlyphePair;
	typedef std::map<Utf32Char, TextureGlyphe> Utf32CharTextureGlypheMap;
	typedef std::map<Uint32, TextAttributeUint32Pair>
		Uint32TextAttributeUint32PairMap;
	typedef std::set<EffectNodePtr> EffectNodePtrSet;
	typedef std::set<EffectNodePortPtr> EffectNodePortPtrSet;
	typedef std::set<boost::uuids::uuid> UuidSet;

	typedef boost::shared_array<double> DoubleSharedArray;

	template<typename T>
	inline String to_string(const std::vector<T> &value)
	{
		StringStream str;
		Uint32 i, count;

		count = value.size();

		for (i = 0; i < count; ++i)
		{
			str << UTF8("[") << i << UTF8("] ") << value[i];
			str << std::endl;
		}

		return String(str.str());
	}

}

#endif	/* UUID_a9b0cc4b_b046_4138_b8f4_d747aac1179a */
