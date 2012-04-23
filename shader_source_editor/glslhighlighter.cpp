/****************************************************************************
 *            glslhighlighter.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "glslhighlighter.hpp"
#include <QTextDocument>

class GlslHighlighter::HighlightingRule
{
	public:
		QRegExp m_pattern;
		QTextCharFormat m_format;
};

GlslHighlighter::GlslHighlighter(QTextDocument *parent):
	QSyntaxHighlighter(parent)
{
	QStringList keyword_patterns, function_patterns, variable_patterns;
	HighlightingRule rule;

	m_keyword_format.setForeground(Qt::darkBlue);
	m_keyword_format.setFontWeight(QFont::Bold);

	keyword_patterns << "\\battribute\\b" << "\\bconst\\b"
		<< "\\buniform\\b" << "\\bvarying\\b" << "\\blayout\\b"
		<< "\\bcentroid\\b" << "\\bflat\\b" << "\\bsmooth\\b"
		<< "\\bnoperspective\\b" << "\\bbreak\\b" << "\\bcontinue\\b"
		<< "\\bdo\\b" << "\\bfor\\b" << "\\bwhile\\b" << "\\bswitch\\b"
		<< "\\bcase\\b" << "\\bdefault\\b" << "\\bif\\b" << "\\belse\\b"
		<< "\\bin\\b" << "\\bout\\b" << "\\binout\\b" << "\\bfloat\\b"
		<< "\\bint\\b" << "\\bvoid\\b" << "\\bbool\\b" << "\\btrue\\b"
		<< "\\bfalse\\b" << "\\binvariant\\b" << "\\bdiscard\\b"
		<< "\\breturn\\b" << "\\bstruct\\b" << "\\bmat2\\b"
		<< "\\bmat3\\b" << "\\bmat4\\b" << "\\bmat2x2\\b"
		<< "\\bmat2x3\\b" << "\\bmat2x4\\b" << "\\bmat3x2\\b"
		<< "\\bmat3x3\\b" << "\\bmat3x4\\b" << "\\bmat4x2\\b"
		<< "\\bmat4x3\\b" << "\\bmat4x4\\b" << "\\bvec2\\b"
		<< "\\bvec3\\b" << "\\bvec4\\b" << "\\bivec2\\b"
		<< "\\bivec3\\b" << "\\bivec4\\b" << "\\bbvec2\\b"
		<< "\\bbvec3\\b" << "\\bbvec4\\b" << "\\buint\\b"
		<< "\\buvec2\\b" << "\\buvec3\\b" << "\\buvec4\\b"
		<< "\\blowp\\b" << "\\bmediump\\b" << "\\bhighp\\b"
		<< "\\bprecision\\b" << "\\bsampler1D\\b" << "\\bsampler2D\\b"
		<< "\\bsampler3D\\b" << "\\bsamplerCube\\b"
		<< "\\bsampler1DShadow\\b" << "\\bsampler2DShadow\\b"
		<< "\\bsamplerCubeShadow\\b" << "\\bsampler1DArray\\b"
		<< "\\bsampler2DArray\\b" << "\\bsampler1DArrayShadow\\b"
		<< "\\bsampler2DArrayShadow\\b" << "\\bisampler1D\\b"
		<< "\\bisampler2D\\b" << "\\bisampler3D\\b"
		<< "\\bisamplerCube\\b" << "\\bisampler1DArray\\b"
		<< "\\bisampler2DArray\\b" << "\\busampler1D\\b"
		<< "\\busampler2D\\b" << "\\busampler3D\\b"
		<< "\\busamplerCube\\b" << "\\busampler1DArray\\b"
		<< "\\busampler2DArray\\b" << "\\bsampler2DRect\\b"
		<< "\\bsampler2DRectShadow\\b" << "\\bisampler2DRect\\b"
		<< "\\busampler2DRect\\b" << "\\bsamplerBuffer\\b"
		<< "\\bisamplerBuffer\\b" << "\\busamplerBuffer\\b"
		<< "\\bsampler2DMS\\b" << "\\bisampler2DMS\\b"
		<< "\\busampler2DMS\\b" << "\\bsampler2DMSArray\\b"
		<< "\\bisampler2DMSArray\\b" << "\\busampler2DMSArray\\b";

	foreach(const QString &pattern, keyword_patterns)
	{
		rule.m_pattern = QRegExp(pattern);
		rule.m_format = m_keyword_format;
		m_highlighting_rules.append(rule);
	}

	m_single_line_comment_format.setForeground(Qt::red);
	rule.m_pattern = QRegExp("//[^\n]*");
	rule.m_format = m_single_line_comment_format;
	m_highlighting_rules.append(rule);

	m_function_format.setForeground(Qt::blue);

	function_patterns << "\\bradians\\b" << "\\bdegrees\\b" << "\\bsin\\b"
		<< "\\bcos\\b" << "\\btan\\b" << "\\basin\\b" << "\\bacos\\b"
		<< "\\batan\\b" << "\\batan\\b" << "\\bsinh\\b" << "\\bcosh\\b"
		<< "\\btanh\\b" << "\\basinh\\b" << "\\bacosh\\b"
		<< "\\batanh\\b" << "\\bpow\\b" << "\\bexp\\b" << "\\blog\\b"
		<< "\\bexp2\\b" << "\\blog2\\b" << "\\bsqrt\\b"
		<< "\\binversesqrt\\b" << "\\babs\\b" << "\\bsign\\b"
		<< "\\bfloor\\b" << "\\btrunc\\b" << "\\bround\\b"
		<< "\\broundEven\\b" << "\\bceil\\b" << "\\bfract\\b"
		<< "\\bmod\\b" << "\\bmodf\\b" << "\\bmin\\b" << "\\bmax\\b"
		<< "\\bclamp\\b" << "\\bmix\\b" << "\\bstep\\b"
		<< "\\bsmoothstep\\b" << "\\bisnan\\b" << "\\bisinf\\b"
		<< "\\bfloatBitsToInt\\b" << "\\bfloatBitsToUint\\b"
		<< "\\bintBitsToFloat\\b" << "\\buintBitsToFloat\\b"
		<< "\\blength\\b" << "\\bdistance\\b" << "\\bdot\\b"
		<< "\\bcross\\b" << "\\bnormalize\\b" << "\\bfaceforward\\b"
		<< "\\breflect\\b" << "\\brefract\\b" << "\\bmatrixCompMult\\b"
		<< "\\bouterProduct\\b" << "\\btranspose\\b"
		<< "\\bdeterminant\\b" << "\\binverse\\b" << "\\blessThan\\b"
		<< "\\blessThanEqual\\b" << "\\bgreaterThan\\b"
		<< "\\bgreaterThanEqual\\b" << "\\bequal\\b"
		<< "\\bnotEqual\\b" << "\\bany\\b" << "\\ball\\b"
		<< "\\bnot\\b" << "\\btextureSize\\b" << "\\btexture\\b"
		<< "\\btextureProj\\b" << "\\btextureLod\\b"
		<< "\\btextureOffset\\b" << "\\btexelFetch\\b"
		<< "\\btexelFetchOffset\\b" << "\\btextureProjOffset\\b"
		<< "\\btextureLodOffset\\b" << "\\btextureProjLod\\b"
		<< "\\btextureProjLodOffset\\b" << "\\btextureGrad\\b"
		<< "\\btextureGradOffset\\b" << "\\btextureProjGrad\\b"
		<< "\\btextureProjGradOffset\\b" << "\\btexture1D\\b"
		<< "\\btexture1DProj\\b" << "\\btexture1DProjLod\\b"
		<< "\\btexture2D\\b" << "\\btexture2DProj\\b"
		<< "\\btexture2DLod\\b" << "\\btexture2DProjLod\\b"
		<< "\\btexture3D\\b" << "\\btexture3DProj\\b"
		<< "\\btexture3DLod\\b" << "\\btexture3DProjLod\\b"
		<< "\\btextureCube\\b" << "\\btextureCubeLod\\b"
		<< "\\bshadow1D\\b" << "\\bshadow2D\\b" << "\\bshadow1DProj\\b"
		<< "\\bshadow2DProj\\b" << "\\bshadow1DLod\\b"
		<< "\\bshadow2DLod\\b" << "\\bshadow1DProjLod\\b"
		<< "\\bshadow2DProjLod\\b" << "\\bdFdx\\b" << "\\bdFdy\\b"
		<< "\\bfwidth\\b" << "\\bnoise1\\b" << "\\bnoise2\\b"
		<< "\\bnoise3\\b" << "\\bnoise4\\b" << "\\bEmitVertex\\b"
		<< "\\bEndPrimitive\\b";

	foreach(const QString &pattern, function_patterns)
	{
		rule.m_pattern = QRegExp(pattern);
		rule.m_format = m_function_format;
		m_highlighting_rules.append(rule);
	}

	m_variable_format.setForeground(Qt::blue);
	m_variable_format.setFontWeight(QFont::Bold);

	variable_patterns << "\\bgl_VertexID\\b" << "\\bgl_InstanceID\\b"
		<< "\\bgl_Position\\b" << "\\bgl_PointSize\\b"
		<< "\\bgl_ClipDistance\\b" << "\\bgl_PerVertex\\b"
		<< "\\bgl_Layer\\b" << "\\bgl_ClipVertex\\b"
		<< "\\bgl_FragCoord\\b" << "\\bgl_FrontFacing\\b"
		<< "\\bgl_ClipDistance\\b" << "\\bgl_FragColor\\b"
		<< "\\bgl_FragData\\b" << "\\bgl_MaxDrawBuffers\\b"
		<< "\\bgl_FragDepth\\b" << "\\bgl_PointCoord\\b"
		<< "\\bgl_PrimitiveID\\b" << "\\bgl_MaxVertexAttribs\\b"
		<< "\\bgl_MaxVertexUniformComponents\\b"
		<< "\\bgl_MaxVaryingFloats\\b"
		<< "\\bgl_MaxVaryingComponents\\b"
		<< "\\bgl_MaxVertexOutputComponents\\b"
		<< "\\bgl_MaxGeometryInputComponents\\b"
		<< "\\bgl_MaxGeometryOutputComponents\\b"
		<< "\\bgl_MaxFragmentInputComponents\\b"
		<< "\\bgl_MaxVertexTextureImageUnits\\b"
		<< "\\bgl_MaxCombinedTextureImageUnits\\b"
		<< "\\bgl_MaxTextureImageUnits\\b"
		<< "\\bgl_MaxFragmentUniformComponents\\b"
		<< "\\bgl_MaxDrawBuffers\\b" << "\\bgl_MaxClipDistances\\b"
		<< "\\bgl_MaxGeometryTextureImageUnits\\b"
		<< "\\bgl_MaxGeometryOutputVertices\\b"
		<< "\\bgl_MaxGeometryOutputVertices\\b"
		<< "\\bgl_MaxGeometryTotalOutputComponents\\b"
		<< "\\bgl_MaxGeometryUniformComponents\\b"
		<< "\\bgl_MaxGeometryVaryingComponents\\b"
		<< "\\bgl_DepthRange\\b";

	foreach(const QString &pattern, variable_patterns)
	{
		rule.m_pattern = QRegExp(pattern);
		rule.m_format = m_variable_format;
		m_highlighting_rules.append(rule);
	}

	m_multi_line_comment_format.setForeground(Qt::red);
	m_comment_start_expression = QRegExp("/\\*");
	m_comment_end_expression = QRegExp("\\*/");
}

GlslHighlighter::~GlslHighlighter()
{
}

void GlslHighlighter::highlightBlock(const QString &text)
{
	int index, length, start_index, end_index;

	foreach(const HighlightingRule &rule, m_highlighting_rules)
	{
		QRegExp expression(rule.m_pattern);

		index = expression.indexIn(text);

		while (index >= 0)
		{
			length = expression.matchedLength();
			setFormat(index, length, rule.m_format);
			index = expression.indexIn(text, index + length);
		}
	}

	setCurrentBlockState(0);

	start_index = 0;

	if (previousBlockState() != 1)
	{
		start_index = m_comment_start_expression.indexIn(text);
	}

	while (start_index >= 0)
	{
		end_index = m_comment_end_expression.indexIn(text, start_index);

		if (end_index == -1)
		{
			setCurrentBlockState(1);
			length = text.length() - start_index;
		}
		else
		{
			length = end_index - start_index
				+ m_comment_end_expression.matchedLength();
		}

		setFormat(start_index, length, m_multi_line_comment_format);
		start_index = m_comment_start_expression.indexIn(text,
			start_index + length);
	}
}
