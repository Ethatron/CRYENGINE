#START-FILE-LIST
# File List auto generated by waf2cmake.py, do not modify manually.

start_sources()
# -- ilmbase.waf_files --
sources_platform(ALL)
add_sources("NoUberFile"
	PROJECTS ilmbase
	SOURCE_GROUP "Configuration"
		"../../SDKs/ilmbase/config.windows/IlmBaseConfig.h"
	SOURCE_GROUP "Half"
		"../../SDKs/ilmbase/Half/half.cpp"
		"../../SDKs/ilmbase/Half/half.h"
		"../../SDKs/ilmbase/Half/halfExport.h"
		"../../SDKs/ilmbase/Half/halfFunction.h"
		"../../SDKs/ilmbase/Half/halfLimits.h"
		"eLut.h"
		"toFloat.h"
	SOURCE_GROUP "Iex"
		"../../SDKs/ilmbase/Iex/IexBaseExc.cpp"
		"../../SDKs/ilmbase/Iex/IexThrowErrnoExc.cpp"
		"../../SDKs/ilmbase/Iex/IexBaseExc.h"
		"../../SDKs/ilmbase/Iex/IexMathExc.h"
		"../../SDKs/ilmbase/Iex/IexThrowErrnoExc.h"
		"../../SDKs/ilmbase/Iex/IexErrnoExc.h"
		"../../SDKs/ilmbase/Iex/IexMacros.h"
		"../../SDKs/ilmbase/Iex/Iex.h"
		"../../SDKs/ilmbase/Iex/IexNamespace.h"
		"../../SDKs/ilmbase/Iex/IexExport.h"
		"../../SDKs/ilmbase/Iex/IexForward.h"
	SOURCE_GROUP "IexMath"
		"../../SDKs/ilmbase/IexMath/IexMathFloatExc.cpp"
		"../../SDKs/ilmbase/IexMath/IexMathFpu.cpp"
		"../../SDKs/ilmbase/IexMath/IexMathFloatExc.h"
		"../../SDKs/ilmbase/IexMath/IexMathFpu.h"
		"../../SDKs/ilmbase/IexMath/IexMathIeeeExc.h"
	SOURCE_GROUP "IlmThread"
		"../../SDKs/ilmbase/IlmThread/IlmThread.cpp"
		"../../SDKs/ilmbase/IlmThread/IlmThreadMutex.cpp"
		"../../SDKs/ilmbase/IlmThread/IlmThreadMutexPosix.cpp"
		"../../SDKs/ilmbase/IlmThread/IlmThreadMutexWin32.cpp"
		"../../SDKs/ilmbase/IlmThread/IlmThreadPool.cpp"
		"../../SDKs/ilmbase/IlmThread/IlmThreadPosix.cpp"
		"../../SDKs/ilmbase/IlmThread/IlmThreadSemaphore.cpp"
		"../../SDKs/ilmbase/IlmThread/IlmThreadSemaphorePosixCompat.cpp"
		"../../SDKs/ilmbase/IlmThread/IlmThreadSemaphorePosix.cpp"
		"../../SDKs/ilmbase/IlmThread/IlmThreadSemaphoreWin32.cpp"
		"../../SDKs/ilmbase/IlmThread/IlmThreadWin32.cpp"
		"../../SDKs/ilmbase/IlmThread/IlmThread.h"
		"../../SDKs/ilmbase/IlmThread/IlmThreadPool.h"
		"../../SDKs/ilmbase/IlmThread/IlmThreadSemaphore.h"
		"../../SDKs/ilmbase/IlmThread/IlmThreadMutex.h"
		"../../SDKs/ilmbase/IlmThread/IlmThreadNamespace.h"
		"../../SDKs/ilmbase/IlmThread/IlmThreadExport.h"
		"../../SDKs/ilmbase/IlmThread/IlmThreadForward.h"
	SOURCE_GROUP "Imath"
		"../../SDKs/ilmbase/Imath/ImathBox.cpp"
		"../../SDKs/ilmbase/Imath/ImathRandom.cpp"
		"../../SDKs/ilmbase/Imath/ImathColorAlgo.cpp"
		"../../SDKs/ilmbase/Imath/ImathShear.cpp"
		"../../SDKs/ilmbase/Imath/ImathFun.cpp"
		"../../SDKs/ilmbase/Imath/ImathVec.cpp"
		"../../SDKs/ilmbase/Imath/ImathMatrixAlgo.cpp"
		"../../SDKs/ilmbase/Imath/ImathBoxAlgo.h"
		"../../SDKs/ilmbase/Imath/ImathBox.h"
		"../../SDKs/ilmbase/Imath/ImathColorAlgo.h"
		"../../SDKs/ilmbase/Imath/ImathColor.h"
		"../../SDKs/ilmbase/Imath/ImathEuler.h"
		"../../SDKs/ilmbase/Imath/ImathExc.h"
		"../../SDKs/ilmbase/Imath/ImathExport.h"
		"../../SDKs/ilmbase/Imath/ImathForward.h"
		"../../SDKs/ilmbase/Imath/ImathFrame.h"
		"../../SDKs/ilmbase/Imath/ImathFrustum.h"
		"../../SDKs/ilmbase/Imath/ImathFrustumTest.h"
		"../../SDKs/ilmbase/Imath/ImathFun.h"
		"../../SDKs/ilmbase/Imath/ImathGL.h"
		"../../SDKs/ilmbase/Imath/ImathGLU.h"
		"../../SDKs/ilmbase/Imath/ImathHalfLimits.h"
		"../../SDKs/ilmbase/Imath/ImathInt64.h"
		"../../SDKs/ilmbase/Imath/ImathInterval.h"
		"../../SDKs/ilmbase/Imath/ImathLimits.h"
		"../../SDKs/ilmbase/Imath/ImathLineAlgo.h"
		"../../SDKs/ilmbase/Imath/ImathLine.h"
		"../../SDKs/ilmbase/Imath/ImathMath.h"
		"../../SDKs/ilmbase/Imath/ImathMatrixAlgo.h"
		"../../SDKs/ilmbase/Imath/ImathMatrix.h"
		"../../SDKs/ilmbase/Imath/ImathNamespace.h"
		"../../SDKs/ilmbase/Imath/ImathPlane.h"
		"../../SDKs/ilmbase/Imath/ImathPlatform.h"
		"../../SDKs/ilmbase/Imath/ImathQuat.h"
		"../../SDKs/ilmbase/Imath/ImathRandom.h"
		"../../SDKs/ilmbase/Imath/ImathRoots.h"
		"../../SDKs/ilmbase/Imath/ImathShear.h"
		"../../SDKs/ilmbase/Imath/ImathSphere.h"
		"../../SDKs/ilmbase/Imath/ImathVecAlgo.h"
		"../../SDKs/ilmbase/Imath/ImathVec.h"
)

