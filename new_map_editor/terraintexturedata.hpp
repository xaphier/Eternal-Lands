#ifndef	UUID_83a25097_83b3_440a_8619_19d11a2ddb1f
#define UUID_83a25097_83b3_440a_8619_19d11a2ddb1f

#include <QString>
#include <QIcon>
#include <QColor>
#include <QListWidgetItem>

enum TerrainTextureRoles
{
	ttr_albedo_map = Qt::UserRole,
	ttr_height_map = Qt::UserRole + 1,
	ttr_specular_map = Qt::UserRole + 2,
	ttr_gloss_map = Qt::UserRole + 3,
	ttr_default_specular = Qt::UserRole + 4,
	ttr_default_gloss = Qt::UserRole + 5,
	ttr_default_height = Qt::UserRole + 6,
	ttr_blend_size = Qt::UserRole + 7,
	ttr_use_blend_size_texture = Qt::UserRole + 8,
	ttr_use_specular_map = Qt::UserRole + 9,
	ttr_use_gloss_map = Qt::UserRole + 10,
	ttr_use_height_map = Qt::UserRole + 11
};

class TerrainTextureData
{
	private:
		QIcon m_icon;
		QString m_albedo_map;
		QString m_specular_map;
		QString m_gloss_map;
		QString m_height_map;
		QColor m_default_specular;
		float m_default_gloss;
		float m_default_height;
		float m_blend_size;
		bool m_use_blend_size_texture;
		bool m_use_specular_map;
		bool m_use_gloss_map;
		bool m_use_height_map;

	public:
		TerrainTextureData();
		TerrainTextureData(const QIcon &icon,
			const QString &albedo_map,
			const QString &specular_map,
			const QString &gloss_map,
			const QString &height_map,
			const QColor &default_specular,
			const float default_gloss,
			const float default_height,
			const float blend_size,
			const bool use_blend_size_texture,
			const bool use_specular_map,
			const bool use_gloss_map,
			const bool use_height_map);
		~TerrainTextureData();
		void set_list_widget_item(QListWidgetItem* item) const;
		QString get_tool_tip() const;

		inline const QIcon &get_icon() const
		{
			return m_icon;
		}

		inline const QString &get_albedo_map() const
		{
			return m_albedo_map;
		}

		inline const QString &get_height_map() const
		{
			return m_height_map;
		}

		inline const QString &get_specular_map() const
		{
			return m_specular_map;
		}

		inline const QString &get_gloss_map() const
		{
			return m_gloss_map;
		}

		inline bool get_use_blend_size_texture() const
		{
			return m_use_blend_size_texture;
		}

		inline float get_blend_size() const
		{
			return m_blend_size;
		}

		inline bool get_use_height_map() const
		{
			return m_use_height_map;
		}

		inline bool get_use_specular_map() const
		{
			return m_use_specular_map;
		}

		inline bool get_use_gloss_map() const
		{
			return m_use_gloss_map;
		}

		inline float get_default_height() const
		{
			return m_default_height;
		}

		inline const QColor &get_default_specular() const
		{
			return m_default_specular;
		}

		inline float get_default_gloss() const
		{
			return m_default_gloss;
		}

		inline void set_icon(
			const QIcon &icon)
		{
			m_icon = icon;
		}

		inline void set_albedo_map(
			const QString &albedo_map)
		{
			m_albedo_map = albedo_map;
		}

		inline void set_height_map(const QString &height_map)
		{
			m_height_map = height_map;
		}

		inline void set_specular_map(const QString &specular_map)
		{
			m_specular_map = specular_map;
		}

		inline void set_gloss_map(const QString &gloss_map)
		{
			m_gloss_map = gloss_map;
		}

		inline void set_use_blend_size_texture(
			const bool use_blend_size_texture)
		{
			m_use_blend_size_texture = use_blend_size_texture;
		}

		inline void set_blend_size(const float blend_size)
		{
			m_blend_size = blend_size;
		}

		inline void set_use_height_map(const bool use_height_map)
		{
			m_use_height_map = use_height_map;
		}

		inline void set_use_specular_map(const bool use_specular_map)
		{
			m_use_specular_map = use_specular_map;
		}

		inline void set_use_gloss_map(const bool use_gloss_map)
		{
			m_use_gloss_map = use_gloss_map;
		}

		inline void set_default_specular(
			const QColor &default_specular)
		{
			m_default_specular = default_specular;
		}

		inline void set_default_height(const float default_height)
		{
			m_default_height = default_height;
		}

		inline void set_default_gloss(const float default_gloss)
		{
			m_default_gloss = default_gloss;
		}

};

typedef QMap<QString, TerrainTextureData> QStringTerrainTextureDataQMap;

#endif	/* UUID_83a25097_83b3_440a_8619_19d11a2ddb1f */
