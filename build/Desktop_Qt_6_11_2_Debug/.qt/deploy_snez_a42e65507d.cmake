include("/home/nnn231488/snez/build/Desktop_Qt_6_11_2_Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/snez-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "/home/nnn231488/snez/build/Desktop_Qt_6_11_2_Debug/snez"
    GENERATE_QT_CONF
)
