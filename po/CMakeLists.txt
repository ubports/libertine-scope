intltool_update_potfile(
  KEYWORDS "_" "_:1,2" "N_" "N_:1,2"
  POTFILES_TEMPLATE POTFILES.in.in
  COPYRIGHT_HOLDER "Canonical Ltd."
  GETTEXT_PACKAGE ${GETTEXT_PACKAGE}
)

if (CLICK_MODE)
  intltool_install_translations(
    ALL
    GETTEXT_PACKAGE ${GETTEXT_PACKAGE}
  )
else()
  intltool_install_translations(
    GETTEXT_PACKAGE ${GETTEXT_PACKAGE}
  )
endif(CLICK_MODE)

file(GLOB_RECURSE ALL_POFILES "*.po")

add_custom_target(potfiles
  SOURCES
    POTFILES.in.in
    ${GETTEXT_PACKAGE}.pot
    ${ALL_POFILES})
