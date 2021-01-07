############################################################################
# executable type
############################################################################
if (WIN32)
	set(EXECUTABLE_TYPE Win32)
else()
	set(EXECUTABLE_TYPE)
endif()

set(TARGET	AsciiFlowWx)

add_executable(${TARGET} ${EXECUTABLE_TYPE})

target_sources(${TARGET} 
	PRIVATE
		main.cpp
		
		App.cpp
		App.h
		
		MainFrame.cpp
		MainFrame.h
)

target_compile_definitions(${TARGET} 
	PRIVATE
		${wxWidgets_DEFINITIONS}
)

target_include_directories(${TARGET} 
	PRIVATE
		${wxWidgets_INCLUDE_DIRS}
)

target_link_directories(${TARGET} 
	PRIVATE
		${wxWidgets_LIBRARY_DIRS}
)

target_link_libraries(${TARGET} 
	PRIVATE
		${wxWidgets_LIBRARIES}
)

