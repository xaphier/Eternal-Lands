#ifndef	UUID_83a25097_83b3_440a_8619_19d11a2ddb1f
#define UUID_83a25097_83b3_440a_8619_19d11a2ddb1f

#include <QString>
#include <QIcon>

class TerrainTextureData
{
	private:
		QIcon m_icon;
		QString m_albedo_map;
		QString m_extra_map;
		bool m_use_blend_size_sampler;
		bool m_use_extra_map;

	public:
		TerrainTextureData();
		TerrainTextureData(const QIcon &icon,
			const QString &albedo_map,
			const QString &extra_map,
			const bool use_blend_size_sampler,
			const bool use_extra_map);
		~TerrainTextureData();

		inline const QIcon &get_icon() const
		{
			return m_icon;
		}

		inline const QString &get_albedo_map() const
		{
			return m_albedo_map;
		}

		inline const QString &get_extra_map() const
		{
			return m_extra_map;
		}

		inline bool get_use_blend_size_sampler() const
		{
			return m_use_blend_size_sampler;
		}

		inline bool get_use_extra_map() const
		{
			return m_use_extra_map;
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

		inline void set_extra_map(
			const QString &extra_map)
		{
			m_extra_map = extra_map;
		}

		inline void set_use_blend_size_sampler(
			const bool use_blend_size_sampler)
		{
			m_use_blend_size_sampler = use_blend_size_sampler;
		}

		inline void set_use_extra_map(const bool use_extra_map)
		{
			m_use_extra_map = use_extra_map;
		}

};

typedef QMap<QString, TerrainTextureData> QStringTerrainTextureDataQMap;

#endif	/* UUID_83a25097_83b3_440a_8619_19d11a2ddb1f */
