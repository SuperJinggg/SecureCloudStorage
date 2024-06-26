TARGET_SOURCES("${PROJECT_NAME}" PRIVATE
               userregisterwidget.cpp
               )

target_link_libraries("${PROJECT_NAME}"
                      Qt5::Core
                      Qt5::Gui
                      Qt5::Network
                      Qt5::Widgets
                      )