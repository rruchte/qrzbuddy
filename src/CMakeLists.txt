set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (WIN32)
    set(APP_GUI_FLAG WIN32)
elseif(APPLE)
    set(APP_GUI_FLAG MACOSX_BUNDLE)
else()
    set(APP_GUI_FLAG "")
endif()

add_executable(qrzbuddy
        ${APP_GUI_FLAG}
        main.cpp
        Action.h
        AppCommand.cpp
        AppCommand.h
        AppController.cpp
        AppController.h
        Configuration.h
        Configuration.cpp
        OutputFormat.h
        QRZClient.h
        Util.h
        Util.cpp
        exception/AuthenticationException.cpp
        model/Callsign.h
        model/CallsignMarshaler.cpp
        model/DXCC.h
        model/DXCCMarshaler.cpp
        render/BioRenderer.h
        render/CallsignConsoleRenderer.h
        render/CallsignCSVRenderer.h
        render/CallsignMarkdownRenderer.h
        render/CallsignJSONRenderer.h
        render/CallsignXMLRenderer.h
        render/DXCCConsoleRenderer.h
        render/DXCCCSVRenderer.h
        render/DXCCJSONRenderer.h
        render/DXCCMarkdownRenderer.h
        render/DXCCXMLRenderer.h
        render/Renderer.h
        render/RendererFactory.h
        mainwindow.h
        mainwindow.cpp
        tablemodel.h
        tablemodel.cpp
        SettingsDialog.cpp
        LoginDialog.h
        LoginDialog.cpp
        js8call/Js8CallClient.h
        js8call/Js8CallClient.cpp
        callsigntableview.h
        callsigntableview.cpp
        exception/NotFoundException.h
        exception/NotFoundException.cpp
        DetailDialog.cpp
        DetailDialog.h
        detailtablemodel.h
        detailtablemodel.cpp
        detailtableview.cpp
        detailtableview.h
        PrintHandler.cpp
        qrzbuddy.qrc
        mapwindow.h
        mapwindow.cpp
        MaidenheadUtils.h
        MaidenheadUtils.cpp
        js8call/Js8CallRequest.h
        js8call/Js8CallRequest.cpp
        js8call/Message.h
        js8call/Message.cpp
        js8call/DriftingDateTime.h
        js8call/DriftingDateTime.cpp
)

if (APPLE)
    # macOS Dock/Finder icons come from the app bundle's .icns resource,
    # not the runtime Qt window icon. Generate it from the shared PNG.
    set(MACOS_ICON "${CMAKE_CURRENT_BINARY_DIR}/qrzbuddy.icns")
    add_custom_command(
        OUTPUT "${MACOS_ICON}"
        COMMAND /bin/bash
            "${CMAKE_CURRENT_SOURCE_DIR}/icons/Darwin/generate-icon.sh"
            "${CMAKE_CURRENT_SOURCE_DIR}/icons/Unix/qrzbuddy_icon.png"
            "${MACOS_ICON}"
        DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/icons/Darwin/generate-icon.sh"
            "${CMAKE_CURRENT_SOURCE_DIR}/icons/Unix/qrzbuddy_icon.png"
        VERBATIM
    )
    set_source_files_properties("${MACOS_ICON}" PROPERTIES
        GENERATED TRUE
        MACOSX_PACKAGE_LOCATION "Resources"
    )
    target_sources(qrzbuddy PRIVATE "${MACOS_ICON}")
    set_target_properties(qrzbuddy PROPERTIES
        MACOSX_BUNDLE_ICON_FILE "qrzbuddy.icns"
        MACOSX_BUNDLE_GUI_IDENTIFIER "io.github.rruchte.QRZBuddy"
    )
endif()

find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
        Network
        WebEngineWidgets
        Xml
        Location
        QuickWidgets
)

find_package(Poco REQUIRED)
find_package(tabulate REQUIRED)
find_package(LibXslt REQUIRED)

target_link_libraries(qrzbuddy
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
        Qt::Xml
        Qt::Location
        Qt::QuickWidgets
        Qt6::WebEngineWidgets
        libxslt::libxslt
        tabulate::tabulate
        Poco::Poco
)

#
# installation
#
install (TARGETS qrzbuddy
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
        BUNDLE DESTINATION . COMPONENT runtime
)

if (NOT WIN32 AND NOT APPLE)
    # Match the desktop-file ID used by QGuiApplication on Linux.
    install(
            FILES io.github.rruchte.QRZBuddy.desktop
            DESTINATION share/applications
    )
    install(
            FILES icons/Unix/qrzbuddy_icon.png
            DESTINATION share/icons/hicolor/1024x1024/apps
            RENAME io.github.rruchte.QRZBuddy.png
    )

    IF("${CMAKE_INSTALL_PREFIX}" STREQUAL "/opt/qrzbuddy")
        execute_process(COMMAND ln -s /opt/qrzbuddy/bin/qrzbuddy lqrzbuddy)

        install(FILES
                ${CMAKE_BINARY_DIR}/lqrzbuddy DESTINATION /usr/bin/ RENAME qrzbuddy
                #COMPONENT runtime
        )
    endif()
endif (NOT WIN32 AND NOT APPLE)

# Platform deployment (windeployqt/macdeployqt/Linux packaging) is handled
# separately from the compilation step, so the Qt installation can be
# discovered from the CI runner rather than a hard-coded developer path.