# -- eLut.waf_files --
sources_platform(ALL)
add_sources("NoUberFile"
	PROJECTS eLut
	SOURCE_GROUP "Root"
		"../../SDKs/ilmbase/Half/eLut.cpp"
)

# -- toFloat.waf_files --
sources_platform(ALL)
add_sources("NoUberFile"
	PROJECTS toFloat
	SOURCE_GROUP "Root"
		"../../SDKs/ilmbase/Half/toFloat.cpp"
)

end_sources()
#END-FILE-LIST

CryEngineStaticModule(ilmbase SOLUTION_FOLDER "Libs")
target_include_directories(${THIS_PROJECT} PRIVATE "${SDK_DIR}/ilmbase/Half" "${SDK_DIR}/ilmbase/Iex" "${SDK_DIR}/ilmbase/IexMath" "${SDK_DIR}/ilmbase/Imath" "${SDK_DIR}/ilmbase/config.windows" )
target_compile_definitions(${THIS_PROJECT} PRIVATE -DNOMINMAX)
target_compile_options(${THIS_PROJECT} PRIVATE /GR /EHsc ) # Enable exceptions.
target_compile_options(${THIS_PROJECT} PRIVATE /W0 ) # Disable warnings from 3rdParty code. 
target_include_directories(${THIS_PROJECT} INTERFACE "${SDK_DIR}/ilmbase/Half" "${SDK_DIR}/ilmbase/Iex" "${SDK_DIR}/ilmbase/IexMath" "${SDK_DIR}/ilmbase/Imath" "${SDK_DIR}/ilmbase/config.windows" )

# Tool to create toFloat.h
CryConsoleApplication(toFloat SOLUTION_FOLDER "Tools/ilm")
target_compile_options(${THIS_PROJECT} PRIVATE /W0)
target_compile_definitions(${THIS_PROJECT} PRIVATE -D_HAS_EXCEPTIONS=0)

# Tool to create eLut.h
CryConsoleApplication(eLut SOLUTION_FOLDER "Tools/ilm")
target_compile_options(${THIS_PROJECT} PRIVATE /W0)
target_compile_definitions(${THIS_PROJECT} PRIVATE -D_HAS_EXCEPTIONS=0)

