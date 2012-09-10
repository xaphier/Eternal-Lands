/****************************************************************************
 *            gpucounters.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "gpucounters.hpp"
#include "logging.hpp"
#ifdef	USE_GPU_COUNTER
#ifndef _WIN32
#define	_LINUX
#endif	/* _WIN32 */

extern "C" {
#include "GPUPerfAPI.h"

}
#endif	/* USE_GPU_COUNTER */

namespace eternal_lands
{

	namespace
	{
/*
		GPA_RegisterLoggingCallbackPtrType el_GPA_RegisterLoggingCallback;

		// Startup / exit
		GPA_InitializePtrType el_GPA_InitializePtrType;
		GPA_DestroyPtrType el_GPA_DestroyPtrType;

		// Context
		GPA_OpenContextPtrType el_GPA_OpenContextPtrType;
		GPA_CloseContextPtrType el_GPA_CloseContextPtrType;
		GPA_SelectContextPtrType el_GPA_SelectContextPtrType;

		// Counter Interrogation
		GPA_GetNumCountersPtrType el_GPA_GetNumCountersPtrType;
		GPA_GetCounterNamePtrType el_GPA_GetCounterNamePtrType;
		GPA_GetCounterDescriptionPtrType el_GPA_GetCounterDescriptionPtrType;
		GPA_GetCounterDataTypePtrType el_GPA_GetCounterDataTypePtrType;
		GPA_GetCounterUsageTypePtrType el_GPA_GetCounterUsageTypePtrType;
		GPA_GetDataTypeAsStrPtrType el_GPA_GetDataTypeAsStrPtrType;
		GPA_GetUsageTypeAsStrPtrType el_GPA_GetUsageTypeAsStrPtrType;
		GPA_GetStatusAsStrPtrType el_GPA_GetStatusAsStrPtrType;

		GPA_EnableCounterPtrType el_GPA_EnableCounterPtrType;
		GPA_DisableCounterPtrType el_GPA_DisableCounterPtrType;
		GPA_GetEnabledCountPtrType el_GPA_GetEnabledCountPtrType;
		GPA_GetEnabledIndexPtrType el_GPA_GetEnabledIndexPtrType;

		GPA_IsCounterEnabledPtrType el_GPA_IsCounterEnabledPtrType;

		GPA_EnableCounterStrPtrType el_GPA_EnableCounterStrPtrType;
		GPA_DisableCounterStrPtrType el_GPA_DisableCounterStrPtrType;

		GPA_EnableAllCountersPtrType el_GPA_EnableAllCountersPtrType;
		GPA_DisableAllCountersPtrType el_GPA_DisableAllCountersPtrType;
		GPA_GetCounterIndexPtrType el_GPA_GetCounterIndexPtrType;

		GPA_GetPassCountPtrType el_GPA_GetPassCountPtrType;

		GPA_BeginSessionPtrType el_GPA_BeginSessionPtrType;
		GPA_EndSessionPtrType el_GPA_EndSessionPtrType;

		GPA_BeginPassPtrType el_GPA_BeginPassPtrType;
		GPA_EndPassPtrType el_GPA_EndPassPtrType;

		GPA_BeginSamplePtrType el_GPA_BeginSamplePtrType;
		GPA_EndSamplePtrType el_GPA_EndSamplePtrType;

		GPA_GetSampleCountPtrType el_GPA_GetSampleCountPtrType;

		GPA_IsSampleReadyPtrType el_GPA_IsSampleReadyPtrType;
		GPA_IsSessionReadyPtrType el_GPA_IsSessionReadyPtrType;
		GPA_GetSampleUInt64PtrType el_GPA_GetSampleUInt64PtrType;
		GPA_GetSampleUInt32PtrType el_GPA_GetSampleUInt32PtrType;
		GPA_GetSampleFloat32PtrType el_GPA_GetSampleFloat32PtrType;
		GPA_GetSampleFloat64PtrType el_GPA_GetSampleFloat64PtrType;
*/
#ifdef	USE_GPU_COUNTER
		void el_log_gpa(GPA_Logging_Type messageType,
			const char* message)
		{
			std::cout << messageType << ": " << message << std::endl;

			LOG_INFO(lt_opengl, "%1%: %2%'", messageType % message);
		}
#endif	/* USE_GPU_COUNTER */

	}
#ifdef	USE_GPU_COUNTER

#define CHECK_GPA(function)	\
	do	\
	{	\
		GPA_Status status;	\
	\
		status = ::function;	\
	\
		if (status != GPA_STATUS_OK)	\
		{	\
			LOG_ERROR(lt_opengl, "GPA error '%1%'",	\
				GPA_GetStatusAsStr(status));	\
		}	\
	}	\
	while (false)
#endif	/* USE_GPU_COUNTER */

