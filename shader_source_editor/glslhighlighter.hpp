/****************************************************************************
 *            glslhighlighter.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_c8234a71_fba2_4510_9572_cb4086737c4c
#define UUID_c8234a71_fba2_4510_9572_cb4086737c4c

#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>

class GlslHighlighter: public QSyntaxHighlighter
{
	Q_OBJECT

	private:
		class HighlightingRule;
		QVector<HighlightingRule> m_highlighting_rules;

		QRegExp m_comment_start_expression;
		QRegExp m_comment_end_expression;

		QTextCharFormat m_keyword_format;
		QTextCharFormat m_single_line_comment_format;
		QTextCharFormat m_multi_line_comment_format;
		QTextCharFormat m_function_format;
		QTextCharFormat m_variable_format;

	protected:
		void highlightBlock(const QString &text);

	public:
		GlslHighlighter(QTextDocument *parent = 0);
		virtual ~GlslHighlighter();

};

#endif	/* UUID_c8234a71_fba2_4510_9572_cb4086737c4c */
