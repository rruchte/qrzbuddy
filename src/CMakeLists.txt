set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")

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

install(TARGETS qrzbuddy)


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
    # install a desktop file so js8call appears in the application start
    # menu with an icon
    install (
            FILES qrzbuddy.desktop
            DESTINATION /usr/share/applications
            #COMPONENT runtime
    )
    install (
            FILES icons/Unix/qrzbuddy_icon.png
            DESTINATION /usr/share/pixmapsbuddy
            #COMPONENT runtime
    )

    IF("${CMAKE_INSTALL_PREFIX}" STREQUAL "/opt/qrzbuddy")
        execute_process(COMMAND ln -s /opt/qrzbuddy/bin/qrzbuddy lqrzbuddy)

        install(FILES
                ${CMAKE_BINARY_DIR}/lqrzbuddy DESTINATION /usr/bin/ RENAME qrzbuddy
                #COMPONENT runtime
        )
    endif()
endif (NOT WIN32 AND NOT APPLE)

if (WIN32)
    set(QT_BIN_DIR "C:/Qt/6.8.3/msvc2022_64/bin")

    add_custom_command(TARGET qrzbuddy POST_BUILD
            COMMAND "${QT_BIN_DIR}/windeployqt.exe"
            --release
            --qmldir "${CMAKE_SOURCE_DIR}/src"
            "$<TARGET_FILE:qrzbuddy>"
            COMMENT "Running windeployqt for qrzbuddy"
    )
endif()