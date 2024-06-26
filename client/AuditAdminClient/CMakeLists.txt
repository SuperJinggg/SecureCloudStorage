target_sources("${PROJECT_NAME}" PRIVATE
               auditentity.cpp
               auditadminwidget.cpp
               auditadminwidget.ui
               resource.qrc
               )

target_link_libraries("${PROJECT_NAME}"
                      Qt5::Core
                      Qt5::Widgets
                      Qt5::Network
                      )
