qt_add_library(CustomControls STATIC)

target_link_libraries(CustomControls PRIVATE Qt6::Quick)

qt_add_qml_module(CustomControls
    URI "CustomControls"
    OUTPUT_DIRECTORY CustomControls
    QML_FILES WaitingAnimationItem.qml
    QML_FILES DeviceButton.qml
    QML_FILES SwitchButton.qml
    QML_FILES OperationButton.qml
    QML_FILES CusTableView.qml
    QML_FILES CusButton.qml
)


