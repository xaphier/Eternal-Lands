/****************************************************************************
 *            shadersourceparameter.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_04c84cd0_2421_47d1_9c6c_e6b1ceeeab9c
#define	UUID_04c84cd0_2421_47d1_9c6c_e6b1ceeeab9c

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "parameterutil.hpp"
#include "parametersizeutil.hpp"
#include "parameterqualifierutil.hpp"

/**
 * @file
 * @brief The @c class ShaderSourceParameter.
 * This file contains the @c class ShaderSourceParameter.
 */
namespace eternal_lands
{

	class ShaderSourceParameter
	{
		private:
			String m_name;
			String m_source;
			ParameterType m_type;
			ParameterQualifierType m_qualifier;
			ParameterSizeType m_size;
			Uint16 m_scale;

		public:
			ShaderSourceParameter();
			ShaderSourceParameter(const xmlNodePtr node);
			ShaderSourceParameter(const String &name,
				const ParameterType type,
				const ParameterQualifierType qualifier,
				const ParameterSizeType size,
				const Uint16 array_size);
			~ShaderSourceParameter() throw();
			void load_xml(const xmlNodePtr node);
			void save_xml(const XmlWriterSharedPtr &writer) const;

			/**
			 * Equal operator, needed for error checking
			 */
			inline bool operator<(
				const ShaderSourceParameter &parameter) const
			{
				return get_name() < parameter.get_name();
			}

			/**
			 * Returns true if this can be used for the given
			 * parameter.
			 */
			bool get_use(const ShaderSourceParameter &parameter)
				const;

			void log(OutStream &str) const;

			/**
			 * @brief Sets the name of the parameter.
			 * Sets the name of the parameter.
			 * @param name the name of the parameter.
			 */
			inline void set_name(const String &name)
			{
				m_name = name;
			}

			/**
			 * @brief Sets the source of the parameter.
			 * Sets the source of the parameter.
			 * @param source the source of the parameter.
			 */
			inline void set_source(const String &source)
			{
				m_source = source;
			}

			/**
			 * @brief Sets the type of the parameter.
			 * Sets the type of the parameter.
			 * @param type the type of the parameter.
			 */
			inline void set_type(const ParameterType type)
			{
				m_type = type;
			}

			/**
			 * @brief Sets the qualifier of the parameter.
			 * Sets the qualifier of the parameter.
			 * @param qualifier the qualifier of the parameter.
			 */
			inline void set_qualifier(
				const ParameterQualifierType qualifier)
			{
				m_qualifier = qualifier;
			}

			/**
			 * @brief Sets the size type of the parameter.
			 * Sets the size type of the parameter.
			 * @param size the size type of the parameter.
			 */
			inline void set_size(const ParameterSizeType size)
			{
				m_size = size;
			}

			/**
			 * @brief Sets the scale of the parameter.
			 * Sets the scale of the parameter.
			 * @param scale the scale of the parameter.
			 */
			inline void set_scale(const Uint16 scale)
			{
				m_scale = scale;
			}

			/**
			 * @brief Gets the name of the parameter.
			 * Gets the name of the parameter.
			 * @return the name of the parameter.
			 */
			inline const String &get_name() const
			{
				return m_name;
			}

			/**
			 * @brief Gets the source of the parameter.
			 * Gets the source of the parameter.
			 * @return the source of the parameter.
			 */
			inline const String &get_source() const
			{
				return m_source;
			}

			/**
			 * @brief Gets the type of the parameter.
			 * Gets the type of the parameter.
			 * @return the type of the parameter.
			 */
			inline ParameterType get_type() const
			{
				return m_type;
			}

			/**
			 * @brief Gets the qualifier of the parameter.
			 * Gets the qualifier of the parameter.
			 * @return the qualifier of the parameter.
			 */
			inline ParameterQualifierType get_qualifier() const
			{
				return m_qualifier;
			}

			/**
			 * @brief Gets the size type of the parameter.
			 * Gets the size type of the parameter.
			 * @return the size type of the parameter.
			 */
			inline ParameterSizeType get_size() const
			{
				return m_size;
			}

			/**
			 * @brief Gets the scale of the parameter.
			 * Gets the scale of the parameter.
			 * @return the scale of the parameter.
			 */
			inline Uint16 get_scale() const
			{
				return m_scale;
			}

			Uint16 get_array_size(
				const ParameterSizeTypeUint16Map &sizes) const;

			void write(const ParameterSizeTypeUint16Map &sizes,
				OutStream &str) const;
			void write(const ParameterSizeTypeUint16Map &sizes,
				const String &prefix, OutStream &str,
				bool &first) const;
			void write_name(OutStream &str, bool &first) const;
			void write_parameter(
				const ParameterSizeTypeUint16Map &sizes,
				OutStream &str, bool &first) const;

	};

	OutStream& operator<<(OutStream &str,
		const ShaderSourceParameter &value);

}

#endif	/* UUID_04c84cd0_2421_47d1_9c6c_e6b1ceeeab9c */