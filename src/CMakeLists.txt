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

		ImportDialog.cpp
		ImportDialog.h
		
		ExportDialog.cpp
		ExportDialog.h

		AboutDialog.cpp
		AboutDialog.h

		PreferencesDialog.cpp
		PreferencesDialog.h

		wxAsciiArt.cpp
		wxAsciiArt.h

		AsciiArtData.cpp
		AsciiArtData.h

		ITool.cpp
		ITool.h

		ToolArrow.cpp
		ToolArrow.h

		ToolClass.cpp
		ToolClass.h

		ToolErase.cpp
		ToolErase.h

		ToolFreehand.cpp
		ToolFreehand.h

		ToolLine.cpp
		ToolLine.h

		ToolMove.cpp
		ToolMove.h

		ToolRectangle.cpp
		ToolRectangle.h

		ToolResize.cpp
		ToolResize.h

		ToolText.cpp
		ToolText.h
)

target_compile_definitions(${TARGET} 
	PRIVATE
		-DGIT_COMMIT_HASH=${GIT_COMMIT_HASH}
		-DGIT_BRANCH=${GIT_BRANCH}
		-DGIT_DESCRIPTION=${GIT_DESCRIPTION}

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

