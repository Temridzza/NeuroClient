{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  packages = with pkgs; [
    qt6.qtbase
    qt6.qtdeclarative
    qt6.qtquicktimeline
    qt6.qtshadertools
    qt6.qttools
    qt6.qtwayland

    cmake
    ninja
    clang
    gdb
    curl
    nlohmann_json
    gtest
    lldb
  ];

  shellHook = ''
    export QT_QPA_PLATFORM=wayland

    export QML2_IMPORT_PATH=${pkgs.qt6.qtdeclarative}/lib/qt-6/qml
    export QT_PLUGIN_PATH=${pkgs.qt6.qtbase}/lib/qt-6/plugins
  '';
}