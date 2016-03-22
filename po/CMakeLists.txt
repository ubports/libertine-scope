intltool_update_potfile(
  ALL
  KEYWORDS "_" "_:1,2" "N_" "N_:1,2"
  POTFILES_TEMPLATE POTFILES.in.in
  GETTEXT_PACKAGE ${GETTEXT_PACKAGE}
)

intltool_install_translations(
  ALL
  GETTEXT_PACKAGE ${GETTEXT_PACKAGE}
)

file(GLOB_RECURSE ALL_POFILES "*.po")

add_custom_target(potfiles
  ALL
  SOURCES
    POTFILES.in.in
    ${GETTEXT_PACKAGE}.pot
    ${ALL_POFILES}
)
