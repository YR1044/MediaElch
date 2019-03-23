add_subdirectory(small_widgets)

add_library(
  mediaelch_ui OBJECT tv_show/TvShowFilesWidget.cpp media_centers/KodiSync.cpp
)

target_link_libraries(
  mediaelch_ui
  PRIVATE
    Qt5::Widgets
    Qt5::Sql
    Qt5::Network
    Qt5::Core
    Qt5::Multimedia
)
mediaelch_post_target_defaults(mediaelch_ui)