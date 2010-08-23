
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAG} ${KDE4_ENABLE_EXCEPTIONS}")

include_directories(${KDE4_INCLUDES})

kde4_add_executable(kalibrate
  kalibrate.cc
  imagelist.cc
  imageviewer.cc
  gui.cc
)

target_link_libraries(kalibrate
  ${KDE4_KDEUI_LIBS} 
  ${KDE4_KIO_LIBS}
  ${KDE4_KTEXTEDITOR_LIBS}
)

install(TARGETS kalibrate  ${INSTALL_TARGETS_DEFAULT_ARGS})


#add_executable(icgenerate
#  icgenerate.cc
#)
#
#target_link_libraries(icgenerate
#  ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY}
#)


# desktop-file
#install(FILES kalibrate.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})

# mime-type
# for KDE 4.2 we require at least version 0.23
#set(SHARED_MIME_INFO_MINIMUM_VERSION "0.23")
#find_package(SharedMimeInfo REQUIRED)
#install(FILES kalibrate.xml DESTINATION ${XDG_MIME_INSTALL_DIR})
#update_xdg_mimetypes(${XDG_MIME_INSTALL_DIR})

#subdirs(icons)
