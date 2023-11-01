#ifndef HLSL_BUILDER_HH
#define HLSL_BUILDER_HH

#ifdef HLSLB_WINDOWS
	#ifdef HLSLB_DYNAMIC_LINK
		#ifdef HLSLB_BUILD_DLL
			#define HLSLB_API __declspec(dllexport)
		#else
			#define HLSLB_API __declspec(dllimport)
		#endif
	#else
		#define HLSLB_API
	#endif
#else
	#define HLSLB_API
#endif

#endif