	GpuCounters::GpuCounters(): m_session_id(0)
	{
#ifdef	USE_GPU_COUNTER
		CHECK_GPA(GPA_Initialize());

		GPA_RegisterLoggingCallback(GPA_LOGGING_ERROR_AND_MESSAGE, el_log_gpa);
#endif	/* USE_GPU_COUNTER */
	}

	GpuCounters::~GpuCounters() noexcept
	{
#ifdef	USE_GPU_COUNTER
		CHECK_GPA(GPA_Destroy());
#endif	/* USE_GPU_COUNTER */
	}

	void GpuCounters::open_gl_context(void* context)
	{
#ifdef	USE_GPU_COUNTER
		CHECK_GPA(GPA_OpenContext(context));
#endif	/* USE_GPU_COUNTER */
	}

	void GpuCounters::close_gl_context()
	{
#ifdef	USE_GPU_COUNTER
		CHECK_GPA(GPA_CloseContext());
#endif	/* USE_GPU_COUNTER */
	}

	void GpuCounters::select_gl_context(void* context)
	{
#ifdef	USE_GPU_COUNTER
		CHECK_GPA(GPA_SelectContext(context));
#endif	/* USE_GPU_COUNTER */
	}

	void GpuCounters::log_counters()
	{
#ifdef	USE_GPU_COUNTER
		StringStream str;
		const char* name;
		const char* description;
		const char* data_str;
		const char* usage_str;
		gpa_uint32 i, count;
		GPA_Type data;
		GPA_Usage_Type usage;

		count = 0;

		GPA_GetNumCounters(&count);

		for (i = 0; i < count; ++i)
		{
			CHECK_GPA(GPA_GetCounterName(i, &name));
			CHECK_GPA(GPA_GetCounterDescription(i, &description));
			CHECK_GPA(GPA_GetCounterDataType(i, &data));
			CHECK_GPA(GPA_GetCounterUsageType(i, &usage));
			CHECK_GPA(GPA_GetDataTypeAsStr(data, &data_str));
			CHECK_GPA(GPA_GetUsageTypeAsStr(usage, &usage_str));

			str << name << "[" << data_str << ":";
			str << usage_str << "]" << description << std::endl;
		}

		LOG_INFO(lt_opengl, "counter\n%1%", str.str());
#endif	/* USE_GPU_COUNTER */
	}

	void GpuCounters::init_counters()
	{
	}

	void GpuCounters::enable_counters()
	{
#ifdef	USE_GPU_COUNTER
		GPA_EnableAllCounters();
#endif	/* USE_GPU_COUNTER */
	}

