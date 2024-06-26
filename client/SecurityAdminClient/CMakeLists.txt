target_sources("${PROJECT_NAME}" PRIVATE
               securityadminwidget.cpp
               userentitywidget.cpp
               securityadminwidget.ui
               userentitywidget.ui
               resource.qrc
               )

target_link_libraries("${PROJECT_NAME}"
                      Qt5::Core
                      Qt5::Widgets
                      Qt5::Network
                      )