	void GpuCounters::disable_counters()
	{
#ifdef	USE_GPU_COUNTER
		GPA_DisableAllCounters();
#endif	/* USE_GPU_COUNTER */
	}

#ifdef	USE_GPU_COUNTER
	static int tada = 0;


/// Given a sessionID, query the counter values and save them to a file
void WriteSession( gpa_uint32 currentWaitSessionID, const char* filename )
{
	static bool doneHeadings = false;
	gpa_uint32 count;
	GPA_GetEnabledCount( &count );

	FILE* f;
	if ( !doneHeadings )
	{
		const char* name;
		f = fopen( filename, "w" );
		assert( f );
		fprintf( f, "Identifier, " );

		for (gpa_uint32 counter = 0 ; counter < count ; counter++ )
		{
			gpa_uint32 enabledCounterIndex;
			GPA_GetEnabledIndex( counter, &enabledCounterIndex );
			GPA_GetCounterName( enabledCounterIndex, &name );
			fprintf( f, "%s\t\t", name );
		}
		fprintf( f, "\n" );
		fclose( f );
		doneHeadings = true;
	}

	f = fopen( filename, "a+" );
	assert( f );
	gpa_uint32 sampleCount;
	GPA_GetSampleCount( currentWaitSessionID, &sampleCount );
	for (gpa_uint32 sample = 0 ; sample < sampleCount ; sample++ )
	{
		fprintf( f, "session: %d; sample: %d, ", currentWaitSessionID,
			sample );
		for (gpa_uint32 counter = 0 ; counter < count ; counter++ )
		{
			gpa_uint32 enabledCounterIndex;
			GPA_GetEnabledIndex( counter, &enabledCounterIndex );
			GPA_Type type;
			GPA_Usage_Type usage;
			std::string usage_str;
			GPA_GetCounterDataType( enabledCounterIndex, &type );
			GPA_GetCounterUsageType( enabledCounterIndex, &usage );

			switch (usage)
			{
				case GPA_USAGE_TYPE_RATIO:		///< Result is a ratio of two different values or types
					usage_str = " ratio";
					break;
				case GPA_USAGE_TYPE_PERCENTAGE:		///< Result is a percentage, typically within [0,100] range, but may be higher for certain counters
					usage_str = " %";
					break;
				case GPA_USAGE_TYPE_CYCLES:		///< Result is in clock cycles
					usage_str = " clock cycles";
					break;
				case GPA_USAGE_TYPE_MILLISECONDS:	///< Result is in milliseconds
 					usage_str = " ms";
					break;
				case GPA_USAGE_TYPE_BYTES:		///< Result is in bytes
					usage_str = " bytes";
					break;
				case GPA_USAGE_TYPE_ITEMS:		///< Result is a count of items or objects (ie, vertices, triangles, threads, pixels, texels, etc)
					usage_str = " items";
					break;
				case GPA_USAGE_TYPE_KILOBYTES:		///< Result is in kilobytes
					usage_str = " kilobytes";
					break;
				default:
					usage_str = "";
			};
				

			switch (type)
			{
				case GPA_TYPE_INT32:
				{
					gpa_uint32 value;
					GPA_GetSampleUInt32( currentWaitSessionID,
						sample, enabledCounterIndex, &value );
					fprintf( f, "%i%s\t\t", value, usage_str.c_str());
					break;
				}
				case GPA_TYPE_INT64:
				{
					gpa_uint64 value;
					GPA_GetSampleUInt64( currentWaitSessionID,
					sample, enabledCounterIndex, &value );
					fprintf( f, "%li%s\t\t", value, usage_str.c_str());
					break;
				}
				case GPA_TYPE_UINT32:
				{
					gpa_uint32 value;
					GPA_GetSampleUInt32( currentWaitSessionID,
						sample, enabledCounterIndex, &value );
					fprintf( f, "%u%s\t\t", value, usage_str.c_str());
					break;
				}
				case GPA_TYPE_UINT64:
				{
					gpa_uint64 value;
					GPA_GetSampleUInt64( currentWaitSessionID,
					sample, enabledCounterIndex, &value );
					fprintf( f, "%lu%s\t\t", value, usage_str.c_str());
					break;
				}
				case GPA_TYPE_FLOAT32:
				{
					gpa_float32 value;
					GPA_GetSampleFloat32( currentWaitSessionID,
						sample, enabledCounterIndex, &value );
					fprintf( f, "%.3f%s\t\t", value, usage_str.c_str());
					break;
				}
				case GPA_TYPE_FLOAT64:
				{
					gpa_float64 value;
					GPA_GetSampleFloat64( currentWaitSessionID,
						sample, enabledCounterIndex, &value );
					fprintf( f, "%.3f%s\t\t", value, usage_str.c_str());
					break;
				}
				default:
					break;
			};

		}
		fprintf( f, "\n" );
	}
	fclose( f );

	if (tada == 2)
	{	
		exit(1);
	}
	tada++;
}
#endif	/* USE_GPU_COUNTER */

	void GpuCounters::print_counters()
	{
#ifdef	USE_GPU_COUNTER
		GPA_Status status;
		bool ready;

		ready = false;

		if (m_session_ids.size() < 2)
		{
			return;
		}

		status = GPA_IsSessionReady(&ready, m_session_ids.front());

		while (status == GPA_STATUS_ERROR_SESSION_NOT_FOUND)
		{
			// skipping a session which got overwritten
			m_session_ids.pop();

			if (m_session_ids.size() < 1)
			{
				return;
			}

			status = GPA_IsSessionReady(&ready,
				m_session_ids.front());
		}

		if (ready)
		{
			WriteSession(m_session_ids.front(),
				"/home/daniel/el_samples.txt");

			m_session_ids.pop();
		}
#endif	/* USE_GPU_COUNTER */
	}

	void GpuCounters::begin_session()
	{
#ifdef	USE_GPU_COUNTER
		gpa_uint32 session_id;

		session_id = m_session_id + 1;

		GPA_BeginSession(&session_id);

		m_session_ids.push(session_id);

		m_session_id = session_id;
#endif	/* USE_GPU_COUNTER */
	}

	void GpuCounters::end_session()
	{
#ifdef	USE_GPU_COUNTER
		GPA_EndSession();
#endif	/* USE_GPU_COUNTER */
	}

	Uint32 GpuCounters::get_required_passes()
	{
#ifdef	USE_GPU_COUNTER
		gpa_uint32 required_passes;

		GPA_GetPassCount(&required_passes);

		return required_passes;
#else
		return 0;
#endif	/* USE_GPU_COUNTER */
	}

	void GpuCounters::begin_pass()
	{
#ifdef	USE_GPU_COUNTER
		GPA_BeginPass();
#endif	/* USE_GPU_COUNTER */
	}

	void GpuCounters::end_pass()
	{
#ifdef	USE_GPU_COUNTER
		GPA_EndPass();
#endif	/* USE_GPU_COUNTER */
	}

	void GpuCounters::begin_sample(const Uint32 index)
	{
#ifdef	USE_GPU_COUNTER
		GPA_BeginSample(index);
#endif	/* USE_GPU_COUNTER */
	}

	void GpuCounters::end_sample()
	{
#ifdef	USE_GPU_COUNTER
		GPA_EndSample();
#endif	/* USE_GPU_COUNTER */
	}

}
